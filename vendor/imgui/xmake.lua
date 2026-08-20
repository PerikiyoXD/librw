-- Dear ImGui 1.92.2b + ImGuizmo -- pristine upstream, no local patches.
-- Bump by dropping in a new export; nothing here needs editing.
--
-- librw's own ImGui renderer backend is deliberately not part of this target;
-- it lives in skeleton/imgui_impl_rw.{cpp,h}.

target("imgui")
    set_kind("static")
    set_group("vendor")
    set_default(false)
    set_languages("cxx14")

    add_includedirs(".", {public = true})

    add_files(
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
        "imgui_demo.cpp",  -- ShowDemoWindow(), used by imguitest and hopalong
        "ImGuizmo.cpp")

    add_headerfiles("*.h", {prefixdir = "imgui"})
target_end()
