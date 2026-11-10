#include "shared/sdf/sdfDelayedResrc/decls.h"

u64 sdfDelayedResrcCheckFunc(void) {
    u32 slotIndex;
    saddr_t *slotCursor;
    if (sdfDelayedResrcFlag != 0) {
        return 1;
    }
    if (sdfDelayedResrcLink != NULL) {
        return 1;
    }
    slotIndex = 0;
    slotCursor = D_005520A8;
    do {
        if (*slotCursor != 0) {
            return 1;
        }
        slotCursor++;
        slotIndex++;
    } while (slotIndex < SDF_PENDING_SLOT_COUNT);
    return 0;
}
