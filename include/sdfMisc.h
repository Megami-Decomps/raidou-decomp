#ifndef SDFMISC_H
#define SDFMISC_H

#include "common.h"
#include "libsdf.h"

void sdfItemArrayFree(sdfItemArray_t *aArray);
void sdfItemArrayGrow(sdfItemArray_t *aArray);
sdfItemArray_t *sdfItemArrayNew(s32 aInitCapa, s32 aElemSize, s32 aGrowStep);

#endif
