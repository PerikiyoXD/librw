-- Console tools build for every backend. Windowed tools need skeleton, and so
-- a backend that can open a window.
--
-- The subsystem is set explicitly rather than left to chance: gl/rwgl3.h
-- exposes SDL/GLFW publicly, so the windowing package propagates to every
-- dependent, and SDL2 drags /SUBSYSTEM:WINDOWS along with it.

local CONSOLE_TOOLS = {
    "dumprwtree",
    "ska2anm",
    -- ps2test needs a DSM assembler for its vu/*.dsm sources.
}

local WINDOWED_TOOLS = {
    "playground", "imguitest", "lights", "subrast",
    "camera", "im2d", "im3d", "hopalong",
}

local function librw_tool(name, windowed)
    target(name)
        set_kind("binary")
        set_group("tools")
        set_targetdir(librw_outdir("bin"))
        set_rundir("$(scriptdir)/" .. name)

        librw_apply_backend()

        add_files(name .. "/*.cpp")

        if windowed then
            -- host owns the entry point (main / WinMain)
            add_deps("librw", "skeleton", "host")
            if is_plat("windows") then
                add_ldflags("/SUBSYSTEM:WINDOWS", "/ENTRY:WinMainCRTStartup",
                            {tools = {"link"}, force = true})
            end
        else
            add_deps("librw")
            if is_plat("windows") then
                add_ldflags("/SUBSYSTEM:CONSOLE", {tools = {"link"}, force = true})
            end
        end
    target_end()
end

if get_config("build_tools") then
    for _, name in ipairs(CONSOLE_TOOLS) do
        librw_tool(name, false)
    end
    if librw_has_skeleton() then
        for _, name in ipairs(WINDOWED_TOOLS) do
            librw_tool(name, true)
        end
    end
end
