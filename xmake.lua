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
add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")

-- Opt-in native CPU instruction tuning.
-- -march=native generates instructions specific to the build host's CPU
-- (e.g. AVX-512 on a Skylake-X machine), producing non-portable binaries
-- that may crash on older or different CPUs.  Use only for local development
-- or when the target machine matches the build machine exactly.
-- Enable with:  xmake f -m release --march=native
--
-- 可选的原生 CPU 指令调优。
-- -march=native 生成针对构建主机 CPU 的指令（如在 Skylake-X 上生成
-- AVX-512 指令），产生的二进制文件不可移植，可能在较旧或不同的 CPU
-- 上崩溃。仅用于本地开发或目标机器与构建机器完全一致时。
-- 启用方式：xmake f -m release --march=native
option("march", {default = "generic", description = "Target CPU microarchitecture (e.g. native, x86-64-v3, znver4)"})
if is_mode("release") and get_config("march") ~= "generic" then
    add_cxflags("-march=" .. get_config("march"), "-mtune=" .. get_config("march"))
end

set_languages("c++26")

set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

-- Generate compile_commands.json for clangd / IDE support
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

-- Package definition for add_requires("M3Lib") consumers.
-- Currently disabled: M3Lib is consumed via source-level inclusion,
-- not through xmake-repo / add_requires().  Re-enable when v0.2.0
-- is ready for package distribution.
--
-- 当前禁用：M3Lib 通过源码级引入而非 xmake-repo / add_requires() 消费。
-- 待 v0.2.0 准备就绪后重新启用。
-- package("M3Lib")
--     set_description("A modern C++ graphics math library")
--     set_license("Apache-2.0")
--     set_homepage("https://github.com/skyler-zbt/M3Lib")

--     on_install(function (package)
--         import("package.tools.xmake").install(package, {})
--     end)
--
--     on_test(function (package)
--         assert(package:check_cxxsnippets({test = [[
--             #include <cstddef>
--             #include <array>
--             #include <cmath>
--             template <int L, typename T>
--             struct Vec { std::array<T, L> data{}; };
--             template <int L, typename T>
--             T dot(const Vec<L, T>& a, const Vec<L, T>& b) {
--                 T r{};
--                 for (int i = 0; i < L; ++i) r += a.data[i] * b.data[i];
--                 return r;
--             }
--             Vec<3, float> a{{{{1.f, 2.f, 3.f}}}};
--             Vec<3, float> b{{{{4.f, 5.f, 6.f}}}};
--             float d = dot(a, b);
--             static_assert(sizeof(int) == 4);
--             return std::abs(d - 32.f) < 0.001f;
--         ]]}, {configs = {languages = "c++26"}}))
--     end)
-- end)

includes "tests"
includes "m3"
