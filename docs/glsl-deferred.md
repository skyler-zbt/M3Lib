# Deferred GLSL coverage (beyond the main roadmap)

> Explains GLSL 4.60 areas M3Lib deliberately does **not** chase in the near term,
> why, and what implementing them would take if priorities change.

Reference: [GLSLangSpec.4.60.pdf](GLSLangSpec.4.60.pdf).

## Why these are deferred

M3Lib targets **CPU-side graphics math** with GLSL-like names and semantics.
Full §8 parity includes shader-centric and bit-packing helpers that most engine/math call sites never use on the CPU. Shipping them early would expand API surface, constexpr/`noexcept` policy, and test matrix cost without helping the v0.3–v0.4 path (correctness, swizzle, matrix algebra, transforms).

Roadmap **does** plan the high-value remainder of §8.1–§8.6 (hyperbolics, relational ops, matrix functions, common overloads, `trunc`/`round`/`fma`/…). This file is only for the long tail.

## Intentionally out of near-term scope

| Area | Spec | Examples | Why defer |
|------|------|----------|-----------|
| Floating-point bit casts | §8.3 | `floatBitsToInt`, `floatBitsToUint`, `intBitsToFloat`, `uintBitsToFloat` | Rare in CPU math APIs; endian/NaN payload rules need explicit policy |
| Pack / unpack | §8.3 | `packUnorm2x16`, `packSnorm4x8`, `unpackHalf2x16`, `packDouble2x32`, … | GPU transfer / attribute packing; usually lives next to asset/GPU code, not core Vec/Mat |
| Integer arithmetic helpers | §8.7 | `uaddCarry`, `usubBorrow`, `umulExtended`, `imulExtended` | Shader ALU emulation; little overlap with transform pipelines |
| Bitfield ops | §8.7 | `bitfieldExtract`, `bitfieldInsert`, `bitfieldReverse`, `bitCount`, `findLSB`, `findMSB` | Same as above; needs `uvec`/`ivec` and well-defined signed shift behaviour |
| Texture / noise / derivative | later §8 | `texture*`, `dFdx`, `noise*` | Not meaningful without a shading environment |
| Language-only features | Ch.4–7 | precision qualifiers, interpolation, invariant, memory qualifiers | Shader language, not a C++ math library |

Also not a “missing GLSL built-in”, but related: **dedicated `std140` / `std430` layout types** are a design decision for v0.4 (see ROADMAP), not automatic inheritance from packed `Vec`/`Mat`.

## If you decide to implement them later

### 1. Policy first

- Publish a short table: function → domain → NaN/Inf → out-of-range → intentional delta vs GLSL.
- Decide whether pack results follow GLSL bit-exact rules or “round-trip friendly” CPU rules.
- Decide module placement: e.g. `m3.math:packing`, `m3.math:integer` — keep them opt-in so core `import m3;` users do not pay compile cost.

### 2. Prerequisites already on the roadmap

- `uvec*` / `ivec*` / `bvec*` aliases (v0.3).
- Stable `value_ptr()` / layout story if pack outputs are meant for GPU upload (v0.3–v0.4).
- Relational / boolean mix if any packing helpers return or consume `bvec` masks.

### 3. Implementation sketch

- **Bit casts:** thin wrappers around `std::bit_cast` (C++20+) with vector overloads via existing `apply_unary`.
- **Pack/unpack:** scalar reference implementations matching GLSL formulas; vector overloads only where the spec defines them; constexpr where bit ops allow.
- **§8.7 integer:** pure integer functors; document undefined cases (e.g. shift amounts) as contracts/`pre` rather than silent UB.
- **Do not** implement texture/noise/derivatives in this library — document as permanently out of scope unless the project gains a shading runtime.

### 4. Work estimate (order-of-magnitude)

| Slice | Rough effort | Notes |
|-------|----------------|-------|
| Bit casts + tests | small | Mostly API + edge NaN cases |
| Unorm/Snorm/half pack family | medium | Many overloads; golden vectors from GLSL/CTS or GLM |
| Double/64-bit pack | small–medium | Careful with `double` ↔ `uvec2` |
| Full §8.7 bitfield/carry | medium–large | Wide overload matrix; contract design |
| Texture/noise/derivatives | n/a | Reject unless scope changes |

### 5. Suggested trigger to reopen

Revisit only when a concrete consumer needs CPU-side pack or bitfield parity (e.g. shared CPU/GPU material encoding, or a tooling path that must match shader pack ops bit-exactly). Until then, keep ROADMAP focused on math used by transforms, lighting vectors, and matrix algebra.

---

# 推迟的 GLSL 覆盖（主路线图之外）

