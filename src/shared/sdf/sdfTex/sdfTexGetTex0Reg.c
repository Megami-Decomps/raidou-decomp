#include "shared/sdf/sdfTex/decls.h"

u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle)
{
    return aHandle->packet->tex0.data;
}
