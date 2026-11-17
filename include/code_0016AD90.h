#ifndef CODE_0016AD90_H
#define CODE_0016AD90_H

#include "common.h"
#include "sdf_resource.h"
#include "dds3KernelDraw.h"
extern f32 mapCallSin11Data;
/* Pulse angle of the cut-camera fan, wraps past 6.28. */
extern f32 mapCallCutGetCamData;
extern struct sdfTexHandle_t *effQueuedResourceNameRecord4;
/* Pad button bytes, negative while held; mapFunc reads 0x24..0x2A. */
extern s8 dds3PadButtonState[];

s32 fldCallBitCheck(u32 n);
void fldLeaf25(void);
void mapCallCutGetCam(s32 x0, s32 y0, s32 x1, s32 y1);

#endif
