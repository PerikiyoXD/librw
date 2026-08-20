-- librw configuration matrix.
--
-- Included from the root xmake.lua before anything else. These are description
-- scope globals: import() does not exist there, so every included xmake.lua
-- reaches this through plain globals rather than a module.

-- define     RW_* macro src/librwconf.h keys off
-- gl         needs a GL loader (glad)
-- gfxlib     needs a windowing/context library
-- desktopgl  links a desktop GL implementation
--
-- There is no gles2/gles3 backend: GLES is a runtime property of gl3, chosen
-- from gl3Caps.gles. wdgl and d3d8 are selectable but incomplete; librwconf.h
-- rejects them with an explanation unless RW_ALLOW_INCOMPLETE_BACKEND is set.
LIBRW_BACKENDS = {
    null  = {define = "RW_NULL"},
    gl3   = {define = "RW_GL3",   gl = true, gfxlib = true, desktopgl = true},
    wdgl  = {define = "RW_WDGL",  gl = true, desktopgl = true},
    d3d8  = {define = "RW_D3D8",  syslinks = {"user32", "gdi32", "d3d8"}},
    d3d9  = {define = "RW_D3D9",  syslinks = {"user32", "gdi32", "d3d9",
                                              "Xinput9_1_0"}},
    ps2   = {define = "RW_PS2"},
}

-- Fallback menu for platforms absent from LIBRW_PLATFORM_BACKENDS.
LIBRW_BACKEND_ORDER = {"null", "gl3", "wdgl", "d3d8", "d3d9", "ps2"}

LIBRW_GFXLIBS = {
    glfw = {define = "LIBRW_GLFW", package = "glfw"},
    sdl2 = {define = "LIBRW_SDL2", package = "libsdl2", alias = "sdl2"},
    sdl3 = {define = "LIBRW_SDL3", package = "libsdl3", alias = "sdl3"},
}

LIBRW_GFXLIB_ORDER = {"glfw", "sdl2", "sdl3"}

-- Only platforms with evidence behind them: windows/mingw/linux/macosx come
-- from CMakeLists.txt, cross covers the PS2 toolchain. Platforms absent here
-- fall back to LIBRW_BACKEND_ORDER rather than being guessed at.
LIBRW_PLATFORM_BACKENDS = {
    windows  = {"null", "gl3", "wdgl", "d3d8", "d3d9"},
    mingw    = {"null", "gl3", "wdgl", "d3d8", "d3d9"},
    linux    = {"null", "gl3", "wdgl"},
    macosx   = {"null", "gl3", "wdgl"},
    cross    = {"null", "gl3", "ps2"},
}

-- Cannot open a window, so cannot build skeleton or any windowed tool.
LIBRW_HEADLESS = {null = true, ps2 = true}

function librw_backend()
    return get_config("backend") or "null"
end

function librw_backend_info()
    return LIBRW_BACKENDS[librw_backend()]
end

function librw_gfxlib()
    return get_config("gfxlib") or "glfw"
end

-- nil when the backend uses no windowing library.
function librw_gfxlib_info()
    local info = librw_backend_info()
    if not (info and info.gfxlib) then
        return nil
    end
    return LIBRW_GFXLIBS[librw_gfxlib()]
end

function librw_gfxlib_package()
    local gfx = librw_gfxlib_info()
    return gfx and (gfx.alias or gfx.package) or nil
end

function librw_needs_glad()
    local info = librw_backend_info()
    return info ~= nil and info.gl == true
end

function librw_has_skeleton()
    return get_config("build_skeleton") and not LIBRW_HEADLESS[librw_backend()]
end

-- <plat>-<arch>-<mode>-<backend[-gfxlib]>
function librw_outdir(kind)
    local combo = librw_backend()
    if librw_gfxlib_info() then
        combo = combo .. "-" .. librw_gfxlib()
    end
    return path.join("$(builddir)", kind, "$(plat)-$(arch)-$(mode)-" .. combo)
end

-- Call inside a target block: applies the backend's defines and system libs.
function librw_apply_backend()
    local info = librw_backend_info()
    if not info then
        return  -- unknown backend; librw_backend_error() reports it
    end

    add_defines(info.define)

    local gfx = librw_gfxlib_info()
    if gfx then
        add_defines(gfx.define)
    end

    if info.syslinks then
        add_syslinks(info.syslinks)
    end

    if info.desktopgl then
        if is_plat("windows", "mingw") then
            add_syslinks("opengl32")
        elseif is_plat("linux") then
            add_syslinks("GL")
        elseif is_plat("macosx") then
            add_frameworks("OpenGL")
        end
    end
end

-- Backends buildable on the target platform. This is what --backend= offers,
-- so xmake rejects an unavailable choice itself; description scope has no way
-- to raise, and a restricted menu is a better error than a raise anyway.
function librw_backend_values()
    local plat = get_config("plat") or os.host()
    return LIBRW_PLATFORM_BACKENDS[plat] or LIBRW_BACKEND_ORDER
end

local function librw_contains(list, value)
    for _, v in ipairs(list) do
        if v == value then
            return true
        end
    end
    return false
end

-- Why the selected backend is unusable, or nil. set_values() only populates
-- the menu, it does not reject, so this has to be enforced separately -- and
-- description scope has no raise(), so the caller carries this string into a
-- script-scope callback as an upvalue and raises it there.
function librw_backend_error()
    local name    = librw_backend()
    local plat    = get_config("plat") or os.host()
    local allowed = LIBRW_PLATFORM_BACKENDS[plat]

    if not LIBRW_BACKENDS[name] then
        return format("unknown backend '%s' (choices: %s)",
                      name, table.concat(LIBRW_BACKEND_ORDER, ", "))
    end
    if allowed and not librw_contains(allowed, name) then
        return format("backend '%s' is not available on %s (choices: %s)",
                      name, plat, table.concat(allowed, ", "))
    end
end
