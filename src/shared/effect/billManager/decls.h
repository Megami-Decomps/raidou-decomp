/* Declarations of smt3-decomp src/smt3/effect/billManager.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_BILLMANAGER_H
#define SHARED_EFFECT_BILLMANAGER_H

#include "common.h"

#include "sdk_libgcc.h"

#include "basic.h"

#include "draw3d.h"

#include "libsdf.h"

#include "fileManager.h"

#include "ps2asm.h"

#include "billboard.h"

#include "draw.h"

#include "sdfFrame.h"

typedef struct dds3P2APolygon_tag {
    f32 dir[4][4];
    f32 rot[4][4];
    struct billHandle_t *hBillboard;
    sdfMat_t *pMat;
} dds3P2APolygon_tag;

typedef struct dds3EffectFunc3_t {
    struct billHandle_t *(*create)(void *);
    void (*draw)(struct billHandle_t *);
    void (*destroy)(struct billHandle_t *);
} dds3EffectFunc3_t;

extern dds3EffectFunc3_t dds3BillboardFuncTable[];

extern struct billHandle_t *hGeneralBillboard[];

#include "sdk_libc.h"

typedef struct billGeneralEntry_t {
    s16 kind;
    u8 pad02[6];
} billGeneralEntry_t;

typedef struct billGeneralRow_t {
    u8 pad00[4];
    s16 kind;
    u8 pad06[2];
} billGeneralRow_t;

extern billGeneralEntry_t BillboardTable[];

extern billGeneralRow_t dds3BillboardGeneralRows[];

extern struct dds3BillResource_tag *dds3BillboardTag;

extern void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

extern void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

extern void *sdfMemGetPtr(struct sdfMemHandle_t *aHandle);

extern fileRequest_t *pacLoad(const char *filename);

extern void pacSync(fileRequest_t *pacFileHandle);

extern void pacFileHandleFree_(void *pacHandle);

billHandle_t *dds3BillboardGeneral(s32 number);

extern sdfGrOt_t *dds3BillboardOtTable[];

typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

extern effBlurDrawBank_t dds3DrawEnv[];

extern billRenderPair_t *dds3MultiBillboardPairList;

extern sdfGrOt_t sdfGrOtTable[];

extern f32 dds3BillboardTagParam[4];

extern f32 dds3BillboardUpAxis[4];

extern sdf3DScreenParam_t dds3ScreenParam;

extern f32 D_002FA240[16];

extern void MultMatrixRev(f32 *aMatrix);

extern f32 atan2f(f32 y, f32 x);

extern sdfTexPacket_t *sdfTexGetPacket(sdfTexHandle_t *aHandle);

extern s32 sdfTexGetPacketLength(sdfTexHandle_t *aHandle);

extern void sdf3DMakeDirectRefPacket(void *aPacket, addr_t aData, s32 aLeng);

extern void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdfBillBoardMakeTMPacket(void *aPacket, const f32 *aTM);

extern f32 sdfCos(f32 aRadius);

extern f32 sdfSin(f32 aRadius);

extern f64 fabs(f64 value);

SYMBOL_AT(sdfGrOt_t, dds3MultiBillboardOt, sdfGrOtTable, 0x580);

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern u32 dds3GetFrameID(void);

extern void sdfGrSetFrameMaskReset(const u8 *aEnv, void *aHdr, s32 aContext);

extern void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, addr_t aPacket);

extern u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor,
                              const void *aUVList, const void *aXYOffsetList,
                              s32 aNum, void *(*aAllocator)(s32));

struct billHandle_t *dds3Billboard_NormalCreate(void *data);

u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1);

typedef struct billColorWork_t {
    u32 word;
    u8 pad04[12];
} billColorWork_t;

typedef struct P2A_ANIMATION_tag {
    s32 spranm;
    s32 alpha;
    u32 attr;  
    u32 times;
    u32 mode;
} P2A_ANIMATION_tag;

typedef struct billData_t {
    struct sdfMemHandle_t *hMem;  
    u8 *pHeader;
    P2A_ANIMATION_tag *pAnimTable;
    s32 AnimNum;
    s32 TexNum;
    s32 count;
    struct dds3BillResource_tag **pResList;
} dds3BillAnimResource_tag;

typedef struct P2A_TEXANIM_tag {
    s16 sx;
    s16 sy;
    s16 cx;
    s16 cy;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    s16 texid;
    s16 attr;  
    f32 angle;  
} P2A_TEXANIM_tag;

typedef struct billOut_t {
    s32 hResource;
    s32 key;
    s32 wait;
    P2A_ANIMATION_tag *pAnim;
    P2A_TEXANIM_tag *pTexAnim;
} dds3BillAnim_tag;

enum {
    BILL_ANIMATION_FLAG_FRAME_COLORS = 0x00000001,
    BILL_ANIMATION_FLAG_PACKET_LIST_2 = 0x00000002,
    BILL_ANIMATION_FLAG_PACKET_LIST_3 = 0x00000004,
    BILL_ANIMATION_FLAG_PACKET_LIST_MASK = 0x00000006,
    BILL_ANIMATION_FLAG_STOP_AT_END = 0x00000010,
    BILL_ANIMATION_FLAG_PLURAL_ENTRIES = 0x10000000
};

/* One row of the plural-entry table a P2A animation points at: the sprite
 * animation to run, its start delay, and the centre the quad is drawn at.
 * The remaster models the table as raw data, so this name is the tree's own. */
