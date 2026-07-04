# M3Lib Roadmap

> Revised 2026-07-04 based on external review feedback.

## Completed

### v0.1.0 — Vector Foundation (released)

- `Vec<1..4, T>` with GLSL-style single-component swizzle
- Contracts-based bounds checking (C++26 P2900R14)
- `=delete("reason")` diagnostics (C++26 P2573R2)
- `constexpr` structured bindings (C++26 P2686R4)
- Matrix types `Mat<2,2>` through `Mat<4,4>` (square only)
- Matrix operators (`+ - *` MM/MV, `== !=`, compound assignment)
- `transpose`, `inverse`, `determinant`, factory functions
- Core GLSL vector functions: `dot`, `cross`, `normalize`, `length`, `distance`, `reflect`, `refract`
- Core GLSL common functions: `mix`, `clamp`, `lerp`
- CI: xmake build + test on push, `-march=native` opt-in
- `CHANGELOG.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `NOTICE`
- README in English and Simplified Chinese

---

## Planned

### v0.2.0 — Graphics Math Foundation

**Goal:** Complete the math function surface needed for basic graphics work,
and provide the type aliases users expect.

**GLSL math function completion**
- Utility: `abs sign floor ceil fract mod step smoothstep min max`
- Trigonometry: `sin cos tan asin acos atan atan2 radians degrees`
- Exponential: `pow exp log exp2 log2 sqrt inversesqrt`
- Scalar + per-element vector overloads

**Type aliases**
- `vec2 vec3 vec4` (`Vec<2,float>` … `Vec<4,float>`)
- `ivec2 ivec3 ivec4` (`Vec<2,int>` … `Vec<4,int>`)
- `mat2 mat3 mat4` (`Mat<2,2,float>` … `Mat<4,4,float>`)

**Mat4 × Vec3 homogeneous transform**
- `Mat4 * Vec3` with implicit `w=1` (position transform)
- `Mat4 * Vec4` (full homogeneous multiply)
- This is the single most important operation for graphics — projecting
  3D points through a 4×4 transform matrix.

**ULP floating-point comparison**
- Replace fixed-epsilon `check_float_eq` with ULP-based comparison
- Addresses the `1e-20f` tolerance issue identified in review

### v0.2.1 — Polish & Multi-char Swizzle

**Goal:** API cleanliness and developer experience.

**Multi-character swizzle**
- `.xy() .xyz() .rgb() .bgr()` etc. (all 2D/3D/4D permutations)
- Correct move and copy semantics
- Minimum: all 2D and 3D permutations; 4D can follow

**API cleanup**
- Eliminate `detail::` types from public signatures
  (`m3::Qualifier` alias re-export)
- Audit and complete `import std;` coverage

**Mat/Mat division semantics**
- Current element-wise (Hadamard) division differs from GLSL convention
  (where matrix "division" means multiply-by-inverse)
- Evaluate: rename to `hadamard_div`, remove, or add explicit
  `mul_inverse` alongside

**CI upgrade**
- mcpp CI tiered coverage (`mcpp-verify` job)
- Contracts ignore-mode testing
- NaN/Inf boundary testing
- mcpp decision: full CI support or official deprecation

### v0.3.0 — 3D Transform Pipeline

**Goal:** Full 3D scene-graph transform support.

**Quaternion**
- `Quat` type (GLSL-style)
- `angleAxis`, `lookRotation`, `euler` constructors
- `slerp`, `nlerp` interpolation
- `mat3_cast` / `mat4_cast` conversion

**Transform pipeline**
- `Transform` type (TRS decompose / compose)
- 3D projection matrices: `perspective`, `ortho`, `frustum`
- Camera helper: `lookAt`
- `faceforward` function

**Testing**
- Property-based testing framework
- Performance benchmark suite (microbenchmarks for hot paths)

### v0.4.0 — Generalisation & Performance

**Goal:** Remove dimension restrictions and establish performance baseline.

**Rectangular matrices**
- Lift the `C == R` restriction on `Mat<C,R,T,Q>`
- Non-square matrix multiplication

**SIMD abstraction layer**
- Design a SIMD backend abstraction (not coupled to `std::simd`)
- Provide SSE/AVX backends as initial implementations
- `std::simd` as an optional backend when standardised
- No dependency on unstandardised features

**Performance benchmarks**
- CI-tracked microbenchmarks for Vec/Mat operations
- Compare against GLM on common workloads

### v0.5.0 — Ecosystem & Cross-platform

**Goal:** Reachable from any platform, integrable with any toolchain.

**Cross-platform**
- Windows MSVC / Clang-CL
- macOS Apple Clang
- Green CI matrix: Linux/macOS/Windows × GCC/Clang/MSVC

**Documentation**
- API reference site (Doxygen / custom generator)
- Migration guide (GLM → M3Lib)
- Getting-started tutorial

**Ecosystem integration**
- GLM / Eigen interop helpers (conversion functions, not bindings)
- xmake-repo package publication
- Docker standardised build images

### v1.0.0 — Stable API

**Goal:** API stability commitment; suitable for production dependency.

- **API stability** (not ABI — template-heavy modular libraries cannot
  realistically promise ABI stability across compiler versions)
- 6 months after C++26 final IS publication (core dependency stabilisation)
- Full green CI matrix across all supported platforms
- API freeze; subsequent changes are minor-version additive only
- Complete documentation site + community governance model

> ⚠️ **Critical external dependency:** C++26 IS publication date.
> If ISO publishes later than 2026-10, v1.0.0 slips accordingly.

---

# M3Lib 路线图

> 2026-07-04 基于外部评审反馈修订。

## 已完成

### v0.1.0 — 向量基础（已发布）

- `Vec<1..4, T>`，GLSL 风格单分量 swizzle
- 基于契约的边界检查（C++26 P2900R14）
- `=delete("reason")` 诊断（C++26 P2573R2）
- `constexpr` 结构化绑定（C++26 P2686R4）
- 矩阵类型 `Mat<2,2>` 至 `Mat<4,4>`（仅方阵）
- 矩阵运算符（`+ - *` MM/MV、`== !=`、复合赋值）
- `transpose`、`inverse`、`determinant`、工厂函数
- 核心 GLSL 向量函数：`dot`、`cross`、`normalize`、`length`、`distance`、`reflect`、`refract`
- 核心 GLSL 通用函数：`mix`、`clamp`、`lerp`
- CI：xmake 构建 + push 测试、`-march=native` 可选化
- `CHANGELOG.md`、`CONTRIBUTING.md`、`CODE_OF_CONDUCT.md`、`NOTICE`
- 中英双语 README

---

## 计划中

### v0.2.0 — 图形数学基础

**目标：** 补齐基础图形工作所需的数学函数面，并提供用户期待的类型别名。

**GLSL 数学函数补齐**
- 常用：`abs sign floor ceil fract mod step smoothstep min max`
- 三角：`sin cos tan asin acos atan atan2 radians degrees`
- 指数：`pow exp log exp2 log2 sqrt inversesqrt`
- 标量 + 逐元素向量重载

**类型别名**
- `vec2 vec3 vec4`（`Vec<2,float>` … `Vec<4,float>`）
- `ivec2 ivec3 ivec4`（`Vec<2,int>` … `Vec<4,int>`）
- `mat2 mat3 mat4`（`Mat<2,2,float>` … `Mat<4,4,float>`）

**Mat4 × Vec3 齐次变换**
- `Mat4 * Vec3`，隐式 `w=1`（位置变换）
- `Mat4 * Vec4`（完整齐次乘法）
- 这是图形编程最核心的操作——将 3D 点通过 4×4 变换矩阵投影

**ULP 浮点比较**
- 用基于 ULP 的比较替换固定 epsilon 的 `check_float_eq`
- 解决评审指出的 `1e-20f` 容差问题

### v0.2.1 — 打磨与多字符 Swizzle

**目标：** API 整洁度与开发者体验。

**多字符 Swizzle**
- `.xy() .xyz() .rgb() .bgr()` 等（全部 2D/3D/4D 排列）
- 正确的移动和拷贝语义
- 最低要求：全部 2D 和 3D 排列；4D 可后续补充

**API 清理**
- 消除公开签名中的 `detail::` 类型（`m3::Qualifier` 别名重导出）
- 审计并补齐 `import std;` 覆盖

**Mat/Mat 除法语义**
- 当前逐元素（Hadamard）除法与 GLSL 惯例不符（GLSL 中矩阵"除法"指乘以逆矩阵）
- 评估方案：重命名为 `hadamard_div`、移除，或同时提供 `mul_inverse`

**CI 升级**
- mcpp CI 阶梯覆盖（`mcpp-verify` job）
- Contracts ignore 模式测试
- NaN/Inf 边界测试
- mcpp 决策：完整 CI 支持或正式弃用

### v0.3.0 — 3D 变换管线

**目标：** 完整 3D 场景图变换支持。

**四元数**
- `Quat` 类型（GLSL 风格）
- `angleAxis`、`lookRotation`、`euler` 构造函数
- `slerp`、`nlerp` 插值
- `mat3_cast` / `mat4_cast` 转换

**变换管线**
- `Transform` 类型（TRS 分解/组合）
- 3D 投影矩阵：`perspective`、`ortho`、`frustum`
- 相机辅助函数：`lookAt`
- `faceforward` 函数

**测试**
- Property-based testing 框架
- 性能基准套件（热点路径微基准）

### v0.4.0 — 泛化与性能

**目标：** 解除维度限制并建立性能基线。

**矩形矩阵**
- 解除 `Mat<C,R,T,Q>` 的 `C == R` 限制
- 非方阵矩阵乘法

**SIMD 抽象层**
- 设计 SIMD 后端抽象（不与 `std::simd` 耦合）
- 提供 SSE/AVX 后端作为初始实现
- `std::simd` 作为可选后端（标准化后接入）
- 不依赖尚未标准化的特性

**性能基准**
- CI 跟踪的 Vec/Mat 操作微基准
- 与 GLM 在常见工作负载上对比

### v0.5.0 — 生态与跨平台

**目标：** 任何平台可触及，任何工具链可集成。

**跨平台**
- Windows MSVC / Clang-CL
- macOS Apple Clang
- 绿色 CI 矩阵：Linux/macOS/Windows × GCC/Clang/MSVC

**文档**
- API 参考站（Doxygen / 自定义生成器）
- 迁移指南（GLM → M3Lib）
- 入门教程

**生态集成**
- GLM / Eigen 互操作辅助（转换函数，非绑定）
- xmake-repo 包发布
- Docker 标准化构建镜像

### v1.0.0 — 稳定 API

**目标：** API 稳定性承诺，适合作为生产依赖。

- **API 稳定**（非 ABI —— 模板重度模块化库无法跨编译器版本承诺
  ABI 稳定性）
- C++26 正式标准发布后 6 个月（核心依赖稳定）
- 全平台 CI 绿色矩阵
- API 冻结，后续仅小版本加法
- 完整文档站 + 社区治理模型

> ⚠️ **关键外部依赖：** C++26 正式标准发布日期。若 ISO 发布晚于
> 2026-10，v1.0.0 顺延。
