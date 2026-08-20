-- Every backend's sources compile unconditionally; the RW_* guards decide what
-- survives preprocessing. Matches the cmake and premake builds, and keeps
-- unselected backends from rotting.

-- Raised from script scope; see librw_backend_error().
local backend_error = librw_backend_error()

target("librw")
    set_kind("static")
    set_basename("rw")
    set_group("librw")
    set_targetdir(librw_outdir("lib"))

    librw_apply_backend()

    -- The public API, and the only include path exported. src/ is deliberately
    -- absent: internal headers (rw*impl.h, shaders/) are reached relative to
    -- the including file, so nothing outside src/ can reach them by accident.
    add_includedirs("$(projectdir)/include", {public = true})

    add_files("*.cpp", "d3d/*.cpp", "gl/*.cpp", "ps2/*.cpp")

    add_cxflags("/wd4996", "/wd4244", {tools = {"cl", "clang_cl"}})

    -- src/png.cpp includes "lodepng.h"; both sources provide that name.
    if get_config("lodepng") == "xrepo" then
        add_packages("lodepng", {public = true})
    else
        add_deps("lodepng")
    end

    -- gl/rwgl3.h is public and includes "glad/glad.h".
    if librw_needs_glad() then
        add_deps("glad")
    end

    -- No windowing package here, deliberately. librw does not create windows
    -- or GL contexts and its headers no longer include <SDL.h>/<GLFW/glfw3.h>,
    -- so SDL/GLFW is the host layer's dependency alone. That is also what
    -- stops SDL2main's /SUBSYSTEM:WINDOWS leaking into the console tools.

    on_load(function (target)
        if backend_error then
            raise(backend_error)
        end
    end)

    add_headerfiles("$(projectdir)/include/(**.h)", {prefixdir = "librw"})
    add_headerfiles("$(projectdir)/include/(**.err)", {prefixdir = "librw"})
target_end()
