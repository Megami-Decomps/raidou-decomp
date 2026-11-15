#include "common.h"
#include "EventBit.h"

typedef struct fldSceneEntry_t {
    s32 scene;
    s32 id;
    s32 unk8;
    s32 result;
    s32 bit;
    s32 value;
} fldSceneEntry_t;

extern fldSceneEntry_t fldFindSceneEntryDataData[2];
extern s32 mapCallSetOt2Data5;

s32 fldFindSceneEntryData(s32 id) {
    s32 i;

    for (i = 0; i < 2; i++) {
        fldSceneEntry_t *entry = &fldFindSceneEntryDataData[i];

        if (entry->scene == mapCallSetOt2Data5 && entry->id == id) {
            if (entry->bit >= 0 && evtBitCheck(entry->bit) == entry->value) {
                return entry->result;
            }
        }
    }
    return id;
}
