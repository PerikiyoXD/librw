-- librw
--
--   xmake f --backend=gl3 --gfxlib=glfw
--   xmake f --backend=d3d9
--   xmake f --lodepng=xrepo
--   xmake
--   xmake install -o /usr/local
--
-- xmake/matrix.lua holds the backend/platform matrix and drives both the
-- option menus and the per-target configuration.

set_project("librw")
set_xmakever("2.9.0")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")
set_languages("c11", "cxx14")
set_allowedmodes("debug", "release")

includes("xmake/matrix.lua")

option("backend")
    set_default("null")
    set_values(librw_backend_values())
    set_description("Rendering backend",
                    "  Choices are filtered to those the target platform can build.")
    set_showmenu(true)
option_end()

option("gfxlib")
    set_default("glfw")
    set_values(LIBRW_GFXLIB_ORDER)
    set_description("Window/context library (GL backends only)")
    set_showmenu(true)
option_end()

option("lodepng")
    set_default("builtin")
    set_values("builtin", "xrepo")
    set_description("lodepng source: vendored or fetched via xrepo")
    set_showmenu(true)
option_end()

option("build_tools")
    set_default(true)
    set_description("Build tool executables")
    set_showmenu(true)
option_end()

option("build_skeleton")
    set_default(true)
    set_description("Build the skeleton example framework",
                    "  Ignored for headless backends (null, ps2).")
    set_showmenu(true)
option_end()

if get_config("lodepng") == "xrepo" then
    add_requires("lodepng", {configs = {cpp = false}})
end

local gfx = librw_gfxlib_info()
if gfx then
    add_requires(gfx.package, gfx.alias and {alias = gfx.alias} or nil)
end

includes("vendor", "src", "host", "skeleton", "tools")
