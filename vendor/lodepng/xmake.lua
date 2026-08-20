-- lodepng -- PNG codec, single translation unit.
--
-- LODEPNG_NO_COMPILE_CPP is public because it changes what lodepng.h declares
-- and must therefore match at every include site. The file is named .cpp
-- upstream but is C, hence silencing rather than fixing the warnings.

target("lodepng")
    set_kind("static")
    set_group("vendor")
    set_default(false)
    set_languages("cxx14")

    add_includedirs(".", {public = true})
    add_files("lodepng.cpp")

    add_defines("LODEPNG_NO_COMPILE_CPP", {public = true})

    add_cxflags("/wd4996", "/wd4244", {tools = {"cl", "clang_cl"}})
    add_cxflags("-Wno-unused-parameter", {tools = {"gcc", "clang"}})

    add_headerfiles("lodepng.h")
target_end()
