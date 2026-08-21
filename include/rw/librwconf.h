#ifndef RW_LIBRWCONF_H
#define RW_LIBRWCONF_H

/* Build configuration.
 *
 * The build system defines exactly one RW_<device> macro, selecting the
 * render device -- the one thing that implements rw::Device (see rwengine.h).
 * Everything derived from that choice is derived here and nowhere else.
 *
 * Note what is NOT selected here:
 *
 *   Native formats.  d3d8, d3d9, xbox, wdgl, ps2 and gl3 each implement
 *   rw::Driver, registered into engine->driver[PLATFORM_*]. Those are always
 *   compiled, so any DFF/TXD loads regardless of which device is active.
 *   They have no selector macro -- xbox never had one, and that is correct.
 *
 *   The device namespace.  Each device header declares its own rw::backend
 *   alias next to the namespace it names, so the two cannot drift apart.
 */

/* -- device selection ----------------------------------------------------- */

/* Written out rather than hidden behind a macro: using defined() inside a
 * macro that is then expanded in #if is undefined behaviour, and MSVC
 * evaluates it to 0. */
#if (defined(RW_NULL) + defined(RW_GL3) + defined(RW_D3D9) + defined(RW_PS2)) == 0
#error "librw: no render device selected. Define exactly one of RW_NULL, RW_GL3, RW_D3D9, RW_PS2."
#elif (defined(RW_NULL) + defined(RW_GL3) + defined(RW_D3D9) + defined(RW_PS2)) > 1
#error "librw: more than one render device selected. Define exactly one of RW_NULL, RW_GL3, RW_D3D9, RW_PS2."
#endif

/* RW_D3D8 and RW_WDGL are not devices. d3d8 and wdgl implement rw::Driver
 * only -- neither defines a rw::Device, so selecting one leaves
 * rw::d3d::renderdevice undefined at link time. Their format support is
 * always compiled; nothing needs to be selected. */
#if defined(RW_D3D8) || defined(RW_WDGL)
#error "librw: RW_D3D8/RW_WDGL are not render devices, they are native formats and are always compiled. Select RW_D3D9, RW_GL3, RW_PS2 or RW_NULL instead."
#endif

/* RW_GLES2/RW_GLES3 are not devices either. GLES is a runtime property of the
 * gl3 device -- see gl3Caps.gles in rw/gl/rwgl3.h and the loader choice in
 * gl3device.cpp. The compile-time GLES paths in src/gl/gl3shader.cpp are
 * disabled, their guards spelled xxxRW_GLES2. */
#if defined(RW_GLES2) || defined(RW_GLES3)
#error "librw: RW_GLES2/RW_GLES3 are not render devices. Build RW_GL3; GLES is selected at runtime."
#endif

/* -- derived configuration ------------------------------------------------ */

#ifdef RW_GL3
#define RW_OPENGL
/* doesn't help */
/* #define RW_GL_USE_VAOS */
#endif

#ifdef RW_D3D9
#define RWHALFPIXEL
#endif

#ifdef RW_PS2
#define RWHALFPIXEL
#endif

#endif
