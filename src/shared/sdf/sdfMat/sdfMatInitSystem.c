#include "shared/sdf/sdfMat/decls.h"

void sdfMatInitSystem(void)
{
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfMatListFree);
    sdfDelayedResrcInitCB(&sdfMatDelayedResrcCB2, (sdfDelayedResrcFreeFunc)sdfMatFree);
}
