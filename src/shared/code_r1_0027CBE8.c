#include "common.h"

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"
#include "dds/btl.h"

extern u32 btlRandomState;
extern s32 (*btlPackedEffectHandlers[])(s32, u32);

/* Actor-status providers select one complete 0x270-byte model record. */

typedef struct BtlObjLink {
    void *owner; /* 0x00: effect owner */
    union {
        s32 arg;
        u16 shortArg; /* Message/wait variants store only the low halfword. */
    };
    s32 elapsedTicks; /* 0x08: elapsed callback updates */
} BtlObjLink;

typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 dialogId;          /* 0x498 */
    s32 alternateDialogId; /* 0x49C */
    s32 thirdDialogId;     /* 0x4A0 */
} BtlJyokyoState;

u32 GetRand(u32 limit);
extern u32 GetRand(u32 limit);
s32 GetIntervalRand(s32 lower, s32 upper);

u32 GetRand(u32 limit) {
    btlRandomState = btlRandomState * 0x41c64e6d + 0x3039;
    return (btlRandomState >> 0x10) * (limit & 0xffff) >> 0x10;
}

s32 GetIntervalRand(s32 lower, s32 upper) {
    s32 lo = upper;
    s32 hi = lower;

    if (lo < hi) {
        s32 t = lo;
        lo = hi;
        hi = t;
    }
    lo = lo - hi + 1;
    return (s32)GetRand(0xFFFF) % lo + hi;
}

INCLUDE_SDATA(const s32, "code_r1_0027CBE8", GetRandData);

