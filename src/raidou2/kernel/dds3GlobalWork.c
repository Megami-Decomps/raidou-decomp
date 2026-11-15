#include "common.h"
#include "basic.h"
#include "kernel.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "newdata.h"
#include "field.h"
#include "fpu.h"
#include "sdfFrame.h"
#include "sdk_libc.h"
#include "sdfVu0.h"
#include "sdfPacketBuff.h"
#include "dds3GlobalWork.h"

/* Declared here because no header carries these. */
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

extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;
extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;

/* Only the stock capacity word is described by this save-work view. */
typedef struct
{
    u8 unk_00000[0xA50];
    s32 maxstock;
} dds3GlobalStockView_t;

u64 *dds3PrimFrameRect(s32 aX, s32 aY, s32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);
u64 *dds3PrimLine(s32 aX0, s32 aY0, s32 aZ0, s32 aColor0, s32 aX1, s32 aY1, s32 aZ1, s32 aColor1, s32 aMode);
extern f32 sdf3DUpVector[4];
extern sdfMatrix4_t sdfViewMatrix;
extern const f32 dds3PrimLineOutline[13][4];
extern const f32 dds3PrimLineScale[4];
extern const u8 dds3PrimLinePairs[20][2];
void *dds3PrimLine3D(s32 x, s32 y, s32 z, s32 highlighted);
extern s8 slNewGameBit2208;
extern s8 slNewGameBit2209;
extern s8 slNewGameBit2210;
extern s8 slNewGameBit2212;
extern s8 slNewGameBit2213;
extern s8 slNewGameBit2323;

INCLUDE_ASM(const s32, "kernel/dds3GlobalWork", dds3CallGrAllocPacketBuff9);

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
