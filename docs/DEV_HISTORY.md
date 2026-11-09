# M3Lib development history

This record covers project-wide changes to structure, builds, and validation. The individual histories describe API, implementation, and testing decisions in more detail.

## History index

| Area | Topics |
| --- | --- |
| [Vector](../m3/vector/DEV_HISTORY.md) | Vector types, storage layout, indexing, and direct output |
| [Matrix](../m3/matrix/DEV_HISTORY.md) | Matrix storage, indexing, products, and component-wise operations |
| [Math](../m3/math/DEV_HISTORY.md) | Geometry, common math, transforms, and matrix inversion |
| [Detail](../m3/detail/DEV_HISTORY.md) | Shared type constraints, alignment, and arithmetic helpers |
| [Tests](../tests/DEV_HISTORY.md) | GLM comparisons, termination cases, regression tests, and benchmarks |

## Naming conventions

- Use `PascalCase` for types and `snake_case` for functions, parameters, and local variables.
- Prefix non-static data members with `m_` and use `snake_case`, as in `m_storage`, `m_columns`, and `m_data`.
- Public static metadata keeps its established names, including `dimension`, `columns`, and `rows`.

## Project changes

### 2026-05-27 · Add a single module entry point

Added `m3/m3.cppm` as the entry point for `import m3;` and added an optional mcpp build configuration. See the [module entry point](../m3/m3.cppm) and [mcpp configuration](../mcpp.toml).

### 2026-06-19 · Export matrices from the main module

Added square matrix types and exported them from `m3`. Vectors and matrices own their storage implementations, with shared alignment rules and type constraints in `detail`. See the [matrix history](../m3/matrix/DEV_HISTORY.md#2026-06-19--introduce-square-matrices) and [storage changes](../m3/detail/DEV_HISTORY.md#2026-06-19--move-storage-to-its-owning-types).

### 2026-07-07 · Split the xmake build configuration

Separated project setup in the root configuration from the library target in `m3/` so downstream projects could include `m3/` on its own. See the [project configuration](../xmake.lua) and [library target](../m3/xmake.lua).

### 2026-08-12 · Remove the old CI workflows

Removed the GitHub Actions fast-lane and lint workflows pending the redesign of the test suite and CI setup.

### 2026-10-08 · Separate repository defaults from library requirements

Moved the repository's default architecture and platform toolchain settings into the root configuration. The `M3Lib` target carries the language, module, and link requirements, allowing downstream projects to select their own architecture and toolchain. See the [project configuration](../xmake.lua) and [library target](../m3/xmake.lua).

### 2026-10-10 · Start reorganizing the test suite

Removed the old test sources, disabled their targets, and added a script to fetch a local GLM checkout. See the [test history](../tests/DEV_HISTORY.md#2026-10-10--remove-old-tests-and-introduce-glm).

### 2026-11-09 · Update APIs, builds, and validation

- Updated the matrix API and vector output paths, and added floating-point square matrix inversion. See [matrix operations](../m3/matrix/DEV_HISTORY.md#2026-11-09--separate-linear-algebra-and-component-wise-operations), [direct vector output](../m3/vector/DEV_HISTORY.md#2026-11-09--direct-output-and-compound-assignment), and [matrix inversion](../m3/math/DEV_HISTORY.md#2026-11-09--floating-point-square-matrix-inversion).
- Registered the regression tests again in xmake and switched mcpp to discovering `tests/test_*.cpp`. The GLM benchmark runs as a separate target. See the [test suite rebuild](../tests/DEV_HISTORY.md#2026-11-09--rebuild-numerical-comparisons-and-regression-tests) and [mcpp configuration](../mcpp.toml).

### 2026-11-09 · Pause Contracts

Commented out the `pre(...)` annotations on vector and matrix indexing, `Div`, `clamp`, and `smoothstep`, along with the corresponding xmake and mcpp flags. The feature's place in the final standard was uncertain. Explicit checks still handle out-of-bounds indexing, zero divisors in `Div`, and reversed bounds in vector `clamp`. Reassess the decision once the final standard settles the inclusion and specification of Contracts. See the [xmake library configuration](../m3/xmake.lua) and [mcpp configuration](../mcpp.toml).

WG21 voted to add Contracts to the C++ working draft in February 2025 through P2900R14. Contracts were included in the C++26 Draft International Standard (DIS) submitted for ballot. In October 2026, the DIS failed the ballot as a whole. German representative Andreas Fertig reported that some national bodies raised concerns about the maturity of Contracts. As of 2026-11-09, the ISO project page recorded voting as closed at stage 40.60. [WG21 minutes N5007](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/n5007.pdf) · [WG21 paper P4433R0](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2026/p4433r0.pdf) · [Fertig's ballot account](https://andreasfertig.com/blog/2026/11/cpp26-dis-failed-how-does-cpp-survive/) · [ISO project status](https://www.iso.org/standard/91179.html)
