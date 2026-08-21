-- The host layer: window, GL context, event loop, input, entry point.
--
-- This is the only target that links SDL/GLFW. librw does not -- see
-- host/host.h for the boundary. Applications link host and supply
-- host::callbacks and host::config.
--
-- Skipped for headless devices, which have no window to host.

if librw_has_skeleton() then

target("host")
    set_kind("static")
    set_group("librw")
    set_targetdir(librw_outdir("lib"))

    librw_apply_backend()

    add_includedirs(".", {public = true})
    add_includedirs("../include")

    -- glfw.cpp / sdl2.cpp / sdl3.cpp guard their whole body on the gfxlib
    -- define; win.cpp on _WIN32. The inactive ones compile to nothing.
    add_files("*.cpp")

    local gfxpkg = librw_gfxlib_package()
    if gfxpkg then
        add_packages(gfxpkg, {public = true})
    end

    add_headerfiles("host.h", {prefixdir = "librw/host"})
target_end()

end
