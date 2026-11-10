/* Declarations of smt3-decomp src/smt3/kernel/dds3KernelMisc.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3KERNELMISC_H
#define SHARED_KERNEL_DDS3KERNELMISC_H

#include "common.h"

#include "kernel.h"

#include "libsdf.h"

extern s8 dds3PadButtonState[64];

extern u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

void SelectSub(s32 *pbase, s32 *pm, s32 list, s32 win, s32 pad, s32 pluskey, s32 minuskey);

s32 dds3SelectMenu(s32 pad, s32 listsx, s32 listsy, s32 winsx, s32 winsy, s32 *basex, s32 *basey, s32 *x, s32 *y);

void dds3ConsPanelTagLink(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy);

void dds3ConsPanelTagLinkZ(sdfDrawTag_t *tag, s32 x, s32 y, s32 sx, s32 sy, s32 z);

#endif
