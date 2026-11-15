#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "kernel.h"
#include "newdata.h"
#include "ps2asm.h"
#include "motion.h"
#include "evtMoon.h"
#include "EventBit.h"

void evtBitAllOff(void);
void evtBitOff(s32 no);

/* dds3ModelEffectCtrlr_t.type: which effect a controller drives. */
typedef enum dds3ModelEffectType {
    dds3ModelEffectType_P2A = 0,
    dds3ModelEffectType_D3P = 1,
    dds3ModelEffectType_TRACK = 2,
    dds3ModelEffectType_IPU = 3
} dds3ModelEffectType;

/* pibHeaderChunk_t.kind, as mdlCreateEffect dispatches on it. */
typedef enum PIB_CHUNK_TYPE {
    PIB_CHUNK_TYPE_P2A = 1,
    PIB_CHUNK_TYPE_D3P = 2,
    PIB_CHUNK_TYPE_TRACK = 3,
    PIB_CHUNK_TYPE_BLUR = 4,
    IPU = 5,
    PIB_CHUNK_TYPE_END = 0xFFFF
} PIB_CHUNK_TYPE;

void evtInitGlobalWork(void);
#ifdef VERSION_RAIDOU1
void evtBitAllOff_FieldDebug(void);
#endif

#define BIT_SHIFT(no) (no)

void evtBitOn(s32 no);
s32 evtBitCheck(s32 no);

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}

#ifdef VERSION_RAIDOU1
void evtBitAllOff(void) {
    s32 i = 0x7f;
    u32 *word = DDS3_GBWK->EventBit;
#else
INCLUDE_ASM(const s32, "event/EventBit", evtBitAllOff);
#endif

#ifdef VERSION_RAIDOU1
    do {
        i -= 1;
        *word = 0;
        word += 1;
    } while (i >= 0);
}

void evtBitAllOff_FieldDebug(void) {
    s32 i = 0;
    do {
        evtBitOff(i++);
    } while (i < 0xC00);
    i = 0xD00;
    do {
        evtBitOff(i++);
    } while (i < 0x1000);
}
#else
INCLUDE_ASM(const s32, "event/EventBit", evtBitAllOff_FieldDebug);
#endif

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void evtBitOn(s32 no) {
    s32 adjustedFlag = (no < 0) ? no + 0x1f : no;
    DDS3_GBWK->EventBit[adjustedFlag >> 5] |= 1 << BIT_SHIFT(no);
}

void evtBitOff(s32 no) {
    s32 adjustedFlag = (no < 0) ? no + 0x1f : no;
    DDS3_GBWK->EventBit[adjustedFlag >> 5] &= ~(1 << BIT_SHIFT(no));
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "event/EventBit", fldProc_CallSisetuReturn);

#endif
s32 evtBitCheck(s32 no) {
    s32 adjustedFlag = (no < 0) ? no + 0x1f : no;
    return (((s32)DDS3_GBWK->EventBit[adjustedFlag >> 5] >> BIT_SHIFT(no)) & 1);
}
