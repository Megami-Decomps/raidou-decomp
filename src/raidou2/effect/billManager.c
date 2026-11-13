#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/billManager.c */
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

extern f32 sdfViewMatrix[16];

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

extern sdfGrOt_t dds3MultiBillboardOt;

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
extern u32 *sdfMultiBillBoardMakeN(u128 *aCenter, void *aColor, void *aUVList, void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

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
/* End of declarations from src/smt3/effect/billManager.c */

INCLUDE_ASM(const s32, "effect/billManager", func_001F3318);

INCLUDE_ASM(const s32, "effect/billManager", func_001F3370);

INCLUDE_ASM(const s32, "effect/billManager", func_001F3488);

INCLUDE_ASM(const s32, "effect/billManager", func_001F34D8);

INCLUDE_ASM(const s32, "effect/billManager", func_001F3978);

INCLUDE_ASM(const s32, "effect/billManager", func_001F3A68);

INCLUDE_ASM(const s32, "effect/billManager", func_001F40C8);

INCLUDE_ASM(const s32, "effect/billManager", func_001F4228);

void dds3Billboard_BasicInit(billHandle_t *pBasic) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    pBasic->mode = 0;
    pBasic->pos[3] = zero;
    pBasic->pos[2] = zero;
    pBasic->pos[1] = zero;
    pBasic->pos[0] = zero;
    pBasic->unk18 = one;
    pBasic->childScaleY = one;
    pBasic->unk1C = zero;
    pBasic->childScaleX = one;
    pBasic->abgr = 0x80808080;
    pBasic->rot = zero;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F43C8);

INCLUDE_ASM(const s32, "effect/billManager", func_001F4418);

void dds3Billboard_NormalDraw(billHandle_t *hData) {
    dds3BillboardTagLink(hData, hData->u.child);
}

billHandle_t *dds3Billboard_P2ACreate(void *resourceData) {
    dds3BillAnimResource_tag *data;
    billHandle_t *newobj;
    s32 n;

    data = NULL;
    if (resourceData != NULL) {
        data = dds3BillAnimLoadP2A(resourceData);
    }
    /* BUG: a NULL resourceData leaves data NULL and retail reads AnimNum through it. */
    n = data->AnimNum;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (dds3BillAnim_tag *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    newobj->pair.cameraFacingMode = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A) {
    dds3BillAnimResource_tag *data;
    s32 n;
    billHandle_t *newobj;

    data = pCopyP2A->u.animationData;
    n = data->AnimNum;
    data->count = data->count + 1;
    newobj = sdfChipAllocDirect(n * 20 + 0x6C);
    newobj->u.animationData = data;
    newobj->resolvedEntries = (dds3BillAnim_tag *)((u8 *)newobj + 0x6C);
    newobj->animationActive = 1;
    newobj->pair.packetList = 0;
    newobj->pair.next = 0;
    dds3Billboard_P2ASetNumber(newobj, 0);
    return newobj;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F4570);

dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim) {
    P2A_ANIMATION_tag *entry = pBillAnim->pAnim;
    dds3BillAnimResource_tag *data = pP2A->u.animationData;
    P2A_TEXANIM_tag *record;
    dds3BillResource_tag *child;
    s32 index;
    u32 color;

    if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        return NULL;
    }
    if (pBillAnim->wait <= 0) {
        pBillAnim->key++;
        if ((u32)pBillAnim->key >= entry->times) {
            if (entry->mode & BILL_ANIMATION_FLAG_STOP_AT_END) {
                pP2A->animationActive = 0;
                pBillAnim->key = entry->times - 1;
            } else {
                pBillAnim->key = 0;
            }
        }
        pBillAnim->wait = SDF_FRAMES(pBillAnim->pTexAnim[pBillAnim->key].attr + 1) - 1;
    } else {
        pBillAnim->wait--;
    }
    index = pBillAnim->key;
    record = pBillAnim->pTexAnim + index;
    if (entry->mode & BILL_ANIMATION_FLAG_FRAME_COLORS) {
        color = ((u32 *)(data->pHeader + entry->alpha))[index];
    } else {
        color = 0x80808080;
    }
    pP2A->abgr = color;
    child = data->pResList[record->texid];
    if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
        pP2A->mode = 2;
    } else if (entry->mode & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
        pP2A->mode = 3;
    } else {
        pP2A->mode = 1;
    }
    pP2A->rot = record->angle;
    child->uv.components[0] = record->u1;
    child->uv.components[1] = record->v1;
    child->uv.components[2] = record->u2;
    child->uv.components[3] = record->v1;
    child->uv.components[4] = record->u2;
    child->uv.components[5] = record->v2;
    child->uv.components[6] = record->u1;
    child->uv.components[7] = record->v2;
    {
        f32 halfHeight = record->sy * 0.5f;
        f32 halfWidth = record->sx * 0.5f;
        f32 y = halfHeight - record->cy;
        f32 x = halfWidth - record->cx;

        child->height = halfHeight;
        child->width = halfWidth;
        child->cy = y;
        child->cx = x;
    }
    return child;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F4770);

