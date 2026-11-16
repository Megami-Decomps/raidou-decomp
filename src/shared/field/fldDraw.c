#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "kernel.h"
#include "ps2asm.h"
#include "basic.h"
#include "billboard.h"
#include "draw.h"
#include "scr.h"
#include "draw3d.h"
#include "field.h"
#include "model.h"
#include "motion.h"
#include "newdata.h"
#include "event.h"
#include "effect.h"
#include "font.h"
#include "sdk_libc.h"
#include "sdfVu0.h"
#include "sdfDraw.h"
#include "sdfTex.h"
#include "sdfPacketBuff.h"
#include "sdf3DPrim.h"
#include "sdfSystem.h"
#include "sdfPrim.h"
#include "sdf_Decomp.h"
#include "sdf2D.h"
#include "sdfBillBoard.h"
#include "sdf3D.h"
#include "fldDraw.h"

typedef struct fldSpriteCorner_t fldSpriteCorner_t;

typedef struct fldSpriteVertex_t fldSpriteVertex_t;

typedef struct TMX_HEADER_t TMX_HEADER_t;

struct fldSpriteCorner_t {
    s32 u;
    s32 v;
    u8 pad08[8];
    s32 x;
    s32 y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
};

struct fldSpriteVertex_t {
    s32 r;
    s32 g;
    s32 b;
    s32 a;
    fldSpriteCorner_t corner[2];
};

typedef struct sdfTexBuf_t sdfTexBuf_t;


typedef struct frFontGlyph_t frFontGlyph_t;

typedef struct {
    s32 px;
    s32 py;
    s32 sx;
    s32 sy;
    u32 pz;
    u32 panelCol;
    u32 frameCol;
    s32 cx;
    s32 cy;
    s32 cz;
    sdfDrawTag_t *tag;
} dbgConsCtl_tag;

extern u32 fldDrawInitDone;
extern addr_t fldDrawEnv;
s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);
sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr);


sdfDrawTag_t *dds3FontReqAllocDrawTag(void);
extern u8 sdfViewMatrix[];
extern sdf3DScreenParam_t dds3ScreenParam;
extern u32 sdf2DMakeStdPacket(u32, s32);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg,
                                       s32 aNumOfRegs, s64 aGifRegs,
                                       s32 aNumOfPrim);

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);
extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);


extern sdfVector4_t fldTestLineVertex[];
extern u32 fldTestLineColor[];



extern u8 fldDbgFmtDec[];
extern void *fldDrwZBuf;
extern sdfGrOt_t sdfGrOtTable[85];
extern s32 rsetFrameMaskOtOffset;

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct {
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} fldDrawBank_t;

sdfGrOt_t *fldDrawGetOt(void);
void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th);

void DrwSprite2DC(s32 x, s32 y, s32 w, s32 h,
                   s32 u, s32 v, s32 uw, s32 vh,
                   u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th);
void fldPoint3To2(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2);
extern void rsetOtOfs(s32 otofs);
void rsetOtOfs(s32 otofs);
void rsetTexa(s32 ta0, s32 aem, u64 ta1);
void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);
void rsetAlpha(s32 mode);
void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 r, u32 g, u32 b);
void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 r, u32 g, u32 b);
void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);
void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 z, u32 r, u32 g, u32 b, u32 a);
void aDrawBoxF_Direct(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a);
void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2);
void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);
void drawSprite3D(sdfTexHandle_t *th, Sprite3D_t *spr3d);
void ooyTestLine3D(u32 x0, u32 y0, f32 z0, f32 col0, f32 x1, f32 y1, f32 z1, f32 col1);

void fldDrawPoly3(s32 x0, s32 y0, s32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2,
                         f32 col0, f32 col1, f32 col2);

void fldDrwZAlloc(void);
void fldDrwZFree(void);
void LF(dbgConsCtl_tag *cons);
void initDrawTag(dbgConsCtl_tag *cons);

INCLUDE_ASM(const s32, "field/fldDraw", mapEffEplInit);

