#include "shared/sdf/sdfTex/decls.h"

u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->clamp.data;
}
