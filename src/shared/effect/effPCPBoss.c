#include "common.h"
#include "raidou_alloc.h"
#include "kernel.h"
#include "ps2asm.h"
#include "fpu.h"
#include "model.h"
#include "motion.h"
#include "draw3d.h"
#include "effect.h"
#include "event.h"
#include "nb.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfModel.h"
#include "effMath.h"
#include "mdlManager.h"
#include "sdfVu0.h"
#include "polyManager.h"
#include "effPart.h"
#include "effPCPThunder.h"
#include "effPCPFlash.h"
#include "sdk_libc.h"
#include "effPCPBoss.h"

typedef struct effPCPBossBED_t
{
    u8 unk0000[0xC3C];
    s32 active;
} effPCPBossBED_t;

typedef union effPCPBossVector_t
{
    u128 q;
    f32 f[4];
} effPCPBossVector_t;

/* The placement block every effEveBEDCutCreate call in this unit fills on the
 * stack. */
typedef struct effPCPBossPlace_t
{
    f32 payload[11];
    u32 color;
} effPCPBossPlace_t;

/* One crack segment: the extents effPCPCrackSizeInit randomizes plus the
 * negative age that delays its start. */
typedef struct effPCPCrackPartFlash_t
{
    f32 length;
    f32 width_in;
    f32 width_out;
    s32 frame;
    u8 type;
    u8 unk11[3];
} effPCPCrackPartFlash_t;

typedef struct effPCPCrackPart_t
{
    dds3FlashPolyList_t *hFlash;
    f32 dir[3];
    u8 unk10[4];
    f32 rotate;
    s32 rotationStartAge;
    f32 rotate_add;
    effPCPCrackPartFlash_t *pSize;
} effPCPCrackPart_t;

/* Parameter fields below are read directly by effPCPCrackDraw. The complete
 * 0x8C head is copied by Create/Copy, followed by the 0x20 runtime suffix. */
typedef struct effPCPCrack_t
{
    f32 position[4];
    u8 drawTrail;
    f32 frameStep;
    f32 scale;
    f32 trailWidth;
    s32 trailColorStartFrame;
    s32 trailColorTransitionFrames;
    u32 trailStartCenterColor;
    u32 trailStartEdgeColor;
    u32 trailEndCenterColor;
    u32 trailEndEdgeColor;
    u16 drawBucket;
    u8 unk3A[2];
    u8 hasCells;
    u8 cellCountFromFrame;
    s32 cellStartFrame;
    s32 delaySpread;
    s32 cellDuration;
    s32 cellFadeIn;
    s32 cellFadeOut;
    s32 offsetRampFrames;
    f32 offsetDistance;
    f32 offsetRandomness;
    f32 baseExtent;
    f32 tipExtent;
    f32 extentRandomness;
    s32 cellColorStartAge;
    s32 cellColorTransitionFrames;
    u32 cellStartCenterColor;
    u32 cellStartEdgeColor;
    u32 cellEndCenterColor;
    u32 cellEndEdgeColor;
    s32 blend;
    u8 directionMode;
    u8 unk89[3];
    effPCPCrackPart_t *pPart;
    void *groupsHandle;
    u32 MarkCount;
    u16 cellCount;
    u8 unk9A[2];
    s32 frame;
    u32 abgr;
    dds3TrackPoly_t *system;
    effPartHandle_t *part;
} effPCPCrack_t;

/* The parameter block effPCPCrackCreate copies into the work head; the two
 * rotation fields sit where the work keeps its group pointer. */
/* The leading block effPCPCrackCreate copies into the work head in one go. */
typedef struct effCrackHead_t
{
    s32 word[35];
} effCrackHead_t;

typedef struct effPCPCrackParam_t
{
    effCrackHead_t head;
    s32 rotationStartAge;
    f32 angularSpeed;
} effPCPCrackParam_t;

/* Five vertex colour words per cell. */
typedef struct effCrackColorSlot_t
{
    u32 color[5];
} effCrackColorSlot_t;

typedef struct effBaelHead_t
{
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
} effBaelHead_t;

typedef struct effPCPBossBael_t
{
    f32 target[4];
    effBaelHead_t param;
    s32 frame;
    u32 abgr;
    effPartHandle_t *hPB;
    effPartHandle_t *hPCF;
} effPCPBossBael_t;

typedef struct effPCPBossONGY_t
{
    struct nbFormation_t *param;
    sdfMat_t *pMat;
    f32 radian;
    s32 frame;
} effPCPBossONGY_t;

/* The argument block effPkt3DPrimMakePoly reads. */
typedef struct effPCPBossWork_t
{
    u16 primCount;
    u16 vertexCount;
    u16 unk04;
    u8 unk06[2];
    u32 color;
    const void *unk0C;
    effPCPBossVector_t *positions;
    u8 unk14[4];
    f32 *uvs;
    u8 unk1C[4];
    u32 *colors;
    u8 unk24[8];
} effPCPBossWork_t;

