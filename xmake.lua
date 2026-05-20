-- xmake.lua  (root)
-- Configure:
--   xmake f --backend=gl3 --gfxlib=glfw
--   xmake f --backend=d3d9          (Windows only)
--   xmake f --backend=null          (default, no renderer)
--   xmake f --lodepng=builtin       (default, compile src/lodepng/lodepng.cpp)
--   xmake f --lodepng=xrepo         (fetch lodepng via xrepo)
-- Build:
--   xmake
-- Install:
--   xmake install -o /usr/local

add_rules("mode.debug", "mode.release")

-- ---------------------------------------------------------------------------
-- options  (must appear before any get_config() calls)
-- ---------------------------------------------------------------------------
option("backend")
    set_default("null")
    set_values("null", "gl3", "d3d9")
    set_description("Rendering backend")
    set_showmenu(true)
option_end()

option("gfxlib")
    set_default("glfw")
    set_values("glfw", "sdl2", "sdl3")
    set_description("Window/context library for the gl3 backend")
    set_showmenu(true)
option_end()

option("lodepng")
    set_default("builtin")
    set_values("builtin", "xrepo")
    set_description("lodepng source: builtin (src/lodepng/lodepng.cpp) or xrepo")
    set_showmenu(true)
option_end()

option("build_tools")
    set_default(true)
    set_description("Build tool executables")
    set_showmenu(true)
option_end()

option("build_skeleton")
    set_default(true)
    set_description("Build the skeleton example framework (requires non-null backend)")
    set_showmenu(true)
option_end()

-- ---------------------------------------------------------------------------
-- output layout:  bin|lib / <plat>-<arch>-<mode>-<backend[-gfxlib]>
-- ---------------------------------------------------------------------------
local function targetdir(kind)
    local backend = get_config("backend") or "null"
    local gfxlib  = get_config("gfxlib")  or "glfw"
    local combo   = (backend == "gl3") and (backend .. "-" .. gfxlib) or backend
    return "$(builddir)/" .. kind .. "/$(plat)-$(arch)-$(mode)-" .. combo
end

-- ---------------------------------------------------------------------------
-- dependencies via add_requires
-- ---------------------------------------------------------------------------
if get_config("lodepng") == "xrepo" then
    add_requires("lodepng", {configs = {cpp = false}})
end

if get_config("backend") == "gl3" then
    local gl = get_config("gfxlib") or "glfw"
    if gl == "glfw" then
        add_requires("glfw")
    elseif gl == "sdl2" then
        add_requires("libsdl2", {alias = "sdl2"})
    elseif gl == "sdl3" then
        add_requires("libsdl3", {alias = "sdl3"})
    end
end

-- ---------------------------------------------------------------------------
-- helpers
-- ---------------------------------------------------------------------------

-- defines + syslinks only; no package links. Safe for all targets.
local function apply_backend(tgt)
    local backend = get_config("backend") or "null"
    local gfxlib  = get_config("gfxlib")  or "glfw"
    if backend == "null" then
        tgt:add("defines", "RW_NULL")
    elseif backend == "gl3" then
        tgt:add("defines", "RW_GL3")
        if gfxlib == "glfw" then
            tgt:add("defines", "LIBRW_GLFW")
        elseif gfxlib == "sdl2" then
            tgt:add("defines", "LIBRW_SDL2")
        elseif gfxlib == "sdl3" then
            tgt:add("defines", "LIBRW_SDL3")
        end
        if is_plat("linux", "macosx") then
            tgt:add("syslinks", "GL")
        elseif is_plat("windows") then
            tgt:add("syslinks", "opengl32")
        end
    elseif backend == "d3d9" then
        tgt:add("defines", "RW_D3D9")
        tgt:add("syslinks", "user32", "gdi32", "d3d9", "Xinput9_1_0")
    end
end

-- Full gfxlib package (headers + link). Called only on skeleton and windowed
-- tools. NOT called on librw or console tools to prevent /SUBSYSTEM:WINDOWS
-- from SDL2 propagating to console binaries.
local function apply_gfxlib(tgt)
    local backend = get_config("backend") or "null"
    local gfxlib  = get_config("gfxlib")  or "glfw"
    if backend ~= "gl3" then return end
    if gfxlib == "glfw" then
        tgt:add("packages", "glfw")
    elseif gfxlib == "sdl2" then
        tgt:add("packages", "sdl2")
    elseif gfxlib == "sdl3" then
        tgt:add("packages", "sdl3")
    end