INCLUDE_ASM(const s32, "effect/billManager", dds3Billboard_P2ADraw);

void dds3Billboard_P2ASetAnim(dds3BillAnimResource_tag *hBillAnimRes, s32 number, dds3BillAnim_tag *pBillAnim) {
    u8 *base;
    P2A_ANIMATION_tag *entry;
    s32 offset;
    s16 value;

    base = hBillAnimRes->pHeader;
    entry = hBillAnimRes->pAnimTable + number;
    offset = entry->spranm;
    pBillAnim->pAnim = entry;
    base = base + offset;
    pBillAnim->key = 0;
    value = ((P2A_TEXANIM_tag *)base)->attr;
    pBillAnim->pTexAnim = (P2A_TEXANIM_tag *)base;
    pBillAnim->wait = SDF_FRAMES(value + 1) - 1;
}

void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number) {
    dds3BillAnimResource_tag *data = pP2A->u.animationData;
    P2A_ANIMATION_tag *entry = data->pAnimTable + number;

    if (entry->times == 0) {
        pP2A->animationActive = 0;
        return;
    }
    if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        billPluralEntry_t *records;
        u32 i = 0;

        printf("billAnim..PLURAL SET\n");
        pP2A->modeFlags = 0x10000000;
        pP2A->animationEntryIndex = number;
        pP2A->entryCount = entry->times;
        records = (billPluralEntry_t *)(data->pHeader + entry->spranm);
        for (; i < entry->times; i++) {
            dds3Billboard_P2ASetAnim(data, records[i].entryIndex, pP2A->resolvedEntries + i);
            (pP2A->resolvedEntries)[i].key = -SDF_FRAMES(records[i].delay);
        }
    } else if (entry->attr & 0xC0) {
        printf("billAnim..(A)MTEX SET\n");
        pP2A->modeFlags = entry->attr;
        pP2A->animationEntryIndex = number;
        pP2A->entryCount = 2;
        dds3Billboard_P2ASetAnim(data, number, pP2A->resolvedEntries);
        dds3Billboard_P2ASetAnim(data, number + 1, pP2A->resolvedEntries + 1);
    } else {
        pP2A->modeFlags = 0;
        pP2A->entryCount = 1;
        pP2A->animationEntryIndex = number;
        dds3Billboard_P2ASetAnim(data, number, pP2A->resolvedEntries);
    }
    if (entry->attr & 0x100) {
        printf("billAnim..P2A POLYGON\n");
    }
    pP2A->animationActive = 1;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F4C90);

INCLUDE_ASM(const s32, "effect/billManager", func_001F4E60);

INCLUDE_ASM(const s32, "effect/billManager", billCopyCurrentRecordToSnapshot);

billHandle_t *dds3BillboardCreate(s32 index, addr_t data) {
    billHandle_t *newobj;

    newobj = dds3BillboardFuncTable[index].create(data);
    dds3Billboard_BasicInit(newobj);
    newobj->type = index;
    newobj->callback = dds3BillboardFuncTable[index].draw;
    return newobj;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5010);

billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard) {
    billHandle_t *copy;
    dds3BillResource_tag *data;
    u16 sourceKind;
    void (*sourceCallback)(struct billHandle_t *);

    if (hBillboard->type == 1) {
        copy = dds3Billboard_P2ACopy(hBillboard);
        dds3Billboard_BasicInit(copy);
        copy->type = hBillboard->type;
        copy->callback = hBillboard->callback;
    } else {
        copy = dds3Billboard_NormalCreate(NULL);
        dds3Billboard_BasicInit(copy);
        sourceKind = hBillboard->type;
        data = hBillboard->u.child;
        sourceCallback = hBillboard->callback;
        copy->type = sourceKind;
        copy->callback = sourceCallback;
        data->count = data->count + 1;
        copy->u.child = data;
    }
    return copy;
}

void dds3BillboardDelete(billHandle_t *hData) {
    dds3BillboardFuncTable[hData->type].destroy(hData);
}

