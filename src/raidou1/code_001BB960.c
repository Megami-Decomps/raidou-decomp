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

s32 evtLeaf9(s32 type) {
    switch (type) {
    case 10:
        return 112;
    case 14:
    case 15:
    case 23:
        return 44;
    case 13:
    case 27:
        return 40;
    case 16:
        return 24;
    case 17:
        return 36;
    case 11:
    case 25:
        return 64;
    default:
        return 0;
    }
}
