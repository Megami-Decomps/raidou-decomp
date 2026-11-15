#include "common.h"
#include "EventBit.h"

typedef struct fldBitCheckEntry_t {
    s32 scene;
    s32 id;
    s32 bit;
    s32 value;
} fldBitCheckEntry_t;

extern fldBitCheckEntry_t fldCallBitCheckData[44];
extern s32 mapCallSetOt2Data5;
extern s32 mapCallSetOt2Data6;

/* NON_MATCHING: 27 of 92 words match. Register allocation differs, retail keeps extra address copies. */
#ifdef NON_MATCHING
s32 fldCallBitCheck(u32 n) {
    s32 i;
    s32 result;
    fldBitCheckEntry_t *base;
    fldBitCheckEntry_t *target;

    if (n == 0) {
        return -1;
    }
    for (i = 0; i < 44; i++) {
        base = &fldCallBitCheckData[i];
        if (base->scene != mapCallSetOt2Data5 || base->id != mapCallSetOt2Data6) {
            continue;
        }
        if (i + n >= 44) {
            return -1;
        }
        target = &fldCallBitCheckData[i + n];
        if (target->scene != mapCallSetOt2Data5) {
            continue;
        }
        if (base->bit < 0) {
            return target->id;
        }
        result = evtBitCheck(base->bit);
        if (result != base->value) {
            continue;
        }
        if (target->bit < 0) {
            continue;
        }
        if (result != target->value) {
            continue;
        }
        if (evtBitCheck(target->bit) == target->value) {
            return target->id;
        }
    }
    return -1;
}
#else
INCLUDE_ASM(const s32, "code_r1_0016AC20", fldCallBitCheck);
#endif
