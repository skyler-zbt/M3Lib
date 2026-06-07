# Changelog

All notable changes to M3Lib will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

# 变更日志

本文档记录 M3Lib 所有重要变更。

格式基于 [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)，
本项目遵循[语义化版本](https://semver.org/lang/zh-CN/)。

---

## [Unreleased]

## [0.1.1] — 2026-06-07

### Fixed

- `operator[]` hot-path: replaced runtime `if (i >= L) std::abort()` with
  `if consteval` + `[[assume]]` to unlock auto-vectorisation (16–20×
  performance gap on element-wise loops).
- Four source-comment inaccuracies:
  1. `vec.cppm`: false Vec<5+> support claim — removed, replaced with
     per-specialisation accessor table.
  2. `apply.cppm`: unreachable generic-loop fallback — corrected to note the
     else-branch exists but is dead for Vec.
  3. `operators.cppm`: "all operators delegate" — corrected to "most;
     operator==/!= use direct unrolling".
  4. `vec.cppm` header: implied all swizzle sets for all dims — replaced with
     per-dimension breakdown.
- `element_ref_t`: added `std::remove_reference_t` so the trait resolves to
  `T` (value type) not `T&`, enabling const Vec support.
- `vector_base.cppm`: const `operator[]` comments made self-contained with
  full rationale, not just a reference to the mutable overload.
- `test_boundary.cpp` / `tests/xmake.lua`: updated comments to reflect the
  `[[assume]]` change — OOB is now caught by contract pre-condition, not
  runtime if-abort guard.
- `CHANGELOG.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `NOTICE`,
  `ROADMAP.md`, `.github/ISSUE_TEMPLATE/`: added complete Chinese
  translations in block-separated format.
- `README.md` / `README.zh-CN.md`: test count corrected to 128 (1 skipped
  due to GCC 16.1.0 ICE in test_contracts).
- `CONTRIBUTING.md`: added concrete test-running commands and mode notes.

### Added

- `element_ref_t<V>` trait in `m3.detail.concepts` — decouples
  `apply_binary` / `apply_unary` from Vec-specific `operator[]` semantics,
  enabling future Matrix reuse.
- `--march` xmake option with whitelist (`generic`, `native`, `x86-64-v3`,
  `x86-64-v4`, `znver3`, `znver4`) — `-march=native` / `-mtune=native` now
  opt-in, defaulting to portable `generic`.
- Project governance baseline: `CHANGELOG.md`, `CONTRIBUTING.md`,
  `CODE_OF_CONDUCT.md`, `NOTICE`, `ROADMAP.md`, GitHub Issue templates
  (`bug_report.yml`, `feature_request.yml`).
- `operator[]` regression tests: all valid indices (Vec<1..4>), const
  access, write-through.
- `element_ref_t` type-resolution tests: `float`, `int`, `double`, and
  `const Vec` variants.

### Changed

- README: honest platform/feature status — "Linux x64 (GCC 16+)" instead of
  "Cross-platform"; mcpp marked "community-maintained, best-effort"; Matrix
  and quaternion moved to Planned; current features listed explicitly.
- `xmake.lua`: `-march=native` disabled by default for distribution safety.

---

## [0.1.1] — 2026-06-07

### 修复

- `operator[]` 热路径：将运行时 `if (i >= L) std::abort()` 替换为
  `if consteval` + `[[assume]]`，解锁自动向量化（逐元素循环约 16–20×
  性能差距）。
- 四处源码注释不准确：
  1. `vec.cppm`：虚假的 Vec<5+> 支持声明 —— 删除，替换为按特化列出的
     访问器表。
  2. `apply.cppm`：不可达的通用循环 fallback —— 修正为标注 else 分支存在
     但对 Vec 不生效。
  3. `operators.cppm`："所有运算符委托" —— 修正为"多数运算符委托；
     operator==/!= 使用直接展开"。
  4. `vec.cppm` 顶部：暗示所有维度有全部 swizzle 集合 —— 替换为按维度
     具体列举。
- `element_ref_t`：添加 `std::remove_reference_t`，使 trait 解析为 `T`
  （值类型）而非 `T&`，支持 const Vec。
- `vector_base.cppm`：const `operator[]` 注释改为自包含完整说明，不再仅
  引用可变重载。
- `test_boundary.cpp` / `tests/xmake.lua`：更新注释反映 `[[assume]]` 改造
  —— OOB 现在由契约 pre 条件捕获，而非运行时 if-abort 守卫。
- `CHANGELOG.md`、`CONTRIBUTING.md`、`CODE_OF_CONDUCT.md`、`NOTICE`、
  `ROADMAP.md`、`.github/ISSUE_TEMPLATE/`：以分块格式添加完整的中文翻译。
- `README.md` / `README.zh-CN.md`：测试数量更正为 128（test_contracts
  中 1 个因 GCC 16.1.0 ICE 跳过）。
- `CONTRIBUTING.md`：添加具体的测试运行命令和模式说明。

### 新增

- `m3.detail.concepts` 中的 `element_ref_t<V>` trait —— 将
  `apply_binary` / `apply_unary` 与 Vec 专属的 `operator[]` 语义解耦，
  为未来 Matrix 复用做准备。
- `--march` xmake 选项，带白名单（`generic`、`native`、`x86-64-v3`、
  `x86-64-v4`、`znver3`、`znver4`）—— `-march=native` / `-mtune=native`
  现为可选启用，默认可移植的 `generic`。
- 项目治理基线：`CHANGELOG.md`、`CONTRIBUTING.md`、`CODE_OF_CONDUCT.md`、
  `NOTICE`、`ROADMAP.md`、GitHub Issue 模板。
- `operator[]` 回归测试：全部合法索引（Vec<1..4>）、const 访问、写穿透。
- `element_ref_t` 类型解析测试：`float`、`int`、`double` 及 `const Vec`
  变体。

### 变更

- README：诚实的平台/功能状态 —— "Linux x64 (GCC 16+)" 替代
  "Cross-platform"；mcpp 标注为"社区维护，尽力支持"；Matrix 和四元数
  移至计划中；列出现有实际功能。
- `xmake.lua`：`-march=native` 默认禁用以确保分发安全。

---

## [0.1.0] — 2026-05-10 (first functional tag)

### Added

- Vector types `Vec<1..4, T>` with GLSL-style single-component swizzle
  accessors.
- Core GLSL math functions: normalize, dot, cross, length, distance, abs,
  sign, floor, ceil, fract, mod, clamp, mix, step, smoothstep, min, max,
  sin, cos, tan, asin, acos, atan, atan2, radians, degrees, pow, exp, log,
  exp2, log2, sqrt, inversesqrt, reflect, refract.
- Contracts-based bounds checking (C++26 P2900R14).
- C++26 `=delete("reason")` diagnostics (P2573R2).
- `constexpr` structured bindings (C++26 P2686R4).
- Safety features: `Div` zero-check, `clamp` argument validation,
  `normalize` zero-vector defence.
- `std::formatter` for `Vec`.
- Dual build system: xmake (primary) + mcpp (community).
- GitHub Actions CI (lint, enforce, test matrix).
- 126 test cases (128 total; 1 skipped due to GCC 16.1.0 ICE).

---

## [0.1.0] — 2026-05-10（首个功能标签）

### 新增

- 向量类型 `Vec<1..4, T>`，提供 GLSL 风格的单分量 swizzle 访问器。
- 核心 GLSL 数学函数：normalize、dot、cross、length、distance、abs、
  sign、floor、ceil、fract、mod、clamp、mix、step、smoothstep、min、max、
  sin、cos、tan、asin、acos、atan、atan2、radians、degrees、pow、exp、log、
  exp2、log2、sqrt、inversesqrt、reflect、refract。
- 基于契约的边界检查（C++26 P2900R14）。
- C++26 `=delete("reason")` 诊断信息（P2573R2）。
- `constexpr` 结构化绑定（C++26 P2686R4）。
- 安全特性：`Div` 零值检查、`clamp` 参数校验、`normalize` 零向量防御。
- `Vec` 的 `std::formatter`。
- 双构建系统：xmake（主构建）+ mcpp（社区）。
- GitHub Actions CI（lint、enforce、测试矩阵）。
- 126 个测试用例（共 128 个；test_contracts 中 1 个因 GCC 16.1.0 ICE 跳过）。
