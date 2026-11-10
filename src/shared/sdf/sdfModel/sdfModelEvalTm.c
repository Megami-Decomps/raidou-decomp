#include "shared/sdf/sdfModel/decls.h"

void sdfModelEvalTm(sdfModelRootNode_t *aRoot)
{
    sdfModelEvalTm2(aRoot, (s8)sdfFrameId);
}
