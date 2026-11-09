#requires -Version 7.0
[CmdletBinding()]
param(
    [string]$Mcpp = 'mcpp',
    [string]$Profile = 'dev',
    [string]$OutputDirectory = 'build/mcpp-validation/guards',
    # This project's Windows UCRT abort probe returns STATUS_STACK_BUFFER_OVERRUN.
    # A different CRT/runner may require an explicit -AbortExitCode (e.g. 3/134).
    [int]$AbortExitCode = $(if ($IsWindows) { -1073740791 } else { 134 }),
    [ValidateRange(1, 3600)][int]$GuardTimeoutSeconds = 30,
    [ValidateRange(1, 7200)][int]$LaunchTimeoutSeconds = 300
)

$ErrorActionPreference = 'Stop'
$guardRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$guardOutput = if ([IO.Path]::IsPathRooted($OutputDirectory)) {
    [IO.Path]::GetFullPath($OutputDirectory)
} else { [IO.Path]::GetFullPath((Join-Path $guardRoot $OutputDirectory)) }
$guardOutput = Join-Path $guardOutput $Profile
New-Item -ItemType Directory -Path $guardOutput -Force | Out-Null
$guardMcpp = (Get-Command $Mcpp -CommandType Application -ErrorAction Stop).Source
if ([IO.Path]::GetExtension($guardMcpp) -in @('.bat', '.cmd')) {
    throw 'Pass the mcpp.exe path; this script starts argv directly without a command shell.'
}

# Launch argv directly and keep stderr separate from machine-readable stdout.
# The environment in mcpp.launch is an overlay, not a complete snapshot.
function Invoke-GuardProcess {
    param(
        [string[]]$Argv,
        [string]$WorkingDirectory,
        $Environment,
        [int]$TimeoutSeconds,
        [string]$LogStem
    )
    $guardStart = [Diagnostics.ProcessStartInfo]::new()
    $guardStart.FileName = $Argv[0]
    $guardStart.WorkingDirectory = $WorkingDirectory
    $guardStart.UseShellExecute = $false
    $guardStart.CreateNoWindow = $true
    $guardStart.RedirectStandardOutput = $true
    $guardStart.RedirectStandardError = $true
    foreach ($guardArgument in @($Argv | Select-Object -Skip 1)) {
        $guardStart.ArgumentList.Add($guardArgument)
    }
    if ($null -ne $Environment) {
        foreach ($guardVariable in $Environment.PSObject.Properties) {
            if ($null -eq $guardVariable.Value) {
                $guardStart.Environment.Remove($guardVariable.Name) | Out-Null
            } else {
                $guardStart.Environment[$guardVariable.Name] = [string]$guardVariable.Value
            }
        }
    }
    $guardProcess = [Diagnostics.Process]::new()
    $guardProcess.StartInfo = $guardStart
    $guardWatch = [Diagnostics.Stopwatch]::StartNew()
    $guardStarted = $false
    $guardTimedOut = $false
    $guardExit = $null
    $guardStdout = ''
    $guardStderr = ''
    $guardError = $null
    try {
        $guardStarted = $guardProcess.Start()
        if (-not $guardStarted) { throw 'Process.Start returned false.' }
        $guardOutTask = $guardProcess.StandardOutput.ReadToEndAsync()
        $guardErrTask = $guardProcess.StandardError.ReadToEndAsync()
        if (-not $guardProcess.WaitForExit($TimeoutSeconds * 1000)) {
            $guardTimedOut = $true
            $guardProcess.Kill($true)
        }
        $guardProcess.WaitForExit()
        $guardStdout = $guardOutTask.GetAwaiter().GetResult()
        $guardStderr = $guardErrTask.GetAwaiter().GetResult()
        $guardExit = $guardProcess.ExitCode
    } catch {
        $guardError = $_.Exception.Message
        if ($guardStarted -and -not $guardProcess.HasExited) {
            $guardProcess.Kill($true)
            $guardProcess.WaitForExit()
        }
    } finally {
        $guardWatch.Stop()
        $guardProcess.Dispose()
    }
    [IO.File]::WriteAllText((Join-Path $guardOutput "$LogStem.stdout.log"), $guardStdout)
    [IO.File]::WriteAllText((Join-Path $guardOutput "$LogStem.stderr.log"), $guardStderr)
    [pscustomobject]@{
        argv = $Argv
        cwd = $WorkingDirectory
        started = $guardStarted
        timedOut = $guardTimedOut
        exitCode = $guardExit
        elapsedMs = $guardWatch.ElapsedMilliseconds
        error = $guardError
        stdout = $guardStdout
        stderr = $guardStderr
    }
}

