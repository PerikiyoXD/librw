#ifdef RW_PS2

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
#include "rw/rwanim.h"
#include "rw/rwplugins.h"
#include "rw/ps2/rwps2.h"
#include "rw/ps2/rwps2plg.h"

#include "rwps2impl.h"

#define PLUGIN_ID 2

namespace rw {
namespace ps2 {

Device renderdevice = {
	16777215.0f, 0.0f,
	null::beginUpdate,
	null::endUpdate,
	null::clearCamera,
	null::showRaster,
	null::rasterRenderFast,
	null::setRenderState,
	null::getRenderState,
	null::im2DRenderLine,
	null::im2DRenderTriangle,
	null::im2DRenderPrimitive,
	null::im2DRenderIndexedPrimitive,
	null::im3DTransform,
	null::im3DRenderPrimitive,
	null::im3DRenderIndexedPrimitive,
	null::im3DEnd,
	null::deviceSystem
};

}
}

#endif
