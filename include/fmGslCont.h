#ifndef FMGSLCONT_H
#define FMGSLCONT_H

#include "common.h"
#include "font.h"

s32 fmDestroyFontMemControl(void);
FontMem_t *fmRentFontMem(void);
void fmRetFontMem(FontMem_t *pFontArea);

#endif
