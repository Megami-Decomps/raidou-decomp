#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effPCPNeedle.c */
#include "common.h"

#include "sdf_types.h"

#include "effPCPData.h"

#include "ps2asm.h"

#include "sdfFrame.h"

/* One needle instance: position plus the colour the draw loop stamps. */
typedef struct dds3NeedleModelPart_tag
{
    f32 position[3];
    u8 unk0C[4];
    u32 abgr;
} dds3NeedleModelPart_tag;

typedef struct dds3NeedleModel_tag
{
    f32 matrix[16];
    dds3NeedleModelPart_tag *pPart;
    u32 number;
    u32 blend;
    f32 scale[3];
    u32 point;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u32 *pVC;
    void *hVNMem;
    void *pMat;
    void *hMem;
} dds3NeedleModel_tag;

/* Argument block effPkt3DPrimMakePoly reads. */
typedef struct effPrimParam_t
{
    u16 primCount;
    u16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    void *parameters;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u8 unk18[8];
    u32 *colors;
    u8 unk24[8];
} effPrimParam_t;

typedef struct effDrawSurface_t
{
    u8 unk00[0x10];
    void (*append)(struct effDrawSurface_t *surface, void *packet);
} effDrawSurface_t;

extern effPrimParam_t dds3NeedleModelPrimParam;

extern f32 dds3NeedleModelDefaultPoints[][4];

extern u32 dds3NeedleModelDefaultColors[];

extern effDrawSurface_t *dds3NeedleModelDrawSurfaces[];

extern void *sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(void *a);

extern void sdfDrawTagLink(void *aTag, addr_t aPacket);

extern void sdf3DPrimLinkMat(void *aTag, void *aMat, s32 aAllocator);

extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);

extern void *effPkt3DPrimMakePoly(effPrimParam_t *aParm);

extern void MultMatrixNoArg(void);

extern void sdfMatFreeDelayed(void *aMat);

extern void sdfMemFreeDirect(void *aHandle);

extern void dds3NeedleModelVNCListDelete(dds3NeedleModel_tag *hNModel);

/* The copied 0x5C-byte needle parameters precede runtime ownership state. */
typedef struct effPCPNeedleSpiralParam_t {
    f32 position[4];
    u32 number;
    s32 life;
    u32 mode;
    s32 fade_in;
    s32 fade_out;
    u32 timing;
    f32 speed;
    f32 accele;
    f32 radius;
    f32 radius_r;
    f32 roll_s;
    u32 n_split;
    f32 radiusScale;
    f32 viewOffset;
    u32 centerColor;
    u32 outerColor;
    f32 width;
    s32 t_length;
    u32 t_abgr;
} effPCPNeedleSpiralParam_t;

typedef struct effPCPNeedleSpiralPart_tag {
    f32 pos[4];
    u8 old_pos[0x10];
    s32 frame;
    f32 radius;
    f32 radian;
} effPCPNeedleSpiralPart_tag; /* 0x2C */

typedef struct effAllocation_t effAllocation_t;

typedef struct trackPolySystem_t trackPolySystem_t;

typedef struct effRandState_t { u32 words[4]; } effRandState_t;

typedef struct effAxis_t { f32 x, y, z; } effAxis_t;

struct BtlUnit;

typedef struct effPCPNeedleSpiral_tag {
    effPCPNeedleSpiralParam_t param;
    effPCPNeedleSpiralPart_tag *pPart;
    u32 abgr;
    u32 number;
    trackPolySystem_t *hTrackPoly;
    dds3NeedleModel_tag *hNModel;
    effAllocation_t *hMem;
} effPCPNeedleSpiral_tag; /* 0x74, followed by number pPart */

#define PAR_CELL_TOPOLOGY_PAIR 0

#define EFFECT_RING_FULL_TURN 6.2831853f

extern effRandState_t dds3EffectBase[];

extern f32 aViewTarget[4], aViewEye[4], dds3NeedleModelUpVector[4];

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern void *sdfMatNew(void);

extern void sdfMatSetConstant(void *aMat, f32 aRatio);

extern void *memset(void *a, s32 b, size_t c);

extern u32 dds3GetRandA(effRandState_t *name);

extern f32 dds3GetRandFloatA(effRandState_t *name);

extern u32 effBTLSkillCheckRun(void);

extern u32 effBTLSkillGetInvokeFormation(void);

extern void effBTLModelGetSphereCenter(struct BtlUnit *pFormation);

extern void LookAtMatrix(f32 *aTarget, f32 *aEye, f32 *aDown);

extern void ReverseMatrix(void);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void SetRotateMatrixAxis(f32 aAngle, const effAxis_t *aAxis);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern trackPolySystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);

