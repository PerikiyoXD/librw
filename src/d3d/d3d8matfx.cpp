#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "rw/rwbase.h"
#include "rw/rwerror.h"
#include "rw/rwplg.h"
#include "rw/rwpipeline.h"
#include "rw/rwobjects.h"
#include "rw/rwanim.h"
#include "rw/rwengine.h"
#include "rw/rwplugins.h"
#include "rw/d3d/rwd3d.h"
#include "rw/d3d/rwd3d8.h"

namespace rw {
namespace d3d8 {
using namespace d3d;

static void*
matfxOpen(void *o, int32, int32)
{
	matFXGlobals.pipelines[PLATFORM_D3D8] = makeMatFXPipeline();
	return o;
}

static void*
matfxClose(void *o, int32, int32)
{
	((ObjPipeline*)matFXGlobals.pipelines[PLATFORM_D3D8])->destroy();
	matFXGlobals.pipelines[PLATFORM_D3D8] = nil;
	return o;
}

void
initMatFX(void)
{
	Driver::registerPlugin(PLATFORM_D3D8, 0, ID_MATFX,
	                       matfxOpen, matfxClose);
}

ObjPipeline*
makeMatFXPipeline(void)
{
	ObjPipeline *pipe = ObjPipeline::create();
	pipe->instanceCB = defaultInstanceCB;
	pipe->uninstanceCB = defaultUninstanceCB;
	pipe->renderCB = defaultRenderCB;
	pipe->pluginID = ID_MATFX;
	pipe->pluginData = 0;
	return pipe;
}

}
}