sdfGrOt_t *fldDrawGetOt(void)
{
    return &sdfGrOtTable[rsetFrameMaskOtOffset];
}

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th) {
    sdfGrPacketHeader_t *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = sdf2DGetPrimPtr(handle);
    s32 ubase = u * 16;
    s32 xl = x * 16 + 0x7000;
    s32 vbase = v * 16;
    s32 yt = y * 8 + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = rgba & 0xFF;
    vtx->g = (rgba >> 8) & 0xFF;
    vtx->b = (rgba >> 16) & 0xFF;
    vtx->a = (rgba >> 24) & 0xFF;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + w * 16;
    vtx->corner[1].y = yt + h * 8;
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, th, 0);
    sdfDrawTagLink(command, EE_ADDR(handle));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void DrwSprite2DC(s32 x, s32 y, s32 w, s32 h,
                   s32 u, s32 v, s32 uw, s32 vh,
                   u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th) {
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 left, top, right, bottom;
    s32 uLeft, vTop, uRight, vBottom;

    list = dds3FontReqAllocDrawTag();
    sdf2DLinkTexPacket(list, th, 0);
    left = (x << 4) + 0x7000;
    top = (y << 3);
    right = (x << 4) + (w << 4) + 0x7000;
    bottom = top + (h << 3) + 0x7900;
    top += 0x7900;
    uLeft = (u << 4);
    vTop = (v << 4);
    uRight = uLeft + (uw << 4);
    vBottom = vTop + (vh << 4);
    sdfPrimLinkGt4(list, 0x40,
        left, top, uLeft, vTop, rgba0,
        right, top, uRight, vTop, rgba1,
        left, bottom, uLeft, vBottom, rgba3,
        right, bottom, uRight, vBottom, rgba2,
        -1, NULL);
    surface = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))surface->regFunc)(surface, list);
}

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th) {
    sdfGrPacketHeader_t *handle = sdf2DAllocSprite(1);
    fldSpriteVertex_t *vtx = sdf2DGetPrimPtr(handle);
    s32 r = rgba & 0xFF;
    s32 g = (rgba >> 8) & 0xFF;
    s32 b = (rgba >> 16) & 0xFF;
    s32 a = (rgba >> 24) & 0xFF;
    s32 ubase = u * 16;
    s32 vbase = v * 16;
    s32 xl = (s32)(x * 16.0f) + 0x7000;
    s32 yt = (s32)(y * 8.0f) + 0x7900;
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    vtx->r = r;
    vtx->g = g;
    vtx->b = b;
    vtx->a = a;
    vtx->corner[0].u = ubase;
    vtx->corner[0].v = vbase;
    vtx->corner[0].x = xl;
    vtx->corner[0].y = yt;
    vtx->corner[0].mask = -1;
    vtx->corner[0].flag = 0;
    vtx->corner[1].u = ubase + uw * 16;
    vtx->corner[1].v = vbase + vh * 16;
    vtx->corner[1].x = xl + (s32)(w * 16.0f);
    vtx->corner[1].y = yt + (s32)(h * 8.0f);
    vtx->corner[1].mask = -1;
    vtx->corner[1].flag = 0;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    sdf2DLinkTexPacket(command, th, 0);
    sdfDrawTagLink(command, EE_ADDR(handle));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void fldPoint3To2(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2) {
    sdfVector4_t vec = { z3, x2, y2, 1.0f };
    sdfVector4_t result;

    SDF_VU0_LOADMATRIX(sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_COPYMATRIX_A_TO_B();
    SDF_VU0_LOADVECTOR_MEMOPS(*(const u128 *)&vec);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&result);
    *x3 = result.x;
    *y3 = result.y;
}

INCLUDE_ASM(const s32, "field/fldDraw", fldPoint3ToZ);

INCLUDE_ASM(const s32, "field/fldDraw", fldPoint3To2H);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallMultMatrixRev);

INCLUDE_ASM(const s32, "field/fldDraw", fldPoint3To2_B);

void rsetOtOfs(s32 otofs)
{
    rsetFrameMaskOtOffset = otofs;
}

