#include "shared/sdf/sdfTex/decls.h"

void sdfTexSetWrapMode(sdfTexHandle_t *aHandle, s32 aMode)
{
    aHandle->texWrapMode = aMode;
    sdfTexRebuildPacket(aHandle);
}
