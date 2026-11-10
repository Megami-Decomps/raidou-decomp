#include "shared/sdf/sdfTex/decls.h"

void sdfTexInit(void)
{
    sdfDelayedResrcInitCB(&sdfTexDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfTexFree);
}
