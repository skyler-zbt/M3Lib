# M3Lib Roadmap

> Living document.  API and module structure are unstable during early
> development; specific items below may shift as implementation reveals
> new constraints or simplifications.

## Completed

### Foundation shipped (pre-v0.1.x)

- Vector types with dimensions 1..4, supporting GLSL-style single-component swizzle
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

- Define homogeneous transformation helpers for Mat4 and Vec3/Vec4,
  with explicit semantics for position and direction transformations.
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

#### Architecture cleanup

Low-risk internal refactors that consolidate duplicated shallow code
into deep modules.  No public API changes; behaviour preserved.

- **Bounds-check primitive** (issue #1): extract a shared
  `detail::bounds_check(index, size)` to replace the four copied
  defence-in-depth blocks in `VectorBase::operator[]` /
  `MatrixBase::operator[]` / `MatrixBase::operator()`.
- **Multi-character swizzle** (issue #3): implement via
  `fixed_string` NTTP + fold-expression compile-time expansion —
  zero-overhead, no P3157 (generative reflection) dependency.  The
  generic `swizzle<"...">()` lives once in `VectorBase` instead of
  30 hand-written accessors across four `Vec` specialisations.
- **Matrix product module** (issue #4): move `mat_matmul` /
  `mat_vec_mul` / `vec_mat_mul` out of `mat_operators.cppm` into a
  dedicated `m3.matrix.product` partition so shape semantics and
  algebraic properties can be tested independently of `operator*`.
- **Reduction primitive** (issue #6): add `detail::transform_reduce`
  (constexpr-unrolled) so `dot` and future Hadamard-dot share one
  reduction instead of `dot` hand-rolling per-dimension sums that
  `length` / `distance` / `normalize` transitively depend on.
- **Qualifier alignment tests** (issue #5, partial): add
  `sizeof` / `alignof` tests for `aligned_low / medium / high`
  across `Vec` and `Mat`; storage-layer de-coupling deferred to v0.5.
- **Internal-helper tests** (issue #7, priority subset): direct
  tests for `apply_*`, `mat_hadamard`, `mat_matmul`, and `Mat`
  contract bounds (currently only `Vec` bounds are contract-tested).
- **Vec/Mat Base responsibility symmetry** : 
  align `VectorBase` / `MatrixBase` responsibility
  boundaries so `Quat` / `Transform` can follow one pattern.  Two
  asymmetries found: (a) pointer-construction lives in `MatrixBase`
  once but is hand-copied across four `Vec` specialisations — hoist it
  into `VectorBase`; (b) `VectorBase` has a cross-dimension
  `= delete(...)` guard, `MatrixBase` has none — add the cross-size
  analogue.  No renaming (`Base` stays `Base`, not `Core`); no
  directory reshuffle (storage stays co-located to honour TD-002).
- **`value_ptr()` contiguity / padding contract** (issue #9): 
  formally document and test the `value_ptr()` contract across
  `aligned_low / medium / high` qualifiers for both `Vec` and `Mat`
  (contiguity, per-column padding, GPU-API direct-use eligibility).
  The storage layout is permanent; the interface is evolvable — record
  this as a CONTRIBUTING invariant.

### v0.4 — 3D transform pipeline

- Quaternion type (`Quat`) with `slerp` / `nlerp`
- `Transform` type (TRS decompose / compose)
- 3D projection matrices (`perspective`, `ortho`, `frustum`)
- Camera helpers (`lookAt`, `faceforward`)
- **`m3.math:matrix` partition** (issue #10):
  free-function matrix algebra — `transpose`, `determinant`, `inverse`,
  `trace` — in a dedicated math module, not as `Mat` members.  Needed
  by projection / lookAt implementations; currently only sketched in
  `math.cppm` as a future partition.
- **`m3.math:quaternion` partition** (issue #11):
  free-function quaternion algebra — `rotate`, `conjugate`, angle
  extraction — alongside the `Quat` type introduced above.
- Property-based testing framework
- Performance micro-benchmarks for hot paths

### v0.5 — Rectangular matrices & SIMD

- Lift `C == R` restriction on `Mat<C, R, T, Q>`
- Non-square matrix multiplication
- Review storage model requirements for SIMD support, including
  register views, alignment, and memory layout constraints.
- SIMD backend abstraction (decoupled from `std::simd`)
- SSE / AVX initial backends
- **Unified element-wise dispatch** (issue #2): make `apply_*` reuse
  `element_ref_t` / `MatrixLike` (already defined in `concepts.cppm`
  but currently unused) so `Mat` Hadamard ops share the `Vec` dispatch
  infrastructure via overload resolution — not `if-constexpr`
  branching, to keep the `Vec` hot path fast (per TD-007 deviation
  note in `mat_operators.cppm`).
- **Evaluate storage-layer decoupling after SIMD requirements are clear** (issue #5): 
  evaluate a flat layout for SIMD register views; reassess the `MatrixStorage → Vec`
  dependency introduced to break the TD-002 module cycle, now that
  non-square matrices and SIMD widen the storage requirements.

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

- 支持维度 1~4 的 Vector 类型，提供 GLSL 风格单分量 swizzle
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

- 为 Mat4 与 Vec3/Vec4 设计齐次变换辅助接口，明确区分位置变换与方向变换语义。
- `Mat4 * Vec4`（完整齐次乘法）
- 图形编程最核心的操作——将 3D 点通过 4×4 变换矩阵投影

---

## 远期方向（不做具体承诺）

以下条目仅跟踪至目标层级。具体范围、排序与设计将在接近每个里程碑时确定。

### v0.3 — Swizzle 与 API 打磨

- 多字符 swizzle（`.xy() .xyz() .rgb() .bgr()`）
- `m3::Qualifier` 别名重导出，消除 `detail::` 泄漏
- 审查 Mat/Mat Hadamard 除法语义与 GLSL 惯例的差异
- 测试基础设施中基于 ULP 的浮点比较

#### 架构清理

低风险内部重构，将重复的浅层代码沉淀为深模块。不改变公共 API，行为保持不变。

- **边界检查原语**（问题 #1）：抽取共享 `detail::bounds_check(index, size)`，
  替换 `VectorBase::operator[]` / `MatrixBase::operator[]` /
  `MatrixBase::operator()` 中四处复制的纵深防御代码块。
- **多字符 swizzle**（问题 #3）：用 `fixed_string` NTTP + 折叠表达式编译期
  展开实现——零开销，不依赖 P3157（生成式反射）。泛型 `swizzle<"...">()`
  只在 `VectorBase` 中存在一处，替代四个 `Vec` 特化中 30 个手写访问器。
- **矩阵乘积模块**（问题 #4）：将 `mat_matmul` / `mat_vec_mul` /
  `vec_mat_mul` 从 `mat_operators.cppm` 移至独立的 `m3.matrix.product` 分区，
  使形状语义与代数性质可独立于 `operator*` 测试。
- **归约原语**（问题 #6）：新增 `detail::transform_reduce`（constexpr 展开），
  使 `dot` 及未来的 Hadamard-dot 共享同一归约，而非 `dot` 手写逐维度求和、
  `length` / `distance` / `normalize` 又传递依赖它。
- **Qualifier 对齐测试**（问题 #5，部分）：为 `aligned_low / medium / high`
  补充 `Vec` 与 `Mat` 的 `sizeof` / `alignof` 测试；存储层去耦推迟到 v0.5。
- **内部辅助函数测试**（问题 #7，优先子集）：直接测试 `apply_*`、
  `mat_hadamard`、`mat_matmul` 及 `Mat` 契约边界（当前仅 `Vec` 边界有契约测试）。
- **Vec/Mat Base 职责对称性**（问题 #8）：对齐
  `VectorBase` / `MatrixBase` 的职责边界，使 `Quat` / `Transform` 能遵循同一模式。
  发现两处不对称：(a) 指针构造在 `MatrixBase` 中只写一次，却在四个 `Vec` 特化中
  各手抄一遍——上提到 `VectorBase`；(b) `VectorBase` 有跨维度 `= delete(...)`
  守卫，`MatrixBase` 没有——补上跨尺寸对应物。不改名（`Base` 保持 `Base`，
  不改 `Core`）；不重排目录（storage 保持 co-located 以遵守 TD-002）。
- **`value_ptr()` 连续性 / padding 契约**（问题 #9）：正式文档化
  并测试 `Vec` 与 `Mat` 在 `aligned_low / medium / high` 各 qualifier 下的
  `value_ptr()` 契约（连续性、列间 padding、GPU API 直接可用性）。存储布局是永久的，
  接口是可演进的——将此作为 CONTRIBUTING 不变量记录。

### v0.4 — 3D 变换管线

- 四元数类型（`Quat`），提供 `slerp` / `nlerp`
- `Transform` 类型（TRS 分解/组合）
- 3D 投影矩阵（`perspective`、`ortho`、`frustum`）
- 相机辅助函数（`lookAt`、`faceforward`）
- **`m3.math:matrix` 分区**（问题 #10）：自由函数矩阵代数——
  `transpose`、`determinant`、`inverse`、`trace`——置于独立 math 模块，
  而非 `Mat` 成员。投影 / lookAt 实现需要；当前仅在 `math.cppm` 中作为
  未来分区草拟。
- **`m3.math:quaternion` 分区**（问题 #11）：
  自由函数四元数代数——`rotate`、`conjugate`、角度提取——与上面引入的 `Quat` 类型并列。
- 基于性质的测试框架
- 热路径性能微基准

### v0.5 — 矩形矩阵与 SIMD

- 解除 `Mat<C, R, T, Q>` 的 `C == R` 限制
- 非方阵矩阵乘法
- 根据 SIMD 支持需求重新评估存储模型，包括寄存器视图、对齐和内存布局约束。
- SIMD 后端抽象（与 `std::simd` 解耦）
- SSE / AVX 初始后端
- **统一逐元素分派**（问题 #2）：让 `apply_*` 复用 `element_ref_t` /
  `MatrixLike`（已在 `concepts.cppm` 定义但当前未使用），通过重载解析使
  `Mat` Hadamard 运算共享 `Vec` 分派基础设施——而非 `if-constexpr` 分支，
  以保 `Vec` 热路径性能（见 `mat_operators.cppm` 中 TD-007 偏离说明）。
- **在明确 SIMD 需求后评估存储层去耦**（问题 #5）：
  评估 SIMD 寄存器视图的扁平布局；重新审视为打破 TD-002 模块循环
  而引入的 `MatrixStorage → Vec` 依赖——非方阵与 SIMD 将扩大
  存储需求，届时一并重新评估。

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