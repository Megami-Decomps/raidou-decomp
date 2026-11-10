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

extern u8 dds3PadVibrationCurrent;

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

extern u32 dds3PadButtonHoldCounters[64];

extern u32 dds3PadStickHoldCounters[4];

extern u8 dds3PadStickPrev[8];

extern s8 dds3PadStickStatusPrev[4];

extern u8 dds3PadStickState[8];

extern u8 dds3PadStickStatusState[4];

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

s32 dds3PadVibrationTest(dds3ProcessID_t *task);

void dds3PadInitialize(void);

extern s8 dds3PadBitTable[32];

extern u8 sdfPadAnalogSticks[8];

extern void dds3UpdateVibrationSlots(void);

extern void effPadVibeFadeUpdate(void);

extern f32 acosf(f32 x);

void dds3RunProcessList(void);

#include "sdk_libc.h"

void dds3GetVibrationLevel(void);

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

void dds3PadVibrationReset(void);

void dds3UpdateVibrationSlots(void);

extern const char dds3StartMenuModeSelect[16];

extern const char dds3StartMenuEndSaveTest[16];

extern const char dds3StartMenuStaffTest[16];

extern const char dds3StartMenuMovieViewer[16];

extern const char dds3StartMenuPuzzleBoyGame[16];

extern const char dds3StartMenuMagatuhiEditor[16];

extern const char dds3StartMenuSpriteViewer[16];

extern const char dds3StartMenuParticleEditor[16];

extern const char dds3StartMenuFontTest[16];

extern const char dds3StartMenuSoundTest[16];

extern const char dds3StartMenuModelViewer[16];

extern const char dds3StartMenuNameEntryTest[16];

extern const char dds3StartMenuResult2Test[16];

extern const char dds3StartMenuTerminalTest[16];

extern const char dds3StartMenuRecoverTest[16];

extern const char dds3StartMenuRagTest[16];

extern const char dds3StartMenuShopTest[16];

extern const char dds3StartMenuCombineTest[16];

extern const char dds3StartMenuCampTest[16];

extern const char dds3StartMenuMessageScriptTest[24];

extern const char dds3StartMenuEventViewer[16];

extern const char dds3StartMenuEventTest[16];

extern const char dds3StartMenuKernelTest[16];

extern const char dds3StartMenuModelCollisionEditor[24];

extern const char dds3StartMenuBattleFieldTest[24];

extern const char dds3StartMenuNewBattleTest[16];

extern const char dds3StartMenuGameStart[16];
/* End of declarations from src/smt3/kernel/dds3PadManager.c */

s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % 0x4650 == 0) {
        dds3PadVibrationSet(0, 1, 0xF);
        dds3PadVibrationSet(1, (u8)(dds3GetRandIntA(0, 150) + 100), 30);
    }
    return 0;
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadInitialize);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00103BE8);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", kwlnPadStepLargeMotorLevel);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104140);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_00104210);

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
INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1B0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1B1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1BF);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C2);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_0046B1C3);

