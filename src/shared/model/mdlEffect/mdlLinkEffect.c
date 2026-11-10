#include "shared/model/mdlEffect/decls.h"

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlLinkEffect(mdlObj_t *owner, dds3ModelEffectCtrlr_t *anchorRecord) {
    sdfModelRootNode_t *model = owner->inner;
    sdfVector4_t position;
    void *resourceInstance;

    switch (anchorRecord->type) {
    case dds3ModelEffectType_P2A:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, &position.x);
        resourceInstance = anchorRecord->payload.part.instance;
        dds3BillboardSetPosition(resourceInstance, &position);
        dds3BillboardDraw((mdlBillboard_t *)resourceInstance);
        break;
    case dds3ModelEffectType_D3P:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, &position.x);
        resourceInstance = anchorRecord->payload.part.instance;
        dds3EffectSetPosition((mdlEffect_t *)resourceInstance, &position);
        dds3EffectDraw((mdlEffect_t *)resourceInstance);
        break;
    case dds3ModelEffectType_TRACK:
        Eff_ModelTrackPolyDraw(anchorRecord->payload.part.track);
        break;
    case dds3ModelEffectType_IPU:
        mdlDrawEffect_IPU(anchorRecord);
        break;
    }
}
