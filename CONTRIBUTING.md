# Contributing to M3Lib

Thank you for your interest in contributing to M3Lib! This document outlines the conventions and workflows for contributing.

## Getting Started

### Prerequisites

- **GCC 16+** (C++26 with contracts P2900R14 and full modules support)
- **xmake 3.08+** (primary build system)

### Setup

```bash
git clone https://github.com/skyler-zbt/M3Lib.git
cd M3Lib

# Debug build (fast iteration)
xmake f -m debug
xmake build M3

# Run tests
xmake build tests && xmake run test_vec
```

## Build System

M3Lib supports two build systems:

| System  | Status       | CI Coverage |
|---------|--------------|-------------|
| xmake   | Primary      | Full        |
| mcpp    | Community    | Planned     |

Always verify changes build and pass tests with **xmake** before submitting.

## Commit Message Format

Commit messages use a bilingual format with a conventional-commit-style prefix. Each block (English / Chinese) is complete, not line-by-line interleaved.

```
<prefix>: <short summary>

<English description — one or more paragraphs>

<Chinese description — one or more paragraphs>
```

Allowed prefixes: `feat:`, `fix:`, `perf:`, `refactor:`, `docs:`, `style:`, `test:`, `chore:`, `ci:`.

**Example:**
```
feat: add element_ref_t trait to decouple apply_* from Vec semantics

Added element_ref_t<V> trait alias in m3.detail.concepts.
For Vec<L,T> this resolves to T& (scalar reference).
For future Mat<M,N,T> this will resolve to Vec<N,T>& (column reference).
This trait decouples apply_binary/apply_unary from the assumption that
operator[] always returns a scalar reference.

在 m3.detail.concepts 中添加 element_ref_t<V> trait 别名。
对 Vec<L,T> 解析为 T&（标量引用）。
对未来的 Mat<M,N,T> 解析为 Vec<N,T>&（列引用）。
此 trait 将 apply_binary/apply_unary 与 "operator[] 始终返回标量引用"
的假设解耦。
```

## Code Style

- **No C macros**, no `#include`, no `try-catch` / exceptions
- Use `import std;` and C++ modules exclusively
- Error handling via `std::expected` / `std::optional`, not exceptions
- Bilingual code comments: complete English paragraph, then complete Chinese paragraph
- Formatting enforced via `.clang-format` (mcpp-community style)

## Pull Request Process

1. **Discuss first:** Open an issue for non-trivial changes before coding. Feature requests and bug reports have dedicated issue templates.
2. **Branch:** Create a feature branch from `dev` (`feat/my-feature` or `fix/my-fix`).
3. **Test:** All tests must pass (`xmake build tests`). Add tests for new functionality.
4. **Style:** Code must pass `clang-format` lint (matching `.clang-format` in the repo root).
5. **PR:** Open against the `dev` branch. The `master` branch tracks stable releases.
6. **Review:** All PRs require at least one reviewer approval. Expect feedback within one week.

## Branch Model

```
master — stable releases (tagged)
dev    — integration branch for the next release
feat/* — feature branches
fix/*  — bug-fix branches
```

- Merge `feat/*` / `fix/*` → `dev`
- Merge `dev` → `master` on release (with a version tag)

## Testing

```bash
# Run all tests
xmake build tests

# Run a specific test
xmake run test_vec
xmake run test_math
xmake run test_boundary
xmake run test_contracts
xmake run test_cxx26
```

Test files live in `tests/`. The shared test infrastructure (`test_common` module) provides `check()`, `check_float_eq()`, and `TestRunner`.

## Issue Labels

| Label        | Meaning                                     |
|--------------|---------------------------------------------|
| `bug`        | Confirmed bug                               |
| `enhancement`| Feature request                             |
| `good first issue` | Suitable for new contributors          |
| `help wanted`| Extra attention / community contribution needed |
| `P0`–`P3`   | Priority (P0 = critical / blocking)         |
| `v0.2.0` etc.| Milestone target                            |

## Licence

By contributing, you agree that your contributions will be licensed under the Apache License 2.0.
