-- M3Lib: Modern · Module · Math — a modular graphics math library
-- Build: xmake >= 3.08     Docs: https://xmake.io/llms-full.txt

set_arch("x64")

add_rules("mode.debug", "mode.release")
--     xmake f -m debug     unoptimized, debug symbols
--     xmake f -m release   optimized

set_toolchains("gcc")

-- std::simd and other C++26 experimental features live in libstdc++exp
add_links("stdc++exp")

set_languages("c++26")

set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

-- Generate compile_commands.json for clangd / IDE support
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

includes "tests"
includes "m3"
