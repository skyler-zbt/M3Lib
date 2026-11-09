# M3Lib

[简体中文](./README.zh-CN.md)

M3Lib (Modern · Module · Math) is a modular C++ graphics math library for CPU-side graphics programming, game engines, and real-time rendering.

- Vector and square matrix types, including GLSL-style vector aliases
- Component-wise matrix operations and explicit inversion of floating-point square matrices
- Common, geometric, trigonometric, and exponential functions, plus basic transform helpers

M3Lib is in early development. APIs, modules, and behavior may change.

## Platforms and compilers

| Platform | GCC | Clang | MSVC |
| --- | --- | --- | --- |
| Linux x64 | Previously tested on Fedora (GCC 16) | Unverified | — |
| Windows x64 | Build verified (MSYS2 UCRT64, GCC 16.2.0) | Unverified | Unverified |
| macOS | Unverified | Unverified | — |

## Build

The default build uses GCC with support for C++26, C++ Modules, and `import std`.

### xmake

The repository configures GCC on Linux x64 and MinGW UCRT64 on Windows x64.

```sh
xmake f -m debug
xmake build M3Lib
```

If xmake cannot find MinGW on Windows, pass the SDK path with `xmake f -m debug --mingw=<path-to-ucrt64>`.

### mcpp

From the repository root, build the library with mcpp 2026.10.31.3 or later. [mcpp configuration](./mcpp.toml)

```sh
mcpp build
```

To use an existing GCC installation, pass `--toolchain path:<gcc-install-dir>` and make Ninja available on `PATH`.

## Quick example

Build this program in a target that depends on `M3Lib`.

```cpp
import m3;

int main() {
    const m3::vec3 v{1.0f, 2.0f, 3.0f};
    return m3::dot(v, v) == 14.0f ? 0 : 1;
}
```

## Documentation

- [API guide](./docs/API.md)
- [Testing and benchmarks](./tests/README.md)
- [Development history](./docs/DEV_HISTORY.md)
- [GLSL 4.60 specification](./docs/GLSLangSpec.4.60.pdf)

## License

[Apache License 2.0](./LICENSE)