typedef struct effPCPBossBUNSIN_t
{
    f32 pos_s[4];
    f32 pos_e[4];
    effPCPBossBUNSINParam_t param;
    void *matrix;
    union {
        s32 unk2C;
        f32 radian;
    };
    s32 frame;
} effPCPBossBUNSIN_t;

typedef struct effPCPBossBUNSIN2_t
{
    effPCPBossBUNSIN2Param_t head;
    s32 frame;
    s32 state;
    effPCPBossBUNSIN_t *parts[3];
} effPCPBossBUNSIN2_t;

typedef struct effPCPBossDengeki_t
{
    u8 unk00[0x54];
    f32 scale;
    void *polyLists[4];
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossDengeki_t;

typedef struct effPCPBossThunderbolt_t
{
    u8 position[0x10];
    u8 param[0x54];
    void *pTPolyList;
    s32 frame;
    u32 color;
} effPCPBossThunderbolt_t;

typedef struct effPCPBossKAGUInvoke_t
{
    union
    {
        u8 param[0x3C0];
        struct
        {
            f32 directions[6][9][4];
            effPCP_ThunderPolyParam_t head;
            s32 limit;
            s32 fadeIn;
            s32 fadeOut;
        } draw;
    };
    s32 pTPList;
    void *point[26];
    s32 frame;
    u32 color;
    void *handle;
} effPCPBossKAGUInvoke_t;

typedef struct effPCPBossKAGUAttack_t
{
    u8 unk00[0x54];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
    u8 unk68[4];
    void *polyLists[32];
    effPCPBossBED_t *hBED;
    void *cut;
    s32 frame;
    u32 color;
} effPCPBossKAGUAttack_t;

typedef struct effPCPBossKAGUThunderbolt_t
{
    u8 position[0x10];
    u8 unk010[0xE0];
    f32 scale;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    void *pTPolyList;
    effPCPCrack_t *pCrack;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
    u32 color;
} effPCPBossKAGUThunderbolt_t;

typedef struct effPCPBossKAGU2Attack_t
{
    u8 unk00[0x54];
    f32 scale;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 groupCount;
    f32 randomness;
    void *polyLists[16];
    effPCPBossBED_t *hBED;
    void *hBEDCut;
    s32 frame;
    u32 color;
} effPCPBossKAGU2Attack_t;

typedef struct effPCPBossKAGU2MUHENK_t
{
    s32 param;
    u32 color;
} effPCPBossKAGU2MUHENK_t;

typedef struct effPCPBossMETATOBeam_t
{
    u8 position[0x10];
    u8 unk10[0x54];
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
    effPCP_ThunderPolyList_t *polyLists[2];
    s32 frame;
    u32 abgr;
} effPCPBossMETATOBeam_t;

typedef struct effPCPScreenCrackModel_t
{
    s32 param;
    effPartHandle_t *hModel;
    s32 frame;
    u32 abgr;
} effPCPScreenCrackModel_t;

typedef struct effPCPMuzzleFlashParam_t
{
    s32 scale_h;
    s32 scale_w;
} effPCPMuzzleFlashParam_t;

typedef struct effPCPMuzzleFlash_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCPMuzzleFlashParam_t param;
    void *hBillboard;
    s32 frame;
    f32 scale;
} effPCPMuzzleFlash_tag;

/* The block effPCPDANTE_Thunder_Copy hands straight back to Create. */
typedef struct effPCPDANTE_Thunder_Param_t
{
    u8 head[0x54];
    s32 mark;
    s32 limit;
    s32 fadeIn;
    s32 fadeOut;
} effPCPDANTE_Thunder_Param_t;

typedef struct effPCPDANTE_Thunder_tag
{
    u8 position[0x10];
    u8 matrix[0x40];
    effPCPDANTE_Thunder_Param_t params;
    f32 scale;
    u32 color;
    void *poly;
    s32 frame;
    u8 drawRequest;
    u8 unkC5[3];
    s32 process;
} effPCPDANTE_Thunder_tag;

typedef struct effPCPDANTE_UV_tag
{
    s32 u;
    s32 v;
    u32 w;
    u32 h;
} effPCPDANTE_UV_tag;

/* The 16-byte block effPCPDANTE_Damage_Create copies into the work head; the
 * last word is the per-target delay step. */
typedef struct effPCPDANTE_Damage_Param_t
{
    s32 total;
    s32 fadeIn;
    s32 fadeOut;
    s32 wait;
} effPCPDANTE_Damage_Param_t;

