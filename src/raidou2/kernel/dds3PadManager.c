#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3PadManager.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "sdfFrame.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

extern u8 dds3PadVibrationState[2];

/* Second view of dds3PadVibrationState[1]: the retail load is its own, not the pointer's. */
SYMBOL_AT(u8, dds3PadVibrationCurrent, dds3PadVibrationState, 1);

extern u8 dds3PadVibrationMode[2];

extern s32 dds3PadVibrationTable[2];

extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 dds3GetGlobalConfigFlag(u32 flag);

extern void dds3GetVibrationLevel(void);

extern void sdfPadSetAct0(s32 pad, u16 value);

extern void sdfPadSetAct1(s32 pad, u8 value);

extern void sdfPadSetAct(s32 pad, s32 value0, s32 value1);

extern u8 dds3PadButtonState[64];

extern u8 dds3PadButtonPrev[64];

extern u32 D_002F56C0[64];

extern u32 D_002F57C0[4];

extern u8 dds3PadStickPrev[8];

extern s8 dds3PadStickStatusPrev[4];

extern u8 dds3PadStickState[8];

extern u8 dds3PadStickStatusState[4];

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

s32 dds3PadVibrationTest(dds3ProcessID_t *task);

void dds3PadInitialize(void);

extern s8 dds3PadBitTable[32];

extern u8 D_005518E0[8];

extern void dds3UpdateVibrationSlots(void);

extern void effPadVibeFadeUpdate(void);

extern f32 acosf(f32 x);

void dds3RunProcessList(void);

#include "sdk_libc.h"

void dds3GetVibrationLevel(void);

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

void dds3_104170(void);

void dds3UpdateVibrationSlots(void);
/* End of declarations from src/smt3/kernel/dds3PadManager.c */

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00103A88);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadInitialize);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00103C18);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104110);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104170);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104240);

void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = 1;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = dds3PadVibrationMode;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < 2; motor++) {
        if (dds3PadVibrationTable[motor] != 0) {
            /* The native countdown wraps as a 32-bit word, including negative durations. */
            dds3PadVibrationTable[motor] = (s32)((u32)dds3PadVibrationTable[motor] - 1);
            if (dds3PadVibrationTable[motor] == 0) {
                levels[motor] = 0;
                if (motor == 0) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + 1;
                if (*largeLevel != targets[1]) {
                    dds3GetVibrationLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}
INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2A8);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2A9);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2B7);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2B8);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2B9);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2BA);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_004DE2BB);

