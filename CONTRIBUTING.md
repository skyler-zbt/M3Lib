# Contributing to M3Lib

Thank you for your interest in contributing to M3Lib!
This document outlines the conventions and workflows for contributing.

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
xmake build tests
```

## Build System

M3Lib supports two build systems:

| System | Status    | CI Coverage |
|--------|-----------|-------------|
| xmake  | Primary   | Full        |
| mcpp   | Community | Planned     |

Always verify changes build and pass tests with **xmake** before submitting.

## Commit Message Format

Commit messages use a bilingual format with a conventional-commit-style
prefix.  Each language block is a complete section, not interleaved.

```
<prefix>: <short summary>

<English description — one or more complete paragraphs>

<Chinese description — one or more complete paragraphs>
```

Allowed prefixes: `feat:`, `fix:`, `perf:`, `refactor:`, `docs:`, `style:`,
`test:`, `chore:`, `ci:`.

Example:

```
feat: add element_ref_t trait to decouple apply_* from Vec semantics

Added element_ref_t<V> trait alias in m3.detail.concepts.
For Vec<L,T> this resolves to T.
For future Mat<M,N,T> this will resolve to Vec<N,T>&.
This trait decouples apply_binary/apply_unary from the assumption that
operator[] always returns a scalar reference.

在 m3.detail.concepts 中添加 element_ref_t<V> trait 别名。
对 Vec<L,T> 解析为 T。
对未来的 Mat<M,N,T> 解析为 Vec<N,T>&。
此 trait 将 apply_binary/apply_unary 与 "operator[] 始终返回标量引用"
的假设解耦。
```

## Code Style {#code-style}

- No C macros, no `#include`, no try-catch / exceptions.
- Use `import std;` and C++ modules exclusively.
- Error handling via `std::expected` / `std::optional`, not exceptions.
- Bilingual code comments: complete English paragraph, blank line, then
  complete Chinese paragraph.  Not line-by-line interleaving.
- Formatting enforced via `.clang-format` (mcpp-community style).

## Pull Request Process

1. **Discuss first:** Open an issue for non-trivial changes before coding.
   Feature requests and bug reports have dedicated issue templates.
2. **Fork & branch:** Fork the repo and create a feature branch
   (`feat/my-feature` or `fix/my-fix`).
3. **Test:** All tests must pass (`xmake build tests`).  Add tests for new
   functionality.
4. **Style:** Code must pass `clang-format` lint (matching `.clang-format`
   in the repo root).
5. **PR:** Open against the `master` branch.
6. **Review:** PRs will be reviewed as time permits.  Expect feedback
   within one week for active contributions.

## Branch Model

```
master — primary development and stable releases (tagged)
feat/* — feature branches
fix/*  — bug-fix branches
```

M3Lib uses a single-branch model: `master` is both the development and
release branch.  External contributors fork the repo and open PRs against
`master`.  There is no separate integration branch — keeping things simple
for a solo-maintainer project.

## Testing

```bash
# Run all tests
xmake build tests

# Run a specific test
xmake run test_vec
xmake run test_math
xmake run test_cxx26
xmake run test_contracts
```

Test files live in `tests/`.  The shared test infrastructure (`test_common`
module) provides `check()`, `check_float_eq()`, and `TestRunner`.

- **Note:** `test_contracts` runs in observe mode — contract violations
are logged but do not terminate the program, so the test driver can verify
that the contract fired.  One test in `test_contracts` is currently skipped
due to a GCC 16.1.0 internal compiler error.

> ℹ️ Project changelog is **not maintained** during early development
> (API and modules are unstable).  Changelog will be reintroduced when
> the project approaches v1.0.0.

> ℹ️ 项目早期开发阶段**不维护变更日志**（接口和模块尚不稳定）。
> 待项目接近 v1.0.0 时再恢复变更日志。

## Issue Labels

| Label              | Meaning                                |
|--------------------|----------------------------------------|
| `bug`              | Confirmed bug                          |
| `enhancement`      | Feature request                        |
| `good first issue` | Suitable for new contributors          |
| `help wanted`      | Extra attention needed                 |
| `P0`–`P3`          | Priority (P0 = critical)               |
| `v0.2` etc.         | Milestone target                       |

## Licence

By contributing, you agree that your contributions will be licensed under
the Apache License 2.0.

---

# 贡献指南

感谢你对 M3Lib 的贡献兴趣！本文档说明了贡献的规范和流程。

## 入门

### 前置条件

- **GCC 16+**（C++26，需要 contracts P2900R14 和完整的 modules 支持）
- **xmake 3.08+**（主构建系统）

### 环境搭建

```bash
git clone https://github.com/skyler-zbt/M3Lib.git
cd M3Lib

# Debug 构建（快速迭代）
xmake f -m debug
xmake build M3

# 运行测试
xmake build tests
```

## 构建系统

M3Lib 支持双构建系统：

| 系统  | 状态     | CI 覆盖   |
|-------|----------|-----------|
| xmake | 主构建   | 全量      |
| mcpp  | 社区维护 | 计划中    |

提交前务必用 **xmake** 验证变更能构建并通过测试。

## 提交消息格式

提交消息使用双语格式，带约定式提交前缀。每个语言块是完整段落，不逐行交错。

允许的前缀：`feat:`、`fix:`、`perf:`、`refactor:`、`docs:`、`style:`、
`test:`、`chore:`、`ci:`。

## 代码风格 {#code-style}

- 不使用 C 宏，不写 `#include`，不用 try-catch / 异常。
- 只用 `import std;` 和 C++ modules。
- 错误处理用 `std::expected` / `std::optional`，不用异常。
- 双语代码注释：先完整英文段落，空一行，再完整中文段落。不逐行交错。
- 格式由 `.clang-format` 强制（mcpp-community 风格）。

## PR 流程

1. **先讨论：** 非微小变更在编码前先开 issue。功能请求和缺陷报告有专用
   模板。
2. **Fork 并分支：** Fork 仓库并创建功能分支（`feat/功能名` 或 `fix/修复名`）。
3. **测试：** 所有测试必须通过（`xmake build tests`）。为新功能添加测试。
4. **格式：** 代码必须通过 `clang-format` lint。
5. **PR：** 向 `master` 分支发起。
6. **审查：** PR 将视时间安排审查。活跃贡献可在一周内期待反馈。

## 分支模型

```
master — 主开发与稳定发布分支（已标签）
feat/* — 功能分支
fix/*  — 修复分支
```

M3Lib 采用单分支模型：`master` 同时作为开发分支和发布分支。外部贡献者
Fork 仓库后向 `master` 发起 PR。不设置独立的集成分支——保持简单，适合
单人维护项目。

## 测试

测试文件位于 `tests/`。共享测试基础设施（`test_common` 模块）提供
`check()`、`check_float_eq()` 和 `TestRunner`。

**注意：** `test_contracts` 使用 observe 模式——契约违反被记录但不终止
程序，以便测试驱动验证契约是否触发。`test_contracts` 中 1 个测试因
GCC 16.1.0 内部编译器错误跳过。

## Issue 标签

| 标签              | 含义                   |
|-------------------|------------------------|
| `bug`             | 已确认的缺陷           |
| `enhancement`     | 功能请求               |
| `good first issue`| 适合新贡献者           |
| `help wanted`     | 需要额外关注           |
| `P0`–`P3`         | 优先级（P0 = 紧急）    |
| `v0.2` 等         | 里程碑目标             |

## 许可证

贡献即表示你同意将贡献内容以 Apache License 2.0 许可发布。
