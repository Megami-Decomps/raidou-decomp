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
#include "sdfChip.h"
#include "sdfVu0.h"
#include "sdfDraw.h"
#include "kernel_draw.h"
#include "dds3KernelMain.h"
#include "sdfMat.h"
#include "billManager.h"
#include "sdfMath.h"
#include "effPacket.h"
#include "sdf3DPrim.h"
#include "sdfPacketBuff.h"
#include "sdf3D.h"
#include "sdfBillBoard.h"
#include "sdfMemory.h"
#include "sdfTex.h"

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

#include "sdk_libc.h"

extern f64 fabs(f64 value);
struct billHandle_t *dds3Billboard_NormalCreate(void *data);

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
void dds3BillboardSetScale(struct billHandle_t *hBillboard, f32 x, f32 y);

typedef struct P2A_HEAD_t {
    s32 tmxofs;
    s32 AnimNum;
} P2A_HEAD_t;

#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4

/* Duplicate a billboard object: an entry list is cloned, a child shares (and refs) the source's data block. */
billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard);
billHandle_t *dds3BillboardCreate(s32 index, addr_t data);
void dds3BillboardDelete(billHandle_t *hData);

/* Select an animation and initialize the plural records' signed start delays. */
void dds3Billboard_P2ASetNumber(billHandle_t *pP2A, s32 number);

/* Advance an entry's frame timer and apply its current record to the shared
 * child payload. Plural descriptors are handled by the dispatcher instead. */
dds3BillResource_tag *dds3Billboard_P2AAnimDraw(billHandle_t *pP2A, dds3BillAnim_tag *pBillAnim);

/* Queue an asset owner for the deferred SdfAsset release callback. */
extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];
extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];
billHandle_t *dds3Billboard_P2ACreate(void *resourceData);
billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A);
void dds3BillboardDraw(billHandle_t *hData);
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition);
u16 dds3BillboardGetType(billHandle_t *hBillboard);
u16 dds3BillboardGetDrawType(billHandle_t *hBillboard);
dds3P2APolygon_tag *dds3P2APolygonCreate(s32 entryIndex);
dds3P2APolygon_tag *dds3P2APolygonCopy(const dds3P2APolygon_tag *source);
void dds3P2APolygonDelete(dds3P2APolygon_tag *hData);
void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos);
void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr);
void dds3Billboard_BasicInit(billHandle_t *pBasic);
billHandle_t *dds3Billboard_NormalCreate(void *data);
void dds3Billboard_NormalDraw(billHandle_t *hData);
void dds3Billboard_P2ASetAnim(dds3BillAnimResource_tag *hBillAnimRes, s32 number, dds3BillAnim_tag *pBillAnim);
sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

/* Set the two child scale components stored at +0x10 and +0x14. */
void dds3BillboardSetScale(billHandle_t *hBillboard, float x, float y);
void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode);
saddr_t dds3BillboardGetTex(billHandle_t *hBillboard);
void dds3BillboardSetDrawType(billHandle_t *hBillboard, s32 type);
void dds3BillboardSetAnimNumber(billHandle_t *hBillboard, u32 number);
void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);
void dds3BillboardSetAnimKey2(billHandle_t *hBillboard, u32 frame);
u16 dds3BillboardGetAnimState(const billHandle_t *hBillboard);
void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard);
void dds3BillboardSetXYOffset(billHandle_t *hBillboard, float x, float y);
void dds3P2APolygonSetScale(s32 hData, float scale);

typedef struct {
    s32 mode;
    dds3BillResource_tag *primary;
    dds3BillResource_tag *secondary;
} dds3P2APolygonResource_t;

void dds3P2APolygonGetResource(dds3P2APolygon_tag *hP2APoly, dds3P2APolygonResource_t *pP2APolyRes);
extern u32 dds3P2APolygonFaceIndex[];
extern u32 dds3P2APolygonVertexColor[];
extern sdfGrOt_t *dds3P2APolygonSurfaceTable[];
void dds3P2APolygonDraw(dds3P2APolygon_tag *instance);

#include "include_asm.h"
#include "sdf_resource.h"
#include "sdf_types.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "code_r1_001DED50.h"

/* Current billboard draw record copied independently of its shared child. */
typedef struct billSnapshot_t {
    f32 x;
    f32 y;
    f32 halfWidth;
    f32 halfHeight;
    sdfTexHandle_t *texture;
    billTextureQuad_t uv;
} billSnapshot_t;

