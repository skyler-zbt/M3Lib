# M3Lib

> M3Lib（Modern · Module · Math）—— 一个模块化的图形学数学库。

[English](./README.md)

M3Lib 的目标是成为"现代化、模块化的 GLM"，为图形学、游戏引擎与实时渲染领域提供更符合现代 C++ 理念的数学基础设施。

---

## 项目状态

> [!CAUTION]
> 早期开发中（v0.1.x）—— 项目处于活跃开发阶段。
> 接口、模块结构、行为以及实现细节可能发生变动。
> 欢迎通过 Issues 提交问题、反馈或想法。

**当前可用：**
- 向量类型 `Vec<1..4, T>`，提供 GLSL 风格的单分量 swizzle（.x、.y、.z、.w、.r、.g、.b、.a、.s、.t、.p、.q）
- 核心 GLSL 向量数学函数（dot、cross、normalize、length、distance、reflect、refract）
- 核心 GLSL 通用函数（mix、clamp、lerp）
- 基于契约的边界检查（C++26 P2900R14）
- C++26 `=delete("reason")` 诊断信息
- `constexpr` 结构化绑定（C++26 P2686R4）
- 128 个测试用例（test_contracts 中 1 个因 GCC 16.1.0 ICE 跳过）

**计划中（详见[路线图](./ROADMAP.md)）：**
- 矩阵类型（v0.2.0）
- 多字符 swizzle（v0.2.0）
- 四元数支持（v0.3.0）
- C++ 标准 SIMD（`std::simd`）集成（v0.5.0）
- 跨平台支持（Windows/macOS）（v0.5.0）

---

## 项目目标

M3Lib 希望成为：

- 一个完全基于 Modules 架构的现代 C++ 数学库
- 更契合现代 C++26 设计思想的 GLM 替代方案

与传统数学库相比，M3Lib 更加注重：

- 完整的 C++ Modules 支持
- GLSL 风格的接口与行为规范
- 清晰且一致的 API 设计
- 强类型安全
- 合理的内存对齐设计
- 对现代 SIMD 及编译器优化能力的利用
- 贴近 GPU / Shader 风格的数学接口

在可行范围内，M3Lib 将尽可能遵循 GLSL 规范与行为，以减少 CPU 端与 GPU 端数学代码之间的差异，为开发者提供更一致的编码体验。

---

## 环境要求

- 支持 C++26 的编译器：**GCC 16+**（contracts P2900R14 和 modules 为必需）
- 完整的 C++ Modules 支持
- **平台：** Linux x64（在 Fedora + GCC 16 上测试通过）

### 平台与编译器支持

|          | GCC       | Clang     | MSVC      |
|----------|-----------|-----------|-----------|
| Linux    | ✅        | 🚧        | ❌        |
| Windows  | ❌        | 🚧        | 🚧        |
| macOS    | ❌        | 🚧        | ❌        |

✅ 已支持 &nbsp;|&nbsp; 🚧 规划中（v0.5.0） &nbsp;|&nbsp; ❌ 暂未计划
---

## 构建

M3Lib 支持双构建系统：

### xmake（主构建系统，CI 覆盖）

```bash
xmake f -m debug                   # debug：未优化，快速迭代
xmake f -m release                 # release：优化，可移植
xmake f -m release --march=native  # release：针对本机 CPU 优化（不可移植）
xmake build M3                     # 编译库
xmake build tests                  # 编译 + 运行全部测试
xmake run test_vec
```

### mcpp（社区维护，尽力支持）

[mcpp](https://github.com/mcpp-community/mcpp) —— 现代模块化构建工具，内置工具链管理。

```bash
mcpp build              # 编译库
mcpp build -p test_vec  # 编译测试
./target/*/bin/test_vec # 运行测试
```

> **注意：** mcpp 构建为社区维护，不保证所有测试目标可用 mcpp 构建。
> mcpp CI 覆盖计划在 v0.2.0 实现。

---

## 运行测试

```bash
# xmake
xmake build tests && xmake run test_vec

# mcpp
mcpp build -p test_vec && ./target/*/bin/test_vec
```

126 个测试用例，覆盖构造、访问器、运算符、复合赋值、边界情况、契约、
结构化绑定和 `=delete("reason")` 诊断。
（共 128 个用例；test_contracts 中 1 个因 GCC 16.1.0 ICE 跳过。）

---

## 许可证

本项目基于 Apache License 2.0 开源。

详见 [LICENSE](./LICENSE)。

---

## 相关链接

- [OpenGL Mathematics (GLM)](https://github.com/icaven/glm) —— 一个符合 GLSL 规范的 C++ 仅头文件数学库
- [xmake](https://github.com/xmake-io/xmake) —— 一个基于 Lua 的轻量级跨平台构建工具
- [mcpp](https://github.com/mcpp-community/mcpp) —— 现代模块化构建工具，内置工具链管理
