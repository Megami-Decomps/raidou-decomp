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

typedef struct effBlurDrawBank_t {
    u8 pad0000[0x1300];
    u8 frameMaskSource[0x8E0];
} effBlurDrawBank_t;

extern f32 sdfCos(f32 aRadius);
extern f32 sdfSin(f32 aRadius);
extern f64 fabs(f64 value);
extern void *sdfGrAllocPacketBuff(s32 aSize);
extern void sdfDrawTagInit(sdfDrawTag_t *a);
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
void *sdfChipAllocDirect(s32 aSize);
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
extern void *memcpy(void *a, const void *b, size_t c);
void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);
sdfMat_t *sdfMatNew(void);

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *aMat);
void sdfChipFreeDirect(void *aPtr);
void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);
void sdfMatSetAmbient(struct sdfMat_t *aMat, u32 aColor);
void sdfMatSetDiffuse(struct sdfMat_t *aMat, u32 aColor);
void sdfMatSetSpecular(struct sdfMat_t *aMat, u32 aColor);
extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];
extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];
billHandle_t *dds3Billboard_P2ACreate(void *resourceData);
billHandle_t *dds3Billboard_P2ACopy(const billHandle_t *pCopyP2A);
void dds3BillboardDraw(billHandle_t *hData);
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetCondition(billHandle_t *hBillboard, s32 condition);
u16 dds3BillboardGetType(billHandle_t *hBillboard);
u16 dds3BillboardGetDrawType(billHandle_t *hBillboard);
s32 dds3BillboardGetAnimFrame(const billHandle_t *hBillboard);
f32 dds3BillboardGetDirectionScale(void *pPos, f32 *pVec);
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
extern void MultMatrixNoArg(void);
extern void sdf3DPrimLinkTm(sdfDrawTag_t *, s32 (*)(s32));
extern void sdf3DPrimLinkMat(sdfDrawTag_t *, sdfMat_t *, s32 (*)(s32));
extern void sdfMatSetTex(sdfMat_t *, sdfTexHandle_t *);
extern void sdfMatSetMultiTex(sdfMat_t *, sdfTexHandle_t *);
extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);
void dds3P2APolygonDraw(dds3P2APolygon_tag *instance);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/eff.h"
#include "dds/ee_mmi.h"

typedef struct DmaPacketHeader DmaPacketHeader;

enum {
    BILL_ANIMATION_ENTRY_MODE_MTEX = 0x00000040,
    BILL_ANIMATION_ENTRY_MODE_AMTEX = 0x00000080,
    BILL_ANIMATION_ENTRY_MODE_TWO_ENTRY_MASK = 0x000000C0,
    BILL_ANIMATION_ENTRY_MODE_P2A_POLYGON_LOG = 0x00000100
};

void billCopyCurrentRecordToSnapshot(billHandle_t *obj, BillSnapshot *snapshot);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardManagerExit);

INCLUDE_ASM(const s32, "effect/billManager", BillboardLoadTex);

INCLUDE_ASM(const s32, "effect/billManager", FreeTex);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardLinkOt);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardTagLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3MultiBillboardPktLink);

INCLUDE_ASM(const s32, "effect/billManager", dds3CallMultiBillboardPktLink);

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

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardNormalCreate);

INCLUDE_ASM(const s32, "effect/billManager", NormalDelete);

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

INCLUDE_ASM(const s32, "effect/billManager", P2ADelete);

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

INCLUDE_ASM(const s32, "effect/billManager", nbPanelFlashRGBAMulti);

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

INCLUDE_ASM(const s32, "effect/billManager", AnimLoadP2A);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillAnimFreeP2A);

/* Copy the billboard's current source record (by kind) into a snapshot. */
void billCopyCurrentRecordToSnapshot(billHandle_t *obj, BillSnapshot *snapshot) {
    BillChildPayload *record;

    if (obj->type == 1) {
        record = dds3Billboard_P2AAnimDraw(obj, obj->resolvedEntries);
    } else if (obj->type == 0 || obj->type == 3) {
        record = obj->u.child;
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

    newobj = dds3BillboardFuncTable[index].create(data);
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

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGeneral_dds3Billboard);

u32 dds3Leaf30(void) {
    return 0x15;
}

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number) {
    return hGeneralBillboard[number]->u.child->hTex;
}

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardSetPosition);

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

INCLUDE_ASM(const s32, "effect/billManager", dds3Leaf31);

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

INCLUDE_ASM(const s32, "effect/billManager", dds3Leaf32);

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetAnimNumber);

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

INCLUDE_ASM(const s32, "effect/billManager", dds3Leaf33);

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

INCLUDE_ASM(const s32, "effect/billManager", dds3BillboardGetDirectionRotate);

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

void dds3P2APolygonSetPosition(dds3P2APolygon_tag *hData, s128 *pPos) {
    dds3BillboardSetPosition(hData->hBillboard, pPos);
}

void dds3P2APolygonSetScale(s32 hData, float scale) {
    dds3BillboardSetScale((EE_ADDR_AS(dds3P2APolygon_tag *, hData))->hBillboard, scale, scale);
}

INCLUDE_ASM(const s32, "effect/billManager", dds3Leaf34);

void dds3P2APolygonSetRGBA(dds3P2APolygon_tag *hData, u32 abgr) {
    dds3PCPSparkleSphereDrawSub(hData->hBillboard, abgr);
}

INCLUDE_ASM(const s32, "effect/billManager", dds3Leaf35);

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
