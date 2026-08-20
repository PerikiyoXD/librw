# Roadmap

Working notes on the state of the tree. Not a changelog.

## Build systems

**xmake is the primary build system.** It is the only one currently working.

**CMake and Premake are on hold.** Both are known-broken and are not being
maintained for now:

- They still reference the pre-`vendor/` layout: `src/lodepng/`,
  `src/gl/glad/`, `skeleton/imgui/*.cpp`. Those paths no longer exist.
- Neither knows about `src/librwconf.h`.
- `skeleton/CMakeLists.txt` still lists `imgui/imgui_impl_rw.cpp`, which moved
  up to `skeleton/imgui_impl_rw.cpp`.

Affected files: `CMakeLists.txt`, `cmake/`, `README.cmake`, `src/CMakeLists.txt`,
`skeleton/CMakeLists.txt`, `tools/**/CMakeLists.txt`, `premake5.lua`,
`premake-vs2019.cmd`, `premake5.exe`.

Decide later whether to repair them or drop them.

## Devices and native formats

These are two different things, and conflating them is the source of most of
the confusion in this tree.

**Render device** -- implements `rw::Device` (`include/rw/rwengine.h:48`):
`beginUpdate`, `clearCamera`, `showRaster`, render state, `im2D*`/`im3D*`.
There is exactly **one** per build, selected by an `RW_*` macro.

| Device | `Device` definition | State |
| --- | --- | --- |
| `null`  | `src/engine.cpp:552`        | builds, runs |
| `gl3`   | `src/gl/gl3device.cpp:2371` | builds, runs (glfw / sdl2 / sdl3) |
| `d3d9`  | `src/d3d/d3ddevice.cpp:2000`| builds, runs |
| `ps2`   | `src/ps2/ps2device.cpp:26`  | untested here; needs a PS2 toolchain |

**Native format** -- implements `rw::Driver` (`include/rw/rwengine.h:74`),
registered into `engine->driver[PLATFORM_*]`: raster create/lock/unlock,
image conversion, plus `readNativeData`/`writeNativeData` and instancing
pipelines. There is an **array** of these, indexed by the runtime `PLATFORM_*`
enum, and they are **always compiled** -- so any DFF/TXD loads regardless of
which device is active.

Formats present: d3d8, d3d9, xbox, wdgl, ps2, gl3. None has or needs a
selector macro; `xbox` never had one and that is the correct precedent.

### d3d8 and wdgl are formats, not devices

They implement `Driver` only. Neither defines a `Device`, so selecting one
leaves `rw::d3d::renderdevice` undefined at link time. `librwconf.h` rejects
`RW_D3D8`/`RW_WDGL` with an explanation. Their format support is unaffected
and always available.

If a d3d8 or wdgl *renderer* is ever wanted, that is new work -- a ~2000-line
`Device` implementation -- not a repair.

### There is no gles2/gles3 device

GLES is a runtime property of gl3 -- see `gl3Caps.gles` in
`include/rw/gl/rwgl3.h` and the loader choice in `gl3device.cpp`. The last
compile-time GLES work was parked by renaming its guards to `xxxRW_GLES2`
(`src/gl/gl3shader.cpp:279,292`).

## Known issues

- `rwgl3.h` is a public header but exposes `SDL_Window**` and includes
  `<SDL.h>`/`<GLFW/glfw3.h>`. librw currently creates the window and GL
  context itself (`gl3device.cpp`, ~600 lines including three near-identical
  SDL2/SDL3/GLFW implementations). Decision taken: **librw should receive a
  context** instead. That removes the triplication and the layering violation
  together.
- `d3ddevice.cpp` and `gl3device.cpp` contain genuinely duplicated code, not
  merely parallel code: `getRenderState`, `setDepthTest`/`setDepthWrite`/
  `setVertexAlpha`, the `RwStateCache` structs and the state map tables. The
  `beginUpdate` view matrix is identical in both down to a typo in a comment.
- Instancing (a format concern, always needed) and `renderCB` (a device
  concern) are mixed in the same pipeline files, e.g. `src/d3d/d3d8skin.cpp`.
- `BIGENDIAN` is tested in `rwbase.h:410` and `base.cpp:758,778` but defined
  nowhere. From `e4b4bf9 "made (most of) streaming work on big endian"` --
  dormant, unfinished, not dead.
- `RWPUBLIC` gates `registerModule` out of public headers in four classes but
  is never defined. It is an unused public/private API mechanism that overlaps
  with the `include/` split.
