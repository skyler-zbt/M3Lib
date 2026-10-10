-- Test targets are available only in debug mode.
-- Build the suite with `xmake build tests`, or run one with `xmake run test_<name>`.

-- Tests need contracts enforced regardless of library build mode.
-- C++26 and stdc++exp are inherited through the M3 dependency. Module policies
-- and test-only contract flags are configured in this sub-project.
set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)
add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")

if is_mode("debug") then
    -- Shared files: test_common.cppm exposes the TestRunner / check /
    -- check_float_eq helpers used by every test.  handle_contract_violation
    -- is defined inline in each test_*.cpp that needs observe-mode
    -- verification (currently only test_contracts), keeping the build
    -- configuration minimal.

    -- target("test_vec")
    --     set_kind("binary")
    --     add_deps("M3Lib")
    --     add_files("test_vec.cpp", "test_common.cppm", {public = true})

end