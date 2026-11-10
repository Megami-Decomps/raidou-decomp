#include "shared/sdf/sdfModel/decls.h"

void sdfModelSetFaceDivParm(sdfModelRootNode_t *aRoot, f32 aSoften, f32 aBias)
{
    aRoot->faceDivSoften = aSoften;
    aRoot->faceDivBias = aBias;
    aRoot->flags = aRoot->flags | SDF_MODEL_FLAG_SELF_FACEDIV_PARM;
}
