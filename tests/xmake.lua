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
target_end()

-- Boundary tests: OOB access in enforce mode (must abort).
-- Verifies the contract pre-condition on VectorBase::operator[].
-- This is the P0 regression suite for the v0.1.1 [[assume]] change.
--
-- 边界测试：enforce 模式下越界访问（必须 abort）。
-- 验证 VectorBase::operator[] 的契约 pre 条件。
-- 这是 v0.1.1 [[assume]] 改造的 P0 回归测试套件。
configure_m3_test_target("test_boundary")

-- Mat type: construction, accessors, operators, comparison, products.
-- S1.7 — Mat core type and basic operations.
--
-- Mat 类型：构造、访问器、运算符、比较、乘法。
-- S1.7——Mat 核心类型与基本运算。
configure_m3_test_target("test_mat")
