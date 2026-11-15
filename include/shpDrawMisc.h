#ifndef SHPDRAWMISC_H
#define SHPDRAWMISC_H

#include "common.h"
#include "libsdf.h"

s32 FloatToSprX(f32 a_x);
s32 FloatToSprY(f32 a_y);
void LineWide(f32 a_x0, f32 a_y0, f32 a_x1, f32 a_y1, u32 a_z, u32 *a_col, f32 a_w);
void shpDrawStraightLineWide(sdfIVector2_t *a_pPos, u32 a_z, s32 a_div, s32 a_top, s32 a_num, u32 a_col, s32 a_alphaType, f32 a_w);

#endif
