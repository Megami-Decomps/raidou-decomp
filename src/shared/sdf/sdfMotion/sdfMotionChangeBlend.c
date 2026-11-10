#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionChangeBlend(sdfMotionCtrlr_t *aCtrlr, s32 aCutNum, s32 aType, f32 aPreFrame,
                          f32 aBlendLeng)
{
    s32 i;
    sdfMotionCutDef_t *cut;
    sdfMotionCutChunk_t *chunk;
    sdfMotionNode_t *node;

    aCtrlr->cutNumber = aCutNum;
    if (aBlendLeng > 0.0f) {
        aCtrlr->blendLength = aBlendLeng;
        aCtrlr->blendPreLength = -aPreFrame;
        for (i = 0; i < aCtrlr->nodeList->count; i++) {
            node = aCtrlr->nodeList->items[i];
            node->vtbl->initBlend(node);
        }
    } else {
        aCtrlr->blendLength = 0.0f;
        aCtrlr->blendPreLength = 0.0f;
    }
    aCtrlr->playType = aType;
    aCtrlr->stat = SDF_MOTION_STAT_SET;
    cut = aCtrlr->playbook->cutList[aCutNum];
    aCtrlr->frame = aCtrlr->blendPreLength;
    if (cut == NULL) {
        aCtrlr->frameLength = 0;
        aCtrlr->stat = SDF_MOTION_STAT_NOCUT;
        return;
    }
    aCtrlr->frameLength = cut->frameLength;
    chunk = cut->chunk;
    for (i = 0; i < aCtrlr->nodeList->count; i++) {
        node = aCtrlr->nodeList->items[i];
        node->vtbl->setCut(node, chunk);
        chunk = (sdfMotionCutChunk_t *)((u8 *)chunk + chunk->size);
    }
}
