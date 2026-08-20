-- imgui_impl_rw.{cpp,h} is librw's own ImGui renderer backend, written against
-- rw and the skeleton event loop. It lives here rather than in vendor/imgui so
-- that target stays pristine upstream.

if librw_has_skeleton() then

target("skeleton")
    set_kind("static")
    set_group("librw")
    set_targetdir(librw_outdir("lib"))

    librw_apply_backend()

    add_includedirs(".", {public = true})

    -- glfw.cpp / sdl2.cpp / sdl3.cpp / win.cpp each guard their whole body on
    -- the LIBRW_* define, so the inactive ones compile to nothing.
    add_files("*.cpp")

    -- host: skeleton is the app that supplies host::callbacks.
    add_deps("librw", "imgui", "host")

    local gfxpkg = librw_gfxlib_package()
    if gfxpkg then
        add_packages(gfxpkg, {public = true})
    end

    add_headerfiles("skeleton.h", "imgui_impl_rw.h",
                    {prefixdir = "librw/skeleton"})
target_end()

end
