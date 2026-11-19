#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "sdfModel.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/sdf/sdfModel.c:1593, found, same function 1.00 (words 4/4, calls 1.00, shape 1.00) */
void sdfModelResetFaceDivParm(sdfModelRootNode_t *aRoot)
{
    aRoot->flags = aRoot->flags & ~SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/sdf/sdfModel.c:1606, adapt, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
f32 sdfModelGetFaceDivSoften(sdfModelRootNode_t *aRoot)
{
    if (aRoot->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        return aRoot->faceDivSoften;
    }
    return sdfFaceDivSoften;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/sdf/sdfModel.c:1606, adapt, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
f32 sdfModelGetFaceDivBias(sdfModelRootNode_t *aRoot)
{
    if (aRoot->flags & SDF_MODEL_FLAG_SELF_FACEDIV_PARM) {
        return aRoot->faceDivBias;
    }
    return sdfFaceDivBias;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/sdf/sdfMat.c:74, adapt, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void sdfSetForcedAssetTextureMode(s32 aMode)
{
    sdfForcedAssetTextureMode = aMode;
}