typedef char billSnapshot_size_must_be_0x24[
    (sizeof(billSnapshot_t) == 0x24) ? 1 : -1];

/* Child manager at retained allocation +0x400; shared by resource and draw owners. */
typedef struct billChildPayload_t {
    sdfTexHandle_t *texture; /* 0x00: acquired texture reference. */
    u16 variant; /* Child variant bits; signed inputs narrow on assignment. */
    u8 pad06[2];
    s32 refCount;      /* 0x08 */
    billTextureQuad_t uv; /* 0x0C */
    f32 x;             /* 0x1C */
    f32 y;             /* 0x20 */
    f32 halfWidth;  /* 0x24 */
    f32 halfHeight; /* 0x28 */
    sdfDrawTag_t *pendingLists[5]; /* 0x2C */
    u16 packetListIndex; /* 0x40 */
    u8 pad42[2];
    struct sdfMemHandle_t *allocation; /* 0x44: allocation descriptor released with this child. */
    sdfBillBoardBatch_t *work; /* 0x48: retained packet-buffer address */
    struct billChildPayload_t *next; /* 0x4C */
} billChildPayload_t;

typedef char billChildPayload_size_must_be_0x50[
    (sizeof(billChildPayload_t) == 0x50) ? 1 : -1];


enum {
    BILL_ANIMATION_ENTRY_MODE_MTEX = 0x00000040,
    BILL_ANIMATION_ENTRY_MODE_AMTEX = 0x00000080,
    BILL_ANIMATION_ENTRY_MODE_TWO_ENTRY_MASK = 0x000000C0,
    BILL_ANIMATION_ENTRY_MODE_P2A_POLYGON_LOG = 0x00000100
};

void billCopyCurrentRecordToSnapshot(billHandle_t *obj, billSnapshot_t *snapshot);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/billManager.c:300, near, same function 1.00 (words 21/21, calls 1.00, shape 1.00) */
void dds3BillboardManagerExit(void) {
    u32 i;

    dds3BillboardTag = NULL;
    for (i = 0; i < 21; i++) {
        dds3BillboardDelete(hGeneralBillboard[i]);
    }
}

dds3BillResource_tag *BillboardLoadTex(void *pTex) {
    sdfMemHandle_t *allocation;
    sdfBillBoardBatch_t *batch;
    dds3BillResource_tag *child;
    sdfTexHandle_t *tex;
    s32 uvWidth;
    s32 uvHeight;
    s32 i;

    allocation = sdfMemAllocDirect(sizeof(sdfBillBoardBatch_t) + sizeof(dds3BillResource_tag));
    batch = sdfMemLock(allocation);
    child = (dds3BillResource_tag *)(batch + 1);
    child->count = 0;
    child->hMem = allocation;
    child->batch = batch;
    batch->count = 0;
    child->next = child;
    child->type = 0;
    for (i = 4; i >= 0; i--) {
        child->pBillTag[i] = NULL;
    }
    child->current_mode = 0;
    tex = sdfTexSet(pTex);
    child->count = child->count + 1;
    uvHeight = (s16)((u16)tex->height << 4);
    uvWidth = (s16)((u16)tex->width << 4);
    child->uv.components[0] = 0;
    child->uv.components[1] = 0;
    child->uv.components[2] = uvWidth;
    child->uv.components[3] = 0;
    child->uv.components[4] = uvWidth;
    child->uv.components[5] = uvHeight;
    child->uv.components[6] = 0;
    child->uv.components[7] = uvHeight;
    child->hTex = tex;
    child->width = (uvWidth >> 5) * 0.0625f;
    child->height = (uvHeight >> 5) * 0.0625f;
    child->cx = 0.0f;
    child->cy = 0.0f;
    return child;
}

void BillboardFreeTex(dds3BillResource_tag *hResource) {
    s32 refCount = hResource->count - 1;

    hResource->count = refCount;
    if (refCount == 0) {
        sdfTexFreeDelayed(hResource->hTex);
        sdfMemFreeDirect(hResource->hMem);
    }
}

