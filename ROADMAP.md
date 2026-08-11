# M3Lib Roadmap

> Living document. Early development — API and module structure may still shift.
> Baseline: GLSL 4.60 math types, operators, and built-ins (§8.1–§8.6) on the CPU side.
> See also: [docs/testing-and-ci.md](docs/testing-and-ci.md), [docs/glsl-deferred.md](docs/glsl-deferred.md).

**Status:** v0.2 foundation is in tree (Vec/Mat, core trig/exp/common/geometric helpers).
This document lists only work still to do or correct.

**Out of scope here:** full shader-language features (precision qualifiers, interpolation, texture sampling). Integer bitfield / pack-unpack / noise are intentionally deferred — see [docs/glsl-deferred.md](docs/glsl-deferred.md).

**Intentional deltas (not bugs):**
- `normalize(zero)` returns a zero vector (GLSL: undefined).
- Default storage is packed CPU math types, not `std140` / `std430` GPU layouts.
- `operator==` / `operator!=` on Vec/Mat return scalar `bool` (C++); component-wise compares belong in relational functions.
- `atan2` and `lerp` are C++-friendly aliases alongside GLSL names.

---

## v0.3 — Correctness gate & GLSL surface fill

### Fix first

- Fix `detail::mat_matmul` to column-major `A * B`: `result(c, r) = Σ_k a(k, r) * b(c, k)`; align comments with the implementation.
- Retract or strictly qualify the “GLSL std430-compatible” claim in `mat.cppm`; document the `value_ptr()` contract (contiguity, per-column padding, GPU direct-use eligibility) for each `Qualifier`.
- Add missing genType operators: `scalar - Mat`, `scalar / Mat`.

### Types, swizzle, aliases

- Multi-component swizzle via `fixed_string` NTTP on `VectorBase` (`.xy()`, `.xyz()`, `.rgb()`, …).
- Add `bvec*` / `uvec*` / `dvec*` aliases; lowercase `mat2` / `mat3` / `mat4`.
- Re-export `m3::Qualifier` (stop leaking `detail::` at the public surface).
- GLSL-style constructors where practical (e.g. `vec4(vec3, float)`, fuller mat column/diagonal construction).

### Built-in coverage

- Complete common overloads: `mix(vec, vec, vec)`, `clamp(vec, vec, vec)`, `step(vec, vec)`, `smoothstep(vec, vec, vec)`, boolean `mix`.
- Add §8.6 relational functions returning `bvec`: `lessThan` / `lessThanEqual` / `greaterThan` / `greaterThanEqual` / `equal` / `notEqual` / `any` / `all` / `not`.
- Add `faceforward` (§8.4).
- Add hyperbolic functions (§8.1): `sinh` / `cosh` / `tanh` / `asinh` / `acosh` / `atanh`.
- Add remaining high-value §8.3 helpers used in graphics math: `trunc` / `round` / `roundEven` / `modf` / `isnan` / `isinf` / `fma`.

### Internal cleanup (only as needed for the above)

- Shared `detail::bounds_check`; align `VectorBase` / `MatrixBase` responsibilities (pointer ctor hoist, cross-size `= delete` on Mat).
- `detail::transform_reduce` for `dot` and dependents.

---

## v0.4 — Matrix algebra & 3D pipeline

- Ship `m3.math:matrix`: `transpose`, `determinant`, `inverse`, `matrixCompMult`, `outerProduct` (and `trace` if useful).
- Quaternion type `Quat` and `m3.math:quaternion` (`slerp` / `nlerp` / conjugate / rotate / angle extraction).
- Projection and camera helpers: `perspective`, `ortho`, `frustum`, `lookAt`.
- Decide packed CPU math vs explicit `std140` / `std430` GPU-layout types (pack/unpack API), feeding back into the `value_ptr()` contract.

---

## v0.5 — Rectangular matrices & SIMD

- Lift the `C == R` restriction on `Mat<C, R, T, Q>`; support non-square products and `matCxR` aliases.
- Review storage for SIMD (register views, alignment, layout).
- SIMD backend abstraction (decoupled from `std::simd`); initial SSE / AVX backends.
- Unify Mat/Vec element-wise dispatch via `MatrixLike` / overload resolution without slowing the Vec hot path.

---

## v0.6 — Ecosystem

- Green CI on Linux + Windows × supported compilers (macOS only if maintainer hardware appears — do not promise it).
- API reference, getting-started notes, GLM → M3Lib migration guide.
- xmake-repo package publication.
- GLM / Eigen conversion helpers (conversion only, not bindings).

---

## v1.0 — Stable API

- API stability commitment (freeze; later changes minor-version additive only — not ABI).
- Trigger: 6 months after the C++26 IS is published. If ISO slips past 2026-10, v1.0 slips with it.

---

# M3Lib 路线图

