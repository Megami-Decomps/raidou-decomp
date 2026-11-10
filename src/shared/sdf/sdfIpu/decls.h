/* Declarations of smt3-decomp src/smt3/sdf/sdfIpu.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFIPU_H
#define SHARED_SDF_SDFIPU_H

#include "fpu.h"

#include "common.h"

#include "eeregs.h"

#include "libsdf.h"

#include "ps2asm.h"

enum {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

#include "sdf_movie_types.h"

void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext);

void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm, SdfStreamRead hdr, struct sdfMovieCb_t *leng);

void sdfIpuAttachTask(sdfIpuDecodeCb_t *cb);

s32 SleepThread(void);

extern s32 sdfCdTrayMode;

extern s32 sdfFileSemaId;

extern u8 sdfFileDefaultSrcType;

extern u8 sdfFileCmdDefaultDevice;

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern u32 p2sdPcmCallback;

extern u32 p2sdPcmState;

extern u32 p2sdPcmBusy;

extern sdfIpuDecodeCb_t *sdfIpuTaskLink;

typedef struct sdfHostPrefix_t {
    s8 b[7];
} sdfHostPrefix_t;

typedef u8 sdfDevOperationCode_t;

typedef s8 sdfDevStateCode_t;

/* Complete 0x40-byte request owner. sdfFileCmd_t describes only its opaque
 * 0x30-byte prefix for consumers outside the device queue implementation. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next;
    struct sdfFileCmdHandle_t *prev;
    struct sdfFileCmdHandle_t *deviceNext;
    struct sdfFileCmdHandle_t *devicePrev;
    char *filename;
    u8 device;
    sdfDevOperationCode_t cmd;
    sdfDevStateCode_t stat;
    u8 phase;
    s32 offset;
    s32 length;
    void *buffer;
    s32 cmdParm;
    s32 fd;
    s32 errorCode;
    s32 currentOffset;
    u8 remainder[4];
    void (*callback)(struct sdfFileCmdHandle_t *, s32, saddr_t, s32, s32);
    s32 userData;
} sdfFileCmdHandle_t;

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

typedef struct sdfClock_t {
    u8 pad00;
    u8 sec;
    u8 min;
    u8 hour;
    u8 day;
    u8 month;
    u16 year;
} sdfClock_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

/* One tag in the to-IPU chain. Written as two quadword halves rather than by
 * field, which is what the single 64-bit store in sdfIpuSetStreamChunk needs. */
struct sdfIpuDmaTag_t {
    u64 control;
    u64 reserved;
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

typedef struct sdfStreamFrameHeader_t {
    u8 reserved[8];
    u16 width;
    u16 height;
    u32 cycleLength;
} sdfStreamFrameHeader_t;

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern s32 DIntr();

#include "sdk_libkernl.h"

extern void *memcpy(void *, const void *, size_t);

extern s8 sdfPrintIopFlag;

extern char sdfPrintStrFmt[];

extern char sdfFileCdromPrefix[];

extern sdfHostPrefix_t sdfFileHostPrefix[];

extern u8 sdfFileHddPrefix[];

extern char sdfFileCdromName[];

extern void EIntr(void);

extern sdfFileCmdHandle_t *sdfFileCmdActiveHead;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *sdfFileCmdCacheHead;

extern sdfFileCmdHandle_t *sdfFileCmdCacheTail;

extern char sdfFileCmdCdromPrefix[];

extern char sdfFileCmdHostPrefix[];

extern char sdfFileCmdPfsPrefix[];

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern u8 sdfDev9Ready;

extern u8 sdfCdClock;

extern sdfClock_t sdfCdClockCache;

extern u32 sdfSndSendCmd_cmd_id;

extern void FlushCache(s32);

extern s32 p2sdSmgHandle;

extern char sdfDumpAddrFmt[];

extern char sdfDumpPadStr[];

extern char sdfDumpMidGapStr[];

extern char sdfDumpByteSepStr[];

extern char sdfDumpByteFmt[];

extern char sdfDumpLineBuf[];

extern char sdfDumpEndStr[];

extern char sdfVifMaskedName[];

extern char sdfVifXferOkName[];

extern char sdfDumpIndentStr[];

extern char sdfDumpWordFmt[];

extern char sdfGsMemTypeFmt[];

extern sdfIpuDecodeCb_t *sdfIpuQueueHead;

extern sdfIpuDecodeCb_t *sdfIpuQueueTail;

void *sdfAllocAuto(s32 size);

extern void * memset(void *, s32, size_t);

extern sdfGsMemSlice_t *sdfGsMemAllocRect(s32, s32, s32, s32, s32);

void sdfFreeAuto(void *data);

extern s32 sceIpuSync(s32, s32);

extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb;

extern void sdfImageSetChainReleaseMem(saddr_t);

extern void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle);

