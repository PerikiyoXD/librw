#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rw/rwbase.h"
#include "rw/rwerror.h"
#include "rw/rwplg.h"
#include "rw/rwpipeline.h"
#include "rw/rwobjects.h"
#include "rw/rwengine.h"

#include "rw/gl/rwgl3.h"
#include "rw/gl/rwgl3shader.h"

#include "rwgl3impl.h"

namespace rw {
namespace gl3 {

// TODO: make some of these things platform-independent

static void*
driverOpen(void *o, int32, int32)
{
#ifdef RW_OPENGL
	engine->driver[PLATFORM_GL3]->defaultPipeline = makeDefaultPipeline();
#endif
	engine->driver[PLATFORM_GL3]->rasterNativeOffset = nativeRasterOffset;
	engine->driver[PLATFORM_GL3]->rasterCreate       = rasterCreate;
	engine->driver[PLATFORM_GL3]->rasterLock         = rasterLock;
	engine->driver[PLATFORM_GL3]->rasterUnlock       = rasterUnlock;
	engine->driver[PLATFORM_GL3]->rasterNumLevels    = rasterNumLevels;
	engine->driver[PLATFORM_GL3]->imageFindRasterFormat = imageFindRasterFormat;
	engine->driver[PLATFORM_GL3]->rasterFromImage    = rasterFromImage;
	engine->driver[PLATFORM_GL3]->rasterToImage      = rasterToImage;

	return o;
}

static void*
driverClose(void *o, int32, int32)
{
	return o;
}

void
registerPlatformPlugins(void)
{
	Driver::registerPlugin(PLATFORM_GL3, 0, PLATFORM_GL3,
	                       driverOpen, driverClose);
	registerNativeRaster();
}

}
}
