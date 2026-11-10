#include "shared/model/mdlManager/decls.h"

void mdlSetClutColor(mdlObj_t *ctx, s32 color, f32 ratio)
{
    sdfTexListSetClutColor((sdfItemArray_t *)ctx->inner->texList, color, ratio);
}
