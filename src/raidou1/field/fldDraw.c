#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldDraw.c */
#include "fpu.h"

#include "common.h"

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

/* Declared here because no header carries these. */
extern frFontHandle_t *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, const char *pStr, frFontHandle_t *pFRQ);

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

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

typedef struct dmaPacketHeader_t dmaPacketHeader_t;

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

sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

extern u32 fldDrawInitDone;

extern addr_t fldDrawEnv;

extern sdfTexHandle_t *fldDrawSpriteTex;

extern addr_t D_00551C9C;

extern u8 sdfTexEmbedded_2FC4A0[];

extern sdfMat_t *sdfMatNew(void);

s32 sdf2DLinkTexPacket(sdfDrawTag_t *aTag, sdfTexHandle_t *aTexHndl, s32 aCtx);

sdfGrPacketHeader_t *sdf2DAllocSprite(s32 aNumOfSpr);

void sdfDrawTagInit(sdfDrawTag_t *a);

void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

s32 sdfGrAllocPacketBuff(s32 aSize);

extern saddr_t sdf2DGetPrimPtr(saddr_t);

extern void sdfPrimLinkGt4(
    sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
    s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2,
    s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3,
    s32 aZ, s32 (*aAllocator)(s32));

sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

void MultMatrixRev(const void *aMatrix);

extern u8 sdfViewMatrix[];

extern sdf3DScreenParam_t dds3ScreenParam;

extern sdf3DScreenParam_t dds3ScreenParam2;

extern u8 aProjMatrix[];

extern u32 sdf2DMakeStdPacket(u32, s32);

sdfGrPacketHeader_t *sdf2DMakePrimPacketHeader(sdfGrPacketHeader_t *aPacketPtr, s32 aPrimReg,
                                       s32 aNumOfRegs, s64 aGifRegs,
                                       s32 aNumOfPrim);

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

struct sdfTexBuf_t *sdfTexGetPacket(sdfTexHandle_t *aHandle);

s32 sdfTexGetPacketLength(const sdfTexHandle_t *aHandle);

void SetRotateMatrixZ(f32 aAngle);

void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdf3DMakeDirectRefPacket(dmaPacketHeader_t *, addr_t, s32);

extern void sdfBillBoardMakeTMPacket(u8 *, const f32 *);

extern void sdfBillBoardMake1(u8 *, const sdfVector4_t *, u32, const UV_t *, const VTX_t *);

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);

extern sdfVector4_t fldTestLineVertex[];

extern u32 fldTestLineColor[];

extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag,
                              s32 (*aAllocator)(s32));

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

extern void *memset(void *, int, size_t);

void sdfFreeAutoDelayed(void *data);

extern void sdfLinkStoreImage(
    sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aSrcX, s32 aSrcY,
    s32 aWidth, s32 aHeight, saddr_t aBuffer, s32 aCallback, s32 aCallbackParam,
    s32 (*aAllocator)(s32));

extern sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));

extern sdfGrFrameSet_t sdfGrFrameSet;

extern void sdfLinkLoadImage(
    sdfDrawTag_t *aDrawTag, sdfGsMemSlice_t *aGsHandle, s32 aDstX,
    s32 aDstY, s32 aWidth, s32 aHeight, saddr_t aBuffer,
    s32 (*aAllocator)(s32));

s32 frDestroyFontRequestAfter(struct frFontGlyph_t *pFRQ);

s32 frDrawFont(frFontGlyph_t *pFRQ);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);

void sdfDevSetPrintCB(sifCommand_t *aCB, s32 aX, s32 aY, s32 aZ, s32 aAttr);

void *sdfDevPrintfEx(void *aCB, const char *aStr, ...);

extern u8 fldDbgFmtDec[];

extern void InversMatrix(void);

extern u8 fldDbgFmtHex[];

f32 sdfSin(f32 aRadius);

f32 sdfCos(f32 aRadius);

extern void *memcpy(void *, const void *, size_t);

extern void *fldDrwZBuf;

extern void *sdfAllocAuto(u32 size);

extern void sdfFreeAutoDelayed(void *p);

extern sdfGrOt_t sdfGrOtTable[85];

extern s32 gencOn;

extern s32 gencMode;

extern s32 rsetFrameMaskOtOffset;

/* dds3DrawBank_t stride; only the held frame texture block at 0x1300 is used here. */
typedef struct {
    u8 unk0000[0x1300];
    u8 frameTex[0x8E0];
} fldDrawBank_t;

extern fldDrawBank_t dds3DrawEnv[];

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskRGB(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, addr_t aEnv, void *aPkt);

void fldDrawInit(void);

sdfGrOt_t *fldDrawGetOt(void);

void DrwSprite2D(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th);

void DrwSprite2DC(s32 x, s32 y, s32 w, s32 h,
                   s32 u, s32 v, s32 uw, s32 vh,
                   u32 rgba0, u32 rgba1, u32 rgba2, u32 rgba3, sdfTexHandle_t *th);

void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh, s32 rgba, sdfTexHandle_t *th);

void fldPoint3To2(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2);

void fldPoint3To2H(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2);

void fldPoint3To2_A(void);

void fldPoint3To2_B(f32 *x3, f32 *y3, f32 z3, f32 x2, f32 y2);

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

void aDrawBoxG(s32 x, s32 y, s32 w, s32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);

void aDrawBoxGF(f32 x, f32 y, f32 w, f32 h, u32 r0, u32 g0, u32 b0, u32 a0, u32 r1, u32 g1, u32 b1, u32 a1, u32 r2, u32 g2, u32 b2, u32 a2, u32 r3, u32 g3, u32 b3, u32 a3);

