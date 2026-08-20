librw
=====

> # UNDER HEAVY REWORK 
> 
> The rework bought a much cleaner boundary between librw and the platform around it:
> 
>   - librw no longer owns windows, GL contexts, input, or event loops. SDL/GLFW dependencies now stop at the host layer.
>   - Embedding librw into another engine or application is easier: the host supplies an opaque surface and four callbacks instead of librw creating the environment itself.
>   - Three duplicated GL window/context implementations collapsed into one renderer path; gl3device.cpp dropped roughly 670 lines.
>   - Public headers are separated under include/, guarded, and no longer leak SDL/GLFW types.
>   - Render devices are now correctly distinguished from native-format drivers, making the build matrix and configuration macros more accurate.
>   - Third-party components and project directories are independent xmake targets, improving packaging and maintenance.
>   - Windows console tools no longer inherit SDL’s GUI subsystem flags, removing the /SUBSYSTEM:CONSOLE workaround.
>   - Input event handling is more consistent: CHARINPUT follows the same pointer convention as other events.
>   - A clean rebuild proves the architecture works across GL3/SDL2, GL3/SDL3, GL3/GLFW, D3D9, and headless/null on Windows x64.
> 
>   The tradeoffs are that the host API is now a real integration surface, existing applications may need migration, and display/video-mode handling is temporarily stubbed until it is fully moved into the host. We also only proved release x64 for those five configurations; debug
>   remains outside this rebuild, and there is no test suite -- "verified" means it builds, links, and the window stays up.

---

This library is supposed to be a re-implementation of RenderWare graphics,
or a good part of it anyway.

It is intended to be cross-platform in two senses:
support rendering on different platforms similar to RW;
supporting all file formats for all platforms at all times and provide
way to convert to all other platforms.

Supported file formats are DFF and TXD for PS2, D3D8, D3D9 and Xbox.
Not all pre-instanced PS2 DFFs are supported.
BSP is not supported at all.

For rendering we have D3D9 and OpenGL (>=2.1, ES >= 2.0) backends.
Rendering some things on the PS2 is working as a test only.

# Uses

librw can be used for rendering [GTA](https://github.com/gtamodding/re3).

# Building

Get premake5. Generate a config, e.g. with ``premake5 gmake``,
and look in the build directory.
