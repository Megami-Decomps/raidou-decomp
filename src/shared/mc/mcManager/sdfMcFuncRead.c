#include "shared/mc/mcManager/decls.h"

void sdfMcFuncRead(s32 descriptor, void *buffer, s32 size)
{
    sceMcRead(descriptor, buffer, size);
}
