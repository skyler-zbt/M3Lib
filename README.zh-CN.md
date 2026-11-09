# M3Lib

[English](./README.md)

M3Lib（Modern · Module · Math）是一个模块化的 C++ 图形数学库，面向 CPU 侧的图形编程、游戏引擎与实时渲染。

- 向量和方阵类型，提供 GLSL 风格的向量别名
- 矩阵逐分量运算和浮点方阵显式求逆
- 通用、几何、三角和指数函数，以及基础变换函数

M3Lib 处于早期开发阶段，API、模块和行为可能变化。

## 平台与编译器

| 平台 | GCC | Clang | MSVC |
| --- | --- | --- | --- |
| Linux x64 | 曾在 Fedora 上测试（GCC 16） | 待验证 | — |
| Windows x64 | 构建已验证（MSYS2 UCRT64，GCC 16.2.0） | 待验证 | 待验证 |
| macOS | 待验证 | 待验证 | — |

## 构建

默认构建使用支持 C++26、C++ Modules 和 `import std` 的 GCC。

### xmake

仓库为 Linux x64 配置 GCC，为 Windows x64 配置 MinGW UCRT64。

```sh
xmake f -m debug
xmake build M3Lib
```

如果 xmake 在 Windows 上无法自动找到 MinGW，使用 `xmake f -m debug --mingw=<path-to-ucrt64>` 指定 SDK 路径。

### mcpp

使用 mcpp 2026.10.31.3 或更新版本，在仓库根目录构建库。 [mcpp 配置](./mcpp.toml)

```sh
mcpp build
```

使用已有的 GCC 安装时，传入 `--toolchain path:<gcc-install-dir>`，并确保 `PATH` 中可以找到 Ninja。

## 最小示例

在依赖 `M3Lib` 的目标中构建此程序。

```cpp
import m3;

int main() {
    const m3::vec3 v{1.0f, 2.0f, 3.0f};
    return m3::dot(v, v) == 14.0f ? 0 : 1;
}
```

## 文档

- [接口说明](./docs/API.md)
- [测试与性能基准](./tests/README.md)
- [开发总记录](./docs/DEV_HISTORY.md)
- [GLSL 4.60 规范](./docs/GLSLangSpec.4.60.pdf)

## 许可证

[Apache License 2.0](./LICENSE)
