# M3Lib

> M3Lib (Modern · Module · Math) is a modular graphics mathematics library.

[简体中文](./README.zh-CN.md)

M3Lib aims to become a "modern, modular GLM" designed for graphics programming, game engines, and real-time rendering, while providing a mathematical foundation that better aligns with modern C++ design philosophies.

---

## Project Status

> [!CAUTION]
> Early development — The project is in active development.
> APIs, module structures, behaviours, and implementation details may change.
> Issues, feedback, and ideas are welcome through GitHub Issues.

**Currently available:**
- Vector types `Vec<1..4, T>` with GLSL-style single-component swizzle (.x, .y, .z, .w, .r, .g, .b, .a, .s, .t, .p, .q)
- Vec arithmetic operators (`+ - * /` element-wise, scalar broadcast, compound assignment, `== !=`)
- Square matrix types `Mat<2..4, T>` with column-major layout, plus `Mat2`/`Mat3`/`Mat4` aliases
- Matrix operators (`+ - *` matrix-matrix and matrix-vector, `== !=`, compound assignment)
- Core GLSL vector math functions (dot, cross, normalize, length, distance, reflect, refract)
- Core GLSL common functions (mix, clamp, lerp)
- Contracts-based bounds checking (C++26 P2900R14)
- C++26 `=delete("reason")` diagnostics
- `constexpr` structured bindings (C++26 P2686R4)

**Planned (see [Roadmap](./ROADMAP.md)):**
- GLSL math function completion (v0.2)
- Type aliases `vec2 vec3 vec4 mat3 mat4` (v0.2)
- Multi-character swizzle (v0.3)
- Quaternion support (v0.4)
- Cross-platform support (v0.6)

---

## Goals

M3Lib aims to become:

- A modern C++ mathematics library built entirely around C++ Modules
- A GLM alternative that better fits modern C++26 design principles

Compared with traditional math libraries, M3Lib places greater emphasis on:

- Full C++ Modules support
- GLSL-style interfaces and behaviours
- Clear and consistent API design
- Strong type safety
- Proper memory alignment design
- Utilisation of modern SIMD and compiler optimisation capabilities
- GPU / shader-oriented mathematical semantics

Whenever practical, M3Lib follows GLSL specifications and behaviours to reduce differences between CPU-side and GPU-side mathematical code, providing developers with a more consistent programming experience.

---

## Requirements

M3Lib relies on **C++26 contracts (P2900R14)**, currently implemented
only in **GCC 16+**.  Static reflection (P2996) is reserved for future
API work and is not yet consumed by the library.  Other compilers
(Clang, MSVC) lag behind on contracts; full multi-compiler support awaits
their implementation.

- **Compiler:** GCC 16 or newer (required for contracts)
- Full C++ Modules support
- **Currently tested platform:** Linux x64 (Fedora + GCC 16)
- **Windows:** planned for testing; not yet validated
- **macOS:** not planned (no maintainer hardware available)

### Platform & Compiler Support

|          | GCC       | Clang     | MSVC      |
|----------|-----------|-----------|-----------|
| Linux    | ✅        | 🚧        | ❌        |
| Windows  | 🚧        | 🚧        | 🚧        |
| macOS    | ❌        | ❌        | ❌        |

✅ Supported &nbsp;|&nbsp; 🚧 Planned &nbsp;|&nbsp; ❌ Not planned
---

## Build

M3Lib supports two build systems:

### xmake (primary, CI-covered)

```bash
xmake f -m debug                   # debug: unoptimised, fast iteration
xmake f -m release                 # release: optimised, portable
xmake f -m release --march=native  # release: host-CPU-optimised (non-portable)
xmake build M3                     # build library
xmake build tests                  # build + run all tests
xmake run test_vec
```

### mcpp (community-maintained, best-effort)

[mcpp](https://github.com/mcpp-community/mcpp) — a modern modular build tool with built-in toolchain management.

```bash
mcpp build              # build library
mcpp build -p test_vec  # build tests
./target/*/bin/test_vec # run tests
```

> **Note:** The mcpp build is community-maintained.  Not all test targets are
> guaranteed to build with mcpp.  mcpp CI coverage is planned for v0.6.

---

## Running Tests

```bash
# xmake
xmake build tests && xmake run test_vec

# mcpp
mcpp build -p test_vec && ./target/*/bin/test_vec
```

The current test suite covers construction, accessors, operators, compound
assignment, boundary conditions, contracts, structured bindings, and
`=delete("reason")` diagnostics.  Test count is not reported here because
the API and feature surface are still evolving rapidly during early
development.

---

## License

This project is licensed under the Apache License 2.0.

See [LICENSE](./LICENSE) for details.

---

## Related Links

- [OpenGL Mathematics (GLM)](https://github.com/icaven/glm) — a header-only C++ mathematics library for graphics software based on the GLSL specifications.
- [xmake](https://github.com/xmake-io/xmake) — a cross-platform build utility based on Lua.
- [mcpp](https://github.com/mcpp-community/mcpp) — a modern modular build tool with built-in toolchain management.
