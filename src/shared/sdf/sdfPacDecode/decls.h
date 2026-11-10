/* Declarations of smt3-decomp src/smt3/sdf/sdfPacDecode.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFPACDECODE_H
#define SHARED_SDF_SDFPACDECODE_H

#include "common.h"

#include "libsdf.h"

/* Declared here because no header carries these. */
extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(s32 aSize);

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeHandleDirect(sdfMemHandle_t **aHandlePtr);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern sdfTexHandle_t *sdfTexSetDirect(void *aTex);

extern sdfItemArray_t *sdfTexListNew(s32 aInitCapa);

extern void sdfDecompInit(struct sdfDecomp_t *cb, u8 *dst);

extern s32 sdfDecomp(struct sdfDecomp_t *cb, const u8 *src, s32 length);

extern void sdfAddressResolver(s32 *aDataBody, saddr_t aOffset, u8 *aAddrInfoPtr, s32 aAddrInfoLeng);

s32 sdfPacStdCallback(sdfPacDecodeCb_t *cb, s32 cause, sdfPacStdBlockHeader_t *hdr);

void sdfPacNewBlock(sdfPacDecodeCb_t *cb);

void sdfPacEndBlock(sdfPacDecodeCb_t *cb);

sdfPacDecodeResrc_t *sdfPacNewResrc(sdfPacDecodeCb_t *cb, const sdfPacStdBlockHeader_t *hdr);

void sdfPacMemDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemDecompDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemStayDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemDecoderFuncInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacMemDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacModelDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacModelDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacMotionDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacMotionDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacTmxDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecompDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxStayDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecoderInitFunc(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr,
                              sdfPacTmxDecodeCb_t *tmxCb);

void sdfPacTmxDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacTexListDecoderTmxBodyEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoder1(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoder0(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void func_002C60E0(sdfPacDecodeCb_t *aState);

void func_002C6D58(sdfPacDecodeCb_t *aState);

void sdfPacTexListDecoderTmxHeaderEndFunc(sdfPacDecodeCb_t *cb);

extern void sdfTexListAdd(sdfItemArray_t *aList, sdfTexHandle_t *aTexHandle);

void sdfPacInit(sdfPacDecodeCb_t *cb, sdfPacCallbackFunc callback);

void sdfPacSetStay(sdfPacDecodeCb_t *cb);

void sdfPacSetFromTail(sdfPacDecodeCb_t *cb);

void sdfPacFree(sdfPacDecodeCb_t *cb);

void func_002C60E0(sdfPacDecodeCb_t *aState);

void sdfPacUseSrc(sdfPacDecodeCb_t *cb, s32 size);

void sdfPacNewBlock(sdfPacDecodeCb_t *cb);

void sdfPacEndBlock(sdfPacDecodeCb_t *cb);

s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length);

sdfPacDecodeResrc_t *sdfPacNewResrc(sdfPacDecodeCb_t *cb, const sdfPacStdBlockHeader_t *hdr);

sdfPacDecodeResrc_t *sdfPacFreeResrc(sdfPacDecodeResrc_t *resrc);

s32 sdfPacStdCallback(sdfPacDecodeCb_t *cb, s32 cause, sdfPacStdBlockHeader_t *hdr);

char *sdfPacGetResrcFileName(sdfPacDecodeResrc_t *resrc);

void sdfPacMemDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemDecompDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemStayDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacMemDecoderFuncInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacMemDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacModelDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacModelDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacMotionDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacMotionDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacTmxDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecompDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxStayDecoderFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecoderInitFunc(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr,
                             sdfPacTmxDecodeCb_t *tmxCb);

void sdfPacTmxDecoderEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacTmxDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

void sdfPacTexListDecoderTmxHeaderEndFunc(sdfPacDecodeCb_t *cb);

void func_002C6D58(sdfPacDecodeCb_t *aState);

void sdfPacTexListDecoderTmxBodyEndFunc(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoder1(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoder0(sdfPacDecodeCb_t *cb);

void sdfPacTexListDecoderInit(sdfPacDecodeCb_t *cb, sdfPacStdBlockHeader_t *hdr);

#endif