void dds3BillboardTagLink(billHandle_t *obj, dds3BillResource_tag *child) {
    f32 matrix[16] __attribute__((aligned(16)));
    sdfVector4_t direction;
    f32 dot;
    sdfBillBoardBatch_t *work;
    s32 selected;
    void *packet;
    u32 *vertexPacket;
    u8 *geometry;
    s32 index;
    f32 x, y, halfWidth, halfHeight;
    f32 cosine, sine;
    f32 cornerX, cornerY;

    selected = child->current_mode;
    if (selected != obj->mode) {
        work = child->batch;
        if (work->count > 0) {
            vertexPacket = sdfBillBoardMakeN((const u128 *)work->centre, work->color,
                work->uv, work->offset, work->count, 0);
            sdfDrawTagLink(child->pBillTag[selected], EE_ADDR(vertexPacket));
            work->count = 0;
        }
        selected = obj->mode;
        child->current_mode = selected;
    }
    if (child->pBillTag[selected] == NULL) {
        child->pBillTag[selected] = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
        sdfDrawTagInit(child->pBillTag[selected]);
        packet = EE_ADDR_AS(void *, sdfGrAllocPacketBuff(0x20));
        sdf3DMakeDirectRefPacket(packet,
            EE_ADDR(sdfTexGetPacket(child->hTex)),
            sdfTexGetPacketLength(child->hTex));
        sdfDrawTagLinkRef(child->pBillTag[selected], EE_ADDR(packet));
        if ((u16)(child->type & 1) != 0) {
            SDF_VU0_LOADVECTOR(aViewTarget);
            SDF_VU0_LOADVECTOR_B(aViewEye);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_NORMALIZE();
            SDF_VU0_LOADVECTOR_B(dds3BillboardUpAxis);
            SDF_VU0_INNERPRODUCT(dot);
            dds3BillboardTagParam[1] = 1.0 - fabs(dot);
            SDF_VU0_STOREVECTOR(&direction);
            SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR_BARRIER(dds3BillboardTagParam);
            SDF_VU0_SCALE_XYZ();
            SDF_VU0_STOREMATRIX(matrix);
        } else {
            SDF_VU0_STOREIDENTITY(matrix);
        }
        geometry = EE_ADDR_AS(u8 *, sdfGrAllocPacketBuff(0x38));
        sdfBillBoardMakeTMPacket(geometry, matrix);
        sdfDrawTagLink(child->pBillTag[selected], EE_ADDR(geometry));
        if (child->next == child) {
            child->next = dds3BillboardTag;
            dds3BillboardTag = child;
        }
    }
    work = child->batch;
    index = work->count;
    SDF_VU0_COPYVECTOR(&work->centre[index], obj->pos);
    work->color[index] = obj->abgr;
    memcpy(&work->uv[index], &child->uv, sizeof(child->uv));
    x = child->cx * obj->childScaleX;
    y = child->cy * obj->childScaleY;
    halfWidth = child->width * obj->childScaleX;
    halfHeight = child->height * obj->childScaleY;
    if (obj->rot == 0.0f) {
        work->offset[index][0] = x - halfWidth;
        work->offset[index][1] = y - halfHeight;
        work->offset[index][2] = x + halfWidth;
        work->offset[index][3] = y - halfHeight;
        work->offset[index][4] = x + halfWidth;
        work->offset[index][5] = y + halfHeight;
        work->offset[index][6] = x - halfWidth;
        work->offset[index][7] = y + halfHeight;
    } else {
        cosine = sdfCos(obj->rot);
        sine = sdfSin(obj->rot);
        cornerX = x - halfWidth;
        cornerY = y - halfHeight;
        work->offset[index][0] = cornerX * cosine - cornerY * sine;
        work->offset[index][1] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y - halfHeight;
        work->offset[index][2] = cornerX * cosine - cornerY * sine;
        work->offset[index][3] = cornerX * sine + cornerY * cosine;
        cornerX = x + halfWidth;
        cornerY = y + halfHeight;
        work->offset[index][4] = cornerX * cosine - cornerY * sine;
        work->offset[index][5] = cornerX * sine + cornerY * cosine;
        cornerX = x - halfWidth;
        cornerY = y + halfHeight;
        work->offset[index][6] = cornerX * cosine - cornerY * sine;
        work->offset[index][7] = cornerX * sine + cornerY * cosine;
    }
    work->count++;
    if (work->count == 15) {
        vertexPacket = sdfBillBoardMakeN((const u128 *)work->centre, work->color,
            work->uv, work->offset, 15, 0);
        sdfDrawTagLink(child->pBillTag[selected], EE_ADDR(vertexPacket));
        work->count = 0;
    }
}

