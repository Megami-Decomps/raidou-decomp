#ifndef SDFGSMEMORY_H
#define SDFGSMEMORY_H

#include "common.h"
#include "sdf_resource.h"

s32 sdfGetTexelBitCount(u32 aFormat);
sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 aW, s32 aH, s32 aPixelFormat);
sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType);
sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);
sdfGsMemSlice_t *sdfGsMemAllocRect(s32 aWidth, s32 aHeight, s32 aFormat, s32 aType, s32 aFromHead);
void sdfGsMemFree(sdfGsMemSlice_t *aHandle);
void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle);
void sdfGsMemInit(void);
extern sdfGsMemSlice_t *gsMemHandleLinkHead;
extern sdfGsMemSlice_t *gsMemHandleLinkTail;

#endif