void func_0012A140(s32 x, s32 y, s32 w, s32 h, u32 gsWord0, u32 gsWord1, u32 gsWord2, u32 gsWord3, u32 vertexTag);

void drawSprite3D(sdfTexHandle_t *th, Sprite3D_t *spr3d);

void rsetFrameMaskRgbOn(void);

void rsetFrameMaskRgbOff(void);

void ooyTestLine3D(u32 x0, u32 y0, f32 z0, f32 col0, f32 x1, f32 y1, f32 z1, f32 col1);

void fldDrawPoly3(s32 x0, s32 y0, s32 z0,
                         f32 x1, f32 y1, f32 z1,
                         f32 x2, f32 y2, f32 z2,
                         f32 col0, f32 col1, f32 col2);

extern u64 fldQuadFaceIndex[];

void fldDrawPoly4(s32 x0, f32 y0, f32 z0, f32 x1,
                  s32 y1, f32 z1, f32 x2, f32 y2,
                  s32 z2, f32 x3, f32 y3, f32 z3,
                  s32 col0, f32 col1, f32 col2, f32 col3);

void fldDrawTexPoly4(s32 x0, f32 y0, f32 z0, f32 x1,
                     s32 y1, f32 z1, f32 x2, f32 y2,
                     s32 z2, f32 x3, f32 y3, f32 z3,
                     s32 s0, f32 t0, f32 s1, f32 t1,
                     sdfTexHandle_t *col0, f32 col1, f32 col2, f32 col3, f32 th);

void fldDrawFrameSprite(void);

void fldDrawBokasi(void);

void fldDrawFrameSpriteAlpha(s32 alpha);

void fldDrawPos(const f32 *pos);

void fldDrawPosCol(const f32 *pos, s32 rgba);

void fldDraw2DBar(s32 x, s32 y, s32 w, s32 h);

void fldDrwZAlloc(void);

void fldDrwZFree(void);

void fldDrwZSave(void);

void fldDrwZLoad(void);

void fldKanjiDraw(s32 x, s32 y, u32 rgba, const u8 *moji);

void LF(dbgConsCtl_tag *cons);

void initDrawTag(dbgConsCtl_tag *cons);

void fldDbgPrint3D(f32 x, f32 y, f32 z, s32 num);

void fldDbgPrint2D(s32 x, s32 y, const char *str);

void fldDbgPrint2DC(s32 x, s32 y, s32 col, s32 str);

void fldDbgPrintNum2D(s32 x, s32 y, s32 num);

void fldDbgPrintNum2DH(s32 x, s32 y, s32 num);

void fldDbgPrintStr2D(s32 x, s32 y, s32 str);

void fldDbgPrintStr2DC(s32 x, s32 y, s32 packetField, s32 drawValue);

void fldDbgPrintStr2DCF(s32 x, s32 y, f32 col, f32 str);

void fldDrawMaruKage(u32 fade, f32 x, f32 y, f32 z, f32 radius);

s32 nbGetEncountOn(void);

void nbSetEncountMode(s32 mode);

extern u32 fldDrawInitDone;

extern void *fldDrwZBuf;
/* End of declarations from src/smt3/field/fldDraw.c */

INCLUDE_ASM(const s32, "field/fldDraw", func_00132850);

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawGetOt);

INCLUDE_ASM(const s32, "field/fldDraw", DrwSprite2D);

INCLUDE_ASM(const s32, "field/fldDraw", DrwSprite2DC);

INCLUDE_ASM(const s32, "field/fldDraw", DrwSprite2DF);

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

INCLUDE_ASM(const s32, "field/fldDraw", func_00132E28);

INCLUDE_ASM(const s32, "field/fldDraw", func_00132EF0);

INCLUDE_ASM(const s32, "field/fldDraw", func_00132FC8);

INCLUDE_ASM(const s32, "field/fldDraw", func_00133030);

void rsetOtOfs(s32 otofs)
{
    rsetFrameMaskOtOffset = otofs;
}

INCLUDE_ASM(const s32, "field/fldDraw", rsetTexa);

INCLUDE_ASM(const s32, "field/fldDraw", rsetTest);

INCLUDE_ASM(const s32, "field/fldDraw", rsetAlpha);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawLineStripPrim);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawLineDirect);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawBoxF);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawBoxFZ);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawBoxF_Direct);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawTriG);

INCLUDE_ASM(const s32, "field/fldDraw", func_00133CD0);

INCLUDE_ASM(const s32, "field/fldDraw", aDrawBoxGF);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134208);

INCLUDE_ASM(const s32, "field/fldDraw", drawSprite3D);

INCLUDE_ASM(const s32, "field/fldDraw", func_001344B8);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134550);

INCLUDE_ASM(const s32, "field/fldDraw", ooyTestLine3D);

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawPoly3);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134818);

INCLUDE_ASM(const s32, "field/fldDraw", func_001349E0);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134AA0);

void func_00134B58(void) {
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

INCLUDE_ASM(const s32, "field/fldDraw", func_00134BB8);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134C48);

INCLUDE_ASM(const s32, "field/fldDraw", fldKanjiDraw);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134D20);

INCLUDE_ASM(const s32, "field/fldDraw", func_00134DA0);

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

INCLUDE_ASM(const s32, "field/fldDraw", fldDrawMaruKage);
INCLUDE_SDATA(const s32, "field/fldDraw", D_0046B730);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0046B734);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0046B738);

INCLUDE_SDATA(const s32, "field/fldDraw", D_0046B740);

