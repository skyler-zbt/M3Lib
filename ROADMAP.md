# M3Lib Roadmap

> Based on the roundtable review of v0.1.0 (2026-06-07)

## v0.1.1 (2026-06-14) — Hotfix

**Goal:** Upgrade the project from "personal experiment" to "discoverable
open-source project."

- [x] `git tag v0.1.0`
- [x] `CHANGELOG.md` (Keep a Changelog 1.1.0 format)
- [x] `CONTRIBUTING.md`
- [x] `.github/ISSUE_TEMPLATE/`
- [x] `CODE_OF_CONDUCT.md` (Contributor Covenant 2.1)
- [x] `NOTICE` (Apache-2.0 requirement)
- [x] `operator[]` `[[assume]]` transformation (auto-vectorisation unlock)
- [x] `element_ref_t<V>` trait (Matrix `apply_binary` reuse prep)
- [x] Source comment honesty fixes (4 places)
- [x] `-march=native` opt-in (`--march` option)
- [x] README honesty (platform, features, mcpp status)

## v0.2.0 (2026-09)

**Goal:** Upgrade from "vector library" to "graphics math library."

### Matrix types

- `Mat<3,3>` and `Mat<4,4>` templates (GLSL column-major)
- Basic matrix operations (`+ - *` MM/MV/VM, `== !=`)
- Matrix × scalar, compound assignment
- `transpose`, `inverse`, `determinant`
- `identity`, `scale`, `rotate` (single-axis), `translate` factories
- Matrix `operator[]` returning column reference

### GLSL math function completion

- Utility: `abs sign floor ceil fract mod step smoothstep min max clamp`
- Trig: `sin cos tan asin acos atan atan2 radians degrees`
- Exponential: `pow exp log exp2 log2 sqrt inversesqrt`
- Per-component vector versions, scalar versions

### Multi-character swizzle

- `.xy() .xyz() .rgb() .bgr()` etc. (at minimum all 2D/3D permutations)
- Correct move and copy semantics

### API cleanup

- `detail::` public signature elimination (`m3::Qualifier` alias re-export)
- `import std;` missing-file completion

### CI/CD upgrade

- mcpp CI tiered coverage (`mcpp-verify` job)
- ULP floating-point comparison migration
- Contracts ignore-mode testing
- NaN/Inf boundary testing

### mcpp decision point

- Evaluate mcpp viability: fix remaining build issues or officially deprecate.
  The v0.1.1 README marks mcpp as community-maintained/best-effort; v0.2.0
  must decide whether to invest in full mcpp CI or remove mcpp support.

## v0.3.0 (2026-12)

**Goal:** Complete 3D transform pipeline support.

- `Quat` quaternion type (GLSL style: `angleAxis/lookRotation/euler/slerp`)
- `Transform` pipeline (TRS decompose/compose)
- 3D projection matrices (`perspective/ortho/frustum`)
- Camera helper (`lookAt`)
- `faceforward` function
- Property-based testing framework
- Performance regression tests (benchmark suite)

## v0.5.0 (2027-03)

**Goal:** Production-ready, ecosystem-positioned.

- `std::simd` integration (first batch: `Vec<3,float>` SSE/AVX)
- Cross-platform build (Windows MSVC/Clang, macOS Apple Clang)
- xmake-repo package publication
- Docker standardised build images
- Documentation site (API reference + migration guide + tutorials)
- Type aliases `vec2/vec3/vec4/ivec3/mat3/mat4`

## v1.0.0 (2027-04 ± 1Q)

**Goal:** Stable API + ABI commitment.

- 6 months after C++26 final IS publication (core dependency stabilisation)
- ABI compatibility commitment (within same GCC major version)
- Green CI matrix across all platforms (Linux/macOS/Windows ×
  GCC/Clang/MSVC)
- Complete docs site + community governance model
- API freeze; subsequent changes are minor-version additive only

> ⚠️ **Critical external dependency:** C++26 IS publication date.  If ISO
> publishes later than 2026-10, v1.0.0 slips accordingly.

---

# M3Lib 路线图

