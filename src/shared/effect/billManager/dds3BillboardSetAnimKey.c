#include "shared/effect/billManager/decls.h"

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame) {
    if (hBillboard->type == 1) {
        s32 entryCount = hBillboard->entryCount;

        if (entryCount > 0) {
            dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
            s32 remaining = entryCount;

            do {
                P2A_ANIMATION_tag *frameData = entry->pAnim;
                u32 wrappedFrame = frame % frameData->times;
                remaining -= 1;
                entry->wait = 0;
                entry->key = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}
