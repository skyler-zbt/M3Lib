# M3Lib Roadmap

> Living document.  API and module structure are unstable during early
> development; specific items below may shift as implementation reveals
> new constraints or simplifications.

## Completed

### Foundation shipped (pre-v0.1.x)

- `Vec<1..4, T>` with GLSL-style single-component swizzle
  (.x .y .z .w, .r .g .b .a, .s .t .p .q)
- Square matrix types `Mat<2..4, T>` with column-major layout and
  `Mat2 / Mat3 / Mat4` aliases
- Matrix operators: Hadamard (`+ - /`), matrix product (`*`),
  scalar broadcast, compound assignment, `== !=`
- Vec–Mat and Mat–Vec products (column-vector right multiplication,
  row-vector left multiplication)
- Core GLSL vector functions: `dot`, `cross`, `normalize`, `length`,
  `distance`, `reflect`, `refract`
- Core GLSL common functions: `mix`, `clamp`, `lerp`
- Vec arithmetic operators: element-wise, scalar broadcast,
  compound assignment, comparison
- Element-wise dispatch infrastructure: `apply_binary`, `apply_unary`,
  `apply_scalar_binary_left/right`, `Add / Sub / Mul / Div / Neg` functors
- Aligned storage with `Qualifier` (`aligned_none / low / medium / high`)
- Contracts-based bounds checking (C++26 P2900R14) for `Vec` and `Mat`
- `=delete("reason")` diagnostics (C++26 P2573R2) for cross-dimension Vec
- `constexpr` structured bindings (C++26 P2686R4) for `Vec`
- `std::formatter` for `Vec` (debug display)
- xmake + mcpp dual build; CI: lint + enforce + observe modes

---

## Next milestone

### v0.2 — Graphics Math Foundation

**Goal:** Provide the function surface and type aliases needed for basic
graphics work, so M3Lib becomes a viable alternative to GLM for shader-
adjacent code.

#### P0 — type aliases

- `vec2 vec3 vec4` (`Vec<2..4, float>`)
- `ivec2 ivec3 ivec4` (`Vec<2..4, int>`)
- `Mat2 Mat3 Mat4` already exist as type aliases; verify coverage

#### P0 — GLSL trigonometric functions

- `sin cos tan asin acos atan atan2 radians degrees`
- Scalar + per-element vector overloads
- Free functions in `m3.math.trig`

#### P0 — GLSL exponential functions

- `pow exp log exp2 log2 sqrt inversesqrt`
- Scalar + per-element vector overloads
- Free functions in `m3.math.exp`

#### P0 — GLSL common utility functions

- `abs sign floor ceil fract mod step smoothstep min max`
- Scalar + per-element vector overloads
- Free functions in `m3.math.common` (extending the existing module)

#### P0 — Mat4 × Vec3 homogeneous transform

- `Mat4 * Vec3` with implicit `w = 1` (position transform)
- `Mat4 * Vec4` (full homogeneous multiply)
- The single most important operation for graphics — projecting 3D
  points through a 4×4 transform matrix

---

## Direction (no detailed plans)

These items are tracked at the goal level only.  Specific scope, ordering,
and design will be worked out when we approach each milestone.

### v0.3 — Swizzle & API polish

- Multi-character swizzle (`.xy() .xyz() .rgb() .bgr()`)
- `m3::Qualifier` alias re-export to remove `detail::` leakage
- Review Mat/Mat Hadamard division semantics vs GLSL convention
- ULP-based floating-point comparison in test infrastructure

### v0.4 — 3D transform pipeline

- Quaternion type (`Quat`) with `slerp` / `nlerp`
- `Transform` type (TRS decompose / compose)
- 3D projection matrices (`perspective`, `ortho`, `frustum`)
- Camera helpers (`lookAt`, `faceforward`)
- Property-based testing framework
- Performance micro-benchmarks for hot paths

### v0.5 — Rectangular matrices & SIMD

- Lift `C == R` restriction on `Mat<C, R, T, Q>`
- Non-square matrix multiplication
- SIMD backend abstraction (decoupled from `std::simd`)
- SSE / AVX initial backends

### v0.6 — Ecosystem & cross-platform

- Windows / macOS port
- Green CI matrix: Linux + Windows + macOS × supported compilers
- API reference site (Doxygen or custom generator)
- Getting-started tutorial; GLM → M3Lib migration guide
- xmake-repo package publication
- GLM / Eigen interop helpers (conversion only, not bindings)

### v1.0 — Stable API

- API stability commitment (API freeze; subsequent changes minor-version
  additive only — not ABI, which is not realistic for template-heavy
  modular libraries across compiler versions)
- Trigger: 6 months after C++26 final IS publication

> ⚠️ **Critical external dependency:** C++26 IS publication date.
> If ISO publishes later than 2026-10, v1.0 slips accordingly.

---

# M3Lib 路线图

> 活文档。早期开发阶段 API 与模块结构尚不稳定；具体条目可能随实现
> 揭示的新约束或简化而调整。

