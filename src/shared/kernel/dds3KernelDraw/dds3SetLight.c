#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetLight(void *light, s32 no)
{
    dds3LightParam.parallel[no] = light;
}
