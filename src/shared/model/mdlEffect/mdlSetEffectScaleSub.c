#include "shared/model/mdlEffect/decls.h"

/* Set billboard/effect scale; other controller kinds have no scale dispatch. */
void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, float aScale) {
    switch (aEffect->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardSetScale((struct mdlBillboard_t *)aEffect->payload.part.instance, aScale, aScale);
        return;
    case dds3ModelEffectType_D3P:
        dds3EffectSetScale((mdlEffect_t *)aEffect->payload.part.instance, aScale);
        break;
    }
}
