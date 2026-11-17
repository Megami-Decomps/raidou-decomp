#include "common.h"
#include "dds3KernelCore.h"

extern const char itfStrTestFontCalc[];
extern const char itfStrTestFontDraw[];

INCLUDE_RODATA(const s32, "code_r1_00183C98", itfStrTestFontCalc);

INCLUDE_RODATA(const s32, "code_r1_00183C98", itfStrTestFontDraw);

INCLUDE_ASM(const s32, "code_r1_00183C98", mjnTestFontCalc);

void itfTestFontCalc(void) {
    dds3KillProcessName(itfStrTestFontCalc, 0);
    dds3KillProcessName(itfStrTestFontDraw, 0);
}

INCLUDE_ASM(const s32, "code_r1_00183C98", itfTestFontCalc2);