> 说明 M3Lib **近期故意不跟** 的 GLSL 4.60 区域、原因，以及若优先级变化时实现它们需要做什么。

参考：[GLSLangSpec.4.60.pdf](GLSLangSpec.4.60.pdf)。

## 为何推迟

M3Lib 面向带 GLSL 风格命名与语义的 **CPU 侧图形数学**。
完整 §8 对齐会包含大量着色器中心、位打包类辅助，多数引擎/数学调用点在 CPU 上根本用不到。过早交付会扩大 API 面、constexpr/`noexcept` 策略与测试矩阵成本，却帮不到 v0.3–v0.4 主路径（正确性、swizzle、矩阵代数、变换）。

路线图 **会** 规划 §8.1–§8.6 中高价值剩余部分（双曲、关系运算、矩阵函数、常用重载、`trunc`/`round`/`fma` 等）。本文只覆盖长尾。

## 近期明确不做

| 类别 | 规范 | 示例 | 推迟原因 |
|------|------|------|----------|
| 浮点位型转换 | §8.3 | `floatBitsToInt`、`floatBitsToUint`、`intBitsToFloat`、`uintBitsToFloat` | CPU 数学 API 中少见；endian / NaN payload 规则需单独定策略 |
| Pack / unpack | §8.3 | `packUnorm2x16`、`packSnorm4x8`、`unpackHalf2x16`、`packDouble2x32` 等 | 偏 GPU 传输 / 属性打包；通常靠近资产与 GPU 代码，而非核心 Vec/Mat |
| 整数算术辅助 | §8.7 | `uaddCarry`、`usubBorrow`、`umulExtended`、`imulExtended` | 着色器 ALU 仿真；与变换管线重叠很少 |
| 位域运算 | §8.7 | `bitfieldExtract`、`bitfieldInsert`、`bitfieldReverse`、`bitCount`、`findLSB`、`findMSB` | 同上；需要 `uvec`/`ivec` 与明确的有符号移位语义 |
| 纹理 / 噪声 / 导数 | 后续 §8 | `texture*`、`dFdx`、`noise*` | 没有着色环境则无意义 |
| 纯语言特性 | Ch.4–7 | 精度限定符、插值、invariant、内存限定符 | 着色器语言，不是 C++ 数学库 |

另有相关但非「缺失内建」项：**独立的 `std140` / `std430` 布局类型** 是 v0.4 的设计决策（见 ROADMAP），不会从打包的 `Vec`/`Mat` 自动继承。

## 若日后决定实现

### 1. 先定策略

- 发布简表：函数 → 定义域 → NaN/Inf → 越界 → 相对 GLSL 的故意语义差。
- 决定 pack 结果跟 GLSL 比特精确规则，还是「往返友好」的 CPU 规则。
- 决定模块落点：如 `m3.math:packing`、`m3.math:integer`——保持可选用，避免核心 `import m3;` 用户承担编译成本。

### 2. 路线图上已有的前置

- `uvec*` / `ivec*` / `bvec*` 别名（v0.3）。
- 若 pack 输出用于 GPU 上传，需稳定的 `value_ptr()` / 布局叙事（v0.3–v0.4）。
- 若打包辅助返回或消费 `bvec` 掩码，需关系函数 / 布尔 `mix`。

### 3. 实现草图

- **位型转换：** 以 `std::bit_cast`（C++20+）薄封装，向量重载走现有 `apply_unary`。
- **Pack/unpack：** 按 GLSL 公式写标量参考实现；仅在规范定义处提供向量重载；位运算允许处做 constexpr。
- **§8.7 整数：** 纯整数函数对象；将未定义情形（如移位量）写成契约/`pre`，而非静默 UB。
- **不要** 在本库实现纹理/噪声/导数——除非项目获得着色运行时，否则文档化为永久范围外。

### 4. 工作量量级（粗估）

| 切片 | 粗估工作量 | 说明 |
|------|------------|------|
| 位型转换 + 测试 | 小 | 主要是 API 与 NaN 边界 |
| Unorm/Snorm/half pack 族 | 中 | 重载多；金标向量可来自 GLSL/CTS 或 GLM |
| Double/64 位 pack | 小–中 | 小心 `double` ↔ `uvec2` |
| 完整 §8.7 位域/进位 | 中–大 | 重载矩阵宽；契约设计 |
| 纹理/噪声/导数 | 不适用 | 除非改范围，否则拒绝 |

### 5. 建议的重开触发条件

仅在有具体消费方需要 CPU 侧 pack 或位域对齐时再议（例如共享的 CPU/GPU 材质编码，或必须与着色器 pack 比特精确一致的工具链）。在此之前，ROADMAP 继续专注变换、光照向量与矩阵代数所用的数学。
