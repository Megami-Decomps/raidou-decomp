#ifndef DDS3KERNELDRAW_0010A750_H
#define DDS3KERNELDRAW_0010A750_H

#include "common.h"
#include "libsdf.h"

void eglLinkBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, u32 unk8, u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3);
void eglLinkBoxZ(s32 x, s32 y, s32 w, s32 h, s32 z, u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3);
void eglLinkMaru2D(s32 cx, s32 cy, s32 fr, const sdfColor32_t *in_rgba, const sdfColor32_t *out_rgba);

void dds3FileLoadSub(struct sdfMat_t *file);

#endif
