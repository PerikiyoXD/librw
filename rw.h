#ifndef RW_H
#define RW_H

#include <cstdio>
#include <cstdlib>
#include <cmath>

// Base includes
#include "src/rwbase.h"
#include "src/rwerror.h"
#include "src/rwplg.h"
#include "src/rwrender.h"
#include "src/rwengine.h"
#include "src/rwpipeline.h"
#include "src/rwobjects.h"
#include "src/rwanim.h"
#include "src/rwplugins.h"
#include "src/rwuserdata.h"
#include "src/rwcharset.h"

// Platform-specific includes
#ifdef __PS2__
#include "src/ps2/rwps2.h"
#include "src/ps2/rwps2plg.h"
#endif

#ifdef __D3D__
#include "src/d3d/rwxbox.h"
#include "src/d3d/rwd3d.h"
#include "src/d3d/rwd3d8.h"
#include "src/d3d/rwd3d9.h"
#endif

#ifdef __GL__
#include "src/gl/rwwdgl.h"
#include "src/gl/rwgl3.h"
#include "src/gl/rwgl3shader.h"
#include "src/gl/rwgl3plg.h"
#endif

#endif // RW_H
