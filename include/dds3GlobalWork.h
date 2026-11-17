#ifndef DDS3GLOBALWORK_H
#define DDS3GLOBALWORK_H

#include "common.h"

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);
u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode);

#endif
