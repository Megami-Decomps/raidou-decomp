#ifndef EFFMATH_H
#define EFFMATH_H

#include "common.h"

struct effArrHdr_t;
struct effCubicBezierSlot_t;

/* The slot array is followed by its effArrHdr_t, which Create returns. */
struct effArrHdr_t *dds3Bezier4PCreate(s32 number);
struct effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *pEffBezier4P, s32 index);
s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
s32 effPCPBlurScreenOffset(f32 scale);
void effScreenOffset(void);
s32 nbCameraLerpVec(struct effCubicBezierSlot_t *slot, f32 *out);

#endif
