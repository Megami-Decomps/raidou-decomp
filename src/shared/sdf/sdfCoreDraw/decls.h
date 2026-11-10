/* Declarations of smt3-decomp src/smt3/sdf/sdfCoreDraw.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFCOREDRAW_H
#define SHARED_SDF_SDFCOREDRAW_H

#include "common.h"

#include "libdma.h"

#include "eeregs.h"

#include "ps2asm.h"

#include "model.h"

#include "draw3d.h"

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_DMA_CHCR_TTE 0x40

extern addr_t corePktCurrLight;

extern f32 corePktZBias;

extern addr_t corePktCurrFog;

extern u128 corePktFogParm;

extern u8 vu0MicroPacket[];

extern u8 vu1MicroPacket[];

extern saddr_t corePktPointBuff;

extern sdfMemHandle_t *sdfCoreDrawBuffHandle;

extern void *sceDmaGetChan(s32);

extern void sceDmaSendN(void *, void *, s32);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern void sdfMemFreeHandleDirect(sdfMemHandle_t **handle);

extern void *sdfMemLock(sdfMemHandle_t *handle);

extern u8 *sdfModelAllocTmPacket(sdfModelNode_t *node);

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *root, sdfCorePacket_t *p);

void sdfCoreDrawSetLight(u8 *light);

void sdfCoreDrawSetFog(u8 *fog);

void sdfCoreDrawSetZBias(f32 z);

void sdfCoreDrawReset(void);

void sdfCoreDrawInit(s32 buffSize);

#endif