extern void dds3TrackPolyDelete(trackPolySystem_t *hTrackPoly);

extern void dds3TrackPolySetMode(trackPolySystem_t *hTrackPoly, u16 mode);

extern void dds3TrackPolyVCSet(addr_t hTrackPoly, s32 type, s32 abgr_in, s32 abgr_out);

extern void dds3TrackPolySet(trackPolySystem_t *hTrackPoly, s32 number, const u128 *pV);

extern void dds3TrackPolyVCRefresh_ParType1(trackPolySystem_t *hTrackPoly, s32 index);

extern void dds3TrackPolyColorSet(addr_t hTrackPoly, s32 number, u32 color);

extern void dds3TrackPolyListInit(trackPolySystem_t *hTrackPoly, s32 number);

extern void dds3TrackPolyDraw(trackPolySystem_t *system);

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam);

dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number);

void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel);

void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel);

void dds3NeedleModelVNCListCreate(dds3NeedleModel_tag *hNModel, u32 split, u32 radius, u32 length, f32 abgr_in, f32 abgr_out);

void dds3NeedleModelSetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);

void dds3NeedleModelGetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr);

void dds3NeedleModelSetMatrix(dds3NeedleModel_tag *hNModel, const f32 *pMatrix);

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam);

void effPCPNeedleSpiralDelete(effPCPNeedleSpiral_tag *hData);

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreatePCF(void *data);

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCopy(effPCPNeedleSpiralParam_t *work);

void effPCPNeedleSpiralDraw(effPCPNeedleSpiral_tag *hData);

void effPCPNeedleSpiralSetPosition(void *hData, void *pPos);

void effPCPNeedleSpiralSetRotateMatrix(effPCPNeedleSpiral_tag *hData, void *pMatrix);

void effPCPNeedleSpiralSetColor(effPCPNeedleSpiral_tag *hData, u32 abgr);

dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number);

void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel);

void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel);

#include "sdk_libc.h"

/* Build a closed triangle fan: one outer vertex, the axis centre and the next
 * outer vertex per segment. */
void dds3NeedleModelVNCListCreate(dds3NeedleModel_tag *hNModel, u32 split, u32 radius, u32 length, f32 abgr_in, f32 abgr_out);

void dds3NeedleModelVNCListDelete(dds3NeedleModel_tag *hNModel);

void dds3NeedleModelSetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);

void dds3NeedleModelGetPosition(dds3NeedleModel_tag *hNModel, s32 index, f32 *pPos);

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr);

/* vu0 routine: copy the resource transform through the matrix bank. */
void dds3NeedleModelSetMatrix(dds3NeedleModel_tag *hNModel, const f32 *pMatrix);
/* End of declarations from src/smt3/effect/effPCPNeedle.c */

effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam) {
    effAllocation_t *allocation = sdfMemAllocDirect(pParam->number * sizeof(effPCPNeedleSpiralPart_tag) + sizeof(effPCPNeedleSpiral_tag));
    effPCPNeedleSpiral_tag *work = (effPCPNeedleSpiral_tag *)sdfMemLock(allocation);
    u32 slotIndex;
    dds3NeedleModel_tag *resource;
    u32 count, delayRange;
    effPCPNeedleSpiralPart_tag *slot;
    f32 radiusBase, radiusJitter;

    work->pPart = (effPCPNeedleSpiralPart_tag *)(work + 1);
    work->param = *pParam;
    work->hMem = allocation;
    work->number = pParam->number;
    work->abgr = 0x80808080;
    work->hTrackPoly = 0;
    resource = dds3NeedleModelCreate(pParam->number);
    resource->blend = pParam->mode;
    work->hNModel = resource;
    dds3NeedleModelVNCListCreate(resource, pParam->n_split, pParam->centerColor,
                            pParam->outerColor, pParam->radiusScale, pParam->viewOffset);
    work->hTrackPoly = dds3TrackPolyCreate(work->number, work->param.t_length, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(EE_ADDR(work->hTrackPoly), 1, work->param.t_abgr, work->param.t_abgr);
    dds3TrackPolySetMode(work->hTrackPoly, (u16)work->param.mode);
    count = work->number;
    delayRange = SDF_FRAMES(pParam->timing);
    slot = work->pPart;
    if (delayRange == 0) delayRange = 1;
    radiusBase = work->param.radius;
    radiusJitter = work->param.radius_r;
    for (slotIndex = 0; slotIndex < count; slotIndex++, slot++) {
        slot->radian = dds3GetRandFloatA(dds3EffectBase) * EFFECT_RING_FULL_TURN;
        slot->radius = radiusBase * (dds3GetRandFloatA(dds3EffectBase) * radiusJitter + (1.0f - radiusJitter));
        slot->frame = -(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayRange);
        dds3NeedleModelSetColor(work->hNModel, slotIndex, 0x808080);
    }
    return work;
}

void effPCPNeedleSpiralDelete(effPCPNeedleSpiral_tag *hData) {
    dds3TrackPolyDelete(hData->hTrackPoly);
    dds3NeedleModelDelete(hData->hNModel);
    sdfMemFreeDirect(hData->hMem);
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00217FF0);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218010);

