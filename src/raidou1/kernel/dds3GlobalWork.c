#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3GlobalWork.c */
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

extern f32 sdf3DUpVector[4];

extern sdfMatrix4_t sdfViewMatrix;

extern const f32 dds3PrimLineOutline[13][4];

extern const f32 dds3PrimLineScale[4];

extern const u8 dds3PrimLinePairs[20][2];

extern void MultMatrixRev(const void *aMatrix);

void *dds3PrimLine3D(s32 x, s32 y, s32 z, s32 highlighted);

extern char dds3GlobalWorkProcName[5];

extern s8 slNewGameBit2208;

extern s8 slNewGameBit2209;

extern s8 slNewGameBit2210;

extern s8 slNewGameBit2211;

extern s8 slNewGameBit2212;

extern s8 slNewGameBit2213;

extern s8 slNewGameBit2323;

extern s8 slNewGameBit162;

extern u16 dds3NewGameSavedHalf0C;

extern s32 D_0054EA18;

extern sdfMemHandle_t *D_0054EA1C;

extern dds3GlobalWork_t *DDS3_GBWK;

extern dds3FontReqFunc_t dds3FontReqTable;

extern dds3FontReqFunc_t dds3FontReqBoxTable;
/* End of declarations from src/smt3/kernel/dds3GlobalWork.c */

INCLUDE_ASM(const s32, "kernel/dds3GlobalWork", func_r1_00122C50);

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol) {
    u64 *packet;
    s32 right;
    s32 bottom;
    u64 depthHigh;

    packet = (u64 *)sdfGrAllocPacketBuff(0x80);
    packet[0] = 7;
    right = aX + aW;
    bottom = aY + aH;
    depthHigh = (u64)(u32)aZ << 32;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0x55555105510ULL;
    packet[4] = 0x146;
    packet[5] = aBgCol | 0x3F80000000000000ULL;
    packet[6] = (u32)(((u32)aX & 0xFFFF) | ((u32)aY << 16)) | depthHigh;
    packet[7] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[8] = 0x142;
    packet[9] = aFrameCol | 0x3F80000000000000ULL;
    packet[14] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[10] = packet[14];
    packet[11] = (u32)(((u32)right & 0xFFFF) | ((u32)(aY - 8) << 16)) | depthHigh;
    packet[12] = (u32)(((u32)right & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[13] = (u32)(((u32)(aX - 0x10) & 0xFFFF) | ((u32)bottom << 16)) | depthHigh;
    packet[15] = 0;
    return packet;
}

u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode) {
    u64 *packet = (u64 *)sdfGrAllocPacketBuff(0x50);

    packet[0] = 4;
    packet[1] = 0x5000000400000000ULL;
    packet[2] = 0x5400000000008001ULL;
    packet[3] = 0x51510;
    packet[4] = aMode | 0x109;
    packet[5] = (u32)aColor0 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | ((u64)((u32)aZ0 >> 4) << 32);
    packet[7] = (u32)aColor1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | ((u64)((u32)aZ1 >> 4) << 32);
    packet[9] = 0;
    return packet;
}

void *dds3PrimLine3D(s32 x, s32 y, s32 z, s32 highlighted)
{
    u64 xyz[13];
    u64 color[13];
    volatile f32 axis[4] __attribute__((aligned(16)));
    volatile s32 point[4] __attribute__((aligned(16)));
    u64 *packet;
    u64 *endColor;
    u64 *endXyz;
    const f32 (*outline)[4];
    const u8 (*pair)[2];
    s32 i;
    s32 shade;

    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW2();
    SDF_VU0_STOREVECTOR(axis);
    if (ffabsf(axis[0]) < 0.000001f && ffabsf(axis[2]) < 0.000001f) {
        axis[2] = -axis[1];
        axis[0] = 0.0f;
        axis[1] = 0.0f;
        SDF_VU0_LOADVECTOR_B(axis);
    } else {
        SDF_VU0_LOADVECTOR_B(sdf3DUpVector);
    }
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW0();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_GET_ROW2();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_VECTOR_W_SET_ZERO();
    SDF_VU0_SET_ROW1();
    SDF_VU0_RESET_ROW3();
    MultMatrixRev(&sdfViewMatrix);
    SDF_VU0_LOADVECTOR(dds3PrimLineScale);
    SDF_VU0_SCALE_XYZ_REV();
    axis[0] = (f32)x;
    axis[1] = (f32)y;
    axis[2] = 160.0f;
    SDF_VU0_LOAD_ROW3(axis);

    shade = highlighted ? 0x101 : 0x10101;
    outline = dds3PrimLineOutline;
    for (i = 0; i != 13; i++) {
        SDF_VU0_LOADVECTOR(outline[i]);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_FTOI0();
        SDF_VU0_STOREVECTOR(point);
        xyz[i] = (((u32)point[0] & 0xFFFF) | ((u32)point[1] << 16)) | ((u64)z << 32);
        color[i] = (u32)(shade * point[2] | 0x80000000) | ((u64)0x3F800000 << 32);
    }

    packet = (u64 *)sdfGrAllocPacketBuff(0x2C0);
    packet[0] = 0x2B;
    packet[1] = 0x5000002B00000000ULL;
    packet[2] = 0x1400000000000001ULL;
    packet[3] = 0;
    packet[4] = 0x189;
    packet[5] = 0;
    packet[6] = 0x4400000000008014ULL;
    packet[7] = 0x5151;
    endColor = packet + 10;
    endXyz = packet + 11;
    pair = dds3PrimLinePairs;
    for (i = 0; i != 20; i++) {
        s32 index = pair[i][0];

        endColor[i * 4 - 2] = color[index];
        endXyz[i * 4 - 2] = xyz[index];
        index = pair[i][1];
        endColor[i * 4] = color[index];
        endXyz[i * 4] = xyz[index];
    }
    return packet;
}
