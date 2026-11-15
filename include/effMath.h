#ifndef EFFMATH_H
#define EFFMATH_H

#include "common.h"

u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);
s32 effPCPBlurScreenOffset(f32 scale);
void effScreenOffset(void);

#endif