end

-- ---------------------------------------------------------------------------
-- librw  (static library)
-- ---------------------------------------------------------------------------
target("librw")
    set_kind("static")
    set_basename("rw")
    set_targetdir(targetdir("lib"))
    set_languages("c11", "cxx14")

    add_includedirs(".", {public = true})

    add_files("src/*.cpp")
    add_files("src/d3d/*.cpp")
    add_files("src/gl/*.cpp")
    add_files("src/ps2/*.cpp")

    on_load(function(tgt)
        local lodepng = get_config("lodepng") or "builtin"
        if lodepng == "builtin" then
            tgt:add("files", "src/lodepng/lodepng.cpp")
            tgt:add("includedirs", "src/lodepng")
        else
            tgt:add("packages", "lodepng", {public = true})
        end
        if get_config("backend") == "gl3" then
            tgt:add("files", "src/gl/glad/*.c")
            local gfxlib  = get_config("gfxlib") or "glfw"
            local pkgname = (gfxlib == "glfw") and "glfw"
                         or (gfxlib == "sdl2") and "sdl2"
                         or "sdl3"
            tgt:add("packages", pkgname, {public = true})
        end
        apply_backend(tgt)
    end)

    add_defines("LODEPNG_NO_COMPILE_CPP")
    add_cxflags("/wd4996", "/wd4244", {tools = {"cl", "clang_cl"}})

    on_install(function(tgt)
        local inc = path.join(tgt:installdir(), "include", "librw")
        os.cp("rw.h",   inc)
        os.cp("args.h", inc)
        os.cp("src/**.h", inc)
        os.cp(tgt:targetfile(), path.join(tgt:installdir(), "lib"))
    end)
target_end()

-- ---------------------------------------------------------------------------
-- skeleton  (static helper lib for windowed example tools)
-- ---------------------------------------------------------------------------
if get_config("build_skeleton") and get_config("backend") ~= "null" then
    target("skeleton")
        set_kind("static")
        set_targetdir(targetdir("lib"))
        set_languages("c11", "cxx14")
        add_includedirs(".", "skeleton", {public = true})
        add_files("skeleton/*.cpp")
        add_files("skeleton/imgui/*.cpp")
        add_deps("librw")
        on_load(function(tgt)
            apply_backend(tgt)
            apply_gfxlib(tgt)
        end)
    target_end()
end

-- ---------------------------------------------------------------------------
-- tool helpers
-- ---------------------------------------------------------------------------
local function console_tool(name, dir)
    target(name)
        set_kind("binary")
        set_targetdir(targetdir("bin"))
        set_rundir(path.join("tools", dir))
        set_languages("c11", "cxx14")
        add_includedirs(".")
        add_files(path.join("tools", dir, "*.cpp"))
        add_deps("librw")
        on_load(function(tgt) apply_backend(tgt) end)
        -- explicitly force CONSOLE subsystem so any /SUBSYSTEM:WINDOWS
        -- injected by SDL2 via the librw {public=true} package chain is overridden
        add_ldflags("/SUBSYSTEM:CONSOLE", {tools = {"link"}, force = true})
    target_end()
end

local function windowed_tool(name, dir)
    if get_config("backend") == "null" then return end
    if not get_config("build_skeleton")  then return end
    target(name)
        set_kind("binary")
        set_targetdir(targetdir("bin"))
        set_rundir(path.join("tools", dir))
        set_languages("c11", "cxx14")
        add_includedirs(".", "skeleton")
        add_files(path.join("tools", dir, "*.cpp"))
        add_deps("librw", "skeleton")
        on_load(function(tgt)
            apply_backend(tgt)
            apply_gfxlib(tgt)
        end)
        add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:WinMainCRTStartup",
                    {tools = {"link"}, force = true})
    target_end()
end

-- ---------------------------------------------------------------------------
-- tools
-- ---------------------------------------------------------------------------
if get_config("build_tools") then
    console_tool("dumprwtree", "dumprwtree")
    console_tool("ska2anm",    "ska2anm")

    windowed_tool("playground", "playground")
    windowed_tool("imguitest",  "imguitest")
    windowed_tool("lights",     "lights")
    windowed_tool("subrast",    "subrast")
    windowed_tool("camera",     "camera")
    windowed_tool("im2d",       "im2d")
    windowed_tool("im3d",       "im3d")
    windowed_tool("hopalong",   "hopalong")
end