-- M3 static library target — self-contained for downstream consumers.
-- Drop m3/ into another project and `includes("m3")` from its root xmake.lua.

-- M3 静态库 target —— 为下游使用者自包含。
-- 将 m3/ 复制到其他项目，并在其根 xmake.lua 中 `includes("m3")` 即可。

set_arch("x64")

-- GCC is the supported compiler (Linux + MinGW).
-- Clang / MSVC await contract + module support; see ROADMAP v0.6.

-- GCC 为当前受支持的编译器（Linux + MinGW）。
-- Clang / MSVC 仍在等待 contract 与 module 支持；详见 ROADMAP v0.6。
if is_host("linux") then
    set_toolchains("gcc")
end

if is_host("windows") then
    -- TODO(skyler): verify mingw[msystem=ucrt64] resolves stdc++exp and
    -- C++26 contracts on Windows MSYS2 ucrt64.  Pending manual test.

    -- TODO(skyler)：验证 mingw[msystem=ucrt64] 是否能解析 stdc++exp
    -- 与 C++26 contracts（Windows MSYS2 ucrt64 环境）。待手动测试。
    set_toolchains("mingw", {msystem = "ucrt64"})
end

-- C++26 contracts P2900R14 + experimental std features.
-- libstdc++exp ships with both system GCC (Linux) and MSYS2 MinGW GCC.

-- C++26 contracts (P2900R14) 与实验性 std 特性。
-- libstdc++exp 随系统 GCC（Linux）与 MSYS2 MinGW GCC 一同提供。
add_links("stdc++exp")
if is_host("linux") then
    -- Prefer Fedora system binutils over xlings 2.42 to avoid as/ld mismatches.
    -- 优先使用 Fedora 系统 binutils，避免与 xlings 2.42 出现 as/ld 版本不匹配。
    add_cxflags("-B/usr/bin")
end

-- Contracts: enforced in debug for runtime safety, stripped in release for
-- zero-overhead binaries.  Downstream tests are expected to enforce separately.

-- Contracts：debug 模式下 enforce 以提供运行时安全保证；
-- release 模式关闭以获得零开销的二进制。下游测试需自行 enforce。
if is_mode("debug") then
    add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")
end

set_languages("c++26")
set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

target("M3")
    set_kind("static")
    add_files("**.cppm", {public = true})