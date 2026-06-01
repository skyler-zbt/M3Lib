-- M3Lib: Modern · Module · Math — a modular graphics math library
-- Build: xmake >= 3.08     Docs: https://xmake.io/llms-full.txt

set_arch("x64")

add_rules("mode.debug", "mode.release")
--     xmake f -m debug     unoptimized, debug symbols
--     xmake f -m release   optimized

set_toolchains("gcc")

-- Prefer system binutils (Fedora as/ld 2.46) over xlings binutils 2.42.
-- add_cxflags("-B/usr/bin") tells GCC to look for as/ld in /usr/bin.
add_cxflags("-B/usr/bin")

-- std::simd, contracts P2900R14, and other C++26 experimental features
add_links("stdc++exp")
add_cxflags("-fcontracts")

set_languages("c++26")

set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

-- Generate compile_commands.json for clangd / IDE support
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

includes "tests"
includes "m3"
