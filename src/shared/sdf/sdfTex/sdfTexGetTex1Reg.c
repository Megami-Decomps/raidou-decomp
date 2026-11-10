#include "shared/sdf/sdfTex/decls.h"

u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->tex1.data;
}
