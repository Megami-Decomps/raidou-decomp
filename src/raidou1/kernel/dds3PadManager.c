#include "common.h"

#include "shared/kernel/dds3PadManager/dds3PadVibrationTest.c"

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadInitialize);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00103BE8);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_001040E0);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104140);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104210);

#include "shared/kernel/dds3PadManager/dds3UpdateVibrationSlots.c"
INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1B0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1B1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1BF);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C2);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C3);

