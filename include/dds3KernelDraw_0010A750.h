#ifndef DDS3KERNELDRAW_0010A750_H
#define DDS3KERNELDRAW_0010A750_H

#include "common.h"
#include "libsdf.h"

void eglLinkBox(s32 x, s32 y, s32 w, s32, const sdfColor32_t *rgba0, const sdfColor32_t *rgba1, const sdfColor32_t *rgba2, const sdfColor32_t *rgba3);
void eglLinkMaru2D(s32 cx, s32 cy, s32 fr, const sdfColor32_t *in_rgba, const sdfColor32_t *out_rgba);

#endif