- The installed tree is unusable for skeleton: `skeleton.h` includes
  `"imgui.h"` but no imgui headers are installed.
- No test suite exists. Verification is "the four devices build" plus manual
  runs of the windowed tools.

## Source layout

`include/` is the public API and the only exported include path. `src/` is
deliberately not on the include path, so internal headers (`rw*impl.h`,
`shaders/`) are reachable only relative to the including file.

Still to do: `src/` root mixes backend-independent core with files carrying
device conditionals -- `base.cpp` (`RW_D3D9`/`RW_GL3`/`RW_PS2`),
`engine.cpp`, `texture.cpp`, `raster.cpp`, `charset.cpp`.

## Breaking changes for downstream

These affect anything building against librw (re3-style projects and similar).

### `rw::RWDEVICE` is now `rw::backend`

The `RWDEVICE` macro is gone. It was a preprocessor substitution of a namespace
name, defined in `rwbase.h` from one set of macros while the contents of the
namespace it named were guarded by a different set — so the two could drift
apart with nothing to catch it.

It is replaced by a real namespace alias, declared in each backend header
beside the namespace it names, so the alias guard and the contents guard are
the same guard:

```cpp
// src/gl/rwgl3.h
#ifdef RW_GL3
namespace rw { namespace backend = gl3; }
#endif
```

Migration is a token swap — `RWDEVICE` was only ever a namespace name, so every
usage form is covered:

```sh
sed -i 's/\bRWDEVICE\b/backend/g' <your sources>
```

| Before | After |
| --- | --- |
| `rw::RWDEVICE::Im2DVertex` | `rw::backend::Im2DVertex` |
| `RWDEVICE::Im2DVertex` | `backend::Im2DVertex` |
| `using namespace rw::RWDEVICE;` | `using namespace rw::backend;` |

Note that `rw::backend` does **not** exist for `d3d8`, deliberately —
`Im2DVertex`/`Im3DVertex` are inside `rwd3d.h`'s `RW_D3D9` guard, so aliasing
the empty `rw::d3d` would be the drift this change exists to remove.

### Exactly one device macro is now required

`librwconf.h` errors if zero or more than one of `RW_NULL`, `RW_GL3`,
`RW_D3D9`, `RW_PS2` is defined. Previously a build with none simply got no
`RWDEVICE` and failed later with unrelated errors.

### `RW_D3D8` and `RW_WDGL` are rejected

They are native formats, not render devices, and are always compiled. Nothing
needs to select them. See "Devices and native formats" above.

### `sk::CHARINPUT` now passes a pointer

It used to smuggle the character *through* the `void *param` slot as a value
(`(void*)(uintptr)c`) while every other event passed a real pointer -- in the
same switch, `KEYDOWN` dereferenced `param` and `CHARINPUT` did not. Handlers
must now read it as `*(int*)param`, like `KEYDOWN`/`KEYUP`.

### `sk::Key` and `sk::MouseState` moved to `host::`

Key codes and mouse state are windowing concerns and live in the host layer.
`skeleton.h` pulls them back into `sk::` with a using-directive, so existing
code writing `sk::KEY_ESC` or `sk::MouseState` still compiles.

### Applications no longer define `engineOpenParams`

It used to be defined in every application's `main.cpp` and filled by the
window layer. The host owns it now; delete your definition.

### `RW_GLES2` / `RW_GLES3` are rejected

They were never backends. Build `RW_GL3`; GLES is selected at runtime.

## Tools

`ps2test` is not wired into the xmake build. It is PS2-only and needs a DSM
assembler for `tools/ps2test/vu/*.dsm`.

## Vendored dependencies

`vendor/{glad,lodepng,imgui}`, one self-contained xmake target each, exporting
its own include root. Sources are unmodified upstream.

- **glad** — generated, not hand-written. Regenerate with glad 0.1.x using
  `--api="gl=3.3,gles2=3.1" --profile=core --generator=c`. Current export is
  0.1.34 (2021-02-17). Kept vendored because reproducing that exact API/profile
  pair through a package is unreliable.
- **lodepng** — also available as an xrepo package via `--lodepng=xrepo`.
- **imgui** — 1.92.2b plus ImGuizmo, tracked by hand. `imgui_impl_rw.{cpp,h}`
  is librw's own renderer backend and deliberately lives in `skeleton/`, not
  here, so this stays a clean upstream drop.
