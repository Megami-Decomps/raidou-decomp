#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_SetCut(sdfMotionNode_t *aNode, sdfMotionCutChunk_t *aTable)
{
    aNode->table = aTable;
}
