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

## Backends

Configuration lives in `xmake/matrix.lua` (build side) and `src/librwconf.h`
(source side).

| Backend | State |
| --- | --- |
| `null`  | builds |
| `gl3`   | builds, with `glfw` / `sdl2` / `sdl3` |
| `d3d9`  | builds |
| `wdgl`  | **incomplete** |
| `d3d8`  | **incomplete** |
| `ps2`   | untested here; needs a PS2 toolchain |

`librwconf.h` rejects the incomplete backends with an explanation rather than
letting them fail at ~45 unrelated use sites. Set `RW_ALLOW_INCOMPLETE_BACKEND`
to build one anyway while working on it.

### wdgl

Defines no device namespace of its own. It also defines `RW_OPENGL`, which
un-guards `rwgl3shader.h` while `rwgl3.h` stays guarded on `RW_GL3` — so the
shader header references gl3 types that were preprocessed away. The guards need
to agree on granularity before this can build.

### d3d8

`rwd3d.h` guards `Im2DVertex` / `Im3DVertex` on `RW_D3D9`, so under d3d8 the
`rw::d3d` namespace exists but is empty. Those types need to move out of the
`RW_D3D9` guard, or d3d8 needs its own.

### There is no gles2/gles3 backend

GLES is a *runtime* property of the gl3 backend — see `gl3Caps.gles` in
`src/gl/rwgl3.h` and the loader choice in `gl3device.cpp`. The last
compile-time GLES code was disabled by renaming its guards to `xxxRW_GLES2`
(`src/gl/gl3shader.cpp`). `RW_GLES2` / `RW_GLES3` are rejected by
`librwconf.h`; build `gl3` instead.

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

### Exactly one backend macro is now required

`librwconf.h` errors if zero or more than one of `RW_NULL`, `RW_GL3`,
`RW_WDGL`, `RW_D3D8`, `RW_D3D9`, `RW_PS2` is defined. Previously a build with
none simply got no `RWDEVICE` and failed later with unrelated errors.

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
