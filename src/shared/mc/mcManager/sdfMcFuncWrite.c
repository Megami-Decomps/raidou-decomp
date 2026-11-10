#include "shared/mc/mcManager/decls.h"

void sdfMcFuncWrite(s32 descriptor, const void *buffer, s32 size)
{
    sceMcWrite(descriptor, buffer, size);
}