typedef struct effPCPDANTE_Damage_tag
{
    effPCPDANTE_Damage_Param_t param;
    u8 delay[0x20];
    s32 unk030;
    u8 unk034;
    u8 unk035[3];
    effPCPBossBED_t *bed;
    void *cuts[0x20];
    s32 counters[0x20];
    u32 color;
} effPCPDANTE_Damage_tag;

typedef struct effPCP_PBCamera_Param_t
{
    s32 life;
} effPCP_PBCamera_Param_t;

typedef struct effPCP_PBCamera_tag
{
    u8 matrix[0x40];
    u8 position[0x10];
    s32 life;
    effPartHandle_t *part;
    u8 cameraOverride;
    u8 unk59[3];
    s32 frame;
} effPCP_PBCamera_tag;

typedef struct effPCPDANTE_ShowTime_Param_t
{
    s32 reserve;
} effPCPDANTE_ShowTime_Param_t;

typedef struct effPCPDANTE_ShowTime_tag
{
    u8 position[0x10];
    s32 param;
    u8 flash[3][0x38];
    void *hTex[2];
    s32 frame;
    s32 frameParts[3]; /* Bingo, AreYouReady, ShowTime at +0xC8/+0xCC/+0xD0. */
    u8 texLoaded;
} effPCPDANTE_ShowTime_tag;

typedef struct effPCP_BED_Param_t
{
    s32 life;
    s32 fade_in;
    s32 fade_out;
} effPCP_BED_Param_t;

typedef struct effPCP_BED_tag
{
    u8 matrix[0x40];
    u8 pos[0x10];
    effPCP_BED_Param_t param;
    u32 abgr;
    s32 frame;
    effPCPBossBED_t *hBED;
    void *hBEDCut;
} effPCP_BED_tag;

typedef struct effPCP_HBED2_Param_t
{
    s32 word[7];
} effPCP_HBED2_Param_t;

typedef struct effPCP_HBED2_tag
{
    effPCP_HBED2_Param_t param;
    void *pHelperBED;
} effPCP_HBED2_tag;

extern u8 dds3EffectBase[];
extern s32 sdfModelGetMarkPosMatrix(void *aRoot, s32 aId);
extern void effPCPCrackInit(effPCPCrack_t *pPCPCrack);
extern u8 dds3RenderTargetVector[], dds3RenderEyeVector[];
extern u32 dds3GetRandA(u8 *name);
extern f32 dds3GetRandFloatA(u8 *name);
extern void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);
extern void dds3TrackPolySetMode(void *hTrackPoly, s32 mode);
extern effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB);
extern effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);


extern effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);
extern void effPCPCrackDraw(effPCPCrack_t *hData);
extern void mdlSetPosition(mdlObj_t *aHandle);
extern void mdlLink(mdlObj_t *aHandle, void *aModelOt);
extern f32 aViewEye[4], aViewTarget[4];
extern s32 sdfModelGetMarkPos(void *aRoot, s32 aId);
extern s32 effPadVibeFrames[2], effPadVibeRemain[2];
extern u8 effPadVibePower[2], effPadVibeFadeIn[2];
extern effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF);
extern effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB);
void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize);
void effPCPCrackInit(effPCPCrack_t *pPCPCrack);
effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB);
effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src);
void effPCPCrackDelete(effPCPCrack_t *hData);
extern f32 effPCPCrackDrawData[4];
extern void mdlSetScale(mdlObj_t *model);
extern u32 effMagaCylinderCalcColor(u32 color, u32 scale);
void effPCPCrackDraw(effPCPCrack_t *work);
effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF);
effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata);
void effPCPBossBaelDelete(effPCPBossBael_t *hData);
effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB);
effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src);
void effPCPCrack2Delete(effPCPCrack_t *hData);
void effPCPCrack2Draw(effPCPCrack_t *hData);
effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam);
effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *hData, s32 bPos, s32 tbldata);
effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *hData, s32 bPos, s32 tbldata);
void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData);

/* nbCameraProcess camera prefix; D_00320E64 is its fovy at +0x24. */
typedef struct effPCPBossCamera_t {
    sdfVector4_t target;
    sdfVector4_t direction;
    f32 distance;
    f32 fovy;
    u32 state_change;
    u32 state;
} effPCPBossCamera_t;

extern s32 effPadVibeFrames[2];
extern u8 effPadVibePower[2];

