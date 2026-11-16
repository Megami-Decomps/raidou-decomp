#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "sdfModel.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfModel.c:1593, found, same function 1.00 (words 4/4, calls 1.00, shape 1.00) */
void sdfModelResetFaceDivParm(sdfModelRootNode_t *aRoot)
{
    aRoot->flags = aRoot->flags & ~SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}

INCLUDE_ASM(const s32, "code_00360EE8", sdfLeaf23);

INCLUDE_ASM(const s32, "code_00360EE8", sdfLeaf24);

INCLUDE_ASM(const s32, "code_00360EE8", sdfSetForcedAssetTextureMode);
