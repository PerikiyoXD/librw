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
#include "rw/d3d/rwxbox.h"

namespace rw {
namespace xbox {

static void*
matfxOpen(void *o, int32, int32)
{
	matFXGlobals.pipelines[PLATFORM_XBOX] = makeMatFXPipeline();
	return o;
}

static void*
matfxClose(void *o, int32, int32)
{
	((ObjPipeline*)matFXGlobals.pipelines[PLATFORM_XBOX])->destroy();
	matFXGlobals.pipelines[PLATFORM_XBOX] = nil;
	return o;
}

void
initMatFX(void)
{
	Driver::registerPlugin(PLATFORM_XBOX, 0, ID_MATFX,
	                       matfxOpen, matfxClose);
}

ObjPipeline*
makeMatFXPipeline(void)
{
	ObjPipeline *pipe = ObjPipeline::create();
	pipe->instanceCB = defaultInstanceCB;
	pipe->uninstanceCB = defaultUninstanceCB;
	pipe->pluginID = ID_MATFX;
	pipe->pluginData = 0;
	return pipe;
}

}
}
