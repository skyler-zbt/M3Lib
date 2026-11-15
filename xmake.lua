-- M3Lib entry point. Keep repository-wide defaults and sub-project wiring here.
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

-- These are defaults for this repository. The M3 target does not pin an
-- architecture or toolchain, allowing an embedding project to choose them.
set_arch("x64")

-- Use repository defaults only when no toolchain is configured.
if not get_config("toolchain") then
    if is_plat("linux") then
        set_toolchains("gcc")
    elseif is_plat("windows") then
        set_toolchains("mingw", {msystem = "ucrt64"})
    end
end

if is_plat("linux") then
    -- Prefer system binutils only for GCC, including explicitly selected GCC.
    add_cxflags("-B/usr/bin", {tools = {"gcc", "gxx"}})
end

includes("m3")
includes("tests")