void dds3BillboardLinkOt(void) {
    dds3BillResource_tag *res = dds3BillboardTag;

    if (res != NULL) {
        do {
            sdfBillBoardBatch_t *batch = res->batch;
            s32 count = batch->count;
            dds3BillResource_tag *next;
            s32 i;

            if (count > 0) {
                u32 *packet = sdfBillBoardMakeN((const u128 *)batch->centre, batch->color, batch->uv,
                                                batch->offset, count, NULL);
                sdfDrawTagLink(res->pBillTag[res->current_mode], EE_ADDR(packet));
                batch->count = 0;
            }
            for (i = 0; i < 5; i++) {
                sdfDrawTag_t *tag = res->pBillTag[i];
                if (tag != NULL) {
                    dds3BillboardOtTable[i]->regFunc(dds3BillboardOtTable[i], tag);
                    res->pBillTag[i] = NULL;
                }
            }
            next = res->next;
            res->next = res;
            res = next;
        } while (res != NULL);
    }
    dds3BillboardTag = NULL;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);

void dds3MultiBillboardPktLink(billRenderPair_t *hResource) {
    u32 colors[8][2];
    u16 uv[7][16];
    sdfBillBoardBatch_t *batch[2];
    s32 count;
    s32 i;
    u32 *packet;

    batch[0] = hResource->children[0]->batch;
    batch[1] = hResource->children[1]->batch;
    count = batch[1]->count;
    for (i = 0; i < count; i++) {
        colors[i][0] = batch[0]->color[i];
        colors[i][1] = batch[1]->color[i];
        uv[i][0] = batch[0]->uv[i][0].u;
        uv[i][1] = batch[0]->uv[i][0].v;
        uv[i][4] = batch[0]->uv[i][1].u;
        uv[i][5] = batch[0]->uv[i][1].v;
        uv[i][8] = batch[0]->uv[i][2].u;
        uv[i][9] = batch[0]->uv[i][2].v;
        uv[i][12] = batch[0]->uv[i][3].u;
        uv[i][13] = batch[0]->uv[i][3].v;
        uv[i][2] = batch[1]->uv[i][0].u;
        uv[i][3] = batch[1]->uv[i][0].v;
        uv[i][6] = batch[1]->uv[i][1].u;
        uv[i][7] = batch[1]->uv[i][1].v;
        uv[i][10] = batch[1]->uv[i][2].u;
        uv[i][11] = batch[1]->uv[i][2].v;
        uv[i][14] = batch[1]->uv[i][3].u;
        uv[i][15] = batch[1]->uv[i][3].v;
    }
    packet = sdfMultiBillBoardMakeN((u128 *)batch[1], colors, uv, batch[1]->offset, count, NULL);
    sdfDrawTagLink(hResource->packetList, EE_ADDR(packet));
    batch[0]->count = 0;
    batch[1]->count = 0;
}

void dds3MultiBillboardLinkOt(void) {
    billRenderPair_t *pair = dds3MultiBillboardPairList;
    sdfDrawTag_t *tag;
    u64 *packet;
    sdfGrRefGifPacketHeader_t *buffer;
    u32 frame;

    if (pair != NULL) {
        do {
            if (pair->children[1]->batch->count != 0) {
                dds3MultiBillboardPktLink(pair);
            }
            dds3MultiBillboardOt.regFunc(&dds3MultiBillboardOt, pair->packetList);
            pair->packetList = NULL;
            pair = pair->next;
        } while (pair != NULL);
    }
    tag = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(tag);
    buffer = EE_ADDR_AS(sdfGrRefGifPacketHeader_t *, sdfGrAllocPacketBuff(0x40));
    frame = dds3GetFrameID();
    sdfGrSetFrameMaskReset((const u8 *)&dds3DrawEnv[frame].stdEnv, buffer, 0);
    sdfDrawTagLinkRefGif(tag, EE_ADDR(buffer));
    packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(0x40));
    packet[0] = 3;
    packet[1] = 0x5000000310000000;
    packet[2] = 0x1000000000008002;
    packet[3] = 0xE;
    packet[4] = 0x71801;
    packet[5] = 0x47;
    packet[6] = 0x48;
    packet[7] = 0x42;
    sdfDrawTagLink(tag, EE_ADDR(packet));
    dds3MultiBillboardOt.regFunc(&dds3MultiBillboardOt, tag);
    dds3MultiBillboardPairList = NULL;
}

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