function Read-GuardDocument {
    param($Result, [string]$Kind)
    if (-not $Result.started -or $Result.timedOut -or $null -ne $Result.error -or $Result.exitCode -ne 0) {
        throw "mcpp failed while requesting $Kind; inspect logs in $guardOutput"
    }
    try { $guardDocument = $Result.stdout | ConvertFrom-Json -Depth 100 }
    catch { throw "mcpp did not return one JSON $Kind document; inspect logs in $guardOutput" }
    if ($guardDocument.schemaVersion -ne 1 -or $guardDocument.kind -ne $Kind) {
        throw "Unsupported mcpp protocol document (expected schemaVersion 1 and $Kind)."
    }
    if ($Kind -eq 'mcpp.launch' -and $guardDocument.kindVersion -ne 1) {
        throw 'Unsupported mcpp.launch kindVersion.'
    }
    return $guardDocument
}

$guardProtocolResult = Invoke-GuardProcess -Argv @($guardMcpp, '--protocol-version') `
    -WorkingDirectory $guardRoot -TimeoutSeconds $LaunchTimeoutSeconds -LogStem 'protocol'
$guardProtocol = Read-GuardDocument -Result $guardProtocolResult -Kind 'mcpp.protocol'
if ('test-launch' -notin $guardProtocol.features -or $guardProtocol.kinds.'mcpp.launch' -ne 1) {
    throw 'mcpp must advertise test-launch and mcpp.launch version 1 (2026.10.31.2 or later).'
}

$guardLaunches = @{}
foreach ($guardTest in @('test_arithmetic', 'test_division_output', 'test_inverse')) {
    # --print-launch may build. Run the normal mcpp test command first when a
    # separate build/test validation log is required.
    $guardLaunchResult = Invoke-GuardProcess `
        -Argv @($guardMcpp, 'test', $guardTest, '--profile', $Profile, '--strict', '--print-launch') `
        -WorkingDirectory $guardRoot -TimeoutSeconds $LaunchTimeoutSeconds -LogStem "$guardTest-launch"
    $guardLaunchDocument = Read-GuardDocument -Result $guardLaunchResult -Kind 'mcpp.launch'
    $guardLaunch = $guardLaunchDocument.data
    if ($guardLaunch.test -ne $guardTest -or @($guardLaunch.argv).Count -ne 1 -or $null -ne $guardLaunch.via) {
        throw "Expected a native launch of exactly $guardTest, without a runner or prefilled arguments."
    }
    if (-not (Test-Path -LiteralPath $guardLaunch.argv[0] -PathType Leaf)) {
        throw "Test executable is missing: $($guardLaunch.argv[0])"
    }
    $guardLaunches[$guardTest] = $guardLaunch
}

$guardCases = [Collections.Generic.List[object]]::new()
foreach ($guardName in @('matrix-divide-zero', 'matrix-scalar-divide-zero', 'matrix-negate-min')) {
    $guardCases.Add([pscustomobject]@{
        name = "arithmetic-$guardName"; test = 'test_arithmetic'; arguments = @("--$guardName")
    })
}
foreach ($guardName in @('singular', 'zero', 'nan', 'infinity', 'negative-infinity', 'overflow')) {
    $guardCases.Add([pscustomobject]@{
        name = "inverse-invalid-$guardName"; test = 'test_inverse'; arguments = @('--invalid', $guardName)
    })
}
foreach ($guardName in @('vector-bounds', 'matrix-column-bounds', 'matrix-row-bounds', 'vector-negate-min')) {
    $guardCases.Add([pscustomobject]@{
        name = "division-policy-$guardName"; test = 'test_division_output'; arguments = @('--policy', $guardName)
    })
}
for ($guardDimension = 1; $guardDimension -le 4; ++$guardDimension) {
    for ($guardLane = 0; $guardLane -lt $guardDimension; ++$guardLane) {
        $guardKind = if ($guardLane % 2 -eq 0) { 'vv' } else { 'vv-assign' }
        $guardCases.Add([pscustomobject]@{
            name = "division-zero-$guardDimension-$guardLane"; test = 'test_division_output'
            arguments = @('--zero', $guardKind, "$guardDimension", "$guardLane", "$($guardLane % 2)")
        })
    }
}
foreach ($guardFixture in @(
    @('vs', '3', '0', '1'), @('vs-assign', '4', '0', '0'),
    @('mm', '3', '8', '1'), @('ms-assign', '2', '0', '0'), @('into', '3', '2', '1')
)) {
    $guardCases.Add([pscustomobject]@{
        name = "division-zero-$($guardFixture[0])"; test = 'test_division_output'
        arguments = @('--zero') + $guardFixture
    })
}
if ($guardCases.Count -ne 28) { throw "Expected 28 guard cases, found $($guardCases.Count)." }

$guardResults = [Collections.Generic.List[object]]::new()
$guardControls = [Collections.Generic.List[object]]::new()
function Invoke-GuardCase {
    param([string]$Name, [string]$Test, [string[]]$Arguments, [int]$ExpectedExitCode)
    $guardLaunch = $guardLaunches[$Test]
    $guardResult = Invoke-GuardProcess -Argv (@($guardLaunch.argv) + $Arguments) `
        -WorkingDirectory $guardLaunch.cwd -Environment $guardLaunch.env `
        -TimeoutSeconds $GuardTimeoutSeconds -LogStem $Name
    $guardPassed = $guardResult.started -and -not $guardResult.timedOut -and `
        $null -eq $guardResult.error -and $guardResult.exitCode -eq $ExpectedExitCode
    [pscustomobject]@{
        name = $Name
        test = $Test
        argv = $guardResult.argv
        cwd = $guardResult.cwd
        expectedExitCode = $ExpectedExitCode
        exitCode = $guardResult.exitCode
        started = $guardResult.started
        timedOut = $guardResult.timedOut
        elapsedMs = $guardResult.elapsedMs
        error = $guardResult.error
        passed = $guardPassed
    }
}