typedef struct billPluralEntry_t {
    s16 entryIndex;
    s16 delay;
    s16 x;
    s16 y;
    u8 unk08[0x10];
} billPluralEntry_t;

#define BILL_VARIANT_MASK 0xFFFF

void dds3BillboardTagLink(billHandle_t *obj, dds3BillResource_tag *child);

dds3BillAnimResource_tag *dds3BillAnimLoadP2A(void *resourceData);

void *sdfChipAllocDirect(s32 aSize);

void dds3BillboardSetScale(struct billHandle_t *hBillboard, f32 x, f32 y);

typedef struct P2A_HEAD_t {
    s32 tmxofs;
    s32 AnimNum;
} P2A_HEAD_t;

#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4

/* Initialize the paired texture/state list, then append one quad to each
 * child stream. The second child owns the geometry and common batch count. */
void dds3MultiBillboardTagLink(billHandle_t *obj, billRenderPair_t *node);

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard);

billHandle_t *dds3BillboardCreate(s32 index, addr_t data);

void dds3BillboardDelete(billHandle_t *hData);

/* Drop one reference; the last one releases every entry and the block itself. */
void dds3BillAnimFreeP2A(dds3BillAnimResource_tag *hBillAnimRes);

/* Select an animation and initialize the plural records' signed start delays. */
void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number);

/* Advance an entry's frame timer and apply its current record to the shared
 * child payload. Plural descriptors are handled by the dispatcher instead. */
dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim);

extern void *memcpy(void *a, const void *b, size_t c);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* Build a quad packet from paired colors and per-corner UV sets. */
u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

sdfMat_t *sdfMatNew(void);

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *aMat);

void sdfChipFreeDirect(void *aPtr);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);

void sdfMatSetAmbient(struct sdfMat_t *aMat, u32 aColor);

void sdfMatSetDiffuse(struct sdfMat_t *aMat, u32 aColor);

void sdfMatSetSpecular(struct sdfMat_t *aMat, u32 aColor);

/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const void *aTex);

extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

void BillboardFreeTex(dds3BillResource_tag *hResource);

dds3BillResource_tag *BillboardLoadTex(void *pTex);

void dds3MultiBillboardPktLink(billRenderPair_t *hResource);

void dds3Billboard_NormalDelete(billHandle_t *hData);

billHandle_t *dds3Billboard_P2ACreate(void *resourceData);

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A);

void dds3Billboard_P2ADelete(billHandle_t *hData);

void dds3Billboard_P2ADraw(billHandle_t *hData);

void dds3BillboardDraw(billHandle_t *hData);

u32 dds3BillboardGeneralNumber(void);

void dds3BillboardSetPosition(void *hBillboard, const void *pPos);

void dds3BillboardSetRotate(billHandle_t *hBillboard, float rot);

void dds3BillboardSetRGBA(billHandle_t *hBillboard, u32 abgr);

void func_0015D050(billHandle_t *effect, const billUvQuad_t *textureQuad);

void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition);

u16 dds3BillboardGetType(billHandle_t *hBillboard);

u16 dds3BillboardGetDrawType(billHandle_t *hBillboard);

s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *hBillboard);

s32 dds3BillboardGetAnimNumber(billHandle_t *hBillboard);

s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard);

s32 dds3BillboardGetAnimType(billHandle_t *hBillboard);

f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec);

dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex);

dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source);

void dds3P2APolygonDelete(dds3P2APolygon_tag *hData);

void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos);

void dds3P2APolygonSetRotateMatrix(dds3P2APolygon_tag *hData, void *pMatrix);

void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr);

void dds3P2APolygonSetDirection(void *hData, void *pMatrix);

void dds3BillboardManagerExit(void);

dds3BillResource_tag *BillboardLoadTex(void *pTex);

void BillboardFreeTex(dds3BillResource_tag *hResource);

/* Adapted from Megami-Decomps/dds-decomp, effect/billManager.c at f79f86dc.
 * Each row is eight actual X/Y offsets, consumed as a 32-byte quad stream. */
