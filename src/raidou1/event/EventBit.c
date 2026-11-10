#include "common.h"

#include "shared/event/EventBit/evtInitGlobalWork.c"

INCLUDE_ASM(const s32, "event/EventBit", evtBitAllOff);

#include "shared/event/EventBit/evtBitAllOff_FieldDebug.c"

INCLUDE_ASM(const s32, "event/EventBit", evtBitOn);

INCLUDE_ASM(const s32, "event/EventBit", evtBitOff);

INCLUDE_ASM(const s32, "event/EventBit", evtBitCheck);
