-- Contracts are paused pending the final ISO specification.
-- if is_mode("debug") then
--     add_cxflags("-fcontracts", "-fcontract-evaluation-semantic=enforce")
-- end

-- Keep the library's requirements on its target. Public settings are inherited
-- by targets that depend on M3Lib; host architecture and toolchain stay external.
target("M3Lib")
    set_kind("static")
    set_languages("c++26", {public = true})

    on_config(function (target)
        if target:has_tool("cxx", "gcc", "gxx") then
            target:add("links", "stdc++exp", {public = true})
        end
    end)

    -- Enable module support for M3Lib's C++ module interface sources.
    set_policy("build.c++.modules", true)
    set_policy("build.c++.modules.std", true)
    set_policy("build.c++.modules.reuse", true)

    add_files("**.cppm", {public = true})
