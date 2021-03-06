#include <cstdio>
#include <cmath>

#include "src/rwbase.h"
#include "src/rwerror.h"
#include "src/rwplg.h"
#include "src/rwpipeline.h"
#include "src/rwobjects.h"
#include "src/rwanim.h"
#include "src/rwengine.h"
#include "src/rwplugins.h"
#include "src/ps2/rwps2.h"
#include "src/ps2/rwps2plg.h"
#include "src/d3d/rwxbox.h"
#include "src/d3d/rwd3d.h"
#include "src/d3d/rwd3d8.h"
#include "src/d3d/rwd3d9.h"
#ifdef RW_OPENGL
#include <GL/glew.h>
#endif
#include "src/gl/rwwdgl.h"
#include "src/gl/rwgl3.h"
#include "src/gl/rwgl3shader.h"