void dds3BillboardDraw(billHandle_t *hData) {
    hData->callback(hData);
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5160);

u32 func_001F51C8(void) {
    return 0x15;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number) {
    return hGeneralBillboard[number]->u.child->hTex;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F51F0);

void func_001F5200(billHandle_t *hBillboard, float rot) {
    hBillboard->rot = rot;
}

void func_001F5208(billHandle_t *hBillboard, float x, float y) {
    hBillboard->childScaleX = x;
    hBillboard->childScaleY = y;
}

void func_001F5218(billHandle_t *hBillboard, u32 abgr) {
    hBillboard->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5220);

void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode) {
    mode = (s16)mode;
    switch (hBillboard->type) {
    case 0:
    case 3:
        hBillboard->mode = mode;
        break;
    case 1: {
        s32 entryCount = hBillboard->entryCount;
        if (entryCount > 0) {
            s32 remaining = entryCount;
            dds3BillAnim_tag *entries = hBillboard->resolvedEntries;
            s32 index = 0;
            do {
                P2A_ANIMATION_tag *entry = entries[index].pAnim;
                u32 flags = entry->mode & ~BILL_ANIMATION_FLAG_PACKET_LIST_MASK;
                entry->mode = flags;
                if (mode == 2) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_2;
                } else if (mode == 3) {
                    entry->mode = flags | BILL_ANIMATION_FLAG_PACKET_LIST_3;
                }
                index++;
            } while (--remaining != 0);
        }
        break;
    }
    }
}

/* Store the variant bits in one child payload or every child of a list. */
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition) {
    s32 remainingChildren;
    dds3BillResource_tag **childEntries;
    dds3BillResource_tag **childCursor;

    condition = (s16)condition;
    switch (hBillboard->type) {
    case 0:
        hBillboard->u.child->type = (u16)condition;
        break;
    case 1:
        remainingChildren = hBillboard->u.animationData->TexNum;
        childEntries = hBillboard->u.animationData->pResList;

        if (remainingChildren > 0) {
            childCursor = childEntries;
            do {
                (*childCursor)->type = (u16)condition;
                childCursor++;
                remainingChildren--;
            } while (remainingChildren != 0);
        }
        break;
    }
}

saddr_t dds3BillboardGetTex(billHandle_t *hBillboard) {
    if (hBillboard->type == 0) {
        return EE_ADDR_S(hBillboard->u.child->hTex);
    }
    return 0;
}

u16 func_001F5398(billHandle_t *hBillboard) {
    return hBillboard->type;
}

void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type) {
    s32 maskedValue = type & BILL_VARIANT_MASK;

    switch (hBillboard->type) {
    case 0:
        hBillboard->u.child->type = (u16)maskedValue;
        break;
    case 1:
        hBillboard->pair.cameraFacingMode = maskedValue;
        break;
    }
}

u16 func_001F53D8(billHandle_t *hBillboard) {
    switch (hBillboard->type) {
    case 0:
        return hBillboard->u.child->type;
    case 1:
        return hBillboard->pair.cameraFacingMode;
    }
    return 0;
}

void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number) {
    if (hBillboard->type == 1 && hBillboard->animationEntryIndex != number) {
        dds3Billboard_P2ASetNumber(hBillboard, number);
    }
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5448);

INCLUDE_ASM(const s32, "effect/billManager", func_001F5468);

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame) {
    if (hBillboard->type == 1) {
        s32 entryCount = hBillboard->entryCount;

        if (entryCount > 0) {
            dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
            s32 remaining = entryCount;

            do {
                P2A_ANIMATION_tag *frameData = entry->pAnim;
                u32 wrappedFrame = frame % frameData->times;
                remaining -= 1;
                entry->wait = 0;
                entry->key = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

void dds3BillboardSetAnimKey2(billHandle_t *hBillboard, u32 frame) {
    if (hBillboard->type == 1) {
        s32 entryCount = hBillboard->entryCount;

        if (entryCount > 0) {
            dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
            s32 remaining = entryCount;

            do {
                P2A_ANIMATION_tag *frameData = entry->pAnim;
                u32 wrappedFrame = frame % frameData->times;
                remaining -= 1;
                entry->wait = 1;
                entry->key = wrappedFrame;
                entry++;
            } while (remaining != 0);
        }
    }
}

/* Read the animation modulus of the first entry, if this is a list billboard. */
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        const dds3BillAnim_tag *entry = hBillboard->resolvedEntries;
        const P2A_ANIMATION_tag *frame = entry->pAnim;
        return frame->times;
    }
    return 0;
}

u16 dds3BillboardGetAnimState(const billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->animationActive;
    }
    return 0;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5598);

void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        hBillboard->modeFlags |= 0x1000000;
    }
}

