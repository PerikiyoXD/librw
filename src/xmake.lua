if is_config("platform", "VK") then
    add_requires("vulkansdk", "spdlog", "glfw", "glm")
end

target("librw")
    set_kind("static")
    set_targetdir("build")

    -- Add source files
    add_files("**.cpp")

    -- Add header files
    add_headerfiles("args.h", "rw.h", "rwanim.h", "rwengine.h", "rwerror.h", 
                    "rwobjects.h", "rwpipeline.h", "rwplg.h", "rwplugins.h", 
                    "rwrender.h", "rwuserdata.h", 
                    "d3d/*.h", "gl/*.h", "gl/glad/*.h", 
                    "ps2/*.h", "lodepng/*.h")

    -- Include directories
    add_includedirs(".", {public = true})

    -- Define macros
    add_defines("LODEPNG_NO_COMPILE_CPP")
    if is_mode("debug") then
        add_defines("DEBUG")
    else
        add_defines("NDEBUG")
    end

    -- Platform-specific defines
    if is_plat("windows") then
        add_defines("RW_WINDOWS")
    elseif is_plat("linux") then
        add_defines("RW_LINUX")
    elseif is_plat("macosx") then
        add_defines("RW_MACOS")
    elseif is_plat("ps2") then
        add_defines("RW_PS2")
    end 

    -- Add GLFW if the platform is GL3 or Vulkan
    if is_config("platform", "GL3") or is_config("platform", "VK") then
        add_defines("RW_GLFW")
        add_packages("glfw")
    end

    if is_config("platform", "VK") then
        add_deps("libRHI")
        add_packages("glm")
    end


    -- Handle platforms
    do_platform_logic()

    -- Compiler-specific flags
    if is_plat("windows") and is_kind("msvc") then
        add_cxflags("/wd4996", "/wd4244")
    elseif is_plat("linux") or is_plat("macosx") then
        add_cxflags("-Wall", "-Wextra", "-Wdouble-promotion", "-Wpedantic")
    end

    -- Platform-specific links
    if is_config("librw-renderer", "gl3") then
        if is_config("gfxlib", "glfw") then
            add_packages("glfw")
        elseif is_config("gfxlib", "sdl2") then
            add_packages("libsdl")
            add_defines("LIBRW_SDL2")
        end
        add_syslinks("GL")
    elseif is_config("librw-renderer", "d3d9") then
        add_syslinks("d3d9", "xinput")
    elseif is_config("librw-renderer", "ps2") then
        add_syslinks("ps2sdk")
    end

    -- Install files
    if is_mode("install") then
        on_install(function(target)
            os.cp("args.h", "$(installdir)/include/")
            os.cp("rw.h", "$(installdir)/include/")
            os.cp("src/**.h", "$(installdir)/include/src/")
            os.cp("d3d/*.h", "$(installdir)/include/src/d3d/")
            os.cp("ps2/*.h", "$(installdir)/include/src/ps2/")
            os.cp("gl/*.h", "$(installdir)/include/src/gl/")
            os.cp("gl/glad/*.h", "$(installdir)/include/src/gl/glad/")
            os.cp("build/lib/librw.a", "$(installdir)/lib/")
        end)
    end    
target_end()
