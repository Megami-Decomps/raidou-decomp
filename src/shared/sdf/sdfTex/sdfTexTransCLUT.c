#include "shared/sdf/sdfTex/decls.h"

void *sdfTexTransCLUT(sdfTexHandle_t *aHandle, s32 aGsAddr, void *aDataPtr, s32 aMemMode)
{
    s32 width;
    s32 height;

    if (aHandle->pixelFormat == 19 || aHandle->pixelFormat == 27) {
        width = 16;
        height = 16;
    } else {
        width = 8;
        height = 2;
    }
    return sdfTexTransData(aGsAddr, width, height, aHandle->CLUTFormat, aDataPtr, aMemMode);
}
