/* Declarations of smt3-decomp src/smt3/sdf/sdfImageSet.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFIMAGESET_H
#define SHARED_SDF_SDFIMAGESET_H

#include "common.h"

#include "eeregs.h"

#include "libsdf.h"

#include "ps2asm.h"

/* GS pixel storage modes, the values of sdfGsMemSlice_t.pixelFormat. */
enum sdf_psm
{
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
    SDF_PSMT4HL = 0x24,
    SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30,
    SDF_PSMZ24 = 0x31,
    SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern s32 iWakeupThread(s32 threadId);

extern void (*sdfGsErrorHandler)(s32 size, s32 type);

/* sdfImageSetPacket_tag.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

typedef struct sdfImageSetRelease
{
    struct sdfImageSetRelease *next;
    void *chip;
    sdfMemHandle_t *memHandle;
    u8 freeMode;
} sdfImageSetPacket_tag;

/* Terminal DMA tag of a queued upload packet. Linking the next packet writes
 * its address into tag and clears the two VIF codes behind it. */
typedef struct sdfImageSetTag
{
    u64 tag;
    u64 vifCodes;
} sdfImageSetTag_t;

typedef struct sdfImageSetCB
{
    s32 semaId;
    sdfImageSetPacket_tag *packetHead;
    sdfImageSetPacket_tag *packetTail;
    addr_t tagHead;
    sdfImageSetTag_t *tagTail;
} sdfImageSetCb_t;

typedef struct sdfImageSetReqParam
{
    void *data;
    sdfMemHandle_t *memHandle;
    u8 freeMode;
    u8 pixelFormat;
    u16 bufferWidth;
    u32 addr;
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} sdfImageSetReqParam_t;

extern s32 WaitSema(s32 semaId);

extern s32 SignalSema(s32 semaId);

extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

extern void *sdfChipAllocZeroDirect(u32 aSize);

extern s32 sdfChipCheck(void *aPtr);

extern sdfMemHandle_t *sdfMemGetHandleByPtr(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern s32 GetThreadId(void);

#include "sdk_libkernl.h"

extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);

extern void *sceDmaGetChan(s32 channel);

extern s32 sceDmaSend(void *chan, void *packet);

extern void FlushCache(s32 mode);

extern void SleepThread(void);

extern sdfImageSetCb_t sdfImageSetCb;

extern u64 D_005293A0[];

s32 sdfImageDataSize(u32 format, s32 width, s32 height);

s32 sdfImageSetDmacHandler(s32 channel, s32 threadId);

void sdfImageSetClearCb(sdfImageSetCb_t *cb);

s32 sdfImageDataSize(u32 format, s32 width, s32 height);

void sdfImageSetReq(sdfImageSetReqParam_t *parm);

void sdfImageSetChainTag(u32 head, sdfImageSetTag_t *tail);

void sdfImageSetChainReleaseMem(saddr_t p);

void sdfImageSetClearCb(sdfImageSetCb_t *cb);

s32 sdfImageSetDmacHandler(s32 channel, s32 threadId);

void sdfImageSetTrans(void);

void sdfImageSetInit(void);

#endif
