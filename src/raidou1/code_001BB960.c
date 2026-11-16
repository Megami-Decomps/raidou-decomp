#include "common.h"
#include "sdfChip.h"
#include "evtEventViewer.h"

void evtCallChipFreeDirect(void) {
    s32 i;

    for (i = 0; i < evtWork2.count; i++) {
        if (evtWork2.info[i].chip != NULL) {
            sdfChipFreeDirect(evtWork2.info[i].chip);
        }
        evtWork2.info[i].chip = NULL;
    }
    evtWork2.count = 0;
    evtWork2.unk2B0 = 0;
}

INCLUDE_ASM(const s32, "code_001BB960", evtLeaf9);
