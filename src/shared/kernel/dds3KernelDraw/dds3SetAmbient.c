#include "shared/kernel/dds3KernelDraw/decls.h"

void dds3SetAmbient(sdfVector4_t *ambient)
{
    SDF_VU0_COPYVECTOR(&dds3LightParam.ambient, ambient);
}
