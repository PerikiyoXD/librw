-- Project configuration
set_project("librw")
set_version("0.0.1")
set_languages("clatest", "cxxlatest")
add_rules("mode.debug", "mode.release")

-- Platform options
option("platform")
    set_default("NULL")
    set_showmenu(true)
    set_description("Select the target platform")
    set_values("NULL", "GL3", "D3D9", "PS2", "VK")
option_end()

option("gfxlib")
    set_default("GLFW")
    set_showmenu(true)
    set_description("Select the graphics library for GL3")
    set_values("GLFW", "SDL2")
option_end()

option("build_tools")
    set_default(false)
    set_showmenu(true)
    set_description("Enable building tools")
option_end()

option("install_files")
    set_default(false)
    set_showmenu(true)
    set_description("Enable installation of files")
option_end()

-- util funcs

function do_platform_logic()
    if is_config("platform", "GL3") then
        add_defines("RW_GL3")
        if is_config("gfxlib", "GLFW") then
            add_packages("glfw")
        elseif is_config("gfxlib", "SDL2") then
            add_packages("libsdl")
            add_defines("LIBRW_SDL2")
        end
        add_syslinks("GL")
    elseif is_config("platform", "D3D9") then
        add_defines("RW_D3D9")
        add_syslinks("d3d9", "xinput")
    elseif is_config("platform", "PS2") then
        add_defines("RW_PS2")
        add_syslinks("ps2sdk")
    elseif is_config("platform", "VK") then
        add_defines("RW_VULKAN", "MAPLE_VULKAN", "USE_VMA_ALLOCATOR", "ENABLE_RAYTRACING")
        add_packages("vulkansdk")
    elseif is_config("platform", "NULL") then
        add_defines("RW_NULL")
    end
end


-- Add subdirectories
if is_config("platform", "VK") then
    includes("tools/libRHI/xmake.lua")
end



includes("src/xmake.lua")

if has_config("build_tools") and not is_config("platform", "PS2") and not is_config("platform", "NULL") then
    includes("skeleton/xmake.lua")
    includes("tools/xmake.lua")
end

-- Install rules
if has_config("install_files") then
    on_install(function(target)
        -- Install headers
        os.cp("src/*.h", "$(installdir)/include/")
        os.cp("src/d3d/*.h", "$(installdir)/include/d3d/")
        os.cp("src/gl/*.h", "$(installdir)/include/gl/")
        os.cp("src/gl/glad/*.h", "$(installdir)/include/gl/glad/")
        os.cp("src/ps2/*.h", "$(installdir)/include/ps2/")
        -- Install libraries
        os.cp("build/*.a", "$(installdir)/lib/")
    end)
end

