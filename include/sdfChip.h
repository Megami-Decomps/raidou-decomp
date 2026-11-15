#ifndef SDFCHIP_H
#define SDFCHIP_H

#include "common.h"
#include "libsdf.h"

void *sdfChipAllocDirect(s32 aSize);
void *sdfChipAllocZeroDirect(s32 size);
s32 sdfChipCheck(void *aPtr);
void sdfChipFreeDelayedDirect(void *aPtr);
void sdfChipFreeDirect(void *aPtr);
void sdfChipGetInfo(sdfChipInfo_t *aInfo);
void sdfChipInit(u32 aSize);

#endif
