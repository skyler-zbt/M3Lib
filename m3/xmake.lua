-- M3 static library target
-- Root module: m3/m3.cppm — aggregates detail + vector submodules

target("M3")
    set_kind("static")

    -- All .cppm files are public interface units (consumers import them)
    add_files("**.cppm", {public = true})

    -- Uncomment when adding implementation (.cpp) files:
    -- add_files("**.cpp")
