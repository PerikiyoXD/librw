-- packages/l/librw/xmake.lua
--
-- Local xrepo package for librw.
--
-- Usage in a consumer project:
--   add_repositories("local-repo <path-to-dir-containing-packages/>")
--   add_requires("librw", {configs = {backend = "gl3", gfxlib = "sdl2"}})
--   ...
--   target("mygame")
--       add_packages("librw")
--
-- To publish to xmake-repo, replace the URL/hash with a pinned release.

package("librw")
    set_homepage("https://github.com/aap/librw")
    set_description("A re-implementation of the RenderWare Graphics engine")
    set_license("MIT")

    add_urls("https://github.com/aap/librw/archive/refs/heads/master.tar.gz",
             "https://github.com/aap/librw.git")
    -- Replace <SHA256> with the real digest once pinned to a commit/tag.
    add_versions("master", "<SHA256>")

    -- configs the consumer may set via add_requires(... {configs = {...}})
    add_configs("backend", {
        description = "Rendering backend: null | gl3 | d3d9",
        default = "null",
        type = "string"
    })
    add_configs("gfxlib", {
        description = "GL3 window/context lib: glfw | sdl2 | sdl3",
        default = "glfw",
        type = "string"
    })

    -- declare deps with add_requires so xrepo resolves and caches them
    -- before on_install runs
    on_load(function(pkg)
        if pkg:config("backend") == "gl3" then
            local gl = pkg:config("gfxlib") or "glfw"
            if gl == "glfw" then
                pkg:add("deps", "glfw")
            elseif gl == "sdl2" then
                pkg:add("deps", "libsdl2")
            elseif gl == "sdl3" then
                pkg:add("deps", "libsdl3")
            end
        end
    end)

    on_install(function(pkg)
        -- write a minimal xmake.lua fragment that calls add_requires for the
        -- chosen gfxlib so the embedded build also resolves packages correctly
        local backend = pkg:config("backend") or "null"
        local gfxlib  = pkg:config("gfxlib")  or "glfw"

        local requires_snippet = ""
        if backend == "gl3" then
            if gfxlib == "glfw" then
                requires_snippet = 'add_requires("glfw")'
            elseif gfxlib == "sdl2" then
                requires_snippet = 'add_requires("libsdl2", {alias = "sdl2"})'
            elseif gfxlib == "sdl3" then
                requires_snippet = 'add_requires("libsdl3", {alias = "sdl3"})'
            end
        end

        -- inject the requires line at the top of xmake.lua before building
        local xmake_file = path.join(os.curdir(), "xmake.lua")
        local content = io.readfile(xmake_file)
        io.writefile(xmake_file, requires_snippet .. "\n" .. content)

        import("package.tools.xmake").install(pkg, {
            backend        = backend,
            gfxlib         = gfxlib,
            build_tools    = false,
            build_skeleton = false,
        })
    end)

    on_test(function(pkg)
        assert(pkg:check_cxxsnippets({test = [[
            #include <librw/rw.h>
            void test() { (void)sizeof(rw::Raster); }
        ]]}, {configs = {languages = "cxx14"}}))
    end)
package_end()