billHandle_t *dds3Billboard_NormalCreate(void *data) {
    billHandle_t *obj = sdfChipAllocDirect(0x34);

    obj->u.child = NULL;
    if (data != NULL) {
        obj->u.child = BillboardLoadTex(data);
    }
    return obj;
}

void dds3Billboard_NormalDelete(billHandle_t *hData) {
    if (hData->u.child != NULL) {
        BillboardFreeTex(hData->u.child);
    }
    sdfChipFreeDirect(hData);
}

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

void dds3Billboard_P2ADelete(billHandle_t *hData) {
    dds3BillAnimFreeP2A(hData->u.animationData);
    sdfChipFreeDirect(hData);
}

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

/* vu0 routine: multiplies the two colours channel by channel, scaled by 1/128. */
u32 dds3Billboard_P2AColorMul(u32 color0, u32 color1) {
    billColorWork_t work[3];
    u32 unit = 0x3C000000;
    u32 out;

    work[0].word = color0;
    MMI_RGBA_UNPACK(&work[0], unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    work[1].word = color1;
    MMI_RGBA_UNPACK(&work[1], unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_dds3ColorFtoI(out, 128.0f);
    return out;
}

void dds3Billboard_P2ADraw(billHandle_t *hData) {
    u32 modeFlags;
    u32 savedColor;
    f32 savedRotation;
    dds3BillResource_tag *child;

    if (hData->animationActive == 0) {
        return;
    }
    modeFlags = hData->modeFlags;
    if (modeFlags & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
        dds3BillAnimResource_tag *data = hData->u.animationData;
        P2A_ANIMATION_tag *entry = data->pAnimTable + hData->animationEntryIndex;
        billPluralEntry_t *records;
        u32 i;

        savedColor = hData->abgr;
        savedRotation = hData->rot;
        records = (billPluralEntry_t *)(data->pHeader + entry->spranm);
        for (i = 0; i < hData->entryCount; i++) {
            dds3BillAnim_tag *out = (dds3BillAnim_tag *)((u8 *)(i * sizeof(dds3BillAnim_tag)) + EE_ADDR(hData->resolvedEntries));
            s32 frameIndex = out->key;

            if (frameIndex >= 0) {
                f32 savedX;
                f32 savedY;

                child = dds3Billboard_P2AAnimDraw(hData, out);
                savedX = child->cx;
                savedY = child->cy;
                child->cx = records[i].x;
                child->cy = -records[i].y;
                hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
                hData->rot = hData->rot + savedRotation;
                dds3BillboardTagLink(hData, child);
                child->cx = savedX;
                child->cy = savedY;
            } else {
                out->key = frameIndex + 1;
            }
        }
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else if (modeFlags & 0x40) {
        savedColor = hData->abgr;
        savedRotation = hData->rot;
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries + 1);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else if (modeFlags & 0x80) {
        u32 flags;

        savedColor = hData->abgr;
        savedRotation = hData->rot;
        hData->pair.children[0] = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->pair.colors[0] = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->pair.children[1] = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries + 1);
        hData->pair.colors[1] = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        flags = hData->resolvedEntries[1].pAnim->mode;
        if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_2) {
            hData->pair.kind = 2;
        } else if (flags & BILL_ANIMATION_FLAG_PACKET_LIST_3) {
            hData->pair.kind = 3;
        } else {
            hData->pair.kind = 1;
        }
        hData->rot = hData->rot + savedRotation;
        dds3MultiBillboardTagLink(hData, &hData->pair);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    } else {
        savedColor = hData->abgr;
        savedRotation = hData->rot;
        child = dds3Billboard_P2AAnimDraw(hData, hData->resolvedEntries);
        hData->abgr = dds3Billboard_P2AColorMul(savedColor, hData->abgr);
        hData->rot = hData->rot + savedRotation;
        dds3BillboardTagLink(hData, child);
        hData->rot = savedRotation;
        hData->abgr = savedColor;
    }
}

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

dds3BillAnimResource_tag *dds3BillAnimLoadP2A(void *resourceData) {
    sdfMemHandle_t *allocation;
    dds3BillAnimResource_tag *block;
    P2A_HEAD_t *base;
    s32 *texOffsets;
    s32 childCount;
    s32 headerSize;
    s32 entryCount;
    s32 i;

    headerSize = ((P2A_HEAD_t *)resourceData)->tmxofs;
    texOffsets = (s32 *)((u8 *)resourceData + headerSize);
    childCount = *texOffsets;
    texOffsets++;
    allocation = sdfMemAllocDirect(headerSize + childCount * sizeof(dds3BillResource_tag) + sizeof(dds3BillAnimResource_tag));
    block = sdfMemLock(allocation);
    base = (P2A_HEAD_t *)(block + 1);
    block->hMem = allocation;
    memcpy(base, resourceData, ((P2A_HEAD_t *)resourceData)->tmxofs);
    block->pHeader = (u8 *)base;
    block->TexNum = childCount;
    block->pAnimTable = (P2A_ANIMATION_tag *)(base + 1);
    block->pResList = (dds3BillResource_tag **)((u8 *)(base + 1) + base->tmxofs);
    for (i = 0; i < childCount; i++) {
        block->pResList[i] = BillboardLoadTex((u8 *)resourceData + *texOffsets++);
    }
    block->count = 1;
    block->AnimNum = 1;
    entryCount = ((P2A_HEAD_t *)block->pHeader)->AnimNum;
    for (i = 0; i < entryCount; i++) {
        P2A_ANIMATION_tag *entry = &block->pAnimTable[i];

        if (entry->mode & BILL_ANIMATION_FLAG_PLURAL_ENTRIES) {
            u32 frameCount = entry->times;

            block->AnimNum = frameCount;
            printf("billAnim no[%d][%d]...PLURAL\n", i, frameCount);
        } else if (entry->attr & 0x40) {
            block->AnimNum = 2;
            printf("billAnim no[%d][%d]...MTEX\n", i, 2);
        } else if (entry->attr & 0x80) {
            block->AnimNum = 2;
            printf("billAnim no[%d][%d]...AMTEX\n", i, 2);
        }
    }
    return block;
}

void dds3BillAnimFreeP2A(dds3BillAnimResource_tag *hBillAnimRes) {
    s32 listRefCount = hBillAnimRes->count - 1;

    hBillAnimRes->count = listRefCount;
    if (listRefCount == 0) {
        s32 i = 0;

        for (; i < hBillAnimRes->TexNum; i++) {
            BillboardFreeTex(hBillAnimRes->pResList[i]);
        }
        sdfMemFreeDirect(hBillAnimRes->hMem);
    }
}

/* Copy the billboard's current source record (by kind) into a snapshot. */
void billCopyCurrentRecordToSnapshot(billHandle_t *obj, billSnapshot_t *snapshot) {
    billChildPayload_t *record;

    if (obj->type == 1) {
        record = (billChildPayload_t *)dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
    } else if (obj->type == 0 || obj->type == 3) {
        record = (billChildPayload_t *)obj->u.child;
    } else {
        return;
    }
    snapshot->x = record->x;
    snapshot->texture = record->texture;
    snapshot->y = record->y;
    snapshot->halfWidth = record->halfWidth;
    snapshot->halfHeight = record->halfHeight;
    memcpy(&snapshot->uv, &record->uv, sizeof(snapshot->uv));
}

billHandle_t *dds3BillboardCreate(s32 index, addr_t data) {
    billHandle_t *newobj;

    newobj = dds3BillboardFuncTable[index].create(EE_ADDR_AS(void *, data));
    dds3Billboard_BasicInit(newobj);
    newobj->type = index;
    newobj->callback = dds3BillboardFuncTable[index].draw;
    return newobj;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3CallDevLoadFileEx);

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

billHandle_t *dds3BillboardGeneral(s32 number) {
    billHandle_t *obj;
    dds3BillResource_tag *child;
    s32 refCount;

    obj = dds3BillboardCreate(BillboardTable[number].kind, 0);
    child = hGeneralBillboard[number]->u.child;
    refCount = child->count;
    obj->u.child = child;
    child->count = refCount + 1;
    return obj;
}

u32 dds3Leaf30(void) {
    return 0x15;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number) {
    return hGeneralBillboard[number]->u.child->hTex;
}

void dds3BillboardSetPosition(void *hBillboard, const void *pPos) {
    SDF_VU0_COPYVECTOR(hBillboard, pPos);
}

void dds3BillboardSetRotate_dds3Billboard(billHandle_t *hBillboard, float rot) {
    hBillboard->rot = rot;
}

void dds3BillboardSetScale_dds3Billboard(billHandle_t *hBillboard, float x, float y) {
    hBillboard->childScaleX = x;
    hBillboard->childScaleY = y;
}

void dds3PCPSparkleSphereDrawSub(billHandle_t *hBillboard, u32 abgr) {
    hBillboard->abgr = abgr;
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/billManager.c:1207, match, same function 1.00 (words 14/14, calls 1.00, shape 1.00) */
void dds3Leaf31(billHandle_t *effect, const billUvQuad_t *textureQuad) {
    if (effect->type == 0) {
        memcpy(&effect->u.child->uv, textureQuad, sizeof(*textureQuad));
    }
}

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

u16 dds3BillboardGetType(billHandle_t *hBillboard) {
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

u16 dds3BillboardGetDrawType(billHandle_t *hBillboard) {
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

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/billManager.c:1312, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
s32 dds3Leaf32(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->animationEntryIndex;
    }
    return 0;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/billManager.c:1319, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
s32 dds3BillboardGetAnimNumber(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return ((P2A_HEAD_t *)hBillboard->u.animationData->pHeader)->AnimNum;
    }
    return 0;
}

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

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/billManager.c:1312, near, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
s32 dds3Leaf33(billHandle_t *hBillboard) {
    if (hBillboard->type == 1) {
        return hBillboard->modeFlags;
    }
    return 0;
}

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

/* vu0 routine: projects pPos and pPos + pVec to the screen and returns the angle between them. */
f32 dds3BillboardGetDirectionRotate(const sdfVector4_t *pPos, const sdfVector4_t *pVec) {
    sdfVector4_t delta;
    sdfVector4_t origin;
    sdfVector4_t tip;

    SDF_VU0_LOADMATRIX(&sdfViewMatrix);
    MultMatrixRev(&dds3ScreenParam.TM);
    SDF_VU0_LOADVECTOR(pPos);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&origin);
    SDF_VU0_LOADVECTOR(pVec);
    SDF_VU0_LOADVECTOR_B(dds3BillboardGetDirectionRotateData3);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_C_TO_B();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_PERSVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&dds3ScreenParam.trans);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&tip);
    delta.x = origin.x - tip.x;
    delta.y = origin.y - tip.y;
    delta.z = 0.0f;
    SDF_VU0_LOADVECTOR_MEMOPS(delta.x);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_MEMOPS(delta.x);
    return sdfAtan2(delta.y, delta.x);
}

