#include "shared/model/mdlEffect/decls.h"

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlCreateEffect_P2A_D3P(mdlObj_t *owner, pibChunkItem_P2A_D3P_t *partRecord, s32 motionSlotIndex, s32 resourceType, void *(*createPart)(mdlEffectResrcItem_t *)) {
    mdlEffectResrcItem_t *partSlot = mdlGetEffectResrc(owner, partRecord->effectIndex);

    if (partSlot != NULL) {
        sdfModelRootNode_t *model = owner->inner;
        s32 recordId = partRecord->markId;
        s32 remainingRecords = partRecord->count;
        f32 optionalAnchorScale = 0.0f;

        /* Retain the native 17-byte size threshold for the optional anchor scale. */
        if (partRecord->size >= MDL_PART_VALUE_SIZE_THRESHOLD) {
            optionalAnchorScale = partRecord->zOffset;
        }
        do {
            mdlMarkPos_t *chunkRecord = sdfModelSearchMarkPosData(model, recordId++);

            if (chunkRecord != NULL) {
                dds3ModelEffectCtrlr_t *resourceItem = mdlCreateEffectCtrlr(owner, resourceType, motionSlotIndex);

                resourceItem->payload.part.instance = createPart(partSlot);
                resourceItem->payload.part.slot = partSlot;
                resourceItem->payload.part.mapPositionRecord = chunkRecord;
                resourceItem->payload.part.anchorScale = optionalAnchorScale;
            }
        } while (--remainingRecords != 0);
    }
}
