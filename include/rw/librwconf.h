#ifndef LIBRW_CONF_H
#define LIBRW_CONF_H

/* Build configuration.
 *
 * The build system defines exactly one RW_<backend> macro. Everything derived
 * from that choice is derived here and nowhere else.
 *
 * Note what is deliberately *not* here: the device namespace. Each backend
 * header declares its own alias (rw::backend) next to the namespace it names,
 * so the two cannot drift apart. This header only records capabilities and
 * rejects combinations that are known not to work.
 */

/* -- backend selection ---------------------------------------------------- */

/* Written out rather than hidden behind a macro: using defined() inside a
 * macro that is then expanded in #if is undefined behaviour, and MSVC
 * evaluates it to 0. */
#if (defined(RW_NULL) + defined(RW_GL3) + defined(RW_WDGL) + defined(RW_D3D8) + defined(RW_D3D9) + defined(RW_PS2)) == 0
#error "librw: no backend selected. Define exactly one of RW_NULL, RW_GL3, RW_WDGL, RW_D3D8, RW_D3D9, RW_PS2."
#elif (defined(RW_NULL) + defined(RW_GL3) + defined(RW_WDGL) + defined(RW_D3D8) + defined(RW_D3D9) + defined(RW_PS2)) > 1
#error "librw: more than one backend selected. Define exactly one of RW_NULL, RW_GL3, RW_WDGL, RW_D3D8, RW_D3D9, RW_PS2."
#endif

/* RW_GLES2/RW_GLES3 are not backends. GLES is a runtime property of the gl3
 * backend -- see the 'gles' field in gl3Caps (src/gl/rwgl3.h) and the loader
 * choice in gl3device.cpp. The last compile-time GLES code was disabled by
 * renaming its guards to xxxRW_GLES2 (src/gl/gl3shader.cpp). */
#if defined(RW_GLES2) || defined(RW_GLES3)
#error "librw: RW_GLES2/RW_GLES3 are not backends. Build with RW_GL3; GLES is selected at runtime."
#endif

/* -- derived configuration ------------------------------------------------ */

/* RW_DEVICE_HAS_IM: the device namespace defines the immediate-mode vertex
 * types (Im2DVertex, Im3DVertex). Everything that uses rw::backend needs them,
 * so this is what decides whether a backend is usable at all. */

#ifdef RW_GL3
#define RW_OPENGL
#define RW_DEVICE_HAS_IM 1
/* doesn't help */
/* #define RW_GL_USE_VAOS */
#endif

#ifdef RW_WDGL
#define RW_OPENGL
/* wdgl has no device namespace of its own. It also defines RW_OPENGL, which
 * un-guards rwgl3shader.h while rwgl3.h stays guarded on RW_GL3, so the shader
 * header references gl3 types that do not exist. */
#define RW_DEVICE_HAS_IM 0
#endif

#ifdef RW_D3D9
#define RWHALFPIXEL
#define RW_DEVICE_HAS_IM 1
#endif

#ifdef RW_D3D8
/* rwd3d.h guards Im2DVertex/Im3DVertex on RW_D3D9, so under d3d8 the rw::d3d
 * namespace exists but is empty. */
#define RW_DEVICE_HAS_IM 0
#endif

#ifdef RW_PS2
#define RWHALFPIXEL
#define RW_DEVICE_HAS_IM 1
#endif

#ifdef RW_NULL
/* No device namespace, and nothing that needs one is compiled. */
#define RW_DEVICE_HAS_IM 0
#endif

/* -- incomplete backends -------------------------------------------------- */

/* Fail here, with a reason, rather than at every rw::backend use site.
 * Define RW_ALLOW_INCOMPLETE_BACKEND to build anyway while working on one. */
#if !defined(RW_ALLOW_INCOMPLETE_BACKEND) && !defined(RW_NULL) && !RW_DEVICE_HAS_IM
#if defined(RW_WDGL)
#error "librw: the wdgl backend is incomplete -- it defines no device namespace, and RW_OPENGL pulls in rwgl3shader.h without rwgl3.h. Define RW_ALLOW_INCOMPLETE_BACKEND to build anyway."
#elif defined(RW_D3D8)
#error "librw: the d3d8 backend is incomplete -- rwd3d.h guards Im2DVertex/Im3DVertex on RW_D3D9, so rw::d3d is empty here. Define RW_ALLOW_INCOMPLETE_BACKEND to build anyway."
#endif
#endif

#endif
