#ifndef RW_GL_RWGL3PLG_H
#define RW_GL_RWGL3PLG_H

namespace rw {
namespace gl3 {

void initMatFX(void);
ObjPipeline *makeMatFXPipeline(void);
void matfxRenderCB(Atomic *atomic, InstanceDataHeader *header);

void initSkin(void);
ObjPipeline *makeSkinPipeline(void);
void skinInstanceCB(Geometry *geo, InstanceDataHeader *header, bool32 reinstance);
void skinRenderCB(Atomic *atomic, InstanceDataHeader *header);
void uploadSkinMatrices(Atomic *atomic);


}
}

#endif
