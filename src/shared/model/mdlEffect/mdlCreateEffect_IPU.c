#include "shared/model/mdlEffect/decls.h"

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlCreateEffect_IPU(mdlObj_t *aHandle, mdlPibIpu_t *aData, s32 aPlayGroup) {
    mdlEffectResrcItem_t *partSlot = mdlGetEffectResrc(aHandle, aData->index);

    if (partSlot != 0) {
        mdlEffectResrc_IPU_t *object = partSlot->handle;
        if (object->inUse == 0) {
            sdfTexHandle_t *texture = sdfTexSearch(aData->dataId);
            if (texture != 0) {
                dds3ModelEffectCtrlr_t *resourceItem;
                u8 *attributes;
                object->inUse = 1;
                resourceItem = mdlCreateEffectCtrlr(aHandle, dds3ModelEffectType_IPU, aPlayGroup);
                /* Fill the object and resource data before attaching the owner. */
                resourceItem->payload.object.object = object;
                attributes = resourceItem->payload.object.attributes;
                resourceItem->payload.object.texture = texture;
                resourceItem->payload.object.owner = aHandle;
                resourceItem->payload.object.minimumTime = aData->minimumTime;
                attributes[1] = 1;
                attributes[2] = aData->loopMode;
                attributes[3] = aData->playbackCadenceStep;
            }
        }
    }
}
