/* Declarations of smt3-decomp src/smt3/sdf/sdfMovie.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFMOVIE_H
#define SHARED_SDF_SDFMOVIE_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "sdf_movie_types.h"

void puts(const char *aStr);

s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);

s32 sdfFileCmdClose(void *aHandle);

void sdfDummyFunc(void);

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

void sdfChipFreeDirect(void *aPtr);

void sdfIpuFree(sdfIpuDecodeCb_t *cb);

s32 p2sdSyncMovie(void);

void *memcpy(void *a, const void *b, size_t c);

s32 sdfFileCmdFileSize(void *aHandle);

s32 sdfFileCmdFree(void *aHandle);

s32 DIntr(void);

s32 EIntr(void);

void *memset(void *a, s32 b, size_t c);

char *strrchr(const char *aStr, s32 aChar);

s32 strcasecmp(const char *aLeft, const char *aRight);

void *sdfChipAllocZeroDirect(s32 aSize);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

void *sdfFileCmdReadOpen(const char *aFileName, SdfStreamRead aCallback, sdfMovieCb_t *aUserData);

void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, sdfMovieCb_t *aContext, s32 aResourceWord);

s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

s32 WaitSema(s32 sema);

s32 SignalSema(s32 sema);

s32 movieReadChunk(void *aHandle, s32 aOp, void *aBuf, s32 aSize, sdfMovieCb_t *stream);

s32 movieCopyChunk(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *stream, s32 aOp, void *aBuf, s32 aSize);

s32 movieParseHeader(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *stream, s32 aOp, void *aBuf, s32 aSize);

extern s32 movieStreamSema;

extern u32 movieSemaCount;

extern sdfMovieCb_t *movieSemaId;

extern const char D_00551B50[];

void movie_2C5080(const char *aStr);

u8 *movieWaitSema(s32 aSize);

/* DMA/VIF/GIF header followed by nine real A+D writes for one movie sprite. */
typedef struct sdfMovieDrawPacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfGifTag_t gifTag;
    sdfGrTagStdReg_t sampling;
    sdfGrTagStdReg_t texture;
    sdfGrTagStdReg_t clamp;
    sdfGrTagStdReg_t textureAlpha;
    sdfGrTagStdReg_t color;
    sdfGrTagStdReg_t sourceOrigin;
    sdfGrTagStdReg_t vertexOrigin;
    sdfGrTagStdReg_t sourceExtent;
    sdfGrTagStdReg_t vertexExtent;
} sdfMovieDrawPacket_t;

extern u8 *movieWaitSema(s32 size);

extern s32 sdfGrAllocPacketBuff(s32 size);

extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocator)(s32));

extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);

extern u32 p2sdSetPcmCallback(addr_t callback);

extern s32 p2sdPlayPcm(u8 channel);

void release_movieobj2(sdfMovieCb_t *obj, sdfGrOt_t *ot);

void movieReadNextChunk(sdfMovieCb_t *aMov);

s32 movieGetStreamSize(void *aHandle, s32 aOp, void *aBuf, s32 aSize, sdfMovieCb_t *aMov);

void movieReadHeaderChunk(sdfMovieCb_t *aMov);

s32 movieReadChunk(void *file, s32 operation, void *readBuffer, s32 bytesRead, sdfMovieCb_t *movie);

s32 movieCopyChunk(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *aMov, s32 aOp, void *aBuf, s32 aSize);

s32 movieParseHeader(sdfIpuDecodeCb_t *aNode, sdfMovieCb_t *aMov, s32 aOp, void *aBuf, s32 aSize);

void movieOpenStream(sdfMovieCb_t *stream, sdfMovieInitParm_t *desc, const char *path);

void movieCloseStream(sdfMovieCb_t *aMov);

s32 sdfMovieIsFinished(sdfMovieCb_t *aMov);

#endif
