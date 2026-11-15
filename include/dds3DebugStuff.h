#ifndef DDS3DEBUGSTUFF_H
#define DDS3DEBUGSTUFF_H

#include "common.h"

void dds3DebugIntEditEx(void *aValue, s32 aValueSize, s64 aMin, s64 aMax, s64 aSmall, s64 aLarge, s8 *aPadMap);
void dds3PrimDebugTextBox(void *packetList, s32 x, s32 y, s32 selected, f32 *values);
void dds3PrimDebugTextBoxRGB(void *packetList, s32 x, s32 y, s32 selected, u32 color, s32 showNormalized);

#endif
