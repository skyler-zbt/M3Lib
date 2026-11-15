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
| Windows x64 | Build and tests verified (MSYS2 UCRT64, GCC 16.2.0, Debug/Release) | Build and tests verified (MSYS2 CLANG64, Clang 23.1.5 + libc++, Debug) | Build and tests verified (MSVC 19.51, Release) |
| macOS | Unverified | Unverified | — |

Windows x64 builds and tests also pass with clang-cl 22.1.3 + MSVC STL in Debug mode. Some `<cmath>` calls still cannot be evaluated at compile time with the listed Clang/MSVC toolchains.

## Build

The default build uses GCC with support for C++26, C++ Modules, and `import std`.

### xmake

When no toolchain is specified, the repository defaults to GCC on Linux x64 and MinGW UCRT64 on Windows x64. Use xmake's built-in `--toolchain` option to select another compiler.

```sh
xmake f -m debug
xmake build M3Lib
```

If xmake cannot find MinGW on Windows, pass the SDK path with `xmake f -m debug --mingw=<path-to-ucrt64>`.

Reconfigure when switching toolchains, then run `xmake build M3Lib`:

```sh
# Windows GCC (MSYS2 UCRT64)
xmake f -c -m debug --toolchain=mingw
# Linux GCC
xmake f -c -m debug --toolchain=gcc
# Clang with the GNU-style driver
xmake f -c -m debug --toolchain=clang
# Windows Clang with the MSVC ABI and flag style
xmake f -c -m debug --toolchain=clang-cl
# Windows MSVC
xmake f -c -m debug --toolchain=msvc
```

Compiler-specific requirements belong in the `on_config` branches in the [library target configuration](./m3/xmake.lua). GCC adds the `stdc++exp` link library, while Clang adds libc++ (link name `c++`); the clang-cl and MSVC branches are reserved for future settings. The Linux `-B/usr/bin` option also applies only to GCC. For additional flags on a particular configuration, pass `xmake f --cxxflags=... --cxflags=... --ldflags=...`.

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
