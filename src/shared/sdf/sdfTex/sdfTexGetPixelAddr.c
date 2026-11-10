#include "shared/sdf/sdfTex/decls.h"

u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle)
{
    return aHandle->pixelGsHandle->addr;
}
