/* Declarations of smt3-decomp src/smt3/kernel/dds3PadManager.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3PADMANAGER_H
#define SHARED_KERNEL_DDS3PADMANAGER_H

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

#endif
