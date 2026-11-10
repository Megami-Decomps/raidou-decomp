#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/EventBit.c */
#include "common.h"

#include "libsdf.h"

#include "model.h"

#include "kernel.h"

#include "newdata.h"

#include "ps2asm.h"

#include "motion.h"

extern void evtSetAgeOfMoon(u8);

extern void evtDisableMoonDraw(void);

extern void evtDisableMoonCalc(void);

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

void evtBitAllOff(void);

void evtBitAllOff_FieldDebug(void);

#define BIT_SHIFT(no) (no)

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void evtBitOn(s32 no);

void evtBitOff(s32 no);

s32 evtBitCheck(s32 no);
/* End of declarations from src/smt3/event/EventBit.c */

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}

INCLUDE_ASM(const s32, "event/EventBit", evtBitAllOff);

INCLUDE_ASM(const s32, "event/EventBit", func_001B8308);

INCLUDE_ASM(const s32, "event/EventBit", evtBitOn);

INCLUDE_ASM(const s32, "event/EventBit", evtBitOff);

INCLUDE_ASM(const s32, "event/EventBit", func_001B83E8);

INCLUDE_ASM(const s32, "event/EventBit", evtBitCheck);