> 基于 v0.1.0 圆桌评审（2026-06-07）

## v0.1.1 (2026-06-14) — 热修复

**目标：** 将项目从"个人实验"升级为"可发现的开源项目"。

- [x] `git tag v0.1.0`
- [x] `CHANGELOG.md`（Keep a Changelog 1.1.0 格式）
- [x] `CONTRIBUTING.md`
- [x] `.github/ISSUE_TEMPLATE/`
- [x] `CODE_OF_CONDUCT.md`（Contributor Covenant 2.1）
- [x] `NOTICE`（Apache-2.0 要求）
- [x] `operator[]` `[[assume]]` 改造（解锁自动向量化）
- [x] `element_ref_t<V>` trait（Matrix 复用 `apply_binary` 准备）
- [x] 源码注释诚实修复（4 处）
- [x] `-march=native` 可选化（`--march` 选项）
- [x] README 诚实化（平台、功能、mcpp 状态）

## v0.2.0 (2026-09)

**目标：** 从"向量库"升级为"图形数学库"。

### 矩阵类型

- `Mat<3,3>` 和 `Mat<4,4>` 模板（GLSL 列主序）
- 矩阵基本运算（`+ - *` MM/MV/VM、`== !=`）
- 矩阵 × 标量、复合赋值
- `transpose`、`inverse`、`determinant`
- `identity`、`scale`、`rotate`（单轴）、`translate` 工厂函数
- 矩阵 `operator[]` 返回列引用

### GLSL 数学函数补齐

- 常用：`abs sign floor ceil fract mod step smoothstep min max clamp`
- 三角：`sin cos tan asin acos atan atan2 radians degrees`
- 指数：`pow exp log exp2 log2 sqrt inversesqrt`
- 逐分量向量版、标量版

### 多字符 Swizzle

- `.xy() .xyz() .rgb() .bgr()` 等（至少二维/三维全排列）
- 正确的移动和拷贝语义

### API 清理

- `detail::` 公开签名消除（`m3::Qualifier` 别名重导出）
- 补齐遗漏的 `import std;`

### CI/CD 升级

- mcpp CI 阶梯覆盖（`mcpp-verify` job）
- ULP 浮点比较迁移
- Contracts ignore 模式测试
- NaN/Inf 边界测试

### mcpp 决策点

- 评估 mcpp 可行性：修复余下构建问题或正式弃用。v0.1.1 的 README
  将 mcpp 标记为社区维护/尽力支持；v0.2.0 必须决定是投入完整的 mcpp
  CI 还是移除 mcpp 支持。

## v0.3.0 (2026-12)

**目标：** 支持完整 3D 变换管线。

- `Quat` 四元数类型（GLSL 风格：`angleAxis/lookRotation/euler/slerp`）
- `Transform` 管线（TRS 分解/组合）
- 3D 投影矩阵（`perspective/ortho/frustum`）
- 相机辅助函数（`lookAt`）
- `faceforward` 函数
- Property-based testing 框架
- 性能回归测试（benchmark suite）

## v0.5.0 (2027-03)

**目标：** 生产就绪，生态就位。

- `std::simd` 集成（第一批：`Vec<3,float>` SSE/AVX）
- 跨平台构建（Windows MSVC/Clang、macOS Apple Clang）
- xmake-repo 包发布
- Docker 标准化构建镜像
- 文档站（API 参考 + 迁移指南 + 教程）
- 类型别名 `vec2/vec3/vec4/ivec3/mat3/mat4`

## v1.0.0 (2027-04 ± 1Q)

**目标：** 稳定 API + ABI 承诺。

- C++26 正式标准发布后 6 个月（核心依赖稳定）
- ABI 兼容承诺（同 GCC 大版本内）
- 全平台 CI 绿色矩阵（Linux/macOS/Windows × GCC/Clang/MSVC）
- 完整文档站 + 社区治理模型
- API 冻结，后续仅小版本加法

> ⚠️ **关键外部依赖：** C++26 正式标准发布日期。若 ISO 发布晚于
> 2026-10，v1.0.0 顺延。
