#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetFog(s32 *fog)
{
    memcpy(&dds3LightParam.fog, fog, 0x14);
}
