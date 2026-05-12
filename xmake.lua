-- using xmake version 3.08 or later

set_arch("x64")
add_rules("mode.debug", "mode.release")

set_toolchains("gcc")
add_links("stdc++exp")

set_languages("c++26")
set_policy("build.c++.modules", true)
set_policy("build.c++.modules.std", true)
set_policy("build.c++.modules.reuse", true)

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

includes "test"
includes "m3"