void rsetTexa(s32 ta0, s32 aem, u64 ta1) {
    sdfDrawTag_t *command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    s32 packet;
    u64 *entry;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    entry = EE_ADDR_AS(u64 *, sdf2DMakeStdPacket(packet, 0x30));
    entry[5] = 0x3B;
    entry[4] = (u64)(aem << 15) | (ta1 << 32) | ta0;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst) {
    sdfDrawTag_t *command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = EE_ADDR_AS(u64 *, sdf2DMakeStdPacket(packet, 0x30));
    data[4] = (ztst << 17) | 0x10000 | (datm << 15) | (date << 14) | (afail << 12) | (aref << 4) | (atst << 1) | ate;
    data[5] = 0x47;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void rsetAlpha(s32 mode) {
    sdfDrawTag_t *command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    s32 packet;
    u64 *data;
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    packet = sdfGrAllocPacketBuff(0x30);
    data = EE_ADDR_AS(u64 *, sdf2DMakeStdPacket(packet, 0x30));
    switch (mode) {
    case 1:
        data[4] = 0x48;
        break;
    case 2:
        data[4] = 0x42;
        break;
    case 3:
        data[4] = 0x84;
        break;
    case 4:
        data[4] = 0x06;
        break;
    case 5:
        data[4] = 0x89;
        break;
    case 6:
        data[4] = 0x42;
        break;
    case 10:
        data[4] = 0x2A;
        break;
    case 20:
        data[4] = 0x4A;
        break;
    default:
        data[4] = 0x44;
        break;
    }
    data[5] = 0x42;
    sdfDrawTagLink(command, packet);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineStripPrim(s32 x0, s32 y0, s32 x1, s32 y1, u32 r, u32 g, u32 b) {
    s32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2)));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = sdf2DGetPrimPtr(packet);
    lo = (u64)r | ((u64)g << 32);
    hi = (u64)b | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawLineDirect(u32 x0, u32 y0, u32 x1, u32 y1, u32 r, u32 g, u32 b) {
    u32 coords[4];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    sdfGrOt_t *descriptor;
    u32 *pos;
    u64 lo;
    u64 hi;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y1;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 2)));
    sdf2DMakePrimPacketHeader(packet, 0x49, 2, 0x41, 2);
    dst = sdf2DGetPrimPtr(packet);
    lo = (u64)r | ((u64)g << 32);
    hi = (u64)b | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = sdf2DGetPrimPtr(packet);
    lo = (u64)r | ((u64)g << 32);
    hi = (u64)b | ((u64)a << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 z, u32 r, u32 g, u32 b, u32 a) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
    s32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = sdf2DGetPrimPtr(packet);
    tag = z;
    lo = (u64)r | ((u64)g << 32);
    hi = (u64)b | ((u64)a << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawBoxF_Direct(s32 x, s32 y, s32 w, s32 h, u32 r, u32 g, u32 b, u32 a) {
    u32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    sdfGrOt_t *descriptor;
    s32 i;

    coords[0] = x;
    coords[1] = y;
    coords[2] = w;
    coords[3] = y;
    coords[4] = w;
    coords[5] = h;
    coords[6] = x;
    coords[7] = h;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = sdf2DGetPrimPtr(packet);
    lo = (u64)r | ((u64)g << 32);
    hi = (u64)b | ((u64)a << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFFFFULL;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

void aDrawTriG(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2) {
    s32 coords[6];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    sdfGrOt_t *descriptor;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    coords[4] = x2 * 16;
    coords[5] = y2 * 16;
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 3)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 3);
    dst = sdf2DGetPrimPtr(packet);
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(coords[i * 2] + 0x7000) |
            ((u64)(coords[i * 2 + 1] + 0x7900) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "field/fldDraw", aDrawBoxG);

void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3) {
    s32 coords[8];
    sdfDrawTag_t *command;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;
    s32 *pos;
    sdfGrOt_t *descriptor;

    coords[0] = (s32)(x * 16.0f);
    coords[1] = (s32)(y * 8.0f);
    coords[2] = (s32)((x + w) * 16.0f);
    coords[3] = (s32)(y * 8.0f);
    coords[4] = (s32)((x + w) * 16.0f);
    coords[5] = (s32)((y + h) * 8.0f);
    coords[6] = (s32)(x * 16.0f);
    coords[7] = (s32)((y + h) * 8.0f);
    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, 4)));
    sdf2DMakePrimPacketHeader(packet, 0x4D, 2, 0x41, 4);
    dst = sdf2DGetPrimPtr(packet);
    for (i = 0, pos = coords; i < 4; i++) {
        if (i == 0) {
            dst[0] = (u64)r0 | ((u64)g0 << 32);
            dst[1] = (u64)b0 | ((u64)a0 << 32);
        } else if (i == 1) {
            dst[0] = (u64)r1 | ((u64)g1 << 32);
            dst[1] = (u64)b1 | ((u64)a1 << 32);
        } else if (i == 2) {
            dst[0] = (u64)r2 | ((u64)g2 << 32);
            dst[1] = (u64)b2 | ((u64)a2 << 32);
        } else {
            dst[0] = (u64)r3 | ((u64)g3 << 32);
            dst[1] = (u64)b3 | ((u64)a3 << 32);
        }
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) |
            ((u64)(pos[1] + 0x7900) << 32);
        dst += 2;
        pos += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldCallGrAllocPacketBuff);

void drawSprite3D(sdfTexHandle_t *th, Sprite3D_t *spr3d) {
    sdfDrawTag_t *command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    DmaPacketHeader *header;
    u8 *packet;
    f32 mat[16];
    sdfGrOt_t *descriptor;

    sdfDrawTagInit(command);
    header = EE_ADDR_AS(DmaPacketHeader *, sdfGrAllocPacketBuff(0x20));
    sdf3DMakeDirectRefPacket(header, EE_ADDR(sdfTexGetPacket(th)), sdfTexGetPacketLength(th));
    sdfDrawTagLinkRef(command, EE_ADDR(header));
    SetRotateMatrixZ(spr3d->rot);
    SDF_VU0_STOREMATRIX(mat);
    packet = EE_ADDR_AS(u8 *, sdfGrAllocPacketBuff(0x38));
    sdfBillBoardMakeTMPacket(packet, mat);
    sdfDrawTagLink(command, EE_ADDR(packet));
    packet = EE_ADDR_AS(u8 *, sdfGrAllocPacketBuff(0x80));
    sdfBillBoardMake1(packet, (const f32 *)&spr3d->pos, spr3d->col, spr3d->uv, (const f32 *)spr3d->vt);
    sdfDrawTagLink(command, EE_ADDR(packet));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "field/fldDraw", rsetFrameMaskRgbOn);

