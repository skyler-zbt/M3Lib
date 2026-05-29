-- M3Lib test target
--    xmake build tests && xmake run test_vec

target("test_vec")
    set_kind("binary")
    add_deps("M3")
    add_files("test_vec.cpp")
    set_languages("c++26")
    set_policy("build.c++.modules", true)
    set_policy("build.c++.modules.std", true)
