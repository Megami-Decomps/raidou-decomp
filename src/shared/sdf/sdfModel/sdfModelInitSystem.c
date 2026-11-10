#include "shared/sdf/sdfModel/decls.h"

void sdfModelInitSystem(void)
{
    sdfDelayedResrcInitCB(&sdfModelDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfModelRootFreeResrc);
}
