# M3Lib

> **M3Lib** (Modern · Module · Math) — a modular C++ graphics math library.

[简体中文](./README.zh-CN.md)

---

## What

M3Lib provides GLSL-oriented vector and matrix types and math helpers for graphics programming, game engines, and real-time rendering. It is built with **C++ Modules** and aims to be a modern, modular alternative to [GLM](https://github.com/icaven/glm).

Import the library with:

```cpp
import m3;
```

---

## Why

- **Modules-first:** designed around C++ Modules rather than a header-only stack.
- **GLSL-aligned semantics:** names and behaviour follow [GLSL 4.60](./docs/GLSLangSpec.4.60.pdf) where practical, so CPU math stays close to shader math.
- **Modern C++26:** contracts for bounds checks, and room to grow with newer language features.
- **Clear layout story:** explicit alignment qualifiers and a documented path toward GPU layout types (see roadmap).

---

## Status

> [!CAUTION]
> Early development. APIs, modules, and behaviour may change. Not a stable release.

| Question | Answer |
|----------|--------|
| **Can I build and use it?** | **Yes**, as an experimental library: build `M3` and `import m3;` on a supported toolchain. |
| **Is it production-ready?** | **No.** Correctness work remains (notably matrix product indexing in v0.3). Treat results with care until that lands. |
| **Are tests reliable?** | The `tests/` tree is due for a full rewrite; do not treat current tests as a completeness or correctness guarantee. |
| **Current surface** | `Vec` / square `Mat`, GLSL-style aliases and core trig / exp / common / geometric helpers, plus basic transform helpers. |

### Platforms & compilers

Requires **GCC 16+** (C++26 contracts / P2900) and C++ Modules support.

|          | GCC 16+ | Clang | MSVC |
|----------|---------|-------|------|
| Linux    | ✅ tested (e.g. Fedora) | 🚧 | ❌ |
| Windows  | ✅ tested (MSYS2 UCRT64) | 🚧 | 🚧 |
| macOS    | ❌ | ❌ | ❌ |

✅ usable today &nbsp;|&nbsp; 🚧 planned when toolchain support exists &nbsp;|&nbsp; ❌ not planned (no maintainer hardware for macOS)

---

## Build

Primary build: **xmake**. Optional: [mcpp](https://github.com/mcpp-community/mcpp) (best-effort).

```bash
xmake f -m debug          # or: xmake f -m release
xmake build M3            # build the library
```

---

## License

[Apache License 2.0](./LICENSE)

---

## Links

| GLSL 4.60 specification | [docs/GLSLangSpec.4.60.pdf](./docs/GLSLangSpec.4.60.pdf) |
|--|--|
| GLM | https://github.com/icaven/glm |
| xmake | https://github.com/xmake-io/xmake |
| mcpp | https://github.com/mcpp-community/mcpp |
