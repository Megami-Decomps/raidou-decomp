#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdf_PadManager.h"
#include "dds3KernelCore.h"
#include "dds3PadManager.h"

extern u8 dds3PadVibrationState[2];
extern u8 dds3PadVibrationMode[2];
extern s32 dds3PadVibrationTable[2];
extern s32 dds3GetRandIntA(s32 aLimit, s32 name);
extern void dds3GetVibrationLevel(void);
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
extern void dds3UpdateVibrationSlots(void);

#include "sdk_libc.h"

void dds3GetVibrationLevel(void);
void dds3UpdateVibrationSlots(void);

#include "sdfDevCons.h"
#include "include_asm.h"
#include "newdata.h"
#include "draw3d.h"

extern u8 Act2SubWork2;
extern fclNamedSlot_t D_003C1C90[];

/* One shared initializer for the three configure modes and unregister. */
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_003C1C90[index].kind = kind;
    D_003C1C90[index].task = NULL;
    D_003C1C90[index].update = update;
    D_003C1C90[index].destroy = destroy;
    D_003C1C90[index].setState = setState;
    D_003C1C90[index].enabled = 0;
    D_003C1C90[index].userValue = userValue;
}

#define DDS3_PAD_INPUTS_PER_SET 16

#define DDS3_PAD_REPEAT_BIT 2

#define DDS3_ALT_CONFIRM_INPUT 0x11

#define DDS3_ALT_CANCEL_INPUT 0x13

#define DDS3_PAD_SMALL_MOTOR 0

#define DDS3_PAD_LARGE_MOTOR 1

#define DDS3_PAD_MOTOR_COUNT 2

#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650

#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF

#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30

#define DDS3_DEBUG_RUMBLE_VARIATION 150

#define DDS3_DEBUG_RUMBLE_BASE 100

#define DDS3_SHARED_RANDOM_SEED 0x12345678

#define DDS3_CELL_COLUMN_SPAN 0xC0

#define DDS3_CELL_ROW_SPAN 0x60

#define DDS3_PREVIEW_MAX_TEXELS 0x100

#define DDS3_PREVIEW_WRAP_TEXELS 0x400

#define DDS3_DIAG_DEPTH 0x0FFFFF80

void Act2Sub(void);

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20

#define DDS3_VIEWER_FAST_PAGE_STEP 10

#define DDS3_MAP_ROW_UNITS 0x1000

#define DDS3_MAP_ROW_SHIFT 12

#define DDS3_MAP_ROW_MASK 0xFFF

#define DDS3_MAP_COLUMN_SHIFT 5

#define DDS3_MAP_COLUMN_UNITS 32

#define DDS3_MAP_COLUMN_COUNT 128

#define DDS3_MAP_ROW_SPAN 0x800

#define DDS3_MAP_BORDER_WIDTH 0x840

#define DDS3_MAP_CELL_X_STEP 16

#define DDS3_MAP_ROW_Y_STEP 8

#define DDS3_MAP_CELL_X_SHIFT 4

#define DDS3_MAP_GREEN 0x80008000

#define DDS3_MAP_YELLOW 0x80008080

#define DDS3_MAP_RED 0x80000080

#define DDS3_MAP_BLUE 0x80800000

#define DDS3_MAP_CYAN 0x80808000

#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

#define DDS3_FADE_TICKS_PER_FRAME 10

#define DDS3_FADE_MARK_NUMERATOR 4

#define DDS3_FADE_COUNT_DOWN_BIT 1

#define DDS3_FADE_COUNT_UP_BIT 2

#define DDS3_FADE_DIRECTION_BITS 3

#define DDS3_FADE_MAX_ALPHA 0x80

#define DDS3_FADE_ALPHA_SHIFT 7

#define DDS3_COLOR_ALPHA_CHANNEL 3

#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000

#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000

#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000

#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF

#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF

#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF

#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31

#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F

#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f

#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

#ifdef VERSION_RAIDOU1
s32 dds3PadVibrationTest(dds3ProcessID_t *task) {
    if (dds3GetProcessRunTime(task) % 0x4650 == 0) {
        dds3PadVibrationSet(0, 1, 0xF);
        dds3PadVibrationSet(1, (u8)(dds3GetRandIntA(0, 150) + 100), 30);
    }
    return 0;
}
#else
INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadVibrationTest);
#endif

void dds3PadInitialize(void) {
    s32 pad;
    s32 half;
    s32 key;

    for (pad = 0; pad < 2; pad++) {
        for (half = 0; half < 2; half++) {
            for (key = 0; key < 16; key++) {
                dds3PadButtonHoldCounters[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonPrev[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonState[pad * 16 + half * 32 + key] = 0;
            }
        }
        for (key = 0; key < 2; key++) {
            dds3PadStickHoldCounters[pad * 2 + key] = 0;
            dds3PadStickPrev[pad * 4 + key * 2] = 0;
            dds3PadStickPrev[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusPrev[pad * 2 + key] = 0;
            dds3PadStickState[pad * 4 + key * 2] = 0;
            dds3PadStickState[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusState[pad * 2 + key] = 0;
        }
    }
    dds3PadVibrationState[0] = 0;
    dds3PadVibrationMode[0] = 0;
    dds3PadVibrationTable[0] = 0;
    dds3PadVibrationState[1] = 0;
    dds3PadVibrationMode[1] = 0;
    dds3PadVibrationTable[1] = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3RunProcessList);

void Act2Sub(void) {
    u8 target = Act2SubWork2;

    if (dds3PadVibrationState[1] < target) {
        if (target - dds3PadVibrationState[1] > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] + 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else if (target < dds3PadVibrationState[1]) {
        if (dds3PadVibrationState[1] - target > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] - 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else {
        dds3PadVibrationState[1] = target;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadAct);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadIsTypeCD);

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
INCLUDE_SDATA(const s32, "kernel/dds3PadManager", dds3PadStickPrev);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", dds3RunProcessListData);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", dds3Data26);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", dds3PadStickState);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", RunProcessData);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", GetAnlogKeyData);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", sbtlCallBtlSetDevilIDEnemyRotData);

