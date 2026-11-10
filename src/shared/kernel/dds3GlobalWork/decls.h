/* Declarations of smt3-decomp src/smt3/kernel/dds3GlobalWork.c, for its bodies in this directory. */
#ifndef SHARED_KERNEL_DDS3GLOBALWORK_H
#define SHARED_KERNEL_DDS3GLOBALWORK_H

#include "common.h"

#include "basic.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "newdata.h"

#include "field.h"

#include "fpu.h"

#include "sdfFrame.h"

/* Declared here because no header carries these. */
extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *a, s32 b, size_t c);

extern s32 evtBitCheck(s32 no);

extern void dds3GlobalWorkUserInit2(s32 mode);

extern void dds3GlobalWorkClearFlags(void);

extern void dds3GlobalWorkUserInit(void);

extern void func_00116C30(void);

extern void dds3GlobalWorkDestroy(void);

extern s32 dds3GlobalWorkMainProc(dds3ProcessID_t *id);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern char dds3GlobalWorkProcName[];

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer, s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc, dds3ProcessFunc_t shutdownFunc,
                                         void *workData);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemUnlock(sdfMemHandle_t *aHandle);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void evtInitGlobalWork(void);

extern void datInitPlayer(void);

extern void datInitBossPress(void);

extern void dds3ClearStockTable(void);

extern void frInitNameData(void);

extern void fclEncycInitRecord(s32 mode);

extern void NewGame2Push(void);

extern void NewGame2Pop(void);

extern void *sdfMemGetPtr(sdfMemHandle_t *aHandle);

extern void fldGbInit_System(s32 mode);

extern void evtBitOn(s32 no);

typedef void (*sdfGrOtRegFunc_t)(sdfGrOt_t *ot, sdfDrawTag_t *tag);

/* The font node frFont hands back. It accumulates the drawn string's width at
 * +0xC, which the centring path in dds3FontReqAddString reads. */
typedef struct
{
    u8 unk00[0xC];
    s32 w_pxls;
} _FRQ;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

typedef struct
{
    dds3FontReq_t req;
    _FRQ *frq;
} dds3FontReqString_t;

typedef struct
{
    dds3FontReq_t req;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    u32 z;
    u32 color;
} dds3FontReqBox_t;

typedef struct
{
    dds3FontReq_t *first;
    dds3FontReq_t *last;
} dds3FontReqList_t;

extern dds3FontReqList_t dds3FontReqList;

extern dds3FontReqFunc_t dds3FontReqTable;

extern dds3FontReqFunc_t dds3FontReqBoxTable;

extern sdfGrOt_t sdfGrOtTable[];

extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;

extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;

extern s8 slNewGameBit2561[0x18];

extern u16 dds3NewGameSavedHalf0C;

extern s32 D_0054EA18;

extern sdfMemHandle_t *D_0054EA1C;

extern sdfMemHandle_t *dds3NewGameSavedBossPress;

extern void *sdfChipAllocZeroDirect(s32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern s32 sdfDrawTagInit(sdfDrawTag_t *a);

extern sdfDrawTag_t *sdfPrimAllocDrawTag(void *aAllocator);

extern void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, u32 aColor, u32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                               s32 aY1, u32 aZ, void *aAllocator);

extern s32 frDrawFont(_FRQ *pFRQ);

extern void frSetFontColorNo(_FRQ *pFRQ, u8 ColorNo);

extern _FRQ *frSetFontString(char *str, s32 style, s32 colorno, s32 tp, _FRQ *frq, s32 mode);

extern void frSetDrawPosition(_FRQ *pFRQ, s32 X, s32 Y);

extern _FRQ *frDestroyFontRequestAfter(_FRQ *pFRQ);

void dds3GlobalWorkInit(void);

void dds3GlobalWorkDestroy(void);

extern u32 dds3GetKernelCount(void);

s32 dds3GlobalWorkMainProc(dds3ProcessID_t *id);

/* Only the stock capacity word is described by this save-work view. */
typedef struct
{
    u8 unk_00000[0xA50];
    s32 maxstock;
} dds3GlobalStockView_t;

void func_00116C30(void);

void dds3GlobalWorkUserInit(void);

void NewGame2Push(void);

void NewGame2Pop(void);

void dds3GlobalWorkUserInit2(s32 mode);

void dds3FontReqListInsert(dds3FontReqList_t *list, void *node, s32 linkofs);

void dds3FontReqListRemove(dds3FontReqList_t *list, void *node, s32 linkofs);

void dds3FontReqListAdd(dds3FontReq_t *req, dds3FontReqFunc_t *func);

void dds3FontReqListRunOne(dds3FontReq_t *req);

void dds3FontReqListRunAll(void);

void func_00117830(dds3FontReq_t *req, s32 group);

void dds3FontReqListRunTo(s32 group);

void dds3FontReqListCount(void);

void dds3FontReqDestroy(dds3FontReqString_t *req);

s32 dds3FontReqDrawFont(dds3FontReqString_t *req);

dds3FontReq_t *dds3FontReqAddString(s32 x, s32 y, char *str);

void dds3FontReqSetColorNo(dds3FontReqString_t *req, u8 colorno);

void dds3FontReqFree(dds3FontReqBox_t *req);

void dds3FontReqDrawBoxLine(dds3FontReqBox_t *req);

dds3FontReq_t *dds3FontReqAddBox(s32 x, s32 y, u32 z, s32 w, s32 h, u32 color);

void func_00117AF8(dds3FontReqBox_t *req, s32 x, s32 y);

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode);

extern f32 D_00529630[4];

extern sdfMatrix4_t D_002FA240;

extern const f32 dds3PrimLineOutline[13][4];

extern const f32 dds3PrimLineScale[4];

extern const u8 dds3PrimLinePairs[20][2];

extern void MultMatrixRev(const void *aMatrix);

void *dds3PrimLine3D(s32 x, s32 y, s32 z, s32 highlighted);

#endif
