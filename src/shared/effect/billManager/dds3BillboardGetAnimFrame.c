#include "shared/effect/billManager/decls.h"

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        const dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
        const P2A_ANIMATION_tag *frame = entry->pAnim;
        return frame->times;
    }
    return 0;
}
