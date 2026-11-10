#include "shared/effect/effBlur_Filter/decls.h"

/* Reset every slot, then stagger its initial phase by -spacing * index. */
void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur) {
    effBlurScaleSlot_t *slot = hRippleBlur->slots;
    s32 count = hRippleBlur->params.number;
    s32 i;

    for (i = 0; i < count; i++, slot++) {
        dds3EffectRipBlurPartInit(hRippleBlur, slot);
        slot->phase = -(hRippleBlur->params.interval * (f32)i);
    }
}
