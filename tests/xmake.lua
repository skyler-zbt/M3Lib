-- M3Lib test targets.  Debug-only: tests are not built in release.
-- Run `xmake build tests` to build all, or `xmake run test_<name>` to run.

-- M3Lib 测试 target。仅 debug 模式：release 模式不构建测试。
-- 运行 `xmake build tests` 构建全部，或 `xmake run test_<name>` 运行单个测试。

-- Tests need contracts enforced regardless of library build mode.
-- Inherits set_languages / module policies / stdc++exp link from m3/xmake.lua.

-- 测试无论库构建模式如何都需要 enforce contracts。
-- 继承 set_languages / module policies / stdc++exp link 自 m3/xmake.lua。
set_languages("c++26")
add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")

if is_mode("debug") then
    -- Shared files: test_common.cppm exposes the TestRunner / check /
    -- check_float_eq helpers used by every test.  handle_contract_violation
    -- is defined inline in each test_*.cpp that needs observe-mode
    -- verification (currently only test_contracts), keeping the build
    -- configuration minimal.

    -- 共享文件：test_common.cppm 暴露所有测试用的 TestRunner / check /
    -- check_float_eq 辅助函数。handle_contract_violation 在需要验证
    -- observe 模式的各 test_*.cpp 内联定义（目前仅 test_contracts），
    -- 保持构建配置最小化。

    target("test_vec")
        set_kind("binary")
        add_deps("M3")
        add_files("test_vec.cpp", "test_common.cppm", {public = true})

    target("test_math")
        set_kind("binary")
        add_deps("M3")
        add_files("test_math.cpp", "test_common.cppm",  {public = true})

    target("test_cxx26")
        set_kind("binary")
        add_deps("M3")
        add_files("test_cxx26.cpp", "test_common.cppm", {public = true})

    target("test_mat")
        set_kind("binary")
        add_deps("M3")
        add_files("test_mat.cpp", "test_common.cppm", {public = true})

    -- observe semantic: violations log instead of abort, so the driver can
    -- verify the contract fired.
    --
    -- observe 语义：违反时记录日志而非终止，以便测试驱动验证契约是否触发。
    target("test_contracts")
        set_kind("binary")
        add_deps("M3")
        add_files("test_contracts.cpp", "test_common.cppm", {public = true})
        add_cxflags("-fcontract-evaluation-semantic=observe")
end