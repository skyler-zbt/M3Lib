# GLM reference tests and benchmarks

The tests cover vector and matrix arithmetic, mathematical functions, output-parameter operations, and their edge cases. Numerical comparisons use the GLM checkout at `tests/third_party/glm`. Shared conversion and comparison code lives in `support/glm_reference.hpp`. See the [test development history](DEV_HISTORY.md) for past test and benchmark results.

## Running the tests

From a clean checkout, prepare GLM from the repository root before running the tests. The script fetches GLM with Git and reuses an existing checkout. Pass `--ref` on the initial fetch to select a branch or tag. See the [GLM setup script](clone_glm.py).

```bash
python tests/clone_glm.py
```

### xmake regression tests

`xmake test` builds and runs the numerical checks and expected-termination cases. A normal library build builds only the default targets. See the [test configuration](xmake.lua).

```bash
xmake f -m debug
xmake test -v
xmake f -m release
xmake test -v
```

If xmake cannot find MinGW on Windows, add `--mingw=<path-to-ucrt64>` to both configuration commands. See the [build requirements](../README.md#build).

### mcpp numerical and termination checks

`mcpp test` discovers and runs the numerical cases from the project configuration. A PowerShell 7 script launches the expected-termination cases separately and checks their exit codes. See the [project configuration](../mcpp.toml) and the [termination check script](run_mcpp_guards.ps1).

```powershell
mcpp test --profile dev --strict
mcpp test --profile release --strict
./tests/run_mcpp_guards.ps1 -Profile dev
./tests/run_mcpp_guards.ps1 -Profile release
```

### Benchmarks

Run the comparison script with PowerShell 7 from the repository root. It runs the xmake Debug and Release regression tests, then builds and runs `benchmark_glm` separately in Release mode. On Windows, pass `-Mingw` if you need to specify the MinGW root directory. See the [comparison script](run_glm_comparison.ps1).

```powershell
./tests/run_glm_comparison.ps1
```

Optional parameters include `-Count`, `-Samples`, `-MinSampleMs`, `-Cpu`, and `-OutputDirectory`. By default, a batch contains 4,096 objects, and each benchmark records seven samples. Calibration targets about 12 ms for the faster implementation. The benchmark first tries to pin execution to logical CPU 2. If CPU 2 falls outside the process's allowed set, the benchmark selects and records an allowed CPU. Benchmarking stops if CPU affinity cannot be set.

## Reference results

| Test file | GLM reference |
| --- | --- |
| `test_arithmetic.cpp` | Vector and matrix construction, arithmetic, and compound assignment |
| `test_matrix.cpp` | `A * B`, `B * A`, matrix-vector multiplication, and homogeneous transforms |
| `test_inverse.cpp` | `glm::inverse` for well-conditioned inputs, with separate checks for residuals on both sides, noncommuting multiplication orders, and expected termination |
| `test_common_math.cpp` | `mix`, `clamp`, `abs`, `sign`, `mod`, `smoothstep`, and other functions. M3's `lerp` corresponds to GLM's `mix`. |
| `test_geometry.cpp` | Dot and cross products, norms, distance, normalization, reflection, and refraction |
| `test_transcendental.cpp` | Trigonometric functions, angle conversions, exponential functions, logarithms, and square roots. M3's `atan2(y, x)` corresponds to GLM's `atan(y, x)`. |
| `test_division_output.cpp` | Arithmetic references for division, output parameters, in-place updates, and special floating-point values, with separate checks for aliasing |

Ordinary floating-point inputs are checked against GLM at the same precision. Comparisons use floating-point tolerances. Tests check the sign of zero separately in cases where it matters, including division and `atan2`.

Tests for very large and very small norms and functions evaluated over wide intervals use GLM with `long double` as a wider reference type. This avoids mistaking intermediate overflow in a same-precision GLM calculation for a defect in M3. Very small nonzero lengths use relative error so that a zero result fails the check. Extended integer geometry cases also use wider GLM calculations for reference values. If the platform's `long double` lacks sufficient exponent range or precision, the affected extreme-value cases print `SKIP`.

The local GLM checkout uses double-precision literals for angle conversion constants. Tests compare `long double` angle conversions at the precision GLM can provide, while M3 retains its own precision.

## M3-specific checks

| M3 behavior | Check |
| --- | --- |
| A denominator of positive or negative zero terminates the process | Run separate process checks for vector `/` and `/=`, scalar denominators, `divide_into`, and matrix `component_divide` |
| Matrix `/` and `/=` take a scalar divisor, and matrix inversion requires a floating-point element type | Check compile-time constraints, including the availability of scalar matrix division |
| Matrix inversion terminates when it detects invalid input or an unrepresentable calculation | Run separate process checks for singular matrices, non-finite inputs, and inverse overflow |
| Out-of-bounds access terminates in Release builds as well | Run separate process checks for vector indices, matrix column indices, and matrix row indices |
| Negating the minimum signed value terminates the process | Run separate process checks for vector and matrix negation |
| Normalizing a zero vector returns zero | Compare against an explicitly constructed GLM zero vector to check M3's policy |
| A transformed homogeneous point with `w == 0` returns its `xyz` components | Compare with the `xyz` components of the GLM homogeneous product before the perspective divide |
| Default-constructed vectors and matrices contain zeros | Compare against GLM values explicitly initialized to zero |
| `*_into` returns a reference to the output and allows the output to alias either input exactly | Compare values with GLM and check reference identity and aliasing separately |
| Packed layout by default and restrictions on dimension conversions | Check `sizeof`, construction constraints, and other static properties |

The matrix operators `*` and `*=` are checked against GLM matrix multiplication. `component_multiply` is checked against `glm::matrixCompMult`. GLM's matrix `/` multiplies by an inverse, so the reference for `component_divide` is built by dividing GLM column vectors individually.

Inversion tests use `glm::inverse` at the same precision for well-conditioned inputs. Extreme scales and nearly singular inputs also require checks of reference precision and product residuals. Compile-time cases check constant evaluation and the interface constraints for integer matrices. See the [inversion tests](test_inverse.cpp) and the [math development history](../m3/math/DEV_HISTORY.md#2026-11-09--floating-point-square-matrix-inversion).

The tests compute `invB = inverse(B)`, then compare `A * invB` and `invB * A` with the corresponding GLM results. Noncommuting inputs distinguish the two paths and expose ordering errors that identity or diagonal matrices can hide.

Tests judge results according to M3's defined behavior. Some cases involving integer overflow, invalid bounds for `clamp` or `smoothstep`, and NaN operand ordering remain outside that specification. Where NaN or signed-zero bit patterns are unspecified, as in some `min` and `max` cases, comparisons follow the function's defined semantics.

## Benchmark results

The comparison script writes results to `build/glm-comparison/` by default.

- `debug-tests.log` and `release-tests.log` capture build and test output.
- `results.csv` records the median, minimum, and maximum for both implementations, the M3/GLM time ratio, and the result error for each benchmark.
- `samples.csv` records raw samples and execution order.
- `environment.json` records the CPU, the thread affinity actually used, the GLM version, source hashes, and the actual compiler command lines.
- `summary.md` contains the full benchmark table.

Results are checked component by component against GLM before timing. Only the operations under test are timed. Allocation, input conversion, verification, and checksum calculation take place outside the timed region. Both implementations use the same inputs, iteration counts, and optimization options at the call site. Their execution order alternates.

Benchmarks cover the default packed `float` types for vectors with two to four elements and square matrices of order two to four, plus selected common, trigonometric, exponential, and transform functions. Matrix component-wise multiplication and division use the GLM references described above and retain M3's checks.

The `inverse` benchmark uses the same well-conditioned inputs for both implementations and times the entire inversion. The `multiply_precomputed_inverse_right/left` benchmarks use GLM to generate a shared inverse before timing. Both implementations receive identical `float` bit patterns and time `A * invB` and `invB * A` separately.

Results report batch throughput in ns per object. `M3/GLM > 1` means M3 took longer. Comparisons apply to the recorded CPU, compiler, data size, and workload.
