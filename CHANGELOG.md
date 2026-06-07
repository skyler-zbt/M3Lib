# Changelog

All notable changes to M3Lib will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.1] — 2026-06-07

### Fixed
- `operator[]` hot-path: replaced runtime `if (i >= L) std::abort()` with `if consteval` + `[[assume]]` to unlock auto-vectorisation (16–20× performance gap on element-wise loops)
- Four source-comment inaccuracies: Vec<5+> support claim, unreachable generic-loop fallback, "all operators delegate" (operator== doesn't), per-specialisation swizzle set description

### Added
- `element_ref_t<V>` trait in `m3.detail.concepts` — decouples `apply_binary`/`apply_unary` from Vec-specific `operator[]` semantics, enabling future Matrix reuse
- `--march` xmake option — `-march=native`/`-mtune=native` now opt-in (`xmake f -m release --march=native`), defaulting to portable `generic`
- Project governance: `CHANGELOG.md`, `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `NOTICE`, GitHub Issue templates

### Changed
- README: honest platform/feature status — "Linux x64 (GCC 16+)" instead of "Cross-platform", mcpp marked "community-maintained", Matrix/quaternion moved to Planned, current features listed explicitly
- `xmake.lua`: `-march=native` disabled by default for distribution safety

## [0.1.0] — 2026-05-10 (first functional tag)

### Added
- Vector types `Vec<1..4, T>` with GLSL-style single-component swizzle accessors
- Core GLSL math functions: normalize, dot, cross, length, distance, abs, sign, floor, ceil, fract, mod, clamp, mix, step, smoothstep, min, max, sin, cos, tan, asin, acos, atan, atan2, radians, degrees, pow, exp, log, exp2, log2, sqrt, inversesqrt, reflect, refract
- Contracts-based bounds checking (C++26 P2900R14)
- C++26 `=delete("reason")` diagnostics (P2573R2)
- `constexpr` structured bindings (C++26 P2686R4)
- Safety features: `Div` zero-check, `clamp` argument validation, `normalize` zero-vector defence
- `std::formatter` for `Vec`
- Dual build system: xmake (primary) + mcpp (community)
- GitHub Actions CI (lint, enforce, test matrix)
- 126 test cases
