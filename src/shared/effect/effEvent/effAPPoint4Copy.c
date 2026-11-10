#include "shared/effect/effEvent/decls.h"

/* Clone channel work, allocate slots and duplicate a parameter template per record.
 * The record count is captured before allocation; a nonpositive delay modulus becomes one. */
effAPPoint4_tag *effAPPoint4Copy(effChanSource_t *source) {
    u32 recordCount = source->head.count;
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordCount * sizeof(effAPPoint4Part_tag) + sizeof(effAPPoint4_tag));
    effAPPoint4_tag *work = EE_ADDR_AS(effAPPoint4_tag *, sdfMemLock(allocationHandle));
    effAPPoint4Part_tag *recordCursor = (effAPPoint4Part_tag *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->param = source->head;
    work->hMem = allocationHandle;
    work->pPart = recordCursor;
    if (work->param.spread <= 0) {
        work->param.spread = 1;
    }
    work->pBezier4P = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->param.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->hPart = dds3EffectPartCopy(parameterTemplate);
            recordCursor->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}