void effPCPCrackSizeInit(effPCPCrack_t *pPCPCrack, effPCPCrackPartFlash_t *pSize)
{
    f32 blend = pPCPCrack->offsetRandomness;
    f32 scale = pPCPCrack->scale;
    f32 t;

    pSize->length = pPCPCrack->offsetDistance *
        (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend)) * scale;
    blend = pPCPCrack->extentRandomness;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    pSize->width_in = pPCPCrack->baseExtent * t * scale;
    pSize->width_out = pPCPCrack->tipExtent * t * scale;
    pSize->type = dds3GetRandA(dds3EffectBase) & 1;
    pSize->frame = -(dds3GetRandA(dds3EffectBase) % pPCPCrack->delaySpread);
}

void effPCPCrackInit(effPCPCrack_t *pPCPCrack)
{
    mdlObj_t *model = dds3EffectPartGetHandle(pPCPCrack->part);
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *nextCells;
    u32 groupIndex;
    u32 cellIndex;

    if (pPCPCrack->delaySpread <= 0) {
        pPCPCrack->delaySpread = 1;
    }
    model->first->speed = pPCPCrack->frameStep;
    mdlSingleMotion(model, 0, 0);
    pPCPCrack->cellCount = model->first->frameLength;
    pPCPCrack->MarkCount = sdfModelGetMarkPosCount(model->inner);
    pPCPCrack->system = dds3TrackPolyCreate(pPCPCrack->MarkCount, pPCPCrack->cellCount, 1, 1);
    dds3TrackPolySetMode(pPCPCrack->system, pPCPCrack->drawBucket);
    pPCPCrack->groupsHandle = NULL;
    if (pPCPCrack->hasCells) {
        pPCPCrack->groupsHandle = sdfMemAllocDirect(pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t)
            + pPCPCrack->cellCount * pPCPCrack->MarkCount * sizeof(effPCPCrackPartFlash_t));
        nextCells = (effPCPCrackPartFlash_t *)sdfMemLock(pPCPCrack->groupsHandle);
        pPCPCrack->pPart = (effPCPCrackPart_t *)nextCells;
        nextCells = (effPCPCrackPartFlash_t *)((u8 *)nextCells + pPCPCrack->MarkCount * sizeof(effPCPCrackPart_t));
        for (groupIndex = 0, group = pPCPCrack->pPart; groupIndex < pPCPCrack->MarkCount; groupIndex++, group++) {
            effPCPCrackPartFlash_t *cell;

            group->hFlash = dds3FlashPolyListCreate(pPCPCrack->cellCount);
            if (pPCPCrack->directionMode == 0) {
                group->dir[0] = 0.0f;
                group->dir[1] = -1.0f;
                group->dir[2] = 0.0f;
            } else {
                group->dir[0] = 0.0f;
                group->dir[1] = 1.0f;
                group->dir[2] = 0.0f;
            }
            group->rotate_add = (0.1f * (3.14159265f / 180.0f));
            group->rotate = 0.0f;
            for (cellIndex = 0; cellIndex < pPCPCrack->cellCount; cellIndex++) {
                f32 *vertices = dds3FlashPolyListGetVertex(group->hFlash, cellIndex);
                effCrackColorSlot_t *colors = dds3FlashPolyListGetVC(group->hFlash, cellIndex);

                colors->color[0] = 0;
                colors->color[1] = 0;
                colors->color[2] = 0;
                colors->color[3] = 0;
                colors->color[4] = 0;
                SDF_VU0_LOADIDENTITYVECTOR();
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_STOREVECTOR(vertices + 4);
                SDF_VU0_STOREVECTOR(vertices + 8);
                SDF_VU0_STOREVECTOR(vertices + 12);
                SDF_VU0_STOREVECTOR(vertices + 16);
            }
            dds3FlashPolyListSetBlend(group->hFlash, pPCPCrack->blend);
            group->pSize = nextCells;
            nextCells += pPCPCrack->cellCount;
            for (cellIndex = 0, cell = group->pSize; cellIndex < pPCPCrack->cellCount; cellIndex++, cell++) {
                effPCPCrackSizeInit(pPCPCrack, cell);
            }
        }
    }
}

effPCPCrack_t *effPCPCrackCreate(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work = sdfChipAllocDirect(0xAC);

    *(effCrackHead_t *)work = pParam->head;
    work->abgr = 0x80808080;
    work->frame = 0;
    work->part = dds3EffectPartCreate(3, pPB);
    effPCPCrackInit(work);
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPBoss.c:1743, match, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
effPCPCrack_t *effPCPCrackCreatePCF(void *parameterTable) {
    effPCPCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(parameterTable, 0);
    effectParams = dds3EffectPCPDataGetData(parameterTable, 1);
    return effPCPCrackCreate(bossParams, effectParams);
}

effPCPCrack_t *effPCPCrackCopy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPCrack_t));
    memcpy(work, src, sizeof(effCrackHead_t));
    work->frame = 0;
    work->abgr = 0x80808080;
    work->part = dds3EffectPartCopy(src->part);
    effPCPCrackInit(work);
    return work;
}

