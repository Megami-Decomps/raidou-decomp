#ifndef SDFIMAGESET_H
#define SDFIMAGESET_H

#include "common.h"

/* Terminal DMA tag of a queued upload packet. Linking the next packet writes
 * its address into tag and clears the two VIF codes behind it. */
typedef struct sdfImageSetTag
{
    u64 tag;
    u64 vifCodes;
} sdfImageSetTag_t;

/* One image upload: source pixels, VRAM destination and rectangle. */
typedef struct sdfImageSetReqParam
{
    void *data;
    struct sdfMemHandle_t *memHandle;
    u8 freeMode;
    u8 pixelFormat;
    u16 bufferWidth;
    u32 addr;
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} sdfImageSetReqParam_t;

void sdfImageSetReq(sdfImageSetReqParam_t *parm);
s32 sdfImageDataSize(u32 format, s32 width, s32 height);
void sdfImageSetChainTag(u32 head, sdfImageSetTag_t *tail);
void sdfImageSetChainReleaseMem(saddr_t p);
void sdfImageSetInit(void);
void sdfImageSetTrans(void);

#endif