extern sdfIpuDecodeCb_t *sdfIpuPendingCb;

extern s32 sdfIpuDecode(sdfIpuDecodeCb_t *);

extern void sceIpuStopDMA(sdfIpuDmaRegs_t *);

extern void sceIpuRestartDMA(sdfIpuDmaRegs_t *);

extern s32 sdfIpuDecodeThId;

extern s32 iWakeupThread(s32 threadId);

extern s32 sdfIpuDoneFrame;

u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle);

extern s32 sdfFromIpuDmacHandlerHandle;

extern s32 sdfToIpuDmacHandlerHandle;

extern void sceIpuInit(void);

extern s32 DmacAddHandler(s32, s32, void *, s32, s32);

extern void EnableDmac(s32);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern u8 sdfIpuDecodeThStack[];

extern void sdfIpuRemoveHandler(void *);

extern s32 DisableDmac(s32);

extern s32 (*p2sdSmgBuf)(void);

void sdfIpuExecQueue(s32 critical);

void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *cb);

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *cb, s32 critical);

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *cb, s32 critical);

void sdfIpuAttachTask(sdfIpuDecodeCb_t *cb);

void sdfIpuDetachTask(sdfIpuDecodeCb_t *cb);

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *cb);

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *cb, u8 *data, s32 leng);

/* Build both frame buffers' GIF upload packets: a BITBLTBUF header followed
 * by one transfer per 16x16 tile. */
void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *cb);

/* Clear the decoder, choose 32-bit/16-bit/dithered output and copy playback options. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm);

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm, SdfStreamRead hdr, struct sdfMovieCb_t *leng);

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *format, SdfStreamRead readSource, struct sdfMovieCb_t *readContext);

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node);

void sdfIpuFree(sdfIpuDecodeCb_t *cb);

/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sdfIpuHandleDmaEnd(sdfIpuDecodeCb_t *feed);

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *cb);

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *cb);

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void);

extern u8 sdfIpuRgbModeBits[][2];

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 critical);

/* Sleep for IPU completion, advance cycle state and requeue non-drained nodes;
 * the DMA worker runs indefinitely and retains the native completion-byte gates. */
void sdfIpuDecodeThread(void);

s32 sdfIpuFromDmacHandler(void);

s32 sdfIpuToDmacHandler(void);

void *sdfIpuGetActivePacket(sdfIpuDecodeCb_t *cb);

void *sdfIpuGetActiveFrame(sdfIpuDecodeCb_t *cb);

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *cb);

extern void sdfImageSetChainTag(u32 aHead, void *aTail);

/* Hand the finished buffer's packet to the image set, building it on first use. */
s32 sdfIpuSetChainTag(sdfIpuDecodeCb_t *cb);

void sdfIpuRunDecode(s32 frames);

void sdfIpuStartDecodeTask(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext, s32 aResourceWord);

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source);

void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext, s32 aResourceWord);

void sdfIpuInit(void);

s32 sdfIpuClose(void);

#endif