void effPCPCrackDelete(effPCPCrack_t *hData)
{
    u32 groupIndex;

    if (hData->groupsHandle != NULL) {
        for (groupIndex = 0; groupIndex < hData->MarkCount; groupIndex++) {
            dds3FlashPolyListDelete(hData->pPart[groupIndex].hFlash);
        }
        sdfMemFreeDirect(hData->groupsHandle);
    }
    dds3TrackPolyDelete(hData->system);
    dds3EffectPartDelete(hData->part);
    sdfChipFreeDirect(hData);
}

/* Transform each trail sample, then emit and fade its delayed flash cells. */
void effPCPCrackDraw(effPCPCrack_t *work) {
    sdfVector4_t perpendicular;
    sdfVector4_t direction;
    sdfVector4_t axis;
    sdfVector4_t scaleVector;
    sdfVector4_t widthVector;
    sdfVector4_t displacement;
    sdfVector4_t baseVector;
    sdfVector4_t tipVector;
    mdlObj_t *model;
    struct sdfModelRootNode_t *map;
    s32 i;
    s32 colorStart;
    s32 mode;
    u32 tint;
    u32 center;
    u128 *historyStart;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    s32 colorFrames;
    u8 cellCountFromFrame;
    effPCPCrackPart_t *group;
    effPCPCrackPartFlash_t *cell;
    dds3TrackPolyList_t *historyCell;
    u128 *positions; /* Position workspace reused for trail history and cell vertices. */
    u32 *colorWords;
    s32 frame;
    s32 groupCount;
    s32 colorCellCount;
    s32 j, count;
    s32 historyCount;
    s32 age, currentAge;
    u32 edge, alphaColor;
    f32 blend, trailLength, cellLength, taperStep;
    f32 taperRange;
    f32 baseExtent, tipExtent;

    frame = SDF_FRAME_TICK(work->frame);
    mode = work->directionMode;
    model = dds3EffectPartGetHandle(work->part);
    map = model->inner;
    SDF_VU0_LOADVECTOR(work->position);
    mdlSetPosition(model);
    scaleVector.x = work->scale;
    scaleVector.y = work->scale;
    scaleVector.z = work->scale;
    scaleVector.w = 0.0f;
    SDF_VU0_LOADVECTOR(&scaleVector);
    mdlSetScale(model);
    mdlLink(model, sdfModelOtTable);
    if (work->drawTrail) {
        colorStart = work->trailColorStartFrame;
        if (frame > colorStart) {
            if (work->trailColorTransitionFrames > 0) {
                blend = (f32)(frame - colorStart) / work->trailColorTransitionFrames;
                center = effColorBlend(work->trailStartCenterColor, work->trailEndCenterColor, blend);
                edge = effColorBlend(work->trailStartEdgeColor, work->trailEndEdgeColor, blend);
            } else {
                center = work->trailEndCenterColor;
                edge = work->trailEndEdgeColor;
            }
        } else {
            center = work->trailStartCenterColor;
            edge = work->trailStartEdgeColor;
        }
        groupCount = work->MarkCount;
        for (i = 0; i < groupCount; i++) {
            colorWords = work->system->trackList[i].pVC;
            colorCellCount = work->cellCount;
            if (colorCellCount != 0) {
                j = colorCellCount;
                do {
                    colorWords[0] = edge;
                    colorWords[1] = center;
                    colorWords[2] = edge;
                    colorWords += 3;
                    --j;
                } while (j != 0);
            }
            groupCount = work->MarkCount;
        }
    }
    groupCount = work->MarkCount;
    widthVector.x = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.y = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.z = (work->trailWidth * 0.5f) * scaleVector.x;
    widthVector.w = 0.0f;
    perpendicular.x = perpendicular.y = perpendicular.z = perpendicular.w = widthVector.w;
    tint = work->abgr;
    i = 0;
    if (groupCount > 0) {
        taperRange = -1.0f;
        do {
            historyCell = &work->system->trackList[i];
            positions = historyCell->pVertex;
            historyCell->color = tint;
            if (mode == 2) {
                direction.x = 0.0f;
                direction.y = 0.0f;
                direction.z = 1.0f;
                sdfModelGetMarkPosMatrix(map, i);
                SDF_VU0_LOADVECTOR(&direction);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(&direction);
                work->pPart[i].dir[0] = direction.x;
                work->pPart[i].dir[1] = direction.y;
                work->pPart[i].dir[2] = direction.z;
            }
            sdfModelGetMarkPos(map, i);
            if (frame < work->cellCount) {
                historyCount = historyCell->count;
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 1));
                SDF_FRAME_WHEN(work->frame, SDF_VU0_STOREVECTOR_NOMEM(positions + historyCount + 2));
                historyCell->count += 3 * SDF_FRAME_DUE(work->frame);
                taperStep = taperRange / frame;
                blend = 1.0f;
                SDF_VU0_LOADVECTOR_C(effPCPCrackDrawData);
                if (frame > 0) {
                    const f32 *trailWidth = &widthVector.x;
                    j = frame;
                    do {
                        SDF_VU0_LOADVECTOR(positions + 4);
                        SDF_VU0_LOADVECTOR_B(positions + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(trailLength);
                        if (trailLength > 1.0f) {
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_OUTERPRODUCT();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR(&perpendicular);
                        } else {
                            SDF_VU0_LOADVECTOR(&perpendicular);
                        }
                        SDF_VU0_LOADVECTOR_B(trailWidth);
                        SDF_VU0_SCALEVECTOR_B_BITS(blend);
                        SDF_VU0_MULVECTOR();
                        SDF_VU0_COPYVECTOR_A_TO_B();
                        SDF_VU0_LOADVECTOR(positions + 1);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(positions);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_STOREVECTOR(positions + 2);
                        --j;
                        blend += taperStep;
                        positions += 3;
                    } while (j != 0);
                }
            }
            i++;
        } while (i < groupCount);
    }
    if (work->drawTrail) {
        dds3TrackPolyDraw(work->system);
    }
    if (work->groupsHandle != NULL) {
        duration = work->cellDuration;
        count = frame - work->cellStartFrame;
        fadeIn = work->cellFadeIn;
        fadeOut = work->cellFadeOut;
        colorStart = work->cellColorStartAge;
        colorFrames = work->cellColorTransitionFrames;
        cellCountFromFrame = work->cellCountFromFrame;
        if (count > 0) {
            ramp = work->offsetRampFrames;
            if (cellCountFromFrame == 1) count = frame;
            if (count > work->cellCount) count = work->cellCount;
            group = work->pPart;
            for (i = 0; i < groupCount; i++, group++) {
                historyCell = &work->system->trackList[i];
                historyStart = historyCell->pVertex;
                cell = group->pSize;
                dds3FlashPolyListSetABGR(group->hFlash, tint);
                for (j = 0; j < count - 1; j++, cell++, historyStart += 3) {
                    positions = (u128 *)dds3FlashPolyListGetVertex(group->hFlash, j);
                    colorWords = dds3FlashPolyListGetVC(group->hFlash, j);
                    age = cell->frame;
                    if (age > colorStart) {
                        if (work->cellColorTransitionFrames > 0 && age < colorStart + colorFrames) {
                            blend = (f32)(age - colorStart) / colorFrames;
                            center = effColorBlend(work->cellStartCenterColor, work->cellEndCenterColor, blend);
                            edge = effColorBlend(work->cellStartEdgeColor, work->cellEndEdgeColor, blend);
                        } else {
                            center = work->cellEndCenterColor;
                            edge = work->cellEndEdgeColor;
                        }
                    } else {
                        center = work->cellStartCenterColor;
                        edge = work->cellStartEdgeColor;
                    }
                    center &= 0xFFFFFF;
                    edge &= 0xFFFFFF;
                    currentAge = cell->frame;
                    if (currentAge >= 0) {
                        if (currentAge >= ramp) direction.y = cell->length;
                        else direction.y = (cell->length * currentAge) / ramp;
                        displacement.x = group->dir[0] * direction.y;
                        displacement.y = group->dir[1] * direction.y;
                        displacement.z = group->dir[2] * direction.y;
                        displacement.w = 0.0f;
                        if (mode == 2 && currentAge >= group->rotationStartAge) {
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(historyStart + 1);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_STOREVECTOR_NOMEM(&axis);
                            SetRotateMatrixAxis(group->rotate, (const sdfVector3_t *)&axis);
                            SDF_VU0_LOADVECTOR(&displacement);
                            SDF_VU0_APPLYMATRIX_XYZ();
                            SDF_VU0_STOREVECTOR(&displacement);
                            group->rotate += SDF_FRAME_STEP(group->rotate_add);
                        }
                        baseExtent = cell->width_in;
                        baseVector.x = baseExtent;
                        baseVector.y = baseExtent;
                        baseVector.z = baseExtent;
                        baseVector.w = 0.0f;
                        tipExtent = cell->width_out;
                        tipVector.x = tipExtent;
                        tipVector.y = tipExtent;
                        tipVector.z = tipExtent;
                        tipVector.w = 0.0f;
                        SDF_VU0_LOADVECTOR(historyStart + 4);
                        SDF_VU0_LOADVECTOR_B(historyStart + 1);
                        SDF_VU0_SUBVECTOR();
                        SDF_VU0_GETLENGTH(cellLength);
                        if (cellLength > 1.0f) {
                            SDF_VU0_NORMALIZE();
                            SDF_VU0_COPYVECTOR_A_TO_B();
                            SDF_VU0_LOADVECTOR(&tipVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&tipVector);
                            SDF_VU0_LOADVECTOR(&baseVector);
                            SDF_VU0_MULVECTOR();
                            SDF_VU0_STOREVECTOR(&baseVector);
                            SDF_VU0_LOADVECTOR_C(&displacement);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&baseVector);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 4);
                            SDF_VU0_LOADVECTOR(historyStart + 4);
                            SDF_VU0_LOADVECTOR_B(&tipVector);
                            SDF_VU0_ADDVECTOR_A_C();
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_SUBVECTOR();
                            SDF_VU0_STOREVECTOR(positions + 1);
                        } else {
                            SDF_VU0_LOADIDENTITYVECTOR();
                            SDF_VU0_STOREVECTOR(positions);
                            SDF_VU0_STOREVECTOR(positions + 1);
                            SDF_VU0_STOREVECTOR(positions + 2);
                            SDF_VU0_STOREVECTOR(positions + 3);
                            SDF_VU0_STOREVECTOR(positions + 4);
                        }
                        alphaColor = 0;
                        if (age <= duration) {
                            if (age < fadeIn && fadeIn != 0) {
                                blend = (f32)age / fadeIn;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else if (duration - age <= fadeOut && fadeOut != 0) {
                                blend = (f32)(duration - age) / fadeOut;
                                alphaColor = effMagaCylinderCalcColor(((u32)(blend * 2147483648.0f) & 0xFF000000) | 0x808080, tint);
                            } else alphaColor = tint;
                        }
                        colorWords[0] = effMagaCylinderCalcColor(edge, alphaColor);
                        colorWords[1] = effMagaCylinderCalcColor(edge, alphaColor);
                        if (cell->type) {
                            colorWords[2] = effMagaCylinderCalcColor(0x80000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0xFF000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        } else {
                            colorWords[2] = effMagaCylinderCalcColor(0xFF000000, alphaColor);
                            colorWords[3] = effMagaCylinderCalcColor(center | 0x40000000, alphaColor);
                            colorWords[4] = colorWords[2];
                        }
                    }
                    cell->frame += SDF_FRAME_DUE(work->frame);
                }
                dds3FlashPolyListDraw(group->hFlash);
            }
        }
    }
    work->frame++;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effPCPCrackSetPosition);

