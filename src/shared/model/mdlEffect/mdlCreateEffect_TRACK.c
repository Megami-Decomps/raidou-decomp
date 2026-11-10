#include "shared/model/mdlEffect/decls.h"

/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateEffect_TRACK(mdlObj_t *owner, pibChunkItem_TRACK_t *record, s32 motionSlotIndex) {
    mdlTrackCreateParams_t effectParams;
    dds3ModelEffectCtrlr_t *resourceItem;

    effectParams.model = owner;
    effectParams.idA = record->mark;
    effectParams.idB = record->endPositionId;
    effectParams.startFrame = record->startFrame;
    effectParams.endFrame = record->endFrame;
    effectParams.sampleInterval = record->interval;
    effectParams.historyLength = record->face;
    effectParams.unk1C = 3;
    effectParams.kind = record->blendAlgo;
    effectParams.gradientColors[0] = record->colors[0];
    effectParams.gradientColors[1] = record->colors[1];
    effectParams.gradientColors[2] = record->colors[2];
    effectParams.gradientColors[3] = record->colors[3];
    resourceItem = mdlCreateEffectCtrlr(owner, dds3ModelEffectType_TRACK, motionSlotIndex);
    resourceItem->payload.part.track = Eff_ModelTrackPolyCreate(&effectParams);
}
