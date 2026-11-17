#include "common.h"
#include "model.h"

void sdfModelResetFaceDivParm(sdfModelRootNode_t *aRoot)
{
    aRoot->flags = aRoot->flags & ~SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}

INCLUDE_ASM(const s32, "code_003D6C70", sdfLeaf23);

INCLUDE_ASM(const s32, "code_003D6C70", sdfLeaf24);

INCLUDE_ASM(const s32, "code_003D6C70", sdfSetForcedAssetTextureMode);
