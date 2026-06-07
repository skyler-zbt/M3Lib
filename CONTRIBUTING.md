# Contributing to M3Lib / 贡献指南

Thank you for your interest in contributing to M3Lib! This document outlines the conventions and workflows for contributing.

感谢你对 M3Lib 的贡献兴趣！本文档说明了贡献的规范和流程。

## Getting Started / 入门

### Prerequisites / 前置条件

- **GCC 16+** (C++26 with contracts P2900R14 and full modules support)
- **xmake 3.08+** (primary build system)

### Setup / 环境搭建

```bash
git clone https://github.com/skyler-zbt/M3Lib.git
cd M3Lib

# Debug build (fast iteration) / Debug 构建（快速迭代）
xmake f -m debug
xmake build M3

# Run tests / 运行测试
xmake build tests && xmake run test_vec
```

## Build System / 构建系统

M3Lib supports two build systems:
M3Lib 支持双构建系统：

| System | Status     | CI Coverage |
|--------|-----------|-------------|
| xmake  | Primary / 主构建 | Full / 全量    |
| mcpp   | Community / 社区 | Planned / 计划中 |

Always verify changes build and pass tests with **xmake** before submitting.
提交前务必用 **xmake** 验证变更能构建并通过测试。

## Commit Message Format / 提交消息格式

Commit messages use a bilingual format with a conventional-commit-style prefix. Each block (English / Chinese) is complete, not line-by-line interleaved.

提交消息使用双语格式，带约定式提交前缀。每个语言块是完整段落，不逐行交错。

```
<prefix>: <short summary / 简短概述>

<English description — one or more paragraphs>
<中文描述 —— 一个或多个段落>
```

Allowed prefixes / 允许的前缀: `feat:`, `fix:`, `perf:`, `refactor:`, `docs:`, `style:`, `test:`, `chore:`, `ci:`.

**Example / 示例:**
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

## Code Style / 代码风格

- **No C macros**, no `#include`, no `try-catch` / exceptions
- Use `import std;` and C++ modules exclusively
- Error handling via `std::expected` / `std::optional`, not exceptions
- Bilingual code comments: complete English paragraph, then complete Chinese paragraph
- Formatting enforced via `.clang-format` (mcpp-community style)

## Pull Request Process / PR 流程

1. **Discuss first:** Open an issue for non-trivial changes before coding. Feature requests and bug reports have dedicated issue templates.
2. **Branch:** Create a feature branch from `dev` (`feat/my-feature` or `fix/my-fix`).
3. **Test:** All tests must pass (`xmake build tests`). Add tests for new functionality.
4. **Style:** Code must pass `clang-format` lint (matching `.clang-format` in the repo root).
5. **PR:** Open against the `dev` branch. The `master` branch tracks stable releases.
6. **Review:** All PRs require at least one reviewer approval. Expect feedback within one week.

## Branch Model / 分支模型

```
master — stable releases / 稳定发布 (tagged)
dev    — integration branch for the next release / 下一版本的集成分支
feat/* — feature branches / 功能分支
fix/*  — bug-fix branches / 修复分支
```

- Merge `feat/*` / `fix/*` → `dev`
- Merge `dev` → `master` on release (with a version tag)

## Testing / 测试

```bash
# Run all tests / 运行全部测试
xmake build tests

# Run a specific test / 运行单个测试
xmake run test_vec
xmake run test_math
xmake run test_boundary
xmake run test_contracts
xmake run test_cxx26
```

```bash
# Run all tests / 运行全部测试
xmake build tests

# Run a specific test / 运行单个测试
xmake run test_vec
xmake run test_math
xmake run test_boundary
xmake run test_cxx26
xmake run test_contracts
```

Test files live in `tests/`. The shared test infrastructure (`test_common` module) provides `check()`, `check_float_eq()`, and `TestRunner`.

**Note:** `test_boundary` uses enforce mode with fork-based signal checking.
`test_contracts` uses observe mode. One test in `test_contracts` is currently
skipped due to a GCC 16.1.0 internal compiler error.

**注意：** `test_boundary` 使用 enforce 模式并通过 fork 进行信号检查。
`test_contracts` 使用 observe 模式。test_contracts 中 1 个测试因
GCC 16.1.0 ICE 跳过。

## Issue Labels / Issue 标签

| Label              | Meaning / 含义                              |
|--------------------|---------------------------------------------|
| `bug`              | Confirmed bug / 已确认的缺陷                  |
| `enhancement`      | Feature request / 功能请求                    |
| `good first issue` | Suitable for new contributors / 适合新贡献者   |
| `help wanted`      | Extra attention needed / 需要额外关注          |
| `P0`–`P3`          | Priority / 优先级 (P0 = critical / 紧急)      |
| `v0.2.0` etc.     | Milestone target / 里程碑目标                  |

## Licence / 许可证

By contributing, you agree that your contributions will be licensed under the Apache License 2.0.
贡献即表示你同意将贡献内容以 Apache License 2.0 许可发布。
