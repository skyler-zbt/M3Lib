-- M3Lib entry point. Keep repository-wide defaults and sub-project wiring here.
add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

-- These are defaults for this repository. The M3 target does not pin an
-- architecture or toolchain, allowing an embedding project to choose them.
set_arch("x64")

-- Select the supported compiler environment for builds of this repository.
if is_host("linux") then
    set_toolchains("gcc")
    -- Prefer system binutils over a separately installed toolchain's binutils.
    add_cxflags("-B/usr/bin")
elseif is_host("windows") then
    set_toolchains("mingw", {msystem = "ucrt64"})
end

includes("m3")
includes("tests")