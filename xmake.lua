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

-- Enable native CPU instructions for release builds (AVX, AVX2, etc.)
-- Debug builds stay at -O0/-Og with baseline ISA for fast iteration.
if is_mode("release") then
    add_cxflags("-march=native", "-mtune=native")
end

set_languages("c++26")

set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

-- Generate compile_commands.json for clangd / IDE support
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

-- Package definition for add_requires("M3Lib") consumers.
-- xmake integration tests exercise both add_requires and includes paths.
package("M3Lib")
    set_description("A modern C++ graphics math library")
    set_license("Apache-2.0")
    set_homepage("https://github.com/skyler-zbt/M3Lib")

    on_install(function (package)
        import("package.tools.xmake").install(package, {})
    end)

    on_test(function (package)
        -- Basic sanity: verify M3Lib can be consumed as a package.
        -- Full integration tests live in integration/xmake/requires/.
        assert(package:check_cxxsnippets({test = [[
            #include <cstddef>
            static_assert(sizeof(int) == 4);
        ]]}, {configs = {languages = "c++26"}}))
    end)

includes "tests"
includes "m3"
