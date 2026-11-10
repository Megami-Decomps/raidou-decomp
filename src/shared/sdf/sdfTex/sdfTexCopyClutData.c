#include "shared/sdf/sdfTex/decls.h"

void sdfTexCopyClutData(sdfTexHandle_t *aHandle, void *aData)
{
    memcpy(aHandle->clutData, aData, aHandle->clutDataSize);
}
