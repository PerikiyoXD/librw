librw
=====

This library is supposed to be a re-implementation of RenderWare graphics,
or a good part of it anyway.

It is intended to be cross-platform in two senses:
support rendering on different platforms similar to RW;
supporting all file formats for all platforms at all times and provide
way to convert to all other platforms.

Supported file formats are DFF and TXD for PS2, D3D8, D3D9 and Xbox.
Not all pre-instanced PS2 DFFs are supported.
BSP is not supported at all.

For rendering we have D3D9 and OpenGL (>=2.1, ES >= 2.0) backends(Currently, it supports vulkan).
Rendering some things on the PS2 is working as a test only.

# Building 

Get premake5. Generate a config, e.g. with ``premake5 gmake``,
and look in the build directory. (it could not be built with a single project, but I will provide some test cases in the future)


# 💡 Vulkan

The Vulkan backend is based on my personal [render lib](https://github.com/flwmxd/libRHI) which is also part of my personal engine.

# 💡 DDGI and Raytracing

I implemented [DDGI](https://github.com/NVIDIAGameWorks/RTXGI]), raytracing reflection/shadow with SVGF [denoiser](https://research.nvidia.com/publication/2017-07_spatiotemporal-variance-guided-filtering-real-time-reconstruction-path-traced).

The fully algorithm is based on my other project [LuxGI](https://github.com/flwmxd/LuxGI), it has a detailed description of how it works.

#  💡 What I have done
* Fully implemented render module with vulkan.
* Altered lighting model, to be more realistic (Generated GBuffer)
* DDGI for low-frequency indirect lighting.
* Raytraced 1 SPP reflection and shadow with SVGF denoiser.
* Better performance im2d/im3d renderer than OpenGL
* Profiler tools integration( [Tracy](https://github.com/wolfpld/tracy) )
* PathTracer for reference

# 💡 TODO-List
* Try to trace particle
    * because particles are just 2D sprites, they are not in TLAS
* Using Deferred-Lighting and better light-culling techs
    * because of the transparency objects and sprites, I kept the original lighting process.
* Using compute shader to compute skeleton-mesh/animations, currently skeleton-meshes are not supported for raytracing.
* Raytraced-AO.
* ReSTIR.
* Better Skybox or Atmosphere model.
* Realtime Volumetric Cloud / Fog.
* Mobile platform porting (Apple M3 / Snapdragon 8 Gen3)