foreach ($guardTest in @('test_arithmetic', 'test_division_output', 'test_inverse')) {
    # Each fixture rejects unknown arguments with status 2. This establishes
    # successful process startup and argument routing before checking abort.
    $guardControl = Invoke-GuardCase -Name "$guardTest-invalid-argument-control" `
        -Test $guardTest -Arguments @('--mcpp-guard-invalid-argument') -ExpectedExitCode 2
    $guardControls.Add($guardControl)
}
if (@($guardControls | Where-Object { -not $_.passed }).Count -eq 0) {
    foreach ($guardCase in $guardCases) {
        $guardResult = Invoke-GuardCase -Name $guardCase.name -Test $guardCase.test `
            -Arguments $guardCase.arguments -ExpectedExitCode $AbortExitCode
        $guardResults.Add($guardResult)
        Write-Output "$(if ($guardResult.passed) { 'PASS' } else { 'FAIL' }) $($guardResult.name): exit $($guardResult.exitCode), expected $AbortExitCode"
    }
}
$guardSummary = [pscustomobject]@{
    profile = $Profile
    mcpp = $guardMcpp
    mcppVersion = $guardProtocol.mcpp.version
    expectedAbortExitCode = $AbortExitCode
    expectedGuardCount = 28
    runGuardCount = $guardResults.Count
    passedGuardCount = @($guardResults | Where-Object passed).Count
    failedGuardCount = @($guardResults | Where-Object { -not $_.passed }).Count
    controls = $guardControls.ToArray()
    cases = $guardResults.ToArray()
}
$guardSummary | ConvertTo-Json -Depth 12 | Set-Content -LiteralPath (Join-Path $guardOutput 'summary.json') -Encoding utf8
if (@($guardControls | Where-Object { -not $_.passed }).Count -ne 0 -or `
    $guardSummary.runGuardCount -ne 28 -or $guardSummary.failedGuardCount -ne 0) {
    throw "Guard verification failed; inspect $guardOutput\summary.json"
}
Write-Output "Guard verification: 28 passed; 0 failed ($Profile). Logs: $guardOutput"
