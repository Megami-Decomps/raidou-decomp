#include "common.h"
#include "sdf_types.h"
#include "effPCPData.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdfVu0.h"
#include "sdfDraw.h"
#include "sdfMat.h"

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
extern void sdf3DPrimLinkMat(void *aTag, void *aMat, s32 aAllocator);
extern void sdf3DPrimLinkTm(void *aTag, s32 aAllocator);
extern void *effPkt3DPrimMakePoly(effPrimParam_t *aParm);
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
extern u32 dds3GetRandA(effRandState_t *name);
extern f32 dds3GetRandFloatA(effRandState_t *name);
extern trackPolySystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);
extern void dds3TrackPolyDelete(trackPolySystem_t *hTrackPoly);
extern void dds3TrackPolySetMode(trackPolySystem_t *hTrackPoly, u16 mode);
extern void dds3TrackPolyVCSet(addr_t hTrackPoly, s32 type, s32 abgr_in, s32 abgr_out);
effPCPNeedleSpiral_tag *effPCPNeedleSpiralCreate(effPCPNeedleSpiralParam_t *pParam);
dds3NeedleModel_tag *dds3NeedleModelCreate(u32 number);
void dds3NeedleModelDelete(dds3NeedleModel_tag *hNModel);
void dds3NeedleModelDraw(dds3NeedleModel_tag *hNModel);
void dds3NeedleModelVNCListCreate(dds3NeedleModel_tag *hNModel, u32 split, u32 radius, u32 length, f32 abgr_in, f32 abgr_out);
void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr);
void effPCPNeedleSpiralDelete(effPCPNeedleSpiral_tag *hData);

#include "sdk_libc.h"

void dds3NeedleModelVNCListDelete(dds3NeedleModel_tag *hNModel);

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

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effPCPNeedleSpiralCreatePCF);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effPCPNeedleSpiralCopy);

void effLeaf264(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effLeaf265);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effLeaf266);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effLeaf267);

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

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effBuildRadialFanStreams);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effReleaseOptionalResource);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", dds3NeedleModelSetPosition);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", dds3NeedleModelGetPosition);

void dds3NeedleModelSetColor(dds3NeedleModel_tag *hNModel, s32 index, u32 abgr)
{
    hNModel->pPart[index].abgr = abgr;
}
