# Test development history

See the [testing guide](README.md) for how to run the tests, their coverage, and the comparison rules. See the [project development history](../docs/DEV_HISTORY.md) for project-wide changes.

## Key changes

### 2026-06-01 · Split tests by module

Split the original vector test file into cases for vectors, mathematical functions, and C++ language features.

### 2026-06-06 to 2026-07-07 · Early termination tests

Added edge-case tests using `fork` and `waitpid` to check out-of-bounds access and division by zero in child processes. Later removed these tests because they depended on POSIX process APIs. Dedicated Contracts cases and CI builds then covered contract verification.

### 2026-10-10 · Remove old tests and introduce GLM

Removed the old test sources and disabled their targets. Added a script to fetch the GLM reference library into `tests/third_party/glm`. See the [current GLM setup script](clone_glm.py).

### 2026-10-31 · Validate matrix operations and inversion

The README and testing guide from that date reported the following results. The original report and logs are unavailable in the current checkout. These figures describe that run.

- xmake reported zero failures across 320,957 numerical checks in each of Debug and Release. All 35 xmake tests behaved as expected.
- mcpp 2026.10.31.3 with GCC 16.2.0 on Windows passed seven numerical cases in each of `dev` and `release`. An external script also verified 28 termination cases in each configuration.
- The local run completed 71 performance comparisons against GLM. The initial `inverse` implementation took substantially longer than GLM on that workload.

### 2026-11-09 · Rebuild numerical comparisons and regression tests

- Organized cases around arithmetic, matrices, inversion, geometry, common math, transcendental functions, and division with output parameters. GLM's matrix `/` multiplies by an inverse, so the component-wise division reference is assembled one column at a time using GLM vector division. Inversion tests compare against GLM and check the residuals when multiplying by the inverse on either side. See the [matrix tests](test_matrix.cpp), the [inversion tests](test_inverse.cpp), and the [matrix interface history](../m3/matrix/DEV_HISTORY.md#2026-11-09--separate-linear-algebra-and-component-wise-operations).
- Restored termination checks in separate processes. xmake registers these cases as expected failures. With mcpp, a helper script launches the test executable, first using invalid arguments to verify startup and argument forwarding. It then checks the exit code expected from each termination case. See the [xmake test configuration](xmake.lua) and the [mcpp termination check script](run_mcpp_guards.ps1).
- Separated the GLM benchmarks from correctness tests to keep concurrent test execution from affecting timings. xmake provides a separate Release target named `benchmark_glm`. mcpp enables that target through the `benchmark` feature. See the [comparison script](run_glm_comparison.ps1) and the [mcpp configuration](../mcpp.toml).

### 2026-11-09 · Check Windows Clang compatibility

- Used Clang 23.1.4 with the `x86_64-w64-windows-gnu` target and libstdc++ from MSYS2 UCRT64 GCC 16.2.0. Built the standard library and M3Lib modules manually, archived the static library, then linked and ran the README example. The example exited with code 0.
- In the same configuration, the arithmetic, division and output, inversion, matrix, and transcendental tests passed. The common math and geometry tests failed to compile because the required calls to functions such as `floor`, `fmod`, `sqrt`, and `hypot` could not be evaluated at compile time. See the [common math tests](test_common_math.cpp) and the [geometry tests](test_geometry.cpp).
- With xmake and `clang-cl` targeting the MSVC ABI, the static library and example source compiled. The first link inherited `stdc++exp` from the library target's public dependencies, and the MSVC linker reported `LNK1104` because it could not find `stdc++exp.lib`.
- Limited public linking of `stdc++exp` to GCC. Built and ran the README example with `clang-cl` 23.1.4 and UCRT64 GCC 16.2.0. Both runs exited with code 0, and the link command confirmed that GCC still received `-lstdc++exp`. See the [library target configuration](../m3/xmake.lua).
- A later build with MSYS2 MINGW64 Clang 23.1.5 and `-stdlib=libc++` produced the static library and ran the README example successfully. The test executable linked against `libc++.dll`. Four of seven numerical tests passed. The common math, geometry, and inversion tests failed to compile because the required `<cmath>` calls could not be evaluated at compile time.
