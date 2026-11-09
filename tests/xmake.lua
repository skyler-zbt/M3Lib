-- C++26 and toolchain-specific link libraries are inherited from M3Lib.
set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)
-- Contracts are paused pending the final ISO specification.
-- add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")

-- Run in either debug or release: xmake run test_matrix
target("test_matrix")
    set_kind("binary")
    set_default(false)
    add_deps("M3Lib")
    add_files("test_matrix.cpp")
    add_includedirs("third_party/glm")
    add_tests("matrix")

for _, name in ipairs({"arithmetic", "common_math", "geometry", "transcendental", "division_output", "inverse"}) do
    target("test_" .. name)
        set_kind("binary")
        set_default(false)
        add_deps("M3Lib")
        add_files("test_" .. name .. ".cpp")
        add_includedirs("third_party/glm")
        add_tests(name)
        if name == "arithmetic" then
            for _, guard in ipairs({"matrix-divide-zero", "matrix-scalar-divide-zero", "matrix-negate-min"}) do
                add_tests(guard, {runargs = {"--" .. guard}, should_fail = true})
            end
        end
        if name == "inverse" then
            for _, invalid in ipairs({"singular", "zero", "nan", "infinity", "negative-infinity", "overflow"}) do
                add_tests("invalid-" .. invalid, {
                    runargs = {"--invalid", invalid}, should_fail = true
                })
            end
        end
        if name == "division_output" then
            for _, policy in ipairs({"vector-bounds", "matrix-column-bounds", "matrix-row-bounds", "vector-negate-min"}) do
                add_tests("policy-" .. policy, {runargs = {"--policy", policy}, should_fail = true})
            end
            for dimension = 1, 4 do
                for lane = 0, dimension - 1 do
                    local kind = lane % 2 == 0 and "vv" or "vv-assign"
                    add_tests("zero-" .. dimension .. "-" .. lane, {
                        runargs = {"--zero", kind, tostring(dimension), tostring(lane), tostring(lane % 2)},
                        should_fail = true
                    })
                end
            end
            for _, case in ipairs({
                {"vs", "3", "0", "1"}, {"vs-assign", "4", "0", "0"},
                {"mm", "3", "8", "1"}, {"ms-assign", "2", "0", "0"},
                {"into", "3", "2", "1"}
            }) do
                add_tests("zero-" .. case[1], {
                    runargs = {"--zero", case[1], case[2], case[3], case[4]}, should_fail = true
                })
            end
        end
end

-- Performance runs are explicit and serial, separate from parallel correctness tests.
if is_mode("release") then
    target("benchmark_glm")
        set_kind("binary")
        set_default(false)
        add_deps("M3Lib")
        add_files("benchmark_glm.cpp", "support/benchmark_platform.cpp")
        add_includedirs("third_party/glm")
end
