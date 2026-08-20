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
skinOpen(void *o, int32, int32)
{
	skinGlobals.pipelines[PLATFORM_D3D8] = makeSkinPipeline();
	return o;
}

static void*
skinClose(void *o, int32, int32)
{
	((ObjPipeline*)skinGlobals.pipelines[PLATFORM_D3D8])->destroy();
	skinGlobals.pipelines[PLATFORM_D3D8] = nil;
	return o;
}

void
initSkin(void)
{
	Driver::registerPlugin(PLATFORM_D3D8, 0, ID_SKIN,
	                       skinOpen, skinClose);
}

ObjPipeline*
makeSkinPipeline(void)
{
	ObjPipeline *pipe = ObjPipeline::create();
	pipe->instanceCB = defaultInstanceCB;
	pipe->uninstanceCB = defaultUninstanceCB;
	pipe->renderCB = defaultRenderCB;
	pipe->pluginID = ID_SKIN;
	pipe->pluginData = 1;
	return pipe;
}

}
}