void effPCPCrackSetColor(effPCPCrack_t *hData, u32 abgr)
{
    hData->abgr = abgr;
}

effPCPBossBael_t *effPCPBossBaelCreate(void *pParam, void *pPB, void *pPCF)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, pParam, 0x10);
    work->abgr = 0x80808080;
    work->frame = 0;
    work->hPB = dds3EffectPartCreate(3, pPB);
    work->hPCF = dds3EffectPartCreate(6, pPCF);
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPBoss.c:2106, match, same function 1.00 (words 25/25, calls 1.00, shape 1.00) */
effPCPBossBael_t *effPCPBossBaelCreatePCF(void *parameterTable) {
    void *firstBlock;
    void *secondBlock;
    void *thirdBlock;

    firstBlock = dds3EffectPCPDataGetData(parameterTable, 0);
    secondBlock = dds3EffectPCPDataGetData(parameterTable, 1);
    thirdBlock = dds3EffectPCPDataGetData(parameterTable, 2);
    return effPCPBossBaelCreate(firstBlock, secondBlock, thirdBlock);
}

effPCPBossBael_t *effPCPBossBaelCopy(effPCPBossBael_t *hData, s32 bPos, s32 tbldata)
{
    effPCPBossBael_t *work;

    work = sdfChipAllocDirect(sizeof(effPCPBossBael_t));
    memcpy(&work->param, &hData->param, 0x10);
    work->frame = 0;
    work->abgr = 0x80808080;
    work->hPB = dds3EffectPartCopy(hData->hPB);
    work->hPCF = dds3EffectPartCopy(hData->hPCF);
    return work;
}

