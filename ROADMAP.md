# M3Lib Roadmap

> Based on the roundtable review of v0.1.0 (2026-06-07)

## v0.1.1 (2026-06-14) — Hotfix

**Goal:** Upgrade the project from "personal experiment" to "discoverable open-source project."

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
- Green CI matrix across all platforms (Linux/macOS/Windows × GCC/Clang/MSVC)
- Complete docs site + community governance model
- API freeze; subsequent changes are minor-version additive only

> ⚠️ **Critical external dependency:** C++26 IS publication date. If ISO publishes later than 2026-10, v1.0.0 slips accordingly.
