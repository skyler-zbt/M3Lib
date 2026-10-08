# M3Lib

> **M3Lib**（Modern · Module · Math）—— 模块化的 C++ 图形数学库。

[English](./README.md)

---

## 是什么

M3Lib 提供面向 GLSL 的向量、矩阵类型与数学辅助，用于图形编程、游戏引擎与实时渲染。库以 **C++ Modules** 构建，目标是成为现代化、模块化的 [GLM](https://github.com/icaven/glm) 替代方案。

使用方式：

```cpp
import m3;
```

---

## 为什么

- **Modules 优先：** 围绕 C++ Modules 设计，而不是仅头文件堆叠。
- **对齐 GLSL 语义：** 在可行范围内遵循 [GLSL 4.60](./docs/GLSLangSpec.4.60.pdf) 的命名与行为，让 CPU 数学贴近着色器数学。
- **现代 C++26：** 用契约做边界检查，并为后续语言特性留出空间。
- **布局叙事清晰：** 显式对齐 qualifier，以及通向 GPU 布局类型的规划（见路线图）。

---

## 当前进度

> [!CAUTION]
> 早期开发。API、模块与行为仍可能变动。不是稳定发行版。

| 问题 | 回答 |
|------|------|
| **能不能编译、能不能用？** | **能**，按实验库使用：在受支持工具链上构建 `M3` 并 `import m3;`。 |
| **能不能上生产？** | **不能。** 仍有正确性工作。在修好之前请谨慎采信计算结果。 |
| **测试可靠吗？** | `tests/` 计划整目录重写；请勿把当前测试当作完整或正确性保证。 |
| **当前能力范围** | `Vec` / 方阵 `Mat`、GLSL 风格别名与核心三角 / 指数 / 通用 / 几何辅助，以及基础变换辅助。 |

### 平台与编译器

需要 **GCC 16+**（C++26 contracts / P2900）及 C++ Modules 支持。

|          | GCC 16+ | Clang | MSVC |
|----------|---------|-------|------|
| Linux    | ✅ 已测（如 Fedora） | 🚧 | ❌ |
| Windows  | ✅ 已测（MSYS2 UCRT64） | 🚧 | 🚧 |
| macOS    | ❌ | ❌ | ❌ |

✅ 今天可用 &nbsp;|&nbsp; 🚧 待工具链跟上后规划 &nbsp;|&nbsp; ❌ 暂未计划（macOS 无维护者硬件）

---

## 构建

主构建：**xmake**。可选：[mcpp](https://github.com/mcpp-community/mcpp)（尽力支持）。

```bash
xmake f -m debug          # 或：xmake f -m release
xmake build M3            # 构建库
```

---

## 许可证

[Apache License 2.0](./LICENSE)

---

## 相关链接

| GLSL 4.60 规范 | [docs/GLSLangSpec.4.60.pdf](./docs/GLSLangSpec.4.60.pdf) |
|--|--|
| GLM | https://github.com/icaven/glm |
| xmake | https://github.com/xmake-io/xmake |
| mcpp | https://github.com/mcpp-community/mcpp |
