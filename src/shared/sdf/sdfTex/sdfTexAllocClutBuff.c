#include "shared/sdf/sdfTex/decls.h"

void sdfTexAllocClutBuff(sdfTexHandle_t *aHandle)
{
    s32 size;

    size = sdfTexGetClutDataSize(aHandle->pixelFormat, aHandle->CLUTFormat, aHandle->numOfCLUT);
    aHandle->clutDataSize = size;
    aHandle->clutData = sdfAllocAuto(size);
}
