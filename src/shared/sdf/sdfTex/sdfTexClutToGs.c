#include "shared/sdf/sdfTex/decls.h"

void sdfTexClutToGs(sdfTexHandle_t *aHandle)
{
    if (aHandle->clutGsHandle != NULL) {
        sdfTexTransCLUT(aHandle, sdfTexGetCLUTAddr(aHandle), aHandle->clutData, 0);
    }
}