## 已完成

### 已交付的基础（pre-v0.1.x）

- `Vec<1..4, T>`，GLSL 风格单分量 swizzle
  （.x .y .z .w，.r .g .b .a，.s .t .p .q）
- 方阵类型 `Mat<2..4, T>`，列主序布局，提供 `Mat2 / Mat3 / Mat4` 别名
- 矩阵运算符：Hadamard（`+ - /`）、矩阵乘法（`*`）、标量广播、
  复合赋值、`== !=`
- Vec–Mat 与 Mat–Vec 乘法（列向量右乘、行向量左乘）
- 核心 GLSL 向量函数：`dot`、`cross`、`normalize`、`length`、
  `distance`、`reflect`、`refract`
- 核心 GLSL 通用函数：`mix`、`clamp`、`lerp`
- Vec 算术运算符：逐分量、标量广播、复合赋值、比较
- 逐元素分派基础设施：`apply_binary`、`apply_unary`、
  `apply_scalar_binary_left/right`，`Add / Sub / Mul / Div / Neg` 函数对象
- 对齐存储，配 `Qualifier`（`aligned_none / low / medium / high`）
- 基于契约的边界检查（C++26 P2900R14），适用于 `Vec` 与 `Mat`
- `=delete("reason")` 诊断（C++26 P2573R2），用于跨维度 Vec
- `constexpr` 结构化绑定（C++26 P2686R4），适用于 `Vec`
- `Vec` 的 `std::formatter`（调试显示）
- xmake + mcpp 双构建；CI：lint + enforce + observe 模式

---

## 下一里程碑

### v0.2 — 图形数学基础

**目标：** 提供基础图形工作所需的函数面与类型别名，使 M3Lib 成为
GLM 在 shader 邻近代码中的可行替代。

#### P0 — 类型别名

- `vec2 vec3 vec4`（`Vec<2..4, float>`）
- `ivec2 ivec3 ivec4`（`Vec<2..4, int>`）
- `Mat2 Mat3 Mat4` 已作为类型别名存在；核实覆盖

#### P0 — GLSL 三角函数

- `sin cos tan asin acos atan atan2 radians degrees`
- 标量 + 逐元素向量重载
- 自由函数置于 `m3.math.trig`

#### P0 — GLSL 指数函数

- `pow exp log exp2 log2 sqrt inversesqrt`
- 标量 + 逐元素向量重载
- 自由函数置于 `m3.math.exp`

#### P0 — GLSL 常用工具函数

- `abs sign floor ceil fract mod step smoothstep min max`
- 标量 + 逐元素向量重载
- 自由函数置于 `m3.math.common`（扩展现有模块）

#### P0 — Mat4 × Vec3 齐次变换

- `Mat4 * Vec3`，隐式 `w = 1`（位置变换）
- `Mat4 * Vec4`（完整齐次乘法）
- 图形编程最核心的操作——将 3D 点通过 4×4 变换矩阵投影

---

## 远期方向（不做具体承诺）

以下条目仅跟踪至目标层级。具体范围、排序与设计将在接近每个里程碑时
确定。

### v0.3 — Swizzle 与 API 打磨

- 多字符 swizzle（`.xy() .xyz() .rgb() .bgr()`）
- `m3::Qualifier` 别名重导出，消除 `detail::` 泄漏
- 审查 Mat/Mat Hadamard 除法语义与 GLSL 惯例的差异
- 测试基础设施中基于 ULP 的浮点比较

### v0.4 — 3D 变换管线

- 四元数类型（`Quat`），提供 `slerp` / `nlerp`
- `Transform` 类型（TRS 分解/组合）
- 3D 投影矩阵（`perspective`、`ortho`、`frustum`）
- 相机辅助函数（`lookAt`、`faceforward`）
- 基于性质的测试框架
- 热路径性能微基准

### v0.5 — 矩形矩阵与 SIMD

- 解除 `Mat<C, R, T, Q>` 的 `C == R` 限制
- 非方阵矩阵乘法
- SIMD 后端抽象（与 `std::simd` 解耦）
- SSE / AVX 初始后端

### v0.6 — 生态与跨平台

- Windows / macOS 移植
- 绿色 CI 矩阵：Linux + Windows + macOS × 受支持编译器
- API 参考站（Doxygen 或自定义生成器）
- 入门教程；GLM → M3Lib 迁移指南
- xmake-repo 包发布
- GLM / Eigen 互操作辅助（仅转换，非绑定）

### v1.0 — 稳定 API

- API 稳定性承诺（API 冻结；后续仅小版本加法——非 ABI，
  因为模板重度的模块化库跨编译器版本承诺 ABI 稳定性不现实）
- 触发条件：C++26 正式标准发布后 6 个月

> ⚠️ **关键外部依赖：** C++26 正式标准发布日期。若 ISO 发布晚于
> 2026-10，v1.0 顺延。