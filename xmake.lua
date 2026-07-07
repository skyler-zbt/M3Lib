-- M3Lib: Modern · Module · Math — modular graphics math library.
-- Top-level xmake.lua: only the build-mode switch and sub-project includes.
-- All toolchain / language / target configuration lives in m3/ and tests/.

-- M3Lib：Modern · Module · Math —— 模块化的图形学数学库。
-- 顶层 xmake.lua：仅包含构建模式开关与子项目引用。
-- 所有工具链 / 语言 / target 配置均位于 m3/ 与 tests/。

add_rules("mode.debug", "mode.release")

-- regenerate compile_commands.json on every build.
-- 每次构建时重新生成 compile_commands.json。
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

includes("m3")
includes("tests")