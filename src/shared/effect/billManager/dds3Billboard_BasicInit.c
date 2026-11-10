#include "shared/effect/billManager/decls.h"

void dds3Billboard_BasicInit(billHandle_t *pBasic) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    pBasic->mode = 0;
    pBasic->pos[3] = zero;
    pBasic->pos[2] = zero;
    pBasic->pos[1] = zero;
    pBasic->pos[0] = zero;
    pBasic->unk18 = one;
    pBasic->childScaleY = one;
    pBasic->unk1C = zero;
    pBasic->childScaleX = one;
    pBasic->abgr = 0x80808080;
    pBasic->rot = zero;
}