void effPCPBossBaelDelete(effPCPBossBael_t *hData)
{
    dds3EffectPartDelete(hData->hPCF);
    dds3EffectPartDelete(hData->hPB);
    sdfChipFreeDirect(hData);
}

void effLeaf288(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", effLeaf289);

void effLeaf290(effPCPCrack_t *hData, s32 frames)
{
    hData->trailColorTransitionFrames = frames;
}

s32 effLeaf291(void)
{
    return 0;
}

s32 effLeaf292(void)
{
    return 0;
}

s32 effLeaf293(void)
{
    return 0;
}

void effLeaf294(void) {
}

void effLeaf295(void) {
}

void effLeaf296(void) {
}

s32 effLeaf297(void)
{
    return 0;
}

s32 effLeaf298(void)
{
    return 0;
}

s32 effLeaf299(void)
{
    return 0;
}

void effLeaf300(void) {
}

void effLeaf301(void) {
}

void effLeaf302(void) {
}

void effLeaf303(void) {
}

s32 effLeaf304(void)
{
    return 0;
}

s32 effLeaf305(void)
{
    return 0;
}

s32 effLeaf306(void)
{
    return 0;
}

void effLeaf307(void) {
}

void effLeaf308(void) {
}

void effLeaf309(void) {
}

s32 effLeaf310(void)
{
    return 0;
}

s32 effLeaf311(void)
{
    return 0;
}

s32 effLeaf312(void)
{
    return 0;
}

void effLeaf313(void) {
}

void effLeaf314(void) {
}

void effLeaf315(void) {
}

s32 effLeaf316(void)
{
    return 0;
}

s32 effLeaf317(void)
{
    return 0;
}

s32 effLeaf318(void)
{
    return 0;
}

void effLeaf319(void) {
}

void effLeaf320(void) {
}

void effLeaf321(void) {
}

void effLeaf322(void) {
}

s32 effLeaf323(void)
{
    return 0;
}

s32 effLeaf324(void)
{
    return 0;
}

s32 effLeaf325(void)
{
    return 0;
}

void effLeaf326(void) {
}

void effLeaf327(void) {
}

void effLeaf328(void) {
}

void effLeaf329(void) {
}

s32 effLeaf330(void)
{
    return 0;
}

s32 effLeaf331(void)
{
    return 0;
}

s32 effLeaf332(void)
{
    return 0;
}

void effLeaf333(void) {
}

void effLeaf334(void) {
}

void effLeaf335(void) {
}

effPCPCrack_t *effPCPCrack2Create(effPCPCrackParam_t *pParam, void *pPB)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCreate(pParam, pPB);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = pParam->rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = pParam->angularSpeed;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPBoss.c:3523, match, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
effPCPCrack_t *effPCPCrack2CreatePCF(void *data) {
    effPCPCrackParam_t *bossParams;
    void *effectParams;

    bossParams = dds3EffectPCPDataGetData(data, 0);
    effectParams = dds3EffectPCPDataGetData(data, 1);
    return effPCPCrack2Create(bossParams, effectParams);
}

effPCPCrack_t *effPCPCrack2Copy(effPCPCrack_t *src)
{
    effPCPCrack_t *work;
    u32 groupIndex;

    work = effPCPCrackCopy(src);
    for (groupIndex = 0; groupIndex < work->MarkCount; groupIndex++) {
        work->pPart[groupIndex].rotationStartAge = src->pPart[groupIndex].rotationStartAge;
        work->pPart[groupIndex].rotate = 0.0f;
        work->pPart[groupIndex].rotate_add = src->pPart[groupIndex].rotate_add;
    }
    return work;
}

void effPCPCrack2Delete(effPCPCrack_t *hData)
{
    effPCPCrackDelete(hData);
}

void effPCPCrackDraw(effPCPCrack_t *hData);

void effPCPCrack2Draw(effPCPCrack_t *hData)
{
    effPCPCrackDraw(hData);
}

void effPCPCrack2SetPosition(void *arg0, void *arg1) {
    effPCPCrackSetPosition(arg0, arg1);
}

void effPCPCrack2SetColor(void *arg0, u32 arg1) {
    effPCPCrackSetColor(arg0, arg1);
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreate(void *pParam)
{
    effPCP_ThunderPolyListParam_t thunder;
    effPCPBossMETATOBeam_t *work = sdfChipAllocDirect(sizeof(effPCPBossMETATOBeam_t));

    memcpy(work, pParam, 0x70);
    work->abgr = 0x80808080;
    work->frame = 0;
    memcpy(&thunder.TPParam, work->unk10, 0x54);
    thunder.count = 4;
    work->polyLists[0] = effPCP_ThunderPolyListCreate(&thunder);
    work->polyLists[1] = effPCP_ThunderPolyListCreate(&thunder);
    return work;
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCreatePCF(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(dds3EffectPCPDataGetData(hData, 0));
}

effPCPBossMETATOBeam_t *effPCPBossMETATOBeamCopy(void *hData, s32 bPos, s32 tbldata)
{
    return effPCPBossMETATOBeamCreate(hData);
}

void effPCPBossMETATOBeamDelete(effPCPBossMETATOBeam_t *hData)
{
    effPCP_ThunderPolyListDelete(hData->polyLists[1]);
    effPCP_ThunderPolyListDelete(hData->polyLists[0]);
    sdfChipFreeDirect(hData);
}