void dds3BillboardTagLink(billHandle_t *obj, dds3BillResource_tag *child);

void dds3BillboardLinkOt(void);

void dds3MultiBillboardPktLink(billRenderPair_t *hResource);

void dds3MultiBillboardLinkOt(void);

void dds3Billboard_BasicInit(billHandle_t *pBasic);

billHandle_t *dds3Billboard_NormalCreate(void *data);

void dds3Billboard_NormalDelete(billHandle_t *hData);

void dds3Billboard_NormalDraw(billHandle_t *hData);

billHandle_t *dds3Billboard_P2ACreate(void *resourceData);

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A);

void dds3Billboard_P2ADelete(billHandle_t *hData);

dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim);

/* vu0 routine: unpack two colours, multiply the lanes and pack. */
u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1);

void dds3Billboard_P2ADraw(billHandle_t *hData);

void dds3Billboard_P2ASetAnim(dds3BillAnimResource_tag *hBillAnimRes, s32 number, dds3BillAnim_tag *pBillAnim);

void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number);

dds3BillAnimResource_tag *dds3BillAnimLoadP2A(void *resourceData);

void dds3BillAnimFreeP2A(dds3BillAnimResource_tag *hBillAnimRes);

void dds3BillboardCopyO(billHandle_t *obj, s32 *out);

billHandle_t *dds3BillboardCreate(s32 index, addr_t data);

billHandle_t *dds3BillboardFileLoad(s32 index, const char *name);

billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard);

void dds3BillboardDelete(billHandle_t *hData);

void dds3BillboardDraw(billHandle_t *hData);

billHandle_t *dds3BillboardGeneral(s32 number);

u32 dds3BillboardGeneralNumber(void);

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

void dds3BillboardSetPosition(void *hBillboard, const void *pPos);

/* Set the radian rotation applied to the billboard quad corners. */
void dds3BillboardSetRotate(billHandle_t *hBillboard, float rot);

/* Set the two child scale components stored at +0x10 and +0x14. */
void dds3BillboardSetScale(billHandle_t *hBillboard, float x, float y);

void dds3BillboardSetRGBA(billHandle_t *hBillboard, u32 abgr);

void func_0015D050(billHandle_t *effect, const billUvQuad_t *textureQuad);

void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode);

/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition);

saddr_t dds3BillboardGetTex(billHandle_t *hBillboard);

u16 dds3BillboardGetType(billHandle_t *hBillboard);

void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type);

u16 dds3BillboardGetDrawType(billHandle_t *hBillboard);

void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number);

/* Read the selected entry for list billboards; other kinds have none. */
s32 dds3BillboardGetCurrentAnimNumber(billHandle_t *hBillboard);

s32 dds3BillboardGetAnimNumber(billHandle_t *hBillboard);

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);

void dds3BillboardSetAnimKey2(billHandle_t *hBillboard, u32 frame);

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard);

u16 dds3BillboardGetAnimState(const billHandle_t *hBillboard);

s32 dds3BillboardGetAnimType(billHandle_t *hBillboard);

void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard);

void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y);

/* vu0 routine: screen-space angle of a world axis at a world position. */
f32 dds3BillboardGetDirectionRotate(void *pPos, f32 *pVec);

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec);

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex);

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source);

void dds3P2APolygonDelete(dds3P2APolygon_tag *hData);

void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos);

void dds3P2APolygonSetScale(s32 hData, float scale);

/* vu0 routine: copy the supplied 4x4 matrix to the instance's second matrix. */
void dds3P2APolygonSetRotateMatrix(dds3P2APolygon_tag *hData, void *pMatrix);

void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void dds3P2APolygonSetDirection(void *hData, void *pMatrix);

typedef struct {
    s32 mode;
    dds3BillResource_tag *primary;
    dds3BillResource_tag *secondary;
} dds3P2APolygonResource_t;

void dds3P2APolygonGetResource(dds3P2APolygon_tag *hP2APoly, dds3P2APolygonResource_t *pP2APolyRes);

extern u32 dds3P2APolygonFaceIndex[];

extern u32 dds3P2APolygonVertexColor[];

extern sdfGrOt_t *dds3P2APolygonSurfaceTable[];

extern void MultMatrixNoArg(void);

extern void sdf3DPrimLinkTm(sdfDrawTag_t *, s32 (*)(s32));

extern void sdf3DPrimLinkMat(sdfDrawTag_t *, sdfMat_t *, s32 (*)(s32));

extern void sdfMatSetTex(sdfMat_t *, sdfTexHandle_t *);

extern void sdfMatSetMultiTex(sdfMat_t *, sdfTexHandle_t *);

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

void dds3P2APolygonDraw(dds3P2APolygon_tag *instance);

#endif
