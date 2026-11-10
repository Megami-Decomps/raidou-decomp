#include "shared/effect/effBlur_Filter/decls.h"

/* Allocate an owner and slot array from only the serialized parameter prefix. */
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam) {
    s32 count = pParam->number;
    void *allocation = sdfMemAllocDirect(count * 0x30 + 0x38);
    effBlurScaleWork_t *work = EE_ADDR_AS(effBlurScaleWork_t *, sdfMemLock(allocation));
    effBlurScaleSlot_t *slot;
    s32 i = 0;

    /* Copy through size, leaving the source handle, allocation and slots owned
       by this new work rather than reading a live-work tail from src. */
    memcpy(work, pParam, 0x2C);
    work->allocation = allocation;
    work->slots = (effBlurScaleSlot_t *)(work + 1);
    work->texture = dds3BillboardGetGeneralTex(3);
    slot = work->slots;
    while (i < count) {
        dds3EffectRipBlurPartInit(work, slot);
        slot->phase = -(work->params.interval * (f32)i);
        i++;
        slot++;
    }
    return work;
}
