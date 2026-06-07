# M3Lib Roadmap / M3Lib 路线图

> Based on the roundtable review of v0.1.0 (2026-06-07)
> 基于 v0.1.0 圆桌评审

## v0.1.1 (2026-06-14) — Hotfix / 热修复

**Goal:** Upgrade the project from "personal experiment" to "discoverable open-source project."

**目标：** 将项目从"个人实验"升级为"可发现的开源项目"。

- [x] `git tag v0.1.0`
- [x] `CHANGELOG.md` (Keep a Changelog 1.1.0 format)
- [x] `CONTRIBUTING.md`
- [x] `.github/ISSUE_TEMPLATE/`
- [x] `CODE_OF_CONDUCT.md` (Contributor Covenant 2.1)
- [x] `NOTICE` (Apache-2.0 要求)
- [x] `operator[]` `[[assume]]` transformation (auto-vectorisation unlock)
- [x] `element_ref_t<V>` trait (Matrix `apply_binary` reuse prep)
- [x] Source comment honesty fixes (4 places)
- [x] `-march=native` opt-in (`--march` option)
- [x] README honesty (platform, features, mcpp status)

## v0.2.0 (2026-09)

**Goal:** Upgrade from "vector library" to "graphics math library."

**目标：** 从"向量库"升级为"图形数学库"。

### Matrix types / 矩阵类型
- `Mat<3,3>` and `Mat<4,4>` templates (GLSL column-major)
- Basic matrix operations (`+ - *` MM/MV/VM, `== !=`)
- Matrix × scalar, compound assignment
- `transpose`, `inverse`, `determinant`
- `identity`, `scale`, `rotate` (single-axis), `translate` factories
- Matrix `operator[]` returning column reference

### GLSL math function completion / GLSL 数学函数补齐
- Utility / 常用: `abs sign floor ceil fract mod step smoothstep min max clamp`
- Trig / 三角: `sin cos tan asin acos atan atan2 radians degrees`
- Exponential / 指数: `pow exp log exp2 log2 sqrt inversesqrt`
- Per-component vector versions, scalar versions

### Multi-character swizzle / 多字符 Swizzle
- `.xy() .xyz() .rgb() .bgr()` etc. (at minimum all 2D/3D permutations)
- Correct move and copy semantics

### API cleanup / API 清理
- `detail::` public signature elimination (`m3::Qualifier` alias re-export)
- `import std;` missing-file completion

### CI/CD upgrade / CI/CD 升级
- mcpp CI tiered coverage (`mcpp-verify` job)
- ULP floating-point comparison migration
- Contracts ignore-mode testing
- NaN/Inf boundary testing

## v0.3.0 (2026-12)

**Goal:** Complete 3D transform pipeline support.

**目标：** 支持完整 3D 变换管线。

- `Quat` quaternion type (GLSL style: `angleAxis/lookRotation/euler/slerp`)
- `Transform` pipeline (TRS decompose/compose)
- 3D projection matrices (`perspective/ortho/frustum`)
- Camera helper (`lookAt`)
- `faceforward` function
- Property-based testing framework
- Performance regression tests (benchmark suite)

## v0.5.0 (2027-03)

**Goal:** Production-ready, ecosystem-positioned.

**目标：** 生产就绪，生态就位。

- `std::simd` integration (first batch: `Vec<3,float>` SSE/AVX)
- Cross-platform build (Windows MSVC/Clang, macOS Apple Clang)
- xmake-repo package publication
- Docker standardised build images
- Documentation site (API reference + migration guide + tutorials)
- Type aliases `vec2/vec3/vec4/ivec3/mat3/mat4`

## v1.0.0 (2027-04 ± 1Q)

**Goal:** Stable API + ABI commitment.

**目标：** 稳定 API + ABI 承诺。

- 6 months after C++26 final IS publication (core dependency stabilisation)
- ABI compatibility commitment (within same GCC major version)
- Green CI matrix across all platforms (Linux/macOS/Windows × GCC/Clang/MSVC)
- Complete docs site + community governance model
- API freeze; subsequent changes are minor-version additive only

> ⚠️ **Critical external dependency / 关键外部依赖:** C++26 IS publication date. If ISO publishes later than 2026-10, v1.0.0 slips accordingly.
> 若 C++26 正式标准发布晚于 2026-10，v1.0.0 顺延。
