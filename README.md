# M3Lib

> M3Lib (Modern · Module · Math) is a modular graphics mathematics library.

[简体中文](./README.zh-CN.md)

M3Lib aims to become a “modern, modular GLM” designed for graphics programming, game engines, and real-time rendering, while providing a mathematical foundation that better aligns with modern C++ design philosophies.

---

## Project Status

> [!CAUTION]
> Early development (WIP) — The project is still in the exploration and initial design stage.  
> APIs, module structures, behaviors, and implementation details may change in future versions.  
> Issues, feedback, and ideas are welcome through GitHub Issues.

---

## Goals

M3Lib aims to become:

- A modern C++ mathematics library built entirely around C++ Modules
- A GLM alternative that better fits modern C++26 design principles

Compared with traditional math libraries, M3Lib places greater emphasis on:

- Full C++ Modules support
- GLSL-style interfaces and behaviors
- Clear and consistent API design
- Strong type safety
- Proper memory alignment design
- Utilization of modern SIMD and compiler optimization capabilities
- GPU / shader-oriented mathematical semantics

Whenever practical, M3Lib follows GLSL specifications and behaviors to reduce differences between CPU-side and GPU-side mathematical code, providing developers with a more consistent programming experience.

---

## Planned Features

- Vector types (`vec2`, `vec3`, `vec4`)
- Matrix types (`mat3`, `mat4`, etc.)
- Quaternion support
- GLSL-style mathematical functions
- Support for C++ standard SIMD (`std::simd`)
- Memory alignment optimizations
- `constexpr`-friendly design
- Pure Modules architecture
- Cross-platform support

---

## Requirements

- A compiler with C++20/C++23 support (C++26 for future features)
- Full C++ Modules support (GCC 14+, Clang 17+, or MSVC 19.34+)

## Build

M3Lib supports dual build systems:

### xmake

```bash
xmake f -m debug
xmake build M3       # build library
xmake build tests    # build + run tests
xmake run test_vec
```

### mcpp

[mcpp](https://github.com/mcpp-community/mcpp) — a modern C++23 modular build tool.

```bash
mcpp build            # build library
mcpp build -p test_vec  # build tests
./target/*/bin/test_vec # run tests
```

## Running Tests

```bash
# xmake
xmake build tests && xmake run test_vec

# mcpp
mcpp build -p test_vec && ./target/*/bin/test_vec
```

54 test cases covering construction, accessors, operators, and edge cases.

---

## License

This project is licensed under the Apache License 2.0.

See [LICENSE](./LICENSE) for details.

---

## Related Links

- [glm](https://github.com/icaven/glm) — a header only C++ mathematics library for graphics software based on the GLSL specifications.
- [xmake](https://github.com/xmake-io/xmake) — a cross-platform build utility based on the Lua scripting language.
- [mcpp](https://github.com/mcpp-community/mcpp) — a modern C++23 modular build tool with built-in toolchain management.