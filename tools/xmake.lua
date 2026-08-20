-- Console tools build for every device. Windowed tools need skeleton, and so
-- a device that can open a window.
--
-- Windowed tools are GUI binaries and say so. Console tools no longer need to
-- force /SUBSYSTEM:CONSOLE: librw stopped exposing SDL/GLFW in its headers, so
-- the windowing package is the host's dependency alone and SDL2main's
-- /SUBSYSTEM:WINDOWS no longer reaches anything that only links librw.

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
