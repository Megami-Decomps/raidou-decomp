/* Declarations of smt3-decomp src/smt3/sdf/sdfDevCons.c, for its bodies in this directory. */
#ifndef SHARED_SDF_SDFDEVCONS_H
#define SHARED_SDF_SDFDEVCONS_H

#include "common.h"

#include "libdma.h"

#include "eeregs.h"

#include "sdfDevCons.h"

#include "libsdf.h"

#include "ps2asm.h"

typedef struct sdfDevConsCB_tag
{
    struct sdfDevConsCB_tag *prev;
    struct sdfDevConsCB_tag *next;
    u16 x;
    u16 y;
    s16 w;
    s16 h;
    s16 locX;
    s16 locY;
    u8 mode;
    u8 pad;
    u8 attr;
    u8 tabwidth;
    sdfMemHandle_t *bufferHandle;
    u8 *bufferPtr;
} sdfDevConsCB_tag;

typedef struct sdfDevConsDrawCB_t
{
    u8 *cur;
    u8 *start;
    u8 *tag;
    u32 *dmaCh;
    u8 *buf[2];
    s32 buffSize;
    u16 dblBuffId;
    u16 stepY;
} sdfDevConsDrawCB_t;

/* SMT3 DrawAll and the DDS1 twin reserve this same 0x1040-byte stack
 * allocation: only the central 0x1000 bytes are passed as DMA storage. */
typedef struct
{
    u8 opaquePrefix[0x30];
    u8 data[0x1000];
    u8 opaqueSuffix[0x10];
} sdfDevConsStackPackets_t;

/* The four print-control words are signed XYZ and packed RGBA when drawing. */
typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 attr;
} sdfDevPrintCB_t;

typedef struct
{
    u16 quadwordCount;
    u8 reserved02;
    u8 tagKind;
    u32 nextAddress;
    u32 vifNop;
    u32 vifDirect;
    u64 setupTag;
    u64 setupRegisters;
    u64 textureClamp;
    u64 textureClampRegister;
    u64 textureFilter;
    u64 textureFilterRegister;
    u64 textureState;
    u64 textureStateRegister;
    u64 glyphTag;
    u64 glyphRegisters;
} sdfDevConsPacketHeader_t;

typedef struct
{
    u64 color;
    u64 firstUv;
    u64 firstPosition;
    u64 secondUv;
    u64 secondPosition;
} sdfDevConsGlyph_t;

extern void sceDmaSendN(void *channel, void *address, s32 quadwords);

extern s32 sceGsSyncPath(s32 mode, s32 timeout);

extern void *sceDmaGetChan(s32 channel);

extern u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);

extern void *sdfGrOpenPacketBuff(void);

extern void sdfGrClosePacketBuff(saddr_t aBuffPtr);

void sdfDevConsDrawBG(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aCB);

extern void sdfDevConsDrawInit(sdfDevConsDrawCB_t *aCB, void *aPacketBuffer, s32 aBufferSize);

extern void sdfDevConsDraw(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aBuf);

extern void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *aCB);

extern void *memset(void *a, s32 b, size_t c);

extern void *memmove(void *a, const void *b, size_t c);

extern s32 vsprintf(char *buf, const char *fmt, void *args);

extern void *sdfChipAllocDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern sdfTexHandle_t *sdfDebugFontHandle;

extern s32 alreadyInitDevCons;

extern u8 sdfDebugFontTex[];

extern u32 sdfDevConsFontUvTable[];

extern sdfDevConsCB_tag *devConsTop;

extern u32 sdfDevConsColorTable[];

extern void *sdfDevVprintf(u32 *aCB, const char *aStr, void *LegacyParamArray);

void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr);

void sdfDevConsInit(void);

sdfTexHandle_t *sdfDevConsGetFontHandle(void);

s16 *sdfDevConsGetFontUvTable(void);

void sdfDevConsLinkToTop(sdfDevConsCB_tag *aHndl);

void sdfDevConsUnlink(sdfDevConsCB_tag *aHndl);

void sdfDevConsClose(sdfDevConsCB_tag *aHndl);

void sdfDevConsClearInside(sdfDevConsCB_tag *aHndl);

void sdfDevConsClear(sdfDevConsCB_tag *aHndl);

sdfDevConsCB_tag *sdfDevConsOpen(s32 aX, s32 aY, s32 aW, s32 aH);

void sdfDevConsNewLine(sdfDevConsCB_tag *aHndl);

void sdfDevConsPutInside(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr);

void sdfDevConsPut(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr);

s32 sdfDevConsPrintf(sdfDevConsCB_tag *aHndl, const char *aFmt, ...);

void sdfDevConsLocate(sdfDevConsCB_tag *aHndl, s32 aX, s32 aY);

void sdfDevConsAttribute(sdfDevConsCB_tag *aHndl, s32 aAttr);

s32 sdfDevConsGetMode(sdfDevConsCB_tag *aHndl);

void sdfDevConsSetMode(sdfDevConsCB_tag *aHndl, s32 aMode);

void sdfDevConsTagClose(sdfDevConsDrawCB_t *aCB);

void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *aCB);

void sdfDevConsDrawBG(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aCB);

void sdfDevConsDraw(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *buffers);

void sdfDevConsDrawInit(sdfDevConsDrawCB_t *aCB, void *aPacketBuffer, s32 aBufferSize);

void sdfDevConsDrawAll(void);

void *sdfDevVprintf(u32 *aCB, const char *aStr, void *LegacyParamArray);

void *sdfDevPrintfEx(u32 *aCB, const char *aStr, ...);

void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

void func_002BD300(u32 *aCB, s32 aMode);

void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr);

#endif
