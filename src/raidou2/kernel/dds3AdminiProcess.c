#include "common.h"

#include "shared/kernel/dds3AdminiProcess/GetAdminiProcessData.c"

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_001029E0);

#include "shared/kernel/dds3AdminiProcess/dds3AdminiCallSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiOverwriteSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiExitSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiGetActiveSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiGetNextSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiGetPrevSequence.c"

#include "shared/kernel/dds3AdminiProcess/dds3AdminiGetPrevHistoryFlag.c"

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102BF8);

#include "shared/kernel/dds3AdminiProcess/dds3AdminiEndCheck.c"

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00102DF0);

#include "shared/kernel/dds3AdminiProcess/dds3AdminiNeoShutdown.c"

#include "shared/kernel/dds3AdminiProcess/dds3InitAdmini.c"

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFindFreeSlot);

#include "shared/kernel/dds3AdminiProcess/fclEncycFreeEntry.c"

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycFreeEntries);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00103518);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", fclEncycAllocEntry);

INCLUDE_ASM(const s32, "kernel/dds3AdminiProcess", func_00103658);

INCLUDE_RODATA(const s32, "kernel/dds3AdminiProcess", D_004A6F98);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE290);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE298);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE29C);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A0);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A4);

INCLUDE_SDATA(const s32, "kernel/dds3AdminiProcess", D_004DE2A7);

