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
#include "sdfDraw.h"
#include "sdfMat.h"
#include "billManager.h"
#include "sdfMath.h"
#include "sdfPacketBuff.h"
#include "sdfQuat.h"
#include "dds3SceneBasic.h"
#include "effBlur_Filter.h"
#include "dds3PathBasic.h"
#include "effPacket.h"
#include "sdf3DPrim.h"
#include "sdk_libm.h"
#include "dds3PadManager.h"
#include "effPCPMisc_00224D10.h"
#include "mdlManager.h"
#include "sdfModel.h"
#include "sdfTex.h"
#include "sdfVu0.h"
#include "polyManager.h"
#include "effPart.h"
#include "code_r1_00226EC8.h"
#include "sbtlWork.h"
#include "sBtlEff.h"
#include "sBtlUnit.h"

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
/* The fragment parameter head effPCP_ThunderPolyListCreate copies out of a
 * work; only the fragment count is set by hand here. */
typedef struct effPCPBossThunderHead_t
{
    u8 unk00[0x24];
    s32 fragmentCount;
    u8 unk28[0x2C];
} effPCPBossThunderHead_t;

typedef struct effPCPBossThunderParam_t
{
    f32 points[10][4];
    s32 count;
    effPCPBossThunderHead_t head;
} effPCPBossThunderParam_t;

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
    void *hFlash;
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

typedef struct effCrackMotion_t
{
    u8 unk00[0x20];
    f32 frameStep;
    u8 unk24[0xA];
    u16 frameCount;
} effCrackMotion_t;

typedef struct effCrackModel_t
{
    u8 unk00[0x18];
    void *inner;
    effCrackMotion_t *first;
} effCrackModel_t;

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
            effPCPBossThunderHead_t head;
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
    void *polyLists[2];
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

extern u8 dds3RenderTargetVector[], dds3RenderEyeVector[];
extern u32 dds3GetRandA(u8 *name);
extern f32 dds3GetRandFloatA(u8 *name);

#include "sdk_libc.h"

extern void dds3BillboardSetRGBA(void *hBillboard, u32 abgr);
extern void dds3BillboardSetScale(void *hBillboard, f32 x, f32 y);
extern void dds3BillboardSetRotate(void *hBillboard, f32 rot);
extern void SetRotateMatrixY_B(f32 aAngle);
extern s32 effPadVibeFrames[2], effPadVibeRemain[2];
extern u8 effPadVibePower[2], effPadVibeFadeIn[2];
void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos);
extern void mdlGetPosition(mdlObj_t *model);

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

#include "dds3obj.h"
#include "kernel_draw.h"
#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "sdf_lighting.h"
#include "basic.h"
#include "field.h"
#include "newdata.h"
#include "code_r1_0021C768.h"
#include "effBattleMisc.h"
#include "fldFileResolver_00132080.h"

/* Copy one four-float vector from read-only source storage to destination. */
void effCopyVector(f32 destination[4], const f32 source[4]);

typedef struct {
    dds3Effect_PartialMultiFilterParam_t params;
    sdfTexHandle_t *texture;
} EffResourceRectWork; /* 0x28 */

typedef char EffResourceRectWork_size_must_be_0x28[(sizeof(EffResourceRectWork) == 0x28) ? 1 : -1];

#define EFFECT_RING_FULL_TURN (6.2831853f)

typedef char effBlurScaleSlotSizeCheck[sizeof(effBlurScaleSlot_t) == 0x30 ? 1 : -1];

typedef char effBlurScaleWorkSizeCheck[sizeof(effBlurScaleWork_t) == 0x38 ? 1 : -1];


f32 asinf(f32 x);

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 angle);
struct billHandle_t *dds3BillboardGeneral(s32 index);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *work);
void dds3EffectPartialMultiBlurDelete(dds3Effect_PartialMultiBlur_t *owner);
void dds3EffectPartialMultiFilterDelete(EffResourceRectWork *work);
void dds3EffectRippleBlurDelete(effBlurScaleWork_t *work);

/* Store the primary or secondary texture owner and mark its draw state dirty. */

typedef struct effSurfaceGridNode_t {
    u8 pad_00[0x10];
    s32 rows;       // 0x10
    s32 field_14;   // 0x14
    s32 field_18;   // 0x18
    s32 columns;    // 0x1C
    u32 type;       // 0x20
    u128 *buffer;     // 0x24
    u32 *tail;       // 0x28
    sdfMat_t *material; // 0x2C: owned material descriptor
    u128 *queueA;     // 0x30
    u32 *queueB;     // 0x34
    struct sdfMemHandle_t *allocation; // 0x38
} effSurfaceGridNode_t;

typedef char effSurfaceGridNodeSizeCheck[(sizeof(effSurfaceGridNode_t) == 0x3C) ? 1 : -1];

void fileAcquireRecord(bhvrHandle_t *record);

typedef struct poltHandle_t {
    u32 handleCount;
    u32 color;
    f32 opacity;
    u32 kind;
    effSurfaceParams_t params; /* 0x10: copied source header */
    u32 index;
    struct sdfMemHandle_t *handleBuffer; // 0x30
    billHandle_t *resource;
    fileJobPayload_t **jobs;          // 0x38
    struct sdfMemHandle_t *jobAllocation;      // 0x3C
    struct ctexlHandle_t *resourceHolder; // 0x40, released separately from the grid record
    bhvrHandle_t *record; // 0x44: retained grid record table
    u16 count;          // 0x48: initialized to 1; remaining role unknown
} poltHandle_t;

/* Per-animation allocation: positions follow this four-word owner header. */
typedef struct effAnimationState_t {
    f32 *positions;        // 0x00
    u32 textureHandle;     // 0x04
    bhvrHandle_t *record; // 0x08
    struct sdfMemHandle_t *allocation; // 0x0C
} effAnimationState_t;

/* An effect's GS register packet: DMA, VIF and GIF tags, then one register write. */
typedef struct effGsPacket_t {
    u64 dmaTag;
    u64 vifTag;
    u64 gifTag;
    u64 registerList;
    u64 registerValue;
    u64 registerAddress;
} effGsPacket_t;

/* Payload owner shared by the paired effect units. +0x3C remains opaque. */
typedef struct effCopiedPayload_t {
    u8 *body;                       /* 0x00 */
    s32 size;                       /* 0x04 */
    u32 slotCount;                  /* 0x08: populated effect/target pairs */
    u32 selectedTargetIndex;        /* 0x0C: chosen target path slot */
    dds3Basic_t *effects[5];      /* 0x10 */
    dds3PathBasicControl_t *targets[5]; /* 0x24 */
    struct sdfMemHandle_t *allocation; /* 0x38 */
    u8 opaque3C[4];
} effCopiedPayload_t;

typedef char effCopiedPayload_size_check[
    (sizeof(effCopiedPayload_t) == 0x40) ? 1 : -1];

/* Callback table for native classes. The units dispatch each slot directly and read payloadSize for sizing. */
typedef struct effClassOps_t {
    void (*initialize)(struct effClassWork_t *work);
    void *(*createResource)(void *source);
    void (*destroyResource)(void *resource);
    void (*update)(struct effClassWork_t *work);
    void (*draw)(struct effClassWork_t *work);
    u32 payloadSize;
} effClassOps_t;

typedef char effClassOpsSizeCheck[(sizeof(effClassOps_t) == 0x18) ? 1 : -1];

#define EFF_PARTICLE_SHARED_PAYLOAD_BYTES 0x98

/* The constructor allocates this 0xAC-byte shared resource.
 * The trailing pointers are owned billboard/reference handles managed by the
 * resource lifecycle. */
typedef struct effParticleSharedResource_t {
    u32 color;
    u32 option;
    s32 state;                      /* 0x08: next frame index; reset to zero, incremented after draw */
    effBehavior_RGBParams_t colorTrack;       /* 0x0C */
    effBehavior_AlphaParams_t alphaTrack;       /* 0x30: surfaceIndex selects the blend (0..6) */
    effScalarTrack_t scaleTrack;      /* 0x40 */
    effScalarTrack_t rotationTrack;   /* 0x6C */
    s32 limit;                      /* 0x98: exclusive frame limit; 0 = unlimited */
    s32 x;                          /* 0x9C */
    s32 y;                          /* 0xA0 */
    struct billHandle_t *billHandle;     /* 0xA4 */
    struct ctexlHandle_t *reference; /* 0xA8 */
} effParticleSharedResource_t;

typedef char effParticleSharedResourceSizeCheck[
    (sizeof(effParticleSharedResource_t) == 0xAC) ? 1 : -1];

typedef struct effEmitterRow_t {
    s32 age;    /* 0x00 */
    f32 speed;  /* 0x04 */
    f32 angle;  /* 0x08 */
    f32 radius; /* 0x0C */
    f32 offset; /* 0x10 */
    f32 width;  /* 0x14 */
} effEmitterRow_t;

typedef char effEmitterRowSizeCheck[(sizeof(effEmitterRow_t) == 0x18) ? 1 : -1];

/* Lens-flare constructors copy 0x40 bytes into a 0x58-byte owner at +0x18.
 * Draw callbacks read strength with LWC1,
 * compare frame/limit signed, and select the flare set with LBU at +0x54. */
typedef struct effLensFlareParams_t {
    effBehavior_RGBParams_t colorCurve;
    effBehavior_AlphaParams_t alphaCurve;
    f32 strength;
    s32 limit;
    u8 flareSet;
    u8 pad3D[3];
} effLensFlareParams_t;

typedef struct effFadeVectorWork_t {
    f32 vector[4];
    s32 frame;
    u32 color;
    effLensFlareParams_t source;
} effFadeVectorWork_t;

typedef char effLensFlareParamsSizeCheck[(sizeof(effLensFlareParams_t) == 0x40) ? 1 : -1];

typedef char effFadeVectorWorkSizeCheck[(sizeof(effFadeVectorWork_t) == 0x58) ? 1 : -1];

/* The 0x20-byte strip file header copied into effStripNode_t at +0x0C. */
typedef struct effStripParams_t {
    u8 flag;
    u8 pad01[3];
    u32 innerColor;
    u32 outerColor;
    f32 crossSize;
    f32 longSize;
    f32 sideSize;
    f32 billSize;
    u32 tint;
} effStripParams_t;

typedef char effStripParamsSizeCheck[(sizeof(effStripParams_t) == 0x20) ? 1 : -1];

/* PartsCreate allocates 0xE8 bytes and copies this 0x98-byte
 * parameter record at +0x30. Draws scale the +0x64 track's result and use the +0x90 track as the angle. */
typedef struct effQuadParams_t {
    effBehavior_RGBParams_t colorTrack;
    effBehavior_AlphaParams_t alphaTrack;
    effScalarTrack_t sizeTrack;
    effScalarTrack_t angleTrack;
    s32 duration;
    u32 sizeDuration;
    u8 noSetup;
    u8 pad95[3];
} effQuadParams_t;

typedef struct ptexHandle_t {
    f32 pos[4];
    f32 rot[4];
    f32 scale;
    u32 abgr;
    u32 type;
    s32 frame;
    effQuadParams_t param;
    u8 unkC8[4];
    s8 unkCC; /* Render reads it signed */
    u8 padCD[3];
    struct billHandle_t *hBillboard;
    struct ctexlHandle_t *hCTexList;
    sdfMat_t *assetHandle;
    s32 unkDC;
    s32 unkE0;
    s32 unkE4;
} ptexHandle_t;

typedef char effQuadParamsSizeCheck[(sizeof(effQuadParams_t) == 0x98) ? 1 : -1];

typedef char ptexHandleSizeCheck[(sizeof(ptexHandle_t) == 0xE8) ? 1 : -1];

typedef struct effBillOutputHeader_t {
    effBillTimedHeader_t timed;
    u8 outputMode;
    u8 pad3D[3];
} effBillOutputHeader_t;

typedef struct effBillFrameHeader_t {
    effBillOutputHeader_t output;
    u8 unk40[4];
    f32 fadeInFraction;
    f32 fadeOutFraction;
    s32 lifetime;
    s32 spawnCount;
    u8 respawn;
    u8 spawnAllOnStart;
    u8 mode;
    u8 pad57;
} effBillFrameHeader_t;

/* Resource kinds 1..8 copy the following
 * record lengths. Unrecovered bytes remain serialized data. */
/* billAdvanceFrameInstances consumes the kind-1 geometry tail. */
typedef struct effBillFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 innerColor;
    u32 outerColor;
    f32 innerWidth;
    f32 outerWidth;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillFrameConfig_t; /* kind 1, 0x80 */

typedef struct effBillCellConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;
    u32 colorB;
    f32 extent;
    u8 unk64[4];
    f32 extentJitter;
    f32 radius;
    f32 radiusJitter;
    f32 centerOffset;
    f32 centerOffsetJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad89[3];
} effBillCellConfig_t; /* kind 2, 0x8C */

/* The constructor allocates this 0x3C-byte strip owner.
 * File constructors copy a 0x20-byte source header into its +0x0C member. */
typedef struct effStripNode_t {
    u32 percent;
    u32 color;
    f32 opacity;
    effStripParams_t copiedHeader;
    struct effTrackSet_t *trackSet;
    struct billHandle_t *resource;
    struct bhvrHandle_t *active;
    u16 count;
} effStripNode_t;

typedef char effStripNodeSizeCheck[(sizeof(effStripNode_t) == 0x3C) ? 1 : -1];

/* Resource kinds 1..8 allocate sixteen bytes before their entry array. */
typedef struct effBillFrameState_t {
    u8 *entries;
    effTrackSet_t *asset;
    struct sdfMemHandle_t *allocation;
    u32 unk0C;
} effBillFrameState_t;

typedef struct effBillCellEntry_t {
    s32 age;
    f32 velocity;
    f32 angle;
    f32 radiusA;
    f32 radiusB;
    f32 centerOffset;
    f32 extent;
} effBillCellEntry_t;

/* Kind-3 particle entry (0x2C bytes), the array behind effBillFrameState_t.entries. */
typedef struct effBillParticleEntry_t {
    s32 age;        /* -1 waiting to spawn, -2 retired, otherwise frames alive */
    f32 rise;
    f32 spinRate;
    f32 angle;
    f32 radiusA;
    f32 radiusB;
    f32 offset;
    f32 offsetStep;
    f32 extent;
    u8 pad24[8];
} effBillParticleEntry_t;

typedef struct effBillParticleConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;             /* 0x58 */
    u32 colorB;             /* 0x5C */
    f32 extent;             /* 0x60 */
    u8 pad64[4];
    f32 extentJitter;       /* 0x68 */
    f32 radius;             /* 0x6C */
    f32 radiusJitter;       /* 0x70 */
    f32 offset;             /* 0x74 */
    f32 offsetJitter;       /* 0x78 */
    f32 endOffset;          /* 0x7C */
    f32 endOffsetJitter;    /* 0x80 */
    f32 spinRate;           /* 0x84 */
    f32 spinRateJitter;     /* 0x88 */
    f32 acceleration;       /* 0x8C */
    f32 rise;               /* 0x90 */
    f32 riseJitter;         /* 0x94 */
    f32 gravity;            /* 0x98 */
} effBillParticleConfig_t; /* kind 3, 0x9C */

typedef struct effBillAnimatedFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    u32 drawProgress; /* 0x70: frames over which the radius grows; at least 1 */
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillAnimatedFrameConfig_t; /* kind 4, 0x80 */

typedef struct effBillEmitterFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 firstColor;     /* 0x58 */
    u32 secondColor;    /* 0x5C */
    f32 width;          /* 0x60 */
    f32 widthJitter;    /* 0x64 */
    f32 radius;         /* 0x68 */
    f32 radiusJitter;   /* 0x6C */
    f32 offset;         /* 0x70 */
    f32 offsetJitter;   /* 0x74 */
    f32 speed;          /* 0x78 */
    f32 speedJitter;    /* 0x7C */
    f32 acceleration;  /* 0x80 */
    u8 reverse;         /* 0x84 */
    u8 padding85[3];
} effBillEmitterFrameConfig_t; /* kind 5, 0x88 */

/* Kind-6 radial strip simulation: copied 0xA4-byte configuration. */
typedef struct effBillStripFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 lengthStart;
    f32 lengthStartJitter;
    f32 lengthEnd;
    f32 lengthEndJitter;
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 tiltStart;
    f32 tiltStartJitter;
    f32 tiltEnd;
    f32 tiltEndJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillStripFrameConfig_t; /* kind 6, 0xA4 */

typedef struct effBillStripEntry_t {
    s32 age;
    f32 angle;
    f32 spin;
    f32 length;
    f32 lengthStep;
    f32 radius;
    f32 radiusStep;
    f32 tilt;
    f32 tiltStep;
    f32 width;
} effBillStripEntry_t;

typedef char effBillStripEntrySizeCheck[(sizeof(effBillStripEntry_t) == 0x28) ? 1 : -1];

/* Kind-7 trails share each randomized width factor across their two ends. */
typedef struct effBillTrailFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;
    u32 colorB;
    f32 widthA;
    f32 widthB;
    f32 widthJitter;
    f32 lengthStart;
    f32 lengthStartJitter;
    f32 lengthEnd;
    f32 lengthEndJitter;
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 tiltStart;
    f32 tiltStartJitter;
    f32 tiltEnd;
    f32 tiltEndJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillTrailFrameConfig_t; /* kind 7, 0xA8 */

/* Kind-8 quad updates read these
 * parameters from the copied 0x98-byte record and advance 0x20-byte entries. */
typedef struct effBillQuadFrameConfig_t {
    effBillFrameHeader_t frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    f32 heightStart;
    f32 heightStartJitter;
    f32 heightEnd;
    f32 heightEndJitter;
    f32 tilt;
    f32 tiltJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad95[3];
} effBillQuadFrameConfig_t; /* kind 8, 0x98 */

typedef struct effBillQuadEntry_t {
    s32 timer;
    f32 velocity;
    f32 angle;
    f32 radius;
    f32 height;
    f32 heightStep;
    f32 tilt;
    f32 width;
} effBillQuadEntry_t;

typedef struct effBillRangeConfig_t {
    effBillPointConfig_t point;
    u8 tilted;
    u8 pad89[3];
    f32 startBase;
    f32 startRand;
    f32 endBase;
    f32 endRand;
    f32 heightScale;
    f32 angularBase;
    f32 angularRand;
    f32 angularAcceleration;
} effBillRangeConfig_t;

/* The resource-op rows copy 0xF8/0xF4/0xD8 bytes for vortex/column/spiral.
 * The animated flame row copies 0x10C bytes for the animated flame. Kind is local to its operation
 * table; effClassWork_t.kind together with that table selects the record. */
typedef struct effBillEmitterHeader_t {
    effBillTimedHeader_t timed;
    effBehavior_RGBParams_t emitterColorTrack;
    effBehavior_AlphaParams_t emitterAlphaTrack;
    u8 pad70[8];
    f32 fadeIn;
    f32 fadeOut;
    u32 life;
    u32 spawnRate;
    u8 respawn;
    u8 pad89[3];
    u32 segments;
} effBillEmitterHeader_t;

typedef struct effBillEmitterCommon_t {
    effBillEmitterHeader_t header;
    f32 rowRadius;
    u8 pad94[4];
    f32 height;
    f32 heightJitter;
    u8 padA0[4];
    f32 rise;
    /* Angular sweep for vortex/spiral; longitudinal extent for column. */
    f32 extent;
    f32 extentJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 drawMode;
    u8 padBA[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
} effBillEmitterCommon_t;

typedef struct effBillVortexConfig_t {
    effBillEmitterCommon_t common;
    f32 climbStart;
    f32 climbStartJitter;
    f32 climbEnd;
    f32 climbEndJitter;
    u8 padDC[4];
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillVortexConfig_t;

typedef struct effBillColumnConfig_t {
    effBillEmitterCommon_t common;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
} effBillColumnConfig_t;

typedef struct effBillSpiralConfig_t {
    effBillEmitterCommon_t common;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} effBillSpiralConfig_t;

typedef struct effBillFlameConfig_t {
    effBillEmitterHeader_t header;
    effScalarCurve_t scaleCurve;
    u8 padB4[5];
    u8 drawMode;
    u8 padBA[2];
    f32 spreadA;
    f32 spreadB;
    f32 driftA;
    f32 driftB;
    f32 length;
    f32 lengthJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 meshMode;
    u8 padDE[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
} effBillFlameConfig_t;

/* Shared strip resource owner used by the flame updater and the existing
 * quantized-row producers in both games. */
typedef struct effStripWork_t {
    u32 count;          // 0x00
    u32 type;           // 0x04
    u32 color;          // 0x08
    s32 rowStride;      // 0x0C
    s32 repeat;         // 0x10
    u8 flag;            // 0x14
    u8 pad_15[3];
    u32 *colors;        // 0x18
    u8 *positions;      // 0x1C
    u8 *uvsA;           // 0x20
    u8 *uvsB;           // 0x24
    u8 *extra;          // 0x28
    sdfMat_t *handle;   // 0x2C
    struct sdfMemHandle_t *allocation; // 0x30
} effStripWork_t;

/* The class operation table selects the interpretation of each 0x30-byte
 * entry. Vortex (002ABDE0/002EF1C0), column (002ACA40/002EFE20) and flame
 * (002AEC60/002F2050) share the timer/radius/spin prefix, but use the
 * +0x10/+0x14 and +0x24..+0x2C words differently. Their allocators and
 * reset callbacks use the same record stride and timer sentinel. */
typedef struct effBillEmitterEntry_t {
    s32 timer;
    u8 unk04[4];
    f32 radius;
    f32 radiusStep;
    union {
        struct {
            f32 climb;
            f32 climbStep;
        } vortex;
        struct {
            f32 drag;
            f32 positionY;
        } column;
        struct {
            f32 drag;
            f32 velocity;
        } flame;
    } vertical;
    f32 spin;
    f32 angle;
    f32 width;
    union {
        struct {
            f32 sweep;
            f32 flare;
            f32 height;
        } vortex;
        struct {
            f32 length;
            f32 flare;
            f32 height;
        } column;
        struct {
            f32 length;
            f32 baseA;
            f32 baseB;
        } flame;
    } geometry;
} effBillEmitterEntry_t;

typedef struct effBillRadialConfig_t {
    effBillPointConfig_t point;
    f32 radius;
} effBillRadialConfig_t;

/* Animation resource kind 4 copies this 0x98 record.
 * Samples are consumed as an unsigned allocation count and a signed row bound. */
typedef struct effBillQuantizedConfig_t {
    effBehavior_RGBParams_t colorTrack;
    effBehavior_AlphaParams_t alphaTrack;
    effScalarCurve_t scaleCurve;
    u8 pad58[0x10];
    f32 alphaFadeInFraction;
    f32 alphaFadeOutFraction;
    u32 drawProgress;
    union {
        u32 quantizedSamples;
        s32 signedRows;
    } samples;
    f32 fadeInEnd;
    f32 fadeOutStart;
    f32 uvARowScale;
    f32 uvBVertexScale;
    f32 rowOffset;
    f32 radius;
    f32 angularSpanDegrees;
    u8 meshMode;
    u8 pad95[3];
} effBillQuantizedConfig_t;

typedef char effBillOutputHeaderSizeCheck[(sizeof(effBillOutputHeader_t) == 0x40) ? 1 : -1];

typedef char effBillFrameHeaderSizeCheck[(sizeof(effBillFrameHeader_t) == 0x58) ? 1 : -1];

typedef char effBillFrameConfigSizeCheck[(sizeof(effBillFrameConfig_t) == 0x80) ? 1 : -1];

typedef char effBillCellConfigSizeCheck[(sizeof(effBillCellConfig_t) == 0x8C) ? 1 : -1];

typedef char effBillParticleConfigSizeCheck[(sizeof(effBillParticleConfig_t) == 0x9C) ? 1 : -1];

typedef char effBillParticleEntrySizeCheck[(sizeof(effBillParticleEntry_t) == 0x2C) ? 1 : -1];

typedef char effBillAnimatedFrameConfigSizeCheck[(sizeof(effBillAnimatedFrameConfig_t) == 0x80) ? 1 : -1];

typedef char effBillEmitterFrameConfigSizeCheck[(sizeof(effBillEmitterFrameConfig_t) == 0x88) ? 1 : -1];

typedef char effBillStripFrameConfigSizeCheck[(sizeof(effBillStripFrameConfig_t) == 0xA4) ? 1 : -1];

typedef char effBillTrailFrameConfigSizeCheck[(sizeof(effBillTrailFrameConfig_t) == 0xA8) ? 1 : -1];

typedef char effBillQuadFrameConfigSizeCheck[(sizeof(effBillQuadFrameConfig_t) == 0x98) ? 1 : -1];

typedef char effBillQuantizedConfigSizeCheck[(sizeof(effBillQuantizedConfig_t) == 0x98) ? 1 : -1];

typedef char effBillRangeConfigSizeCheck[(sizeof(effBillRangeConfig_t) == 0xAC) ? 1 : -1];

typedef char effBillEmitterHeaderSizeCheck[(sizeof(effBillEmitterHeader_t) == 0x90) ? 1 : -1];

typedef char effBillEmitterCommonSizeCheck[(sizeof(effBillEmitterCommon_t) == 0xCC) ? 1 : -1];

typedef char effBillVortexConfigSizeCheck[(sizeof(effBillVortexConfig_t) == 0xF8) ? 1 : -1];

typedef char effBillColumnConfigSizeCheck[(sizeof(effBillColumnConfig_t) == 0xF4) ? 1 : -1];

typedef char effBillSpiralConfigSizeCheck[(sizeof(effBillSpiralConfig_t) == 0xD8) ? 1 : -1];

typedef char effBillFlameConfigSizeCheck[(sizeof(effBillFlameConfig_t) == 0x10C) ? 1 : -1];

typedef char effBillRadialConfigSizeCheck[(sizeof(effBillRadialConfig_t) == 0x8C) ? 1 : -1];

typedef char effBillQuadEntrySizeCheck[(sizeof(effBillQuadEntry_t) == 0x20) ? 1 : -1];

typedef char effBillEmitterEntrySizeCheck[(sizeof(effBillEmitterEntry_t) == 0x30) ? 1 : -1];

typedef char effStripWorkSizeCheck[(sizeof(effStripWork_t) == 0x34) ? 1 : -1];

/* One 0x30-byte particle state row used by the billboard column updater. */
typedef struct effBillColumnParticle_t {
    s32 age;             /* 0x00: spawn sentinel or elapsed particle frames */
    u32 opaque04;        /* 0x04: preserved state not read by the updater */
    f32 radius;          /* 0x08: current radial distance */
    f32 radiusStep;      /* 0x0C: radial change per frame */
    f32 drag;            /* 0x10: vertical drag applied per frame */
    f32 verticalOffset;  /* 0x14: current vertical position */
    f32 spin;            /* 0x18: angular speed */
    f32 spinPhase;       /* 0x1C: initial angular phase */
    f32 width;           /* 0x20: ribbon width */
    f32 extent;          /* 0x24: ribbon length along its direction */
    f32 flare;           /* 0x28: radial flare amount */
    f32 height;          /* 0x2C: vertical start height, advanced by rise */
} effBillColumnParticle_t;

typedef char effBillColumnParticleSizeCheck[
    (sizeof(effBillColumnParticle_t) == 0x30) ? 1 : -1];

/* Shared 0x34-byte backing record used by the alternate ribbon renderer.
 * surfaceIndex comes from the alpha track; drawMode is the caller's byte
 * mode, which the renderer tests only for zero/nonzero. */
typedef struct effRibbonWork_t {
    u32 count;                 /* 0x00 */
    u32 surfaceIndex;          /* 0x04 */
    u32 color;                 /* 0x08 */
    s32 rowStride;             /* 0x0C */
    s32 repeat;                /* 0x10 */
    u8 drawMode;               /* 0x14 */
    u8 pad_15[3];
    effRefObj_t *resource;          /* 0x18: null selects the shared wind texture */
    u32 *colors;               /* 0x1C */
    u8 *positions;             /* 0x20 */
    u8 *uvs;                    /* 0x24 */
    u8 *extra;                  /* 0x28 */
    sdfMat_t *handle;           /* 0x2C */
    struct sdfMemHandle_t *allocation; /* 0x30 */
} effRibbonWork_t;

effRibbonWork_t *effCreateRibbonWithSharedResource(
    u32 count, u32 repeat, struct sdfTextureFileHeader_t *resource);

effRibbonWork_t *effCloneRibbonWithSharedResource(effRibbonWork_t *source);

void effSharedAssetReferenceRelease(effRibbonWork_t *work);

typedef char effRibbonWorkSizeCheck[(sizeof(effRibbonWork_t) == 0x34) ? 1 : -1];

/* Shared frame and output views consumed by effUpdateScaledClassFrameRows. */
typedef struct effClassFrameResource_t {
    u8 pad_00[8];
    u32 frameCount;
    u8 pad_0C[4];
    u8 *rows;
} effClassFrameResource_t;

typedef struct effClassRowOutput_t {
    u8 pad_00[0x1C];
    u8 *rows;
} effClassRowOutput_t;

typedef char effClassFrameResource_size_check[
    (sizeof(effClassFrameResource_t) == 0x14) ? 1 : -1];

typedef char effClassRowOutput_size_check[
    (sizeof(effClassRowOutput_t) == 0x20) ? 1 : -1];

typedef struct effRingResource_t {
    effPointSet_t *pointSet;
} effRingResource_t;

typedef char effRingResourceSizeCheck[
    (sizeof(effRingResource_t) == 0x04) ? 1 : -1];

effPointSet_t *effCreatePointSet5(s32 count);

effPointSet_t *effCreatePointSet4(u32 count);

void effReleasePointSetAsset(effPointSet_t *set);

typedef struct effPointSetRow_t {
    struct effPointSet_t *set; /* 0x00 */
    s32 key;                 /* 0x04 */
    u32 color;               /* 0x08 */
    f32 angle;               /* 0x0C */
} effPointSetRow_t;

typedef struct effPointSetTable_t {
    effPointSetRow_t *rows;
} effPointSetTable_t;

typedef char effPointSetRowSizeCheck[(sizeof(effPointSetRow_t) == 0x10) ? 1 : -1];

typedef char effPointSetTableSizeCheck[(sizeof(effPointSetTable_t) == 0x04) ? 1 : -1];

/* Effect transform consumed as four float columns or four VU0/DMA rows. */
typedef struct effMatrix4_t {
    union {
        float m[4][4];
        s128 rows[4];
    } u;
} effMatrix4_t;

typedef char effMatrix4_size_must_be_0x40[
    sizeof(effMatrix4_t) == 0x40 ? 1 : -1];

/* Shared billboard/frame resource view. The generic effFrameState_t owner can
 * point at several resource kinds; only cast its asset slot to this record in
 * frame callbacks. The +0x18 pointer is preserved as an opaque pointer because
 * the paired units disagree on its signed element type and do not consume it. */
typedef struct effFrameAsset_t {
    u8 pad00[8];
    u32 frameCount;          /* 0x08 */
    u8 pad0C[0x0C];
    void *animationFrames;   /* 0x18 */
    u32 *frameStorage;       /* 0x1C: per-entry frame counters for bill effects. */
    f32 *transformRows;      /* 0x20 */
    u32 pad24;
    u32 *colorRows;          /* 0x28 */
} effFrameAsset_t;

typedef char effFrameAssetSizeCheck[
    (sizeof(effFrameAsset_t) == 0x2C) ? 1 : -1];

typedef char effRandState_size_must_be_0x10[
    (sizeof(effRandState_t) == 0x10) ? 1 : -1];

/* Animated billboard effect state. */
typedef struct effBillboardWork_t {
    sdfVector4_t position;
    f32 rotation[4];
    f32 scale;
    u32 color;
    s32 frame;
    f32 heightScale;
    f32 widthScale;
    u8 opaque34[0x30];
    struct billHandle_t *billboard;
} effBillboardWork_t;

typedef char effBillboardWork_size_must_be_0x68[
    (sizeof(effBillboardWork_t) == 0x68) ? 1 : -1];

extern void evtDestroySelectionState(void);
extern s32 evtSelStateCreate(s32 countLimit, s16 frames, s32 firstValue, s32 secondValue);

static inline void effSubmitSurfacePacket(sdfGrOt_t *surface, sdfDrawTag_t *list) {
    surface->regFunc(surface, list);
}

extern u32 effDrawFivePointGroupsData[];
extern sdfGrOt_t *effDrawFivePointGroupsData2[];
extern sdfGrOt_t *effDrawThreePointGroupsData[];
extern u32 effData7[];
extern char effStrToolEffectEp[];
extern u8 *effAllocateTexturedStripWork(u32 count, u32 repeat);
extern u32 effScalyTextureHandle;
extern s32 effSharedStripReferenceCount;
extern effRefObj_t *effSharedScalyStripResource;
extern u32 effWindTextureHandle;
extern void *effLoadWindTextureData;
extern struct sdfMemHandle_t *effLoadScalyTextureData;
extern u8 *effSubmitIndexedRenderPacketData[];
extern struct sdfDrawTag_t *effCurrentRenderPacket;
extern s32 effSharedRibbonReferenceCount;
extern u32 effSharedAssetReferenceReleaseData;
extern struct ctexlHandle_t *effCacheTexCreate(u32);
extern struct sdfTextureFileHeader_t *effFlashTextureHandles;
extern struct effModelResource_t *Create_effPolygonCrack(u16, void *, void *, u32);
extern effSurfaceGridNode_t *effCreateSurfaceGridNode(u32, u32);
extern void effFillSurfaceGridColorGradient(effSurfaceGridNode_t *, u32 *);
extern void effReleaseSurfaceGridBuffers(effSurfaceGridNode_t *);
extern sdf3DPrimPolyParm_t effDrawTrackSetWork;
extern effTrackSet_t *effCreateTrackSetWithSharedReferences(u32, u32, u32);
extern void effReleaseResourceRefs(effTrackSet_t *);
extern f32 mnuMeasureProjectedPerpendicularDistance(f32);

typedef struct effObjectFlag_t {
    u32 state;
    u8 pad04[8];
    u32 flags;
} effObjectFlag_t;

extern u8 btlIsRuntimeAllocated(void);
void effDrawThreePointGroups(effPointSet_t *, effMatrix4_t *);
extern s32 effCreateTrackSetWithSharedReferencesData2[2];
extern effRefObj_t *effData34[2];
extern s32 effPadVibeFrames[2];
extern s32 effPadVibeRemain[2];
extern u8 effPadVibeFadeIn[2];

/* Native resource operations add cloning before the frame callbacks.
 * The final word is the copied payload size, not another callback.
 * Factories retain this unit's existing address-word interface. */
typedef struct effResourceOps_t {
    void (*initialize)();      /* 0x00 */
    u32 (*createResource)();    /* 0x04 */
    void (*destroyResource)(); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)();          /* 0x10 */
    void (*draw)();            /* 0x14 */
    u32 payloadSize;           /* 0x18 */
} effResourceOps_t;

/* Active frame callbacks receive the parent; destruction receives its child. */
typedef struct effActiveInstanceOps_t {
    void (*initialize)(effClassWork_t *); /* 0x00 */
    u32 (*createResource)(); /* 0x04 */
    void (*destroyResource)(void *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C */
    void (*update)(effClassWork_t *); /* 0x10 */
    void (*draw)(effClassWork_t *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} effActiveInstanceOps_t;

typedef char effActiveInstanceOpsSizeCheck[(sizeof(effActiveInstanceOps_t) == 0x1C) ? 1 : -1];

/* Class-resource callbacks operate on the parent class-work owner. */
typedef struct effClassResourceOps_t {
    void (*initialize)(effClassWork_t *); /* 0x00 */
    void *(*createResource)(effBillPointConfig_t *); /* 0x04 */
    void (*destroyResource)(effClassWork_t *); /* 0x08 */
    void (*update)(effClassWork_t *); /* 0x0C */
    void (*draw)(effClassWork_t *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} effClassResourceOps_t;

typedef char effClassResourceOps_size_must_be_0x18[
    (sizeof(effClassResourceOps_t) == 0x18) ? 1 : -1];

/* Block-resource callbacks receive the owning class work, not the payload. */
typedef struct effBlockOps_t {
    void (*initialize)(effClassWork_t *); /* 0x00 */
    u32 (*createResource)(); /* 0x04: preserve the existing generic result ABI */
    void (*destroyResource)(effClassWork_t *); /* 0x08 */
    u32 (*cloneResource)(void *); /* 0x0C: preserve the existing generic result ABI */
    void (*update)(effClassWork_t *); /* 0x10 */
    void (*draw)(effClassWork_t *); /* 0x14 */
    u32 payloadSize; /* 0x18 */
} effBlockOps_t;

typedef char effBlockOps_size_must_be_0x1C[
    (sizeof(effBlockOps_t) == 0x1C) ? 1 : -1];

struct effModelResource_t;

struct effSpanConfig_t;

struct effSpanTable_t;

/* This table has a single populated kind with span-resource callbacks. */
typedef struct effModelResourceOps_t {
    void (*initialize)(struct effModelResource_t *); /* 0x00 */
    struct effSpanTable_t *(*createResource)(struct effSpanConfig_t *, mdlObj_t *); /* 0x04 */
    void (*destroyResource)(struct effSpanTable_t *); /* 0x08 */
    void (*update)(struct effModelResource_t *); /* 0x0C */
    void (*draw)(struct effModelResource_t *); /* 0x10 */
    u32 payloadSize; /* 0x14 */
} effModelResourceOps_t;

typedef char effModelResourceOps_size_must_be_0x18[
    (sizeof(effModelResourceOps_t) == 0x18) ? 1 : -1];

extern effActiveInstanceOps_t effActiveInstanceOperations[];
extern effClassOps_t effClassWorkOperations[];
extern effClassResourceOps_t effClassResourceWorkOperations[];
extern effBlockOps_t effBlockResourceOperations[];
extern effBlockOps_t effModelBlockOperations[];
extern effModelResourceOps_t effModelResourceOperations[];

/* Shared work object for the effEditScalarTrack helpers (layout inferred). */
typedef struct effBaObj_t {
    u8 pad_0x00[0x0C]; // 0x00
    u8 *x0C;           // 0x0C
} effBaObj_t; // 0x10

struct effCameraCreateRow_t;

extern char effStrPolyTrack[];
extern char effStrEnvironment[];
extern char effStrPolyFlash[];
extern char effStrPolyRing[];
extern char effStrPolyThunder[];
extern char effStrObjParticle[];
extern char effStrPolyScaly[];
extern char effStrPolyCrack[];

/* Value holder with a float field and a u16 data pointer (layout inferred). */
typedef struct effValPtr44_t {
    u8 pad_0x00[0x08]; // 0x00
    float f08;         // 0x08
    u8 pad_0x0C[0x38]; // 0x0C
    u16 *p44;          // 0x44
} effValPtr44_t; // 0x48

struct effKindWork_t;

/* Native kind operations include handle destruction and per-frame update;
 * zero-argument notifications retain the original unprototyped callback ABI. */
typedef struct effKindDesc_t {
    void *(*create)(void *); // 0x00
    void (*destroy)();       // 0x04: release the created handle
    void (*update)();        // 0x08: advance work through its kind callback
    void (*initialize)(struct effKindWork_t *, sdfTexHandle_t *); // 0x0C
    u32 payloadSize;         // 0x10: bytes copied after the work header
} effKindDesc_t; // 0x14

/* Native kind work: 0x40-byte header, followed by the copied source payload.
 * Fade callbacks consume this same object: frame supplies their signed limit,
 * handle is the renderer output, and payload holds the curve configuration. */
typedef struct effKindWork_t {
    f32 position[4]; /* 0x00: vector passed to projection routines */
    s32 mode;       // 0x10
    u32 color;      // 0x14
    f32 scale;      // 0x18
    s32 kind;       // 0x1C
    u32 frame;        // 0x20, incremented by the kind callback dispatcher
    void *handle;   // 0x24
    void *payload;  // 0x28
    u32 sourceKind; // 0x2C
    sdfTexHandle_t *target;     // 0x30
    u8 pad_0x34[0xC]; // 0x34
} effKindWork_t; // 0x40

/* Source payload's kind selects the initial rendering mode. */
typedef struct effKindSource_t {
    u8 pad_00[0x28];
    s32 modeKind;
} effKindSource_t;

extern effKindDesc_t effData5[];
extern effKindDesc_t effData6[];

/* VU0 model helpers consume vf10 directly, matching the original macro-mode setup. */
extern void effDrawScalyTexturedStrip(effStripWork_t *, void *);
extern void Init_effModel(mdlObj_t *);
extern s32 effComputeLightDirectionVU(mdlObj_t *, sdf3DLightParallelPacket_t *);
void effDestroyModelContext(mdlObj_t *model);
mdlObj_t *effLoadViewerModelWithVUState(void *modelData, u32 modelDataSize);

typedef struct effFadeHeaderWork_t {
    s32 frame;
    s32 payload[2];
    effQuakeSlot_t *quake;
} effFadeHeaderWork_t;

typedef char effFadeHeaderWorkSizeCheck[(sizeof(effFadeHeaderWork_t) == 0x10) ? 1 : -1];

effFadeHeaderWork_t *effDuplicateSmallHeader(const void *source);
effFadeHeaderWork_t *effCreateSmallHeaderFromFile(void *work);
effFadeHeaderWork_t *effCloneSmallHeaderFromWork(effFadeHeaderWork_t *work);
effFadeVectorWork_t *effCreateFadeVectorWork();
effFadeVectorWork_t *effCreateFadeVectorFromFile(void *work);
void effFreeFadeVectorWork(effFadeVectorWork_t *work);
effFadeVectorWork_t *effCloneFadeVectorWork(effFadeVectorWork_t *work);

typedef struct effSelectionHeaderWork_t {
    s32 frame;
    s32 payload[4];
} effSelectionHeaderWork_t;

typedef char effSelectionHeaderWorkSizeCheck[(sizeof(effSelectionHeaderWork_t) == 0x14) ? 1 : -1];

effSelectionHeaderWork_t *effDuplicatePayloadHeader(const void *source);
effSelectionHeaderWork_t *effCreateSelectionHeaderFromFile(void *work);
void effReleaseSelectionHeaderAllocation(effSelectionHeaderWork_t *allocation);
effSelectionHeaderWork_t *effCloneSelectionHeaderFromWork(effSelectionHeaderWork_t *work);
void effSelectionFrameAdvance(effSelectionHeaderWork_t *counter);

typedef struct effFadeConfig_t {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    effBehavior_RGBParams_t blendA;   /* 0x00 */
    effBehavior_AlphaParams_t blendB2;  /* 0x24 */
    effScalarCurve_t blendB;   /* 0x34 */
    u8 pad58[8];
    effScalarTrack_t rateA;   /* 0x60 */
    effScalarTrack_t rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 padBC[4];
    dds3Effect_Filter_t out; /* 0xC0 */
} effFadeConfig_t;

typedef struct effRateConfig_t {
    /* Color and alpha tracks at 0x00/0x24, followed by three scalar
     * tracks at 0x34, 0x60 and 0x8C. The scalar extents differ. */
    effBehavior_RGBParams_t blendA;   /* 0x00 */
    effBehavior_AlphaParams_t blendB2;  /* 0x24 */
    effScalarCurve_t blendB;   /* 0x34 */
    u8 pad58[8];
    effScalarTrack_t rateA;   /* 0x60 */
    effScalarTrack_t rateB;   /* 0x8C */
    s32 duration;         /* 0xB8 */
    u8 fixedMode;         /* 0xBC */
    u8 padBD[3];
    dds3Effect_Blur_t out;       /* 0xC0 */
} effRateConfig_t;

void effUpdateFadeBlendA(effKindWork_t *work);
void *effCreateFadeBlendWorkFromOutput(void *work);
void effReleaseFadeBlendWork(void *resourceHandle);
void effUpdateProjectedBlurFadeRectangle(effKindWork_t *work);
void effUpdateTarget(effKindWork_t *work, sdfTexHandle_t *target);
void *effCreateFixedSlotBlurWorkFromFadeOutput(void *source);
void effReleaseFixedSlotBlurWork(effBlurScatterWork_t *handle);
void effUpdateFadeMapA(effKindWork_t *work);
void effTextureReferenceRelease(effKindWork_t *work, sdfTexHandle_t *target);
void *effCreateVariableSlotBlurWorkFromFadeOutput(void *source);
void effReleaseVariableSlotBlurWork(effBlurScaleWork_t *work);
void effUpdateFadeMapB(effKindWork_t *work);
void effReplaceKindLinkedTarget(effKindWork_t *work, sdfTexHandle_t *target);
void effUpdateFadeBlendB(effKindWork_t *work);
void effUpdateFadeBlendC(effKindWork_t *work);
void *effCreateFadeColorWorkFromOutput(void *work);
void effReleaseFadeColorWork(EffResourceRectWork *work);

/* This projected fade also consumes effKindWork_t: position, handle and payload. */
extern void effComputeBlurRectBounds(EffResourceRectWork *work);
void effUpdateProjectedFadeColor(effKindWork_t *work);
void effReplaceLinkedKindWorkTarget(effKindWork_t *work, sdfTexHandle_t *target);
effKindWork_t *effAllocateKindWork(u16 kind, u8 *source);
effKindWork_t *effCreateKindWorkFromFile(fileJobPayload_t *work);
void effReleaseLinkedTarget(effKindWork_t *work);
effKindWork_t *effCloneKindWork(effKindWork_t *work);
effKindWork_t *effAllocateAlternateKindWork(u16 kind, u8 *source);
effKindWork_t *effCreateKindWorkFromFileB(fileJobPayload_t *work);
void effReleaseAlternateKindWork(effKindWork_t *work);
effKindWork_t *effCloneAlternateKindWork(effKindWork_t *work);
u8 *Create_effMuzzleFlash(fileJobPayload_t *source);
void Delete_effMuzzleFlash(effBillboardWork_t *work);
void ResourceCopy_effMuzzleFlash(s32 dst, s32 src);
u8 *Copy_effMuzzleFlash(const u8 *source);
void effBillboardEntryFrameReset(effBillboardWork_t *work);

/* Shared 0xC per-instance prefix: entries, asset view and allocation handle. */
typedef struct effFrameState_t {
    u8 *entries;
    u8 *asset;
    struct sdfMemHandle_t *allocation;
} effFrameState_t;

typedef char effFrameState_size_check[
    (sizeof(effFrameState_t) == 0xC) ? 1 : -1];

void effClearBillFrames(u8 *work);
u8 *effCreateBillFrameNode(effBillFrameConfig_t *config, u32 resource);
void effReleaseBillFrameNode(s32 work);

typedef struct effBillFrameEntry_t {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 innerWidth;
    f32 outerWidth;
} effBillFrameEntry_t;

typedef char effBillFrameEntrySizeCheck[(sizeof(effBillFrameEntry_t) == 0x18) ? 1 : -1];

extern effRandState_t effSharedRandomState[];
extern u8 effOnesVector[];
void billAdvanceFrameInstances(effClassWork_t *work);
extern void effDrawTrackSet(effTrackSet_t *, const effMatrix4_t *);

typedef struct billCellDrawWork_t {
    u8 opaque00[0x10];
    u8 transform[0x10];
    f32 scale;
    s32 baseColor;
    u32 frameLimit;
    u8 opaque2C[4];
    u32 *instances;
    u8 *config;
} billCellDrawWork_t;

typedef char billCellDrawWorkSizeCheck[(sizeof(billCellDrawWork_t) == 0x38) ? 1 : -1];

void billUpdateFrameDrawColorAndTransform(billCellDrawWork_t *work);
void billResetCellIndices(u8 *work);
u8 *billCreateCellNode(effBillCellConfig_t *config, u32 resource);
void billReleaseCellNode(s32 work);
extern f32 billRingCosines[12];
extern f32 billRingSines[12];
void billAdvanceCellInstances(effClassWork_t *work);
void billUpdateCellDrawColorAndTransform(billCellDrawWork_t *work);
void billResetParticleIndices(u8 *work);
u8 *billCreateParticleNode(effBillParticleConfig_t *config, u32 resource);
void billReleaseParticleNode(s32 work);
void billUpdateParticleDrawColorAndTransform(billCellDrawWork_t *work);
void effClearAnimatedFrames(u8 *work);
u8 *billAllocateAnimatedTransformEntries(effBillAnimatedFrameConfig_t *config);
void effInitializeAlternatingTransformRows(u8 *node, u8 *config);
u8 *billCreateAnimatedTransform(effBillAnimatedFrameConfig_t *config, u32 resource);
extern u8 *billAllocEmitterNode(u8 *);
extern u8 *billAllocQuadNode(u8 *);
extern effTrackSet_t *effDuplicateResourceRefs(const effTrackSet_t *);
extern void billInitializeEmitterRows(u8 *work, u8 *descriptor);
extern void billInitializeQuadRows(u8 *work, u8 *descriptor);
u8 *billCloneAnimatedTransform(u8 *work);
void billReleaseAlternatingTransformNode(s32 work);

/* Kind-4 animated frame entry: the radius grows by radiusStep each frame
 * for the config's drawProgress frames. */
typedef struct effBillAnimatedEntry_t {
    s32 age;
    f32 spin;
    f32 angle;
    f32 radius;
    f32 radiusStep;
    f32 width;
} effBillAnimatedEntry_t;

void billAdvanceAnimatedFrameTransforms(effClassWork_t *work);
void billUpdateAlternatingDrawColorAndTransform(billCellDrawWork_t *work);
void billResetEmitterIndices(u8 *work);
u8 *billAllocEmitterNode(u8 *config);
void billInitializeEmitterRows(u8 *work, u8 *descriptor);
u8 *billCreateEmitterTransform(effBillEmitterFrameConfig_t *config, u32 resource);
u8 *billCloneEmitterTransform(u8 *work);
void billReleaseEmitterNode(s32 work);
void billAdvanceEmitterInstances(effClassWork_t *work);
void billUpdateEmitterDrawColorAndTransform(u8 *work);
void effClearStripFrames(u8 *work);
u8 *billAllocStripNode(u8 *config);
void billInitializeStripRows(u8 *node, u8 *config);
extern u8 *billAllocStripNode(u8 *);
u8 *billCreateStripTransform(effBillStripFrameConfig_t *config, u32 resource);
u8 *billCloneStripTransform(u8 *work);
void billReleaseStripNode(s32 work);
void billAdvanceStripInstances(effClassWork_t *work);
void billUpdateStripDrawColorAndTransform(u8 *work);
void billResetTrailIndices(u8 *work);
u8 *billCreateTrailNode(effBillTrailFrameConfig_t *config, u32 resource);
void billReleaseTrailNode(s32 work);
void billUpdateTrailDrawColorAndTransform(u8 *work);
void billResetQuadIndices(u8 *work);
u8 *billAllocQuadNode(u8 *config);
void billInitializeQuadRows(u8 *work, u8 *descriptor);
u8 *billCreateQuadTransform(effBillQuadFrameConfig_t *config, u32 resource);
u8 *billCloneQuadTransform(u8 *work);
void billReleaseQuadNode(s32 work);
void billAdvanceQuadInstances(effClassWork_t *work);
void billUpdateQuadDrawColorAndTransform(u8 *work);

/* Four-byte owner header followed by its class-work pointer array. */
typedef struct effClassWorkList_t {
    effClassWork_t **entries;
} effClassWorkList_t;

effClassWork_t *effAllocateActiveInstanceWork(u16 kind, void *source);

/* The ring source selects a minimum of three segments and repeats its three colors. */
typedef struct effRingSource_t {
    u8 pad_00[0x38];
    u32 segments;       // 0x38
    u8 pad_3C[8];
    u32 firstColor;     // 0x44
    u32 middleColor;    // 0x48
    u32 lastColor;      // 0x4C
} effRingSource_t;

/* Class kind 3 copies this complete 0x68-byte serialized parameter record.
 * The color/alpha prefix is the existing interpolation provider's layout;
 * alphaTrack.surfaceIndex at 0x28 also selects the point-set draw type. */
typedef struct effRadialRingParams_t {
    effBehavior_RGBParams_t colorTrack; /* 0x00 */
    effBehavior_AlphaParams_t alphaTrack; /* 0x24 */
    u32 duration;            /* 0x34 */
    u32 segments;            /* 0x38 */
    u8 flag;                 /* 0x3C */
    u8 pad3D[3];
    f32 baseRadius;          /* 0x40 */
    u32 firstColor;          /* 0x44 */
    u32 middleColor;         /* 0x48 */
    u32 lastColor;           /* 0x4C */
    f32 widths[3];           /* 0x50 */
    f32 speed;               /* 0x5C */
    f32 acceleration;        /* 0x60 */
    u8 reverseTime;          /* 0x64 */
    u8 pad65[3];
} effRadialRingParams_t;

typedef char effRadialRingParams_size_must_be_0x68[(sizeof(effRadialRingParams_t) == 0x68) ? 1 : -1];

effClassWork_t *effCreateResourceInstanceA(u16 kind, void *source, u32 option);
effClassWork_t *effCreateActiveInstanceFromFile(fileJobPayload_t *request);
void effDestroyActiveInstanceWork(effClassWork_t *work);
effClassWork_t *effCreateActiveResource(effClassWork_t *work);
void effResetActiveInstanceFrame(effClassWork_t *work);
void effAdvanceActiveInstanceFrame(effClassWork_t *work);
void effDispatchActiveInstanceDraw(effClassWork_t *work);
effTrackSet_t *effCreateTrackSet(s32 count, u16 kind);
extern struct sdfTextureFileHeader_t *effCreateTrackSetWithSharedReferencesData;
effTrackSet_t *effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef);
void effReleaseResourceRefs(effTrackSet_t *refs);
effTrackSet_t *effDuplicateResourceRefs(const effTrackSet_t *original);
extern sdfTexHandle_t *sdfGetOrCreateTextureForReference(sdfGrOt_t *, effRefObj_t *);

struct sdfMat_t;

/* Draw track-set positions/colors/UVs; a null matrix selects the identity transform. */
void effDrawTrackSet(effTrackSet_t *track, const effMatrix4_t *matrix);
extern struct sdfMemHandle_t *effLoadFlashTexturesData[2];
void effLoadFlashTextures(void);

typedef struct effCounterHeader_t {
    u32 unk00;
    u32 frame;
} effCounterHeader_t;

/* Class draw modes own either a counter header or per-segment scales. */
typedef struct effClassDrawState_t {
    union {
        effCounterHeader_t *ring;
        f32 *scales;
    };
    effClassWork_t *effect;
    effTrackSet_t *trackSet;
    struct sdfMemHandle_t *allocation;
} effClassDrawState_t;

u32 *effSegmentPointerSet(u8 *work);
extern void effAssetQueueRelease(effPointSet_t *);
void effReleaseRingResourceHandle(u32 *handle);

/* Class 1 ring parameters have a distinct 0x6C-byte copied layout. */
typedef struct effExpandingRingParams_t {
    effBehavior_RGBParams_t colorTrack;
    effBehavior_AlphaParams_t alphaTrack;
    s32 duration;
    u32 segments;
    u8 flag;
    u8 pad3D[3];
    f32 baseRadius;
    u32 colors[3];
    f32 widths[3];
    u32 unk5C;
    f32 speed;
    f32 acceleration;
    u8 reverseTime;
    u8 pad69[3];
} effExpandingRingParams_t;

typedef char effExpandingRingParams_size[(sizeof(effExpandingRingParams_t) == 0x6C) ? 1 : -1];

void effUpdateExpandingRingPoints(effClassWork_t *work);
extern void effDrawFourPointGroups(effPointSet_t *, const effMatrix4_t *);
void billDrawCellBlendA(billCellDrawWork_t *work);
void effResetClassFrameAndFlags(s32 work);

/* The cached class payload is copied from the 0x5C-byte source prefix. */
typedef struct effRingClassConfig_t {
    effRingSource_t ring;
    u8 pad50[0xC];
    u8 classConfig[0x5C];
    u8 padB8[0x10];
    f32 scale;
    f32 scaleRand;
} effRingClassConfig_t;

effClassDrawState_t *effCreateScaledClassDrawState(effRingClassConfig_t *source);
void effReleaseClassDrawResources(s32 work);
extern void effAdvanceClassFrame(effClassWork_t *);
void effUpdateScaledClassFrameRows(billCellDrawWork_t *work);
extern void effRunClassPostFrame(effClassWork_t *);
void billDrawClassUpdatedCellBlend(billCellDrawWork_t *work);
effRingResource_t *effCreateRingHandle(effRadialRingParams_t *work);
void effReleaseRingHandle(effRingResource_t *handle);
void effUpdateCenteredRadialRingVertices(effClassWork_t *work);
void billDrawCellBlendB(effClassWork_t *work);
effClassWork_t *effCreateClassWork(u16 kind, void *source);
effClassWork_t *effCreateClassWorkFromFile(fileJobPayload_t *request);
void effDestroyClassWork(effClassWork_t *work);
effClassWork_t *effCloneClassWork(effClassWork_t *work);
void effInitializeClassFrame(effClassWork_t *work);
void effAdvanceClassFrame(effClassWork_t *work);
void effRunClassPostFrame(effClassWork_t *work);
void effUpdateClassFrame(effClassWork_t *work);
extern sdf3DPrimPolyParm_t effWork[];
void effAssetQueueRelease(effPointSet_t *set);

/* vu0 routine: SDK loads the supplied transform or constructs identity. */
void effDrawFourPointGroups(effPointSet_t *set, const effMatrix4_t *matrix);
poltHandle_t *effCreateSurfaceNode(u32 handleCount);
poltHandle_t *effCreateSurfaceNodeForGrid(fileKeyBlock_t *grid);
extern void effRebuildSurfaceHandles(poltHandle_t *, u16, void *);
void effReplaceResourceRef(poltHandle_t *node, u32 entryId, const void *resource);
poltHandle_t *effResourceReferenceReplaceFromFile(fileJobPayload_t *work);
extern void effRebuildSurfaceJobs(poltHandle_t *, void *);
extern void effReplaceSurfaceResourceHolder(poltHandle_t *node, u32 resource);
void effReplaceSurfacePrimaryBillboard(poltHandle_t *node, u32 resourceId);
void effReplaceSurfaceFlaggedBillboard(poltHandle_t *node, u32 resourceId);
void effSetSurfaceRetainedResource(poltHandle_t *node, u32 resourceId);
poltHandle_t *effInitializeSurfaceForKind(fileJobPayload_t *work);
void effDestroySurfaceNode(poltHandle_t *node);
poltHandle_t *effRecreateSurfaceNodeFromWork(u8 *work);
extern void effCloneSurfaceGridResources(poltHandle_t *, u8 *);
poltHandle_t *effSCopy(u8 *work);
void effCloneSurfaceGridResources(poltHandle_t *dst, u8 *work);
void effRebuildSurfaceHandles(poltHandle_t *node, u16 kind, void *source);
void effRebuildSurfaceJobs(poltHandle_t *node, void *source);
void effReplaceSurfaceResourceHolder(poltHandle_t *node, u32 resource);
void effClearSurfaceNodeRecordReferences(s32 node);
void effAcquireSurfaceRecord(s32 node);
void effMenuRecordVectorSet(s32 node, const u128 *value);
void effSetSurfaceRecordSecondaryVector(s32 node, const u128 *value);
void effIndexedFloatCallbackDispatch(effValPtr44_t *p, float v);
extern sdf3DPrimPolyParm_t effDrawSurfaceTrailHistoryWork;
extern sdf3DPrimPolyParm_t effWork2;
extern u8 effData8[];

/* vu0 routine: grid surface node with rows of three vertex columns */
effSurfaceGridNode_t *effCreateSurfaceGridNode(u32 count, u32 columns);
void effFillSurfaceGridColorGradient(effSurfaceGridNode_t *node, u32 *colors);
void effReleaseSurfaceGridBuffers(effSurfaceGridNode_t *node);
void effResetSurfaceGridFrame(effSurfaceGridNode_t *node);
void effCopyRingFrameVectors(effSurfaceGridNode_t *node, u128 *destination, s32 frame);
void effAppendSurfaceGridVectors(effSurfaceGridNode_t *work, const f32 (*source)[4]);
extern void vuCatmullRomPoint(f32 (*)[4], f32);
void effInterpolateSurfaceGridVectors(effSurfaceGridNode_t *work, const f32 (*vectors)[4]);
void effBeginMatrixVuDrawPacket(const effMatrix4_t *matrix);

/* Point history of a grid-surface trail: a ring of 16-byte points with packed colors,
 * drawn as triangle strips into the current render packet, then a fixed end cap. */
typedef struct effSurfaceTrailHistory_t {
    u8 pad00[0x10];
    s32 pointCapacity;        /* 0x10 */
    s32 activePointCount;     /* 0x14 */
    s32 nextPointWriteIndex;  /* 0x18 */
    u8 pad1C[0x08];
    u128 *points;             /* 0x24 */
    u32 *colors;              /* 0x28 */
    s32 resourceHandle;       /* 0x2C */
    u128 *endPoints;          /* 0x30 */
    u32 *endColors;           /* 0x34 */
} effSurfaceTrailHistory_t;

void effDrawSurfaceTrailHistory(effSurfaceTrailHistory_t *history, u32 color);

typedef struct effDrawEntry_t {
    u32 handle;
    s32 frame;
    u8 pad08[8];
    void (*draw)(u8 *, struct sdfDrawTag_t *);
} effDrawEntry_t;

void effSubmitIndexedRenderPacket(u32 index);
void effRandomizeParticleFields(effClassWork_t *work);
effPointSetTable_t *effCreateAlphaRampPointSetRows(effBillPointConfig_t *src);
void effFreeIndexedEntries(effClassWork_t *work);
extern f32 effWorldUpVector[4];
void effUpdateRadialWavePointRows(effClassWork_t *work);

typedef struct effMatrix4_t effMatrix4_t;

void effDrawFivePointGroups(effPointSet_t *set, effMatrix4_t *matrix);
void effDrawTintedFivePointGroupRows(effClassWork_t *work);
extern void effResetDispatchCounter(effClassWork_t *);
void effResetRadialClassResourceFrames(effClassWork_t *work);
extern effClassWork_t *effCreateClassResourceWork(u16, void *);
effClassWorkList_t *effCreateRadialClassResourceEntries(effBillPointConfig_t *config);
extern void effDestroyClassResourceWork(effClassWork_t *);
void effDestroyRadialClassResourceEntries(effClassWork_t *work);
extern void effAdvanceClassResourceFrame(effClassWork_t *);
void effAdvanceRadialClassResourceFrames(effClassWork_t *work);
extern void effCopyClassResourcePosition(void *, void *);
extern void effCopyClassResourceOrientation(void *, void *);
extern void effSetClassResourceMatrixComponent(effClassWork_t *, f32);
extern void effSetClassResourceColor(effClassWork_t *, u32);
extern void effDrawClassResourceWork(effClassWork_t *);
void effUpdateRadialClassInstances(effClassWork_t *work);

typedef struct effScaleRange_t {
    effScaleRangeEntry_t *entries;
    f32 start;
    f32 delta;
    struct sdfMemHandle_t *allocation;
} effScaleRange_t;

void effSeedBillScaleRange(effClassWork_t *work);
effScaleRange_t *effCreateRetainedPointSetColorRows(effBillPointConfig_t *src);
void effReleaseBillPointEntries(effClassWork_t *work);
void effUpdateOrbitalPointSpanWaves(effClassWork_t *work);
void effDrawRetainedPointSetRows(effClassWork_t *work);
effClassWork_t *effCreateClassResourceWork(u16 kind, void *source);
effClassWork_t *effCreateClassResourceFromFile(fileJobPayload_t *request);
void effDestroyClassResourceWork(effClassWork_t *work);
effClassWork_t *effCloneClassResourceWork(effClassWork_t *work);
void effResetDispatchCounter(effClassWork_t *work);
void effAdvanceClassResourceFrame(effClassWork_t *work);
void effDrawClassResourceWork(effClassWork_t *work);
void effCopyClassResourcePosition(void *dst, void *src);
void effCopyClassResourceOrientation(void *work, void *src);
void effSetClassResourceColor(effClassWork_t *work, u32 color);
void effSetClassResourceMatrixComponent(effClassWork_t *work, float value);
extern sdf3DPrimPolyParm_t effWork3[];
effStripNode_t *effCreateStripNode(u32 percent);
effStripNode_t *effCreateStripNodeFromGrid(fileKeyBlock_t *grid);
extern void effReplaceFileResourceRef(effStripNode_t *, u32, const void *);
effStripNode_t *effFileResourceReferenceReplace(fileJobPayload_t *work);
void Delete_effPolygonTrack(effStripNode_t *work);
effStripNode_t *effCloneStripResourceFromOwner(effStripNode_t *work);
void effReplaceFileResourceRef(effStripNode_t *node, u32 entryId, const void *resource);
void effClearStripRecordReferences(effStripNode_t *node);
void effAcquireStripRecord(effStripNode_t *node);
void effDrawEffectStrip(effStripNode_t *node);
void effUpdateAndDrawStripRecord(effStripNode_t *node);
void effSetStripRecordVector(effStripNode_t *node, const u128 *value);
void effSetStripRecordSecondaryVector(effStripNode_t *node, const u128 *value);
void effSetStripOpacity(effStripNode_t *p, f32 v);
void effResetBillTable(u8 *work);
u8 *effAllocateRingFadeEntries(effBillVortexConfig_t *config);
void effFillRingFadeGradient(u8 *node, effBillEmitterCommon_t *config);
u8 *effCreateRingFadeTable(effBillVortexConfig_t *config, u32 resource);
u8 *effAssetPointerSet(u8 *work);
void effReleaseRingFadeTable(s32 work);
extern void effDrawRibbon(effRibbonWork_t *, const effMatrix4_t *);
void effBlendBillboardInstanceColorsAndTransforms(u8 *work);
u8 *effAllocateBillFadeFrameEntries(effBillColumnConfig_t *config);
void effFillBillFadeGradient(u8 *node, effBillEmitterCommon_t *config);
u8 *effCreateBillFadeTable(effBillColumnConfig_t *config, u32 resource);
u8 *effCloneBillFadeTable(u8 *work);
void effReleaseBillFadeTable(s32 work);
void effUpdateColumnParticlesAndRibbons(effClassWork_t *work);
void effBillBlendCellColorAndUpdateTransform(billCellDrawWork_t *work);
u8 *effAllocateCompactRingFadeEntries(effBillSpiralConfig_t *config);
void effFillCompactRingFadeGradient(u8 *node, effBillEmitterCommon_t *config);
u8 *effCreateCompactRingFadeTable(effBillSpiralConfig_t *config, u32 resource);
u8 *effCloneBillboardFrameAsset(u8 *work);
void effReleaseCompactRingFadeTable(s32 work);

typedef struct effSpiralParticle_t {
    s32 timer;      /* -1 free, -2 retired */
    u8 pad04[4];
    f32 tiltX;      /* 0x08 */
    f32 tiltY;      /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radiusStep; /* 0x14 */
    f32 spin;       /* 0x18 */
    f32 phase;      /* 0x1C */
    f32 width;      /* 0x20 */
    f32 sweep;      /* 0x24 */
    f32 height;     /* 0x28 */
} effSpiralParticle_t;

void effUpdateBillSpiral(effClassWork_t *work);
void effUpdateCompactRingDrawColorAndTransform(u8 *work);
effClassWork_t *effAllocateBlock(u16 kind, void *source);
extern effClassWork_t *effAllocateBlock(u16, void *);
effClassWork_t *effCreateResourceInstanceB(u16 kind, void *source, u32 option);
effClassWork_t *effCreateBlockResourceFromFile(fileJobPayload_t *request);
void effDestroyBlockResourceWork(effClassWork_t *work);
effClassWork_t *effDuplicateActiveResourceB(effClassWork_t *work);
void effResetBlockResourceFrame(effClassWork_t *work);
void effAdvanceBlockResourceFrame(effClassWork_t *work);
void effDrawBlockResourceWork(effClassWork_t *work);
extern sdf3DPrimPolyParm_t effDrawRibbonWork;
extern sdfGrOt_t *effDrawRibbonData[];

static inline void unpackNormalizedColor(u32 color) {
    u32 rgba[4];
    rgba[0] = color;
    EE_MMI_RGBA_UNPACK(rgba, 1.0f / 128.0f);
}

static inline u32 packNormalizedColor(void) {
    u32 rgba[4];
    EE_MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

void effDrawRibbon(effRibbonWork_t *work, const effMatrix4_t *matrix);
u32 effGetWindTextureHandle(void);
void effResetAnimationFrameEntries(u8 *work);
u32 *effAllocateAnimationBuffer(effBillFlameConfig_t *work);
void effFillFadeColorRows(u8 *work, u8 *config);
extern u32 effCreateTexturedStripWithSharedTexture(u32, u32);
u32 *effPrepareTextureAnimation(effBillFlameConfig_t *work);
extern u32 effAllocateStripFromWorkAndRetainTexture(u8 *);
u32 *effPrepareOwnedTextureAnimation(u8 *work);
void effReleaseScalyStripResources(u8 *work);
void effReleaseTextureAnimationWork(s32 work);
void effUpdateTintedScaledMesh(billCellDrawWork_t *work);
void effInitializeAnimationPositions(effClassWork_t *work);
u32 effClampSlotCount(fileKeyBlock_t *grid);
extern u32 effRetainScalyTextureReference(void);
effAnimationState_t *effCreateAnimationState(u32 unused, u32 count);
effAnimationState_t *effActivateAnimationState(s32 work);
extern void effReleaseScalyTextureReference(s32);
void effReleaseAnimationFrameResources(u8 *work);
void effSynchronizeFileTransform(u8 *work);
extern u32 effClampSlotCount(fileKeyBlock_t *);
effAnimationState_t *effCreatePrimarySlotAnimationState(u8 *work);
effAnimationState_t *effCreateAlternateSlotAnimationState(u8 *work);
u32 *effAllocateQuantizedBuffer(effBillQuantizedConfig_t *work);
void effBuildQuantizedTexturedStrip(u32 *buffer, effBillQuantizedConfig_t *config);
extern void effBuildQuantizedTexturedStrip(u32 *, effBillQuantizedConfig_t *);
u32 *effPrepareQuantizedTexture(effBillQuantizedConfig_t *work);
u32 *effPrepareOwnedQuantizedTexture(u8 *work);
void effReleaseBillboardFrameAsset(s32 work);
void effUpdateFadedMeshTransform(billCellDrawWork_t *work);
effClassWork_t *effAllocateBlockWithModel(u16 kind, void *source);
extern effClassWork_t *effAllocateBlockWithModel(u16, void *);
effClassWork_t *effCreateResourceInstanceC(u16 kind, void *source);
effClassWork_t *effCreateModelBlockFromFile(fileJobPayload_t *request);
void effDestroyModelBlockWork(effClassWork_t *work);
effClassWork_t *effRecreateActiveByClass(effClassWork_t *work);
void effResetModelBlockFrame(effClassWork_t *work);
void effAdvanceDispatchCounter(effClassWork_t *work);
void effDrawModelBlock(effClassWork_t *work);
u32 effRetainScalyTextureReference(void);
void effReleaseScalyTextureReference(s32 unused);

/* Allocate position/UV/color rows and a draw handle using count and repeat. */
u8 *effAllocateTexturedStripWork();
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat);
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work);

void effDrawScalyTexturedStrip(effStripWork_t *work, void *matrix);
void effLoadScalyTexture(void);
u32 effGetScalyTextureHandle(void);

typedef char effModelResource_size_must_be_0x40[
    (sizeof(effModelResource_t) == 0x40) ? 1 : -1];

void effSeedParticleSpanParameters(effModelResource_t *work);
extern effPointSet_t *effCreatePointSet3(s32 count);
effSpanTable_t *effCreateParticleSpanTable(effSpanConfig_t *config, mdlObj_t *model);
extern void effReleaseModelPointSetAsset(effPointSet_t *);
void effReleaseParticleList(effSpanTable_t *list);
void effDrawParticleSpanPointsAndReferences(effModelResource_t *work);
effModelResource_t *Create_effPolygonCrack(u16 kind, void *source, void *secondary, u32 param);
u32 PartsCreate_effPolygonCrack(fileJobPayload_t *work);

typedef struct effModelCreateRequest_t {
    u8 pad0[0x2C];
    u16 kind;
    u8 pad2E[2];
    mdlObj_t *assetId;
    u32 attributes;
    u8 pad38[4];
    void *source;
} effModelCreateRequest_t;

void Delete_effPolygonCrack(effModelResource_t *effect);
effModelResource_t *Copy_effPolygonCrack(effModelCreateRequest_t *work);
void effResetModelResourceUpdateCount(effModelResource_t *effect);
void effAdvanceModelResourceUpdateCount(effModelResource_t *effect);
void effDispatchModelResourceCallback(effModelResource_t *effect);
extern sdf3DPrimPolyParm_t effWork4[];
effPointSet_t *effCreatePointSet3(s32 count);
void effReleaseModelPointSetAsset(effPointSet_t *set);
void effDrawThreePointGroups(effPointSet_t *set, effMatrix4_t *matrix);

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR 0

#define EFF_COLLECT_ACTORS_ORIGINAL_SELECTOR 1

#define EFF_COLLECT_ACTORS_VARIANT_SELECTOR_ALIAS 2

#define EFF_COLLECT_ACTORS_ORIGINAL_GROUP 3

#define EFF_COLLECT_ACTORS_VARIANT_GROUP 4

#define EFF_COLLECT_ACTORS_EITHER_GROUP 5

#define EFF_COLLECT_ACTORS_OVERRIDE_SELECTOR 6

#define EFF_COLLECT_ACTORS_FINAL_SELECTOR 7

extern u128 sbtlLeaf50Work2[];
extern s8 effData36;
extern u128 effWork5[];
extern u128 effComputeLightDirectionVUWork[];
extern sdf3DLight_t *effComputeLightDirectionVUData[3];
s32 effComputeLightDirectionVU(mdlObj_t *vector, sdf3DLightParallelPacket_t *target);

/* Payload of resource kind 2: two colours plus the position request that feeds the shared light vector. */
typedef struct effFieldColorConfig_t {
    u32 firstColor;   /* 0x00 */
    u32 secondColor;  /* 0x04 */
    effVectorRequest_t direction; /* 0x08 */
} effFieldColorConfig_t;

typedef struct effCopiedSpawnList_t {
    u32 count;
    fldSpawnBatch_t *groups;
} effCopiedSpawnList_t;

void effDestroyCopiedEffectPayload(effCopiedPayload_t *payload);

extern u128 D_0037EE90, D_0037EEA0, D_0037EEB0, D_0037EEC0, D_0037EED0;

typedef struct effAnimInfo_t {
    u16 id;
    u16 flags;
    u8 actorSelection;
    u8 unk05;
    u16 loop;
} effAnimInfo_t;

void effResetSlots(void);

/* The final timer word also supplies the default tint's halfword ID. */
typedef struct effTintTransitionParams_t {
    u32 duration;
    u32 resourceArgument;
    u16 resourceKind;
    u16 unk0A;
    union {
        u32 delay;
        struct {
            u16 defaultKind;
            u16 unk0E;
        } ids;
    } tail;
} effTintTransitionParams_t;

effParticleSharedResource_t *eff2PartsCreate(fileJobPayload_t *source);
void effReleaseParticleResources(effParticleSharedResource_t *work);
void eff2ResourceCopy(effParticleSharedResource_t *, effParticleSharedResource_t *);
effParticleSharedResource_t *Copy_eff2D(effParticleSharedResource_t *source);
void eff2ResourceCopy(effParticleSharedResource_t *work, effParticleSharedResource_t *source);

/* Payload option (+0x0C) selects the asset identifier at +0x08. */
typedef struct effAssetIdentifier_t {
    u8 pad_00[4];
    u16 type;       // 0x04
    u8 pad_06[2];
    u16 id;         // 0x08
} effAssetIdentifier_t;

typedef struct effCameraCreateRow_t {
    effCameraCreateRequest_t request;
    u32 sourceTail;
} effCameraCreateRow_t;

typedef char effCameraCreateRowSizeCheck[(sizeof(effCameraCreateRow_t) == 0x1C) ? 1 : -1];

typedef struct effBlock90_t {
    u32 word[36];
} effBlock90_t;

typedef struct effAlignedBlock128_t {
    u64 word[16];
} effAlignedBlock128_t;

extern char D_003B3A90[], D_003B3AA8[];

typedef struct effStateSnapshot_t {
    s128 vectors[4];
    f32 position[4]; /* 0x40: copied to the debug camera position and follow offset. */
    s128 unused50;
    u8 pad60[8];
    u32 flags; /* 0x68: transform flags tested by the camera diagnostic. */
    u8 pad6C[0x14];
} effStateSnapshot_t;

typedef char effStateSnapshotSizeCheck[(sizeof(effStateSnapshot_t) == 0x80) ? 1 : -1];

/* The camera snapshot payload is the 0x90-byte record consumed by this updater. */
typedef union EffectCameraSnapshotView {
    u32 word[36];
    struct {
        u8 pad00[0x40];
        f32 offset[4];
        f32 quat[4];
        u8 pad60[0x30];
    } camera;
} EffectCameraSnapshotView;

typedef struct effCameraDebugState_t {
    u32 unknown00[4];
    u32 flags;
    u32 unknown14[9];
} effCameraDebugState_t;

typedef struct effCameraDebugScratch_t {
    s128 basePosition;
    s128 segmentStart;
    s128 segmentEnd;
    s128 rotationMatrix[4];
} effCameraDebugScratch_t;

typedef struct effPollRecord_t {
    char fullName[0xC8]; /* Formatted path or selected built-in name. */
    s32 type;   // 0xC8
    s32 selectionStatus;  // 0xCC
    s32 pathVariant; // 0xD0
} effPollRecord_t;

/* The effect mapping dispatcher stores its data pointer at +0x0C. */
typedef struct effMappingRequest_t {
    u8 pad_00[0x0C];
    s32 object;          // 0x0C
} effMappingRequest_t;

/* Parameters passed to the mapping dispatcher from distinct object channels. */
typedef struct effMappingObject_t {
    u8 pad_00[0x34];
    s32 value34;       // 0x34
    u32 value38;       // 0x38
    s32 value3C;
    u8 pad40[0xC];
    u32 value4C;
    u8 pad50[0x20];
    u32 value70;       // 0x70
    s32 value74;
    u8 pad78[8];
    u32 value80;       // 0x80
    u32 value84;       // 0x84
    u8 pad_88[4];
    s32 value8C;       // 0x8C
    u8 pad_90[0x28];
    u32 valueB8;       // 0xB8
    u8 pad_BC[0x18];
    s32 valueD4;       // 0xD4
    s32 valueD8;       // 0xD8
    u8 pad_DC[0x18];
    s32 valueF4;       // 0xF4
} effMappingObject_t;

typedef struct effFileResourceRecord_t {
    char *name;
    u8 pad4[4];
    u16 mode;
    u8 padA[2];
    u8 *buffer;
    u32 size;
    struct sdfMemHandle_t *allocation;
} effFileResourceRecord_t;

typedef struct effFileHeader_t {
    u8 unk_00[8];
    u16 mode;
    u8 unk_0A[2];
    u32 start;
    u32 length;
} effFileHeader_t;

effFadeHeaderWork_t *effDuplicateSmallHeader(const void *source) {
    effFadeHeaderWork_t *buffer = sdfChipAllocDirect(sizeof(effFadeHeaderWork_t));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    buffer->quake = effLeaf345();
    return buffer;
}

effFadeHeaderWork_t *effCreateSmallHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((fileJobPayload_t *)work);
    return effDuplicateSmallHeader(resource);
}

void effReleaseFadeHeaderAllocation(effFadeHeaderWork_t *allocation) {
    fclFunc2(allocation->quake);
    sdfChipFreeDirect(allocation);
}

effFadeHeaderWork_t *effCloneSmallHeaderFromWork(effFadeHeaderWork_t *work) {
    return effDuplicateSmallHeader(work->payload);
}

void effResetSmallHeaderFrame(effFadeHeaderWork_t *work) {
    work->frame = 0;
}

void effFadeFrameAdvance(effFadeHeaderWork_t *counter) {
    s32 frame;

    if ((effSRAnimation() & 1) == 0) {
        frame = counter->frame;
        if (frame == 0) {
            if (counter->payload[0] == 0) {
                fclLeaf7(counter->quake, counter->payload[1]);
            } else {
                fclDrawSprite(counter->quake, counter->payload[1], counter->payload[0]);
            }
            frame = counter->frame;
        }
        counter->frame = frame + 1;
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCreateFadeVectorWork);

effFadeVectorWork_t *effCreateFadeVectorFromFile(void *work) {
    const effLensFlareParams_t *resource;

    resource = fileResolvePrimaryBuffer(work);
    return effCreateFadeVectorWork(resource);
}

void effFreeFadeVectorWork(effFadeVectorWork_t *work) {
    sdfChipFreeDirect(work);
}

effFadeVectorWork_t *effCloneFadeVectorWork(effFadeVectorWork_t *work) {
    return effCreateFadeVectorWork(&work->source);
}

void effResetFadeVectorFrame(effFadeVectorWork_t *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effDrawLensFlare);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf354);

void effSetFadeVectorColor(effFadeVectorWork_t *work, u32 color) {
    work->color = color;
}

void effResetSelectionEntryBuffers(sdfFlagListWork_t *work) {
    u32 count;
    sdfFlagListMark_t *entries;
    u32 index;

    index = 0;
    count = work->params.count;
    entries = work->marks;
    if (count != 0) {
        do {
            index = index + 1;
            entries->timer = -1;
            entries++;
        } while (index < count);
    }
    memset(work->colors, 0, count << 3);
}

sdfFlagListWork_t *effCreateSelectionFlagListFromWork(const sdfFlagListParams_t *work) {
    return sdfCreateFlagListWork(work);
}

sdfFlagListWork_t *effCreateSelectionFlagListFromFile(void *file) {
    return sdfInitializeFlagListFromResource(file);
}

void effReleaseSelectionFlagList(sdfFlagListWork_t *work) {
    sdfReleaseFlagListResource(work);
}

sdfFlagListWork_t *effCreateEmbeddedSelectionFlagList(sdfFlagListWork_t *work) {
    return effCreateSelectionFlagListFromWork(&work->params);
}

void effResetSelectionEntriesAndState(sdfFlagListWork_t *p) {
    effResetSelectionEntryBuffers(p);
    p->frame = 0;
}

void effUpdateSelectionEntryState(sdfFlagListWork_t *work) {
    sdfUpdateFlagListEntries(work);
}

void effDrawSelectionEntryVectors(sdfFlagListWork_t *work) {
    sdfDrawFlagListVertices(work);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSRAnimation5);

void effSetSelectionFlagListColor(sdfFlagListWork_t *work, u32 color) {
    work->unk04 = color;
}

effSelectionHeaderWork_t *effDuplicatePayloadHeader(const void *source) {
    effSelectionHeaderWork_t *buffer = sdfChipAllocDirect(sizeof(effSelectionHeaderWork_t));
    buffer->frame = 0;
    memcpy(buffer->payload, source, sizeof(buffer->payload));
    return buffer;
}

effSelectionHeaderWork_t *effCreateSelectionHeaderFromFile(void *work) {
    void *resource;

    resource = fileResolvePrimaryBuffer((fileJobPayload_t *)work);
    return effDuplicatePayloadHeader(resource);
}

void effReleaseSelectionHeaderAllocation(effSelectionHeaderWork_t *allocation) {
    evtDestroySelectionState();
    sdfChipFreeDirect(allocation);
}

effSelectionHeaderWork_t *effCloneSelectionHeaderFromWork(effSelectionHeaderWork_t *work) {
    return effDuplicatePayloadHeader(work->payload);
}

void effResetSelectionHeaderFrame(effSelectionHeaderWork_t *work) {
    work->frame = 0;
}

void effSelectionFrameAdvance(effSelectionHeaderWork_t *counter) {
    s32 frame;

    frame = counter->frame;
    if (frame == 0) {
        evtSelStateCreate(counter->payload[0], (s16)counter->payload[1], counter->payload[2], counter->payload[3]);
        frame = counter->frame;
    }
    counter->frame = frame + 1;
}

/* Draw through the configured duration; zero duration uses frame zero.
 * Blend the packed color and use percent-scaled curves for the output rates. */
void effUpdateFadeBlendA(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->blend = work->mode;
    effDrawBlurRectangle(out);
}

void *effCreateFadeBlendWorkFromOutput(void *work) {
    return dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeBlendWork(void *resourceHandle) {
    dds3EffectPartialMultiBlurDelete((dds3Effect_PartialMultiBlur_t *)resourceHandle);
}

/* Draw the fade in fixed subpixel units, either centered or at the projected position.
 * Projected X/Y use sixteen units per pixel; output Y is doubled. */
void effUpdateProjectedBlurFadeRectangle(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    dds3Effect_PartialMultiBlur_t *out = (dds3Effect_PartialMultiBlur_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->body.Blur.center[0] = 0;
        out->body.Blur.center[1] = 0;
        out->body.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->body.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->body.Blur.center[0] = px;
        out->body.Blur.center[1] = py << 1;
    }
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->body.Blur.abgr = blended[0];
    out->body.Blur.scale = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f + 1.0f;
    out->body.Blur.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->body.Blur.blend = work->mode;
    effDrawBlurFixedPointRectangle(out);
}

void effUpdateTarget(effKindWork_t *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((dds3Effect_PartialMultiBlur_t *)work->handle)->texture = target;
}

void *effCreateFixedSlotBlurWorkFromFadeOutput(void *source) {
    return dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)((u8 *)source + 0xC0));
}

void effReleaseFixedSlotBlurWork(effBlurScatterWork_t *handle) {
    dds3EffectDistortionBlurDelete(handle);
}

/* Draw the pixel-unit fade using the handle's output record and payload's curves.
 * Zero projected mode suppresses drawing; the origin and Y scaling differ from subpixels. */
void effUpdateFadeMapA(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    effBlurScatterWork_t *out = (effBlurScatterWork_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.distortion = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    effBlurStepScatterSlotsAndDraw(out);
}

void effTextureReferenceRelease(effKindWork_t *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((effBlurScatterWork_t *)work->handle)->texture = target;
}

void *effCreateVariableSlotBlurWorkFromFadeOutput(void *source) {
    return dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)((u8 *)source + 0xc0));
}

void effReleaseVariableSlotBlurWork(effBlurScaleWork_t *work) {
    dds3EffectRippleBlurDelete(work);
}

/* Draw the same pixel-unit fade into the wider renderer output record.
 * The native work header and duration gate remain shared with the other kind callbacks. */
void effUpdateFadeMapB(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    effBlurScaleWork_t *out = (effBlurScaleWork_t *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.area = (s32)rate;
        out->params.center[0] = 0;
        out->params.center[1] = 0;
    } else {
        s32 mode;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)mnuMeasureProjectedPerpendicularDistance(rate);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR(pos);
        out->params.center[0] = (s32)pos[0] - 0x800;
        out->params.center[1] = ((s32)pos[1] - 0x800) << 1;
    }
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->params.abgr = blended[0];
    out->params.frequency = effSampleScalarCurve(&config->blendB, frame, duration) * 0.01f;
    out->params.rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration) * 0.01f;
    out->params.blend = work->mode;
    effBlurStepScaleSlotsAndDraw(out);
}

void effReplaceKindLinkedTarget(effKindWork_t *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((effBlurScaleWork_t *)work->handle)->texture = target;
}

/* Draw a framebuffer fade using raw curve rates rather than percent-scaled rates.
 * The generic kind work supplies its packed color, mode and frame limit. */
void effUpdateFadeBlendB(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Blur_t *out = &config->out;
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->center[0] = 0;
    out->center[1] = 0;
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    out->abgr = blended[0];
    out->scale = effSampleScalarCurve(&config->blendB, frame, duration) + 1.0f;
    out->rotate = effSampleScalarCurve(&config->rateA.curve, frame, duration);
    out->blend = work->mode;
    effBlurDrawFramebufferQuad(out);
}

/* Draw the color-only fade from the copied payload's curves.
 * Its output uses fixed rectangle bounds and the kind work's packed color and mode. */
void effUpdateFadeBlendC(effKindWork_t *work) {
    effFadeConfig_t *config = work->payload;
    s32 duration = config->duration;
    s32 frame = 0;
    dds3Effect_Filter_t *out = &config->out;
    u32 packed;
    u32 unit;
    u32 second;
    u32 baseColor;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    out->xy[0][0] = 0;
    out->xy[0][1] = 0;
    out->xy[1][0] = 0x200;
    out->xy[1][1] = 0x1C0;
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    /* Capture the base word before the unit: see docs/idioms.md,
     * "Trace-justified base color capture: effUpdateFadeBlendC". */
    baseColor = work->color;
    unit = 0x3C000000;
    eeUnpackRGBAWord(baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->abgr = packed;
    out->blend = work->mode;
    dds3EffectFilter(out);
}

void *effCreateFadeColorWorkFromOutput(void *work) {
    return dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)((u8 *)work + 0xc0));
}

void effReleaseFadeColorWork(EffResourceRectWork *work) {
    dds3EffectPartialMultiFilterDelete(work);
}

/* Update the fade rectangle's projected extent, center and multiplied tint. */
void effUpdateProjectedFadeColor(effKindWork_t *work) {
    effRateConfig_t *config = (effRateConfig_t *)work->payload;
    EffResourceRectWork *out = (EffResourceRectWork *)work->handle;
    s32 duration = config->duration;
    s32 frame = 0;
    f32 rate;
    f32 pos[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 second;

    if (duration != 0) {
        frame = work->frame;
    }
    if (duration < frame) {
        return;
    }
    rate = effSampleScalarCurve(&config->rateB.curve, frame, duration);
    if (config->fixedMode != 0) {
        out->params.center[0] = 0;
        out->params.center[1] = 0;
        out->params.area = (s32)(rate * 16.0f);
    } else {
        s32 mode;
        s32 px;
        s32 py;

        rate *= work->scale;
        SDF_VU0_LOADVECTOR(work);
        mode = (s32)(mnuMeasureProjectedPerpendicularDistance(rate) * 16.0f);
        out->params.area = mode;
        if (mode == 0) {
            return;
        }
        SDF_VU0_STOREVECTOR_NOMEM(pos);
        py = (s32)(pos[1] * 16.0f) - 0x8000;
        px = (s32)(pos[0] * 16.0f) - 0x8000;
        out->params.center[0] = px;
        out->params.center[1] = py << 1;
    }
    second = effColorAnimation(&config->blendA, &config->blendB2, frame, duration);
    color1[0] = work->color;
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK_F128(packed);
    blended[0] = packed;
    *(u32 *)out->params.Filter.color = blended[0];
    out->params.Filter.blendControl = work->mode;
    effComputeBlurRectBounds(out);
}

void effReplaceLinkedKindWorkTarget(effKindWork_t *work, sdfTexHandle_t *target) {
    sdfTexHandle_t *previous = work->target;
    if (previous != 0 && previous != target) {
        if (work->sourceKind != 4) {
            sdfTexFreeDelayed(previous);
        }
        work->target = target;
    }
    ((EffResourceRectWork *)work->handle)->texture = target;
}

effKindWork_t *effAllocateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = effData5[kind].payloadSize;
    effKindWork_t *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((effKindSource_t *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (effData5[kind].create != NULL) {
        work->handle = effData5[kind].create(source);
    }
    return work;
}

effKindWork_t *effCreateKindWorkFromFile(fileJobPayload_t *work) {
    void *source = fileResolvePrimaryBuffer(work);
    effKindWork_t *effect = effAllocateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (effData5[effect->kind].initialize != NULL) {
            u32 kind = work->primary.selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexSet((TMX_HEADER_t *)secondary);
                break;
            case 4:
                effect->target = dds3BillboardGetGeneralTex(secondary[0]);
                break;
            }
            effData5[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

void effReleaseLinkedTarget(effKindWork_t *work) {
    void *handle = work->handle;
    if (handle != NULL) {
        effData5[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexFreeDelayed(work->target);
    }
    sdfChipFreeDirect(work);
}

effKindWork_t *effCloneKindWork(effKindWork_t *work) {
    return effAllocateKindWork((u16)work->kind, work->payload);
}

void effResetKindWorkFrame(effKindWork_t *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effKindWorkFrameUpdate);

void effSetKindWorkPosition(effKindWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->position, position);
}

void effSetKindWorkColor(effKindWork_t *work, u32 color) {
    work->color = color;
}

void effSetKindWorkScale(effKindWork_t *work, f32 scale) {
    work->scale = scale;
}

/* Allocate work for the alternate kind table, copying its payload and creating its handle. */
effKindWork_t *effAllocateAlternateKindWork(u16 kind, u8 *source) {
    u32 headerSize = 0x40;
    u32 size = effData6[kind].payloadSize;
    effKindWork_t *work = sdfChipAllocZeroDirect(size + headerSize);

    work->kind = kind;
    work->target = 0;
    work->color = 0x80808080;
    work->scale = 1.0f;
    VU0_STORE_VF($vf0, work);
    work->payload = (u8 *)work + headerSize;
    memcpy(work->payload, source, size);
    switch (((effKindSource_t *)source)->modeKind) {
    case 1:
        work->mode = 0x44;
        break;
    case 2:
        work->mode = 0x48;
        break;
    case 3:
    case 4:
        work->mode = 0x42;
        break;
    case 5:
    case 6:
        work->mode = 6;
        break;
    }
    if (effData6[kind].create != NULL) {
        work->handle = effData6[kind].create(source);
    }
    return work;
}

effKindWork_t *effCreateKindWorkFromFileB(fileJobPayload_t *work) {
    void *source = fileResolvePrimaryBuffer(work);
    effKindWork_t *effect = effAllocateAlternateKindWork(work->option, source);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary != NULL) {
        if (effData6[effect->kind].initialize != NULL) {
            u32 kind = work->primary.selector;
            effect->sourceKind = kind;
            switch (kind) {
            case 1:
                effect->target = sdfTexSet((TMX_HEADER_t *)secondary);
                break;
            case 4:
                effect->target = dds3BillboardGetGeneralTex(secondary[0]);
                break;
            }
            effData6[effect->kind].initialize(effect, effect->target);
        }
    }
    return effect;
}

/* Release the alternate-kind handle, owned secondary reference, and work allocation. */
void effReleaseAlternateKindWork(effKindWork_t *work) {
    void *handle = work->handle;
    if (handle != NULL) {
        effData6[work->kind].destroy(handle);
    }
    if (work->target != 0 && work->sourceKind != 4) {
        sdfTexFreeDelayed(work->target);
    }
    sdfChipFreeDirect(work);
}

/* Reallocate the source kind and payload; this path does not copy its secondary target. */
effKindWork_t *effCloneAlternateKindWork(effKindWork_t *work) {
    return effAllocateAlternateKindWork((u16)work->kind, work->payload);
}

void effResetAlternateKindWorkFrame(effKindWork_t *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSRAnimation6);

void effSetAlternateKindWorkPosition(effKindWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->position, position);
}

void effSetAlternateKindWorkColor(effKindWork_t *work, u32 color) {
    work->color = color;
}

void effSetAlternateKindWorkScale(effKindWork_t *work, f32 scale) {
    work->scale = scale;
}

extern sdfVector4_t Draw_effMuzzleFlashTable;

u8 *Create_effMuzzleFlash(fileJobPayload_t *source) {
    u8 *work = (u8 *)sdfChipAllocDirect(0x68);
    memset(work, 0, 0x68);
    ((effBillboardWork_t *)work)->frame = 0;
    ((effBillboardWork_t *)work)->color = 0x80808080;
    ((effBillboardWork_t *)work)->scale = 1.0f;
    VU0_STORE_VF(vf0, work);
    VU0_STORE_VF(vf0, work + 0x10);
    if (source == NULL) {
        return work;
    }
    memcpy(work + 0x2C, fileResolvePrimaryBuffer(source),
           source->primary.size);
    ((effBillboardWork_t *)work)->billboard =
        dds3BillboardCreate(1, (u32)fileResolveSecondaryBuffer(source));
    return work;
}

void Delete_effMuzzleFlash(effBillboardWork_t *work) {
    billHandle_t *billboard;

    billboard = work->billboard;
    if (billboard != 0) {
        dds3BillboardDelete(billboard);
    }
    sdfChipFreeDirect(work);
}

u8 *Copy_effMuzzleFlash(const u8 *source) {
    u8 *effect = (u8 *)Create_effMuzzleFlash(NULL);
    memcpy(effect + 0x2C, source + 0x2C, 0x38);
    ResourceCopy_effMuzzleFlash((s32)effect, (s32)source);
    return effect;
}

void ResourceCopy_effMuzzleFlash(s32 dst, s32 src) {
    billHandle_t *billboard;

    if (((effBillboardWork_t *)dst)->billboard != 0) {
        dds3BillboardDelete(((effBillboardWork_t *)dst)->billboard);
    }
    billboard = dds3BillboardCopy(((effBillboardWork_t *)src)->billboard);
    ((effBillboardWork_t *)dst)->billboard = billboard;
}

void effBillboardEntryFrameReset(effBillboardWork_t *work) {
    dds3BillboardSetAnimKey2(work->billboard, 0);
    work->frame = 0;
}

void Draw_effMuzzleFlash(effBillboardWork_t *work) {
    sdfVector4_t direction;
    s32 animFrame = dds3BillboardGetAnimFrame(work->billboard);
    s32 frame = work->frame;
    f32 rot;
    f32 sc;
    f32 scale;

    if (frame < animFrame) {
        dds3BillboardSetAnimKey2(work->billboard, frame);
        SDF_VU0_LOADVECTOR(work->rotation);
        ToMatrix();
        SDF_VU0_LOADVECTOR(&Draw_effMuzzleFlashTable);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR(&direction);
        rot = dds3BillboardGetDirectionRotate(&work->position, &direction);
        sc = dds3BillboardGetDirectionScale(&work->position, &direction);
        if (sc < 0.29999998211860657f) {
            sc = 0.29999998211860657f;
        }
        scale = work->scale;
        sc = sc * scale;
        dds3BillboardSetScale(work->billboard, sc * work->widthScale, work->heightScale * scale);
        dds3BillboardSetRotate(work->billboard, rot);
        dds3BillboardSetPosition(work->billboard, &work->position);
        dds3BillboardDraw(work->billboard);
        if ((effSRAnimation() & 1) == 0) {
            work->frame++;
        }
    }
}

void effSetBillboardPosition(effBillboardWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(&work->position, position);
}

void effSetBillboardRotation(effBillboardWork_t *work, const f32 rotation[4]) {
    SDF_VU0_COPYVECTOR(work->rotation, rotation);
}

void effSetBillboardColor(effBillboardWork_t *work, u32 color) {
    work->color = color;
}

void effSetBillboardScale(effBillboardWork_t *work, f32 scale) {
    work->scale = scale;
}

void effClearBillFrames(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *effCreateBillFrameNode(effBillFrameConfig_t *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(count, 0, resource);
    return node;
}

/* Release the frame node's shared tracks and backing allocation. */
void effReleaseBillFrameNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

void billAdvanceFrameInstances(effClassWork_t *work) {
    effBillFrameConfig_t *config = (effBillFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effTrackSet_t *asset;
    effBillFrameEntry_t *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 innerColor;
    u32 outerColor;
    u32 *colors;
    u128 *vertices;
    f32 innerVector[4];
    f32 outerVector[4];
    f32 direction[4];
    f32 radius[4];
    f32 innerWidth[4];
    f32 outerWidth[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (effBillFrameState_t *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (effBillFrameEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    innerColor = config->innerColor & 0xFFFFFF;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(innerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(innerVector);
    outerColor = config->outerColor & 0xFFFFFF;
    eeUnpackRGBAWordFloat(outerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(outerVector);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    respawn = config->frame.respawn;
    item = 0;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    for (; item < itemCount; item++, entries++, vertices += 5, colors += 5) {
        effBillFrameEntry_t *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 factor;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (dds3GetRandA((effRandState_t *)effSharedRandomState) & 1) entry->spin = -entry->spin;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * config->radiusJitter + (1.0f - config->radiusJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                factor = randomValue * config->widthJitter + (1.0f - config->widthJitter);
                entry->innerWidth = config->innerWidth * factor;
                entry->outerWidth = config->outerWidth * factor;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((effRandState_t *)effSharedRandomState) % (u32)lifetime);
                else entry->age = 0;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 4;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 spin = entry->spin;
            f32 time = (f32)age;
            f32 angle;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 outerPacked;
            u32 innerPacked;
            if (spin > 0.0f) angle = (spin + (acceleration * time) * 0.5f) * time;
            else angle = (spin - (acceleration * time) * 0.5f) * time;
            angle += entry->angle;
            SDF_VU0_LOADVECTOR(effOnesVector);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radius);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, entry->innerWidth);
            VU0_SCALE_VF(vf11, entry->outerWidth);
            SDF_VU0_STOREVECTOR_NOMEM(innerWidth);
            SDF_VU0_STOREVECTOR_B_NOMEM(outerWidth);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(innerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(innerWidth);
            SDF_VU0_LOADVECTOR(outerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(outerWidth);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_LOADVECTOR_B(radius);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(innerWidth);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_STOREVECTOR(vertices + 2);
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR_B(vertices + 4);
            SDF_VU0_LOADVECTOR(outerWidth);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_ADDVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(vertices + 1);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(outerVector);
            SDF_VU0_MULVECTOR();
            outerPacked = eePackRGBAWord(128.0f);
            colors[0] = outerPacked;
            colors[1] = outerPacked;
            SDF_VU0_LOADVECTOR(innerVector);
            SDF_VU0_MULVECTOR();
            innerPacked = eePackRGBAWord(128.0f);
            if (item & 1) {
                colors[3] = innerPacked | 0xFF000000;
                innerPacked = (u32)(alpha * 128.0f) << 24;
                colors[2] = innerPacked;
                colors[4] = innerPacked;
            } else {
                colors[3] = innerPacked | 0x40000000;
                innerPacked = (u32)(alpha * 255.0f) << 24;
                colors[2] = innerPacked;
                colors[4] = innerPacked;
            }
            entry->age = (s32)((u32)age + 1u);
        }
    }
}

void billUpdateFrameDrawColorAndTransform(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void billResetCellIndices(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x1C;
        }
    }
}

u8 *billCreateCellNode(effBillCellConfig_t *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x1C + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseCellNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

/* Spawn and advance the kind-2 cell instances: each live cell moves out
 * along its heading and is drawn as a 13-vertex fan (center plus two
 * six-step rings of radii A and B), faded in and out over its lifetime. */
void billAdvanceCellInstances(effClassWork_t *work) {
    effBillCellConfig_t *config = (effBillCellConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effTrackSet_t *asset;
    effBillCellEntry_t *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 outputCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startAge;
    s32 endAge;
    s32 ageStep;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 firstColor;
    u32 secondColor;
    u32 respawn;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 center[4];
    f32 direction[4];
    f32 width[4];
    f32 radialA[4];
    f32 radialB[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (effBillFrameState_t *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (effBillCellEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->reverse != 0) {
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
        startAge = lifetime;
        endAge = 0;
        ageStep = -1;
    } else {
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
        startAge = 0;
        endAge = lifetime;
        ageStep = 1;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->colorA & 0xFFFFFF;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->colorB & 0xFFFFFF;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 13, colors += 13) {
        effBillCellEntry_t *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 jitter;
                f32 base;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                jitter = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                base = 1.0f - jitter;
                entry->radiusA = config->radius * (randomValue * jitter + base);
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radiusB = config->radius * (randomValue * jitter + base);
                jitter = config->centerOffsetJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->centerOffset = config->centerOffset * (randomValue * jitter + (1.0f - jitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->velocity = config->velocity * (randomValue * config->velocityJitter + (1.0f - config->velocityJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                jitter = randomValue * config->extentJitter + (1.0f - config->extentJitter);
                entry->extent = config->extent * jitter;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((effRandState_t *)effSharedRandomState) % (u32)lifetime);
                else entry->age = startAge;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age == endAge) {
            u32 *color = colors;
            u128 *position = vertices;
            outputCount = 12;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--outputCount >= 0);
            entry->age = retiredState;
        } else {
            f32 time = (f32)age;
            f32 motionTime = time;
            f32 speed;
            f32 angle = entry->angle;
            f32 distance;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            f32 *ringA;
            f32 *ringB;
            u128 *position;
            u32 *color;
            if (acceleration < 0.0f) {
                f32 stopTime;
                speed = entry->velocity;
                stopTime = (-speed / (acceleration * 0.5f)) * 0.5f;
                if (stopTime < time) motionTime = stopTime;
            } else {
                speed = entry->velocity;
            }
            distance = (speed + (acceleration * motionTime) * 0.5f) * motionTime;
            distance += entry->centerOffset;
            SDF_VU0_LOADVECTOR(effOnesVector);
            VU0_SCALE_VF(vf10, entry->extent);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            VU0_SCALE_VF(vf10, entry->radiusA);
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_STOREVECTOR_NOMEM(radialA);
            VU0_SCALE_VF(vf11, entry->radiusB);
            SDF_VU0_STOREVECTOR_B_NOMEM(radialB);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_SCALE_VF(vf10, distance);
            SDF_VU0_STOREVECTOR_NOMEM(center);
            VU0_MOVE_VF(vf2, vf10);
            SDF_VU0_COPYVECTOR_C_TO_A();
            VU0_MOVE_VF(vf12, vf2);
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SET_VF10_COMPONENT(x, sine);
            cosine = -cosine;
            VU0_SET_VF10_COMPONENT(z, cosine);
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_STOREVECTOR_C(vertices);
            for (outputCount = 0; outputCount < 6; outputCount++) {
                SDF_VU0_LOADVECTOR(radialA);
                VU0_SCALE_VF(vf10, billRingSines[outputCount]);
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_SCALE_VF(vf11, billRingCosines[outputCount]);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(vertices + 1 + outputCount);
            }
            for (outputCount = 0; outputCount < 6; outputCount++) {
                SDF_VU0_LOADVECTOR(radialB);
                VU0_SCALE_VF(vf10, billRingSines[outputCount + 6]);
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_LOADVECTOR_B(width);
                VU0_SCALE_VF(vf11, billRingCosines[outputCount + 6]);
                SDF_VU0_ADDVECTOR();
                SDF_VU0_STOREVECTOR(vertices + 7 + outputCount);
            }
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            color = colors + 1;
            outputCount = 11;
            do {
                *color = secondPacked;
                color++;
            } while (--outputCount >= 0);
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[0] = firstPacked | ((u32)(alpha * 128.0f) << 24);
            entry->age = (s32)((u32)age + (u32)ageStep);
        }
    }
}

void billUpdateCellDrawColorAndTransform(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void billResetParticleIndices(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    effBillParticleEntry_t *entry = (effBillParticleEntry_t *)((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            entry->age = -1;
            ++entry;
        }
    }
}

u8 *billCreateParticleNode(effBillParticleConfig_t *config, u32 resource) {
    s32 count = config->frame.output.timed.count;
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(count * 0x2C + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(count, 1, resource);
    return node;
}

void billReleaseParticleNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", billAdvanceParticleInstances);

void billUpdateParticleDrawColorAndTransform(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void effClearAnimatedFrames(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocateAnimatedTransformEntries(effBillAnimatedFrameConfig_t *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->frame.output.timed.count * 0x18 + headerSize);
    u8 *node = (u8 *)sdfMemLock(base);
    u8 *entries = node + headerSize;

    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = entries;
    if (config->drawProgress == 0) {
        config->drawProgress = 1;
    }
    return node;
}

void effInitializeAlternatingTransformRows(u8 *node, u8 *config) {
    u32 count = ((effBillTimedHeader_t *)config)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((effBillFrameState_t *)node)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateAnimatedTransform(effBillAnimatedFrameConfig_t *config, u32 resource) {
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 3, resource);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

u8 *billCloneAnimatedTransform(u8 *work) {
    u8 *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = billAllocateAnimatedTransformEntries(config);
    ((effBillFrameState_t *)node)->asset = effDuplicateResourceRefs(((effBillFrameState_t *)state)->asset);
    effInitializeAlternatingTransformRows(node, config);
    return node;
}

void billReleaseAlternatingTransformNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

/* Spawn and advance the kind-4 animated frames: each live entry spins with
 * an accelerating rate, grows its radius over drawProgress frames and is
 * drawn as a four-vertex frame, faded in and out over its lifetime. */
void billAdvanceAnimatedFrameTransforms(effClassWork_t *work) {
    effBillAnimatedFrameConfig_t *config = (effBillAnimatedFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effTrackSet_t *asset;
    effBillAnimatedEntry_t *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 frames;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 firstColor;
    u32 secondColor;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 direction[4];
    f32 radius[4];
    f32 width[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (effBillFrameState_t *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (effBillAnimatedEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->colorB;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    frames = config->drawProgress;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 4, colors += 4) {
        effBillAnimatedEntry_t *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (dds3GetRandA((effRandState_t *)effSharedRandomState) & 1) entry->spin = -entry->spin;
                entry->radius = 0.0f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radiusStep = config->radius * (randomValue * config->radiusJitter + (1.0f - config->radiusJitter)) / (f32)frames;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->width = config->width * (randomValue * config->widthJitter + (1.0f - config->widthJitter));
                if (randomAge != 0) {
                    entry->age = (s32)(dds3GetRandA((effRandState_t *)effSharedRandomState) % (u32)lifetime);
                    entry->radius += entry->radiusStep * (f32)(entry->age % frames);
                } else {
                    entry->age = 0;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 spin = entry->spin;
            f32 time = (f32)age;
            f32 angle;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            if (spin > 0.0f) angle = (spin + (acceleration * time) * 0.5f) * time;
            else angle = (spin - (acceleration * time) * 0.5f) * time;
            angle += entry->angle;
            if (age < frames) entry->radius += entry->radiusStep;
            SDF_VU0_LOADVECTOR(effOnesVector);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radius);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_LOADVECTOR_B(radius);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_STOREVECTOR(vertices + 2);
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR_B(vertices + 3);
            SDF_VU0_LOADVECTOR(width);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            VU0_NEGATE_XYZ(vf11);
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_ADDVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(vertices + 1);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            colors[0] = secondPacked;
            colors[1] = secondPacked;
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[2] = firstPacked;
            colors[3] = firstPacked;
            entry->age = (s32)((u32)age + 1u);
        }
    }
}

void billUpdateAlternatingDrawColorAndTransform(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void billResetEmitterIndices(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x18;
        }
    }
}

u8 *billAllocEmitterNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((effBillTimedHeader_t *)config)->count * 0x18 + headerSize);
    u8 *body = (u8 *)sdfMemLock(base);
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeEmitterRows(u8 *work, u8 *descriptor) {
    u32 count = ((effBillTimedHeader_t *)descriptor)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((effBillFrameState_t *)work)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateEmitterTransform(effBillEmitterFrameConfig_t *config, u32 resource) {
    u8 *node = billAllocEmitterNode(config);
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 4, resource);
    billInitializeEmitterRows(node, config);
    return node;
}

u8 *billCloneEmitterTransform(u8 *work) {
    u8 *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = billAllocEmitterNode(config);
    ((effBillFrameState_t *)node)->asset = effDuplicateResourceRefs(((effBillFrameState_t *)state)->asset);
    billInitializeEmitterRows(node, config);
    return node;
}

void billReleaseEmitterNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

void billAdvanceEmitterInstances(effClassWork_t *work) {
    effBillEmitterFrameConfig_t *config = (effBillEmitterFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effTrackSet_t *asset;
    effEmitterRow_t *entries;
    s32 lifetime;
    s32 remainingSpawn;
    u32 randomAge;
    s32 item;
    s32 itemCount;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startAge;
    s32 endAge;
    s32 ageStep;
    s32 retiredState;
    s32 waitingState;
    s32 inactiveState;
    u32 firstColor;
    u32 secondColor;
    u32 respawn;
    u32 *colors;
    u128 *vertices;
    f32 firstVector[4];
    f32 secondVector[4];
    f32 direction[4];
    f32 width[4];
    f32 radial[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) return;
    resource = (effBillFrameState_t *)work->resource;
    lifetime = config->frame.lifetime;
    entries = (effEmitterRow_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) return;
    if (config->reverse != 0) {
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
        startAge = lifetime;
        endAge = 0;
        ageStep = -1;
    } else {
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
        startAge = 0;
        endAge = lifetime;
        ageStep = 1;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }
    firstColor = config->firstColor;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    direction[1] = 0.0f;
    direction[3] = 0.0f;
    eeUnpackRGBAWordFloat(firstColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(firstVector);
    secondColor = config->secondColor;
    eeUnpackRGBAWordFloat(secondColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(secondVector);
    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) return;
    waitingState = -1;
    inactiveState = -2;
    retiredState = respawn != 0 ? waitingState : inactiveState;
    item = itemCount;
    for (; item != 0; item--, entries++, vertices += 4, colors += 4) {
        effEmitterRow_t *entry = entries;
        s32 age = entry->age;
        if (age == inactiveState) continue;
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 jitter;
                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                jitter = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * jitter + (1.0f - jitter));
                jitter = config->offsetJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->offset = config->offset * (randomValue * jitter + (1.0f - jitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->speed = config->speed * (randomValue * config->speedJitter + (1.0f - config->speedJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                jitter = randomValue * config->widthJitter + (1.0f - config->widthJitter);
                entry->width = config->width * jitter;
                if (randomAge != 0) entry->age = (s32)(dds3GetRandA((effRandState_t *)effSharedRandomState) % (u32)lifetime);
                else entry->age = startAge;
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
        } else if (age == endAge) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 time = (f32)age;
            f32 motionTime = time;
            f32 speed;
            f32 angle = entry->angle;
            f32 distance;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 secondPacked;
            u32 firstPacked;
            if (acceleration < 0.0f) {
                f32 stopTime;
                speed = entry->speed;
                stopTime = (-speed / (acceleration * 0.5f)) * 0.5f;
                if (stopTime < time) motionTime = stopTime;
            } else {
                speed = entry->speed;
            }
            distance = (speed + (acceleration * motionTime) * 0.5f) * motionTime;
            distance += entry->offset;
            direction[0] = sdfCos(angle);
            direction[2] = sdfSin(angle);
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR_NOMEM(radial);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, distance);
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            SDF_VU0_GETVECTOR_X(cosine);
            SDF_VU0_GETVECTOR_Z(sine);
            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(width);
            SDF_VU0_LOADVECTOR(radial);
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);
            SDF_VU0_LOADVECTOR(radial);
            SDF_VU0_NEGVECTOR_XYZ();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);
            if (age < fadeStart) alpha = time / (f32)fadeStart;
            else if (fadeEnd < age) alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            else alpha = 1.0f;
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(secondVector);
            SDF_VU0_MULVECTOR();
            secondPacked = eePackRGBAWord(128.0f);
            colors[0] = secondPacked;
            colors[1] = secondPacked;
            SDF_VU0_LOADVECTOR(firstVector);
            SDF_VU0_MULVECTOR();
            firstPacked = eePackRGBAWord(128.0f);
            colors[2] = firstPacked;
            colors[3] = firstPacked;
            entry->age = (s32)((u32)age + (u32)ageStep);
        }
    }
}

void billUpdateEmitterDrawColorAndTransform(u8 *work) {
    u8 *config = ((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void effClearStripFrames(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x28;
        }
    }
}

u8 *billAllocStripNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((effBillTimedHeader_t *)config)->count * 0x28 + headerSize);
    u8 *body = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeStripRows(u8 *node, u8 *config) {
    u32 count = ((effBillTimedHeader_t *)config)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((effBillFrameState_t *)node)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateStripTransform(effBillStripFrameConfig_t *config, u32 resource) {
    u8 *node = billAllocStripNode(config);

    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 3, resource);
    billInitializeStripRows(node, config);
    return node;
}

u8 *billCloneStripTransform(u8 *work) {
    u8 *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = billAllocStripNode(config);
    ((effBillFrameState_t *)node)->asset = effDuplicateResourceRefs(((effBillFrameState_t *)state)->asset);
    billInitializeStripRows(node, config);
    return node;
}

void billReleaseStripNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

void billAdvanceStripInstances(effClassWork_t *work) {
    effBillStripFrameConfig_t *config = (effBillStripFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effBillStripEntry_t *entries;
    effTrackSet_t *asset;
    s32 lifetime;
    s32 itemCount;
    s32 remainingSpawn;
    u32 randomAge;
    s32 remainingItems;
    s32 fadeStart;
    s32 fadeEnd;
    s32 waitingState;
    s32 inactiveState;
    s32 retiredState;
    u32 respawn;
    u32 colorA;
    u32 colorB;
    u32 *colors;
    u128 *vertices;
    f32 colorVectorA[4];
    f32 colorVectorB[4];
    f32 center[4];
    f32 direction[4];
    f32 lateral[4];
    f32 acceleration;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) {
        return;
    }
    lifetime = config->frame.lifetime;
    resource = (effBillFrameState_t *)work->resource;
    entries = (effBillStripEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) {
        return;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }

    colorA = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    center[1] = 0.0f;
    center[3] = 0.0f;
    eeUnpackRGBAWordFloat(colorA, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorA);
    colorB = config->colorB;
    eeUnpackRGBAWordFloat(colorB, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorB);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    respawn = config->frame.respawn;
    waitingState = -1;
    inactiveState = -2;
    {
    u32 packedB[4];
    u32 packedA[4];

    for (remainingItems = 0; remainingItems < itemCount; remainingItems++, entries++, vertices += 4, colors += 4) {
        effBillStripEntry_t *entry = entries;
        s32 age = entry->age;
        f32 time;

        retiredState = respawn != 0 ? waitingState : inactiveState;
        if (age == inactiveState) {
            continue;
        }
        if (age == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 start;
                f32 end;

                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->lengthStart * (randomValue * config->lengthStartJitter + (1.0f - config->lengthStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->lengthEnd * (randomValue * config->lengthEndJitter + (1.0f - config->lengthEndJitter));
                entry->length = start;
                entry->lengthStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->radiusStart * (randomValue * config->radiusStartJitter + (1.0f - config->radiusStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->radiusEnd * (randomValue * config->radiusEndJitter + (1.0f - config->radiusEndJitter));
                entry->radius = start;
                entry->radiusStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                start = config->tiltStart * (randomValue * config->tiltStartJitter + (1.0f - config->tiltStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                end = config->tiltEnd * (randomValue * config->tiltEndJitter + (1.0f - config->tiltEndJitter));
                entry->tilt = start;
                entry->tiltStep = (end - start) / (f32)lifetime;

                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->width = config->width * (randomValue * config->widthJitter + (1.0f - config->widthJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (randomValue * config->spinJitter + (1.0f - config->spinJitter));
                if (randomAge != 0) {
                    time = (f32)(dds3GetRandA((struct effRandState_t *)effSharedRandomState) % (u32)lifetime);
                    entry->age = (s32)time;
                    entry->length += entry->lengthStep * time;
                    entry->radius += entry->radiusStep * time;
                    entry->tilt += entry->tiltStep * time;
                } else {
                    entry->age = 0;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1);
            }
        } else if (age >= lifetime) {
            u32 *color = colors;
            u128 *position = vertices;
            s32 count = 3;
            do {
                VU0_STORE_VF(vf0, position);
                *color = 0;
                position++;
                color++;
            } while (--count >= 0);
            entry->age = retiredState;
        } else {
            f32 angle;
            f32 radius;
            f32 tilt;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 packedColor;

            time = (f32)age;
            angle = (entry->spin + (acceleration * time) * 0.5f) * time + entry->angle;
            radius = entry->radius;
            tilt = entry->tilt;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);

            center[0] = cosine * radius;
            center[2] = sine * radius;
            direction[0] = cosine * tilt;
            direction[1] = tilt + -1.0f;
            direction[2] = sine * tilt;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_C();

            direction[0] = sine;
            direction[1] = 0.0f;
            cosine = -cosine;
            direction[2] = cosine;
            SDF_VU0_LOADVECTOR(direction);
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR_NOMEM(lateral);
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);
            SDF_VU0_COPYVECTOR_C_TO_B();
            VU0_SCALE_VF(vf11, entry->length);
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 0);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);

            entry->length += entry->lengthStep;
            entry->tilt += entry->tiltStep;
            entry->radius += entry->radiusStep;
            if (age < fadeStart) {
                alpha = time / (f32)fadeStart;
            } else if (fadeEnd < age) {
                alpha = (f32)(s32)((u32)lifetime - (u32)age) / (f32)(s32)((u32)lifetime - (u32)fadeEnd);
            } else {
                alpha = 1.0f;
            }
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(colorVectorB);
            SDF_VU0_MULVECTOR();
            {
                EE_MMI_RGBA_PACK_UNIT(packedB[0], 128.0f);
                packedColor = packedB[0];
                colors[0] = packedColor;
                colors[1] = packedColor;
            }
            SDF_VU0_LOADVECTOR(colorVectorA);
            SDF_VU0_MULVECTOR();
            {
                EE_MMI_RGBA_PACK_UNIT(packedA[0], 128.0f);
                packedColor = packedA[0];
                colors[2] = packedColor;
                colors[3] = packedColor;
            }
            entry->age = (s32)((u32)age + 1);
        }
    }
    }
}

void billUpdateStripDrawColorAndTransform(u8 *work) {
    u8 *config = ((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void billResetTrailIndices(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    u8 *entry = ((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            *(s32 *)entry = -1;
            entry += 0x2C;
        }
    }
}

u8 *billCreateTrailNode(effBillTrailFrameConfig_t *config, u32 resource) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->frame.output.timed.count * 0x2C + headerSize);
    u8 *cursor = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *header = cursor;
    /* Read the configured count before writing the retained header. */
    u32 frameCount = config->frame.output.timed.count;

    cursor += headerSize;
    ((effBillFrameState_t *)header)->allocation = base;
    *(u8 **)header = cursor;
    ((effBillFrameState_t *)header)->asset = effCreateTrackSetWithSharedReferences(frameCount, 0, resource);
    return header;
}

void billReleaseTrailNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

/* NON_MATCHING: 58 of 490 words match. VU0 macros keep vectors in registers where retail spills to stack; body from DDS C, checked against pseudocode. */
#ifdef NON_MATCHING
typedef struct effBillTrailEntry_t {
    s32 age;
    f32 angle;
    f32 spin;
    f32 length;
    f32 lengthStep;
    f32 radius;
    f32 radiusStep;
    f32 tilt;
    f32 tiltStep;
    f32 widthA;
    f32 widthB;
} effBillTrailEntry_t;

typedef char effBillTrailEntrySizeCheck[(sizeof(effBillTrailEntry_t) == 0x2C) ? 1 : -1];

void billAdvanceTrailInstances(effClassWork_t *work) {
    effBillTrailFrameConfig_t *config = (effBillTrailFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effBillTrailEntry_t *entry;
    effTrackSet_t *asset;
    s32 lifetime;
    u32 randomAge;
    s32 itemCount;
    s32 remainingSpawn;
    s32 item;
    s32 fadeStart;
    s32 fadeEnd;
    u32 respawn;
    u32 *colors;
    u128 *vertices;
    f32 colorVectorA[4];
    f32 colorVectorB[4];
    f32 center[4];
    f32 direction[4];
    f32 lateralA[4];
    f32 lateralB[4];
    f32 acceleration;
    f32 scale;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) {
        return;
    }
    lifetime = config->frame.lifetime;
    resource = (effBillFrameState_t *)work->resource;
    entry = (effBillTrailEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) {
        return;
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }

    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    center[1] = 0.0f;
    center[3] = 0.0f;
    eeUnpackRGBAWordFloat(config->colorA & 0xFFFFFF, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorA);
    eeUnpackRGBAWordFloat(config->colorB & 0xFFFFFF, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorB);
    fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
    fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    acceleration = config->spinAccel;
    respawn = config->frame.respawn;

    for (item = 0; item < itemCount; item++, entry++, vertices += 5, colors += 5) {
        s32 age = entry->age;
        f32 time;

        if (age == -2) {
            continue;
        }
        if (age == -1) {
            if (remainingSpawn != 0) {
                f32 random;
                f32 start;
                f32 end;

                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                random = dds3GetRandFloatA(effSharedRandomState);
                start = config->lengthStart * (random * config->lengthStartJitter + (1.0f - config->lengthStartJitter));
                random = dds3GetRandFloatA(effSharedRandomState);
                end = config->lengthEnd * (random * config->lengthEndJitter + (1.0f - config->lengthEndJitter));
                entry->length = start;
                entry->lengthStep = (end - start) / (f32)lifetime;

                random = dds3GetRandFloatA(effSharedRandomState);
                start = config->radiusStart * (random * config->radiusStartJitter + (1.0f - config->radiusStartJitter));
                random = dds3GetRandFloatA(effSharedRandomState);
                end = config->radiusEnd * (random * config->radiusEndJitter + (1.0f - config->radiusEndJitter));
                entry->radius = start;
                entry->radiusStep = (end - start) / (f32)lifetime;

                random = dds3GetRandFloatA(effSharedRandomState);
                start = config->tiltStart * (random * config->tiltStartJitter + (1.0f - config->tiltStartJitter));
                random = dds3GetRandFloatA(effSharedRandomState);
                end = config->tiltEnd * (random * config->tiltEndJitter + (1.0f - config->tiltEndJitter));
                entry->tilt = start;
                entry->tiltStep = (end - start) / (f32)lifetime;

                random = dds3GetRandFloatA(effSharedRandomState);
                scale = random * config->widthJitter + (1.0f - config->widthJitter);
                entry->widthA = config->widthA * scale;
                entry->widthB = config->widthB * scale;
                random = dds3GetRandFloatA(effSharedRandomState);
                entry->spin = config->spin * (random * config->spinJitter + (1.0f - config->spinJitter));
                if (randomAge != 0) {
                    time = (f32)(dds3GetRandA(effSharedRandomState) % (u32)lifetime);
                    entry->age = (s32)time;
                    entry->length += entry->lengthStep * time;
                    entry->radius += entry->radiusStep * time;
                    entry->tilt += entry->tiltStep * time;
                } else {
                    entry->age = 0;
                }
                remainingSpawn--;
            }
        } else if (age >= lifetime) {
            s32 vertex;

            for (vertex = 0; vertex < 5; vertex++) {
                SDF_VU0_STOREIDENTITYVECTOR(vertices + vertex);
                colors[vertex] = 0;
            }
            entry->age = respawn != 0 ? -1 : -2;
        } else {
            f32 angle;
            f32 radius;
            f32 tilt;
            f32 cosine;
            f32 sine;
            u32 packed;

            time = (f32)age;
            angle = (entry->spin + (acceleration * time) * 0.5f) * time + entry->angle;
            radius = entry->radius;
            tilt = entry->tilt;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);

            /* The spoke's root on the ring, and its unit direction out and down. */
            center[0] = cosine * radius;
            center[2] = sine * radius;
            direction[0] = cosine * tilt;
            direction[1] = tilt + -1.0f;
            direction[2] = sine * tilt;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_NORMALIZE();
            SDF_VU0_COPYVECTOR_A_TO_C();

            /* The tangent, scaled to each end's half width. */
            direction[0] = sine;
            direction[1] = 0.0f;
            cosine = -cosine;
            direction[2] = cosine;
            SDF_VU0_LOADVECTOR(direction);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_SCALEVECTOR_BITS(entry->widthA);
            SDF_VU0_SCALEVECTOR_B_BITS(entry->widthB);
            SDF_VU0_STOREVECTOR_NOMEM(lateralA);
            SDF_VU0_STOREVECTOR_B_NOMEM(lateralB);

            /* The root end: its center, then either side. */
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_STOREVECTOR(vertices + 3);
            SDF_VU0_LOADVECTOR_B(lateralA);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 4);

            /* The far end, a length out along the direction, either side. */
            SDF_VU0_COPYVECTOR_C_TO_B();
            SDF_VU0_SCALEVECTOR_B_BITS(entry->length);
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_LOADVECTOR_B(lateralB);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 0);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);

            entry->length += entry->lengthStep;
            entry->tilt += entry->tiltStep;
            entry->radius += entry->radiusStep;
            if (age < fadeStart) {
                scale = time / (f32)fadeStart;
            } else if (fadeEnd < age) {
                scale = (f32)(lifetime - age) / (f32)(lifetime - fadeEnd);
            } else {
                scale = 1.0f;
            }
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            SDF_VU0_SCALEVECTOR_B_BITS(scale);
            SDF_VU0_LOADVECTOR(colorVectorB);
            SDF_VU0_MULVECTOR();
            packed = eePackRGBAWord(128.0f);
            colors[0] = packed;
            colors[1] = packed;
            SDF_VU0_LOADVECTOR(colorVectorA);
            SDF_VU0_MULVECTOR();
            packed = eePackRGBAWord(128.0f);
            /* The far end takes color B with a zero alpha byte; the root's
             * center color A with alpha 0xFF on odd entries and 0x40 on even
             * ones, and its sides only the fade alpha, of 128 or of 255. */
            if (item & 1) {
                colors[3] = packed | 0xFF000000;
                packed = (u32)(scale * 128.0f) << 24;
            } else {
                colors[3] = packed | 0x40000000;
                packed = (u32)(scale * 255.0f) << 24;
            }
            colors[2] = packed;
            colors[4] = packed;
            entry->age = age + 1;
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", billAdvanceTrailInstances);
#endif

void billUpdateTrailDrawColorAndTransform(u8 *work) {
    u8 *config = ((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

void billResetQuadIndices(u8 *work) {
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *config = ((effClassWork_t *)work)->payload;
    effTrackSet_t *asset = ((effBillFrameState_t *)state)->asset;
    s32 remaining = ((effBillTimedHeader_t *)config)->count;
    effBillQuadEntry_t *entry = (effBillQuadEntry_t *)((effBillFrameState_t *)state)->entries;

    memset(asset->buffer, 0, asset->rows * 0x10);
    if (remaining > 0) {
        s32 i;
        for (i = remaining; i != 0; --i) {
            entry->timer = -1;
            entry++;
        }
    }
}

u8 *billAllocQuadNode(u8 *config) {
    u32 headerSize = 0x10;
    struct sdfMemHandle_t *base = sdfMemAllocDirect(((effBillTimedHeader_t *)config)->count * sizeof(effBillQuadEntry_t) + headerSize);
    u8 *body = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)base));
    u8 *node = body;

    body += headerSize;
    ((effBillFrameState_t *)node)->allocation = base;
    *(u8 **)node = body;
    return node;
}

void billInitializeQuadRows(u8 *work, u8 *descriptor) {
    u32 count = ((effBillTimedHeader_t *)descriptor)->count;
    u32 index;
    f32 *transform;

    if (count == 0) {
        return;
    }
    transform = (f32 *)((effBillFrameState_t *)work)->asset->columns;
    index = 0;
    for (; index < count; index++, transform += 8) {
        if (index & 1) {
            transform[0] = 0.0f;
            transform[1] = 0.0f;
            transform[2] = 0.5f;
            transform[3] = 0.0f;
            transform[4] = 0.0f;
            transform[5] = 1.0f;
            transform[6] = 0.5f;
        } else {
            transform[0] = 0.5f;
            transform[1] = 0.0f;
            transform[2] = 1.0f;
            transform[3] = 0.0f;
            transform[4] = 0.5f;
            transform[5] = 1.0f;
            transform[6] = 1.0f;
        }
        transform[7] = 1.0f;
    }
}

u8 *billCreateQuadTransform(effBillQuadFrameConfig_t *config, u32 resource) {
    u8 *node = billAllocQuadNode(config);
    ((effBillFrameState_t *)node)->asset = effCreateTrackSetWithSharedReferences(config->frame.output.timed.count, 4, resource);
    billInitializeQuadRows(node, config);
    return node;
}

u8 *billCloneQuadTransform(u8 *work) {
    u8 *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = billAllocQuadNode(config);
    ((effBillFrameState_t *)node)->asset = effDuplicateResourceRefs(((effBillFrameState_t *)state)->asset);
    billInitializeQuadRows(node, config);
    return node;
}

void billReleaseQuadNode(s32 work) {
    effReleaseResourceRefs(((effBillFrameState_t *)work)->asset);
    sdfMemFreeDirect(((effBillFrameState_t *)work)->allocation);
}

void billAdvanceQuadInstances(effClassWork_t *work) {
    effBillQuadFrameConfig_t *config = (effBillQuadFrameConfig_t *)work->payload;
    effBillFrameState_t *resource;
    effBillQuadEntry_t *entries;
    effTrackSet_t *asset;
    s32 lifetime;
    s32 itemCount;
    s32 remainingSpawn;
    u32 randomAge;
    s32 remainingItems;
    s32 fadeStart;
    s32 fadeEnd;
    s32 startFrame;
    s32 stopFrame;
    s32 frameStep;
    s32 stopEntry;
    s32 waitingState;
    s32 inactiveState;
    u32 respawn;
    u32 colorA;
    u32 colorB;
    f32 colorVectorA[4];
    f32 colorVectorB[4];
    f32 center[4];
    f32 lateral[4];
    f32 basis[4];
    f32 acceleration;
    u32 *colors;
    u128 *vertices;

    if (config->frame.output.timed.time.progress < work->frame &&
        config->frame.output.timed.time.progress != 0) {
        return;
    }

    lifetime = config->frame.lifetime;
    resource = (effBillFrameState_t *)work->resource;
    entries = (effBillQuadEntry_t *)resource->entries;
    asset = resource->asset;
    itemCount = (s32)config->frame.output.timed.count;
    if (lifetime == 0) {
        return;
    }

    if (config->reverse != 0) {
        startFrame = lifetime;
        stopFrame = 0;
        frameStep = -1;
        fadeStart = (s32)((1.0f - config->frame.fadeOutFraction) * (f32)lifetime);
        fadeEnd = (s32)((1.0f - config->frame.fadeInFraction) * (f32)lifetime);
    } else {
        startFrame = 0;
        stopFrame = lifetime;
        frameStep = 1;
        fadeStart = (s32)(config->frame.fadeInFraction * (f32)lifetime);
        fadeEnd = (s32)(config->frame.fadeOutFraction * (f32)lifetime);
    }
    if (config->frame.spawnAllOnStart != 0 && work->frame == 0) {
        remainingSpawn = itemCount;
        randomAge = 1;
    } else {
        remainingSpawn = config->frame.spawnCount;
        randomAge = 0;
    }

    colorA = config->colorA;
    vertices = (u128 *)asset->buffer;
    colors = (u32 *)asset->tail;
    center[1] = 0.0f;
    center[3] = 0.0f;
    eeUnpackRGBAWordFloat(colorA, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorA);
    colorB = config->colorB;
    eeUnpackRGBAWordFloat(colorB, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(colorVectorB);

    acceleration = config->acceleration;
    respawn = config->frame.respawn;
    if (itemCount <= 0) {
        return;
    }

    waitingState = -1;
    inactiveState = -2;
    stopEntry = respawn != 0 ? waitingState : inactiveState;
    remainingItems = itemCount;
    for (; remainingItems != 0; remainingItems--, entries++, vertices += 4, colors += 4) {
        effBillQuadEntry_t *entry = entries;
        s32 frame = entry->timer;

        if (frame == inactiveState) {
            continue;
        }
        if (frame == waitingState) {
            if (remainingSpawn != 0) {
                f32 randomValue;
                f32 randomFactor;
                f32 heightStartJitter;
                f32 heightEndJitter;
                f32 velocityJitter;
                f32 widthJitter;
                f32 tiltJitter;
                f32 radialStart;
                f32 radialEnd;

                entry->angle = dds3GetRandFloatA(effSharedRandomState) * 6.28318500518798828125f;
                randomFactor = config->radiusJitter;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                entry->radius = config->radius * (randomValue * randomFactor + (1.0f - randomFactor));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                heightStartJitter = config->heightStartJitter;
                radialStart = config->heightStart * (randomValue * heightStartJitter + (1.0f - heightStartJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                heightEndJitter = config->heightEndJitter;
                radialEnd = config->heightEnd * (randomValue * heightEndJitter + (1.0f - heightEndJitter));
                entry->height = radialStart;
                entry->heightStep = (radialEnd - radialStart) / (f32)lifetime;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                velocityJitter = config->velocityJitter;
                entry->velocity = config->velocity * (randomValue * velocityJitter + (1.0f - velocityJitter));
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                widthJitter = config->widthJitter;
                randomFactor = randomValue * widthJitter + (1.0f - widthJitter);
                entry->width = config->width * randomFactor;
                randomValue = dds3GetRandFloatA(effSharedRandomState);
                tiltJitter = config->tiltJitter;
                entry->tilt = config->tilt * (randomValue * tiltJitter + (1.0f - tiltJitter));
                if (randomAge != 0) {
                    entry->timer = (s32)(dds3GetRandA((struct effRandState_t *)effSharedRandomState) % (u32)lifetime);
                } else {
                    entry->timer = startFrame;
                }
                remainingSpawn = (s32)((u32)remainingSpawn - 1u);
            }
            continue;
        }
        if (frame == stopFrame) {
            {
                s32 corner;
                for (corner = 0; corner < 4; corner++) {
                    VU0_STORE_VF(vf0, vertices + corner);
                    colors[corner] = 0;
                }
            }
            entry->timer = stopEntry;
            continue;
        }

        {
            f32 angle = entry->angle;
            f32 time = (f32)frame;
            f32 velocity;
            f32 shapeFactor;
            f32 motionDistance;
            f32 radialOffset;
            f32 cosine;
            f32 sine;
            f32 alpha;
            u32 packedColor;

            if (acceleration < 0.0f) {
                f32 maxTime;
                velocity = entry->velocity;
                maxTime = ((-velocity) / (acceleration * 0.5f)) * 0.5f;
                if (maxTime < time) {
                    time = maxTime;
                }
            } else {
                velocity = entry->velocity;
            }
            motionDistance = (velocity + (acceleration * time) * 0.5f) * time;
            radialOffset = entry->height + entry->heightStep * time;
            shapeFactor = entry->tilt;
            cosine = sdfCos(angle);
            sine = sdfSin(angle);

            center[0] = cosine * shapeFactor;
            center[1] = shapeFactor + -1.0f;
            center[2] = sine * shapeFactor;
            SDF_VU0_LOADVECTOR(center);
            SDF_VU0_COPYVECTOR_A_TO_B();
            VU0_SCALE_VF(vf10, entry->radius);
            SDF_VU0_STOREVECTOR(basis);
            SDF_VU0_COPYVECTOR_B_TO_A();
            VU0_SCALE_VF(vf10, motionDistance);
            SDF_VU0_STOREVECTOR(center);
            center[0] += cosine * radialOffset;
            center[2] += sine * radialOffset;

            VU0_SCALAR_OP(sine, "vaddx.x vf10, vf0, vf2x");
            VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
            cosine = -cosine;
            VU0_SCALAR_OP(cosine, "vaddx.z vf10, vf0, vf2x");
            VU0_SCALE_VF(vf10, entry->width);
            SDF_VU0_STOREVECTOR(lateral);

            SDF_VU0_LOADVECTOR_C(center);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 0);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 1);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_NEGVECTOR_XYZ();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_LOADVECTOR_B(lateral);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 2);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_SUBVECTOR();
            SDF_VU0_STOREVECTOR(vertices + 3);

            if (frame < fadeStart) {
                alpha = (f32)frame / (f32)fadeStart;
            } else if (fadeEnd < frame) {
                alpha = (f32)(lifetime - frame) / (f32)(lifetime - fadeEnd);
            } else {
                alpha = 1.0f;
            }
            SDF_VU0_LOADVECTOR_B(effOnesVector);
            VU0_SCALE_VF(vf11, alpha);
            SDF_VU0_LOADVECTOR(colorVectorB);
            SDF_VU0_MULVECTOR();
            packedColor = eePackRGBAWord(128.0f);
            colors[0] = packedColor;
            colors[1] = packedColor;
            SDF_VU0_LOADVECTOR(colorVectorA);
            SDF_VU0_MULVECTOR();
            packedColor = eePackRGBAWord(128.0f);
            colors[2] = packedColor;
            colors[3] = packedColor;
            entry->timer = (s32)((u32)frame + (u32)frameStep);
        }
    }
}

void billUpdateQuadDrawColorAndTransform(u8 *work) {
    u8 *config = ((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
    out->flag = ((effBillFrameHeader_t *)config)->mode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawTrackSet(out, (effMatrix4_t *)mtx);
}

effClassWork_t *effAllocateActiveInstanceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effActiveInstanceOperations[kind].payloadSize;
    u8 *effect = sdfChipAllocDirect(size + headerSize);
    ((effClassWork_t *)effect)->payload = effect + headerSize;
    ((effClassWork_t *)effect)->color = 0x80808080;
    ((effClassWork_t *)effect)->scale = 1.0f;
    ((effClassWork_t *)effect)->kind = kind;
    ((effClassWork_t *)effect)->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect + 0x10);
    memcpy(((effClassWork_t *)effect)->payload, source, size);
    return (effClassWork_t *)effect;
}

effClassWork_t *effCreateResourceInstanceA(u16 kind, void *source, u32 option) {
    effClassWork_t *effect = effAllocateActiveInstanceWork(kind, source);
    effect->resource = (void *)effActiveInstanceOperations[kind].createResource(source, option);
    effActiveInstanceOperations[kind].initialize(effect);
    return effect;
}

effClassWork_t *effCreateActiveInstanceFromFile(fileJobPayload_t *request) {
    u32 *secondary = fileResolveSecondaryBuffer(request);
    void *source;
    switch (request->primary.selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceA(request->option, source, (u32)secondary);
}

void effDestroyActiveInstanceWork(effClassWork_t *work) {
    effActiveInstanceOperations[work->kind].destroyResource(work->resource);
    sdfChipFreeDirect(work);
}

effClassWork_t *effCreateActiveResource(effClassWork_t *work) {
    effClassWork_t *effect;
    if (effActiveInstanceOperations[work->kind].cloneResource == NULL) {
        effect = effCreateResourceInstanceA(work->kind, work->payload, 0);
    } else {
        void *clonedResource;
        s32 kind;
        effect = effAllocateActiveInstanceWork(work->kind, work->payload);
        clonedResource = (void *)effActiveInstanceOperations[work->kind].cloneResource(work);
        /* Capture the clone's class before publishing its resource. */
        kind = work->kind;
        effect->resource = clonedResource;
        effActiveInstanceOperations[kind].initialize(effect);
    }
    return effect;
}

void effResetActiveInstanceFrame(effClassWork_t *work) {
    effActiveInstanceOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceActiveInstanceFrame(effClassWork_t *work) {
    effActiveInstanceOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDispatchActiveInstanceDraw(effClassWork_t *work) {
    effActiveInstanceOperations[work->kind].draw(work);
}

void effCallSRAnimation7(effClassWork_t *work) {
    if ((effSRAnimation() & 1) == 0) {
        effAdvanceActiveInstanceFrame(work);
    }
    effDispatchActiveInstanceDraw(work);
}

void effSetActiveInstancePosition(effClassWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.position, position);
}

void effSetActiveInstanceRotation(effClassWork_t *work, const f32 rotation[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.orientation, rotation);
}

void effSetActiveInstanceColor(effClassWork_t *work, u32 color) {
    work->color = color;
}

void effSetActiveInstanceScale(effClassWork_t *work, f32 scale) {
    work->scale = scale;
}

INCLUDE_RODATA(const s32, "code_r1_00226EC8", Draw_effMuzzleFlashTable);

effTrackSet_t *effCreateTrackSet(s32 count, u16 kind) {
    s32 rows;
    s32 cols;
    s32 size;
    struct sdfMemHandle_t *base;
    u8 *trackStorage;
    effTrackSet_t *set;

    switch (kind) {
    case 0:
        cols = 0;
        rows = count * 5;
        break;
    case 1:
        cols = 0;
        rows = count * 13;
        break;
    case 2:
        rows = count * 4;
        cols = 0;
        break;
    case 3:
        rows = count * 4;
        cols = rows;
        break;
    case 4:
        rows = count * 4;
        cols = rows;
        break;
    default:
        rows = 0;
        cols = 0;
        break;
    }
    size = ((rows * 2 + cols) * 2 + rows) * 4;
    size = (((size >> 4) + ((size & 0xF) != 0)) << 4);
    base = sdfMemAllocDirect(size + 0x30);
    trackStorage = (u8 *)sdfMemLock(base);
    set = (effTrackSet_t *)(trackStorage + size);
    set->buffer = trackStorage;
    trackStorage += rows * 16;
    if (cols > 0) {
        set->columns = trackStorage;
        trackStorage += cols * 8;
    } else {
        set->columns = 0;
    }
    set->color = 0x80808080;
    set->type = 2;
    set->tail = trackStorage;
    set->rows = rows;
    set->kind = kind;
    set->count = count;
    set->allocation = base;
    set->flag = 0;
    set->shared = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(&effDrawTrackSetWork, 0, 0x2C);
    effDrawTrackSetWork.attr = 0x4000;
    return set;
}

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effLoadFlashTexturesData);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effFlashTextureHandles);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effCreateTrackSetWithSharedReferencesData);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effCreateTrackSetWithSharedReferencesData2);

effTrackSet_t *effCreateTrackSetWithSharedReferences(u32 count, u32 kind, u32 sharedRef) {
    effTrackSet_t *effect = effCreateTrackSet(count, (u16)kind);

    if (effect->columns != 0) {
        if (sharedRef == 0) {
            switch (effect->kind) {
            case 3:
                if (effCreateTrackSetWithSharedReferencesData2[0] == 0) {
                    effData34[0] = effCloneSharedReferenceWithValue(effFlashTextureHandles, 0x100);
                }
                effCreateTrackSetWithSharedReferencesData2[0]++;
                break;
            case 4:
                if (effCreateTrackSetWithSharedReferencesData2[1] == 0) {
                    effData34[1] = effCloneSharedReferenceWithValue(effCreateTrackSetWithSharedReferencesData, 0x101);
                }
                effCreateTrackSetWithSharedReferencesData2[1]++;
                break;
            }
        } else {
            effect->shared = effCreateSharedTextureReference((struct sdfTextureFileHeader_t *)sharedRef);
        }
    }
    return effect;
}

/* Release the track set's retained reference, draw asset, and allocation. */
void effReleaseResourceRefs(effTrackSet_t *refs) {
    if (refs->columns != 0) {
        effRefObj_t *ref = refs->shared;
        if (ref == NULL) {
            switch (refs->kind) {
            case 3:
                if (--effCreateTrackSetWithSharedReferencesData2[0] == 0) {
                    effReleaseSharedReference(effData34[0]);
                    effData34[0] = NULL;
                }
                break;
            case 4:
                if (--effCreateTrackSetWithSharedReferencesData2[1] == 0) {
                    effReleaseSharedReference(effData34[1]);
                    effData34[1] = NULL;
                }
                break;
            }
        } else {
            effReleaseSharedReference(ref);
        }
    }
    sdfMatFreeDelayed(refs->handle);
    sdfMemFreeDirect(refs->allocation);
}

/* Copy a track set: same size and kind, retaining the source's shared reference (or counting one more user of the built-in one). */
effTrackSet_t *effDuplicateResourceRefs(const effTrackSet_t *original) {
    effTrackSet_t *effect = effCreateTrackSet(original->count, original->kind);

    if (effect->columns != 0) {
        if (original->shared != 0) {
            effect->shared = effRetainSharedReference(original->shared);
        } else {
            switch (effect->kind) {
            case 3:
                effCreateTrackSetWithSharedReferencesData2[0]++;
                break;
            case 4:
                effCreateTrackSetWithSharedReferencesData2[1]++;
                break;
            }
        }
    }
    return effect;
}

/* NON_MATCHING: 107 of 321 words match. Register allocation and block order differ from retail; same calls, tables and branches. */
#ifdef NON_MATCHING
extern sdfGrOt_t *effDrawTrackSetData5[];
extern u32 effDrawTrackSetData[];
extern u32 effDrawTrackSetData2[];
extern u32 effDrawTrackSetData3[];
extern u32 effDrawTrackSetData4[];

void effDrawTrackSet(effTrackSet_t *track, const effMatrix4_t *matrix)
{
    sdfGrOt_t *surface;
    sdfDrawTag_t *list;
    effGsPacket_t *packet;
    effRefObj_t *shared;
    sdfTexHandle_t *texture;
    sdfMat_t *handle;
    s32 rows;

    if ((track->color & 0xFF000000) == 0) {
        return;
    }
    surface = effDrawTrackSetData5[track->type];
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    if (matrix == NULL) {
        SDF_VU0_LOADIDENTITY();
    } else {
        VU0_LOAD_MATRIX(matrix);
    }
    sdf3DPrimLinkTm(list, 0);
    if (track->columns != NULL) {
        shared = track->shared;
        texture = NULL;
        if (shared == NULL) {
            if (track->kind == 3) {
                shared = effData34[0];
            } else if (track->kind == 4) {
                shared = effData34[1];
            }
        }
        if (shared != NULL) {
            texture = sdfGetOrCreateTextureForReference(surface, shared);
        }
        sdfMatSetTex(track->handle, texture);
    }
    sdf3DPrimLinkMat(list, track->handle, 0);
    if (track->flag == 0) {
        packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x31801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    rows = track->rows;
    effDrawTrackSetWork.vertexColor = (u32 *)track->tail;
    effDrawTrackSetWork.vertex = (sdfVector4_t *)track->buffer;
    effDrawTrackSetWork.uv = (sdfVector2_t *)track->columns;
    effDrawTrackSetWork.color = track->color;
    switch (track->kind) {
    case 0:
        effDrawTrackSetWork.faceCount = 15;
        effDrawTrackSetWork.pointCount = 25;
        effDrawTrackSetWork.faceIndex = effDrawTrackSetData;
        while (rows >= 25) {
            rows -= 25;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
            effDrawTrackSetWork.vertex += 25;
            effDrawTrackSetWork.vertexColor += 25;
        }
        if (rows >= 5) {
            effDrawTrackSetWork.pointCount = rows;
            effDrawTrackSetWork.faceCount = 3 * (rows / 5);
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
        }
        break;
    case 1:
        effDrawTrackSetWork.faceCount = 12;
        effDrawTrackSetWork.pointCount = 13;
        effDrawTrackSetWork.faceIndex = effDrawTrackSetData2;
        while (rows >= 13) {
            rows -= 13;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
            effDrawTrackSetWork.vertex += 13;
            effDrawTrackSetWork.vertexColor += 13;
        }
        break;
    case 2:
        effDrawTrackSetWork.faceCount = 16;
        effDrawTrackSetWork.pointCount = 32;
        effDrawTrackSetWork.faceIndex = effDrawTrackSetData3;
        while (rows >= 32) {
            rows -= 32;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
            effDrawTrackSetWork.vertex += 32;
            effDrawTrackSetWork.vertexColor += 32;
        }
        if (rows >= 4) {
            effDrawTrackSetWork.pointCount = rows;
            effDrawTrackSetWork.faceCount = 2 * (rows >> 2);
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
        }
        break;
    case 3:
    case 4:
        effDrawTrackSetWork.faceCount = 16;
        effDrawTrackSetWork.pointCount = 32;
        effDrawTrackSetWork.faceIndex = effDrawTrackSetData4;
        while (rows >= 32) {
            rows -= 32;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
            effDrawTrackSetWork.vertex += 32;
            effDrawTrackSetWork.vertexColor += 32;
            effDrawTrackSetWork.uv += 32;
        }
        if (rows >= 4) {
            effDrawTrackSetWork.pointCount = rows;
            effDrawTrackSetWork.faceCount = 2 * (rows >> 2);
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawTrackSetWork));
        }
        break;
    default:
        break;
    }
    if (track->flag == 0) {
        packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x51801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    surface->regFunc(surface, list);
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", effDrawTrackSet);
#endif

void effLoadFlashTextures(void) {
    effLoadFlashTexturesData[0] = sdfDevLoadFileEx("/effect/flash00.tmx", (u32 *)&effFlashTextureHandles, 0);
    effLoadFlashTexturesData[1] = sdfDevLoadFileEx("/effect/flash01.tmx", (u32 *)(&effFlashTextureHandles + 1), 0);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf361);

void effResetRingResourceFrame(effClassWork_t *work) {
    effRingResource_t *ring = work->resource;

    ring->pointSet->color = 0;
}

/* Create a point-set reference and initialize four colors per segment. */
u32 *effSegmentPointerSet(u8 *work) {
    u32 *pointSetRef = sdfChipAllocDirect(4);
    u32 segments = ((effRingSource_t *)work)->segments;
    effPointSet_t *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        ((effRingSource_t *)work)->segments = 3;
        segments = 3;
    }
    pointSet = effCreatePointSet4(segments);
    first = ((effRingSource_t *)work)->firstColor;
    groups = pointSet->rows / 4;
    *pointSetRef = (u32)pointSet;
    entry = (u32 *)pointSet->tail;
    second = ((effRingSource_t *)work)->middleColor;
    third = ((effRingSource_t *)work)->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingResourceHandle(u32 *handle) {
    effAssetQueueRelease((effPointSet_t *)*handle);
    sdfChipFreeDirect(handle);
}

/* Generate the timed ring's four radial point bands; reverse-time mode is retained. */
void effUpdateExpandingRingPoints(effClassWork_t *work) {
    effExpandingRingParams_t *config = work->payload;
    effPointSet_t *points = ((effRingResource_t *)work->resource)->pointSet;
    f32 direction[4];
    f32 bands[3][4];
    f32 time;
    f32 speed;
    f32 acceleration;
    f32 radius;
    f32 angle;
    f32 angleStep;
    f32 limit;
    f32 firstBand;
    f32 secondBand;
    f32 thirdBand;
    f32 (*out)[4];
    u32 groups;
    u32 i;

    if (config->duration < (s32)work->frame && config->duration != 0) {
        return;
    }
    if (config->reverseTime != 0) {
        time = (f32)(s32)((u32)config->duration - work->frame);
    } else {
        time = (f32)(s32)work->frame;
    }
    speed = ffabsf(config->speed);
    acceleration = config->acceleration;
    if (acceleration < 0.0f) {
        limit = -speed / (acceleration * 0.5f) * 0.5f;
        if (limit < time) {
            time = limit;
        }
    }
    radius = (speed + acceleration * time * 0.5f) * time;
    radius += config->baseRadius;
    groups = (u32)(points->rows / 4);
    out = (f32 (*)[4])points->buffer;
    angle = 0.0f;
    angleStep = EFFECT_RING_FULL_TURN / (f32)config->segments;
    direction[1] = 0.0f;
    direction[3] = 1.0f;
    firstBand = config->widths[0] + radius;
    secondBand = firstBand + config->widths[1];
    thirdBand = secondBand + config->widths[2];
    SDF_VU0_LOADVECTOR(effOnesVector);
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_SCALAR_OP_CLOBBER(firstBand, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_STOREVECTOR(bands[0]);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_SCALAR_OP_CLOBBER(secondBand, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_CLOBBER(thirdBand, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_STOREVECTOR(bands[1]);
    SDF_VU0_STOREVECTOR_B(bands[2]);
    for (i = 0; i < groups; i++) {
        direction[0] = sdfCos(angle);
        direction[2] = sdfSin(angle);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_COPYVECTOR_A_TO_B();
        VU0_SCALAR_OP_CLOBBER(radius, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_STOREVECTOR(out[0]);
        SDF_VU0_LOADVECTOR(bands[0]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[1]);
        SDF_VU0_LOADVECTOR(bands[1]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[2]);
        SDF_VU0_LOADVECTOR(bands[2]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[3]);
        angle += angleStep;
        out += 4;
    }
}

void billDrawCellBlendA(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effPointSet_t *out = (effPointSet_t *)(u32)list[0];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    if ((packed & 0xFF000000) != 0) {
        out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
        out->flag = ((effBillOutputHeader_t *)config)->outputMode;
        SDF_VU0_LOADVECTOR(work->transform);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups(out, (effMatrix4_t *)mtx);
    }
}

void effResetClassFrameAndFlags(s32 work) {
    effClassWork_t *effect;

    effect = ((effClassDrawState_t *)((effClassWork_t *)work)->resource)->effect;
    ((effCounterHeader_t *)((effClassDrawState_t *)((effClassWork_t *)work)->resource)->trackSet)->frame = 0;
    effInitializeClassFrame(effect);
}

effClassDrawState_t *effCreateScaledClassDrawState(effRingClassConfig_t *source) {
    u32 count = source->ring.segments;
    u32 size;
    struct sdfMemHandle_t *allocation;
    f32 *scales;
    effClassDrawState_t *state;
    effTrackSet_t *tracks;
    u32 *colors;
    u32 first;
    u32 second;
    u32 i;

    if (count < 3) {
        source->ring.segments = 3;
        count = 3;
    }
    size = count * sizeof(f32);
    allocation = sdfMemAllocDirect(size + sizeof(effClassDrawState_t));
    scales = (f32 *)sdfMemLock(allocation);
    state = (effClassDrawState_t *)((u8 *)scales + size);
    state->allocation = allocation;
    state->scales = scales;
    memcpy(source->classConfig, source, sizeof(source->classConfig));
    state->effect = effCreateClassWork(1, source->classConfig);
    tracks = effCreateTrackSetWithSharedReferences(count, 2, 0);
    first = source->ring.firstColor;
    state->trackSet = tracks;
    colors = (u32 *)tracks->tail;
    second = source->ring.middleColor;
    for (i = 0; i < count; i++) {
        colors[0] = first;
        colors[1] = first;
        colors[2] = (first & second) + (((first ^ second) & 0xFEFEFEFE) >> 1);
        colors[3] = first;
        colors += 4;
        *scales++ = source->scale * (dds3GetRandFloatA(effSharedRandomState) * source->scaleRand + (1.0f - source->scaleRand));
    }
    return state;
}

void effReleaseClassDrawResources(s32 work) {
    effReleaseResourceRefs(((effClassDrawState_t *)work)->trackSet);
    effDestroyClassWork(((effClassDrawState_t *)work)->effect);
    sdfMemFreeDirect(((effClassDrawState_t *)work)->allocation);
}

/* Advance frame geometry and rebuild scaled row offsets with predecessor wrapping. */
void effUpdateScaledClassFrameRows(billCellDrawWork_t *work) {
    u8 *config = work->config;
    s32 limit = work->frameLimit;
    s32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effClassWork_t *classWork;
    f32 *scales;
    effClassFrameResource_t **resource;
    effClassRowOutput_t *output;
    effClassFrameResource_t *frames;
    u8 *src;
    u8 *dst;
    u8 *wrap;
    u32 count;
    u32 lastFrame;
    u32 i;

    classWork = (effClassWork_t *)list[1];
    resource = (effClassFrameResource_t **)classWork->resource;
    output = (effClassRowOutput_t *)list[2];
    scales = (f32 *)list[0];

    if (progress < limit && progress != 0) {
        return;
    }
    effAdvanceClassFrame(classWork);
    count = ((effBillTimedHeader_t *)config)->count;
    frames = *resource;
    lastFrame = frames->frameCount - 1;
    dst = output->rows;
    src = frames->rows;
    if (config[0xC4] != 0) {
        return;
    }
    i = 0;
    if (count == 0) {
        return;
    }
    wrap = src + lastFrame * 0x10 - 0x60;
    do {
        SDF_VU0_LOADVECTOR(src + 0x10);
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(src + 0x30);
        SDF_VU0_STOREVECTOR(dst + 0x20);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        VU0_SCALAR_OP(*scales, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(dst);
        SDF_VU0_COPYVECTOR(dst + 0x10, src + 0x50);
        if (i == 0) {
            SDF_VU0_COPYVECTOR(dst + 0x30, wrap);
        } else {
            SDF_VU0_COPYVECTOR(dst + 0x30, src - 0x30);
        }
        i++;
        dst += 0x40;
        wrap += 0x40;
        src += 0x40;
        scales++;
    } while (i < count);
}

void billDrawClassUpdatedCellBlend(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillTimedHeader_t *)config)->time.progress;
    u32 *list = work->instances;
    effTrackSet_t *out = (effTrackSet_t *)list[2];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillTimedHeader_t *)config)->colorTrack, &((effBillTimedHeader_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK_UNCLOBBERED(packed);
    blended[0] = packed;
    out->color = packed;
    if (packed & 0xFF000000) {
        effClassWork_t *dst = (effClassWork_t *)list[1];

        dst->color = work->baseColor;
        dst->scale = work->scale;
        SDF_VU0_COPYVECTOR(dst->transform, work);
        SDF_VU0_COPYVECTOR(dst->transform + 0x10, work->transform);
        effRunClassPostFrame((s32)dst);
        out->type = ((effBillTimedHeader_t *)config)->alphaTrack.surfaceIndex;
        out->flag = ((effBillOutputHeader_t *)config)->outputMode;
        SDF_VU0_LOADVECTOR(work->transform);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawTrackSet(out, (effMatrix4_t *)mtx);
    }
}

void effResetClassRingFrame(effClassWork_t *work) {
    effRingResource_t *ring = work->resource;

    ring->pointSet->color = 0;
}

/* Create a point-set reference and initialize four colors per segment. */
effRingResource_t *effCreateRingHandle(effRadialRingParams_t *work) {
    effRingResource_t *pointSetRef = sdfChipAllocDirect(sizeof(effRingResource_t));
    u32 segments = work->segments;
    effPointSet_t *pointSet;
    u32 *entry;
    u32 groups;
    u32 i;
    u32 first;
    u32 second;
    u32 third;

    if (segments < 3) {
        work->segments = 3;
        segments = 3;
    }
    pointSet = effCreatePointSet4(segments);
    first = work->firstColor;
    groups = pointSet->rows / 4;
    pointSetRef->pointSet = pointSet;
    entry = (u32 *)pointSet->tail;
    second = work->middleColor;
    third = work->lastColor;
    for (i = 0; i < groups; i++) {
        entry[0] = first;
        entry[1] = second;
        entry[2] = second;
        entry[3] = third;
        entry += 4;
    }
    return pointSetRef;
}

void effReleaseRingHandle(effRingResource_t *handle) {
    effAssetQueueRelease(handle->pointSet);
    sdfChipFreeDirect(handle);
}

void effUpdateCenteredRadialRingVertices(effClassWork_t *work) {
    effRadialRingParams_t *config = work->payload;
    effPointSet_t *points = ((effRingResource_t *)work->resource)->pointSet;
    f32 direction[4];
    f32 bands[3][4];
    f32 time;
    f32 speed;
    f32 acceleration;
    f32 radius;
    f32 angle;
    f32 angleStep;
    f32 limit;
    f32 firstBand;
    f32 secondBand;
    f32 thirdBand;
    f32 (*out)[4];
    u32 groups;
    u32 i;

    if ((s32)config->duration < (s32)work->frame && config->duration != 0) {
        return;
    }
    if (config->reverseTime != 0) {
        time = (f32)(s32)((u32)config->duration - work->frame);
    } else {
        time = (f32)(s32)work->frame;
    }
    speed = ffabsf(config->speed);
    acceleration = config->acceleration;
    if (acceleration < 0.0f) {
        limit = -speed / (acceleration * 0.5f) * 0.5f;
        if (limit < time) {
            time = limit;
        }
    }
    radius = (speed + acceleration * time * 0.5f) * time;
    radius += config->baseRadius;
    groups = (u32)(points->rows / 4);
    out = (f32 (*)[4])points->buffer;
    angle = 0.0f;
    angleStep = EFFECT_RING_FULL_TURN / (f32)config->segments;
    direction[1] = 0.0f;
    direction[3] = 1.0f;
    firstBand = config->widths[0] + radius;
    secondBand = firstBand + config->widths[1];
    thirdBand = secondBand + config->widths[2];
    SDF_VU0_LOADVECTOR(effOnesVector);
    SDF_VU0_COPYVECTOR_A_TO_B();
    VU0_SCALAR_OP_CLOBBER(firstBand, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_STOREVECTOR(bands[0]);
    SDF_VU0_COPYVECTOR_B_TO_A();
    VU0_SCALAR_OP_CLOBBER(secondBand, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_CLOBBER(thirdBand, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_STOREVECTOR(bands[1]);
    SDF_VU0_STOREVECTOR_B(bands[2]);
    for (i = 0; i < groups; i++) {
        direction[0] = sdfCos(angle);
        direction[2] = sdfSin(angle);
        VU0_STORE_VF(vf0, out[0]);
        SDF_VU0_LOADVECTOR_B(direction);
        SDF_VU0_LOADVECTOR(bands[0]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[1]);
        SDF_VU0_LOADVECTOR(bands[1]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[2]);
        SDF_VU0_LOADVECTOR(bands[2]);
        SDF_VU0_MULVECTOR();
        SDF_VU0_STOREVECTOR(out[3]);
        angle += angleStep;
        out += 4;
    }
}

void billDrawCellBlendB(effClassWork_t *work) {
    effRadialRingParams_t *config = work->payload;
    u32 limit = work->frame;
    u32 progress = config->duration;
    effRingResource_t *list = (effRingResource_t *)work->resource;
    effPointSet_t *out = list->pointSet;
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&config->colorTrack, &config->alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->color, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    if ((packed & 0xFF000000) != 0) {
        out->type = config->alphaTrack.surfaceIndex;
        out->flag = config->flag;
        SDF_VU0_LOADVECTOR(work->vectors.orientation);
        ToMatrix();
        SDF_VU0_LOADVECTOR(effOnesVector);
        VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
        SDF_VU0_SCALE_XYZ();
        SDF_VU0_LOADVECTOR(work);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(mtx);
        effDrawFourPointGroups(out, (effMatrix4_t *)mtx);
    }
}

effClassWork_t *effCreateClassWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassWorkOperations[kind].payloadSize;
    u8 *effect = sdfChipAllocDirect(size + headerSize);
    ((effClassWork_t *)effect)->payload = effect + headerSize;
    ((effClassWork_t *)effect)->color = 0x80808080;
    ((effClassWork_t *)effect)->scale = 1.0f;
    ((effClassWork_t *)effect)->frame = 0;
    ((effClassWork_t *)effect)->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect + 0x10);
    memcpy(((effClassWork_t *)effect)->payload, source, size);
    ((effClassWork_t *)effect)->resource = effClassWorkOperations[kind].createResource(source);
    effClassWorkOperations[kind].initialize(effect);
    return (effClassWork_t *)effect;
}

effClassWork_t *effCreateClassWorkFromFile(fileJobPayload_t *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateClassWork(request->option, source);
}

void effDestroyClassWork(effClassWork_t *work) {
    effClassWorkOperations[work->kind].destroyResource(work->resource);
    sdfChipFreeDirect(work);
}

effClassWork_t *effCloneClassWork(effClassWork_t *work) {
    return effCreateClassWork(work->kind, work->payload);
}

void effInitializeClassFrame(effClassWork_t *work) {
    effClassWorkOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effCallSRAnimation8(effClassWork_t *work) {
    effClassWorkOperations[work->kind].update(work);
    if ((effSRAnimation() & 1) == 0) {
        work->frame++;
    }
}

void effRunClassPostFrame(effClassWork_t *work) {
    effClassWorkOperations[work->kind].draw(work);
}

void effUpdateClassFrame(effClassWork_t *work) {
    effAdvanceClassFrame(work);
    effRunClassPostFrame(work);
}

void effSetClassWorkPosition(effClassWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.position, position);
}

void effSetClassWorkRotation(effClassWork_t *work, const f32 rotation[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.orientation, rotation);
}

void effSetClassWorkColor(effClassWork_t *work, u32 color) {
    work->color = color;
}

void effSetClassWorkScale(effClassWork_t *work, f32 scale) {
    work->scale = scale;
}

effPointSet_t *effCreatePointSet4(u32 count) {
    s32 rows = count * 4 + 4;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    effPointSet_t *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (effPointSet_t *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(effWork, 0, 0x2C);
    effWork[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effAssetQueueRelease(effPointSet_t *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

extern sdfGrOt_t *effDrawFourPointGroupsData[];

#ifdef VERSION_RAIDOU1
#define EFF_FOUR_POINT_OT_BASE 60
#else
#define EFF_FOUR_POINT_OT_BASE 61
#endif

void effDrawFourPointGroups(effPointSet_t *set, const effMatrix4_t *matrix) {
    sdfDrawTag_t *list;
    sdfDrawTag_t *setup;
    effGsPacket_t *packet;
    s32 remaining;
    s32 surfaceId;

    if (set->color & 0xFF000000) {
        list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        if (matrix == NULL) {
            SDF_VU0_LOADIDENTITY();
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdf3DPrimLinkTm(list, 0);
        sdf3DPrimLinkMat(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        remaining = set->rows;
        effWork[0].vertexColor = (u32 *)set->tail;
        effWork[0].vertex = (sdfVector4_t *)set->buffer;
        effWork[0].color = set->color;
        effWork[0].faceCount = 12;
        effWork[0].pointCount = 12;
        effWork[0].faceIndex = effData7;
        while (remaining >= 12) {
            remaining -= 8;
            sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effWork)));
            effWork[0].vertex += 8;
            effWork[0].vertexColor += 8;
        }
        if (remaining >= 8) {
            effWork[0].faceCount = 6;
            effWork[0].pointCount = remaining;
            sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effWork)));
        }
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        if (set->type < 5) {
            effSubmitSurfacePacket(effDrawFourPointGroupsData[set->type], list);
        } else {
            effGsPacket_t *blendPacket;

            surfaceId = set->type == 5 ? EFF_FOUR_POINT_OT_BASE : EFF_FOUR_POINT_OT_BASE + 6;
            setup = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(setup);
            blendPacket = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            blendPacket->registerValue = 6;
            blendPacket->dmaTag = 2;
            blendPacket->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            blendPacket->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            blendPacket->registerList = 0xE;
            blendPacket->registerAddress = 0x42;
            sdfDrawTagLink(setup, (u32)blendPacket);
            effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], setup);
            blendPacket = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            blendPacket->dmaTag = 2;
            blendPacket->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            blendPacket->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            blendPacket->registerList = 0xE;
            switch (surfaceId) {
            case EFF_FOUR_POINT_OT_BASE:
                blendPacket->registerValue = 0x44;
                break;
            case EFF_FOUR_POINT_OT_BASE + 6:
                blendPacket->registerValue = 0x42;
                break;
            }
            blendPacket->registerAddress = 0x42;
            sdfDrawTagLink(list, (u32)blendPacket);
            effSubmitSurfacePacket(&sdfGrOtTable[surfaceId], list);
        }
    }
}

/* Create an empty surface node for handleCount grid cells; no handles are allocated yet. */
poltHandle_t *effCreateSurfaceNode(u32 handleCount) {
    poltHandle_t *node = sdfChipAllocZeroDirect(sizeof(poltHandle_t));
    node->handleCount = handleCount;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->index = 0;
    node->record = 0;
    node->count = 1;
    return node;
}

poltHandle_t *effCreateSurfaceNodeForGrid(fileKeyBlock_t *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* Retail 002A5A58 interprets the capacity factors signed (MULT). */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return effCreateSurfaceNode(n > 100 ? 100 : n);
}

poltHandle_t *effResourceReferenceReplaceFromFile(fileJobPayload_t *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    fileKeyBlock_t *params = (fileKeyBlock_t *)(source + 0x1C);
    poltHandle_t *node = effCreateSurfaceNodeForGrid(params);

    effRebuildSurfaceHandles(node, work->option, source);
    effReplaceResourceRef(node, work->option, params);
    return node;
}

poltHandle_t *effInitializeSurfaceForKind(fileJobPayload_t *work) {
    poltHandle_t *node = effResourceReferenceReplaceFromFile(work);
    u32 *secondary = fileResolveSecondaryBuffer(work);

    if (secondary == NULL) {
        return node;
    }
    switch (work->primary.selector) {
    case 1:
        effReplaceSurfacePrimaryBillboard(node, (u32)secondary);
        break;
    case 2:
        effReplaceSurfaceFlaggedBillboard(node, (u32)secondary);
        break;
    case 3:
        break;
    case 4:
        effSetSurfaceRetainedResource(node, *secondary);
        break;
    case 5:
        effRebuildSurfaceJobs(node, secondary);
        break;
    case 6:
        break;
    case 7:
        effReplaceSurfaceResourceHolder(node, (u32)secondary);
        break;
    }
    node->kind = work->primary.selector;
    return node;
}

void effDestroySurfaceNode(poltHandle_t *node) {
    u32 i;
    u32 count;

    if (node->resource != NULL) {
        dds3BillboardDelete(node->resource);
    }
    if (node->jobAllocation != 0) {
        count = node->record->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers((effSurfaceGridNode_t *)((u32 *)node->index)[i]);
        }
        sdfMemFreeDirect(node->handleBuffer);
    }
    if (node->resourceHolder != 0) {
        effCacheTexDelete(node->resourceHolder);
    }
    if (node->record != 0) {
        effPCPHelperBEDGetModel(node->record);
    }
    sdfChipFreeDirect(node);
}

poltHandle_t *effRecreateSurfaceNodeFromWork(u8 *work) {
    fileKeyBlock_t *params = (fileKeyBlock_t *)((poltHandle_t *)work)->record->data1;
    poltHandle_t *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((poltHandle_t *)work)->record->type, &((poltHandle_t *)work)->params);
    effReplaceResourceRef(node, ((poltHandle_t *)work)->record->type, params);
    return node;
}

poltHandle_t *effSCopy(u8 *work) {
    fileKeyBlock_t *params = (fileKeyBlock_t *)((poltHandle_t *)work)->record->data1;
    poltHandle_t *node = effCreateSurfaceNodeForGrid(params);
    effRebuildSurfaceHandles(node, ((poltHandle_t *)work)->record->type, &((poltHandle_t *)work)->params);
    effReplaceResourceRef(node, ((poltHandle_t *)work)->record->type, params);
    effCloneSurfaceGridResources(node, work);
    return node;
}

/* Replace the destination's kind-selected resources with clones or retained references. */
void effCloneSurfaceGridResources(poltHandle_t *dst, u8 *work) {
    poltHandle_t *src = (poltHandle_t *)work;
    u32 selector = src->kind;

    switch (selector) {
    case 1:
    case 2:
    case 4:
        if (dst->resource != NULL) {
            dds3BillboardDelete(dst->resource);
        }
        dst->resource = dds3BillboardCopy(src->resource);
        dds3BillboardSetAnimColorAdvMode(dst->resource);
        if (dst->record != 0) {
            bhvrHandle_t *record = dst->record;
            dds3BillboardSetMode(dst->resource, (s16)((fileKeyBlock_t *)record->data0)->alphaTrack.surfaceIndex);
        }
        break;
    case 5: {
        u32 count = src->record->count;
        s32 size;
        u32 i;

        if (dst->jobAllocation != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfMemFreeDirect(dst->jobAllocation);
            dst->jobs = 0;
            dst->jobAllocation = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobAllocation = sdfMemAllocDirect(size);
        dst->jobs = (fileJobPayload_t **)sdfMemLock(dst->jobAllocation);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = Copy_effParts(src->jobs[0]);
        }
        break;
    }
    case 7:
        if (dst->resourceHolder != 0) {
            effCacheTexDelete(dst->resourceHolder);
        }
        dst->resourceHolder = Copy_effCacheTexList(src->resourceHolder);
        break;
    }
    dst->kind = src->kind;
}

/* Replace the per-cell grid handles from the copied source header. kind is unused here. */
void effRebuildSurfaceHandles(poltHandle_t *node, u16 kind, void *source) {
    effSurfaceParams_t *params = source;
    u32 size;
    u32 i;
    effSurfaceGridNode_t *handle;

    node->params = *params;
    size = node->handleCount * 4;
    if (size == 0) {
        return;
    }
    if (node->index != 0) {
        for (i = 0; i < node->handleCount; i++) {
            effReleaseSurfaceGridBuffers((effSurfaceGridNode_t *)((u32 *)node->index)[i]);
        }
        sdfMemFreeDirect(node->handleBuffer);
    }
    node->handleBuffer = sdfMemAllocDirect(size);
    node->index = sdfMemLock(node->handleBuffer);
    for (i = 0; i < node->handleCount; i++) {
        handle = effCreateSurfaceGridNode(params->opaque00[0], params->opaque00[1]);
        ((u32 *)node->index)[i] = (u32)handle;
        effFillSurfaceGridColorGradient(handle, params->opaque00 + 2);
    }
}

void effReplaceResourceRef(poltHandle_t *node, u32 entryId, const void *resource) {
    if (node->record != NULL) {
        effPCPHelperBEDGetModel(node->record);
    }
    node->record = fileAllocateGridRecordSlots((u16)entryId, node->handleCount, resource);
}

void effSetSurfaceRetainedResource(poltHandle_t *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardGeneral(resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfacePrimaryBillboard(poltHandle_t *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(0, resourceId);
    node->resource = resource;
    if (node->record != 0) {
        dds3BillboardSetMode(resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effReplaceSurfaceFlaggedBillboard(poltHandle_t *node, u32 resourceId) {
    billHandle_t *resource = node->resource;
    if (resource != 0) {
        dds3BillboardDelete(resource);
    }
    resource = dds3BillboardCreate(1, resourceId);
    node->resource = resource;
    dds3BillboardSetAnimColorAdvMode(resource);
    if (node->record != 0) {
        dds3BillboardSetMode(node->resource, (s16)((fileKeyBlock_t *)node->record->data0)->alphaTrack.surfaceIndex);
    }
}

void effRebuildSurfaceJobs(poltHandle_t *node, void *source) {
    u32 count = node->record->count;
    u32 i;
    u32 size;

    if (node->jobAllocation != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfMemFreeDirect(node->jobAllocation);
        node->jobs = 0;
        node->jobAllocation = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobAllocation = sdfMemAllocDirect(size);
        node->jobs = (fileJobPayload_t **)sdfMemLock(node->jobAllocation);
        node->jobs[0] = Create_effParts((fileJobPayload_t *)source);
        for (i = 1; i < count; i++) {
            node->jobs[i] = Copy_effParts(node->jobs[0]);
        }
    }
}

void effReplaceSurfaceResourceHolder(poltHandle_t *node, u32 resource) {
    struct ctexlHandle_t *holder;

    if (node->resourceHolder != 0) {
        effCacheTexDelete(node->resourceHolder);
    }
    holder = effCacheTexCreate(resource);
    node->resourceHolder = holder;
}

void effClearSurfaceNodeRecordReferences(s32 node) {
    if (((poltHandle_t *)node)->record != 0) {
        fileClearRecordReferences(((poltHandle_t *)node)->record);
        return;
    }
}

void effAcquireSurfaceRecord(s32 node) {
    if (((poltHandle_t *)node)->record != 0) {
        fileAcquireRecord(((poltHandle_t *)node)->record);
        return;
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallToMatrix);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSRAnimation9);

void effMenuRecordVectorSet(s32 node, const u128 *value) {
    mnuRecordSetVector(((poltHandle_t *)node)->record, value);
}

void effSetSurfaceRecordSecondaryVector(s32 node, const u128 *value) {
    fileSetRecordSecondVector(((poltHandle_t *)node)->record, value);
}

void effSetSurfaceNodeColor(poltHandle_t *work, u32 color) {
    work->color = color;
}

void effIndexedFloatCallbackDispatch(effValPtr44_t *p, float v) {
    p->f08 = v;
    dds3DispatchIndexedCallback(p->p44, v);
}

/* NON_MATCHING: 51 of 84 words match. Size round-up to 16 schedules differently (sra before andi, register choice). */
#ifdef NON_MATCHING
extern u8 effCreateSurfaceGridNodeData[];

effSurfaceGridNode_t *effCreateSurfaceGridNode(u32 count, u32 columns)
{
    u32 rows;
    s32 size;
    sdfMemHandle_t *handle;
    u128 *buffer;
    effSurfaceGridNode_t *node;
    u128 *queue;

    rows = 3 * count * columns + 6;
    size = 20 * rows + 160;
    if ((size & 0xF) != 0) {
        size = ((size >> 4) + 1) * 16;
    } else {
        size = (size >> 4) * 16;
    }
    handle = sdfMemAllocDirect(size + 60);
    buffer = sdfMemLock(handle);
    node = (effSurfaceGridNode_t *)((u8 *)buffer + size);
    node->type = 2;
    node->buffer = buffer;
    queue = buffer + rows;
    node->queueA = queue;
    queue += 8;
    node->queueB = (u32 *)queue;
    node->field_18 = 3;
    node->rows = rows;
    node->columns = columns;
    node->allocation = handle;
    node->tail = (u32 *)(queue + 2);
    node->field_14 = 0;
    node->material = sdfMatNew();
    sdfMatSetConstant(node->material, 1.0f);
    memset(&effDrawSurfaceTrailHistoryWork, 0, 0x2C);
    effDrawSurfaceTrailHistoryWork.attr = 0;
    effDrawSurfaceTrailHistoryWork.faceIndex = (u32 *)effData8;
    memset(&effWork2, 0, 0x2C);
    effWork2.attr = 0;
    effWork2.pointCount = 8;
    effWork2.faceIndex = (u32 *)effCreateSurfaceGridNodeData;
    effWork2.faceCount = 6;
    return node;
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", effCreateSurfaceGridNode);
#endif

/* vu0 routine: fade-blended vertex colours written into the surface node's tail */
void effFillSurfaceGridColorGradient(effSurfaceGridNode_t *node, u32 *colors) {
    u32 count = node->rows / 3;
    u32 *out = node->tail;
    f32 step = 1.0f / count;
    f32 t = 0.0f;
    f32 corner0[4];
    f32 corner2[4];
    f32 corner1[4];
    f32 corner3[4];
    s32 color0[4];
    s32 color1[4];
    s32 color2[4];
    s32 color3[4];
    s32 blendedA[4];
    s32 blendedB[4];
    u32 packedA;
    u32 packedB;
    u32 i;

    color0[0] = colors[0];
    EE_MMI_RGBA_UNPACK(color0, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner0);
    color1[0] = colors[1];
    EE_MMI_RGBA_UNPACK(color1, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner1);
    color2[0] = colors[2];
    EE_MMI_RGBA_UNPACK(color2, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner2);
    color3[0] = colors[3];
    EE_MMI_RGBA_UNPACK(color3, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(corner3);
    for (i = 0; i < count; i++) {
        SDF_VU0_LOADVECTOR(corner0);
        SDF_VU0_LOADVECTOR_B(corner2);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        SDF_VU0_ADDVECTOR();
        EE_MMI_RGBA_PACK_UNIT(packedA, 128.0f);
        blendedA[0] = packedA;
        out[1] = blendedA[0];
        SDF_VU0_LOADVECTOR(corner1);
        SDF_VU0_LOADVECTOR_B(corner3);
        VU0_SCALAR_OP(t, "vmulx.xyzw vf10, vf10, vf2x");
        VU0_SCALAR_OP(1.0f - t, "vmulx.xyzw vf11, vf11, vf2x");
        SDF_VU0_ADDVECTOR();
        EE_MMI_RGBA_PACK_UNIT(packedB, 128.0f);
        blendedB[0] = packedB;
        out[2] = out[0] = blendedB[0];
        t += step;
        out += 3;
    }
}

void effReleaseSurfaceGridBuffers(effSurfaceGridNode_t *node) {
    sdfMatFreeDelayed(node->material);
    sdfMemFreeDirect(node->allocation);
}

void effResetSurfaceGridFrame(effSurfaceGridNode_t *node) {
    node->field_14 = 0;
    node->field_18 = 3;
}

/* Three-vector sampler reads the primary surface ring counters. */
void effCopyRingFrameVectors(effSurfaceGridNode_t *node, u128 *destination, s32 frame) {
    s32 index = node->field_18 - 3 * (node->columns * (frame - 1) + frame);
    u128 *source;
    s32 i;

    if (index < 3) {
        index += node->rows - 3;
    }
    source = node->buffer + index;
    for (i = 0; i < 3; i++) {
        SDF_VU0_COPYVECTOR(destination + i, source + i);
    }
}

/* data holds two vector addresses: the ring head, then the input frame.
 * ring_mirror_pointer_epochs_probe (hooked==unhooked): GCSE preserves the
 * head-to-mirror copy; separate append/mirror lifetimes keep native allocation. */
void effAppendSurfaceGridVectors(effSurfaceGridNode_t *work, const f32 (*source)[4]) {
    s32 index = work->field_18;
    const u128 *data = work->buffer;
    u128 *destination = (u128 *)data + index;
    s32 capacity;
    s32 i;

    for (i = 0; i < 3; i++) {
        SDF_VU0_COPYVECTOR(destination + i, source + i);
    }
    capacity = work->rows;
    work->field_18 = index + 3;
    if (work->field_18 == capacity) {
        u128 *mirror = (u128 *)data;
        data = (const u128 *)source;
        for (i = 0; i < 3; i++) {
            SDF_VU0_COPYVECTOR(mirror + i, data + i);
        }
        work->field_18 = 3;
    }
    if (work->field_14 < capacity - 3) {
        work->field_14 += 3;
    }
}

/* vu0 routine: interpolate three-vector ring frames with SDK spline and lerp operations. */
void effInterpolateSurfaceGridVectors(effSurfaceGridNode_t *work, const f32 (*vectors)[4]) {
    f32 frames[4][3][4];
    f32 controlVectors[4][4];
    f32 output[3][4];
    s32 count = work->columns;
    s32 retainedVectors = work->field_14;
    s32 i;
    s32 j;
    f32 step;
    f32 t;

    if (count == 1 || retainedVectors == 0) {
        effAppendSurfaceGridVectors(work, vectors);
        return;
    }
    if (retainedVectors >= count * 6) {
        effCopyRingFrameVectors(work, (u128 *)frames[0], 2);
        effCopyRingFrameVectors(work, (u128 *)frames[1], 1);
        for (j = 0; j < 3; j++) {
            SDF_VU0_COPYVECTOR(frames[2][j], vectors[j]);
            SDF_VU0_COPYVECTOR(frames[3][j], vectors[j]);
        }
        step = 1.0f / count;
        t = 0.0f;
        for (i = 0; i < count; i++) {
            t += step;
            for (j = 0; j < 3; j++) {
                SDF_VU0_COPYVECTOR(controlVectors[0], frames[0][j]);
                SDF_VU0_COPYVECTOR(controlVectors[1], frames[1][j]);
                SDF_VU0_COPYVECTOR(controlVectors[2], frames[2][j]);
                SDF_VU0_COPYVECTOR(controlVectors[3], frames[3][j]);
                vuCatmullRomPoint(controlVectors, t);
                SDF_VU0_STOREVECTOR(output[j]);
            }
            effAppendSurfaceGridVectors(work, output);
        }
    } else {
        effCopyRingFrameVectors(work, (u128 *)controlVectors, 1);
        step = 1.0f / count;
        t = 0.0f;
        for (i = 0; i < count; i++) {
            t += step;
            for (j = 0; j < 3; j++) {
                SDF_VU0_LOADVECTOR(controlVectors[j]);
                SDF_VU0_LOADVECTOR_B(vectors[j]);
                VU0_LERP_VF10(t);
                SDF_VU0_STOREVECTOR(output[j]);
            }
            effAppendSurfaceGridVectors(work, output);
        }
    }
}

void effBeginMatrixVuDrawPacket(const effMatrix4_t *matrix) {
    struct sdfDrawTag_t *work = (struct sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    effCurrentRenderPacket = work;
    sdfDrawTagInit(work);
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(effCurrentRenderPacket, 0);
}

void effDrawSurfaceTrailHistory(effSurfaceTrailHistory_t *history, u32 color) {
    s32 start[4];
    s32 length[4];
    s32 recent;
    s32 wrapped;
    s32 i;
    s32 remaining;
    sdf3DPrimPolyParm_t *draw;
    u32 *colors;
    sdfDrawTag_t *list;

    if ((color & 0xFF000000) == 0) {
        return;
    }
    if (history->activePointCount < 6) {
        return;
    }
    sdf3DPrimLinkMat(effCurrentRenderPacket, history->resourceHandle, 0);
    recent = history->activePointCount;
    start[0] = history->nextPointWriteIndex - recent;
    if (start[0] < 3) {
        wrapped = start[0] - 3;
        start[1] = 0;
        start[0] = wrapped + history->pointCapacity;
        length[0] = -wrapped;
        length[1] = recent - length[0] + 3;
    } else {
        length[0] = recent;
        length[1] = 0;
    }
    effDrawSurfaceTrailHistoryWork.vertexColor = history->colors;
    for (i = 0; i < 2; i++) {
        draw = &effDrawSurfaceTrailHistoryWork;
        draw->vertex = history->points + start[i];
        draw->color = color;
        draw->faceCount = 16;
        remaining = length[i];
        draw->pointCount = 15;
        while (remaining >= 15) {
            remaining -= 12;
            list = effCurrentRenderPacket;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawSurfaceTrailHistoryWork));
            effDrawSurfaceTrailHistoryWork.vertex += 12;
            effDrawSurfaceTrailHistoryWork.vertexColor += 12;
        }
        if (remaining >= 6) {
            draw->faceCount = (remaining / 3) * 4 - 4;
            draw->pointCount = remaining;
            list = effCurrentRenderPacket;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(draw));
            draw->vertexColor += remaining;
        }
    }
    colors = history->endColors;
    effWork2.vertex = history->endPoints;
    colors[0] = effDrawSurfaceTrailHistoryWork.vertexColor[-2];
    effWork2.vertexColor = colors;
    colors[7] = colors[6] = colors[5] = colors[4] =
        colors[3] = colors[2] = colors[1] = effDrawSurfaceTrailHistoryWork.vertexColor[-3];
    effWork2.color = color;
    list = effCurrentRenderPacket;
    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effWork2));
}

void effSubmitIndexedRenderPacket(u32 index) {
    u8 *entry = effSubmitIndexedRenderPacketData[index];
    ((effDrawEntry_t *)entry)->draw(entry, effCurrentRenderPacket);
    effCurrentRenderPacket = NULL;
}

/* NON_MATCHING: 182 of 359 words match. Retail inlines the 0x98 and 0xA0 byte param copies as alignment-tested ldl/ldr loops, memcpy here; end-frame math follows IDA. Needs effQuadParams_t.sizeScale renamed to u32 sizeDuration. */
#ifdef NON_MATCHING
extern sdf3DPrimPolyParm_t PartsCreateWork;
extern u32 PartsCreateData[];
extern u32 PartsCreateData2[];

/* Frames until the particle's colour, alpha and size tracks have all finished. */
static u32 partsEndFrame(ptexHandle_t *ptex)
{
    u32 end = 0;
    s32 key = 0;
    f32 value;
    u32 fadeEnd;

    switch (ptex->param.colorTrack.segmentMode) {
    case 1:
    case 2:
        key = (ptex->param.colorTrack.segmentMode == 2) ? 1 : 0;
        end = (u32)((f32)(u32)ptex->param.duration * (1.0f - ptex->param.colorTrack.keys[key].fraction));
        break;
    default:
        break;
    }
    value = (f32)(u32)ptex->param.duration * (1.0f - ptex->param.alphaTrack.fadeOutFraction);
    fadeEnd = (u32)value;
    if (end < fadeEnd) {
        end = fadeEnd;
    }
    switch (ptex->param.sizeTrack.curve.mode) {
    case 1:
    case 2:
        key = (ptex->param.sizeTrack.curve.mode == 2) ? 1 : 0;
        value = (f32)ptex->param.sizeDuration * (1.0f - ptex->param.sizeTrack.curve.keys[key].fraction);
        fadeEnd = (u32)value;
        if (end < fadeEnd) {
            end = fadeEnd;
        }
        break;
    default:
        break;
    }
    return end;
}

ptexHandle_t *PartsCreate(fileJobPayload_t *payload)
{
    ptexHandle_t *ptex;
    void *buffer;

    ptex = sdfChipAllocZeroDirect(sizeof(ptexHandle_t));
    VU0_STORE_VF(vf0, ptex->pos);
    VU0_STORE_VF(vf0, ptex->rot);
    ptex->hBillboard = NULL;
    ptex->abgr = 0x80808080;
    ptex->scale = 1.0f;
    ptex->hCTexList = NULL;
    ptex->assetHandle = sdfMatNew();
    sdfMatSetConstant(ptex->assetHandle, 1.0f);
    memset(&PartsCreateWork, 0, 0x2C);
    PartsCreateWork.pointCount = 4;
    PartsCreateWork.attr = 0x4000;
    PartsCreateWork.faceIndex = PartsCreateData;
    PartsCreateWork.vertexColor = PartsCreateData2;
    PartsCreateWork.faceCount = 2;
    if (payload == NULL) {
        return ptex;
    }
    ptex->type = payload->option;
    buffer = fileResolvePrimaryBuffer(payload);
    if (ptex->type == 1) {
        memcpy(&ptex->param, buffer, sizeof(effQuadParams_t));
    } else if (ptex->type == 2) {
        memcpy(&ptex->param, buffer, (u8 *)&ptex->hBillboard - (u8 *)&ptex->param);
    }
    buffer = fileResolveSecondaryBuffer(payload);
    if (buffer != NULL) {
        switch (payload->primary.selector) {
        case 1:
            ptex->hBillboard = dds3BillboardCreate(0, (u32)buffer);
            break;
        case 2:
            ptex->hBillboard = dds3BillboardCreate(1, (u32)buffer);
            break;
        case 4:
            ptex->hBillboard = dds3BillboardGeneral(*(s32 *)buffer);
            break;
        case 7:
            ptex->hCTexList = effCacheTexCreate((u32)buffer);
            break;
        }
        if (ptex->hBillboard != NULL) {
            dds3BillboardSetAnimColorAdvMode(ptex->hBillboard);
            if (ptex->type == 1 || ptex->type == 2) {
                dds3BillboardSetMode(ptex->hBillboard, (u16)ptex->param.alphaTrack.surfaceIndex);
            }
        }
    }
    if (ptex->type == 2) {
        ptex->unkE4 = partsEndFrame(ptex);
    }
    return ptex;
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", PartsCreate);
#endif

INCLUDE_ASM(const s32, "code_r1_00226EC8", Delete_effPolygonTex);

ptexHandle_t *Copy_effPolygonTex(ptexHandle_t *src) {
    ptexHandle_t *dst = PartsCreate(NULL);

    dst->type = src->type;
    switch (src->type) {
    case 1:
        memcpy(&dst->param, &src->param, sizeof(effQuadParams_t));
        break;
    case 2:
        memcpy(&dst->param, &src->param, (u8 *)&dst->hBillboard - (u8 *)&dst->param);
        break;
    }
    ResourceCopy_effPolygonTex(dst, src);
    return dst;
}

void ResourceCopy_effPolygonTex(ptexHandle_t *dst, ptexHandle_t *src)
{
    if (src->hBillboard != NULL) {
        if (dst->hBillboard != NULL) {
            dds3BillboardDelete(dst->hBillboard);
        }
        dst->hBillboard = dds3BillboardCopy(src->hBillboard);
        dds3BillboardSetAnimColorAdvMode(dst->hBillboard);
        switch (dst->type) {
        case EFFPOLYGON_TEX_TYPE_BOARD:
            dds3BillboardSetMode(dst->hBillboard, (s16)dst->param.alphaTrack.surfaceIndex);
            break;
        case EFFPOLYGON_TEX_TYPE_BOARD_LOOP:
            dds3BillboardSetMode(dst->hBillboard, (s16)dst->param.alphaTrack.surfaceIndex);
            break;
        }
    } else {
        if (dst->hCTexList != NULL) {
            effCacheTexDelete(dst->hCTexList);
        }
        dst->hCTexList = Copy_effCacheTexList(src->hCTexList);
    }
}

void effResetRenderResourceFrame(ptexHandle_t *work) {
    work->frame = 0;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", Draw_effPolygonTex);

void effSetRenderResourcePosition(ptexHandle_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->pos, position);
}

void effSetRenderResourceRotation(ptexHandle_t *work, const f32 rotation[4]) {
    SDF_VU0_COPYVECTOR(work->rot, rotation);
}

void effSetRenderResourceColor(ptexHandle_t *work, u32 color) {
    work->abgr = color;
}

void effSetRenderResourceScale(ptexHandle_t *work, f32 scale) {
    work->scale = scale;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", Update_effPolygonTex);

INCLUDE_ASM(const s32, "code_r1_00226EC8", GetOT);

INCLUDE_ASM(const s32, "code_r1_00226EC8", Render);

void effRandomizeParticleFields(effClassWork_t *work) {
    u32 index = 0;
    u32 count = ((effBillTimedHeader_t *)work->payload)->count;
    effPointSetRow_t *entry = ((effPointSetTable_t *)work->resource)->rows;

    if (count != 0) {
        do {
            entry->key = -1 - (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 3);
            entry++;
            index++;
        } while (index < count);
    }
}

effPointSetTable_t *effCreateAlphaRampPointSetRows(effBillPointConfig_t *src) {
    u32 count = src->timed.count;
    effPointSetTable_t *table;
    effPointSetRow_t *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    table = sdfChipAllocDirect(count * sizeof(effPointSetRow_t) + 4);
    table->rows = (effPointSetRow_t *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = table->rows;
    for (i = 0; i < count; i++) {
        effPointSet_t *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;

            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->key = ~(i * 4);
        row->angle = 0.0f;
        row++;
    }
    return table;
}

void effFreeIndexedEntries(effClassWork_t *work) {
    u32 index = 0;
    u32 count = ((effBillTimedHeader_t *)work->payload)->count;
    effPointSetTable_t *table = (effPointSetTable_t *)work->resource;
    effPointSetRow_t *entry = table->rows;

    if (count != 0) {
        do {
            effReleasePointSetAsset(entry->set);
            entry++;
            index++;
        } while (index < count);
    }
    sdfChipFreeDirect(table);
}

/* Advance radial five-point wave strips from the class orientation. */
void effUpdateRadialWavePointRows(effClassWork_t *work) {
    f32 offsets[5];
    f32 widths[5];
    f32 direction[4];
    f32 view[4];
    f32 axis[4];
    effBillPointConfig_t *config = work->payload;
    effPointSetRow_t *entry = ((effPointSetTable_t *)work->resource)->rows;
    u32 progress = config->timed.time.progress;
    s32 count;
    s32 remaining;
    s32 respawnDelay;
    f32 layers;
    f32 step;
    f32 waveStep;
    f32 amplitude;
    f32 bendScale;
    f32 phase;
    f32 speed;
    f32 speedJitter;

    if (progress < work->frame && progress != 0) {
        return;
    }
    count = config->timed.count;
    respawnDelay = config->respawnDelay;
    speed = config->speed;
    speedJitter = config->speedJitter;
    layers = (f32)(u32)config->layers;
    step = config->totalLength / layers;
    waveStep = ((f32)config->periods * 3.14159265f) / layers;
    amplitude = config->amplitude;
    if (step <= 0.0f) {
        return;
    }
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_LOADVECTOR_B(&aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(view);
    SDF_VU0_LOADVECTOR(work->vectors.orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effWorldUpVector);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    bendScale = 0.26179933547973633f;
    widths[0] = 0;
    widths[1] = config->edgeWidth;
    widths[2] = widths[1] + config->halfWidth;
    widths[3] = widths[2] + config->halfWidth;
    widths[4] = widths[3] + config->edgeWidth;
    if (count > 0) {
        remaining = count;
        for (; remaining != 0; remaining--, entry++) {
            if (entry->key == -1) {
                entry->color = 0xA0808080;
            } else {
                f32 r = dds3GetRandFloatA(effSharedRandomState);
                f32 bendStep = speed * (r * speedJitter + (1.0f - speedJitter));
                f32 bend;

                phase = entry->angle;
                bendStep /= layers;
                if (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 1) {
                    bendStep = -bendStep;
                }
                bend = bendStep;
                if (entry->color & 0xFF000000) {
                    entry->color += 0xE0000000;
                } else {
                    entry->key = -1;
                    if (respawnDelay > 0) {
                        entry->key -= (u32)dds3GetRandA((struct effRandState_t *)effSharedRandomState) % (u32)respawnDelay;
                    }
                    continue;
                }
                {
                    effPointSet_t *set = entry->set;
                    f32 *vertices = (f32 *)set->buffer;
                    s32 rows = set->rows / 5;
                    f32 stepSquared;
                    f32 waveAngle;
                    f32 waveHeight;
                    f32 waveRadius;
                    f32 oldSlope;
                    f32 inner;
                    f32 width;
                    f32 newHeight;
                    f32 delta;
                    f32 segmentLength;
                    s32 j;
                    s32 k;

                    if (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 1) {
                        amplitude = -amplitude;
                    }
                    stepSquared = step * step;
                    waveAngle = dds3GetRandFloatA(effSharedRandomState) * 3.14159265f;
                    waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.9f + (1.0f - 0.9f));
                    waveHeight = sdfSin(waveAngle) * waveRadius;
                    waveAngle += waveStep;
                    newHeight = sdfSin(waveAngle) * waveRadius;
                    delta = newHeight - waveHeight;
                    waveHeight = newHeight;
                    segmentLength = fsqrtf(delta * delta + stepSquared);
                    oldSlope = asinf(delta / segmentLength) + phase;
                    inner = config->halfWidth;
                    width = inner + config->edgeWidth;
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_LOADVECTOR_B(view);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    SDF_VU0_LOADVECTOR_B(direction);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_STOREVECTOR(axis);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_STOREVECTOR(vertices);
                    SDF_VU0_NEGVECTOR_XYZ();
                    SDF_VU0_STOREVECTOR(vertices + 16);
                    VU0_SCALE_VF(vf11, inner);
                    SDF_VU0_STOREVECTOR_B(vertices + 4);
                    VU0_NEGATE_XYZ(vf11);
                    SDF_VU0_STOREVECTOR_B(vertices + 12);
                    VU0_STORE_VF(vf0, vertices + 8);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    VU0_SCALE_VF(vf11, waveHeight);
                    for (j = 0; j < 5; j++) {
                        SDF_VU0_LOADVECTOR(vertices + j * 4);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(vertices + j * 4);
                        offsets[j] = 0;
                    }
                    vertices += 20;
                    for (k = 1; k < rows; k++) {
                        f32 tilt = bendScale * (dds3GetRandFloatA(effSharedRandomState) * 0.75 + 0.25);
                        f32 slope;
                        f32 tangentScale;
                        f32 sine;

                        if (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 1) {
                            bendScale = -bendScale;
                        }
                        waveAngle += waveStep;
                        if (waveAngle >= 3.14159265f) {
                            waveAngle -= 3.14159265f;
                            amplitude = -amplitude;
                            waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.9f + (1.0f - 0.9f));
                        }
                        newHeight = sdfSin(waveAngle) * waveRadius;
                        delta = newHeight - waveHeight;
                        segmentLength = fsqrtf(delta * delta + stepSquared);
                        slope = asinf(delta / segmentLength) + (tilt + bend);
                        SetRotateMatrixAxis(oldSlope, (const sdfVector3_t *)axis);
                        slope += phase;
                        SDF_VU0_LOADVECTOR(direction);
                        SDF_VU0_APPLYMATRIX_XYZ();
                        delta = (slope - oldSlope) * 0.5f;
                        oldSlope = slope;
                        waveHeight = newHeight;
                        bend += bendStep;
                        sine = sdfSin(delta);
                        tangentScale = sine / sdfCos(delta);
                        SDF_VU0_COPYVECTOR_A_TO_C();
                        for (j = 0; j < 5; j++) {
                            f32 extra = widths[j] * tangentScale;
                            f32 length;
                            offsets[j] += extra;
                            SDF_VU0_COPYVECTOR_C_TO_B();
                            SDF_VU0_LOADVECTOR(vertices + j * 4 - 20);
                            length = segmentLength + offsets[j];
                            VU0_SCALE_VF(vf11, length);
                            SDF_VU0_ADDVECTOR();
                            SDF_VU0_STOREVECTOR(vertices + j * 4);
                            offsets[j] = extra;
                        }
                        vertices += 20;
                    }
                }
            }
            entry->key = (s32)((u32)entry->key + 1);
        }
    }
}

/* Per-frame update of a five-point-group class: tint every active set and draw it under the class transform. */
void effDrawTintedFivePointGroupRows(effClassWork_t *work) {
    effBillPointConfig_t *config = (effBillPointConfig_t *)work->payload;
    effBillTimedHeader_t *header = &config->timed;
    effPointSetRow_t *row = ((effPointSetTable_t *)work->resource)->rows;
    s32 frame = work->frame;
    s32 time = header->time.duration;
    s32 count;
    s32 i;
    f32 tint[4];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 rowColor[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (time < frame && time != 0) {
        return;
    }
    count = header->count;
    second = effColorAnimation(&header->colorTrack, &header->alphaTrack, frame, time);
    color1[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    SDF_VU0_LOADIDENTITY();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    for (i = 0; i < count; i++, row++) {
        if (row->key > 0) {
            effPointSet_t *set = row->set;

            rowColor[0] = row->color;
            EE_MMI_RGBA_UNPACK(rowColor, 1.0f / 128.0f);
            SDF_VU0_LOADVECTOR_B(tint);
            SDF_VU0_MULVECTOR();
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            blended[0] = packed;
            set->color = blended[0];
            set->type = header->alphaTrack.surfaceIndex;
            set->flag = config->drawFlag;
            effDrawFivePointGroups(set, mtx);
        }
    }
}

void effResetRadialClassResourceFrames(effClassWork_t *work) {
    u32 index = 0;
    u32 count = ((effBillTimedHeader_t *)work->payload)->count;
    effClassWork_t **entry = ((effClassWorkList_t *)work->resource)->entries;

    if (count != 0) {
        do {
            effResetDispatchCounter(*entry);
            entry++;
            index++;
        } while (index < count);
    }
}

effClassWorkList_t *effCreateRadialClassResourceEntries(effBillPointConfig_t *config) {
    u32 count = config->timed.count;
    effClassWorkList_t *allocation = sdfChipAllocDirect(count * 4 + 4);
    effClassWork_t **entries = (effClassWork_t **)(allocation + 1);
    effBillPointConfig_t copy;
    u32 index;
    f32 step;
    f32 position;
    f32 offset;

    allocation->entries = entries;
    if ((u32)config->layers < 3) {
        config->layers = 3;
    }
    if (count == 0) {
        return allocation;
    }
    memcpy(&copy, config, 0x84);
    copy.timed.count = 1;
    copy.totalLength = config->totalLength;
    step = 6.2831852f / (f32)count;
    position = step * ((dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f);
    for (index = 0; index < count; index++) {
        effClassWork_t *resourceWork = effCreateClassResourceWork(1, &copy);
        u8 **resourceSlot = (u8 **)resourceWork->resource;

        *entries++ = resourceWork;
        *(f32 *)(*resourceSlot + 0xC) = position;
        offset = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f;
        position += step + step * offset * 0.25f;
    }
    return allocation;
}

void effDestroyRadialClassResourceEntries(effClassWork_t *work) {
    u8 *config = work->payload;
    effClassWorkList_t *state = (effClassWorkList_t *)work->resource;
    u32 count = ((effBillTimedHeader_t *)config)->count;
    u32 index = 0;
    effClassWork_t **entry = state->entries;

    if (count != 0) {
        do {
            effDestroyClassResourceWork(*entry);
            entry++;
        } while (++index < count);
    }
    sdfChipFreeDirect(state);
}

void effAdvanceRadialClassResourceFrames(effClassWork_t *work) {
    u32 index = 0;
    u32 count = ((effBillTimedHeader_t *)work->payload)->count;
    effClassWork_t **entry = ((effClassWorkList_t *)work->resource)->entries;

    if (count != 0) {
        do {
            effAdvanceClassResourceFrame(*entry);
            entry++;
            index++;
        } while (index < count);
    }
}

/* vu0 routine: packed color blend and SDK vector copies. */
void effUpdateRadialClassInstances(effClassWork_t *work) {
    effBillRadialConfig_t *config = work->payload;
    s32 frame = work->frame;
    s32 progress = config->point.timed.time.duration;
    effClassWork_t **entries = ((effClassWorkList_t *)work->resource)->entries;
    f32 position[4];
    f32 orientation[4];
    u32 unit;
    u32 second;
    u32 color;
    u32 count;
    u32 i;
    f32 radius;
    f32 scale;

    if (progress < frame && progress != 0) {
        return;
    }
    count = config->point.timed.count;
    radius = config->radius;
    second = effColorAnimation(&config->point.timed.colorTrack, &config->point.timed.alphaTrack, frame, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->color, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    color = eePackRGBAWord(128.0f);
    scale = work->scale;
    SDF_VU0_COPYVECTOR(position, work->vectors.position);
    SDF_VU0_COPYVECTOR(orientation, work->vectors.orientation);
    for (i = 0; i < count; i++, entries++) {
        effClassWork_t *entry = *entries;
        effPointSetTable_t *points = (effPointSetTable_t *)entry->resource;
        f32 angle = points->rows->angle;

        position[0] = work->vectors.position[0] - sdfSin(angle) * radius;
        position[1] = work->vectors.position[1] - sdfCos(angle) * radius;
        effCopyClassResourcePosition(*entries, position);
        effCopyClassResourceOrientation(*entries, orientation);
        effSetClassResourceMatrixComponent(*entries, scale);
        effSetClassResourceColor(*entries, color);
        effDrawClassResourceWork(*entries);
    }
}

void effSeedBillScaleRange(effClassWork_t *work) {
    effBillRangeConfig_t *config = work->payload;
    effScaleRange_t *range = (effScaleRange_t *)work->resource;
    s32 steps = config->point.timed.time.duration;
    effScaleRangeEntry_t *entry = range->entries;
    f32 start = config->startBase * (dds3GetRandFloatA(effSharedRandomState) * config->startRand + (1.0f - config->startRand));
    u32 index;
    u32 count;

    if (steps > 0) {
        f32 end = config->endBase * (dds3GetRandFloatA(effSharedRandomState) * config->endRand + (1.0f - config->endRand));
        range->start = start;
        range->delta = (end - start) / (f32)steps;
    } else {
        range->start = start;
        range->delta = 0.0f;
    }
    count = config->point.timed.count;
    index = 0;
    if (count != 0) {
        do {
            index++;
            entry->negativeSeed = -1 - (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 7);
            entry++;
        } while (index < count);
    }
}

effScaleRange_t *effCreateRetainedPointSetColorRows(effBillPointConfig_t *src) {
    u32 count = src->timed.count;
    struct sdfMemHandle_t *allocation;
    effScaleRange_t *table;
    effScaleRangeEntry_t *row;
    u32 i;
    u32 alphaA;
    u32 alphaB;
    u32 alphaC;
    u32 lowA;
    u32 lowB;
    u32 lowC;
    s32 rampIn;
    s32 rampOut;

    allocation = sdfMemAllocDirect(count * sizeof(effScaleRangeEntry_t) + sizeof(effScaleRange_t));
    table = (effScaleRange_t *)sdfMemLock((struct sdfMemHandle_t *)((u32)allocation));
    table->allocation = allocation;
    table->entries = (effScaleRangeEntry_t *)(table + 1);
    if ((u32)src->layers < 3) {
        src->layers = 3;
    }
    lowA = src->colorA & 0xFFFFFF;
    lowB = src->colorB & 0xFFFFFF;
    lowC = src->colorC & 0xFFFFFF;
    alphaA = src->colorA >> 24;
    alphaB = src->colorB >> 24;
    alphaC = src->colorC >> 24;
    rampIn = (s32)(src->rangeFadeInEnd * (f32)(src->layers + 1));
    rampOut = (s32)(src->rangeFadeOutStart * (f32)(src->layers + 1));
    row = table->entries;
    for (i = 0; i < count; i++) {
        effPointSet_t *set = effCreatePointSet5(src->layers);
        u32 n;
        u32 *rec;
        u32 j;

        row->set = set;
        n = set->rows / 5;
        rec = (u32 *)set->tail;
        for (j = 0; j < n; j++) {
            f32 ratio;
            if (j < rampIn) {
                ratio = (f32)j / (f32)rampIn;
            } else if (j <= rampOut) {
                ratio = 1.0f;
            } else {
                ratio = (f32)(n - j) / (f32)(n - rampOut);
            }
            rec[0] = lowC | ((u32)((f32)alphaC * ratio) << 24);
            rec[1] = lowB | ((u32)((f32)alphaB * ratio) << 24);
            rec[2] = lowA | ((u32)((f32)alphaA * ratio) << 24);
            rec[3] = rec[1];
            rec[4] = rec[0];
            rec += 5;
        }
        row->negativeSeed = ~(i * 4);
        row++;
    }
    return table;
}

void effReleaseBillPointEntries(effClassWork_t *work) {
    effBillTimedHeader_t *config = work->payload;
    effScaleRange_t *range = (effScaleRange_t *)work->resource;
    u32 count = config->count;
    u32 index = 0;
    effScaleRangeEntry_t *entry = range->entries;

    if (count != 0) {
        do {
            effReleasePointSetAsset(entry->set);
            entry++;
        } while (++index < count);
    }
    sdfMemFreeDirect(range->allocation);
}

/* Advance orbiting entries and rebuild their camera-facing, wavy point spans. */
void effUpdateOrbitalPointSpanWaves(effClassWork_t *work) {
    f32 offsets[5];
    f32 widths[5];
    f32 direction[4];
    f32 radial[4];
    f32 tangent[4];
    f32 view[4];
    f32 position[4];
    f32 previous[4];
    f32 rotated[4];
    effBillRangeConfig_t *config = work->payload;
    effScaleRange_t *range = (effScaleRange_t *)work->resource;
    effScaleRangeEntry_t *entry = (effScaleRangeEntry_t *)range->entries;
    u32 progress = config->point.timed.time.progress;
    s32 count;
    u8 useHeight;
    s32 respawnDelay;
    s32 remaining;
    f32 step;
    f32 layers;
    f32 phase;
    f32 waveStep;
    f32 amplitude;
    f32 bendScale;
    f32 acceleration;
    f32 bendStep;
    f32 radius;
    if (progress < work->frame && progress != 0) {
        return;
    }
    count = config->point.timed.count;
    respawnDelay = config->point.respawnDelay;
    layers = (f32)(u32)config->point.layers;
    step = config->point.totalLength / layers;
    waveStep = ((f32)config->point.periods * 3.14159265f) / layers;
    amplitude = config->point.amplitude;
    acceleration = config->angularAcceleration;
    useHeight = config->tilted;
    if (step <= 0.0f) {
        return;
    }
    SDF_VU0_LOADVECTOR(&aViewEye);
    SDF_VU0_LOADVECTOR_B(&aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(view);
    widths[0] = 0;
    widths[1] = config->point.edgeWidth;
    widths[2] = widths[1] + config->point.halfWidth;
    widths[3] = widths[2] + config->point.halfWidth;
    widths[4] = widths[3] + config->point.edgeWidth;
    position[3] = direction[3] = 1.0f;
    bendStep = -((step / 10.0f) * (5.0f * (3.14159265f / 180.0f)));
    bendScale = (20.0f * (3.14159265f / 180.0f));
    radius = range->start;
    range->start = radius + range->delta;
    if (count > 0) {
    remaining = count;
    for (; remaining != 0; remaining--, entry++) {
        if (entry->negativeSeed == -1) {
            f32 r = dds3GetRandFloatA(effSharedRandomState);
            f32 jitter = config->angularRand;
            f32 baseSpeed = config->angularBase;
            entry->phase = 0;
            entry->angularRate = baseSpeed * (r * jitter + (1.0f - jitter));
            if (!useHeight) {
                entry->yaw = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                entry->height = 0;
            } else {
                entry->yaw = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * (45.0f * (3.14159265f / 180.0f));
                entry->height = -config->heightScale * dds3GetRandFloatA(effSharedRandomState);
            }
            entry->pitch = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
            SetRotateMatrixX(entry->yaw);
            SetRotateMatrixY_B(entry->pitch);
            MultMatrixRevNoArg();
            phase = entry->phase;
            position[0] = sdfCos(phase) * radius;
            position[1] = 0;
            position[2] = sdfSin(phase) * radius;
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_APPLYMATRIX_XYZ();
            SDF_VU0_STOREVECTOR(position);
            entry->position[0] = position[0];
            entry->color = 0xA0808080;
            entry->position[1] = position[1];
            entry->position[2] = position[2];
        } else {
            if (entry->color & 0xFF000000) {
                entry->color += 0xE0000000;
            } else {
                entry->negativeSeed = -1;
                if (respawnDelay > 0) {
                    entry->negativeSeed -= (u32)dds3GetRandA((struct effRandState_t *)effSharedRandomState) % (u32)respawnDelay;
                }
                continue;
            }
            {
                effPointSet_t *set = entry->set;
                f32 *vertices = (f32 *)set->buffer;
                s32 rows = set->rows / 5;
                f32 stepSquared;
                f32 waveAngle;
                f32 waveHeight;
                f32 waveRadius;
                f32 oldSlope;
                f32 inner;
                f32 edgeWidth;
                f32 width;
                f32 curve;
                f32 curveSample;
                f32 newHeight;
                f32 delta;
                f32 segmentLength;
                s32 j;
                s32 k;
                if (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 1) {
                    amplitude = -amplitude;
                }
                stepSquared = step * step;
                waveAngle = dds3GetRandFloatA(effSharedRandomState) * 3.14159265f;
                waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.95f + (1.0f - 0.95f));
                waveHeight = sdfSin(waveAngle) * waveRadius;
                waveAngle += waveStep;
                newHeight = sdfSin(waveAngle) * waveRadius;
                delta = newHeight - waveHeight;
                waveHeight = newHeight;
                segmentLength = fsqrtf(delta * delta + stepSquared);
                oldSlope = asinf(delta / segmentLength);
                inner = config->point.halfWidth;
                edgeWidth = config->point.edgeWidth;
                curveSample = dds3GetRandFloatA(effSharedRandomState);
                width = inner + edgeWidth;
                phase = (entry->angularRate + acceleration * (f32)(entry->negativeSeed + 1) * 0.5f) * (f32)(entry->negativeSeed + 1);
                phase = phase + entry->phase;
                curve = bendStep * (curveSample * 0.7f + (1.0f - 0.7f));
                SetRotateMatrixX(entry->yaw);
                SetRotateMatrixY_B(entry->pitch);
                MultMatrixRevNoArg();
                position[0] = sdfCos(phase) * radius;
                position[1] = 0;
                position[2] = sdfSin(phase) * radius;
                SDF_VU0_LOADVECTOR(position);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR(position);
                SDF_VU0_STOREVECTOR(previous);
                SDF_VU0_NORMALIZE();
                SDF_VU0_STOREVECTOR(radial);
                direction[0] = previous[0] - entry->position[0];
                direction[1] = previous[1] - entry->position[1];
                direction[2] = previous[2] - entry->position[2];
                SDF_VU0_LOADVECTOR(direction);
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_LOADVECTOR_B(radial);
                SDF_VU0_STOREVECTOR(direction);
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_STOREVECTOR(tangent);
                SDF_VU0_LOADVECTOR_B(view);
                SDF_VU0_COPYVECTOR_C_TO_A();
                SDF_VU0_OUTERPRODUCT();
                SDF_VU0_NORMALIZE();
                SDF_VU0_COPYVECTOR_A_TO_C();
                SDF_VU0_COPYVECTOR_A_TO_B();
                VU0_SCALE_VF(vf10, width);
                SDF_VU0_STOREVECTOR(vertices);
                SDF_VU0_NEGVECTOR_XYZ();
                SDF_VU0_STOREVECTOR(vertices + 16);
                VU0_SCALE_VF(vf11, inner);
                SDF_VU0_STOREVECTOR_B(vertices + 4);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_STOREVECTOR_B(vertices + 12);
                VU0_STORE_VF(vf0, vertices + 8);
                position[1] += entry->height;
                SDF_VU0_LOADVECTOR_B(position);
                for (j = 0; j < 5; j++) {
                    SDF_VU0_LOADVECTOR(vertices + j * 4);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(vertices + j * 4);
                    offsets[j] = 0;
                }
                entry->position[0] = previous[0];
                entry->position[1] = previous[1];
                entry->position[2] = previous[2];
                vertices += 20;
                for (k = 1; k < rows; k++) {
                    f32 tilt = bendScale * (dds3GetRandFloatA(effSharedRandomState) * 0.75 + 0.25);
                    f32 slope;
                    f32 distance;
                    f32 tangentScale;
                    if (dds3GetRandA((struct effRandState_t *)effSharedRandomState) & 1) {
                        bendScale = -bendScale;
                    }
                    waveAngle += waveStep;
                    if (waveAngle >= 3.14159265f) {
                        waveAngle -= 3.14159265f;
                        amplitude = -amplitude;
                        waveRadius = amplitude * (dds3GetRandFloatA(effSharedRandomState) * 0.95f + (1.0f - 0.95f));
                        curve = bendStep * (dds3GetRandFloatA(effSharedRandomState) * 0.7f + (1.0f - 0.7f));
                    }
                    newHeight = sdfSin(waveAngle) * waveRadius;
                    delta = newHeight - waveHeight;
                    segmentLength = fsqrtf(delta * delta + stepSquared);
                    slope = asinf(delta / segmentLength) + tilt;
                    SetRotateMatrixAxis(oldSlope, (const sdfVector3_t *)radial);
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(rotated);
                    SetRotateMatrixAxis(curve, (const sdfVector3_t *)tangent);
                    SDF_VU0_LOADVECTOR(direction);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(direction);
                    SDF_VU0_LOADVECTOR(previous);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_APPLYMATRIX_XYZ();
                    SDF_VU0_STOREVECTOR(previous);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_GETLENGTH(distance);
                    tangentScale = atanf((slope - oldSlope) * 0.5f);
                    oldSlope = slope;
                    waveHeight = newHeight;
                    SDF_VU0_LOADVECTOR_C(rotated);
                    for (j = 0; j < 5; j++) {
                        f32 extra = widths[j] * tangentScale;
                        f32 length;
                        offsets[j] += extra;
                        SDF_VU0_COPYVECTOR_C_TO_B();
                        SDF_VU0_LOADVECTOR(vertices + j * 4 - 20);
                        length = distance + offsets[j];
                        VU0_SCALE_VF(vf11, length);
                        SDF_VU0_ADDVECTOR();
                        SDF_VU0_STOREVECTOR(vertices + j * 4);
                        offsets[j] = extra;
                    }
                    vertices += 20;
                }
            }
        }
        entry->negativeSeed++;
    }
    }
}

/* vu0 routine: tint point sets and draw under the class quaternion transform.
 * Retail color homes +0x50/+0x60/+0x70/+0x80 are 16-byte SDK-sized locals. */
void effDrawRetainedPointSetRows(effClassWork_t *work) {
    effBillRangeConfig_t *config = work->payload;
    s32 frame = work->frame;
    s32 duration = config->point.timed.time.duration;
    effScaleRangeEntry_t *row = ((effScaleRange_t *)work->resource)->entries;
    s32 count;
    f32 colorFactor[4];
    effMatrix4_t matrix;
    s32 classColor[4];
    s32 fadeColor[4];
    s32 entryColor[4];
    s32 blended[4];
    u32 fade;
    u32 packed;
    u32 unit;

    if (duration < frame && duration != 0) {
        return;
    }
    count = config->point.timed.count;
    fade = effColorAnimation(&config->point.timed.colorTrack,
        &config->point.timed.alphaTrack, frame, duration);
    classColor[0] = work->color;
    unit = 0x3C000000;
    EE_MMI_RGBA_UNPACK(classColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    fadeColor[0] = fade;
    EE_MMI_RGBA_UNPACK(fadeColor, unit);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(colorFactor);
    SDF_VU0_LOADVECTOR(work->vectors.orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work->vectors.position);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(&matrix);
    if (count > 0) {
        s32 remaining = count;
        do {
            if (row->negativeSeed > 0) {
                effPointSet_t *set = row->set;
                entryColor[0] = row->color;
                EE_MMI_RGBA_UNPACK(entryColor, 1.0f / 128.0f);
                SDF_VU0_LOADVECTOR_B(colorFactor);
                SDF_VU0_MULVECTOR();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                blended[0] = packed;
                set->color = blended[0];
                set->type = config->point.timed.alphaTrack.surfaceIndex;
                set->flag = config->point.drawFlag;
                effDrawFivePointGroups(set, &matrix);
            }
            remaining--;
            row++;
        } while (remaining != 0);
    }
}

effClassWork_t *effCreateClassResourceWork(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effClassResourceWorkOperations[kind].payloadSize;
    effClassWork_t *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->frame = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    effect->resource = effClassResourceWorkOperations[kind].createResource(source);
    effClassResourceWorkOperations[kind].initialize(effect);
    return effect;
}

effClassWork_t *effCreateClassResourceFromFile(fileJobPayload_t *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateClassResourceWork(request->option, source);
}

void effDestroyClassResourceWork(effClassWork_t *work) {
    effClassResourceWorkOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

effClassWork_t *effCloneClassResourceWork(effClassWork_t *work) {
    return effCreateClassResourceWork((u16)work->kind, work->payload);
}

void effResetDispatchCounter(effClassWork_t *work) {
    effClassResourceWorkOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceClassResourceFrame(effClassWork_t *work) {
    effClassResourceWorkOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawClassResourceWork(effClassWork_t *work) {
    effClassResourceWorkOperations[work->kind].draw(work);
}

void effCallSRAnimation10(effClassWork_t *work) {
    if ((effSRAnimation() & 1) == 0) {
        effAdvanceClassResourceFrame(work);
    }
    effDrawClassResourceWork(work);
}

void effCopyClassResourcePosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

void effPCPBossBaelSetPosition(effPCPBossBael_t *hData, const void *pPos)
{
    SDF_VU0_COPYVECTOR(&hData->param, pPos);
}

void effSetClassResourceColor(effClassWork_t *work, u32 color) {
    work->color = color;
}

void effSetClassResourceMatrixComponent(effClassWork_t *work, float value) {
    work->scale = value;
}

effPointSet_t *effCreatePointSet5(s32 count) {
    s32 rows = count * 5 + 5;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    effPointSet_t *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (effPointSet_t *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(effWork3, 0, 0x2C);
    effWork3[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleasePointSetAsset(effPointSet_t *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

void effDrawFivePointGroups(effPointSet_t *set, effMatrix4_t *matrix) {
    void *list;
    effGsPacket_t *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        if (matrix == NULL) {
            SDF_VU0_LOADIDENTITY();
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdf3DPrimLinkTm(list, 0);
        sdf3DPrimLinkMat(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        remaining = set->rows;
        effWork3[0].vertexColor = (u32 *)set->tail;
        effWork3[0].vertex = (u128 *)set->buffer;
        effWork3[0].color = set->color;
        effWork3[0].faceCount = 0x10;
        effWork3[0].pointCount = 0xF;
        effWork3[0].faceIndex = effDrawFivePointGroupsData;
        while (remaining >= 0xF) {
            remaining -= 0xA;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(effWork3));
            effWork3[0].vertex += 0xA;
            effWork3[0].vertexColor += 0xA;
        }
        if (remaining >= 0xA) {
            effWork3[0].faceCount = 8;
            effWork3[0].pointCount = remaining;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(effWork3));
        }
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        effDrawFivePointGroupsData2[set->type]->regFunc(effDrawFivePointGroupsData2[set->type], list);
    }
}

effStripNode_t *effCreateStripNode(u32 percent) {
    effStripNode_t *node = sdfChipAllocZeroDirect(sizeof(effStripNode_t));
    node->percent = percent;
    node->color = 0x80808080;
    node->opacity = 1.0f;
    node->active = 0;
    node->trackSet = effCreateTrackSetWithSharedReferences(percent * 4, 2, 0);
    node->resource = dds3BillboardGeneral(0);
    node->count = 1;
    return node;
}

effStripNode_t *effCreateStripNodeFromGrid(fileKeyBlock_t *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* The strip capacity uses the same signed product as 002A5A58. */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return effCreateStripNode(n > 100 ? 100 : n);
}

effStripNode_t *effFileResourceReferenceReplace(fileJobPayload_t *work) {
    u8 *source = fileResolvePrimaryBuffer(work);
    fileKeyBlock_t *params = (fileKeyBlock_t *)(source + 0x20);
    effStripNode_t *node = effCreateStripNodeFromGrid(params);

    memcpy(&node->copiedHeader, source, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->option, params);
    return node;
}

void Delete_effPolygonTrack(effStripNode_t *work) {
    if (work->resource != NULL) {
        dds3BillboardDelete(work->resource);
    }
    if (work->trackSet != NULL) {
        effReleaseResourceRefs(work->trackSet);
    }
    if (work->active != 0) {
        effPCPHelperBEDGetModel(work->active);
    }
    sdfChipFreeDirect(work);
}

effStripNode_t *effCloneStripResourceFromOwner(effStripNode_t *work) {
    fileKeyBlock_t *params = (fileKeyBlock_t *)work->active->data1;
    effStripNode_t *node = effCreateStripNodeFromGrid(params);
    memcpy(&node->copiedHeader, params, sizeof(node->copiedHeader));
    effReplaceFileResourceRef(node, work->active->type, params);
    return node;
}

void effReplaceFileResourceRef(effStripNode_t *node, u32 entryId, const void *resource) {
    if (node->active != 0) {
        effPCPHelperBEDGetModel(node->active);
    }
    node->active = fileAllocateGridRecordSlots((u16)entryId, node->percent, resource);
}

void effClearStripRecordReferences(effStripNode_t *node) {
    if (node->active != 0) {
        fileClearRecordReferences(node->active);
        return;
    }
}

void effAcquireStripRecord(effStripNode_t *node) {
    if (node->active != 0) {
        fileAcquireRecord(node->active);
        return;
    }
}

void effDrawEffectStrip(effStripNode_t *node) {
    effMatrix4_t matrix;
    f32 nodeTint[4];
    f32 tint[4];
    u128 translation[1];
    u128 rotation[1];
    f32 view[4];
    f32 rightAxis[4];
    f32 viewUp[4];
    f32 right[4];
    f32 up[4];
    f32 position[4];
    f32 innerTint[4];
    f32 outerTint[4];
    u32 packedInnerColor;
    u32 packedOuterColor;
    u32 childColor;
    fileKeyBlock_t *info;
    effTrackSet_t *output;
    effPosture_t *entry;
    f32 (*positions)[4];
    f32 (*quad)[4];
    u32 *colors;
    u32 *quadColors;
    struct billHandle_t *billboard;
    s32 count;
    s32 remaining;
    u32 hasTransform;
    s32 i;
    f32 opacity;
    f32 billSize;
    f32 crossSize;
    f32 longSize;
    f32 sideSize;
    f32 scale;
    f32 cross;
    f32 billExtent;
    f32 along;
    f32 side;

    info = (fileKeyBlock_t *)node->active->data0;
    eeUnpackRGBAWordFloat(node->color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(nodeTint);
    eeUnpackRGBAWordFloat(node->copiedHeader.tint, 1.0f / 128.0f);
    SDF_VU0_LOADVECTOR_B(nodeTint);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(tint);
    hasTransform = node->active->flags & 1;
    if (!hasTransform) {
        EE_MMI_UNIT_MATRIX(&matrix);
    } else {
        fileReadRecordSecondVector(node->active, rotation);
        fileReadVectorPtr20(node->active, translation);
        SDF_VU0_LOADVECTOR(rotation);
        ToMatrix();
        SDF_VU0_LOADVECTOR(translation);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_SET_ROW3();
        VU0_STORE_MATRIX(&matrix);
    }
    billboard = node->resource;
    opacity = node->opacity;
    billSize = node->copiedHeader.billSize;
    dds3BillboardSetMode(billboard, (s16)info->alphaTrack.surfaceIndex);
    SDF_VU0_LOADVECTOR(&aViewTarget);
    SDF_VU0_LOADVECTOR_B(&aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(view);
    SDF_VU0_LOADVECTOR_B(&aViewDown);
    SDF_VU0_STOREVECTOR_B_NOMEM(up);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR_NOMEM(right);
    output = node->trackSet;
    entry = node->active->slots;
    count = node->percent;
    positions = (f32 (*)[4])output->buffer;
    colors = (u32 *)output->tail;
    if (info->length <= 0 || count == 0) {
        return;
    }
    longSize = node->copiedHeader.longSize;
    sideSize = node->copiedHeader.sideSize;
    crossSize = node->copiedHeader.crossSize;
    eeUnpackRGBAWordFloat(node->copiedHeader.innerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(innerTint);
    eeUnpackRGBAWordFloat(node->copiedHeader.outerColor, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR_NOMEM(outerTint);
    if (count > 0) {
        remaining = count;
        do {
            if ((u32)(FILE_SLOT_FRAME(entry) + 2) < 2) {
                quad = positions;
                for (i = 0; i < 4; i++, quad += 4) {
                    VU0_STORE_VF(vf0, quad[0]);
                    VU0_STORE_VF(vf0, quad[1]);
                    VU0_STORE_VF(vf0, quad[2]);
                    VU0_STORE_VF(vf0, quad[3]);
                }
            } else if (FILE_SLOT_FRAME(entry) > 0) {
                scale = entry->scale * opacity;
                if (hasTransform) {
                    VU0_LOAD_MATRIX(&matrix);
                    SDF_VU0_LOADVECTOR(entry->pos);
                    SDF_VU0_APPLYMATRIX_XYZ1();
                    SDF_VU0_STOREVECTOR(position);
                } else {
                    SDF_VU0_COPYVECTOR(position, entry->pos);
                }
                eeUnpackRGBAWordFloat(entry->color, 1.0f / 128.0f);
                SDF_VU0_LOADVECTOR_B(tint);
                SDF_VU0_MULVECTOR();
                childColor = eePackRGBAWord(128.0f);
                cross = crossSize * scale;
                billExtent = billSize * scale;
                dds3BillboardSetRGBA(billboard, childColor);
                dds3BillboardSetScale(billboard, billExtent, billExtent);
                dds3BillboardSetRotate(billboard, entry->angle);
                effCopyVector(billboard->pos, position);
                dds3BillboardDraw(billboard);
                along = longSize * scale * 1.5f;
                side = sideSize * scale * 1.5f;
                SetRotateMatrixAxis(entry->angle, (const sdfVector3_t *)view);
                SDF_VU0_LOADVECTOR(right);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(rightAxis);
                SDF_VU0_LOADVECTOR(up);
                SDF_VU0_APPLYMATRIX_XYZ();
                SDF_VU0_STOREVECTOR_NOMEM(viewUp);
                SDF_VU0_LOADVECTOR_C(entry->pos);
                SDF_VU0_STOREVECTOR_C(positions[2]);
                SDF_VU0_STOREVECTOR_C(positions[6]);
                SDF_VU0_LOADVECTOR(rightAxis);
                VU0_SCALE_VF(vf10, along);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(positions[0]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(positions[4]);
                SDF_VU0_LOADVECTOR(viewUp);
                VU0_SCALE_VF(vf10, cross);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(positions[1]);
                SDF_VU0_STOREVECTOR(positions[5]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(positions[3]);
                SDF_VU0_STOREVECTOR_B(positions[7]);
                quad = positions + 8;
                /* Retail addresses this store from positions (+0xA0) but the next one from quad (+0x60):
                 * a bare volatile asm (no memory clobber) flushes cse's table, so the second address is not
                 * folded (cc1probe --cse, hooked==unhooked: uid599 stays quad+96). */
                VU0_STORE_VF_UNCLOBBERED(vf12, quad[2]);
                SDF_VU0_STOREVECTOR_C(quad[6]);
                SDF_VU0_LOADVECTOR(viewUp);
                VU0_SCALE_VF(vf10, side);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(quad[0]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(quad[4]);
                SDF_VU0_LOADVECTOR(rightAxis);
                VU0_SCALE_VF(vf10, cross);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_ADDVECTOR_A_C();
                SDF_VU0_STOREVECTOR(quad[1]);
                SDF_VU0_STOREVECTOR(quad[5]);
                VU0_NEGATE_XYZ(vf11);
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(quad[3]);
                SDF_VU0_STOREVECTOR_B(quad[7]);
                eeUnpackRGBAWordFloat((entry->color & 0xFF000000) | 0x808080, 1.0f / 128.0f);
                SDF_VU0_COPYVECTOR_A_TO_B();
                SDF_VU0_LOADVECTOR(innerTint);
                SDF_VU0_MULVECTOR();
                packedInnerColor = eePackRGBAWord(128.0f);
                SDF_VU0_LOADVECTOR(outerTint);
                SDF_VU0_MULVECTOR();
                packedOuterColor = eePackRGBAWord(128.0f);
                quadColors = colors;
                for (i = 3; i >= 0; i--, quadColors += 4) {
                    quadColors[0] = packedOuterColor;
                    quadColors[1] = packedOuterColor;
                    quadColors[2] = packedInnerColor;
                    quadColors[3] = packedOuterColor;
                }
            }
            entry++;
            positions += 16;
            colors += 16;
        } while (--remaining != 0);
    }
    output->color = node->color;
    output->flag = node->copiedHeader.flag;
    output->type = info->alphaTrack.surfaceIndex;
    effDrawTrackSet(output, &matrix);
}

/* Advance the active strip record, rebuild its geometry, and submit its tracks. */
void effUpdateAndDrawStripRecord(effStripNode_t *node) {
    effAcquireStripRecord(node);
    effDrawEffectStrip(node);
}

void effSetStripRecordVector(effStripNode_t *node, const u128 *value) {
    mnuRecordSetVector(node->active, value);
}

void effSetStripRecordSecondaryVector(effStripNode_t *node, const u128 *value) {
    fileSetRecordSecondVector(node->active, value);
}

void effSetStripRecordColor(effStripNode_t *work, u32 color) {
    work->color = color;
}

/* Store opacity and notify the strip's active-record callback. */
void effSetStripOpacity(effStripNode_t *p, f32 v) {
    p->opacity = v;
    dds3DispatchIndexedCallback((u16 *)p->active, v);
}

void effResetBillTable(u8 *work) {
    u32 index = 0;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u32 count = ((effBillEmitterCommon_t *)((effClassWork_t *)work)->payload)->header.timed.count;
    effBillEmitterEntry_t *entry = (effBillEmitterEntry_t *)((effFrameState_t *)state)->entries;
    u32 *flags = ((effRibbonWork_t *)((effFrameState_t *)state)->asset)->colors;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            entry->timer = -1;
            entry++;
        } while (index < count);
    }
}

u8 *effAllocateRingFadeEntries(effBillVortexConfig_t *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * sizeof(effBillEmitterEntry_t) + 0xC);
    effFrameState_t *node = (effFrameState_t *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

/* Ring fade tables of the ring effects: `segments + 1` entries of colors
 * (4 words each) and radii (8 words each). The alpha fades in over the first
 * fadeIn fraction of the entries and out from the fadeOut fraction; the table
 * is then copied for every remaining layer. */
void effFillRingFadeGradient(u8 *node, effBillEmitterCommon_t *config) {
    effBillEmitterCommon_t *cfg = config;
    effRibbonWork_t *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (effRibbonWork_t *)((effFrameState_t *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateRingFadeTable(effBillVortexConfig_t *config, u32 resource) {
    u8 *node = effAllocateRingFadeEntries(config);

    ((effFrameState_t *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct sdfTextureFileHeader_t *)(u32)resource);
    effFillRingFadeGradient(node, &config->common);
    return node;
}

u8 *effAssetPointerSet(u8 *work) {
    effBillVortexConfig_t *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = effAllocateRingFadeEntries(config);
    ((effFrameState_t *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((effRibbonWork_t *)((effFrameState_t *)state)->asset);
    effFillRingFadeGradient(node, &config->common);
    return node;
}

void effReleaseRingFadeTable(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effSharedAssetReferenceRelease((effRibbonWork_t *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

/* NON_MATCHING: 168 of 448 words match. Same calls and stores, 2 extra words; register allocation and load scheduling differ (colors pointer hoisted, 1.0f kept in f29). */
#ifdef NON_MATCHING
/* vu0 routine: spawns and advances vortex ribbons; each segment row flares out around flareCenter. */
void effCallGetRandFloatA(effClassWork_t *work) {
    effBillVortexConfig_t *config = (effBillVortexConfig_t *)work->payload;
    effFrameState_t *state;
    effRibbonWork_t *asset;
    effBillEmitterEntry_t *particle;
    f32 pos[4];
    f32 normal[4];
    u32 *colors;
    u32 i;
    u32 count;
    u32 stride;
    u32 segments;
    s32 burst;
    u32 spawn;
    u8 respawn;
    u32 life;
    f32 (*positions)[4];
    f32 *heights;
    u32 j;
    s32 timer;
    f32 segmentCount;
    f32 spinAccel;
    f32 heightScale;
    f32 rise;
    f32 flareCenter;
    f32 radius;
    f32 width;
    f32 angle;
    f32 step;
    f32 climb;
    f32 y;
    f32 flare;
    f32 height;
    f32 start;
    f32 time;

    if (config->common.header.timed.time.progress < work->frame && config->common.header.timed.time.progress != 0) {
        return;
    }
    life = config->common.header.life;
    count = config->common.header.timed.count;
    if (life == 0) {
        return;
    }
    if (config->common.burst != 0 && work->frame == 0) {
        burst = 1;
        spawn = count;
    } else {
        burst = 0;
        spawn = config->common.header.spawnRate;
    }
    segments = config->common.header.segments + 1;
    state = (effFrameState_t *)work->resource;
    asset = (effRibbonWork_t *)state->asset;
    particle = (effBillEmitterEntry_t *)state->entries;
    stride = asset->rowStride;
    respawn = config->common.header.respawn;
    spinAccel = config->spinAccel;
    positions = (f32 (*)[4])asset->positions;
    heights = (f32 *)asset->uvs;
    segmentCount = segments;
    colors = asset->colors;
    heightScale = config->common.height;
    rise = config->common.rise;
    flareCenter = config->flareCenter;
    for (i = 0; i < count; i++, particle++) {
        timer = particle->timer;
        if (timer != -2) {
            if (timer == -1) {
                if (spawn != 0) {
                    f32 delta;
                    start = config->common.radiusStart * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusStartJitter + (1.0f - config->common.radiusStartJitter));
                    delta = (config->common.radiusEnd * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusEndJitter + (1.0f - config->common.radiusEndJitter)) - start) / (f32)life;
                    particle->radius = start;
                    particle->radiusStep = delta;
                    start = -config->climbStart * (dds3GetRandFloatA(effSharedRandomState) * config->climbStartJitter + (1.0f - config->climbStartJitter));
                    delta = (-config->climbEnd * (dds3GetRandFloatA(effSharedRandomState) * config->climbEndJitter + (1.0f - config->climbEndJitter)) - start) / (f32)life;
                    particle->vertical.vortex.climb = start;
                    particle->vertical.vortex.climbStep = delta;
                    particle->geometry.vortex.flare = config->flare * (dds3GetRandFloatA(effSharedRandomState) * config->flareJitter + (1.0f - config->flareJitter));
                    particle->angle = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                    particle->spin = config->spin * (dds3GetRandFloatA(effSharedRandomState) * config->spinJitter + (1.0f - config->spinJitter));
                    particle->width = config->common.width * (dds3GetRandFloatA(effSharedRandomState) * config->common.widthJitter + (1.0f - config->common.widthJitter));
                    particle->geometry.vortex.sweep = config->common.extent * (dds3GetRandFloatA(effSharedRandomState) * config->common.extentJitter + (1.0f - config->common.extentJitter));
                    particle->geometry.vortex.height = config->common.height * (dds3GetRandFloatA(effSharedRandomState) * config->common.heightJitter + (1.0f - config->common.heightJitter));
                    *colors = 0;
                    if (burst != 0) {
                        particle->timer = dds3GetRandA(effSharedRandomState) % life;
                        time = (f32)particle->timer;
                        particle->radius += particle->radiusStep * time;
                        particle->vertical.vortex.climb += particle->vertical.vortex.climbStep * time;
                    } else {
                        particle->timer = 0;
                    }
                    spawn--;
                }
            } else if ((u32)timer >= life) {
                *colors = 0;
                particle->timer = respawn ? -1 : -2;
            } else {
                time = (f32)timer;
                particle->vertical.vortex.climb += particle->vertical.vortex.climbStep;
                particle->radius += particle->radiusStep;
                radius = particle->radius;
                width = particle->width * 0.25f;
                angle = (particle->spin + spinAccel * time * 0.5f) * time;
                angle += particle->angle;
                climb = particle->vertical.vortex.climb / segmentCount;
                normal[1] = climb;
                if (particle->spin >= 0.0f) {
                    step = particle->geometry.vortex.sweep / segmentCount;
                } else {
                    step = -particle->geometry.vortex.sweep / segmentCount;
                }
                flare = particle->geometry.vortex.flare;
                height = particle->geometry.vortex.height;
                y = 0.0f;
                for (j = 0; j < segments; j++) {
                    f32 row = (f32)j / segmentCount;
                    f32 c = sdfCos(angle);
                    f32 s = sdfSin(angle);
                    f32 bulge;
                    f32 rowHeight;

                    pos[1] = y;
                    bulge = (row - flareCenter) * (row - flareCenter) * flare;
                    normal[2] = s * bulge;
                    normal[0] = c * bulge;
                    pos[2] = s * (radius + bulge);
                    pos[0] = c * (radius + bulge);
                    SDF_VU0_LOADVECTOR(normal);
                    SDF_VU0_NORMALIZE();
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[1]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[2]);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[3]);
                    angle += step;
                    y += climb;
                    positions += 4;
                    rowHeight = heightScale * ((f32)j / segmentCount) + height;
                    heights[7] = rowHeight;
                    heights[1] = rowHeight;
                    heights[3] = rowHeight;
                    heights[5] = rowHeight;
                    heights += 8;
                }
                *colors++ = effColorAnimation(&config->common.header.emitterColorTrack, &config->common.header.emitterAlphaTrack, timer, life);
                particle->geometry.vortex.height += rise;
                particle->timer++;
                continue;
            }
        }
        colors++;
        positions += stride;
        heights += stride * 2;
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallGetRandFloatA);
#endif

void effBlendBillboardInstanceColorsAndTransforms(u8 *work) {
    effBillVortexConfig_t *config = (effBillVortexConfig_t *)((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effRibbonWork_t *out = (effRibbonWork_t *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (effMatrix4_t *)mtx);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effResetBillboardFrameInstanceCounters);

u8 *effAllocateBillFadeFrameEntries(effBillColumnConfig_t *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * sizeof(effBillEmitterEntry_t) + 0xC);
    effFrameState_t *node = (effFrameState_t *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillBillFadeGradient(u8 *node, effBillEmitterCommon_t *config) {
    effBillEmitterCommon_t *cfg = config;
    effRibbonWork_t *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (effRibbonWork_t *)((effFrameState_t *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateBillFadeTable(effBillColumnConfig_t *config, u32 resource) {
    u8 *node = effAllocateBillFadeFrameEntries(config);

    ((effFrameState_t *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct sdfTextureFileHeader_t *)(u32)resource);
    effFillBillFadeGradient(node, &config->common);
    return node;
}

u8 *effCloneBillFadeTable(u8 *work) {
    effBillColumnConfig_t *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = effAllocateBillFadeFrameEntries(config);
    ((effFrameState_t *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((effRibbonWork_t *)((effFrameState_t *)state)->asset);
    effFillBillFadeGradient(node, &config->common);
    return node;
}

void effReleaseBillFadeTable(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effSharedAssetReferenceRelease((effRibbonWork_t *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

/* Update the column's particle records and the ribbon vertices for this frame. */
void effUpdateColumnParticlesAndRibbons(effClassWork_t *work) {
    effBillColumnConfig_t *config = (effBillColumnConfig_t *)work->payload;
    effBillEmitterCommon_t *common = &config->common;
    u32 frameLimit = work->frame;
    u32 progress = common->header.timed.time.progress;
    u32 layer;
    u32 particleCount;
    s32 vertexStride;
    s32 spawnAll;
    u8 respawn;
    f32 rise;
    f32 spinAccel;
    f32 gravity;
    f32 flareCenter;
    f32 columnHeight;
    u32 life;
    u32 segmentCount;
    f32 segmentCountAsFloat;
    u32 spawnRemaining;
    effFrameState_t *frameState;
    effBillColumnParticle_t *particles;
    effRibbonWork_t *asset;
    u32 *colors;
    f32 *vertices;
    f32 *heightRows;

    if (progress < frameLimit && progress != 0) {
        return;
    }
    particleCount = common->header.timed.count;
    life = common->header.life;
    if (life == 0) {
        return;
    }

    if (common->burst != 0 && frameLimit == 0) {
        spawnAll = 1;
        spawnRemaining = particleCount;
    } else {
        spawnAll = 0;
        spawnRemaining = common->header.spawnRate;
    }
    frameState = (effFrameState_t *)work->resource;
    particles = (effBillColumnParticle_t *)frameState->entries;
    asset = (effRibbonWork_t *)frameState->asset;
    segmentCount = common->header.segments + 1;
    respawn = common->header.respawn;
    spinAccel = config->spinAccel;
    gravity = config->gravity;
    vertices = (f32 *)asset->positions;
    heightRows = (f32 *)asset->uvs;
    colors = asset->colors;
    vertexStride = asset->rowStride;
    segmentCountAsFloat = (f32)segmentCount;
    columnHeight = common->height;
    layer = 0;
    flareCenter = config->flareCenter;
    rise = common->rise;

    for (; layer < particleCount; layer++, particles++) {
        effBillColumnParticle_t *particle = particles;
        s32 age = particle->age;
        f32 ageF;

        if (age != -2) {
            if (age == -1) {
                f32 radius;
                f32 radiusStep;

                if (spawnRemaining != 0) {
                    radius = common->radiusStart *
                        (dds3GetRandFloatA(effSharedRandomState) * common->radiusStartJitter +
                         (1.0f - common->radiusStartJitter));
                    radiusStep =
                        (common->radiusEnd * (dds3GetRandFloatA(effSharedRandomState) * common->radiusEndJitter +
                                              (1.0f - common->radiusEndJitter)) - radius) /
                        (f32)life;
                    particle->radius = radius;
                    particle->radiusStep = radiusStep;
                    particle->drag = config->drag *
                        (dds3GetRandFloatA(effSharedRandomState) * config->dragJitter +
                         (1.0f - config->dragJitter));
                    particle->verticalOffset = -config->velocity * dds3GetRandFloatA(effSharedRandomState);
                    particle->flare = config->flare *
                        (dds3GetRandFloatA(effSharedRandomState) * config->flareJitter +
                         (1.0f - config->flareJitter));
                    particle->spinPhase = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                    particle->spin = config->spin *
                        (dds3GetRandFloatA(effSharedRandomState) * config->spinJitter +
                         (1.0f - config->spinJitter));
                    particle->width = common->width *
                        (dds3GetRandFloatA(effSharedRandomState) * common->widthJitter +
                         (1.0f - common->widthJitter));
                    particle->extent = common->extent *
                        (dds3GetRandFloatA(effSharedRandomState) * common->extentJitter +
                         (1.0f - common->extentJitter)) * 10.0f;
                    {
                        f32 random = dds3GetRandFloatA(effSharedRandomState);
                        f32 height = common->height *
                            (random * common->heightJitter + (1.0f - common->heightJitter));
                        colors[0] = 0;
                        particle->height = height;
                    }

                    if (spawnAll) {
                        s32 birthAge = dds3GetRandA((effRandState_t *)effSharedRandomState) % life;
                        f32 gravityDisplacement;
                        ageF = (f32)birthAge;
                        gravityDisplacement = gravity * ageF * ageF * 0.5f;
                        particle->age = birthAge;
                        particle->radius += particle->radiusStep * ageF;
                        particle->verticalOffset -= particle->drag * ageF;
                        particle->verticalOffset += gravityDisplacement;
                    } else {
                        particle->age = 0;
                    }
                    spawnRemaining--;
                }
            } else if ((u32)age >= life) {
                colors[0] = 0;
                particle->age = respawn ? -1 : -2;
            } else {
                ageF = (f32)age;
                {
                    f32 angularStep = particle->spin + spinAccel * ageF * 0.5f;
                    f32 angle = angularStep * ageF;
                    f32 oldHeight;
                    f32 radius;
                    f32 currentY;
                    f32 flare;
                    f32 phaseOffset;
                    f32 center[4];
                    f32 direction[4];
                    f32 width;
                    f32 extentStep;
                    f32 cosine;
                    f32 sine;
                    u32 segment;

                    angle += particle->spinPhase;
                    oldHeight = particle->height;
                    radius = particle->radius;
                    currentY = particle->verticalOffset;
                    flare = particle->flare;
                    phaseOffset = ageF / (f32)life - flareCenter;
                    radius += phaseOffset * phaseOffset * flare;
                    cosine = sdfCos(angle);
                    sine = sdfSin(angle);
                    center[0] = cosine * radius;
                    center[1] = currentY;
                    center[2] = sine * radius;
                    SDF_VU0_LOADVECTOR_B(center);
                    VU0_MOVE_VF_EXTENDED(vf12, vf11);

                    currentY -= particle->drag;
                    currentY += gravity * ageF;
                    radius = particle->radius + particle->radiusStep;
                    {
                        f32 nextPhaseOffset = (ageF + 1.0f) / (f32)life - flareCenter;
                        radius += nextPhaseOffset * nextPhaseOffset * flare;
                    }

                    center[0] = cosine * radius;
                    center[1] = currentY;
                    center[2] = sine * radius;
                    SDF_VU0_LOADVECTOR(center);
                    SDF_VU0_SUBVECTOR_EXTENDED();
                    VU0_NORMALIZE_VF10_EXTENDED();
                    SDF_VU0_STOREVECTOR(direction);

                    width = particle->width * 0.25f;
                    extentStep = particle->extent / segmentCountAsFloat;
                    particle->verticalOffset = currentY;
                    particle->radius += particle->radiusStep;
                    center[0] = cosine;
                    center[1] = 0.0f;
                    center[2] = sine;
                    SDF_VU0_LOADVECTOR_B(center);
                    VU0_CROSS_XYZ_EXTENDED(vf10, vf10, vf11);
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED();

                    for (segment = 0; segment < segmentCount; segment++) {
                        f32 fraction;
                        f32 pointY;
                        VU0_MOVE_VF_EXTENDED(vf10, vf12);
                        VU0_ADD_EXTENDED(vf10, vf10, vf11);
                        SDF_VU0_STOREVECTOR(vertices + 4);
                        VU0_ADD_EXTENDED(vf10, vf10, vf11);
                        SDF_VU0_STOREVECTOR(vertices);
                        VU0_MOVE_VF_EXTENDED(vf10, vf12);
                        SDF_VU0_SUBVECTOR_EXTENDED();
                        SDF_VU0_STOREVECTOR(vertices + 8);
                        SDF_VU0_SUBVECTOR_EXTENDED();
                        SDF_VU0_STOREVECTOR(vertices + 12);
                        vertices += 16;
                        fraction = (f32)segment / segmentCountAsFloat;
                        pointY = columnHeight * fraction + oldHeight;
                        heightRows[1] = pointY;
                        heightRows[3] = pointY;
                        heightRows[5] = pointY;
                        heightRows[7] = pointY;
                        heightRows += 8;

                        SDF_VU0_LOADVECTOR(direction);
                        VU0_SCALE_VF(vf10, extentStep);
                        VU0_ADD_EXTENDED(vf12, vf12, vf10);
                    }

                    *colors++ = effColorAnimation(&common->header.emitterColorTrack,
                                                           &common->header.emitterAlphaTrack, age, life);
                    particle->age = (s32)((u32)particle->age + 1U);
                    particle->height += rise;
                }
                continue;
            }
        }
        colors++;
        vertices += vertexStride * 4;
        heightRows += vertexStride * 2;
    }
}

void effBillBlendCellColorAndUpdateTransform(billCellDrawWork_t *work) {
    effBillColumnConfig_t *config = (effBillColumnConfig_t *)work->config;
    u32 limit = work->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = work->instances;
    effRibbonWork_t *out = (effRibbonWork_t *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (effMatrix4_t *)mtx);
}

void effResetParticleBillFrameCounters(effClassWork_t *work) {
    u32 index = 0;
    effFrameState_t *state = work->resource;
    effBillEmitterCommon_t *common = work->payload;
    u32 count = common->header.timed.count;
    effBillParticleEntry_t *entry = (effBillParticleEntry_t *)state->entries;
    u32 *frameCounters = ((effFrameAsset_t *)state->asset)->frameStorage;

    if (count != 0) {
        do {
            index++;
            *frameCounters++ = 0;
            entry->age = -1;
            entry++;
        } while (index < count);
    }
}

u8 *effAllocateCompactRingFadeEntries(effBillSpiralConfig_t *config) {
    struct sdfMemHandle_t *base = sdfMemAllocDirect(config->common.header.timed.count * 0x2C + 0xC);
    effFrameState_t *node = (effFrameState_t *)sdfMemLock(base);
    u32 count = config->common.header.segments;
    u8 *entries = (u8 *)(node + 1);

    node->entries = entries;
    node->allocation = base;
    if (count < 3) {
        config->common.header.segments = 3;
    }
    return (u8 *)node;
}

void effFillCompactRingFadeGradient(u8 *node, effBillEmitterCommon_t *config) {
    effBillEmitterCommon_t *cfg = config;
    effRibbonWork_t *table;
    u32 *colors;
    f32 *radii;
    u32 *colorsStart;
    f32 *radiiStart;
    u32 layers;
    s32 segments;
    s32 fadeInEnd;
    s32 fadeOutStart;
    u32 entries;
    u32 words;
    f32 radius;
    f32 fadeIn;
    f32 fadeOut;
    f32 fade;
    u32 alpha;
    u32 color;
    u32 i;

    layers = cfg->header.timed.count;
    if (layers == 0) {
        return;
    }
    segments = cfg->header.segments;
    entries = segments + 1;
    words = entries * 4;
    table = (effRibbonWork_t *)((effFrameState_t *)node)->asset;
    colors = (u32 *)table->extra;
    radii = (f32 *)table->uvs;
    colorsStart = colors;
    radiiStart = radii;
    fadeOut = cfg->header.fadeOut;
    fadeIn = cfg->header.fadeIn;
    fadeInEnd = (s32)(fadeIn * (f32)segments);
    fadeOutStart = (s32)(fadeOut * (f32)segments);
    radius = cfg->rowRadius / 3.0f;
    for (i = 0; i < entries; i++) {
        if (i < fadeInEnd) {
            fade = (f32)i / (f32)fadeInEnd;
        } else {
            fade = 1.0f;
            if (fadeOutStart < i) {
                fade = (f32)(segments - i) / (f32)(segments - fadeOutStart);
            }
        }
        alpha = (u32)(fade * 128.0f);
        color = (alpha << 24) | 0x808080;
        colors[0] = 0x808080;
        colors[1] = color;
        colors[2] = color;
        colors[3] = 0x808080;
        radii[0] = 0.0f;
        radii[2] = radius;
        radii[4] = radius * 2.0f;
        radii[6] = radius * 3.0f;
        colors += 4;
        radii += 8;
    }
    for (i = 1; i < layers; i++) {
        memcpy(colors, colorsStart, words * 4);
        colors += words;
        memcpy(radii, radiiStart, words * 8);
        radii += words * 2;
    }
}

u8 *effCreateCompactRingFadeTable(effBillSpiralConfig_t *config, u32 resource) {
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((effFrameState_t *)node)->asset = (u8 *)effCreateRibbonWithSharedResource(config->common.header.timed.count, config->common.header.segments, (struct sdfTextureFileHeader_t *)(u32)resource);
    effFillCompactRingFadeGradient(node, &config->common);
    return node;
}

u8 *effCloneBillboardFrameAsset(u8 *work) {
    effBillSpiralConfig_t *config = ((effClassWork_t *)work)->payload;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u8 *node = effAllocateCompactRingFadeEntries(config);

    ((effFrameState_t *)node)->asset = (u8 *)effCloneRibbonWithSharedResource((effRibbonWork_t *)((effFrameState_t *)state)->asset);
    effFillCompactRingFadeGradient(node, &config->common);
    return node;
}

void effReleaseCompactRingFadeTable(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effSharedAssetReferenceRelease((effRibbonWork_t *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

/* vu0 routine: native matrix, cross/normalize and ribbon extrusion. */
void effUpdateBillSpiral(effClassWork_t *work) {
    effBillSpiralConfig_t *config = (effBillSpiralConfig_t *)work->payload;
    effFrameState_t *state;
    effRibbonWork_t *asset;
    effSpiralParticle_t *particle;
    f32 pos[4];
    u32 *colors;
    u32 i;
    u32 count;
    u32 stride;
    u32 segments;
    s32 burst;
    u32 spawn;
    u8 respawn;
    u32 life;
    f32 (*positions)[4];
    f32 *heights;
    u32 j;
    s32 timer;
    f32 segmentCount;
    f32 spinAccel;
    f32 heightScale;
    f32 rise;
    f32 radius;
    f32 width;
    f32 angle;
    f32 step;
    f32 height;
    f32 start;
    f32 time;

    if (config->common.header.timed.time.progress < work->frame && config->common.header.timed.time.progress != 0) {
        return;
    }
    life = config->common.header.life;
    count = config->common.header.timed.count;
    if (life == 0) {
        return;
    }
    if (config->common.burst != 0 && work->frame == 0) {
        burst = 1;
        spawn = count;
    } else {
        burst = 0;
        spawn = config->common.header.spawnRate;
    }
    segments = config->common.header.segments + 1;
    state = (effFrameState_t *)work->resource;
    /* The creator stores the ribbon address in this actual 12-byte header. */
    asset = (effRibbonWork_t *)state->asset;
    particle = (effSpiralParticle_t *)state->entries;
    respawn = config->common.header.respawn;
    /* Config reads before the ribbon's buffer loads: in retail the spin acceleration load (+0xD4) precedes the
     * asset+0x1C positions load (cc1probe --sched, sched1 equal-priority ties decide@luid), which keeps the
     * positions spill store behind it in sched2. */
    spinAccel = config->spinAccel;
    positions = (f32 (*)[4])asset->positions;
    heights = (f32 *)asset->uvs;
    colors = asset->colors;
    stride = asset->rowStride;
    segmentCount = segments;
    heightScale = config->common.height;
    rise = config->common.rise;
    for (i = 0; i < count; i++, particle++) {
        timer = particle->timer;
        if (timer != -2) {
            if (timer == -1) {
                if (spawn != 0) {
                    f32 radiusDelta;
                    start = config->common.radiusStart * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusStartJitter + (1.0f - config->common.radiusStartJitter));
                    radiusDelta = (config->common.radiusEnd * (dds3GetRandFloatA(effSharedRandomState) * config->common.radiusEndJitter + (1.0f - config->common.radiusEndJitter)) - start) / (f32)life;
                    particle->radius = start;
                    particle->radiusStep = radiusDelta;
                    particle->tiltX = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                    particle->tiltY = (dds3GetRandFloatA(effSharedRandomState) - 0.5f) * 2.0f * 3.14159265f;
                    particle->phase = dds3GetRandFloatA(effSharedRandomState) * 6.2831853f;
                    particle->spin = config->spin * (dds3GetRandFloatA(effSharedRandomState) * config->spinJitter + (1.0f - config->spinJitter));
                    particle->width = config->common.width * (dds3GetRandFloatA(effSharedRandomState) * config->common.widthJitter + (1.0f - config->common.widthJitter));
                    particle->sweep = config->common.extent * (dds3GetRandFloatA(effSharedRandomState) * config->common.extentJitter + (1.0f - config->common.extentJitter));
                    particle->height = config->common.height * (dds3GetRandFloatA(effSharedRandomState) * config->common.heightJitter + (1.0f - config->common.heightJitter));
                    *colors = 0;
                    if (burst != 0) {
                        particle->timer = dds3GetRandA(effSharedRandomState) % life;
                        time = (f32)particle->timer;
                        particle->radius += particle->radiusStep * time;
                    } else {
                        particle->timer = 0;
                    }
                    spawn--;
                }
            } else if ((u32)timer >= life) {
                *colors = 0;
                particle->timer = respawn ? -1 : -2;
            } else {
                particle->radius += particle->radiusStep;
                radius = particle->radius;
                time = (f32)timer;
                angle = (particle->spin + spinAccel * time * 0.5f) * time;
                angle += particle->phase;
                width = particle->width * 0.25f;
                if (particle->spin >= 0.0f) {
                    step = particle->sweep / segmentCount;
                } else {
                    step = -particle->sweep / segmentCount;
                }
                height = particle->height;
                SetRotateMatrixX(particle->tiltX);
                SetRotateMatrixY_B(particle->tiltY);
                MultMatrixRevNoArg();
                pos[0] = sdfCos(angle);
                pos[1] = 0.0f;
                pos[2] = sdfSin(angle);
                for (j = 0; j < segments; j++) {
                    f32 rowHeight;
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    VU0_SCALE_VF(vf10, radius);
                    SDF_VU0_COPYVECTOR_A_TO_C();
                    angle += step;
                    pos[0] = sdfCos(angle);
                    pos[1] = 0.0f;
                    pos[2] = sdfSin(angle);
                    SDF_VU0_LOADVECTOR(pos);
                    SDF_VU0_APPLYMATRIX_XYZ();
                    VU0_SCALE_VF(vf10, radius);
                    SDF_VU0_COPYVECTOR_C_TO_B();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    VU0_SCALE_VF(vf10, width);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[1]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(positions[0]);
                    SDF_VU0_COPYVECTOR_C_TO_A();
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[2]);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_STOREVECTOR(positions[3]);
                    positions += 4;
                    rowHeight = heightScale * ((f32)j / segmentCount) + height;
                    heights[1] = rowHeight;
                    heights[3] = rowHeight;
                    heights[5] = rowHeight;
                    heights[7] = rowHeight;
                    heights += 8;
                }
                *colors++ = effColorAnimation(&config->common.header.emitterColorTrack, &config->common.header.emitterAlphaTrack, timer, life);
                particle->height += rise;
                particle->timer++;
                continue;
            }
        }
        colors++;
        positions += stride;
        heights += stride * 2;
    }
}

void effUpdateCompactRingDrawColorAndTransform(u8 *work) {
    effBillSpiralConfig_t *config = (effBillSpiralConfig_t *)((billCellDrawWork_t *)work)->config;
    u32 limit = ((billCellDrawWork_t *)work)->frameLimit;
    u32 progress = config->common.header.timed.time.progress;
    u32 *list = ((billCellDrawWork_t *)work)->instances;
    effRibbonWork_t *out = (effRibbonWork_t *)(u32)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&config->common.header.timed.colorTrack, &config->common.header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(((billCellDrawWork_t *)work)->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    out->color = packed;
    out->surfaceIndex = config->common.header.timed.alphaTrack.surfaceIndex;
    out->drawMode = config->common.drawMode;
    SDF_VU0_LOADVECTOR(work + 0x10);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP_CLOBBER(((billCellDrawWork_t *)work)->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawRibbon(out, (effMatrix4_t *)mtx);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf368);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallMemAllocDirect2);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallMemcpy4);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effFunc16);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effBillboardMotionResourceInitialize);

void effFunc17(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effSharedAssetReferenceRelease((effRibbonWork_t *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallGetRandFloatA2);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallColorAnimation2);

effClassWork_t *effAllocateBlock(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effBlockResourceOperations[kind].payloadSize;
    effClassWork_t *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF(vf0, effect);
    VU0_STORE_VF(vf0, (u8 *)effect + 0x10);
    memcpy(effect->payload, source, size);
    return effect;
}

effClassWork_t *effCreateResourceInstanceB(u16 kind, void *source, u32 option) {
    effClassWork_t *effect = effAllocateBlock(kind, source);
    effect->resource = (void *)effBlockResourceOperations[kind].createResource(source, option);
    effBlockResourceOperations[kind].initialize(effect);
    return effect;
}

effClassWork_t *effCreateBlockResourceFromFile(fileJobPayload_t *request) {
    u32 *secondary = fileResolveSecondaryBuffer(request);
    void *source;
    switch (request->primary.selector) {
    case 1:
        break;
    case 4:
        secondary = NULL;
        break;
    }
    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceB(request->option, source, (u32)secondary);
}

void effDestroyBlockResourceWork(effClassWork_t *work) {
    effBlockResourceOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

effClassWork_t *effDuplicateActiveResourceB(effClassWork_t *work) {
    effClassWork_t *effect = effAllocateBlock(work->kind, work->payload);
    void *active = (void *)effBlockResourceOperations[work->kind].cloneResource(work);
    s32 kind = work->kind;
    effect->resource = active;
    effBlockResourceOperations[kind].initialize(effect);
    return effect;
}

void effResetBlockResourceFrame(effClassWork_t *work) {
    effBlockResourceOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceBlockResourceFrame(effClassWork_t *work) {
    effBlockResourceOperations[work->kind].update(work);
    work->frame = work->frame + 1;
}

void effDrawBlockResourceWork(effClassWork_t *work) {
    effBlockResourceOperations[work->kind].draw(work);
}

void effCallSRAnimation11(effClassWork_t *work) {
    if ((effSRAnimation() & 1) == 0) {
        effAdvanceBlockResourceFrame(work);
    }
    effDrawBlockResourceWork(work);
}

void effSetBlockResourcePosition(effClassWork_t *work, const f32 position[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.position, position);
}

void effSetBlockResourceRotation(effClassWork_t *work, const f32 rotation[4]) {
    SDF_VU0_COPYVECTOR(work->vectors.orientation, rotation);
}

void effSetBlockResourceColor(effClassWork_t *work, u32 color) {
    work->color = color;
}

void effSetBlockResourceScale(effClassWork_t *work, f32 scale) {
    work->scale = scale;
}

effRibbonWork_t *effCreateRibbonWork(u32 count, u32 repeat) {
    u32 rowStride = repeat * 4 + 4;
    u32 size = (rowStride * 0x1C + 4) * count;
    u32 cells = rowStride * count;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(size + 0x34);
    u8 *p = (u8 *)sdfMemLock((struct sdfMemHandle_t *)((u32)allocation));
    effRibbonWork_t *work = (effRibbonWork_t *)(p + size);
    u32 i;

    work->positions = p;
    p += cells * 16;
    work->uvs = p;
    p += cells * 8;
    work->extra = p;
    p += cells * 4;
    work->surfaceIndex = 2;
    work->color = 0x80808080;
    work->rowStride = rowStride;
    work->repeat = repeat;
    work->allocation = allocation;
    work->colors = (u32 *)p;
    work->count = count;
    work->drawMode = 0;
    for (i = 0; i < count; i++) {
        ((u32 *)p)[i] = 0x80808080;
    }
    work->handle = sdfMatNew();
    sdfMatSetConstant(work->handle, 1.0f);
    memset(&effDrawRibbonWork, 0, sizeof(sdf3DPrimPolyParm_t));
    effDrawRibbonWork.attr = 0x4000;
    return work;
}

effRibbonWork_t *effCreateRibbonWithSharedResource(u32 count, u32 repeat, struct sdfTextureFileHeader_t *resource) {
    effRibbonWork_t *node = effCreateRibbonWork(count, repeat);

    if (resource == 0) {
        s32 references = effSharedRibbonReferenceCount;
        node->resource = NULL;
        if (references == 0) {
            effSharedAssetReferenceReleaseData = (u32)effCloneSharedReferenceWithValue((struct sdfTextureFileHeader_t *)effWindTextureHandle, 0x300);
            references = effSharedRibbonReferenceCount;
        }
        references++;
        effSharedRibbonReferenceCount = references;
    } else {
        node->resource = effCreateSharedTextureReference(resource);
    }
    return node;
}

void effSharedAssetReferenceRelease(effRibbonWork_t *work) {
    if (work->resource == NULL) {
        effSharedRibbonReferenceCount = effSharedRibbonReferenceCount - 1;
        if (effSharedRibbonReferenceCount == 0) {
            effReleaseSharedReference((effRefObj_t *)effSharedAssetReferenceReleaseData);
            effSharedAssetReferenceReleaseData = 0;
        }
    }
    else {
        effReleaseSharedReference(work->resource);
    }
    sdfMatFreeDelayed(work->handle);
    sdfMemFreeDirect(work->allocation);
}

effRibbonWork_t *effCloneRibbonWithSharedResource(effRibbonWork_t *source) {
    effRibbonWork_t *node = effCreateRibbonWork(source->count, source->repeat);
    effRefObj_t *texture = source->resource;

    if (texture != NULL) {
        node->resource = effRetainSharedReference(texture);
    } else {
        effSharedRibbonReferenceCount++;
        node->resource = NULL;
    }
    return node;
}

void effDrawRibbon(effRibbonWork_t *work, const effMatrix4_t *matrix) {
    sdfGrOt_t *surface;
    void *list;
    effGsPacket_t *packet;
    u32 segment;
    u32 count;
    s32 rowStride;
    s32 remaining;
    u32 *colors;
    u8 *positions;
    u8 *uvs;
    u8 *extra;
    u32 color;
    u32 segmentColor;
    f32 baseVector[4];
    sdfTexHandle_t *texture;

    count = work->count;
    if (count == 0) {
        return;
    }
    color = work->color;
    if ((color & 0xFF000000) == 0) {
        return;
    }
    surface = effDrawRibbonData[work->surfaceIndex];
    eeUnpackRGBAWordFloat(color, 1.0f / 128.0f);
    SDF_VU0_STOREVECTOR(baseVector);
    list = (void *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    if (matrix == NULL) {
        SDF_VU0_LOADIDENTITY();
    } else {
        VU0_LOAD_MATRIX(matrix);
    }
    sdf3DPrimLinkTm(list, 0);
    if (work->resource == NULL) {
        texture = sdfGetOrCreateTextureForReference(surface, (effRefObj_t *)effSharedAssetReferenceReleaseData);
    } else {
        texture = sdfGetOrCreateTextureForReference(surface, work->resource);
    }
    sdfMatSetTex(work->handle, texture);
    sdf3DPrimLinkMat(list, work->handle, 0);
    if (work->drawMode == 0) {
        packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x31801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    positions = work->positions;
    uvs = work->uvs;
    extra = work->extra;
    colors = work->colors;
    rowStride = work->rowStride;
    segment = 0;
    if (count != 0) {
        s32 extraStep;
        s32 uvStep;
        s32 positionStep;

        positionStep = rowStride * 16;
        uvStep = rowStride * 8;
        extraStep = rowStride * 4;

        do {
            segmentColor = *colors;
            if (segmentColor & 0xFF000000) {
                remaining = rowStride;
                effDrawRibbonWork.vertexColor = (u32 *)extra;
                effDrawRibbonWork.vertex = (u128 *)positions;
                effDrawRibbonWork.uv = (f32 *)uvs;
                unpackNormalizedColor(segmentColor);
                SDF_VU0_LOADVECTOR_B(baseVector);
                SDF_VU0_MULVECTOR();
                effDrawRibbonWork.color = packNormalizedColor();
                effDrawRibbonWork.faceCount = 12;
                effDrawRibbonWork.pointCount = 12;
                effDrawRibbonWork.faceIndex = effData7;
                while (remaining >= 12) {
                    remaining -= 8;
                    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawRibbonWork));
                    effDrawRibbonWork.vertex += 8;
                    effDrawRibbonWork.vertexColor += 8;
                    effDrawRibbonWork.uv += 8;
                }
                if (remaining >= 8) {
                    effDrawRibbonWork.faceCount = 6;
                    effDrawRibbonWork.pointCount = remaining;
                    sdfDrawTagLink(list, effPkt3DPrimMakePoly(&effDrawRibbonWork));
                }
            }
            positions += positionStep;
            colors++;
            segment++;
            uvs += uvStep;
            extra += extraStep;
        } while (segment < count);
    }
    if (work->drawMode == 0) {
        packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
        packet->dmaTag = 2;
        packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
        packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
        packet->registerList = 0xE;
        packet->registerValue = 0x51801;
        packet->registerAddress = 0x47;
        sdfDrawTagLink(list, (u32)packet);
    }
    surface->regFunc(surface, (sdfDrawTag_t *)list);
}

void effLoadWindTexture(void) {
    effLoadWindTextureData = sdfDevLoadFileEx("/effect/wind00.tmx", &effWindTextureHandle, 0);
}

u32 effGetWindTextureHandle(void) {
    return effWindTextureHandle;
}

void effResetAnimationFrameEntries(u8 *work) {
    u32 index = 0;
    u8 *state = (u8 *)((effClassWork_t *)work)->resource;
    u32 count = ((effBillFlameConfig_t *)((effClassWork_t *)work)->payload)->header.timed.count;
    effBillEmitterEntry_t *entry = (effBillEmitterEntry_t *)((effFrameState_t *)state)->entries;
    u32 *flags = ((effStripWork_t *)((effFrameState_t *)state)->asset)->colors;

    if (count != 0) {
        do {
            index++;
            *flags++ = 0;
            entry->timer = -1;
            entry++;
        } while (index < count);
    }
}

u32 *effAllocateAnimationBuffer(effBillFlameConfig_t *work) {
    u32 age;
    u32 *buffer;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(work->header.timed.count * sizeof(effBillEmitterEntry_t) + 0xC);

    buffer = (u32 *)sdfMemLock(allocation);
    age = work->header.segments;

    buffer[0] = (u32)(buffer + 3);
    buffer[2] = (u32)allocation;
    if (age < 3) {
        work->header.segments = 3;
    }
    return buffer;
}

void effFillFadeColorRows(u8 *work, u8 *config) {
    effBillFlameConfig_t *cfg = (effBillFlameConfig_t *)config;
    u32 rows = cfg->header.timed.count;
    u32 i;

    if (rows != 0) {
        s32 width = cfg->header.segments;
        f32 fw = width;
        s32 fadeInEnd = cfg->header.fadeIn * fw;
        s32 fadeOutStart = cfg->header.fadeOut * fw;
        u32 cols = width + 1;
        u32 stride = cols * 4;
        u32 *color = ((effFrameAsset_t *)((effFrameState_t *)work)->asset)->colorRows;
        u32 *first = color;

        for (i = 0; i < cols; i++) {
            f32 t;
            u32 alpha;

            if (i < fadeInEnd) {
                t = (f32)i / (f32)fadeInEnd;
            } else if (fadeOutStart < i) {
                t = (f32)(width - i) / (f32)(width - fadeOutStart);
            } else {
                t = 1.0f;
            }
            alpha = (u32)(t * 128.0f) << 24;
            color[0] = 0x808080;
            color[1] = alpha | 0x808080;
            color[2] = alpha | 0x808080;
            color[3] = 0x808080;
            color += 4;
        }
        for (i = 1; i < rows; i++) {
            memcpy(color, first, stride * 4);
            color += stride;
        }
    }
}

u32 *effPrepareTextureAnimation(effBillFlameConfig_t *work) {
    u32 *buffer = effAllocateAnimationBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(work->header.timed.count, work->header.segments);
    effFillFadeColorRows(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedTextureAnimation(u8 *work) {
    u8 *anim = ((effClassWork_t *)work)->payload;
    u8 *owner = (u8 *)((effClassWork_t *)work)->resource;
    u32 *buffer = effAllocateAnimationBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((effFrameState_t *)owner)->asset);
    effFillFadeColorRows(buffer, anim);
    return buffer;
}

void effReleaseTextureAnimationWork(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effReleaseScalyStripResources((u8 *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effUpdateBillFlame);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateTintedScaledMesh(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillFlameConfig_t *)config)->header.timed.time.progress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    u32 packed;
    u32 unit;
    u32 second;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillFlameConfig_t *)config)->header.timed.colorTrack, &((effBillFlameConfig_t *)config)->header.timed.alphaTrack, limit, progress);
    unit = 0x3C000000;
    eeUnpackRGBAWord(work->baseColor, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    eeUnpackRGBAWord(second, unit);
    SDF_VU0_MULVECTOR();
    packed = eePackRGBAWord(128.0f);
    ((effStripWork_t *)out)->color = packed;
    ((effStripWork_t *)out)->type = ((effBillFlameConfig_t *)config)->header.timed.alphaTrack.surfaceIndex;
    ((effStripWork_t *)out)->flag = ((effBillFlameConfig_t *)config)->meshMode;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(work->scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawScalyTexturedStrip(out, mtx);
}

void effInitializeAnimationPositions(effClassWork_t *work) {
    effAnimationState_t *state = (effAnimationState_t *)work->resource;
    bhvrHandle_t *record = state->record;
    float *positions = state->positions;
    u32 count = record->count;
    u32 i = 0;

    fileClearRecordReferences(record);
    for (; i < count; i++) {
        positions[0] = dds3GetRandFloatA(effSharedRandomState);
        positions[1] = dds3GetRandFloatA(effSharedRandomState);
        positions += 2;
    }
}

u32 effClampSlotCount(fileKeyBlock_t *grid) {
    u32 n;
    s32 w = grid->emissionDuration;
    /* Retail 002AF560 uses signed capacity factors before the unsigned cap. */
    n = (w ? w : (s32)grid->spawnRate) * (w ? (s32)grid->spawnRate : grid->length);
    return n > 200 ? 200 : n;
}

effAnimationState_t *effCreateAnimationState(u32 unused, u32 count) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(count * 8 + 0x10);
    effAnimationState_t *state = (effAnimationState_t *)sdfMemLock(allocation);

    state->allocation = allocation;
    state->positions = (f32 *)(state + 1);
    state->record = 0;
    state->textureHandle = effRetainScalyTextureReference();
    return state;
}

effAnimationState_t *effActivateAnimationState(s32 work) {
    effAnimationState_t *owner = (effAnimationState_t *)((effClassWork_t *)work)->resource;
    bhvrHandle_t *resource = owner->record;
    effAnimationState_t *state = effCreateAnimationState((u32)((effClassWork_t *)work)->payload, resource->count);

    resource = owner->record;
    state->record = fileAllocateGridRecordSlots(resource->type, resource->count,
                               resource->data1);
    return state;
}

void effReleaseAnimationFrameResources(u8 *work) {
    effAnimationState_t *state = (effAnimationState_t *)((effClassWork_t *)work)->resource;

    effReleaseScalyTextureReference(state->textureHandle);
    if (state->record != 0) {
        effPCPHelperBEDGetModel(state->record);
    }
    sdfMemFreeDirect(state->allocation);
}

void effSynchronizeFileTransform(u8 *work) {
    effAnimationState_t *state = (effAnimationState_t *)((effClassWork_t *)work)->resource;
    bhvrHandle_t *record = state->record;

    if (record != 0) {
        mnuRecordSetVector(record, work);
        fileSetRecordSecondVector(state->record, work + 0x10);
        dds3DispatchIndexedCallback((u16 *)state->record, ((effClassWork_t *)work)->scale);
        fileAcquireRecord(state->record);
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallToMatrix2);

effAnimationState_t *effCreatePrimarySlotAnimationState(u8 *work) {
    fileKeyBlock_t *mapping = (fileKeyBlock_t *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    effAnimationState_t *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(1, count, mapping);
    return state;
}

effAnimationState_t *effCreateAlternateSlotAnimationState(u8 *work) {
    fileKeyBlock_t *mapping = (fileKeyBlock_t *)(work + 0x3C);
    u32 count = effClampSlotCount(mapping);
    effAnimationState_t *state = effCreateAnimationState((u32)work, count);

    state->record = fileAllocateGridRecordSlots(3, count, mapping);
    return state;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf371);

u32 *effAllocateQuantizedBuffer(effBillQuantizedConfig_t *work) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0xC);
    u32 *buffer = (u32 *)sdfMemLock(allocation);
    u32 count = work->samples.quantizedSamples;

    buffer[2] = (u32)allocation;
    if (count < 4) {
        work->samples.quantizedSamples = 4;
        count = 4;
    }
    buffer[0] = count >> 2;
    if ((work->samples.quantizedSamples & 3) != 0) {
        buffer[0] = (count >> 2) + 1;
    }
    return buffer;
}

void effBuildQuantizedTexturedStrip(u32 *buffer, effBillQuantizedConfig_t *config) {
    f32 radii[4];
    f32 heights[4];
    s32 segment;
    s32 segmentCount;
    f32 rowStep;
    f32 angleStep;
    f32 rowScaleB;
    f32 rowScaleA;
    s32 fadeOutLimit;
    s32 vertexCount;
    u32 *colors;
    s32 rowCount;
    s32 fadeInLimit;
    effStripWork_t *strip;
    f32 *positions;
    u32 *vertexColors;
    f32 *uvA;
    f32 *uvB;
    f32 sampleStep;
    f32 radius;
    f32 vertexScaleB;
    f32 vertexScaleA;
    f32 segmentAngle;
    f32 angle;
    f32 rowAngle;
    s32 row;
    s32 sample;
    f32 cosine;
    f32 sine;
    f32 fraction;
    f32 fractionStep;
    f32 rowFraction;
    f32 alpha;

    segmentCount = (s32)buffer[0];
    vertexCount = segmentCount * 4;
    angle = config->angularSpanDegrees * 0.017453292f;
    angleStep = angle / segmentCount;
    rowCount = config->samples.signedRows;
    rowStep = 6.2831852f / rowCount;
    rowCount++;
    strip = (effStripWork_t *)buffer[1];
    vertexColors = (u32 *)strip->extra;
    uvA = (f32 *)strip->uvsA;
    uvB = (f32 *)strip->uvsB;
    colors = strip->colors;
    positions = (f32 *)strip->positions;
    sampleStep = angleStep / 3.0f;
    radius = config->radius;
    rowScaleB = config->fadeInEnd;
    vertexScaleB = config->fadeOutStart;
    rowScaleA = config->uvARowScale;
    vertexScaleA = config->uvBVertexScale;
    fadeInLimit = config->alphaFadeInFraction * vertexCount;
    fadeOutLimit = config->alphaFadeOutFraction * vertexCount;
    segmentAngle = 0.0f;

    for (segment = 0; segment < segmentCount; segment++) {
        angle = segmentAngle;
        for (sample = 0; sample < 4; sample++) {
            radii[sample] = radius * sdfSin(angle);
            heights[sample] = -radius * sdfCos(angle);
            angle += sampleStep;
        }
        segmentAngle += angleStep;
        rowAngle = 0.0f;
        /* Retail keeps this unused sample. */
        sdfSin(segmentAngle);
        for (row = 0; row < rowCount; row++) {
            f32 *position;
            f32 *texA;
            f32 *texB;
            cosine = sdfCos(rowAngle);
            sine = sdfSin(rowAngle);
            rowFraction = (f32)row / rowCount;
            fraction = (f32)segment / segmentCount;
            fractionStep = (1.0f / segmentCount) / 3.0f;
            texB = uvB;
            texA = uvA;
            position = positions;
            for (sample = 0; sample < 4; sample++) {
                s32 index;
                position[0] = cosine * radii[sample];
                position[1] = heights[sample];
                position[2] = sine * radii[sample];
                position += 4;
                texA[1] = fraction * vertexScaleA;
                texA[0] = rowFraction * rowScaleA;
                texA += 2;
                texB[1] = fraction * vertexScaleB;
                texB[0] = rowFraction * rowScaleB;
                texB += 2;
                fraction += fractionStep;
                index = segment * 4 + sample;
                if (index < fadeInLimit) {
                    alpha = (f32)index / fadeInLimit;
                } else if (index > fadeOutLimit) {
                    alpha = (f32)(vertexCount - index) / (vertexCount - fadeOutLimit);
                } else {
                    alpha = 1.0f;
                }
                vertexColors[sample] = 0x00808080 | ((u32)(alpha * 128.0f) << 24);
            }
            rowAngle += rowStep;
            positions += 16;
            vertexColors += 4;
            uvA += 8;
            uvB += 8;
        }
        *colors++ = 0x80808080;
    }
}

u32 *effPrepareQuantizedTexture(effBillQuantizedConfig_t *work) {
    u32 *buffer = effAllocateQuantizedBuffer(work);

    buffer[1] = effCreateTexturedStripWithSharedTexture(buffer[0], work->samples.quantizedSamples);
    effBuildQuantizedTexturedStrip(buffer, work);
    return buffer;
}

u32 *effPrepareOwnedQuantizedTexture(u8 *work) {
    effBillQuantizedConfig_t *anim = (effBillQuantizedConfig_t *)((effClassWork_t *)work)->payload;
    u8 *owner = (u8 *)((effClassWork_t *)work)->resource;
    u32 *buffer = effAllocateQuantizedBuffer(anim);

    buffer[1] = effAllocateStripFromWorkAndRetainTexture(((effFrameState_t *)owner)->asset);
    effBuildQuantizedTexturedStrip(buffer, anim);
    return buffer;
}

void effReleaseBillboardFrameAsset(s32 work) {
    effFrameState_t *state;

    state = (effFrameState_t *)((effClassWork_t *)work)->resource;
    effReleaseScalyStripResources((u8 *)state->asset);
    sdfMemFreeDirect(state->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effOffsetNodeRowsVU);

/* vu0 routine: fade-blended colour and scaled transform of a mesh draw record */
void effUpdateFadedMeshTransform(billCellDrawWork_t *work) {
    u8 *config = work->config;
    u32 limit = work->frameLimit;
    u32 progress = ((effBillQuantizedConfig_t *)config)->drawProgress;
    u32 *list = work->instances;
    u8 *out = (u8 *)list[1];
    u128 mtx[4];
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    u32 second;
    f32 scale;

    if (progress < limit && progress != 0) {
        return;
    }
    second = effColorAnimation(&((effBillQuantizedConfig_t *)config)->colorTrack, &((effBillQuantizedConfig_t *)config)->alphaTrack, limit, progress);
    unit = 0x3C000000;
    color1[0] = work->baseColor;
    EE_MMI_RGBA_UNPACK(color1, unit);
    SDF_VU0_COPYVECTOR_A_TO_B();
    color2[0] = second;
    EE_MMI_RGBA_UNPACK(color2, unit);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    ((effStripWork_t *)out)->color = blended[0];
    ((effStripWork_t *)out)->type = ((effBillQuantizedConfig_t *)config)->alphaTrack.surfaceIndex;
    ((effStripWork_t *)out)->flag = ((effBillQuantizedConfig_t *)config)->meshMode;
    scale = effSampleScalarCurve(&((effBillQuantizedConfig_t *)config)->scaleCurve, limit, progress) * work->scale;
    SDF_VU0_LOADVECTOR(work->transform);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX(mtx);
    effDrawScalyTexturedStrip(out, mtx);
}

effClassWork_t *effAllocateBlockWithModel(u16 kind, void *source) {
    u32 headerSize = 0x40;
    u32 size = effModelBlockOperations[kind].payloadSize;
    effClassWork_t *effect = sdfChipAllocDirect(size + headerSize);
    effect->payload = (u8 *)effect + headerSize;
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->kind = kind;
    effect->frame = 0;
    VU0_STORE_VF($vf0, effect);
    VU0_STORE_VF($vf0, effect->vectors.orientation);
    memcpy(effect->payload, source, size);
    return effect;
}

effClassWork_t *effCreateResourceInstanceC(u16 kind, void *source) {
    effClassWork_t *effect = effAllocateBlockWithModel(kind, source);
    effect->resource = (void *)effModelBlockOperations[kind].createResource(source);
    effModelBlockOperations[kind].initialize(effect);
    return effect;
}

effClassWork_t *effCreateModelBlockFromFile(fileJobPayload_t *request) {
    void *source;

    source = fileResolvePrimaryBuffer(request);
    return effCreateResourceInstanceC(request->option, source);
}

void effDestroyModelBlockWork(effClassWork_t *work) {
    effModelBlockOperations[work->kind].destroyResource(work);
    sdfChipFreeDirect(work);
}

effClassWork_t *effRecreateActiveByClass(effClassWork_t *work) {
    effClassWork_t *effect = effAllocateBlockWithModel(work->kind, work->payload);
    void *resource = (void *)effModelBlockOperations[work->kind].cloneResource(work);
    s32 kind = work->kind;
    effect->resource = resource;
    effModelBlockOperations[kind].initialize(effect);
    return effect;
}

void effResetModelBlockFrame(effClassWork_t *work) {
    effModelBlockOperations[work->kind].initialize(work);
    work->frame = 0;
}

void effAdvanceDispatchCounter(effClassWork_t *work) {
    effModelBlockOperations[work->kind].update(work);
    work->frame++;
}

void effDrawModelBlock(effClassWork_t *work) {
    effModelBlockOperations[work->kind].draw(work);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSRAnimation12);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf372);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf373);

void effSetModelBlockColor(effClassWork_t *work, u32 color) {
    work->color = color;
}

void effSetModelBlockScale(effClassWork_t *work, f32 scale) {
    work->scale = scale;
}

/* Retain and return the lazy shared reference to /effect/scaly00.tmx. */
u32 effRetainScalyTextureReference(void) {
    if (effSharedStripReferenceCount == 0) {
        effSharedScalyStripResource = effCloneSharedReferenceWithValue((struct sdfTextureFileHeader_t *)effScalyTextureHandle, 0x200);
    }
    effSharedStripReferenceCount = effSharedStripReferenceCount + 1;
    return (u32)effSharedScalyStripResource;
}

/* Drop a scaly texture reference; unused is ignored and the last owner releases the clone. */
void effReleaseScalyTextureReference(s32 unused) {
    effSharedStripReferenceCount = effSharedStripReferenceCount - 1;
    if (effSharedStripReferenceCount == 0) {
        effReleaseSharedReference(effSharedScalyStripResource);
        effSharedScalyStripResource = 0;
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effAllocateTexturedStripWork);

/* Create a scaly strip and retain its shared texture reference. */
u32 effCreateTexturedStripWithSharedTexture(u32 count, u32 repeat) {
    u32 strip = effAllocateTexturedStripWork(count, repeat);
    effRetainScalyTextureReference();
    return strip;
}

void effReleaseScalyStripResources(u8 *work) {
    effReleaseScalyTextureReference((s32)effSharedScalyStripResource);
    sdfMatFreeDelayed(((effStripWork_t *)work)->handle);
    sdfMemFreeDirect(((effStripWork_t *)work)->allocation);
}

/* Recreate the source strip's dimensions and retain another shared texture reference. */
u32 effAllocateStripFromWorkAndRetainTexture(u8 *work) {
    u32 strip = effAllocateTexturedStripWork(((effStripWork_t *)work)->count, ((effStripWork_t *)work)->repeat);
    effSharedStripReferenceCount++;
    return strip;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effDrawScalyTexturedStrip);

void effLoadScalyTexture(void) {
    effLoadScalyTextureData = (u32)sdfDevLoadFileEx("/effect/scaly00.tmx", &effScalyTextureHandle, 0);
}

u32 effGetScalyTextureHandle(void) {
    return effScalyTextureHandle;
}

void effSeedParticleSpanParameters(effModelResource_t *work) {
    u32 index = 0;
    effSpanTable_t *table = work->childResource;
    effSpanConfig_t *config = work->source;
    u32 total = table->total;
    u32 per = config->perSpan;
    u32 spans = total / per;
    effSpanRecord_t *record = table->records;
    u32 span;
    effSpanEntry_t *entry;

    if (total % per != 0) {
        spans++;
    }
    if (table->count != 0) {
        do {
            record->pointCount = 0;
            entry = record->entries;
            span = 0;
            if (spans != 0) {
                do {
                    span++;
                    entry->first = dds3GetRandFloatA(effSharedRandomState) * config->firstRand + (1.0f - config->firstRand);
                    entry->second = config->secondBase * (dds3GetRandFloatA(effSharedRandomState) * config->rangeRand + (1.0f - config->rangeRand));
                    entry->referenceAge = 0;
                    entry++;
                } while (span < spans);
            }
            index++;
            record++;
        } while (index < table->count);
    }
}

/* Build the point and reference sets for each model map-position record. */
effSpanTable_t *effCreateParticleSpanTable(effSpanConfig_t *config, mdlObj_t *model) {
    u32 total = model->first->frameLength;
    u32 count = sdfModelGetMarkPosCount(model->inner);
    u32 spans;
    u32 partialSpan;
    struct sdfMemHandle_t *allocation;
    effSpanTable_t *table;
    effSpanRecord_t *record;
    effSpanEntry_t *entries;
    u32 i;
    u32 j;
    u32 triplets;
    u32 *colors;
    u32 middleColor;
    u32 edgeColor;
    effTrackSet_t *tracks;

    model->first->speed = 1.0f;
    mdlSingleMotion(model, 0, 0);
    if (config->perSpan == 0) {
        config->perSpan = 1;
    }
    partialSpan = total % config->perSpan != 0;
    spans = partialSpan + total / config->perSpan;
    if (config->referenceRampDuration == 0) {
        config->referenceRampDuration = 1;
    }
    allocation = sdfMemAllocDirect(sizeof(effSpanTable_t) +
                 count * sizeof(effSpanRecord_t) + count * spans * sizeof(effSpanEntry_t));
    table = (effSpanTable_t *)sdfMemLock(allocation);
    table->allocation = allocation;
    table->records = (effSpanRecord_t *)(table + 1);
    entries = (effSpanEntry_t *)(table->records + count);
    table->total = total;
    table->count = count;
    for (i = 0, record = table->records; i < count; i++, record++) {
        record->pointSet = effCreatePointSet3(total);
        record->pointSet->type = (u32)config->pointAlphaTrack.surfaceIndex;
        record->pointSet->flag = config->pointSetFlag;
        if (config->drawPoints) {
            triplets = record->pointSet->rows / 3;
            colors = (u32 *)record->pointSet->tail;
            middleColor = config->middleColor;
            edgeColor = config->edgeColor;
            for (j = 0; j < triplets; j++, colors += 3) {
                colors[0] = edgeColor;
                colors[1] = middleColor;
                colors[2] = edgeColor;
            }
        }
        record->entries = entries;
        entries += spans;
        if (config->drawReferences) {
            tracks = effCreateTrackSetWithSharedReferences(spans, 0, 0);
            record->references = tracks;
            tracks->type = (u32)config->referenceAlphaTrack.surfaceIndex;
            tracks->flag = config->pointSetFlag;
        } else {
            record->references = 0;
        }
    }
    return table;
}

void effReleaseParticleList(effSpanTable_t *list) {
    u32 i;
    effSpanRecord_t *entry = list->records;

    for (i = 0; i < list->count; i++, entry++) {
        effReleaseModelPointSetAsset(entry->pointSet);
        if (entry->references != 0) {
            effReleaseResourceRefs(entry->references);
        }
    }
    sdfMemFreeDirect(list->allocation);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effUpdateParticleSpanGeometry);

/* vu0 routine: quaternion matrix and SDK RGBA modulation. */
void effDrawParticleSpanPointsAndReferences(effModelResource_t *work) {
    effMatrix4_t matrix;
    u32 color1[4];
    u32 color2[4];
    u32 blended[4];
    effSpanConfig_t *config = work->source;
    effSpanTable_t *table = work->childResource;
    f32 scale = work->scale * config->drawScale;
    u32 count;
    u32 i;
    u32 color;
    effSpanRecord_t *record;

    SDF_VU0_LOADVECTOR(work->orientation);
    ToMatrix();
    SDF_VU0_LOADVECTOR(effOnesVector);
    VU0_SCALAR_OP(scale, "vmulx.xyzw vf10, vf10, vf2x");
    SDF_VU0_SCALE_XYZ();
    SDF_VU0_LOADVECTOR(work->position);
    SDF_VU0_VECTOR_W_SET_ONE();
    SDF_VU0_SET_ROW3();
    VU0_STORE_MATRIX_M(matrix.u.rows[0], matrix.u.rows[1], matrix.u.rows[2], matrix.u.rows[3]);
    count = table->count;

    if (config->drawPoints != 0 && work->updateCount < config->progress) {
        color = effColorAnimation(&config->pointColorTrack, &config->pointAlphaTrack, work->updateCount, config->progress);
        color1[0] = work->color;
        EE_MMI_RGBA_UNPACK(color1, 0.0078125f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        color2[0] = color;
        EE_MMI_RGBA_UNPACK(color2, 0.0078125f);
        SDF_VU0_MULVECTOR();
        EE_MMI_RGBA_PACK_F128(blended[0]);
        color = blended[0];

        record = table->records;
        for (i = 0; i < count; i++, record++) {
            if (record->pointCount != 0) {
                record->pointSet->rows = record->pointCount;
                record->pointSet->color = color;
                effDrawThreePointGroups(record->pointSet, &matrix);
            }
        }
    }

    if (config->drawReferences != 0) {
        color = work->color;
        record = table->records;
        for (i = 0; i < count; i++, record++) {
            effTrackSet_t *references = record->references;

            references->color = color;
            effDrawTrackSet(references, &matrix);
        }
    }
}

effModelResource_t *Create_effPolygonCrack(u16 kind, void *source, void *secondary, u32 param) {
    u32 headerSize = 0x40;
    u32 size = effModelResourceOperations[kind].payloadSize;
    effModelResource_t *effect = (effModelResource_t *)sdfChipAllocDirect(size + headerSize);

    effect->source = (effSpanConfig_t *)((u8 *)effect + headerSize);
    effect->color = 0x80808080;
    effect->scale = 1.0f;
    effect->updateCount = 0;
    effect->kind = kind;
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->position);
    VU0_STORE_VF_UNCLOBBERED($vf0, effect->orientation);
    memcpy(effect->source, source, size);
    if (secondary != NULL) {
        effect->model = effLoadViewerModelWithVUState(secondary, param);
        effect->attributes = param;
        effect->childResource = effModelResourceOperations[kind].createResource(effect->source, effect->model);
        effModelResourceOperations[kind].initialize(effect);
    }
    return effect;
}

u32 PartsCreate_effPolygonCrack(fileJobPayload_t *work) {
    void *first = fileResolvePrimaryBuffer(work);
    void *second = fileResolveSecondaryBuffer(work);
    return (u32)Create_effPolygonCrack(work->option, first, second, work->secondary.size);
}

void Delete_effPolygonCrack(effModelResource_t *effect) {
    effModelResourceOperations[effect->kind].destroyResource(effect->childResource);
    effDestroyModelContext(effect->model);
    sdfChipFreeDirect(effect);
}

effModelResource_t *Copy_effPolygonCrack(effModelCreateRequest_t *work) {
    effModelResource_t *effect = Create_effPolygonCrack(work->kind, work->source, 0, 0);
    s32 x = mdlGetMajorNum(work->assetId);
    s32 y = mdlGetMinorNum(work->assetId);
    mdlObj_t *model = mdlLoad(x, y);

    effect->model = model;
    Init_effModel(model);
    effect->attributes = work->attributes;
    effect->childResource = effModelResourceOperations[effect->kind].createResource(effect->source, effect->model);
    effModelResourceOperations[effect->kind].initialize(effect);
    return effect;
}

void effResetModelResourceUpdateCount(effModelResource_t *effect) {
    effModelResourceOperations[effect->kind].initialize(effect);
    effect->updateCount = 0;
}

void effAdvanceModelResourceUpdateCount(effModelResource_t *effect) {
    effModelResourceOperations[effect->kind].update(effect);
    effect->updateCount = effect->updateCount + 1;
}

void effDispatchModelResourceCallback(effModelResource_t *effect) {
    effModelResourceOperations[effect->kind].draw(effect);
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSRAnimation13);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf374);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf375);

void effSetModelResourceColor(effModelResource_t *work, u32 color) {
    work->color = color;
}

void effSetModelResourceScale(effModelResource_t *work, f32 scale) {
    work->scale = scale;
}

effPointSet_t *effCreatePointSet3(s32 count) {
    s32 rows = count * 3 + 3;
    s32 size = rows * 20;
    struct sdfMemHandle_t *base;
    u8 *data;
    effPointSet_t *set;

    size = ((size >> 4) + ((size & 0xF) != 0)) << 4;
    base = sdfMemAllocDirect(size + 0x20);
    data = (u8 *)sdfMemLock(base);
    set = (effPointSet_t *)(data + size);
    set->buffer = data;
    data += rows * 16;
    set->color = 0x80808080;
    set->type = 2;
    set->rows = rows;
    set->allocation = base;
    set->tail = data;
    set->flag = 0;
    set->handle = sdfMatNew();
    sdfMatSetConstant(set->handle, 1.0f);
    memset(effWork4, 0, 0x2C);
    effWork4[0].attr = 0x4000;
    return set;
}

/* Queue the draw asset for release and return the backing allocation. */
void effReleaseModelPointSetAsset(effPointSet_t *set) {
    sdfMatFreeDelayed(set->handle);
    sdfMemFreeDirect(set->allocation);
}

void effDrawThreePointGroups(effPointSet_t *set, effMatrix4_t *matrix) {
    void *list;
    effGsPacket_t *packet;
    s32 remaining;

    if (set->color & 0xFF000000) {
        list = (void *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(list);
        if (matrix == NULL) {
            SDF_VU0_LOADIDENTITY();
        } else {
            VU0_LOAD_MATRIX(matrix);
        }
        sdf3DPrimLinkTm(list, 0);
        sdf3DPrimLinkMat(list, set->handle, 0);
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x31801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        remaining = set->rows;
        effWork4[0].vertexColor = (u32 *)set->tail;
        effWork4[0].vertex = (u128 *)set->buffer;
        effWork4[0].color = set->color;
        effWork4[0].faceCount = 0x10;
        effWork4[0].pointCount = 0xF;
        effWork4[0].faceIndex = (u32 *)effData8;
        while (remaining >= 0xF) {
            remaining -= 0xC;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(effWork4));
            effWork4[0].vertex += 0xC;
            effWork4[0].vertexColor += 0xC;
        }
        if (remaining >= 6) {
            effWork4[0].faceCount = 4;
            effWork4[0].pointCount = remaining;
            sdfDrawTagLink(list, effPkt3DPrimMakePoly(effWork4));
        }
        if (set->flag == 0) {
            packet = (effGsPacket_t *)sdfGrAllocPacketBuff(0x30);
            packet->dmaTag = 2;
            packet->vifTag = (((u64)0x50000002 << 16) | 0x1000) << 16;
            packet->gifTag = ((u64)0x10000000 << 32) | 0x8001;
            packet->registerList = 0xE;
            packet->registerValue = 0x51801;
            packet->registerAddress = 0x47;
            sdfDrawTagLink(list, (u32)packet);
        }
        effDrawThreePointGroupsData[set->type]->regFunc(effDrawThreePointGroupsData[set->type], list);
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", PartsInit);

INCLUDE_ASM(const s32, "code_r1_00226EC8", PartsCalc);

INCLUDE_ASM(const s32, "code_r1_00226EC8", Create_effObjectParticle);

INCLUDE_ASM(const s32, "code_r1_00226EC8", ParamsCreate);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCreateSlotNodeFromFileJob);

INCLUDE_ASM(const s32, "code_r1_00226EC8", Delete_effObjectParticle);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCloneOwnedState);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCloneSlotNodeSecondaryResources);

INCLUDE_ASM(const s32, "code_r1_00226EC8", BehaviorSet);

INCLUDE_ASM(const s32, "code_r1_00226EC8", ModelSet);

INCLUDE_ASM(const s32, "code_r1_00226EC8", PartsListSet_effObjectParticle);

typedef struct effSlotNode80_t {
    u32 count;
    u32 color;
    f32 opacity;
    s32 resourceKind;
    u8 randomDirection;
    u8 pad11[3];
    f32 seedDirection[3];
    u8 pad20[4];
    f32 angularSpeed;
    f32 speedRandomness;
    f32 angularAcceleration;
    u16 scaleByOpacity;
    u8 pad32[0x2E];
    mdlObj_t *model;
    void *deviceSlot;
    plistHandle_t **resourceEntries;
    struct sdfMemHandle_t *entryAllocation;
    bhvrHandle_t *record;
    u8 *positions;
    struct sdfMemHandle_t *positionAllocation;
    u16 active;
} effSlotNode80_t;

void effCalc(bhvrHandle_t *record);

void effClearSurfaceRecordReferences(void *node) {
    bhvrHandle_t *record = ((effSlotNode80_t *)node)->record;

    if (record != NULL) {
        effCalc(record);
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", Calc);

INCLUDE_ASM(const s32, "code_r1_00226EC8", Draw_effObjectParticle);

INCLUDE_ASM(const s32, "code_r1_00226EC8", CalcAndDraw);

void effSetActiveSlotPrimaryRecordVector(void *node, f32 vector[4]) {
    mnuRecordSetVector(((effSlotNode80_t *)node)->record, vector);
}

void effSetActiveSlotSecondaryRecordVector(void *node, f32 vector[4]) {
    fileSetRecordSecondVector(((effSlotNode80_t *)node)->record, vector);
}

void effSetActiveSlotColor(effSlotNode80_t *work, u32 color) {
    work->color = color;
}

void effSetActiveSlotOpacity(void *work, f32 opacity) {
    ((effSlotNode80_t *)work)->opacity = opacity;
    dds3DispatchIndexedCallback((u16 *)((effSlotNode80_t *)work)->record, opacity);
}

/* Alternate particle handles share a header but occupy distinct slots. */
effParticleSharedResource_t *eff2PartsCreate(fileJobPayload_t *source) {
    effParticleSharedResource_t *work = sdfChipAllocDirect(sizeof(effParticleSharedResource_t));
    void *buffer;

    memset(work, 0, sizeof(effParticleSharedResource_t));
    work->color = 0x80808080;
    work->billHandle = NULL;
    work->reference = NULL;
    if (source == NULL) {
        return work;
    }
    work->option = source->option;
    buffer = fileResolvePrimaryBuffer(source);
    memcpy(&work->colorTrack, buffer, EFF_PARTICLE_SHARED_PAYLOAD_BYTES);
    buffer = fileResolveSecondaryBuffer(source);
    if (buffer != NULL) {
        switch (source->primary.selector) {
        case 1:
            work->billHandle = dds3BillboardCreate(0, (u32)buffer);
            break;
        case 2:
            work->billHandle = dds3BillboardCreate(1, (u32)buffer);
            break;
        case 4:
            work->billHandle = dds3BillboardGeneral(*(s32 *)buffer);
            break;
        case 7:
            work->reference = effCacheTexCreate((u32)buffer);
            break;
        }
        if (work->billHandle != NULL) {
            dds3BillboardSetAnimColorAdvMode(work->billHandle);
        }
    }
    return work;
}

void effReleaseParticleResources(effParticleSharedResource_t *work) {
    billHandle_t *particle = work->billHandle;
    if (particle != NULL) {
        dds3BillboardDelete(particle);
    }
    if (work->reference != NULL) {
        effCacheTexDelete(work->reference);
    }
    sdfChipFreeDirect(work);
}

effParticleSharedResource_t *Copy_eff2D(effParticleSharedResource_t *source) {
    effParticleSharedResource_t *effect = eff2PartsCreate(NULL);
    memcpy(&effect->colorTrack, &source->colorTrack, EFF_PARTICLE_SHARED_PAYLOAD_BYTES);
    eff2ResourceCopy(effect, source);
    return effect;
}

void eff2ResourceCopy(effParticleSharedResource_t *work, effParticleSharedResource_t *source) {
    billHandle_t *billboard;
    if (source->billHandle != 0) {
        if (work->billHandle != 0) {
            dds3BillboardDelete(work->billHandle);
        }
        billboard = dds3BillboardCopy(
            source->billHandle);
        work->billHandle = billboard;
        dds3BillboardSetAnimColorAdvMode(billboard);
        return;
    }
    if (work->reference != NULL) {
        effCacheTexDelete(work->reference);
    }
    work->reference = Copy_effCacheTexList(source->reference);
}

void effResetParticleSharedState(effParticleSharedResource_t *work) {
    work->state = 0;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", eff2Draw);

void effSetParticleSharedColor(effParticleSharedResource_t *work, u32 color) {
    work->color = color;
}

void effDrawEffHelperSub(void) {
}

/* Battle light effect payload (0x20 bytes, effRuntimeResourceOperations entry after sbtlCallBtlEffCreateUnitList). */
typedef struct sbtlEffLightParam_t {
    u8 pad00[0x14];
    u8 area;      /* sbtlEffCreateUnitList area */
    u8 flag15;
    u8 flag16;
    u8 lightSlot; /* sbtlSetLight_SetPos slot */
    f32 range;
    f32 value1C;
} sbtlEffLightParam_t;

typedef char sbtlEffLightParamSizeCheck[(sizeof(sbtlEffLightParam_t) == 0x20) ? 1 : -1];

extern s8 sbtlLeaf51Data;

#ifdef VERSION_RAIDOU1
s32 sbtlEffSetActer(s32 id) {
    sbtleffbo_acter = sbtlSearchUnit_ID(wk_sbtl->unitList, id);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlEffSetActer);
#endif

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlEffSetTarget);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlEffCreateUnitList);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlLeaf48);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallBtlSetLightMap);

void sbtlLeaf49(void) {
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallSetUnitColorRGB);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallSetUnitColorRGB2);

s32 effComputeLightDirectionVU(mdlObj_t *vector, sdf3DLightParallelPacket_t *target) {
    s32 result = btlIsRuntimeAllocated();

    if (result != 0) {
        if (effData36 == 0) {
            return 0;
        }
        mdlGetPosition(vector);
        VU0_LOAD_VF($vf11, effWork5);
        VU0_SUB($vf10, $vf10, $vf11);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_NORMALIZE();
        VU0_STORE_VF($vf10, effComputeLightDirectionVUWork);
        sdf3DMakeLightParallelPacket(target, effComputeLightDirectionVUData, sbtlLeaf50Work2);
        return 1;
    }
    return result;
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effResetDefaultColorTables);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlLeaf50);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallBtlEffCreateUnitList);

void sbtlLeaf51(effActiveResource_t *work) {
    sbtlEffLightParam_t *param = work->payload;

    param->lightSlot = sbtlLeaf51Data;
    sbtlLeaf51Data++;
    if (sbtlLeaf51Data >= 127) {
        sbtlLeaf51Data = 1;
    }
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallBtlSetLightMap2);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlLeaf52);

INCLUDE_ASM(const s32, "code_r1_00226EC8", sbtlCallBtlEffCreateUnitList2);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallSetUnitColorRGB);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effCallBtlEffCreateUnitList);

void effLeaf376(void) {
}

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf377);

INCLUDE_ASM(const s32, "code_r1_00226EC8", effLeaf378);

void effDestroyCopiedEffectPayload(effCopiedPayload_t *payload) {
    dds3Basic_t **effects = payload->effects;
    dds3PathBasicControl_t **targets = payload->targets;
    u32 i;

    for (i = 0; i < 5; i++) {
        if (*targets != 0) {
            dds3DeletePathControl(*targets);
        }
        targets++;
        if (*effects != 0) {
            dds3DeleteSceneBasicData(*effects);
        }
        effects++;
    }
    sdfMemFreeDirect(payload->allocation);
}

void effLeaf379(void) {
}

void effLeaf380(void) {
}

void effLeaf381(void) {
}

void effLeaf382(void) {
}

void effLeaf383(void) {
}

void effResetSlots(void) {
    effPadVibeFrames[0] = 0;
    effPadVibeRemain[0] = 0;
    effPadVibePower[0] = 0;
    effPadVibeFadeIn[0] = 0;
    effPadVibeFrames[1] = 0;
    effPadVibeRemain[1] = 0;
    effPadVibePower[1] = 0;
    effPadVibeFadeIn[1] = 0;
    dds3PadIsTypeCD();
}

void effPadVibeFadeUpdate(void) {
    f32 full = 1.0f;
    f32 ratio;
    s32 port;

    for (port = 0; port < 2; port++) {
        if (effPadVibePower[port] != 0) {
            if (effPadVibeRemain[port] > 0 && effPadVibeFrames[port] > 0) {
                ratio = (f32)effPadVibeRemain[port] / (f32)effPadVibeFrames[port];
                if (effPadVibeFadeIn[port] != 0) {
                    ratio = full - ratio;
                }
                dds3PadAct(port, (u32)(effPadVibePower[port] * ratio), 100);
                effPadVibeRemain[port]--;
            } else if (effPadVibeFadeIn[port] == 1) {
                dds3PadAct(port, effPadVibePower[port], 100);
            } else {
                dds3PadAct(port, 0, 0);
                effPadVibeFrames[port] = 0;
            }
        }
    }
}
#ifdef VERSION_RAIDOU1

effKindDesc_t effData5[6] = {
    { NULL, NULL, NULL, NULL, 0x0 },
    { NULL, NULL, effUpdateFadeBlendA, NULL, 0xE8 },
    { effCreateFadeBlendWorkFromOutput, effReleaseFadeBlendWork, effUpdateProjectedBlurFadeRectangle, effUpdateTarget, 0xEC },
    { effCreateFixedSlotBlurWorkFromFadeOutput, effReleaseFixedSlotBlurWork, effUpdateFadeMapA, effTextureReferenceRelease, 0xEC },
    { effCreateVariableSlotBlurWorkFromFadeOutput, effReleaseVariableSlotBlurWork, effUpdateFadeMapB, effReplaceKindLinkedTarget, 0xEC },
    { NULL, NULL, effUpdateFadeBlendB, NULL, 0xE8 },
};

effKindDesc_t effData6[3] = {
    { NULL, NULL, NULL, NULL, 0x0 },
    { NULL, NULL, effUpdateFadeBlendC, NULL, 0xD8 },
    { effCreateFadeColorWorkFromOutput, effReleaseFadeColorWork, effUpdateProjectedFadeColor, effReplaceLinkedKindWorkTarget, 0xE4 },
};
#endif

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sbtlCallBtlEffCreateUnitList2Table);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrAccurate);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrLength);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrResource);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrSaveEp);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrAmplitude);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrMaterial);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrVcEndOut);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrVcEndIn);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrVcStartOut);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrVcStartIn);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrTBoard2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrCycleRot);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrLifeScl);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrLifeCol);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEmtSpre);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEmtRadi);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEQuake2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEdgeOut);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrBillboard);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrEmtCurve);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrEmtHeight);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrEmtRadius);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrScaleUv);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrCrackPolygon);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrFlashPolygon);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrDirection);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrOF2Cam);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrOLightS);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrOMcolrS);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePAspak1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePAspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePHspak1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePHspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSphre1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSpirl1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSpark2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSpark1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSmoke2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSmoke1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSamplePSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleGHspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleGSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleGSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleGSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleGSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleKHspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleKSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleKSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleKSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleKSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleBParal0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleBRiple0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleBDist0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleBMulti0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleBFull0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleFMulti0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleFFull0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleTBoard1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleTBoard0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleEQuake20);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleEQuake0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleEFlare0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleERain0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSCylnd0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSRing0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSSpark1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSRadi1Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSRngT0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSSctT0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSSpkT1Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSSpkT0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleSRadT0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleRCrcle1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleRCrcle0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleRTrack0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleRNomal1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleRNomal0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleDBug1Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleDBug0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleDPoint0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleWHspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleWSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleWSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleWSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleWSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNCyldr0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNCylnd1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNCylnd0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNSpirl2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNSpirl1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleNSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleUSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleUSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleUSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleUCylnd0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleCNomal0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOMcolr2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOLight2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOUnitd0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOFovy0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOVibe0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOFldF0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOFldL0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOSe0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOMot0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOEffL0Ep);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOMcolr0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrToolEffectSampleOLight0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleXNomal0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMAspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMHspak0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMSphre0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMSpirl0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMSpark0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectSampleMSmoke0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrResetOk);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrInstanceOk);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrDeleteOk);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrCopyOk);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrExitOk);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrCameraY);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrCamera);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrGround);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPosition);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrOffset);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrSize);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrInstance);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPoseture);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrBattleOnly);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrMuzzleFlash);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrF2Camera);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrFloorModel);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEnvironment);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyCrack);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyScaly);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrPolyWind);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyThunder);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyRing);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyFlash);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrPolyTex);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyTrackS2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPolyTrack);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrObjParticle);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", sysStrParticle);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrTestMenu2);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrTestMenu1);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrTestMenu0);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrPartsList);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrEffectEditor);

INCLUDE_RODATA(const s32, "code_r1_00226EC8", effStrToolEffectEp);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effData34);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effData35);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effCurrentRenderPacket);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effLoadWindTextureData);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effWindTextureHandle);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effSharedRibbonReferenceCount);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effSharedAssetReferenceReleaseData);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effLoadScalyTextureData);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effScalyTextureHandle);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effSharedStripReferenceCount);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effSharedScalyStripResource);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", sbtleffbo_acter);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", sbtleffbo_target);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", sbtlLeaf51Data);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effData36);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effPadVibeFrames);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effPadVibeRemain);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effPadVibePower);

INCLUDE_SDATA(const s32, "code_r1_00226EC8", effPadVibeFadeIn);

