#include "shared/model/mdlEffect/decls.h"

/* Set billboard/effect color; other controller kinds have no RGBA dispatch. */
void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor) {
    switch (aEffect->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardSetRGBA((mdlBillboard_t *)aEffect->payload.part.instance, aColor);
        return;
    case dds3ModelEffectType_D3P:
        dds3EffectSetRGBA((mdlEffect_t *)aEffect->payload.part.instance, aColor);
        break;
    }
}