void func_00218028(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218030);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218040);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218058);

dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number)
{
    struct effAllocation_t *allocation = sdfMemAllocDirect(number * sizeof(dds3NeedleModelPart_tag) + sizeof(dds3NeedleModel_tag));
    dds3NeedleModel_tag *work = (void *)sdfMemLock(allocation);
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;

    work->blend = 2;
    work->pPart = (dds3NeedleModelPart_tag *)(work + 1);
    work->hMem = allocation;
    work->number = number;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->hVNMem = NULL;
    SDF_VU0_STOREIDENTITY(work->matrix);
    work->pMat = sdfMatNew();
    sdfMatSetConstant(work->pMat, 1.0f);
    entry = work->pPart;
    for (entryIndex = 0; entryIndex < number; entryIndex++, entry++) {
        entry->abgr = 0x80808080;
    }
    memset(&dds3NeedleModelPrimParam, 0, sizeof(dds3NeedleModelPrimParam));
    dds3NeedleModelPrimParam.primitive = 0x4000;
    return work;
}

void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel)
{
    sdfMatFreeDelayed(hNModel->pMat);
    dds3NeedleModelVNCListDelete(hNModel);
    sdfMemFreeDirect(hNModel->hMem);
}

void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel)
{
    f32 matrix[16] __attribute__((aligned(16)));
    void *packet;
    dds3NeedleModelPart_tag *entry;
    u32 entryIndex;
    u32 remaining;
    u32 perPrim;

    packet = sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    sdf3DPrimLinkMat(packet, hNModel->pMat, 0);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = hNModel->scale[0];
    matrix[5] = hNModel->scale[1];
    matrix[10] = hNModel->scale[2];
    SDF_VU0_LOADMATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(hNModel->matrix);
    MultMatrixNoArg();
    SDF_VU0_STOREMATRIX(matrix);
    dds3NeedleModelPrimParam.parameters = NULL;
    entry = hNModel->pPart;
    for (entryIndex = 0; entryIndex < hNModel->number; entryIndex++, entry++) {
        perPrim = 3;
        matrix[12] = entry->position[0];
        matrix[13] = entry->position[1];
        matrix[14] = entry->position[2];
        SDF_VU0_LOADMATRIX(matrix);
        sdf3DPrimLinkTm(packet, 0);
        if (hNModel->hVNMem == NULL) {
            remaining = 12;
            dds3NeedleModelPrimParam.colors = dds3NeedleModelDefaultColors;
            dds3NeedleModelPrimParam.positions = dds3NeedleModelDefaultPoints;
            dds3NeedleModelPrimParam.normals = NULL;
        } else {
            dds3NeedleModelPrimParam.positions = hNModel->positions;
            dds3NeedleModelPrimParam.colors = hNModel->pVC;
            dds3NeedleModelPrimParam.normals = hNModel->normals;
            remaining = hNModel->point;
        }
        dds3NeedleModelPrimParam.color = entry->abgr;
        dds3NeedleModelPrimParam.primCount = 16;
        dds3NeedleModelPrimParam.vertexCount = 48;
        while (remaining >= 48) {
            sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&dds3NeedleModelPrimParam)));
            if (hNModel->hVNMem == NULL) {
                dds3NeedleModelPrimParam.colors += 48;
                dds3NeedleModelPrimParam.positions += 48;
            } else {
                dds3NeedleModelPrimParam.colors += 48;
                dds3NeedleModelPrimParam.positions += 48;
                dds3NeedleModelPrimParam.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            dds3NeedleModelPrimParam.primCount = remaining / perPrim;
            dds3NeedleModelPrimParam.vertexCount = remaining;
            sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(&dds3NeedleModelPrimParam)));
        }
    }
    dds3NeedleModelDrawSurfaces[hNModel->blend]->append(dds3NeedleModelDrawSurfaces[hNModel->blend], packet);
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218450);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00218678);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_002186A8);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_002186D8);

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr)
{
    hNModel->pPart[index].abgr = abgr;
}