/* vu0 routine: 1 - |axis . normalized view direction|. */
f32 dds3BillboardGetDirectionScale(const sdfVector4_t *pPos, const sdfVector4_t *pVec) {
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

void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos) {
    dds3BillboardSetPosition(hData->hBillboard, pPos);
}

void dds3P2APolygonSetScale(s32 hData, float scale) {
    dds3BillboardSetScale((EE_ADDR_AS(dds3P2APolygon_tag *, hData))->hBillboard, scale, scale);
}

/* body: matched in smt3 @ 9401f79eb: src/smt3/effect/billManager.c:1499, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
/* vu0 routine: copy the 4x4 rotation matrix into the polygon. */
void dds3Leaf34(dds3P2APolygon_tag *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STOREMATRIX(hData->rot);
}

void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr) {
    dds3PCPSparkleSphereDrawSub(hData->hBillboard, abgr);
}

/* vu0 routine: copy the 4x4 direction matrix into the polygon. */
void dds3Leaf35(dds3P2APolygon_tag *hData, const void *pDir) {
    SDF_VU0_LOADMATRIX(pDir);
    SDF_VU0_STOREMATRIX(hData->dir);
}

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
    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    SDF_VU0_LOADMATRIX(instance->rot);
    SDF_VU0_LOADMATRIX_B(instance->dir);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX_NOMEM((f32 (*)[4])matrix);
    matrix[3][0] = instance->hBillboard->pos[0];
    matrix[3][1] = instance->hBillboard->pos[1];
    matrix[3][2] = instance->hBillboard->pos[2];
    SDF_VU0_LOADMATRIX((f32 (*)[4])matrix);
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