void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y) {
    if (hBillboard->type == 0) {
        dds3BillResource_tag *child = hBillboard->u.child;
        child->width = x * 0.5f;
        child->height = y * 0.5f;
    }
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5610);

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec) {
    f32 dot;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(pVec);
    SDF_VU0_INNERPRODUCT(dot);
    dot = sdfFabs(dot);
    return 1.0f - dot;
}

/* Create the indexed billboard and render asset; only the second matrix is initialized. */
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCreate(1, entryIndex);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    SDF_VU0_STOREIDENTITY(instance->rot);
    return instance;
}

/* Retain a cloned billboard's shared data, create a fresh render asset, and reset both matrices. */
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source) {
    dds3P2APolygon_tag *instance = sdfChipAllocDirect(sizeof(dds3P2APolygon_tag));

    instance->hBillboard = dds3BillboardCopy(
        source->hBillboard);
    instance->pMat = sdfMatNew();
    sdfMatSetConstant(instance->pMat, 1.0f);
    sdfMatSetDiffuse(instance->pMat, 0x80808080);
    sdfMatSetAmbient(instance->pMat, 0x80808080);
    sdfMatSetSpecular(instance->pMat, 0x80808080);
    SDF_VU0_STOREIDENTITY(instance->rot);
    SDF_VU0_STOREIDENTITY(instance->dir);
    return instance;
}

void dds3P2APolygonDelete(dds3P2APolygon_tag *hData) {
    sdfMatFreeDelayed((sdfMat_t *)hData->pMat);
    dds3BillboardDelete(hData->hBillboard);
    sdfChipFreeDirect(hData);
}

void func_001F5960(dds3P2APolygon_tag *hData, s128 *pPos) {
    func_001F51F0(hData->hBillboard, pPos);
}

void dds3P2APolygonSetScale(s32 hData, float scale) {
    dds3BillboardSetScale((EE_ADDR_AS(dds3P2APolygon_tag *, hData))->hBillboard, scale, scale);
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F5998);

void func_001F59C8(dds3P2APolygon_tag *hData, u32 abgr) {
    func_001F5218(hData->hBillboard, abgr);
}

INCLUDE_ASM(const s32, "effect/billManager", func_001F59E0);

void dds3P2APolygonGetResource(dds3P2APolygon_tag *hP2APoly, dds3P2APolygonResource_t *pP2APolyRes) {
    billHandle_t *billboard = hP2APoly->hBillboard;

    if (billboard->type == 1) {
        u32 modeFlags = billboard->modeFlags;

        if (modeFlags & 0x40) {
            pP2APolyRes->mode = 2;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else if (modeFlags & 0x80) {
            pP2APolyRes->mode = 3;
            pP2APolyRes->secondary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries + 1);
        } else {
            pP2APolyRes->mode = 0;
            pP2APolyRes->primary = dds3Billboard_P2AAnimDraw(billboard, billboard->resolvedEntries);
        }
    }
}