INCLUDE_ASM(const s32, "field/fldDraw", rsetFrameMaskRgbOff);

void ooyTestLine3D(u32 x0, u32 y0, f32 z0, f32 col0, f32 x1, f32 y1, f32 z1, f32 col1) {
    sdfDrawTag_t *list;
    saddr_t record;
    sdfGrOt_t *descriptor;
    fldTestLineVertex[0].x = z0;
    fldTestLineVertex[0].y = col0;
    fldTestLineVertex[0].z = x1;
    fldTestLineVertex[1].x = y1;
    fldTestLineVertex[1].y = z1;
    fldTestLineVertex[1].z = col1;
    fldTestLineColor[1] = y0;
    fldTestLineColor[0] = x0;
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    record = EE_ADDR_S(effDebugDrawWireModel(fldTestLineVertex, fldTestLineColor, 2, 0x80));
    sdfDrawTagLink(list, record);
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, list);
}

void fldDrawPoly3(s32 x0, s32 y0, s32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2,
                         f32 col0, f32 col1, f32 col2) {
    sdf3DPrimPolyParm_t desc;
    sdfVector4_t vertices[3];
    u32 colors[3];
    sdfDrawTag_t *command;
    sdfGrOt_t *descriptor;

    command = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(command);
    sdf3DPrimLinkResetTm(command, 0);
    sdf3DPrimLinkMat(command, EE_ADDR_AS(sdfMat_t *, fldDrawEnv), 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.faceCount = 1;
    desc.pointCount = 3;
    desc.vertex = vertices;
    desc.vertexColor = colors;
    vertices[0].x = x1;
    vertices[0].y = y1;
    vertices[0].z = z1;
    vertices[1].x = x2;
    vertices[1].y = y2;
    vertices[1].z = z2;
    vertices[2].x = col0;
    vertices[2].y = col1;
    vertices[2].z = col2;
    colors[0] = x0;
    colors[1] = y0;
    colors[2] = z0;
    sdfDrawTagLink(command, EE_ADDR(sdf3DPrimMakePoly(&desc)));
    descriptor = &sdfGrOtTable[rsetFrameMaskOtOffset];
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))descriptor->regFunc)(descriptor, command);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawFrameSprite);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "field/fldDraw", fldDrawBokasi);

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawFrameSpriteAlpha);

#endif
INCLUDE_ASM(const s32, "field/fldDraw", fldDrawPos);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallOtOfs);

void fldLeaf17(void) {
}

void fldDrwZAlloc(void)
{
    if (fldDrwZBuf == NULL) {
        fldDrwZBuf = sdfAllocAuto(0x70000);
    }
}

void fldDrwZFree(void)
{
    if (fldDrwZBuf != NULL) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = NULL;
    }
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDrwZSave);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallPrimAllocDrawTag);

INCLUDE_ASM(const s32, "field/fldDraw", fldKanjiDraw);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallSetFontString);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallSetFontString2);

void LF(dbgConsCtl_tag *cons)
{
    cons->cy += 0x60;
}

void initDrawTag(dbgConsCtl_tag *cons) {
    s32 packet;
    sdfDrawTag_t *packetList;

    packetList = dds3FontReqAllocDrawTag();
    cons->tag = packetList;
    packet = sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(EE_ADDR_AS(sdfPrimTransPacket_t *, packet));
    sdfDrawTagLink(cons->tag, packet);
}

INCLUDE_ASM(const s32, "field/fldDraw", fldDbgPrint3D);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "field/fldDraw", fldObjTaskSub);

INCLUDE_ASM(const s32, "field/fldDraw", fldCallSetOt);

#endif
INCLUDE_ASM(const s32, "field/fldDraw", fldDrawMaruKage);

INCLUDE_ASM(const s32, "field/fldDraw", nbProcSequenceSub);

INCLUDE_ASM(const s32, "field/fldDraw", nbSetEncountMode);

INCLUDE_ASM(const s32, "field/fldDraw", nbCallSetOt4);

INCLUDE_ASM(const s32, "field/fldDraw", nbCallGetY);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrawInitDone);

void *fldDrwZBuf = 0;

SDATA_PAD(4);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDbgFmtDec);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "field/fldDraw", fldObjTaskSubData);

#endif
INCLUDE_SDATA(const s32, "field/fldDraw", nbData4);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDbgFmtHex);

INCLUDE_SDATA(const s32, "field/fldDraw", fldDrawEnv);

