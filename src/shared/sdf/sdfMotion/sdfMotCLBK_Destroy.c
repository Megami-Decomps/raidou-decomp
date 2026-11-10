#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_Destroy(sdfMotionNode_t *aNode)
{
    sdfChipFreeDirect(aNode);
}
