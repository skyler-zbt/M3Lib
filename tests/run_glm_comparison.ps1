param(
    [int]$Count = 4096,
    [int]$Samples = 7,
    [double]$MinSampleMs = 12,
    [int]$Cpu = 2,
    [string]$Mingw = '',
    [string]$OutputDirectory = 'build/glm-comparison'
)

$ErrorActionPreference = 'Stop'
$PSNativeCommandUseErrorActionPreference = $false
$comparisonRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
Push-Location -LiteralPath $comparisonRoot
try {
    if (-not (Test-Path -LiteralPath 'tests/third_party/glm/glm/glm.hpp')) {
        throw 'Local GLM is missing. Run tests/clone_glm.py before the comparison.'
    }
    $comparisonOutput = if ([IO.Path]::IsPathRooted($OutputDirectory)) {
        [IO.Path]::GetFullPath($OutputDirectory)
    } else { [IO.Path]::GetFullPath((Join-Path $comparisonRoot $OutputDirectory)) }
    New-Item -ItemType Directory -Path $comparisonOutput -Force | Out-Null

    function Invoke-ComparisonXmake {
        param([string[]]$Arguments, [string]$LogName)
        $comparisonLog = Join-Path $comparisonOutput $LogName
        & xmake @Arguments *> $comparisonLog
        if ($LASTEXITCODE -ne 0) {
            Get-Content -LiteralPath $comparisonLog | Select-Object -Last 45 | Write-Output
            throw "xmake failed; see $comparisonLog"
        }
    }

    foreach ($comparisonMode in @('debug', 'release')) {
        Write-Output "Running GLM reference tests: $comparisonMode"
        $comparisonConfig = @('f', '-m', $comparisonMode)
        if ($Mingw) { $comparisonConfig += '--mingw=' + $Mingw }
        Invoke-ComparisonXmake -Arguments $comparisonConfig -LogName "$comparisonMode-config.log"
        Invoke-ComparisonXmake -Arguments @('test', '-v') -LogName "$comparisonMode-tests.log"
        Get-Content -LiteralPath (Join-Path $comparisonOutput "$comparisonMode-tests.log") |
            Select-String -Pattern 'checks,|tests passed' | ForEach-Object { $_.Line }
    }

    Write-Output 'Building the Release performance comparison'
    Invoke-ComparisonXmake -Arguments @('build', '-v', 'benchmark_glm') -LogName 'benchmark-build.log'
    $comparisonTargetJson = (& xmake show -t benchmark_glm --format=json) -join "`n"
    if ($LASTEXITCODE -ne 0) { throw 'Cannot resolve benchmark_glm executable' }
    $comparisonTarget = ($comparisonTargetJson | ConvertFrom-Json).targetfile
    $comparisonExecutable = if ([IO.Path]::IsPathRooted($comparisonTarget)) {
        [IO.Path]::GetFullPath($comparisonTarget)
    } else { [IO.Path]::GetFullPath((Join-Path $comparisonRoot $comparisonTarget)) }
    if (-not (Test-Path -LiteralPath $comparisonExecutable)) {
        throw "Benchmark executable not found: $comparisonExecutable"
    }
    $comparisonCsv = Join-Path $comparisonOutput 'results.csv'
    $comparisonRaw = Join-Path $comparisonOutput 'samples.csv'
    $comparisonEnvironment = Join-Path $comparisonOutput 'benchmark-environment.txt'
    $comparisonMinMs = $MinSampleMs.ToString([Globalization.CultureInfo]::InvariantCulture)
    Write-Output 'Running paired, pinned performance samples (M3 and GLM alternate order)'
    & $comparisonExecutable --count $Count --samples $Samples `
        --min-ms $comparisonMinMs --cpu $Cpu `
        --raw-output $comparisonRaw > $comparisonCsv 2> $comparisonEnvironment
    if ($LASTEXITCODE -ne 0) {
        Get-Content -LiteralPath $comparisonEnvironment | Write-Output
        throw 'The benchmark failed verification or execution; no successful comparison is reported.'
    }

    $comparisonRows = @(Import-Csv -LiteralPath $comparisonCsv)
    if ($comparisonRows.Count -eq 0) { throw 'No performance rows were produced' }
    $comparisonCommands = @()
    if (Test-Path -LiteralPath 'compile_commands.json') {
        $comparisonCommands = @(Get-Content -Raw -LiteralPath 'compile_commands.json' | ConvertFrom-Json |
            Where-Object { $_.file -match 'benchmark_glm\.cpp$|benchmark_platform\.cpp$' })
    }
    $comparisonCpuName = [Runtime.InteropServices.RuntimeInformation]::ProcessArchitecture.ToString()
    if ($IsWindows) {
        $comparisonCpuName = (Get-ItemProperty -LiteralPath `
            'HKLM:\HARDWARE\DESCRIPTION\System\CentralProcessor\0' -Name ProcessorNameString).ProcessorNameString.Trim()
    } elseif (Test-Path -LiteralPath '/proc/cpuinfo') {
        $comparisonCpuLine = Get-Content -LiteralPath '/proc/cpuinfo' |
            Select-String -Pattern '^model name\s*:' | Select-Object -First 1
        if ($comparisonCpuLine) { $comparisonCpuName = ($comparisonCpuLine.Line -split ':', 2)[1].Trim() }
    }
    $comparisonSourceFiles = @(Get-ChildItem -LiteralPath 'm3' -Recurse -Filter '*.cppm') +
        @(Get-ChildItem -LiteralPath 'tests' -Filter '*.cpp') +
        @(Get-Item -LiteralPath 'tests/support/benchmark_platform.cpp', 'tests/support/glm_reference.hpp',
            'tests/run_glm_comparison.ps1', 'tests/xmake.lua')
    $comparisonSourceHashes = @($comparisonSourceFiles |
        ForEach-Object { [pscustomobject]@{
            path = [IO.Path]::GetRelativePath($comparisonRoot, $_.FullName)
            sha256 = (Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash
        } })
    $comparisonMetadata = [ordered]@{
        timestamp = (Get-Date -Format o)
        cpu = $comparisonCpuName
        os = [Runtime.InteropServices.RuntimeInformation]::OSDescription
        m3_git_head = (& git rev-parse HEAD).Trim()
        m3_worktree_status = @(& git status --short)
        glm_git_head = (& git -C tests/third_party/glm rev-parse HEAD).Trim()
        glm_origin = (& git -C tests/third_party/glm remote get-url origin).Trim()
        glm_worktree_status = @(& git -C tests/third_party/glm status --short)
        benchmark_environment = @(Get-Content -LiteralPath $comparisonEnvironment)
        compile_commands = $comparisonCommands
        source_sha256 = $comparisonSourceHashes
        count = $Count
        samples = $Samples
        minimum_sample_ms = $MinSampleMs
        performance_cases = $comparisonRows.Count
    }
    $comparisonMetadata | ConvertTo-Json -Depth 9 |
        Set-Content -LiteralPath (Join-Path $comparisonOutput 'environment.json') -Encoding utf8

    $comparisonSummary = [Collections.Generic.List[string]]::new()
    $comparisonSummary.Add('# M3 / local GLM comparison')
    $comparisonSummary.Add('')
    $comparisonSummary.Add("CPU: $comparisonCpuName")
    $comparisonSummary.Add('')
    $comparisonSummary.Add('Times are median nanoseconds per object in a batch. M3/GLM > 1 means M3 took longer.')
    $comparisonSummary.Add('')
    $comparisonSummary.Add('| Case | M3 ns | GLM ns | M3/GLM |')
    $comparisonSummary.Add('|---|---:|---:|---:|')
    foreach ($comparisonRow in $comparisonRows) {
        $comparisonSummary.Add(('| {0} | {1:F3} | {2:F3} | {3:F2} |' -f $comparisonRow.case,
            [double]$comparisonRow.m3_median_ns, [double]$comparisonRow.glm_median_ns,
            [double]$comparisonRow.m3_over_glm))
    }
    $comparisonSummary.Add('')
    $comparisonSummary.Add('M3 retains its zero-divisor and other checks. component_divide is compared with GLM column-vector division, not GLM inverse-based matrix division. component_multiply is compared with glm::matrixCompMult.')
    $comparisonSummary.Add('')
    $comparisonSummary.Add('Inverse cases use strictly diagonally dominant matrices and time inverse() itself. The multiply_precomputed_inverse_right/left cases time multiplication only: inverses are generated once with GLM before timing and shared bit-identically with M3. They do not measure an inverse-plus-multiply operation.')
    $comparisonSummary.Add('')
    $comparisonSummary.Add('Packed float defaults are used for both libraries. No forced GLM intrinsics, native-architecture flags, or fast-math are added. Results are local microbenchmarks, not application-level speed measurements.')
    $comparisonSummary | Set-Content -LiteralPath (Join-Path $comparisonOutput 'summary.md') -Encoding utf8
    Write-Output "Completed $($comparisonRows.Count) performance cases. Results: $comparisonOutput"
} finally {
    Pop-Location
}
