#include "shared/model/mdlEffect/decls.h"

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr) {
    switch (aCtrlr->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardDelete((mdlBillboard_t *)aCtrlr->payload.part.instance);
        break;
    case dds3ModelEffectType_D3P:
        dds3EffectDelete((mdlEffect_t *)aCtrlr->payload.part.instance);
        break;
    case dds3ModelEffectType_TRACK:
        Eff_ModelTrackPolyDelete(aCtrlr->payload.part.track);
        break;
    }
    sdfChipFreeDirect((void *)aCtrlr);
}
