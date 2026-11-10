#include "shared/effect/billManager/decls.h"

void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode) {
    mode = (s16)mode;
    switch (hBillboard->type) {
    case 0:
    case 3:
        hBillboard->mode = mode;
        break;
    case 1: {
        s32 entryCount = hBillboard->entryCount;
        if (entryCount > 0) {
            s32 remaining = entryCount;
            dds3BillAnim_tag *entries = hBillboard->resolvedEntries;
            s32 index = 0;
            do {
                P2A_ANIMATION_tag *entry = entries[index].pAnim;
                u32 flags = entry->mode & ~BILL_ANIMATION_FLAG_PACKET_LIST_MASK;
                entry->mode = flags;
                if (mode == 2) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_2;
                } else if (mode == 3) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_3;
                }
                index++;
            } while (--remaining != 0);
        }
        break;
    }
    }
}
