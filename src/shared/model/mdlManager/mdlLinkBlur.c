#include "shared/model/mdlManager/decls.h"

void mdlLinkBlur(mdlObj_t *ctx, struct sdfGrOt_t **surfaces) {
    dds3ModelBlurCb_t *list = ctx->devList;
    sdfModelRootNode_t *original = ctx->inner;
    dds3ModelBlurInstance_t *slot;
    sdfModelRootNode_t *model;
    sdfVector4_t tint;
    s32 remaining;
    u32 color;

    if (list->spawnCount != 0 && --list->intervalCount == 0) {
        list->spawnCount--;
        list->intervalCount = list->interval;
        slot = sdfChipAllocDirect(sizeof(dds3ModelBlurInstance_t));
        model = sdfModelMake(ctx->sub->texList, ctx->sub->modelDefPtr);
        /* Finish initialization before publishing the new list head. */
        slot->next = list->instants;
        slot->modelobj = model;
        slot->keepCount = 1;
        model->zBias = 1000.0f;
        list->instants = slot;
    }
    UnpackRGBAWordFloat(original->color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR(&tint);
    slot = list->instants;
    while (slot != NULL) {
        remaining = slot->keepCount;
        model = slot->modelobj;
        remaining--;
        if (remaining == 0) {
            remaining = list->keepCount;
            sdfModelCopyStat(model, original);
        } else {
            sdfModelEvalTm(model);
        }
        /* Both callbacks complete before the countdown is published. */
        slot->keepCount = remaining;
        SDF_VU0_LOADVECTOR_BARRIER(list->colorBase);
        SDF_VU0_LOADVECTOR_B(list->colorDelta);
        SDF_VU0_LOADVECTOR_C(&tint);
        color = PackDevSlotColor(remaining - 1, 128.0f);
        model->color = color;
        sdfModelLinkOt(surfaces, model);
        slot = slot->next;
    }
}