void dds3P2APolygonDraw(dds3P2APolygon_tag *instance) {
    sdf3DPrimPolyParm_t params;
    dds3P2APolygonResource_t resources;
    /* VU0 scratch is reloaded after its translation row is replaced. */
    volatile f32 matrix[4][4] __attribute__((aligned(16)));
    sdfVector4_t positions[4];
    f32 uv[4][2];
    f32 secondaryUv[4][2];
    sdfDrawTag_t *list;
    dds3BillResource_tag *child;
    sdfTexHandle_t *texture;
    sdfGrOt_t *surface;
    f32 x, y;
    f32 scaleX, scaleY;
    f32 cornerX, cornerY;
    f32 cosine, sine;
    f32 width, height;

    if (!dds3BillboardGetAnimState(instance->hBillboard)) {
        return;
    }
    dds3P2APolygonGetResource(instance, &resources);
    list = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    SDF_VU0_LOADMATRIX(instance->rot);
    SDF_VU0_LOADMATRIX_B(instance->dir);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX_NOMEM(matrix);
    matrix[3][0] = instance->hBillboard->pos[0];
    matrix[3][1] = instance->hBillboard->pos[1];
    matrix[3][2] = instance->hBillboard->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    memset(&params, 0, sizeof(params));
    params.attr = 0x4000;
    params.faceIndex = dds3P2APolygonFaceIndex;
    params.vertex = positions;
    params.vertexColor = dds3P2APolygonVertexColor;
    params.uv = (sdfVector2_t *)uv;
    params.color = instance->hBillboard->abgr;
    params.faceCount = 2;
    params.pointCount = 4;
    child = resources.primary;
    scaleX = instance->hBillboard->childScaleX;
    scaleY = instance->hBillboard->childScaleY;
    x = child->cx * scaleX;
    y = child->cy * scaleY;
    width = child->width * scaleX;
    height = child->height * scaleY;
    cosine = sdfCos(instance->hBillboard->rot);
    sine = sdfSin(instance->hBillboard->rot);
    cornerX = x - width;
    cornerY = y - height;
    positions[0].x = cornerX * cosine - cornerY * sine;
    positions[0].y = cornerX * sine + cornerY * cosine;
    positions[0].z = 0.0f;
    cornerX = x + width;
    cornerY = y - height;
    positions[1].x = cornerX * cosine - cornerY * sine;
    positions[1].y = cornerX * sine + cornerY * cosine;
    positions[1].z = 0.0f;
    cornerX = x + width;
    cornerY = y + height;
    positions[3].x = cornerX * cosine - cornerY * sine;
    positions[3].y = cornerX * sine + cornerY * cosine;
    positions[3].z = 0.0f;
    cornerX = x - width;
    cornerY = y + height;
    positions[2].x = cornerX * cosine - cornerY * sine;
    positions[2].y = cornerX * sine + cornerY * cosine;
    positions[2].z = 0.0f;
    width = 1.0f / (f32)(child->hTex->width * 16);
    height = 1.0f / (f32)(child->hTex->height * 16);
    texture = child->hTex;
    uv[0][0] = child->uv.components[0] * width;
    uv[0][1] = child->uv.components[1] * height;
    uv[1][0] = child->uv.components[2] * width;
    uv[1][1] = child->uv.components[3] * height;
    uv[2][0] = child->uv.components[6] * width;
    uv[2][1] = child->uv.components[7] * height;
    uv[3][0] = child->uv.components[4] * width;
    uv[3][1] = child->uv.components[5] * height;
    sdfMatSetTex(instance->pMat, texture);
    switch ((u32)resources.mode) {
    case 2:
    case 3:
        child = resources.secondary;
        scaleX = instance->hBillboard->childScaleX;
        scaleY = instance->hBillboard->childScaleY;
        x = child->cx * scaleX;
        y = child->cy * scaleY;
        width = child->width * scaleX;
        height = child->height * scaleY;
        params.attr |= 0x1000;
        params.multiUv = (sdfVector2_t *)secondaryUv;
        cosine = sdfCos(instance->hBillboard->rot);
        sine = sdfSin(instance->hBillboard->rot);
        cornerX = x - width;
        cornerY = y - height;
        positions[0].x = cornerX * cosine - cornerY * sine;
        positions[0].y = cornerX * sine + cornerY * cosine;
        positions[0].z = 0.0f;
        cornerX = x + width;
        cornerY = y - height;
        positions[1].x = cornerX * cosine - cornerY * sine;
        positions[1].y = cornerX * sine + cornerY * cosine;
        positions[1].z = 0.0f;
        cornerX = x + width;
        cornerY = y + height;
        positions[3].x = cornerX * cosine - cornerY * sine;
        positions[3].y = cornerX * sine + cornerY * cosine;
        positions[3].z = 0.0f;
        cornerX = x - width;
        cornerY = y + height;
        positions[2].x = cornerX * cosine - cornerY * sine;
        positions[2].y = cornerX * sine + cornerY * cosine;
        positions[2].z = 0.0f;
        texture = child->hTex;
        width = (f32)(texture->width * 16);
        height = (f32)(texture->height * 16);
        secondaryUv[0][0] = child->uv.components[0] / width;
        secondaryUv[0][1] = child->uv.components[1] / height;
        secondaryUv[1][0] = child->uv.components[2] / width;
        secondaryUv[1][1] = child->uv.components[3] / height;
        secondaryUv[2][0] = child->uv.components[6] / width;
        secondaryUv[2][1] = child->uv.components[7] / height;
        secondaryUv[3][0] = child->uv.components[4] / width;
        secondaryUv[3][1] = child->uv.components[5] / height;
        sdfMatSetMultiTex(instance->pMat, texture);
        break;
    }
    sdf3DPrimLinkMat(list, instance->pMat, 0);
    sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(&params)));
    surface = dds3P2APolygonSurfaceTable[instance->hBillboard->mode];
    surface->regFunc(surface, list);
}
