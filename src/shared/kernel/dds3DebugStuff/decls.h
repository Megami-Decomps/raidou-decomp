/* Declarations of smt3-decomp src/smt3/kernel/dds3DebugStuff.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3DEBUGSTUFF_H
#define SHARED_KERNEL_DDS3DEBUGSTUFF_H

#include "common.h"

#include "sdk_libgcc.h"

#include "basic.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "newdata.h"

#include "field.h"

extern u8 dds3PrimDebugTextBoxAttrs[];

extern char *dds3PrimDebugTextBoxLabels[];

extern char dds3DebugLabelB[], dds3DebugLabelG[], dds3DebugLabelR[], dds3DebugFmtInt3[], dds3DebugFmtFloat3[];

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void *sdfDevPrintf(s32 aX, s32 aY, s32 aZ, s32 aAttr, const char *aStr, ...);

extern void dds3MenuStepValue(char *text, f32 value);

extern u8 dds3PadButtonState[];

extern void dds3DebugFloatEditEx(f32 *value, u8 *pad, f32 step, f32 min, f32 max, f32 scale);

extern void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap);

extern s32 dds3DebugColorEditEx(u32 *aValue, s32 aColumn, s8 *aPadMap);

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void rsetOtOfs(s32 otofs);

extern void rsetAlpha(s32 mode);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void fldDrawPoly3(s32 c0, s32 c1, s32 c2, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2);

void dds3DebugFloatEditEx(f32 *controlledValue, u8 *padState, f32 minimum, f32 maximum, f32 step, f32 bigStep);

void dds3DebugFloatEdit(f32 *value, f32 step, f32 min, f32 max, f32 scale);

void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap);

void dds3DebugIntEdit(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge);

s32 dds3DebugColorEditEx(u32 *aValue, s32 aColumn, s8 *aPadMap);

void dds3DebugColorEdit(u32 *aValue, s32 aColumn);

void dds3MenuStepValue(char *out, f32 value);

void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values);

void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y, s32 selected,
                   u32 color, s32 showNormalized);

u32 dds3NullDrawProc(void);

void dds3DrawUnitObjectShadow(s32 fade, f32 x, f32 y, f32 z, f32 radius);

#endif
