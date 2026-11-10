/* Declarations of smt3-decomp src/smt3/sdf/sdfTex.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFTEX_H
#define SHARED_SDF_SDFTEX_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

extern sdfTexHandle_t *sdfTexHandleLink;

extern sdfDelayedResrcCB_t sdfTexDelayedResrcCB;

extern void sdfDelayedResrcInitCB(sdfDelayedResrcCB_t *aCB, sdfDelayedResrcFreeFunc aFreeFunc);

extern void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem);

extern void *sdfChipAllocDirect(s32 aSize);

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void *sdfAllocAuto(s32 size);

extern void sdfFreeAuto(void *ptr);

extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

extern sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType);

extern void *memcpy(void *a, const void *b, size_t c);

extern sdfTexHandle_t *sdfTexSetEx(TMX_HEADER_t *aTex, s32 aMemMode);

extern void *sdfTexTransData(s32 aGsAddr, s32 aWidth, s32 aHeight, s32 aPixelFormat, void *aSrcData,
                             s32 aMemMode);

extern void sdfTexSetPacket(sdfTexPacket_t *aPacket, s32 aWidth, s32 aHeight, s32 aGsAddr,
                            s32 aPixelFormat, s32 aClutAddr, s32 aClutFormat, s32 aColorComp,
                            s32 aNumOfMipmap, s32 aMipmapParm, s32 aWrapMode, s32 aContext);

sdfGsMemSlice_t *sdfTexAllocClut(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT);

void sdfTexAllocClutBuff(sdfTexHandle_t *aHandle);

void sdfTexCopyClutData(sdfTexHandle_t *aHandle, void *aData);

void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle);

void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *aHandle);

void sdfTexRebuildPacket(sdfTexHandle_t *aHandle);

/* One image transfer request; dimensions and source rectangle are halfwords. */
typedef struct
{
    void *pixels;
    sdfMemHandle_t *allocation;
    u8 allocationMode;
    u8 format;
    s16 bufferWidth;
    u32 destination;
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} sdfImageUploadRequest_t;

extern s32 sdfImageDataSize(s32 format, s32 width, s32 height);

extern void sdfImageSetReq(sdfImageUploadRequest_t *parm);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle);

s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);

sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *aHandle);

s32 sdfTexGetPacketCtx2Length(sdfTexHandle_t *aHandle);

s32 sdfTexGetCLUTCount(sdfTexHandle_t *aHandle);

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *aHandle);

u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle);

s32 sdfTexGetPixelBitCount(s32 aFormat);

s32 sdfTexGetTexelBitCount(s32 aFormat);

u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);

u64 sdfTexGetTex1Reg(sdfTexHandle_t *aHandle);

u64 sdfTexGetClampReg(sdfTexHandle_t *aHandle);

void sdfTexSetFilter(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode);

void sdfTexSetFilterCtx2(sdfTexHandle_t *aHandle, s32 aMagMode, s32 aMinMode);

void sdfTexSetWrapMode(sdfTexHandle_t *aHandle, s32 aMode);

void *sdfTexTransData(s32 aGsAddr, s32 aWidth, s32 aHeight, s32 aPixelFormat, void *aSrcData, s32 aMemMode);

void *sdfTexTransCLUT(sdfTexHandle_t *aHandle, s32 aGsAddr, void *aDataPtr, s32 aMemMode);

void sdfTexClutToGs(sdfTexHandle_t *aHandle);

void sdfTexLink(sdfTexHandle_t *handle);

sdfTexHandle_t *sdfTexNew(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap,
                          sdfGsMemSlice_t *aPixelGsHandle, s32 aCLUTFormat, s32 aNumOfCLUT,
                          sdfGsMemSlice_t *aCLUTGsHandle);

sdfTexHandle_t *sdfTexDup(sdfTexHandle_t *aHandle);

sdfGsMemSlice_t *sdfTexAllocClut(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT);

s32 __attribute__((const)) sdfTexGetClutDataSize(s32 aPixelFormat, s32 aCLUTFormat, s32 aNumOfCLUT);

void sdfTexAllocClutBuff(sdfTexHandle_t *aHandle);

void sdfTexCopyClutData(sdfTexHandle_t *aHandle, void *aData);

sdfTexHandle_t *sdfTexAlloc(s32 aWidth, s32 aHeight, s32 aPixelFormat, s32 aNumOfMipmap,
                            s32 aCLUTFormat, s32 aNumOfCLUT);

void sdfTexFree(sdfTexHandle_t *aHandle);

void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

sdfTexPacket_t *sdfTexMakeGIFPacketSub(sdfTexHandle_t *aHandle, s32 aContext);

void sdfTexMakeGIFPacket(sdfTexHandle_t *aHandle);

void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *aHandle);

void sdfTexRebuildPacket(sdfTexHandle_t *aHandle);

sdfTexHandle_t *sdfTexSetEx(TMX_HEADER_t *aTex, s32 aMemMode);

sdfTexHandle_t *sdfTexSet(TMX_HEADER_t *aTex);

sdfTexHandle_t *sdfTexSetDirect(TMX_HEADER_t *aTex);

void sdfTexSetMonoClut(sdfTexHandle_t *aHandle);

void sdfTexSetClutColor(sdfTexHandle_t *texture, s32 tint, f32 amount);

sdfTexHandle_t *sdfTexSearch(u32 id);

void sdfTexInit(void);

#endif