> 活文档。早期开发阶段 API 与模块结构仍可能变动。
> 对齐基准：GLSL 4.60 在 CPU 侧的数学类型、运算符与内建函数（§8.1–§8.6）。
> 另见：[docs/testing-and-ci.md](docs/testing-and-ci.md)、[docs/glsl-deferred.md](docs/glsl-deferred.md)。

**现状：** v0.2 基础已在库中（Vec/Mat、核心三角/指数/通用/几何辅助）。
本文只列仍需完成或修正的工作。

**不在此范围：** 完整着色器语言特性（精度限定符、插值、纹理采样等）。整数位域 / pack-unpack / 噪声等故意推迟——见 [docs/glsl-deferred.md](docs/glsl-deferred.md)。

**故意语义差（非缺陷）：**
- `normalize(零向量)` 返回零向量（GLSL：未定义）。
- 默认存储为打包的 CPU 数学类型，而非 `std140` / `std430` GPU 布局。
- Vec/Mat 的 `operator==` / `operator!=` 返回标量 `bool`（C++）；分量比较由关系函数提供。
- `atan2` 与 `lerp` 为并列于 GLSL 名称的 C++ 友好别名。

---

## v0.3 — 正确性门槛与 GLSL 表面补齐

### 优先修复

- 将 `detail::mat_matmul` 修正为列主序 `A * B`：`result(c, r) = Σ_k a(k, r) * b(c, k)`；注释与实现对齐。
- 撤回或严格限定 `mat.cppm` 中的 “GLSL std430-compatible” 声明；文档化各 `Qualifier` 下 `value_ptr()` 契约（连续性、列间 padding、GPU 直接可用性）。
- 补齐 genType 运算符：`scalar - Mat`、`scalar / Mat`。

### 类型、swizzle、别名

- 用 `fixed_string` NTTP 在 `VectorBase` 上实现多分量 swizzle（`.xy()`、`.xyz()`、`.rgb()` 等）。
- 增加 `bvec*` / `uvec*` / `dvec*` 别名；小写 `mat2` / `mat3` / `mat4`。
- 重导出 `m3::Qualifier`（公共面不再泄漏 `detail::`）。
- 在可行处补齐 GLSL 式构造（如 `vec4(vec3, float)`、更完整的矩阵列/对角构造）。

### 内建覆盖

- 补齐常用重载：`mix(vec, vec, vec)`、`clamp(vec, vec, vec)`、`step(vec, vec)`、`smoothstep(vec, vec, vec)`、布尔 `mix`。
- 增加返回 `bvec` 的 §8.6 关系函数：`lessThan` / `lessThanEqual` / `greaterThan` / `greaterThanEqual` / `equal` / `notEqual` / `any` / `all` / `not`。
- 增加 `faceforward`（§8.4）。
- 增加双曲函数（§8.1）：`sinh` / `cosh` / `tanh` / `asinh` / `acosh` / `atanh`。
- 增加图形数学常用的其余 §8.3 辅助：`trunc` / `round` / `roundEven` / `modf` / `isnan` / `isinf` / `fma`。

### 内部清理（仅服务于上述功能）

- 共享 `detail::bounds_check`；对齐 `VectorBase` / `MatrixBase` 职责（指针构造上提、Mat 跨尺寸 `= delete`）。
- 用 `detail::transform_reduce` 统一 `dot` 及其依赖。

---

## v0.4 — 矩阵代数与 3D 管线

- 落地 `m3.math:matrix`：`transpose`、`determinant`、`inverse`、`matrixCompMult`、`outerProduct`（及有用的 `trace`）。
- 四元数类型 `Quat` 与 `m3.math:quaternion`（`slerp` / `nlerp` / 共轭 / 旋转 / 角度提取）。
- 投影与相机辅助：`perspective`、`ortho`、`frustum`、`lookAt`。
- 决定打包 CPU 数学类型 vs 显式 `std140` / `std430` GPU 布局类型（pack/unpack API），并反哺 `value_ptr()` 契约。

---

## v0.5 — 矩形矩阵与 SIMD

- 解除 `Mat<C, R, T, Q>` 的 `C == R` 限制；支持非方阵乘法与 `matCxR` 别名。
- 按 SIMD 需求审视存储（寄存器视图、对齐、布局）。
- SIMD 后端抽象（与 `std::simd` 解耦）；首批 SSE / AVX 后端。
- 通过 `MatrixLike` / 重载解析统一 Mat/Vec 逐元分派，且不拖慢 Vec 热路径。

---

## v0.6 — 生态

- Linux + Windows × 受支持编译器的绿色 CI（macOS 仅在有维护者硬件时再议——不做空头承诺）。
- API 参考、入门说明、GLM → M3Lib 迁移指南。
- xmake-repo 包发布。
- GLM / Eigen 转换辅助（仅转换，非绑定）。

---

## v1.0 — 稳定 API

- API 稳定性承诺（冻结；后续仅小版本加法——非 ABI）。
- 触发条件：C++26 正式标准发布后 6 个月。若 ISO 晚于 2026-10，v1.0 同步后延。
