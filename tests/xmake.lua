-- M3Lib test targets
--    xmake build tests         (all tests)
--    xmake run test_vec        (Vec type tests)
--    xmake run test_math       (math function tests)
--    xmake run test_cxx26      (C++26 feature tests)
--    xmake run test_contracts  (contract violation tests)

-- Shared configuration for all M3Lib test targets.
-- Applies C++26 standard, module policies, contracts flag, and experimental std link.
local function configure_m3_test_target(target_name)
    target(target_name)
        set_kind("binary")
        add_deps("M3")
        add_files(target_name .. ".cpp", "test_common.cppm", {public = true})
        set_languages("c++26")
        set_policy("build.c++.modules", true)
        set_policy("build.c++.modules.std", true)
        add_cxflags("-fcontracts")
        add_links("stdc++exp")
    target_end()
end

-- Vec type: construction, accessors, operators, comparison, formatter
configure_m3_test_target("test_vec")

-- Math functions: dot, cross, normalize, length, distance,
-- reflect, refract, mix, clamp, lerp
configure_m3_test_target("test_math")

-- C++26 features: structured bindings, tuple protocol, contracts valid access
configure_m3_test_target("test_cxx26")

-- Contract violations: pre(i < L) with observe semantic (violations log, don't terminate)
target("test_contracts")
    set_kind("binary")
    add_deps("M3")
    add_files("test_contracts.cpp", "test_common.cppm", {public = true})
    set_languages("c++26")
    set_policy("build.c++.modules", true)
    set_policy("build.c++.modules.std", true)
    add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=observe")
    add_links("stdc++exp")
    -- Suppress libstdc++ assertion competing with contract observe mode.
    -- GCC 16 enables _GLIBCXX_ASSERTIONS by default in non-optimized builds,
    -- causing std::array::operator[] to abort before contract survival is proven.
    -- TODO(contract-arch): revisit when GCC supports <contract> violation handler.
    --
    -- 抑制与合约 observe 模式竞争的标准库断言。
    -- GCC 16 默认在非优化构建中启用 _GLIBCXX_ASSERTIONS，
    -- 导致 std::array::operator[] 在合约存活验证完成前 abort。
    -- 待议(合约架构): 等 GCC 支持 <contract> violation handler 后重新评估。
    add_defines("_GLIBCXX_NO_ASSERTIONS")
target_end()
