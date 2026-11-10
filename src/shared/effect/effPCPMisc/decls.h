/* Declarations of smt3-decomp src/smt3/effect/effPCPMisc.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPCPMISC_H
#define SHARED_EFFECT_EFFPCPMISC_H

#include "common.h"

#include "effPCPData.h"

#include "fpu.h"

#include "ps2asm.h"

#include "sdf_resource.h"

#include "sdf_types.h"

#include "sdfFrame.h"

struct dds3ModelResrc_t;

struct dds3ModelResrcPlayBook_t;

struct P2A_ANIMATION_tag;

struct sdfBillBoardBatch_t;

struct P2A_TEXANIM_tag;

struct billRenderPair_t;

struct billUvQuad_t;

struct effBTL_SkillPositionPtr_t;

struct effBTL_SkillPartsList_t;

struct effBTL_SkillCutTable_t;

union effBTLPartValue_t;

struct effPCPSparkleSphereParam_t;

struct effPCPSparkleSphere_tag;

struct dds3Effect_Blur_t;

struct dds3Effect_RippleBlurParam_t;

struct effBlurScaleSlot_t;

struct effBlurScaleWork_t;

struct dds3Effect_DistortionBlurParam_t;

struct effBlurScatterSlot_t;

struct effBlurScatterWork_t;

struct dds3Effect_PartialMultiBlurParam_t;

struct effPCPAgiraoFirePartList_t;

struct effPCPRingPolyExp2Param_t;

struct effPCPRingPolyExp2_tag;

struct dds3RingPolyExpList_tag;

struct effPCPRingPolyExpParam_t;

struct effPCPRingPolyExp_tag;

struct effPCPAgiFireballPartList_t;

struct effPCPAgiFireballParam_t;

struct effPCPAgiFireball_tag;

struct effPCPEyeThunder_tag;

struct effPCPMudo_tag;

struct effPCPMultiBlurExp2_tag;

struct effPCPMultiFilterExpParam_t;

struct effPCPRippleBlurExpParam_t;

struct effPCPDistortionBlurExpParam_t;

struct effPCPMultiBlurExp2Param_t;

struct effPCPDistortionBlurExp_tag;

struct effPCPHamaon_tag;

struct effPCPMHanma01_tag;

struct effPCPRadiateBEDParam_t;

struct effPCPRadiateBEDPart_tag;

struct effPCPRadiateBED_tag;

struct effPCPHelperBEDParam_t;

struct effEveBEDParams_t;

struct effPCPAnalyzeBED_tag;

struct effPCPBlurExpParam_t;

struct effPCPBlurExp_tag;

struct effPCPShockWave01_tag;

struct effPCPFilterExpParam_t;

struct effPCPFilterExp_tag;

struct effPCPRadiateThunderPart_tag;

struct effPCPRadiateThunderParam_t;

struct effPCPRadiateThunder_tag;

struct effPCPThunderSphereExp_tag;

struct effPCPHelperBEDPart_tag;

struct effPCPHelperBED_tag;

struct effPCPRadiateTBParam_t;

struct effPCPRadiateTBPart_tag;

struct effPCPRadiateTB_tag;

struct effPCPAgiFireball2Param_t;

struct effPCPAgiFireball2_tag;

struct effPCPThunderSphereExpParam_t;

struct effPCPBlinkMultiBlurBase_tag;

struct effPCPBlinkMultiBlur_tag;

struct effPCPBreathParam_t;

struct effPCPBreath_tag;

struct effPCPHwave01_tag;

struct effPCPSpiralBEDPart_tag;

struct effPCPSpiralBEDParam_t;

struct effPCPSpiralBED_tag;

struct effPCPBufuIce_tag;

struct effPCPMakka_tag;

struct effPCPHama02_tag;

struct effPCPZiodin01_tag;

struct effPCPBlinkMultiBlurParam_t;

struct effPCPMultiBlurExp_tag;

struct effPCPAgiraoFireParam_t;

struct effPCPAgiraoFire_tag;

struct effPCPHama01_tag;

struct effDebugPolyParams_t;

struct effRandState_t;

struct effResourceRectBounds_t;

struct effResourceRectDrawParams_t;

struct effPCPThunderSpherePolyParam_t;

struct effPCP_ThunderPolyParam_t;

struct effPCPThunderSpherePolyPart_t;

struct effPCPThunderSpherePoly_t;

struct effTrackPolyParams_t;

struct sdfIpuDmaRegs_t;

struct dds3ModelBlurCb_t;

struct dds3ModelBlurInstance_t;

struct mdlIpuAttachment_t;

struct pibChunkItem_t;

struct dds3ModelEffectCtrlr_t;

struct sdfMotionCutDef_t;

struct sdfMovieStrmLinear_t;

union movieDecodeCtx_t;

struct parCell_t;

struct sdfVector3_t;

struct sdfMatPacket_t;

struct sdfModelPacketHeader_tag;

struct sdfModelNode_t;

struct sdfModelNodeDef_t;

struct sdfModelNodeDefList_t;

struct sdfMotionNodeDef_t;

struct sdfIpuDecodeCb_t;

struct sdfIpuDmaTag_t;

struct sdfGsMemSlice_t;

struct effBTLBank_t;

typedef struct billUvQuad_t billUvQuad_t;

typedef struct sdfBillBoardBatch_t sdfBillBoardBatch_t;

typedef struct sdfTexPacket_t sdfTexPacket_t;

typedef struct dds3BillResource_tag dds3BillResource_tag;

typedef struct P2A_ANIMATION_tag P2A_ANIMATION_tag;

typedef struct dds3BillAnimResource_tag dds3BillAnimResource_tag;

typedef struct P2A_TEXANIM_tag P2A_TEXANIM_tag;

typedef struct dds3BillAnim_tag dds3BillAnim_tag;

typedef struct billRenderPair_t billRenderPair_t;

typedef struct billHandle_t billHandle_t;

typedef struct sdfGrOt_t sdfGrOt_t;

typedef struct effPCPSparkleSphereParam_t effPCPSparkleSphereParam_t;

typedef struct effPCPSparkleSphere_tag effPCPSparkleSphere_tag;

typedef struct dds3Effect_Blur_t dds3Effect_Blur_t;

typedef struct dds3Effect_PartialMultiBlurParam_t dds3Effect_PartialMultiBlurParam_t;

typedef struct dds3Effect_PartialMultiBlur_t dds3Effect_PartialMultiBlur_t;

typedef struct effHandle_t effHandle_t;

typedef struct effPCPAgiraoFirePartList_t effPCPAgiraoFirePartList_t;

typedef struct effPCPRingPolyExp2Param_t effPCPRingPolyExp2Param_t;

typedef struct sdfMatPacket_t sdfMatPacket_t;

typedef union sdfMatUvwParm_t sdfMatUvwParm_t;

typedef struct dds3RingPolyExpList_tag dds3RingPolyExpList_tag;

typedef struct effPCPRingPolyExp2_tag effPCPRingPolyExp2_tag;

typedef struct effPCPRingPolyExpParam_t effPCPRingPolyExpParam_t;

typedef struct effPCPRingPolyExp_tag effPCPRingPolyExp_tag;

typedef struct effPCPAgiFireballPartList_t effPCPAgiFireballPartList_t;

typedef struct effPCPAgiFireballParam_t effPCPAgiFireballParam_t;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct effPCPAgiFireball_tag effPCPAgiFireball_tag;

typedef struct effPCPEyeThunder_tag effPCPEyeThunder_tag;

typedef struct effPCPMudo_tag effPCPMudo_tag;

typedef struct effResourceRectBounds_t effResourceRectBounds_t;

typedef struct effResourceRectDrawParams_t effResourceRectDrawParams_t;

typedef struct effPCPMultiBlurExp2_tag effPCPMultiBlurExp2_tag;

typedef struct effPCPMultiFilterExpParam_t effPCPMultiFilterExpParam_t;

typedef struct dds3Effect_RippleBlurParam_t dds3Effect_RippleBlurParam_t;

typedef struct effPCPRippleBlurExpParam_t effPCPRippleBlurExpParam_t;

typedef struct dds3Effect_DistortionBlurParam_t dds3Effect_DistortionBlurParam_t;

typedef struct effPCPDistortionBlurExpParam_t effPCPDistortionBlurExpParam_t;

typedef struct effPCPMultiBlurExp2Param_t effPCPMultiBlurExp2Param_t;

typedef struct effBlurScaleSlot_t effBlurScaleSlot_t;

typedef struct effBlurScaleWork_t effBlurScaleWork_t;

typedef struct effBlurScatterSlot_t effBlurScatterSlot_t;

typedef struct effBlurScatterWork_t effBlurScatterWork_t;

typedef struct effPCPDistortionBlurExp_tag effPCPDistortionBlurExp_tag;

typedef struct effPCPHamaon_tag effPCPHamaon_tag;

typedef struct effPCPMHanma01_tag effPCPMHanma01_tag;

typedef struct effPCPRadiateBEDPart_tag effPCPRadiateBEDPart_tag;

typedef struct effPCPRadiateBEDParam_t effPCPRadiateBEDParam_t;

typedef struct effBTL_SkillPositionPtr_t effBTL_SkillPositionPtr_t;

typedef union effBTLPartValue_t effBTLPartValue_t;

typedef struct effBTL_SkillCutTable_t effBTL_SkillCutTable_t;

typedef struct effBTLBank_t effBTLBank_t;

typedef struct effBTL_t effBTL_t;

typedef struct effPCPRadiateBED_tag effPCPRadiateBED_tag;

typedef struct effPCPHelperBEDParam_t effPCPHelperBEDParam_t;

typedef struct effEveBEDParams_t effEveBEDParams_t;

typedef struct effPCPThunderSpherePolyParam_t effPCPThunderSpherePolyParam_t;

typedef struct effPCPThunderSpherePolyPart_t effPCPThunderSpherePolyPart_t;

typedef struct parCell_t parCell_t;

typedef struct parSystem_t parSystem_t;

typedef struct effPCPThunderSpherePoly_t effPCPThunderSpherePoly_t;

typedef struct effPCPAnalyzeBED_tag effPCPAnalyzeBED_tag;

typedef struct effPCPFLEACHBlurParam_t effPCPFLEACHBlurParam_t;

typedef struct effPCPFLEACHBlur_tag effPCPFLEACHBlur_tag;

typedef struct effPCPShockWave01_tag effPCPShockWave01_tag;

typedef struct effPCPFilterExpParam_t effPCPFilterExpParam_t;

typedef struct effPCPFilterExp_tag effPCPFilterExp_tag;

typedef struct effPCPBlurExpParam_t effPCPBlurExpParam_t;

typedef struct effPCPBlurExp_tag effPCPBlurExp_tag;

typedef struct effPCPRadiateThunderPart_tag effPCPRadiateThunderPart_tag;

typedef struct effPCP_ThunderPolyParam_t effPCP_ThunderPolyParam_t;

typedef struct effPCPRadiateThunderParam_t effPCPRadiateThunderParam_t;

typedef struct effPCPRadiateThunder_tag effPCPRadiateThunder_tag;

typedef struct effPCPThunderSphereExp_tag effPCPThunderSphereExp_tag;

typedef struct effPCPHelperBEDPart_tag effPCPHelperBEDPart_tag;

typedef struct effPCPHelperBED_tag effPCPHelperBED_tag;

typedef struct effPCPRadiateTBPart_tag effPCPRadiateTBPart_tag;

typedef struct effPCPRadiateTBParam_t effPCPRadiateTBParam_t;

typedef struct effPCPRadiateTB_tag effPCPRadiateTB_tag;

typedef struct effPCPAgiFireball2Param_t effPCPAgiFireball2Param_t;

typedef struct effPCPAgiFireball2_tag effPCPAgiFireball2_tag;

typedef struct effPCPThunderSphereExpParam_t effPCPThunderSphereExpParam_t;

typedef struct effPCPBlinkMultiBlurBase_tag effPCPBlinkMultiBlurBase_tag;

typedef struct effPCPBlinkMultiBlur_tag effPCPBlinkMultiBlur_tag;

typedef struct effPCPBreathParam_t effPCPBreathParam_t;

typedef struct effPCPBreath_tag effPCPBreath_tag;

typedef struct effPCPHwave01_tag effPCPHwave01_tag;

typedef struct effPCPSpiralBEDPart_tag effPCPSpiralBEDPart_tag;

typedef struct effPCPSpiralBEDParam_t effPCPSpiralBEDParam_t;

typedef struct effPCPSpiralBED_tag effPCPSpiralBED_tag;

typedef struct effPCPBufuIce_tag effPCPBufuIce_tag;

typedef struct effPCPHama02_tag effPCPHama02_tag;

typedef struct effPCPZiodin01_tag effPCPZiodin01_tag;

typedef struct effPCPBlinkMultiBlurParam_t effPCPBlinkMultiBlurParam_t;

typedef struct effPCPMultiBlurExp_tag effPCPMultiBlurExp_tag;

typedef struct effPCPAgiraoFireParam_t effPCPAgiraoFireParam_t;

typedef struct effPCPAgiraoFire_tag effPCPAgiraoFire_tag;

typedef struct effPCPHama01_tag effPCPHama01_tag;

typedef struct effRandState_t effRandState_t;

typedef struct sdfMotionCutDef_t sdfMotionCutDef_t;

typedef struct sdfMotionNodeDef_t sdfMotionNodeDef_t;

typedef struct sdfMotionPlayBook_t sdfMotionPlayBook_t;

typedef struct dds3ModelResrcPlayBook_t dds3ModelResrcPlayBook_t;

typedef struct sdfItemArray_t sdfItemArray_t;

typedef struct pibChunkItem_t pibChunkItem_t;

typedef struct sdfModelExtraChunk_t sdfModelExtraChunk_t;

typedef struct sdfModelNodeDef_t sdfModelNodeDef_t;

typedef struct sdfModelDef_t sdfModelDef_t;

typedef struct dds3ModelResrc_t dds3ModelResrc_t;

typedef struct sdfMotionCtrlr_t sdfMotionCtrlr_t;

typedef struct sdfModelPacketHeader_tag sdfModelPacketHeader_tag;

typedef struct sdfModelNode_t sdfModelNode_t;

typedef struct sdfModelRootNode_t sdfModelRootNode_t;

typedef struct dds3ModelBlurInstance_t dds3ModelBlurInstance_t;

typedef struct dds3ModelBlurCb_t dds3ModelBlurCb_t;

typedef struct dds3HermitianTrackPoly_tag dds3HermitianTrackPoly_tag;

typedef struct effTrackPolyParams_t effTrackPolyParams_t;

typedef struct mdlTrackWork_t mdlTrackWork_t;

typedef struct sdfIpuDmaRegs_t sdfIpuDmaRegs_t;

typedef struct sdfFileCmdHandle_t sdfFileCmdHandle_t;

typedef struct sdfMovieStrmLinear_t sdfMovieStrmLinear_t;

typedef struct sdfMovieStrmPac_t sdfMovieStrmPac_t;

typedef union movieDecodeCtx_t movieDecodeCtx_t;

typedef struct sdfIpuDecodeCb_t sdfIpuDecodeCb_t;

typedef struct movieStream_t movieStream_t;

typedef struct sdfIpuDmaTag_t sdfIpuDmaTag_t;

typedef struct mdlEffectResrc_IPU_t mdlEffectResrc_IPU_t;

typedef struct mdlIpuAttachment_t mdlIpuAttachment_t;

typedef struct mdlEffectResrcItem_t mdlEffectResrcItem_t;

typedef struct mdlMarkPos_t mdlMarkPos_t;

typedef struct dds3ModelEffectCtrlr_t dds3ModelEffectCtrlr_t;

typedef struct effDebugPolyParams_t effDebugPolyParams_t;

#define EFF_DEG2RAD 0.017453292f

struct billHandle_t;

struct dds3BillResource_tag;

/* Native billboard UVs are four ordered U/V corners, not a float vector. */
struct billUvQuad_t {
    u16 components[8];
};

/* Fifteen billboard quads precede the child manager in its 0x450 allocation. */
struct sdfBillBoardBatch_t {
    f32 centre[15][4]; /* 0x000 */
    u32 color[15];       /* 0x0F0 */
    billUvQuad_t uv[15]; /* 0x12C */
    f32 offset[15][8];   /* 0x21C: four X/Y corner offset per quad */
    s32 count;           /* 0x3FC */
};

struct sdfDrawTag_t;

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

struct sdfTexHandle_t;

struct sdfTexPacket_t;

struct sdfTexPacket_t {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u32 gifTagControl; /* High GIFtag control word; one packed register. */
    u64 gifTagRegisters; /* Packed register list: A+D. */
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 samplingRegister; /* GS TEX1_1/TEX1_2 register selector. */
    u64 textureState; /* GS TEX0 data. */
    u64 textureRegister; /* GS TEX0_1/TEX0_2 register selector. */
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
};

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

/* Child manager at retained allocation +0x400; shared by resource and draw owners. */
struct dds3BillResource_tag {
    sdfTexHandle_t *hTex; /* 0x00: acquired texture reference. */
    u16 type; /* Child type bits; signed inputs narrow on assignment. */
    u8 pad06[2];
    s32 count;      /* 0x08 */
    billUvQuad_t uv; /* 0x0C */
    f32 cx;             /* 0x1C */
    f32 cy;             /* 0x20 */
    f32 width;  /* 0x24 */
    f32 height; /* 0x28 */
    sdfDrawTag_t *pBillTag[5]; /* 0x2C */
    u16 current_mode; /* 0x40 */
    u8 pad42[2];
    struct sdfMemHandle_t *hMem; /* 0x44: hMem descriptor released with this child. */
    sdfBillBoardBatch_t *batch; /* 0x48: retained packet-buffer address */
    struct dds3BillResource_tag *next; /* 0x4C */
};

struct dds3BillAnimResource_tag;

struct P2A_ANIMATION_tag {
    s32 spranm;
    s32 alpha;
    u32 attr; /* 0x08: 0x40/0x80 select two-entry render modes; 0x100 remains unclassified. */
    u32 times;
    u32 mode;
};

/* Runtime header precedes the copied resource bytes; the child table is indirect. */
struct dds3BillAnimResource_tag {
    struct sdfMemHandle_t *hMem; /* Retained general-heap descriptor. */
    u8 *pHeader;
    P2A_ANIMATION_tag *pAnimTable;
    s32 AnimNum;
    s32 TexNum;
    s32 count;
    struct dds3BillResource_tag **pResList;
};

struct dds3BillAnim_tag;

/* Billboard common prefix and animation-list view (max 0x64); child allocations are 0x34 bytes. */
struct dds3BillAnim_tag;

/* Ordinary animation frames contain geometry and texture coordinates. */
struct P2A_TEXANIM_tag {
    s16 sx;
    s16 sy;
    s16 cx;
    s16 cy;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    s16 texid;
    s16 attr; /* 0x12: signed countdown loaded when selecting a frame */
    f32 angle; /* 0x14: copied into the runtime quad rotation in radians. */
};

struct dds3BillAnim_tag {
    s32 hResource;
    s32 key;
    s32 wait;
    P2A_ANIMATION_tag *pAnim;
    P2A_TEXANIM_tag *pTexAnim;
};

/* Two-child draw descriptor at +0x34 in animation-list objects; child allocations end before it. */
struct billRenderPair_t {
    struct dds3BillResource_tag *children[2];
    u16 cameraFacingMode; /* 0x08: value 1 builds a view-facing matrix; other values use identity. */
    u16 kind;
    u32 colors[2];
    sdfDrawTag_t *packetList; /* 0x14 */
    struct billRenderPair_t *next; /* 0x18 */
};

struct billHandle_t {
    f32 pos[4]; /* 0x00: copied as a four-component pos by both render paths. */
    f32 childScaleX;  /* 0x10: set by dds3BillboardSetScale */
    f32 childScaleY;  /* 0x14 */
    f32 unk18;
    f32 unk1C;
    f32 rot;  /* 0x20: radian angle used to rotate the quad corners. */
    u32 abgr;   /* 0x24: set by dds3BillboardSetRGBA */
    void (*callback)(struct billHandle_t *); /* 0x28: called by dds3BillboardDraw; set from the
                          per-index table by dds3BillboardCreate */
    u16 type;         /* 0x2C: type 0/3 child, type 1 animation list */
    u16 mode; /* 0x2E: selected child packet-list pool */
    union {
        void *payload; /* 0x30: type-discriminated payload when its type is unknown */
        struct dds3BillResource_tag *child; /* child kinds */
        struct dds3BillAnimResource_tag *animationData; /* type 1 */
    };
    billRenderPair_t pair; /* 0x34: used by the mode-0x80 entry renderer */
    u16 animationActive; /* 0x50: cleared by empty or completed nonlooping animation */
    u8 pad52[2];
    u32 modeFlags; /* 0x54: bits 0x40/0x80 select billboard entry modes */
    u32 animationEntryIndex; /* 0x58: selected nonempty animation descriptor */
    s32 entryCount;
    struct dds3BillAnim_tag *resolvedEntries; /* 0x60: runtime animation state array */
};

struct sdfGrOt_t;

/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
struct sdfGrOt_t {
    struct sdfGrOt_t *next; /* 0x00 */
    sdfDrawTag_t *tagHead;     /* 0x04 */
    sdfDrawTag_t *tagTail;     /* 0x08 */
    u32 noSortTag;
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);          /* 0x10 */
    s32 (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    u32 data;
    u32 pad;
};

extern struct sdfGrOt_t *D_002F69B8[4];

extern u8 effPCPZiodin01Vec[];

extern s32 BlinkMultiBlurCount;

extern s8 effPCPBufuLight_Start;

extern f32 effPCPBufuLightDirection[4];

extern f32 effPCPBufuLightAxis[4];

/* The two titles register these eight compact table rows at matching indices.
 * These names describe registrations only; serialized kinds remain u16. */
enum EffParamWorkKind {
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0,
    EFF_PARAM_WORK_KIND_ANIMATED_BILLBOARD = 1,
    EFF_PARAM_WORK_KIND_CHILD_BILLBOARD = 2,
    EFF_PARAM_WORK_KIND_VIEWER_CONTEXT = 3,
    EFF_PARAM_WORK_KIND_EXTENDED_WORK = 4,
    EFF_PARAM_WORK_KIND_BILLBOARD_UNIT_OBJECT = 5,
    EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK = 6,
    EFF_PARAM_WORK_KIND_FILE_QUEUE = 7
};

/* Creation reads the requested mode from this parameter record. */
struct effPCPSparkleSphereParam_t {
    s32 abgr;
};

/* The first sixteen bytes of this allocated billboard set remain opaque. */
struct effPCPSparkleSphere_tag {
    u8 pos[0x10];
    u32 param;
    u32 abgr;
    f32 area;
    struct billHandle_t *hBill;
};

struct dds3Effect_PartialMultiBlur_t;

/* Shared rectangle and sampling payload; color is packed or read by channel. */
struct dds3Effect_Blur_t {
    u32 abgr; /* Byte consumers read the packed word through u8 object access. */
    s32 blend;
    f32 rotate;
    f32 scale;
    /* Native movie blends index the renderer center as a two-element vector. */
    s32 center[2];
    /* Native movie blends interpolate these four xy with a 2x2 loop. */
    s32 xy[2][2];
};

/* The textured blur allocation stores this copied body followed by a
 * separately acquired source texture. */
struct dds3Effect_PartialMultiBlurParam_t {
    s32 area;
    dds3Effect_Blur_t Blur;
};

struct dds3Effect_PartialMultiBlur_t {
    dds3Effect_PartialMultiBlurParam_t param;
    struct sdfTexHandle_t *texture;
};

struct effHandle_t;

/* Live effect node allocated in a 0x10-byte size-class block. */
struct effHandle_t {
    u32 type;       /* 0x00: type-operation row */
    s32 arg;        /* 0x04 */
    s32 instance;   /* 0x08: type-specific instance word */
    f32 unkC;       /* 0x0C */
};

/* Complete serialized effect-resource header. The payload's size and format
 * depend on type; this is not the separately allocated live effHandle_t.
 * Compatibility conversion examines the complete type and argument words,
 * while node creation consumes their low halfwords. */
struct effPCPAgiraoFirePartList_t {
    u32 type;       /* 0x00 */
    u32 arg;        /* 0x04 */
    u32 pad08;      /* 0x08 */
    f32 version;   /* 0x0C */
    u8 payload[0]; /* 0x10: variable-length tagged payload */
};

/* The copied 0x5C input head for the linked/ring beam variant. */
struct effPCPRingPolyExp2Param_t {
    f32 pos[4];
    f32 incline;
    f32 rotate;
    f32 rotate_s;
    f32 rotate_a;
    u8 mode;
    u8 pad21[3];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 radius;
    f32 radius_s;
    f32 radiusDecay;
    u32 split;
    u32 drawKind;
    f32 radiusStepA;
    u32 firstColor;
    f32 radiusStepB;
    u32 middleColor;
    f32 radiusStepC;
    u32 lastColor;
};

struct sdfMat_t;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
/* Names from the IDB, which include/libsdf.h also carries. The three
 * composites keep this unit's coarser reading: they cover pad+vifcode2, the
 * two three-register texture blocks and the two 24-byte transforms. */
struct sdfMatPacket_t {
    u32 vifcode0;          /* 0x00 */
    u32 ambient;           /* 0x04 */
    u32 diffuse;           /* 0x08 */
    u32 mtexDiffuse;       /* 0x0C: packed color applied by the secondary draw state */
    u32 specularColor;     /* 0x10: copied from the owning asset, or forced to zero. */
    u32 refmapColor;       /* 0x14 */
    u32 vifcode1;          /* 0x18 */
    f32 constant;          /* 0x1C */
    u32 mtexAlpha;         /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 mtexAlgo;          /* 0x24: secondary packet mode copied from SdfAsset */
    f32 fdBias;            /* 0x28 */
    f32 fdSoften;          /* 0x2C */
    u8 pad30[8];           /* 0x30: pad and vifcode2 */
    u64 tex1Data;          /* 0x38 */
    u64 tex0Data;
    u64 clampData;
    u64 mtexTex1Data;      /* 0x50 */
    u64 mtexTex0Data;
    u64 mtexClampData;
    /* sdfMatSetUvwParm writes one transform per texture: a 2x2 then an offset. */
    sdfMatTexTm_t texTm;   /* 0x68 */
    sdfMatTexTm_t mtexTexTm;
    u32 cmd_call_spec;          /* 0x98: VIF ITOP with immediate 0xC */
    u32 cmd_call_kind;          /* 0x9C: VIF MSCAL with microprogram index zero */
};

union sdfMatUvwParm_t;

/* Optional packed or five-float state block owned by an SdfAsset. */
union sdfMatUvwParm_t {
    struct {
        u64 unk0;
        u64 unk8;
        u32 unk10;
        u32 unk14;
    } packed;
    struct {
        f32 values[5];
        u32 unk14;
    } scalar;
    /* Named view of scalar.values; the last word remains opaque. */
    struct {
        f32 centerOffsetX;
        f32 centerOffsetY;
        f32 basisScaleX;
        f32 basisScaleY;
        f32 rotationInput;
        u32 unk14;
    } transformInputs;
};

/* One beam draw node owns its asset and vertex allocation separately. */
struct dds3RingPolyExpList_tag {
    f32 matrix[4][4];
    f32 rot[4][4];
    u128 pos;
    f32 scale;
    u32 blend; /* Indexes the node draw-dispatch table. */
    u32 color;
    u32 vertexCount;
    f32 *pVertex;
    u32 *pVC;
    struct sdfMat_t *pMat;
    struct sdfMemHandle_t *hMem;
};

struct effPCPRingPolyExp2_tag {
    effPCPRingPolyExp2Param_t head;
    s32 frame;
    u32 abgr;
    f32 rotate_z;
    f32 rotate_y;
    f32 rotate_s;
    u8 rotate_a[4];
    f32 radius;
    f32 radius_s;
    dds3RingPolyExpList_tag *hPoly;
};

/* The parameter prefix copied into the standard beam work. */
struct effPCPRingPolyExpParam_t {
    f32 pos[4];
    s32 fade_in;
    s32 fade_out;
    s32 wait;
    s32 life_a;
    u8 type_a[4];
    s32 life_r;
    f32 radius_s;
    f32 radius_e;
    u32 split;
    u32 blend;
    f32 width_e_in;
    u32 color_e_in;
    f32 width_c;
    u32 color_c;
    f32 width_e_out;
    u32 color_e_out;
};

struct effPCPRingPolyExp_tag {
    effPCPRingPolyExpParam_t param;
    s32 frame;
    u32 abgr;
    u32 point_max;
    dds3RingPolyExpList_tag *hPoly;
};

struct effPCPAgiFireballPartList_t {
    void *pHelper;
    void *pAttach[5];
    void *pTrack[3];
    void *pStart[5];
    void *pOkSmoke;
};

/* The serialized parameter block copied into each block-set work record. */
struct effPCPAgiFireballParam_t {
    f32 pos[4];
    s32 fade_in;
    s32 fade_out;
    f32 scale;
    s32 TrackCount[3];
    f32 TrackScale[3];
    f32 TrackScale_r[3];
    f32 TrackArea[3];
    s32 OkSmokeFrame;
};

struct effPartHandle_t;

/* One randomized channel record. */
struct effPartHandle_t;

/* Compact effect owner: its halfword kind selects the opaque payload's operations. */
struct effPartHandle_t {
    u16 kind;
    u8 pad02[2];
    void *payload;
};

/* Shared retained work owner used by construction, cloning, drawing and release.
   A non-NULL source borrows its resources; NULL owns the listed handles. */
struct effPCPAgiFireball_tag {
    f32 matrix[16];
    f32 pos_old[4];
    f32 pos[4];
    effPCPAgiFireballParam_t param;
    u32 frame;
    u32 count;
    u32 color;
    u32 mode;
    effPartHandle_t *headHandle;
    effPartHandle_t *handleA[5];
    effPartHandle_t **list[3];
    effPartHandle_t *handleB[5];
    effPartHandle_t *tailHandle;
    struct sdfMemHandle_t *alloc[3];
    struct effPCPAgiFireball_tag *source;
};

struct effPCP_ThunderPoly_t;

typedef struct effPCP_ThunderPoly_t effPCP_ThunderPoly_t;

/* Owner for a muzzle thunder ray and its fragment effect. */
struct effPCPEyeThunder_tag {
    f32 pos[4];                         /* 0x00 */
    u32 abgr;                            /* 0x10 */
    u8 frame[4];
    f32 start;                       /* 0x18 */
    f32 length;                               /* 0x1C */
    effPCP_ThunderPoly_t *pThunderPoly;    /* 0x20 */
};

/* Captured model-point history and its two parameter resources share this
 * general-heap allocation. Creation and cloning initialize the live tail and
 * vector words; neither operation copies the sampled arrays. */
struct effPCPMudo_tag {
    f32 samplePositions[25][7][4];
    u32 vectorWords[4];
    u32 animationFrames[25][7];
    f32 sampleScales[25][7];
    u32 sampleAges[25][7];
    u32 color;
    f32 scale;
    u32 historyCount;
    u32 updateCount;
    u32 baseColor;
    effPartHandle_t *secondaryHandle;
    effPartHandle_t *primaryHandle;
    struct sdfMemHandle_t *allocationHandle;
};

struct effResourceRectBounds_t {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

/* Passed directly to the quad renderer, independently of the extent/center. */
struct effResourceRectDrawParams_t {
    /* The movie blender stores packed RGBA; the renderer samples channels. */
    union { u8 color[4]; u32 rgba; };
    s32 blendControl;
    effResourceRectBounds_t bounds;
};

/* Nine copied words; the selected source texture belongs to the owner. */
typedef struct {
    s32 area;
    /* The native movie blender interpolates both center coordinates in one loop. */
    s32 center[2];
    effResourceRectDrawParams_t Filter;
} dds3Effect_PartialMultiFilterParam_t; /* 0x24 */

typedef struct {
    dds3Effect_PartialMultiFilterParam_t param;
    sdfTexHandle_t *hAlphaTex;
} dds3Effect_PartialMultiFilter_t; /* 0x28 */

/* Both compact-fade lifecycles use the same 0x3C chip allocation. The
 * resource union is selected by the short rectangle or long textured-blur
 * constructor and each path has its own matching release routine. */
/* The Exp filter/blur inputs are a fade timeline followed by the resource
 * block that is handed to the renderer unchanged. */
struct effPCPFilterExpParam_t {
    s32 life;
    s32 fade_in;
    s32 fade_out;
    effResourceRectDrawParams_t Filter;
};

struct effPCPFilterExp_tag {
    effPCPFilterExpParam_t param;
    u32 abgr;
    u32 abgr_base;
    s32 frame;
};

struct effPCPBlurExpParam_t {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    dds3Effect_Blur_t Blur;
};

struct effPCPBlurExp_tag {
    effPCPBlurExpParam_t param;
    u32 abgr;
    u32 abgr_base;
    s32 frame;
};

struct effPCPMultiBlurExp2_tag {
    f32 position[4];
    u8 flags;
    u8 pad11[3];
    u32 color;
    u32 baseColor;
    s32 frame;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    s32 startExtent;
    s32 endExtent;
    f32 area;
    union {
        dds3Effect_PartialMultiFilter_t *rectangle;
        dds3Effect_PartialMultiBlur_t *blur;
    } resource;
};

/* Serialized prefix shared by the compact rectangle/scatter/scale/blur inputs. */
struct effPCPMultiFilterExpParam_t {
    u8 type;
    u8 pad01[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 area_s;
    s32 area_e;
    dds3Effect_PartialMultiFilterParam_t res;
};

/* Phase-driven blur parameters and the scale-slot allocation they own. */
struct dds3Effect_RippleBlurParam_t {
    s32 number;
    f32 speed;
    f32 interval;
    u32 abgr;
    s32 blend; /* Copied to the slot quad blend control. */
    f32 rotate; /* Copied to the slot quad angle. */
    f32 power; /* Displacement amplitude used by the native update. */
    f32 frequency;
    /* Native movie blends index the renderer center as a two-element vector. */
    s32 center[2];
    s32 area;
};

struct effPCPRippleBlurExpParam_t {
    u8 type;
    u8 pad01[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 area_s;
    s32 area_e;
    dds3Effect_RippleBlurParam_t res;
};

/* Random-position blur parameters and their allocated slot owner. */
struct dds3Effect_DistortionBlurParam_t {
    s32 number;
    s32 interval;
    f32 speed;
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 distortion;
    /* Native movie blends index the renderer center as a two-element vector. */
    s32 center[2];
    s32 area;
    s32 width;
};

struct effPCPDistortionBlurExpParam_t {
    u8 type;
    u8 pad01[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 area_s;
    s32 area_e;
    dds3Effect_DistortionBlurParam_t DistBlur;
};

struct effPCPAnalyzeBED_tag {
    struct effBTL_t *hBED;
    struct effEveBEDCutHandle_t *hBEDCut;
    f32 scale;
    u32 abgr;
};

/* The FLEACH blur interpolates the two template bodies across the timeline. */
struct effPCPFLEACHBlurParam_t {
    dds3Effect_PartialMultiBlurParam_t start;
    dds3Effect_PartialMultiBlurParam_t end;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
};

struct effPCPFLEACHBlur_tag {
    effPCPFLEACHBlurParam_t param;
    dds3Effect_PartialMultiBlur_t *hMultiBlur;
    s32 frame;
    u32 abgr;
};

struct effPCPMultiBlurExp2Param_t {
    u8 type;
    u8 pad01[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 area_s;
    s32 area_e;
    dds3Effect_PartialMultiBlurParam_t MultiBlur;
};

struct effBlurScaleSlot_t {
    f32 phase;
    f32 angle;
    dds3Effect_Blur_t quad;
};

struct effBlurScaleWork_t {
    dds3Effect_RippleBlurParam_t params;
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocation;
    effBlurScaleSlot_t *slots;
};

struct effBlurScatterSlot_t {
    s32 delay;
    f32 angle;
    dds3Effect_Blur_t quad;
};

struct effBlurScatterWork_t {
    dds3Effect_DistortionBlurParam_t params;
    struct sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocation;
    effBlurScatterSlot_t *slots;
};

struct effPCPDistortionBlurExp_tag {
    f32 position[4];
    u8 type;
    u8 pad11[3];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    u32 abgr;
    u32 abgr_base;
    s32 area_s;
    s32 area_e;
    s32 frame;
    union {
        effBlurScatterWork_t *scatter;
        effBlurScaleWork_t *scale;
    } resource;
};

struct effPCPHamaon_tag {
    f32 pos[4];
    u32 color;
    f32 scale;
    effPartHandle_t *base; /* Parameter work for the anchor model. */
    effPartHandle_t *handle[4][3];
    u8 pad4C[0x60];
    u32 remainingDelay[4][3];
};

/* Six paired parameter handles share this 0x60-byte allocation. The first
 * three words are initialized by creation and cloning; their meanings remain
 * unknown. */
struct effPCPMHanma01_tag {
    f32 pos[4];
    u32 abgr;
    f32 scale;
    effPartHandle_t *handle[12];
    u32 remainingDelay[6];
};

struct effEveBEDCutHandle_t;

typedef struct effEveBEDCutHandle_t effEveBEDCutHandle_t;

/* Both PCP drift owners allocate count * 0x18 bytes after their 0x11C-byte
 * work header, initialize these rows, and release each retained event. */
struct effPCPRadiateBEDPart_tag {
    struct effEveBEDCutHandle_t *hBEDCut;
    s32 frame;
    f32 radius;
    f32 radius_s;
    f32 radian;
    f32 radian_s;
};

struct effPCPRadiateBEDParam_t {
    f32 position[4];
    u8 pad10[0x44];
    s32 duration;
    u32 count;
    s32 timing;
    s32 fadeIn;
    s32 fadeOut;
    f32 startPosition, endPosition, startJitter, endJitter;
    f32 angleStep;
    f32 angleDecay;
    f32 angleJitter;
    u8 pad84[0x80];
};

struct effBTL_t;

struct effBTLCut_t;

typedef struct effBTLCut_t effBTLCut_t;

typedef void (*effBTLCallback_t)(effBTLCut_t *effect);

struct effBTL_SkillPositionPtr_t {
    u8 target;
    u8 height;           /* 0x01 */
    u8 direction;     /* 0x02 */
    u8 dmy;
    s32 radius;    /* 0x04: zero uses the source range */
};

union effBTLPartValue_t {
    u32 sourceOffset;
    struct effPartHandle_t *work;
};

typedef struct effBTL_SkillPartsList_t {
    u16 type;                 /* 0x00 */
    u8 pad02[2];
    effBTLPartValue_t value; /* 0x04 */
    effBTL_SkillPositionPtr_t pos_ptr;        /* 0x08 */
    u32 timing;             /* 0x10 */
    u8 reverberations;                       /* 0x14 */
    u8 pad15[3];
} effBTL_SkillPartsList_t; /* 0x18 */

struct effBTL_SkillCutTable_t {
    s32 frame;              /* 0x00 */
    u8 pad04[2];
    u16 shake;                     /* 0x06: screen-shake amount; 31 or more logs and caps at 300 frames */
    u16 shakeFrame;                /* 0x08: frame at which the shake starts */
    u16 shakeFade;                 /* 0x0A: fade applied with the shake */
    u8 colorKeys[0x24];            /* 0x0C: colour keyframes read by the unit colour keyframe routine */
    u8 rgbKeys[0x18];              /* 0x30: RGB keyframes read by the unit RGB keyframe routine */
    s32 callbackFrame;             /* 0x48 */
    u8 pad4C[0x20];
    u16 count;                     /* 0x6C */
    u8 pad6E[2];
    s32 colorRampEndFrame;         /* 0x70 */
    u8 actorScaleEnabled;          /* 0x74 */
    u8 pad75[3];
    effBTL_SkillPartsList_t entries[1];     /* 0x78 */
};

/* One cut in effBTL_t's list. effBattle.h reads the same record without the
 * colour and flag words at +8, and neither reading is exercised here. */
struct effBTLCut_t {
    void *owner; /* 0x00 */
    effBTL_t *state; /* 0x04 */
    u32 color; /* 0x08 */
    u16 flags; /* 0x0C */
    u8 pad0E[2];
    u32 currentFrame; /* 0x10 */
    u32 triggerFrame; /* 0x14 */
    u32 colorRampEndFrame; /* 0x18 */
    u16 kind; /* 0x1C */
    u8 pad1E[2];
    s32 unk20;
    struct effBTL_SkillCutTable_t *list; /* 0x24 */
    struct effPartHandle_t *parameterWorks[0x3C]; /* 0x28 */
    effBTLCallback_t sourceFrameCallback; /* 0x118 */
    effBTLCallback_t triggerFrameCallback; /* 0x11C */
    u32 unk120;
    effBTLCut_t *next; /* 0x124 */
};

struct effBTLBank_t {
    u8 pad00[8];
    effBTL_SkillCutTable_t list;       /* 0x08 */
    u8 pad98[0x580];
};

struct effBTL_t {
    effBTLBank_t banks[2];
    u8 pad0C30[8];
    struct sdfMemHandle_t *resource; /* 0xC38: owns the mixer allocation. */
    u32 active;
    effBTLCut_t *voiceList;
};

struct effPCPRadiateBED_tag {
    effPCPRadiateBEDParam_t param;
    effPCPRadiateBEDPart_tag *pPart;
    effBTL_t *owner;
    u8 original;
    u8 pad10D[3];
    f32 scale;
    u32 color;
    struct sdfMemHandle_t *handle;
};

/* Serialized model placement and fade parameters copied into the owner. */
struct effPCPHelperBEDParam_t {
    f32 pos[4];
    s32 fade_in;
    s32 fade_out;
    f32 speed;
};

/* Misc clients use payload[0..6] as position and [7..10] as scale values;
 * Thunder callers retain the same layout with event-specific slots unknown. */
struct effEveBEDParams_t {
    f32 center[4];
    f32 rot[4];
    f32 r;
    f32 h;
    f32 scale;
    u32 abgr;
};

/* Alpha-system parameters supply three packed words to the particle system. */
/* The sphere-poly parameters, 0x4C bytes the constructors copy whole. The tail
 * is the thunder family's six fields: a width and a colour for the centre, the
 * border and the edge. The decomp read it twice, once for the widths and once
 * for the colours, which is why the two views agreed on everything before. */
struct effPCPThunderSpherePolyParam_t {
    f32 origin[4];
    u16 systemParam;
    u8 pad12[2];
    u32 cellCount;
    u8 pad18[4];
    f32 width;
    f32 height;
    f32 rotationScale;
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 perCell;
    u8 pad32[2];
    f32 width_center; /* 0x34: nothing reads it */
    u32 color_center;
    f32 width_border;
    u32 color_border;
    f32 width_edge;
    u32 color_edge;
};

/* Indexed-vector parameters retain a dispatch pointer and geometry widths. */
struct effPCPThunderSpherePolyPart_t {
    u32 wait;
    u32 refresh;
    f32 dir[3];
    f32 axis[3];
    f32 rotate;
    f32 radius;
    u32 abgr;
};

struct parSystem_t;

struct parCell_t {
    u128 *history;   /* 0x00 */
    u32 *colors;     /* 0x04: packed per-history-vector colors */
    s32 vertexCount; /* 0x08: active vectors in the topology-specific history */
    s32 historyAdvanceCountdown; /* 0x0C: updates until the next history shift */
    u32 color;       /* 0x10 initialized to grey 0x80808080 */
};

/* The cell-system allocation ends with this complete 0x2C-byte owner. */
struct parSystem_t {
    u16 kind;            /* 0x00: ParCellTopology code; native integer storage */
    u16 bucket;          /* 0x02: packet submission bucket */
    s32 cellCount;       /* 0x04 */
    s32 vertexWordCount; /* 0x08 */
    s32 groupDivisor;   /* 0x0C: cell-system allocator input */
    struct sdfMemHandle_t *allocation; /* 0x10: geometry allocation descriptor */
    parCell_t *cells;      /* 0x14 */
    u128 *vertices;      /* 0x18: backing vector history for all cells */
    u32 *colors;         /* 0x1C: backing color stream for all cells */
    struct sdfMat_t *asset; /* 0x20: submitted draw asset */
    struct parSystem_t *next; /* 0x24: pending cell-system list */
    s32 unk28;           /* 0x28 */
};

/* The complete work is followed by cellCount cells in the same allocation. */
struct effPCPThunderSpherePoly_t {
    effPCPThunderSpherePolyParam_t head;
    effPCPThunderSpherePolyPart_t *pPart;
    u32 abgr;
    f32 width_base;
    f32 height_base;
    struct parSystem_t *hPoly;
    struct sdfMemHandle_t *allocationHandle;
};

struct effPCPShockWave01_tag {
    u8 pos[0x10];
    f32 scale; /* Settable input; not read by the observed fade updates. */
    u32 abgr;
    u32 frame;
    effPCPThunderSpherePoly_t *pThunderPoly; /* 0x1C complete vector work owner */
};

/* One 0x18-byte per-group fragment entry in the owner's trailing allocation. */
struct effPCPRadiateThunderPart_tag {
    effPCP_ThunderPoly_t *pThunderPoly; /* 0x00 */
    s32 frame; /* 0x04: negative until the start delay expires */
    f32 radius; /* 0x08: initial interpolated radius */
    f32 radius_s; /* 0x0C: change per frame */
    f32 radian; /* 0x10: evenly spaced radian in radians */
    f32 radian_s;
};

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
struct effPCP_ThunderPolyParam_t {
    f32 start[4];
    f32 end[4];
    u16 blend;         /* 0x20 */
    u8 pad22[2];
    u32 number;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 split;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 color_center;     /* 0x40: packed center color used by symmetric cell ramps */
    f32 width_border;          /* 0x44: width of the continued strip */
    u32 color_border;    /* 0x48: packed middle color used by symmetric cell ramps */
    f32 width_edge;          /* 0x4C: added outer margin */
    u32 color_edge;      /* 0x50: packed edge color used by symmetric cell ramps */
};

/* Serialized 0x164-byte group-set input copied into each live owner. */
struct effPCPRadiateThunderParam_t {
    f32 origin[4];
    u8 pad10[0x40];
    u8 repeat; /* 0x50: restart after the duration. */
    u8 pad51[3];
    s32 life; /* 0x54: interpolation duration */
    u32 number; /* 0x58: handles per group */
    s32 timing; /* 0x5C: random initial delay */
    s32 fadeIn; /* 0x60 */
    s32 fadeOut; /* 0x64 */
    f32 radius_s; /* 0x68: interpolated position at frame zero */
    f32 radius_e; /* 0x6C: interpolated position at final frame */
    f32 radius_s_r; /* 0x70: fractional random variation */
    f32 radius_e_r; /* 0x74: fractional random variation */
    f32 radian_s;
    f32 angularDamping;
    f32 radian_r;
    f32 startRadialBlend;
    f32 endRadialBlend;
    u8 activeGroups[4];
    effPCP_ThunderPolyParam_t ThunderParam;
    u8 padE4[0x80];
};

/* The general allocation contains this 0x180-byte prefix followed by
 * count entries. The four duplicate-handle groups use a separate allocation. */
struct effPCPRadiateThunder_tag {
    effPCPRadiateThunderParam_t param;
    effPCPRadiateThunderPart_tag *pPart; /* 0x164: pPart at the end of the main allocation */
    f32 scale; /* 0x168: multiplies start/end positions */
    f32 height; /* Distance along the normalized fragment direction. */
    u32 abgr; /* 0x170 */
    effPartHandle_t **phAttach; /* 0x174: four groups of parameter work */
    struct sdfMemHandle_t *hAttachMem; /* 0x178 */
    struct sdfMemHandle_t *hMem; /* 0x17C */
};

struct effPCPThunderSphereExp_tag {
    f32 pos[4];
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 radius_s;
    s32 radius_e;
    u32 abgr;
    s32 frame;
    f32 scale;
    effPCPThunderSpherePoly_t *pThunderSpherePoly;
};

/* One event per model-map position. Init seeds the entry, then the updater
 * latches its initial position and advances the event through its frame span. */
struct effPCPHelperBEDPart_tag {
    f32 pos[4];
    u8 move;
    u8 pad11[3];
    s32 frame;
    s32 life;
    struct effEveBEDCutHandle_t *hBEDCut;
};

/* The factory allocates this owner as one 0x40-byte size-class block; the
 * per-position entries are a separate count * 0x20 allocation. */
struct effPCPHelperBED_tag {
    effPCPHelperBEDParam_t param;
    effPCPHelperBEDPart_tag *pPart;
    struct sdfMemHandle_t *hPartMem;
    struct effBTL_t *hBED;
    struct effPartHandle_t *hHelper;
    f32 scale;
    s32 frame;
    s32 frameLimit;
    u32 count;
    u32 color;
};

/* The paired PCP event owners allocate count * 0x20 bytes after the 0xA4
 * byte work header and initialize/release two event nodes per row. */
struct effPCPRadiateTBPart_tag {
    struct effPCP_ThunderPoly_t *pThunderPoly;
    struct effEveBEDCutHandle_t *eventA;
    struct effEveBEDCutHandle_t *eventB;
    f32 radian;
    f32 radius;
    f32 rotate;
    f32 rotate_s;
    s32 frame;
};

struct effPCPRadiateTBParam_t {
    f32 origin[4];
    u8 repeat;
    u8 pad11[3];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 baseRadius, radiusJitter, baseTiltStep, tiltJitter;
    effPCP_ThunderPolyParam_t fragmentParams;
};

struct effPCPRadiateTB_tag {
    effPCPRadiateTBParam_t param;
    effPCPRadiateTBPart_tag *pPart;
    effBTL_t *hBED, *ownerB;
    u8 original;
    u8 pad99[3];
    u32 abgr;
    struct sdfMemHandle_t *hMem;
};

struct effPCPAgiFireball2Param_t {
    s32 startFrame[3];
    effPCPAgiFireballParam_t group[3];
};

struct effPCPAgiFireball2_tag {
    effPCPAgiFireball2Param_t params;
    effPCPAgiFireball_tag *blockSets[3];
    s32 frame;
};

struct effPCPThunderSphereExpParam_t {
    s32 life;
    s32 fade_in;
    s32 fade_out;
    s32 radius_s;
    s32 radius_e;
    effPCPThunderSpherePolyParam_t ThunderSpherePoly;
};

/* Shared by ref-counted trail tasks; constructors allocate this full record. */
struct effPCPBlinkMultiBlurBase_tag {
    f32 pos[4];
    u32 abgr;
    u32 abgr_base;
    u32 frame;
    f32 area; /* 0x1C: converted to the rendered extent by the camera helper. */
    u32 blink;
    u32 blink_base; /* 0x24: original trail frame blink. */
    u32 blink_abgr[2];
    f32 ratio;
    s32 area_min;
    struct dds3Effect_PartialMultiBlur_t *hBlur;
};

struct effPCPBlinkMultiBlur_tag {
    u32 frame;
};

/* Serialized inputs copied in full into the orbiting aim-node work owner. */
struct effPCPBreathParam_t {
    f32 pos[4];
    u32 start;
    u32 life;
    s32 fade_out;
    f32 angle;
    f32 move;
};

/* Constructor allocation is 0x78 bytes and initializes this complete owner. */
struct effPCPBreath_tag {
    f32 matrix[16];
    effPCPBreathParam_t param;
    u32 abgr;
    u32 frame;
    f32 rotate;
    f32 speed;
    struct effHandle_t *hD3P;
};

/* Shared spawn-once work record used by the paired effect implementations. */
struct effPCPHwave01_tag {
    f32 pos[4]; /* 0x00: loaded together by the spawn callback's VU pos load. */
    u32 abgr; /* 0x10 */
    effPartHandle_t *hPB; /* 0x14 */
    effPartHandle_t *hD3P; /* 0x18 */
};

/* Spawn-range owners allocate count * 0x20 bytes after their 0xAC-byte header. */
struct effPCPSpiralBEDPart_tag {
    struct effEveBEDCutHandle_t *hBEDCut;
    s32 frame;
    f32 up;
    f32 up_s;
    f32 rotate;
    f32 rotate_s;
    f32 radius;
    f32 radius_s;
};

/* The radial event setup reads the typed tail and preserves its opaque prefix. */
struct effPCPSpiralBEDParam_t {
    f32 origin[4];
    u8 pad10[0x44];
    s32 duration;
    u32 count;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialAngularStep;
    f32 angularJitter;
    f32 angularDamping;
    f32 initialHeightStep;
    f32 heightJitter;
    f32 heightDamping;
    f32 startPosition;
    f32 endPosition;
    f32 startJitter;
    f32 endJitter;
    f32 startHeightRange;
};

/* The 0xAC-byte owner precedes its count * 0x20 event rows in one allocation. */
struct effPCPSpiralBED_tag {
    effPCPSpiralBEDParam_t param;
    effPCPSpiralBEDPart_tag *pPart;
    struct effBTL_t *hBED;
    u8 original;
    u8 pad9D[3];
    f32 scale;
    u32 color;
    struct sdfMemHandle_t *handle;
};

/* The pair variants allocate through handle1 at +0x64 (0x68 bytes); the
 * single variants allocate the 0x64-byte prefix and only use handle0. The EE
 * ABI rounds sizeof this u128-aligned view up to 0x70. */
struct effPCPBufuIce_tag {
    u8 pad00[0x10];
    u128 matrix[4];
    s32 frame;
    f32 angle;
    f32 scale;
    u32 color;
    effPartHandle_t *handle0;
    effPartHandle_t *handle1;
};

typedef struct effPCPMakka_tag {
    f32 pos[4];       /* 0x00 vector loaded by the pulse updater */
    f32 scale;        /* 0x10 */
    u32 abgr;        /* 0x14 */
    u32 number;        /* 0x18 particle number */
    s32 frame;        /* 0x1C current update frame */
    s32 startFrame[10]; /* 0x20 */
    f32 rotationY[10];  /* 0x48 random start angles */
    effPartHandle_t *handle[10];   /* 0x70 */
} effPCPMakka_tag; /* 0x98 */

/* Eight delayed pairs share this size-class allocation. The input-position
 * callback copies all four vector lanes; the update uses the first three. */
struct effPCPHama02_tag {
    f32 position[4];
    u32 color;
    f32 scale;
    f32 verticalOffset[8];
    effPartHandle_t *handle[16];
    u32 remainingDelay[8];
};

struct effPCPZiodin01_tag {
    f32 pos[4];
    u32 radius;
    f32 scale; /* Settable, but not read by the observed group updates. */
    u32 abgr;
    effPCP_ThunderPoly_t *handles[1];
};

struct effPCPBlinkMultiBlurParam_t {
    u32 count;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    dds3Effect_PartialMultiBlurParam_t res;
};

struct effPCPMultiBlurExp_tag {
    f32 pos[4];
    u32 abgr;
    u32 abgr_base;
    u32 frame;
    f32 area;
    u32 blink;
    u32 blink_abgr[2];
    f32 ratio; /* multiplies the computed trail size */
    s32 area_min;
    dds3Effect_PartialMultiBlur_t *hBlur;
};

struct effPCPAgiraoFireParam_t {
    f32 origin[4];
    f32 offset[3][2];
    s32 groupStartFrame[3];
    s32 handleDelay[7];
};

struct effPCPAgiraoFire_tag {
    effPCPAgiraoFireParam_t head;
    s32 frame;
    u32 color;
    struct effHandle_t *handles[21];
};

/* The twin effect owns eight pairs of parameter works, one shared work per
 * slot, and per-slot state/countdowns. Its constructors allocate 0xBC bytes. */
struct effPCPHama01_tag {
    f32 pos[4];
    u32 abgr;
    u32 frame;
    f32 scale;
    effPartHandle_t *pair[8][2];
    effPartHandle_t *shared[8];
    u32 state[8];
    u32 counter[8];
};

/* Four ordered 32-bit words in the shared effect random-number state. */
struct effRandState_t {
    u32 words[4];
};

struct dds3ModelHandle_t;

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

struct sdfMotionPlayBook_t;

/* Serialized clip duration/reserved remain halfwords (tools/fld.py).
 * The viewer MAXFRAME diagnostic reads their packed little-endian word. */
struct sdfMotionCutDef_t {
    union {
        struct {
            u16 frameLength;
            u16 pad;
        };
        u32 packedHeader;
    };
    u32 chunk[1];
};

struct sdfMotionNodeDef_t {
    union {
        u32 typeId;
        struct {
            /* Runtime dispatch shifts the command word right by 16. */
            u16 selector;
            u16 family;
        } binding;
    };
    u32 nodeId;
};

/* Resource header followed by the binding commands used to construct a motion. */
struct sdfMotionPlayBook_t {
    u16 numOfCut; /* Motion-index upper bound returned by mdlGetNumOfCut. */
    u16 numOfNode;
    sdfMotionCutDef_t **cutList;
    sdfMotionNodeDef_t nodeDefList[1];
};

struct dds3ModelResrcPlayBook_t {
    s32 flags;
    s16 motionSlotIndex;
    s16 motionIndex;
    sdfMotionPlayBook_t *motionTable;
    struct sdfMemHandle_t *resourceHandle;
};

struct sdfItemArray_t;

struct sdfItemArray_t {
    struct sdfMemHandle_t *memHandle;
    s16 count;
    s16 capacity;
    s16 elemSize;
    s16 growStep;
    void *items;
};

/* Relative-linked viewer records share packed payload and parameter words.
 * Kinds 1/2 use the count and part-index halfwords at 0x0C/0x0E; kind 3
 * reads the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
struct pibChunkItem_t {
    s32 type;       /* 0x00: 0xFFFF terminates the record chain */
    s32 size; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    union {
        u32 word10; /* 0x10: type-four stream parameter */
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
};

struct sdfModelDef_t;

struct sdfModelExtraChunk_t;

/* One serialized model extra chunk: the id and size, then the payload words
 * the chunk kind defines. sdfModelDef_t.extraData points at the first. */
typedef struct sdfModelExtraChunk_t {
    u32 chunkId; /* 0x00: zero terminates the chunk stream */
    u32 size;    /* 0x04: byte extent to the next chunk */
    u32 data0;
    u32 data1;
    s32 sPos;
} sdfModelExtraChunk_t;

/* Native model constructor reference: AMB roots and the FLD model wrapper's
 * four-word subrecord. The list has a variable-length body of node
 * definitions following its header. */
typedef struct sdfModelNodeDefList_t sdfModelNodeDefList_t;

/* Per-node definition after packed-resource or scene relocation, applied by
 * sdfModelNodeBaseSet. The shape pointers are relocated in place. */
struct sdfModelNodeDef_t {
    u16 format;       /* 0x00: selects the shape-pointer form */
    u16 pad0;         /* 0x02 */
    u32 pad1;         /* 0x04 */
    s32 id;           /* 0x08 */
    s32 parentIndex;  /* 0x0C: negative means this node has no parent */
    sdfVector4_t rotate; /* 0x10: Euler angles supplied to the quaternion builder */
    u128 pos;         /* 0x20 */
    u128 scale;       /* 0x30 */
    const f32 *bounds; /* 0x40: optional two xyz corners used by clipping */
    union {
        struct sdfShapeHeader_t *shape[3]; /* format 0: one header per ot slot */
        struct {
            struct sdfShapeHeader_t **table; /* format 1: null-terminated table */
            u32 unk48;
            u32 unk4C;
        } indirect;
    } shapes;         /* 0x44: other formats leave the tail unused */
};

/* Variable-length serialized list: a 0x10-byte header, followed by count
 * complete 0x50-byte node definitions. */
struct sdfModelNodeDefList_t {
    s32 count;
    u8 pad[0x0C];
};

struct sdfModelDef_t {
    sdfModelNodeDefList_t *modelDef; /* 0x00 */
    u32 *matDef_New; /* 0x04: count followed by flag-driven asset records */
    s32 morphCount; /* 0x08: passed to sdfModelMakeMorphList */
    struct sdfModelExtraChunk_t *extraData; /* 0x0C: borrowed by SdfModel */
};

/* Group owners allocate 0xB4 bytes and retain eight resource records. */
struct dds3ModelResrc_t {
    struct dds3ModelResrc_t *next;
    struct dds3ModelResrc_t *prev;
    u16 group;
    u16 type;
    u8 ownsResources;
    u8 pad0D[3];
    dds3ModelHandle_t *modelContext;
    sdfItemArray_t *resourceList;
    struct sdfModelDef_t *itemList;
    struct sdfMemHandle_t *requestAllocation;
    dds3ModelResrcPlayBook_t slots[8];
    struct sdfMemHandle_t *resourceHandle;
    struct pibChunkItem_t *partInfo; /* Borrowed relative-linked record-list view. */
    sdfItemArray_t *partList;
    f32 unk_AC;
    f32 objectRadius; /* Copied into the model context and object radius. */
};

struct sdfModelRootNode_t;

/* Copy runtime model and draw-node state into an existing model overlay.
 * Both models and their resource and draw-node lists must be initialized. */
struct sdfModelRootNode_t;

struct sdfMotionCtrlr_t;

/* SDK motions are allocated as 0x34-byte records; model slots retain these nodes. */
struct sdfMotionCtrlr_t {
    struct sdfMotionCtrlr_t *next;
    sdfModelRootNode_t *owner;
    sdfMotionPlayBook_t *motionTable;
    s32 playbookMemHandle;
    sdfItemArray_t *request;
    f32 blendDurationFrames;
    f32 blendStartFrame;
    f32 currentFrame;
    f32 frameStep;
    s32 userData;
    s16 searchId;
    s16 slotIndex;
    u16 motionIndex;
    u16 frameCount;
    u8 state;
    u8 previousState;
    u8 loopEnabled;
    u8 pad33;
};

/* Command-list node: allocation owns the node and trailing command data.
 * A null descriptor identifies a separately chip-allocated node. */
struct sdfModelPacketHeader_tag {
    struct sdfModelPacketHeader_tag *next;
    u8 kind;
    s8 packetSelector;
    s16 quadwordCount;
    void *payload; /* Packet bytes for kind 1; serialized commands for kinds 2/3. */
    struct sdfMemHandle_t *allocation;
};

/* Allocated as 0x100-byte nodes; each owns a circular list of child draw nodes. */
struct sdfModelNode_t {
    struct sdfModelNode_t *prev; /* 0x00 */
    struct sdfModelNode_t *next;     /* 0x04 */
    struct sdfModelNode_t *parent;   /* 0x08 */
    struct sdfModelNode_t *children; /* 0x0C */
    struct sdfModelRootNode_t *root;        /* 0x10: backlink installed by sdfModelNodeAdd */
    u16 flags;                   /* 0x14 */
    s16 drawAttr;                   /* 0x16: constructor sentinel is -1 */
    s32 id;                   /* 0x18: lookup key when model flags bit 0 is set */
    u32 color;                   /* 0x1C */
    u8 drawPacketMemHandle[8];
    sdfModelPacketHeader_tag *drawPacket[2];     /* 0x28: command drawPacket for both buffered frames */
    u8 *tmPacket; /* Two 0x80-byte frame packets, allocated on first use. */                  /* 0x30 */
    const f32 *bounds;     /* 0x34: optional two xyz corners used by clipping */
    void *shape;             /* 0x38: item this node was built from */
    u32 pad0;
    void *userData;
    u32 pad1[3];
    f32 rotate[4];           /* 0x50 */
    f32 pos[4];          /* 0x60 */
    f32 scale[4];                /* 0x70 */
    f32 localNoScaledTm[4][4];       /* 0x80: all four local basis/pos rows */
    f32 tm[4][4];       /* 0xC0: local transform composed with its parent */
};

/* The buffered-transform constructor allocates and clears all 0x9C bytes. */
struct sdfModelRootNode_t {
    sdfItemArray_t *list;
    sdfModelNode_t *rootNode;
    void *assetData;
    sdfItemArray_t *resources;
    sdfItemArray_t *slotPairs;
    struct sdfMotionCtrlr_t *motionList;
    u8 complexNodeId;
    u8 flags;
    u16 drawFlags;
    u32 color;
    f32 matrix[4][4];
    f32 rotationQuaternion[4];
    f32 scaleVector[4];
    sdfDmaTag_t *lighting; /* Borrowed first DMA reference prefix. */
    sdfDmaTag_t *secondaryLighting; /* Borrowed second DMA reference prefix. */
    f32 scalarOverrideFirst;
    f32 scalarOverrideSecond;
    struct sdfModelExtraChunk_t *chunkTable; /* Borrowed from the model resource data. */

    f32 modelScale;
    u8 lodIndex; /* Native model-viewer and MODEL_LOD_CHG setters use byte 0x98. */
    u8 pad99[3];
};

struct dds3ModelBlurInstance_t {
    struct dds3ModelBlurInstance_t *next;
    sdfModelRootNode_t *modelobj;
    s32 keepCount;
};

/* This owner has inline color interpolation data, not a request buffer at +0xC. */
struct dds3ModelBlurCb_t {
    dds3ModelBlurInstance_t *instants;
    s16 spawnCount;
    u16 count;
    s16 interval;
    s16 intervalCount;
    s32 keepCount;
    f32 colorBase[4];
    f32 colorDelta[4];
};

struct mdlTrackWork_t;

struct dds3HermitianTrackPoly_tag;

/* Owner record stored after the track's point and gradient buffers. */
struct dds3HermitianTrackPoly_tag {
    u32 blend;            /* 0x00: draw-target selector */
    u32 abgr;             /* 0x04 */
    s32 point;            /* 0x08: slots, including the reserved pair */
    s32 count;            /* 0x0C: of those, the live ones */
    s32 current;          /* 0x10 */
    s32 split;            /* 0x14 */
    u128 *pVertex;        /* 0x18 */
    u32 *pVC;             /* 0x1C: gradient entries after pVertex */
    sdfMat_t *pMat;       /* 0x20: owned draw asset */
    sdfMemHandle_t *hMem; /* 0x24: containing allocation */
};

/* Thirteen copied words, followed by an update counter and owned history. */
struct effTrackPolyParams_t {
    struct dds3ModelHandle_t *model;
    s32 idA;
    s32 idB;
    f32 startFrame;
    f32 endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
};

struct mdlTrackWork_t {
    effTrackPolyParams_t params;
    u32 updateCount;
    struct dds3HermitianTrackPoly_tag *data;
};

struct mdlEffectResrc_IPU_t;

/* IPU register state saved across stream callbacks (0x24 bytes). */
struct sdfIpuDmaRegs_t {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
};

struct movieStream_t;

struct sdfFileCmdHandle_t;

/* Protocol IDs use distinct byte-sized storage codes in DevState. */
typedef u8 SdfDevOperationCode;

typedef s8 SdfDevStateCode;

/* Complete 0x40-byte owner allocated by sdfFileCmdNewHandle. */
struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next; /* 0x00 */
    struct sdfFileCmdHandle_t *prev; /* 0x04 */
    struct sdfFileCmdHandle_t *deviceNext; /* 0x08 */
    struct sdfFileCmdHandle_t *devicePrev; /* 0x0C */
    char *filename; /* 0x10 */
    u8 device; /* 0x14 */
    SdfDevOperationCode cmd; /* 0x15 */
    SdfDevStateCode stat; /* 0x16 */
    u8 phase; /* 0x17 */
    s32 offset; /* 0x18 */
    s32 length; /* 0x1C */
    void *buffer; /* 0x20 */
    s32 cmdParm; /* 0x24 */
    s32 fd; /* 0x28 */
    s32 errorCode; /* 0x2C */
    s32 currentOffset; /* 0x30 */
    u8 remainder[4]; /* 0x34 */
    void (*callback)(struct sdfFileCmdHandle_t *, s32, saddr_t, s32, s32); /* 0x38 */
    s32 userData; /* 0x3C: opaque callback-context word */
};

/* The +0x1C movie work pointer selects one of two separately allocated records.
 * isPac at +3 chooses the active arm; the variant payload types live in
 * sdf_movie_stream.h. */
struct sdfMovieStrmLinear_t;

/* Chip-heap work record for the linear movie stream (0x14 bytes). */
struct sdfMovieStrmLinear_t {
    struct sdfMemHandle_t *allocation;
    u8 *bufferStart;
    u8 *readCursor;
    u8 *writeCursor;
    s32 bufferedBytes;
};

struct sdfMovieStrmPac_t;

/* Chip-heap work record for movie-PAC streaming (0x78 bytes). The
 * block-mask allocation and the backing ring allocation are separate owners. */
struct sdfMovieStrmPac_t {
    u8 pad00[0x18];
    s32 packetBytes;
    s32 blockBytes;
    u8 scratchBuffer[0x20];
    struct sdfMemHandle_t *payloadAllocation;
    u8 *blockMask;
    s32 blockIndex;
    struct sdfMemHandle_t *allocation;
    u8 *pacInputBuffer;
    u8 *pacBuffer;
    s32 pacReadOffset;
    s32 pacBufferedBytes;
    u8 *ringBuffer;
    s32 ringOffset;
    s32 ringLength;
    s32 unk6C;
    s32 scratchSize;
    u8 *scratch;
};

union movieDecodeCtx_t {
    struct sdfMovieStrmLinear_t *linear;
    struct sdfMovieStrmPac_t *pac;
};

struct sdfIpuDecodeCb_t {
    struct sdfIpuDecodeCb_t *prev;
    struct sdfIpuDecodeCb_t *streamNext;
    struct sdfIpuDecodeCb_t *next;
    u8 active;
    u8 queueLinked;
    u8 ipuCycleInitialized; /* First-use IPU setup has run for this playback cycle. */
    u8 drained;
    u8 playbackPhase;
    u8 inputFeedEnabled; /* Restart input feeding after DMA completion while input remains. */
    u8 inputDmaStartPending;
    u8 imageTrans; /* Queued output submissions not yet retired by a playback update. */
    u8 rgbMode;
    u8 loopMode;
    u8 fps;
    u8 playbackCadenceRemainder;
    u8 bufferIndex;
    u8 getBuffId;
    u8 activeBuffCount; /* Completed output buffers waiting for playback submission. */
    u8 pad1B;
    s32 buffSize;
    void *frameBuffers[2];
    void *transferPacketBuffers[2];
    u32 transferPacketBytes;
    s32 resourceWord;
    sdfGsMemSlice_t *textureHead;
    u16 width;
    u16 height;
    u32 frameCount;
    u32 tickCount;
    s32 frame;
    struct sdfIpuDmaTag_t *inputDmaChain;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u8 *scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct sdfIpuDecodeCb_t *, struct movieStream_t *, s32, void *, s32);
    struct movieStream_t *strmCallbackParm; /* Owner context passed to the read callback. */
    u8 inputFeedDmaInFlight;
    u8 strmToIpuDmaStep;
    u8 pad66[2];
    sdfIpuDmaRegs_t dma;
};

struct movieStream_t {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    struct sdfFileCmdHandle_t *file;
    s32 totalBytes;
    s32 remainingBytes;
    movieDecodeCtx_t stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    sdfIpuDecodeCb_t soundNode;
};

/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
struct sdfIpuDmaTag_t;

/* One 16-byte tag in the IPU input DMA chain. */
/* One tag in the to-IPU chain. Written as two quadword halves rather than by
 * field, which is what the single 64-bit store in sdfIpuSetStreamChunk needs. */
struct sdfIpuDmaTag_t {
    u64 control;
    u64 reserved;
};

/* The model-object allocation ends with its complete embedded stream node. */
struct mdlEffectResrc_IPU_t {
    u8 *sourceAddress; /* 0x00: frame data passed to sdfIpuSetDecodeParams */
    sdfMemHandle_t *backingAllocation; /* 0x04: released by mdlDeleteEffectResrc_IPU */
    u8 inUse; /* 0x08: claimed by a model resource item */
    u8 initialized; /* 0x09: stream-node initialization guard */
    u8 pad0A[6];
    s32 sourceSize; /* 0x10 */
    u8 pad14[0x0C];
    sdfIpuDecodeCb_t soundNode; /* 0x20: complete 0x8C embedded node */
};

struct mdlIpuAttachment_t {
    dds3ModelHandle_t *owner;
    struct mdlEffectResrc_IPU_t *object; /* Borrowed from the owner's heterogeneous part list. */
    struct sdfTexHandle_t *texture; /* Borrowed texture-list node used for stream creation. */
    s32 minimumTime;
    u8 attributes[8];
};

struct mdlEffectResrcItem_t;

/* One model-viewer part-list slot; the request buffer uses a 0x10-byte stride. */
struct mdlEffectResrcItem_t {
    u32 type;       /* 0x00 */
    s32 count;      /* 0x04 */
    void *handle;   /* 0x08 */
    u8 opaque0C[4]; /* 0x0C */
};

struct mdlMarkPos_t;

struct mdlMarkPos_t {
    u32 nodeId;      /* 0x00: draw-node lookup key */
    s32 id;          /* 0x04: map-position record lookup key */
    u8 reserved08[8];
    u128 position;   /* 0x10 */
    u128 up;         /* 0x20 */
    u128 direction;  /* 0x30 */
};

/* The paired viewer allocators clear 0x20 bytes and prepend this tagged item
 * to dds3ModelHandle_t's list at +0x14. The part and object payloads share that allocation. */
struct dds3ModelEffectCtrlr_t {
    struct dds3ModelEffectCtrlr_t *next;
    u16 type; /* dds3ModelEffectType value, with native halfword storage. */
    s16 motionSlotIndex; /* Owning Motion.slotIndex; -1 tags context-wide entries. */
    union {
        struct {
            /* Type 2 stores a track; other part kinds store their created instance. */
            union {
                void *instance;
                struct mdlTrackWork_t *track;
            };
            struct mdlEffectResrcItem_t *slot;
            struct mdlMarkPos_t *mapPositionRecord;
            f32 anchorScale;
            u8 pad18[8];
        } part;
        mdlIpuAttachment_t object;
    } payload;
};

/* Context constructors allocate 0x38 bytes; their SDK model slot lives at +0x18. */
struct dds3ModelHandle_t {
    u32 flags;
    dds3ModelHandle_t *next;
    dds3ModelHandle_t *previous;
    dds3ModelResrc_t *sub;
    union {
        u32 word;
        struct {
            s16 id;
            s16 motionIndex; /* Entry index passed to sdfMotionChangeBlend. */
        } h;
    } current;
    dds3ModelEffectCtrlr_t *resourceItems;
    sdfModelRootNode_t *modelRoot;
    sdfMotionCtrlr_t *first;
    sdfMotionCtrlr_t *slots[4];
    dds3ModelBlurCb_t *devList;
    f32 objectRadius; /* Propagated to the rebuilt object's modelRoot radius. */
};

extern void dds3BillboardDraw(billHandle_t *hData);

void dds3BillboardSetMode(struct billHandle_t *hBillboard, s32 mode);

void dds3BillboardSetRGBA(struct billHandle_t *hBillboard, u32 abgr);

void dds3BillboardSetScale(struct billHandle_t *hBillboard, f32 x, f32 y);

struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
};

extern effDebugPolyParams_t dds3RingPolyExpListPolyParam;

extern sdfGrOt_t *dds3RingPolyExpListBlendOt[];

extern u8 dds3RingPolyExpListParameters[];

/* Factories accept the serialized parameter prefix used by effect callbacks. */
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *pParam);

extern u8 effPCPEyeThunderParam04_0[];

extern u8 effPCPEyeThunderParam04_1[];

dds3Effect_PartialMultiBlur_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *pParam);

effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *pParam);

dds3Effect_PartialMultiFilter_t *dds3EffectPartialMultiFilterCreate(dds3Effect_PartialMultiFilterParam_t *pParam);

void dds3EffectPartialMultiFilterDelete(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);

void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *hDistortionBlur);

void dds3EffectRippleBlurDelete(effBlurScaleWork_t *hRippleBlur);

void dds3EffectFilter(effResourceRectDrawParams_t *params);

void dds3EffectBlur(dds3Effect_Blur_t *quad);

u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

effHandle_t *dds3EffectCopy(const effHandle_t *source);

extern effPCP_ThunderPolyParam_t effPCPShockWave02Param[];

/* Copy one 128-bit vector from the read-only source to destination storage. */
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);

void dds3EffectSetPosition(struct effHandle_t *pData, const void *pPos);

struct billHandle_t *dds3BillboardGeneral(s32 number);

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *parameters);

void effPCPThunderSpherePolyDelete(effPCPThunderSpherePoly_t *hData);

effPCPThunderSpherePolyParam_t *effPCPThunderSpherePolyGetParam(effPCPThunderSpherePoly_t *pThunderPoly);

void dds3BillboardDelete(struct billHandle_t *hData);

extern void *effPkt3DPrimMakePoly(effDebugPolyParams_t *aParm);

extern u8 effPCPEyeThunderParam01_0[];

extern u8 effPCPEyeThunderParam01_1[];

extern effRandState_t dds3EffectBase[];

void dds3EffectDelete(effHandle_t *pData);

void dds3EffectPartDelete(effPartHandle_t *hEffPart);

void dds3EffectPartialMultiBlurDraw(dds3Effect_PartialMultiBlur_t *owner);

/* Clone a mixer owner or release all of its active voices. */
struct effBTL_t *effEveBEDCreate(struct effBTL_t *pData);

/* The event constructor copies a serialized, caller-specific 0x30-byte
 * parameter record. Its retained work owner is opaque to external users. */
effEveBEDCutHandle_t *effEveBEDCutCreate(struct effBTL_t *hEveBED, u16 CutNum, const void *pParams);

void effEveBEDCutDelete(effEveBEDCutHandle_t *hEveBEDCut);

void effEveBEDDelete(struct effBTL_t *hEveBED);

extern u8 effPCP_ShockWave01_ThunderSpherePoly_Param[];

extern effPCP_ThunderPolyParam_t effPCPMZiodin01ThunderParam[];

extern u8 effPCP_Houden01_ThunderSpherePoly_Param[];

extern s32 effPCPBlurScreenOffset(f32 value);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);

extern u32 effMagaCylinderCalcColor(u32 flags, u32 color);

extern u8 effPCPEyeThunderParam00_0[];

extern effPCP_ThunderPolyParam_t effPCPZiodin02Param[];

extern u8 effPCPEyeThunderParam00_1[];

extern u8 effPCPEyeThunderParam05_0[];

extern u8 effPCPEyeThunderParam05_1[];

effPartHandle_t *dds3EffectPCPDataPartCreate(dds3EffectPCPData_t *hPCPData, u32 number);

/* Optional source and matrix operations retain their generic pointer domains. */
void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, void *pPos);

void dds3EffectPartSetScale(effPartHandle_t *hEffPart, f32 scale);

void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, void *pMatrix);

void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

void *dds3EffectPartGetHandle(effPartHandle_t *hEffPart);

void dds3EffectPartDraw(effPartHandle_t *hEffPart);

void effPCPAgiFireballSetColor(effPCPAgiFireball_tag *hData, u32 abgr);

extern void effPCPRingPolyExpSetVertex(f32 pPCPRingPolyExp, struct effPCPRingPolyExp_tag *radius);

void effPCPRingPolyExp2SetVertex(effPCPRingPolyExp2_tag *pPCPRingPolyExp2, f32 radius);

effPCPRingPolyExp_tag *effPCPRingPolyExpCreate(effPCPRingPolyExpParam_t *pParam);

effPCPRingPolyExp2_tag *effPCPRingPolyExp2Create(effPCPRingPolyExp2Param_t *hData);

void dds3RingPolyExpListDelete(dds3RingPolyExpList_tag *hData);

effPCPSpiralBED_tag *effPCPSpiralBEDCreate(effPCPSpiralBEDParam_t *pParam, void *pBEDData);

effPCPRadiateBED_tag *effPCPRadiateBEDCreate(effPCPRadiateBEDParam_t *pParam, void *pBEDData);

effPCPRadiateTB_tag *effPCPRadiateTBCreate(
    effPCPRadiateTBParam_t *pParam, void *pBEDData0, void *pBEDData1);

effPCPAnalyzeBED_tag *effPCPAnalyzeBEDCreate(void *pBEDData);

void effPCPAnalyzeInit(effPCPAnalyzeBED_tag *pAnalyzeBED);

effPCPFLEACHBlur_tag *effPCPFLEACHBlurCreate(effPCPFLEACHBlurParam_t *pFLEACHBlurParam);

void effPCPAgiFireballSetRotateMatrix(effPCPAgiFireball_tag *hData, const void *pMatrix);

void effPCPAgiFireballSetPosition(effPCPAgiFireball_tag *hData, const void *pPos);

effPCPAgiFireball_tag *effPCPAgiFireballCreate(
    const effPCPAgiFireballParam_t *pParam, const effPCPAgiFireballPartList_t *pPart);

effPCPRadiateThunder_tag *effPCPRadiateThunderCreate(const effPCPRadiateThunderParam_t *pParam, void *pData);

extern effPCPBlinkMultiBlurBase_tag *pBlinkMultiBlurBase;

effPCPBreath_tag *effPCPBreathCreate(void *params, effPCPAgiraoFirePartList_t *handleParams);

void dds3EffectPartialMultiBlurDelete(dds3Effect_PartialMultiBlur_t *hPartialMultiBlur);

extern u8 effPCP_Megidoraon01_ThunderSpherePoly_Param[];

void dds3EffectSetRGBA(effHandle_t *pData, u32 abgr);

extern effPCP_ThunderPolyParam_t effPCPZiodin01ThunderParam[];

extern u8 effPCP_Megidora01_ThunderSpherePoly_Param[];

extern effPCP_ThunderPolyParam_t effPCPMZiodin02ThunderParam[];

effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters);

/* Borrow the parameter block stored at the beginning of the owner. */
effPCP_ThunderPolyParam_t *effPCP_ThunderPolyGetParam(effPCP_ThunderPoly_t *pThunderPoly);

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData);

void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData);

void dds3EffectDraw(effHandle_t *pData);

u32 effBTLSkillCheckRun(void);

u8 *effBTLSkillGetTargetFormation(s32 number);

void mdlSetRotate(dds3ModelHandle_t *aHandle);

void sdfMotionNext(sdfMotionCtrlr_t *aCtrlr);

void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

void sdfModelLinkOt(struct sdfGrOt_t **aOt, sdfModelRootNode_t *aRoot);

/* Euler angles into the primary VU0 matrix bank. */
void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

/* vf10 = vf10 * vf11 through the primary matrix bank. */
void Mult(void);

extern const u8 effPCPMakkaUnitY[16];

/* The model root's node list, whose item array sits at 0x0C. */
typedef struct {
    u8 pHelper[0xC];
    sdfModelNode_t **PartList;
} effPCPAgiFireball2PartList_t;

u32 effBTLSkillCheckRun(void);

u8 *effBTLSkillGetTargetFormation(s32 number);

void effBTLModelGetSphereCenter(void *pFormation);

s32 effBTLModelGetID(void *pFormation);

void effBTLPartGetPositionPtr(void *pFormation, effBTL_SkillPositionPtr_t *pPosPtr, f32 *pPos);

extern effPCP_ThunderPolyParam_t effPCPZiodin03Param[];

extern u8 effPCPEyeThunderParam02_0[];

extern u8 effPCPEyeThunderParam02_1[];

extern effPCP_ThunderPolyParam_t effPCPZiodin04Param[];

extern u8 effPCPEyeThunderParam03_0[];

extern u8 effPCPEyeThunderParam03_1[];

/* Motion-record selection and lookup. */
void mdlSingleMotion(dds3ModelHandle_t *aHandle, s32 aGroup, s32 aNumber);

void mdlSetColor(dds3ModelHandle_t *aHandle, u32 aColor);

void mdlLink(dds3ModelHandle_t *aHandle, struct sdfGrOt_t **aModelOt);

void mdlSetPosition(dds3ModelHandle_t *aHandle);

extern void *memcpy(void *, const void *, size_t);

extern void *memset(void *a, int b, size_t c);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void *sdfChipAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. */
void MultMatrixNoArg(void);

void effBTLFieldLightDirectionSet(const f32 *pDir);

void effBTLFieldLightAxisSet(const f32 *pAxis);

void effBTLFieldLightAnimationSet(s32 frame, f32 angle);

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

u32 sdfModelGetMarkPosCount(const struct sdfModelRootNode_t *aRoot);

struct sdfMat_t *sdfMatNew(void);

void sdfMatFreeDelayed(sdfMat_t *aMat);

f32 sdfCos(f32 aRadius);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);

f32 sdfSin(f32 aRadius);

extern u8 aViewTarget[];

extern u8 aViewEye[];

extern u8 aViewDown[];

/* Read vector inputs; VU0 forms first minus second and stores second in vf31. */
void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 aAngle);

/* Load a Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixZ(f32 aAngle);

extern void SetRotateMatrixY(f32 aAngle);

extern void SetRotateMatrixX_B(f32 aAngle);

extern void SetRotateMatrixZ_B(f32 aAngle);

static inline void ShiftThunderHandles(effPCPZiodin01_tag *work, s32 count) {
    u128 saved[2];
    u8 *obj;
    s32 i;

    for (i = 0; i < count; i++) {
        obj = (u8 *)effPCP_ThunderPolyGetParam(work->handles[i]);
        SDF_VU0_LOADVECTOR(obj + 0x10);
        SDF_VU0_STOREVECTOR(&saved[1]);
        SDF_VU0_LOADVECTOR_B(work);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(obj + 0x10);
        SDF_VU0_LOADVECTOR(obj);
        SDF_VU0_STOREVECTOR(&saved[0]);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(obj);
        effPCP_ThunderPolyDraw(work->handles[i]);
        SDF_VU0_COPYVECTOR(obj, &saved[0]);
        SDF_VU0_COPYVECTOR(obj + 0x10, &saved[1]);
    }
}

/* Dependencies for the restored DDS effect implementations. */
/* Three selectable heads precede the shared block-set inputs. */
typedef struct {
    void *head[3];
    effPCPAgiFireballPartList_t selected;
} effPCPBlockSetInputs_t;

extern u16 effPCPMudoSampleStep[8];

extern f32 effPCPMudoSampleScale[8];

extern const f32 effPCPRadiateTBParam[4] __attribute__((aligned(16)));

void ReverseMatrix(void);

void effScreenOffset(void);

effHandle_t *dds3EffectLoad(effPCPAgiraoFirePartList_t *pD3P);

void effPCPThunderSpherePolySetColor(effPCPThunderSpherePoly_t *hData, u32 abgr);

void effPCPThunderSpherePolySetPosition(void *hData, const void *pPos);

void effPCPThunderSpherePolySetScale(effPCPThunderSpherePoly_t *hData, f32 scale);

void effPCPThunderSpherePolyDraw(effPCPThunderSpherePoly_t *hData);

void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);

void dds3EffectRippleBlurReset(effBlurScaleWork_t *hRippleBlur);

void dds3EffectRippleBlurDraw(effBlurScaleWork_t *work);

void mdlSetScale(dds3ModelHandle_t *aHandle);

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);

void effPCP_ThunderPolySetColor(effPCP_ThunderPoly_t *hData, u32 abgr);

void dds3EffectPartialMultiFilterDraw(dds3Effect_PartialMultiFilter_t *hPartialMultiFilter);

void SetRotateMatrixY_B(f32 aAngle);

void effEveBEDCutSetScale(effEveBEDCutHandle_t *hEveBEDCut, f32 scale);

void effEveBEDCutSetParams(effEveBEDCutHandle_t *hEveBEDCut, const void *pParams);

void effEveBEDCutDraw(effEveBEDCutHandle_t *hEveBEDCut);

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* Both compact fade renderers interpolate their extent with integer truncation. */
static inline s32 InterpolateCompactExtent(s32 from, s32 to,
    s32 frame, s32 duration) {
    to -= from;
    to = (s32)((f32)to * (f32)frame / (f32)duration);
    return from + to;
}

effPCPSparkleSphere_tag *effPCPSparkleSphereCreate(effPCPSparkleSphereParam_t *params);

void effPCPSparkleSphereDelete(effPCPSparkleSphere_tag *hData);

void effPCPSparkleSphereDraw(effPCPSparkleSphere_tag *hData);

void effPCPSparkleSphereSetPosition(effPCPSparkleSphere_tag *hData, const void *pPos);

void effPCPSparkleSphereSetColor(effPCPSparkleSphere_tag *hData, u32 abgr);

void effPCPSparkleSphereSetArea(effPCPSparkleSphere_tag *hData, f32 area);

void effPCPHama01PartInit(effPCPHama01_tag *pPCPHama01, s32 number);

effPCPHama01_tag *effPCPHama01Create(void *src);

void effPCPHama01Delete(effPCPHama01_tag *hData);

effPCPHama01_tag *effPCPHama01Copy(effPCPHama01_tag *src);

void effPCPHama01Draw(effPCPHama01_tag *hData);

void effPCPHama01SetPosition(effPCPHama01_tag *hData, const void *pPos);

void effPCPHama01SetScale(effPCPHama01_tag *hData, f32 scale);

void effPCPHama01SetColor(effPCPHama01_tag *hData, u32 abgr);

void effPCPHama02PartInit(effPCPHama02_tag *pPCPHama02, s32 number);

effPCPHama02_tag *effPCPHama02Create(void *args);

void effPCPHama02Delete(effPCPHama02_tag *hData);

effPCPHama02_tag *effPCPHama02Copy(effPCPHama02_tag *source);

void effPCPHama02Draw(effPCPHama02_tag *hData);

void effPCPHama02SetPosition(effPCPHama02_tag *hData, const void *pPos);

void effPCPHama02SetScale(effPCPHama02_tag *hData, f32 scale);

void effPCPHama02SetColor(effPCPHama02_tag *hData, u32 abgr);

void effPCPHamaonPartInit(effPCPHamaon_tag *pPCPHamaon, u32 kind, u32 attach);

effPCPHamaon_tag *effPCPHamaonCreate(void *src);

void effPCPHamaonDelete(effPCPHamaon_tag *hData);

effPCPHamaon_tag *effPCPHamaonCopy(effPCPHamaon_tag *src);

void effPCPHamaonDraw(effPCPHamaon_tag *hData);

void effPCPHamaonSetPosition(effPCPHamaon_tag *hData, const void *pPos);

void effPCPHamaonSetScale(effPCPHamaon_tag *hData, f32 scale);

void effPCPHamaonSetColor(effPCPHamaon_tag *hData, u32 abgr);

void effPCPMHanma01PartInit(effPCPMHanma01_tag *pPCPMHanma01, s32 number);

effPCPMHanma01_tag *effPCPMHanma01Create(void *source);

void effPCPMHanma01Delete(effPCPMHanma01_tag *hData);

effPCPMHanma01_tag *effPCPMHanma01Copy(effPCPMHanma01_tag *source);

void effPCPMHanma01Draw(effPCPMHanma01_tag *hData);

void effPCPMHanma01SetPosition(effPCPMHanma01_tag *hData, const void *pPos);

void effPCPMHanma01SetScale(effPCPMHanma01_tag *hData, f32 scale);

void effPCPMHanma01SetColor(effPCPMHanma01_tag *hData, u32 abgr);

void effPCPMudoPartInit(effPCPMudo_tag *pPCPMudo);

effPCPMudo_tag *effPCPMudoCreate(void *source);

void effPCPMudoDelete(effPCPMudo_tag *hData);

effPCPMudo_tag *effPCPMudoCopy(effPCPMudo_tag *source);

void effPCPMudoDraw(effPCPMudo_tag *hData);

void effPCPMudoSetPosition(effPCPMudo_tag *hData, const void *pPos);

void effPCPMudoSetScale(effPCPMudo_tag *hData, f32 scale);

void effPCPMudoSetColor(effPCPMudo_tag *hData, u32 abgr);

effPCPHwave01_tag *effPCPHwave01Create(void *src);

void effPCPHwave01Delete(effPCPHwave01_tag *hData);

effPCPHwave01_tag *effPCPHwave01Copy(effPCPHwave01_tag *src);

void effPCPHwave01Draw(effPCPHwave01_tag *hData);

void effPCPHwave01SetPosition(effPCPHwave01_tag *hData, const void *pPos);

void effPCPHwave01SetColor(effPCPHwave01_tag *hData, u32 abgr);

void effPCPZiodin01Init(effPCPZiodin01_tag *pPCPZiodin01);

void *effPCPZiodin01Create(void);

void effPCPZiodin01Delete(effPCPZiodin01_tag *hData);

void *effPCPZiodin01Copy(void);

void effPCPZiodin01Draw(effPCPZiodin01_tag *hData);

void effPCPZiodin01SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPZiodin01SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPZiodin01SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPZiodin02Init(effPCPZiodin01_tag *pPCPZiodin02);

void *effPCPZiodin02Create(void);

void effPCPZiodin02Delete(effPCPZiodin01_tag *hData);

void *effPCPZiodin02Copy(void);

void effPCPZiodin02Draw(effPCPZiodin01_tag *hData);

void effPCPZiodin02SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPZiodin02SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPZiodin02SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPZiodin03Init(effPCPZiodin01_tag *pPCPZiodin03);

void *effPCPZiodin03Create(void);

void effPCPZiodin03Delete(effPCPZiodin01_tag *hData);

void *effPCPZiodin03Copy(void);

void effPCPZiodin03Draw(effPCPZiodin01_tag *hData);

void effPCPZiodin03SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPZiodin03SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPZiodin03SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPZiodin04Init(effPCPZiodin01_tag *pPCPZiodin04);

void *effPCPZiodin04Create(void);

void effPCPZiodin04Delete(effPCPZiodin01_tag *hData);

void *effPCPZiodin04Copy(void);

void effPCPZiodin04Draw(effPCPZiodin01_tag *hData);

void effPCPZiodin04SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPZiodin04SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPZiodin04SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPMZiodin01Init(effPCPZiodin01_tag *pPCPMZiodin01);

void *effPCPMZiodin01Create(void);

void effPCPMZiodin01Delete(effPCPZiodin01_tag *hData);

void *effPCPMZiodin01Copy(void);

void effPCPMZiodin01Draw(effPCPZiodin01_tag *hData);

void effPCPMZiodin01SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPMZiodin01SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPMZiodin01SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPMZiodin02Init(effPCPZiodin01_tag *pPCPMZiodin02);

void *effPCPMZiodin02Create(void);

void effPCPMZiodin02Delete(effPCPZiodin01_tag *hData);

void *effPCPMZiodin02Copy(void);

void effPCPMZiodin02Draw(effPCPZiodin01_tag *hData);

void effPCPMZiodin02SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPMZiodin02SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPMZiodin02SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPShockWave01Init(effPCPShockWave01_tag *pPCPShockWave01);

void *effPCPShockWave01Create(void);

void effPCPShockWave01Delete(effPCPShockWave01_tag *hData);

void *effPCPShockWave01Copy(void);

void effPCPShockWave01Draw(effPCPShockWave01_tag *hData);

void effPCPShockWave01SetPosition(effPCPShockWave01_tag *hData, const void *pPos);

void effPCPShockWave01SetScale(effPCPShockWave01_tag *hData, f32 scale);

void effPCPShockWave01SetColor(effPCPShockWave01_tag *hData, u32 abgr);

void effPCPShockWave02Init(effPCPZiodin01_tag *pPCPShockWave02);

void *effPCPShockWave02Create(void);

void effPCPShockWave02Delete(effPCPZiodin01_tag *hData);

void *effPCPShockWave02Copy(void);

void effPCPShockWave02Draw(effPCPZiodin01_tag *hData);

void effPCPShockWave02SetPosition(effPCPZiodin01_tag *hData, const void *pPos);

void effPCPShockWave02SetScale(effPCPZiodin01_tag *hData, f32 scale);

void effPCPShockWave02SetColor(effPCPZiodin01_tag *hData, u32 abgr);

void effPCPHouden01Init(effPCPShockWave01_tag *pPCPHouden01);

void *effPCPHouden01Create(void);

void effPCPHouden01Delete(effPCPShockWave01_tag *hData);

void *effPCPHouden01Copy(void);

void effPCPHouden01Draw(effPCPShockWave01_tag *hData);

void effPCPHouden01SetPosition(effPCPShockWave01_tag *hData, const void *pPos);

void effPCPHouden01SetScale(effPCPShockWave01_tag *hData, f32 scale);

void effPCPHouden01SetColor(effPCPShockWave01_tag *hData, u32 abgr);

effPCPBlinkMultiBlur_tag *effPCPBlinkMultiBlurCreate(effPCPBlinkMultiBlurParam_t *params);

void effPCPBlinkMultiBlurDelete(effPCPBlinkMultiBlur_tag *hData);

void effPCPBlinkMultiBlurDraw();

void effPCPBlinkMultiBlurSetPosition(effPCPBlinkMultiBlur_tag *hData, const void *pPos);

void effPCPBlinkMultiBlurSetColor(effPCPBlinkMultiBlur_tag *hData, u32 abgr);

void effPCPBlinkMultiBlurSetArea(effPCPBlinkMultiBlur_tag *hData, f32 area);

effPCPMultiBlurExp_tag *effPCPMultiBlurExpCreate(effPCPBlinkMultiBlurParam_t *params);

void effPCPMultiBlurExpDelete(effPCPMultiBlurExp_tag *hData);

void effPCPMultiBlurExpDraw(effPCPMultiBlurExp_tag *hData);

void effPCPMultiBlurExpSetPosition(effPCPMultiBlurExp_tag *hData, const void *pPos);

void effPCPMultiBlurExpSetColor(effPCPMultiBlurExp_tag *hData, u32 abgr);

void effPCPMultiBlurExpSetArea(effPCPMultiBlurExp_tag *hData, f32 area);

effPCPMultiBlurExp2_tag *effPCPMultiFilterExpCreate(effPCPMultiFilterExpParam_t *params);

effPCPMultiBlurExp2_tag *effPCPMultiFilterExpCreatePCF(void *data);

effPCPMultiBlurExp2_tag *effPCPMultiFilterExpCopy(effPCPMultiBlurExp2_tag *work);

void effPCPMultiFilterExpDelete(effPCPMultiBlurExp2_tag *hData);

void effPCPMultiFilterExpDraw(effPCPMultiBlurExp2_tag *hData);

void effPCPMultiFilterExpSetPosition(effPCPMultiBlurExp2_tag *hData, const void *pPos);

void effPCPMultiFilterExpSetColor(effPCPMultiBlurExp2_tag *hData, u32 abgr);

void effPCPMultiFilterExpSetArea(effPCPMultiBlurExp2_tag *hData, f32 area);

effPCPMultiBlurExp2_tag *effPCPMultiBlurExp2Create(effPCPMultiBlurExp2Param_t *pParam);

effPCPMultiBlurExp2_tag *effPCPMultiBlurExp2CreatePCF(void *data);

effPCPMultiBlurExp2_tag *effPCPMultiBlurExp2Copy(effPCPMultiBlurExp2_tag *work);

void effPCPMultiBlurExp2Delete(effPCPMultiBlurExp2_tag *hData);

void effPCPMultiBlurExp2Draw(effPCPMultiBlurExp2_tag *hData);

void effPCPMultiBlurExp2SetPosition(effPCPMultiBlurExp2_tag *hData, const void *pPos);

void effPCPMultiBlurExp2SetColor(effPCPMultiBlurExp2_tag *hData, u32 abgr);

void effPCPMultiBlurExp2SetArea(effPCPMultiBlurExp2_tag *hData, f32 area);

effPCPFilterExp_tag *effPCPFilterExpCreate(effPCPFilterExpParam_t *pParam);

effPCPFilterExp_tag *effPCPFilterExpCreatePCF(void *data);

effPCPFilterExp_tag *effPCPFilterExpCopy(effPCPFilterExp_tag *work);

void effPCPFilterExpDelete(effPCPFilterExp_tag *hData);

void effPCPFilterExpDraw(effPCPFilterExp_tag *hData);

void effPCPFilterExpSetColor(effPCPFilterExp_tag *hData, u32 abgr);

effPCPBlurExp_tag *effPCPBlurExpCreate(effPCPBlurExpParam_t *pParam);

effPCPBlurExp_tag *effPCPBlurExpCreatePCF(void *data);

effPCPBlurExp_tag *effPCPBlurExpCopy(effPCPBlurExp_tag *work);

void effPCPBlurExpDelete(effPCPBlurExp_tag *hData);

void effPCPBlurExpDraw(effPCPBlurExp_tag *hData);

void effPCPBlurExpSetColor(effPCPBlurExp_tag *hData, u32 abgr);

effPCPDistortionBlurExp_tag *effPCPDistortionBlurExpCreate(effPCPDistortionBlurExpParam_t *pParam);

effPCPDistortionBlurExp_tag *effPCPDistortionBlurExpCreatePCF(void *data);

effPCPDistortionBlurExp_tag *effPCPDistortionBlurExpCopy(effPCPDistortionBlurExp_tag *work);

void effPCPDistortionBlurExpDelete(effPCPDistortionBlurExp_tag *hData);

void effPCPDistortionBlurExpDraw(effPCPDistortionBlurExp_tag *hData);

void effPCPDistortionBlurExpSetPosition(effPCPDistortionBlurExp_tag *hData, const void *pPos);

void effPCPDistortionBlurExpSetColor(effPCPDistortionBlurExp_tag *hData, u32 abgr);

effPCPDistortionBlurExp_tag *effPCPRippleBlurExpCreate(effPCPRippleBlurExpParam_t *pParam);

effPCPDistortionBlurExp_tag *effPCPRippleBlurExpCreatePCF(void *data);

effPCPDistortionBlurExp_tag *effPCPRippleBlurExpCopy(effPCPDistortionBlurExp_tag *work);

void effPCPRippleBlurExpDelete(effPCPDistortionBlurExp_tag *hData);

void effPCPRippleBlurExpDraw(effPCPDistortionBlurExp_tag *hData);

void effPCPRippleBlurExpSetPosition(effPCPDistortionBlurExp_tag *hData, const void *pPos);

void effPCPRippleBlurExpSetColor(effPCPDistortionBlurExp_tag *hData, u32 abgr);

void effPCPMegidora01Init(effPCPShockWave01_tag *pPCPMegidora01);

void *effPCPMegidora01Create(void);

void effPCPMegidora01Delete(effPCPShockWave01_tag *hData);

void *effPCPMegidora01Copy(void);

void effPCPMegidora01Draw(effPCPShockWave01_tag *hData);

void effPCPMegidora01SetPosition(effPCPShockWave01_tag *hData, const void *pPos);

void effPCPMegidora01SetScale(effPCPShockWave01_tag *hData, f32 scale);

void effPCPMegidora01SetColor(effPCPShockWave01_tag *hData, u32 abgr);

void effPCPMegidoraon01Init(effPCPShockWave01_tag *pPCPMegidoraon01);

void *effPCPMegidoraon01Create(void);

void effPCPMegidoraon01Delete(effPCPShockWave01_tag *hData);

void *effPCPMegidoraon01Copy(void);

void effPCPMegidoraon01Draw(effPCPShockWave01_tag *hData);

void effPCPMegidoraon01SetPosition(effPCPShockWave01_tag *hData, const void *pPos);

void effPCPMegidoraon01SetScale(effPCPShockWave01_tag *hData, f32 scale);

void effPCPMegidoraon01SetColor(effPCPShockWave01_tag *hData, u32 abgr);

effPCPBreath_tag *effPCPBreathCreate(void *params, effPCPAgiraoFirePartList_t *handleParams);

effPCPBreath_tag *effPCPBreathCreatePCF(void *data);

effPCPBreath_tag *effPCPBreathCopy(effPCPBreath_tag *work);

void effPCPBreathDelete(effPCPBreath_tag *hData);

extern void *effBTLSkillGetInvokeFormation(void);

extern u32 effBTLSkillGetTargetParty(void);

extern void effBTLModelGroupGetCenter(u32 group);

extern f32 sdfAtan2(f32 x, f32 y);

extern void dds3EffectSetRotateMatrix(effHandle_t *pData, const void *pMatrix);

/* Center the orbit on the first battle frame, then position and fade its node. */
void effPCPBreathDraw(effPCPBreath_tag *work);

void effPCPBreathSetPosition(effPCPBreath_tag *hData, const void *pPos);

void effPCPBreathSetColor(effPCPBreath_tag *hData, u32 abgr);

/* vu0 routine: turn the source matrix by half a turn around Y */
void effPCPBreathSetRotateMatrix(effPCPBreath_tag *hData, const void *pMatrix);

effPCPAgiraoFire_tag *effPCPAgiraoFireCreate(effPCPAgiraoFireParam_t *pParam, effPCPAgiraoFirePartList_t **pPartList);

effPCPAgiraoFire_tag *effPCPAgiraoFireCreatePCF(void *data);

effPCPAgiraoFire_tag *effPCPAgiraoFireCopy(effPCPAgiraoFire_tag *src);

void effPCPAgiraoFireDelete(effPCPAgiraoFire_tag *hData);

/* vu0 routine: place three staggered groups in the camera-relative basis. */
void effPCPAgiraoFireDraw(effPCPAgiraoFire_tag *hData);

void effPCPAgiraoFireSetPosition(effPCPAgiraoFire_tag *hData, const void *pPos);

void effPCPAgiraoFireSetColor(effPCPAgiraoFire_tag *hData, u32 abgr);

effPCPAgiFireball_tag *effPCPAgiFireballCreate(
    const effPCPAgiFireballParam_t *pParam, const effPCPAgiFireballPartList_t *pPart);

effPCPAgiFireball_tag *effPCPAgiFireballCreatePCF(void *parameterTable);

void effPCPAgiFireballCopySub(effPCPAgiFireball_tag *pPCPAgiFireball, effPCPAgiFireball_tag *pPCPAgiFireballSrc);

effPCPAgiFireball_tag *effPCPAgiFireballCopy(effPCPAgiFireball_tag *src);

void effPCPAgiFireballDelete(effPCPAgiFireball_tag *hData);

/* Native look-at up vector: (0, 1, 0, 0). */
extern f32 effPCPAgiFireballParam[4];

/* Update the head, attached nodes and randomized three-channel trail. */
void effPCPAgiFireballDraw(effPCPAgiFireball_tag *work);

void effPCPAgiFireballSetPosition(effPCPAgiFireball_tag *hData, const void *pPos);

void effPCPAgiFireballSetColor(effPCPAgiFireball_tag *hData, u32 abgr);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPAgiFireballSetRotateMatrix(effPCPAgiFireball_tag *hData, const void *pMatrix);

effPCPAgiFireball_tag *effPCPAgiFireballSpCreatePCF(void *parameterTable);

effPCPAgiFireball_tag *effPCPAgiFireballSpCopy(effPCPAgiFireball_tag *src);

effPCPAgiFireball_tag *effPCPAgiFireballSp2CreatePCF(void *parameterTable);

effPCPAgiFireball_tag *effPCPAgiFireballSp2Copy(effPCPAgiFireball_tag *src);

effPCPAgiFireball2_tag *effPCPAgiFireball2Create(effPCPAgiFireball2Param_t *pAFB2Param, effPCPBlockSetInputs_t *pAFB2Part);

effPCPAgiFireball2_tag *effPCPAgiFireball2CreatePCF(void *args);

effPCPAgiFireball2_tag *effPCPAgiFireball2Copy(effPCPAgiFireball2_tag *src);

void effPCPAgiFireball2Delete(effPCPAgiFireball2_tag *hData);

void effPCPAgiFireball2Draw(effPCPAgiFireball2_tag *hData);

void effPCPAgiFireball2SetPosition(effPCPAgiFireball2_tag *hData, const void *pPos);

void effPCPAgiFireball2SetColor(effPCPAgiFireball2_tag *hData, u32 abgr);

void effPCPAgiFireball2SetRotateMatrix(effPCPAgiFireball2_tag *hData, const void *pMatrix);

effPCPBufuIce_tag *effPCPBufuIceCreate(void *src);

effPCPBufuIce_tag *effPCPBufuIceCopy(effPCPBufuIce_tag *src);

void effPCPBufuIceDelete(effPCPBufuIce_tag *hData);

void effPCPBufuIceDraw(effPCPBufuIce_tag *hData);

void effPCPBufuIceSetPosition(effPCPBufuIce_tag *hData, const void *pPos);

void effPCPBufuIceSetColor(effPCPBufuIce_tag *hData, u32 abgr);

void effPCPBufuIceSetScale(effPCPBufuIce_tag *hData, f32 scale);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPBufuIceSetRotateMatrix(effPCPBufuIce_tag *hData, const void *pMatrix);

effPCPBufuIce_tag *effPCPBufuraIceCreate(void *src);

effPCPBufuIce_tag *effPCPBufuraIceCopy(effPCPBufuIce_tag *src);

void effPCPBufuraIceDelete(effPCPBufuIce_tag *hData);

void effPCPBufuraIceDraw(effPCPBufuIce_tag *hData);

void effPCPBufuraIceSetPosition(effPCPBufuIce_tag *hData, const void *pPos);

void effPCPBufuraIceSetColor(effPCPBufuIce_tag *hData, u32 abgr);

void effPCPBufuraIceSetScale(effPCPBufuIce_tag *hData, f32 scale);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPBufuraIceSetRotateMatrix(effPCPBufuIce_tag *hData, const void *pMatrix);

effPCPBufuIce_tag *effPCPBufudnIceCreate(void *src);

effPCPBufuIce_tag *effPCPBufudnIceCopy(effPCPBufuIce_tag *src);

void effPCPBufudnIceDelete(effPCPBufuIce_tag *hData);

void effPCPBufudnIceDraw(effPCPBufuIce_tag *hData);

void effPCPBufudnIceSetPosition(effPCPBufuIce_tag *hData, const void *pPos);

void effPCPBufudnIceSetColor(effPCPBufuIce_tag *hData, u32 abgr);

void effPCPBufudnIceSetScale(effPCPBufuIce_tag *hData, f32 scale);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPBufudnIceSetRotateMatrix(effPCPBufuIce_tag *hData, const void *pMatrix);

void *effPCPBufuLightCreate(void *params);

void *effPCPBufuLightCopy(void *work);

void effPCPBufuLightDelete(void);

void effPCPBufuLightDraw(void *hData);

effPCPEyeThunder_tag *effPCPEyeThunderCreate00_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy00_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate00_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy00_1(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate01_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy01_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate01_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy01_1(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate02_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy02_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate02_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy02_1(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate03_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy03_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate03_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy03_1(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate04_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy04_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate04_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy04_1(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate05_0(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy05_0(void);

effPCPEyeThunder_tag *effPCPEyeThunderCreate05_1(u32 unused);

effPCPEyeThunder_tag *effPCPEyeThunderCopy05_1(void);

void effPCPEyeThunderDelete(effPCPEyeThunder_tag *hData);

void effPCPEyeThunderDraw(effPCPEyeThunder_tag *hData);

void effPCPEyeThunderSetPosition(effPCPEyeThunder_tag *hData, const void *pPos);

void effPCPEyeThunderSetColor(effPCPEyeThunder_tag *hData, u32 abgr);

effPCPThunderSphereExp_tag *effPCPThunderSphereExpCreate(effPCPThunderSphereExpParam_t *pParam);

effPCPThunderSphereExp_tag *effPCPThunderSphereExpCreatePCF(void *data);

effPCPThunderSphereExp_tag *effPCPThunderSphereExpCopy(effPCPThunderSphereExp_tag *work);

void effPCPThunderSphereExpDelete(effPCPThunderSphereExp_tag *hData);

void effPCPThunderSphereExpDraw(effPCPThunderSphereExp_tag *hData);

void effPCPThunderSphereExpSetPosition(effPCPThunderSphereExp_tag *hData, const void *pPos);

void effPCPThunderSphereExpSetColor(effPCPThunderSphereExp_tag *hData, u32 abgr);

void effPCPThunderSphereExpSetScale(effPCPThunderSphereExp_tag *hData, f32 scale);

/* vu0 routine: initialize both transforms with the libvu0 identity primitive. */
dds3RingPolyExpList_tag *dds3RingPolyExpListCreate(u32 face);

void dds3RingPolyExpListDelete(dds3RingPolyExpList_tag *hData);

/* vu0 routine: compose the beam transform and submit batches of its vertices. */
void dds3RingPolyExpListDraw(dds3RingPolyExpList_tag *hData);

void effPCPRingPolyExpSetVertex(f32 pPCPRingPolyExp, effPCPRingPolyExp_tag *radius);

void effPCPRingPolyExpInit(effPCPRingPolyExp_tag *pPCPRingPolyExp);

effPCPRingPolyExp_tag *effPCPRingPolyExpCreate(effPCPRingPolyExpParam_t *pParam);

effPCPRingPolyExp_tag *effPCPRingPolyExpCreatePCF(void *data);

effPCPRingPolyExp_tag *effPCPRingPolyExpCopy(effPCPRingPolyExp_tag *work);

void effPCPRingPolyExpDelete(effPCPRingPolyExp_tag *hData);

void effPCPRingPolyExpDraw(effPCPRingPolyExp_tag *hData);

void effPCPRingPolyExpSetPosition(effPCPRingPolyExp_tag *hData, const void *pPos);

void effPCPRingPolyExpSetScale(effPCPRingPolyExp_tag *hData, f32 scale);

void effPCPRingPolyExpSetColor(effPCPRingPolyExp_tag *hData, u32 abgr);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPRingPolyExpSetRotateMatrix(effPCPRingPolyExp_tag *hData, const void *pMatrix);

void effPCPRingPolyExpSetDirection(effPCPRingPolyExp_tag *hData, void *pMatrix);

void effPCPRingPolyExp2SetVertex(effPCPRingPolyExp2_tag *pPCPRingPolyExp2, f32 radius);

void effPCPRingPolyExp2Init(effPCPRingPolyExp2_tag *pPCPRingPolyExp2);

effPCPRingPolyExp2_tag *effPCPRingPolyExp2Create(effPCPRingPolyExp2Param_t *hData);

effPCPRingPolyExp2_tag *effPCPRingPolyExp2CreatePCF(void *data);

effPCPRingPolyExp2_tag *effPCPRingPolyExp2Copy(effPCPRingPolyExp2_tag *work);

void effPCPRingPolyExp2Delete(effPCPRingPolyExp2_tag *hData);

void effPCPRingPolyExp2Draw(effPCPRingPolyExp2_tag *hData);

void effPCPRingPolyExp2SetPosition(effPCPRingPolyExp2_tag *hData, const void *pPos);

void effPCPRingPolyExp2SetScale(effPCPRingPolyExp2_tag *hData, f32 scale);

void effPCPRingPolyExp2SetColor(effPCPRingPolyExp2_tag *hData, u32 abgr);

/* vu0 routine: copy the rotation matrix through vf28-vf31 */
void effPCPRingPolyExp2SetRotateMatrix(effPCPRingPolyExp2_tag *hData, const void *pMatrix);

effPCPRadiateThunder_tag *effPCPRadiateThunderCreate(const effPCPRadiateThunderParam_t *pParam, void *pData);

effPCPRadiateThunder_tag *effPCPRadiateThunderCreatePCF(void *args);

effPCPRadiateThunder_tag *effPCPRadiateThunderCopy(effPCPRadiateThunder_tag *work);

void effPCPRadiateThunderDelete(effPCPRadiateThunder_tag *hData);

void effPCPRadiateThunderPartInit(effPCPRadiateThunder_tag *pRadiateThunder, s32 index);

void effPCPRadiateThunderDraw(effPCPRadiateThunder_tag *hData);

void effPCPRadiateThunderDraw(effPCPRadiateThunder_tag *work);

void effPCPRadiateThunderSetPosition(effPCPRadiateThunder_tag *hData, const void *pPos);

void effPCPRadiateThunderSetScale(effPCPRadiateThunder_tag *hData, f32 scale);

void effPCPRadiateThunderSetColor(effPCPRadiateThunder_tag *hData, u32 abgr);

effPCPMakka_tag *effPCPMakkaCreat(void *src);

effPCPMakka_tag *effPCPMakkaCopy(effPCPMakka_tag *src);

void effPCPMakkaDelete(effPCPMakka_tag *hData);

void effPCPMakkaUpdateNode(sdfModelNode_t *node);

void effPCPMakkaDraw(effPCPMakka_tag *hData);

void effPCPMakkaSetPosition(effPCPMakka_tag *hData, const void *pPos);

void effPCPMakkaSetColor(effPCPMakka_tag *hData, u32 abgr);

void effPCPMakkaSetScale(effPCPMakka_tag *hData, f32 scale);

effPCPRadiateBED_tag *effPCPRadiateBEDCreate(effPCPRadiateBEDParam_t *pParam, void *pBEDData);

effPCPRadiateBED_tag *effPCPRadiateBEDCreatePCF(void *data);

effPCPRadiateBED_tag *effPCPRadiateBEDCopy(effPCPRadiateBED_tag *src);

void effPCPRadiateBEDDelete(effPCPRadiateBED_tag *hData);

void effPCPRadiateBEDPartInit(effPCPRadiateBED_tag *pRadiateBED, s32 index);

void effPCPRadiateBEDDraw(effPCPRadiateBED_tag *hData);

void effPCPRadiateBEDSetPosition(effPCPRadiateBED_tag *hData, const void *pPos);

void effPCPRadiateBEDSetScale(effPCPRadiateBED_tag *hData, f32 scale);

void effPCPRadiateBEDSetColor(effPCPRadiateBED_tag *hData, u32 abgr);

effPCPRadiateTB_tag *effPCPRadiateTBCreate(effPCPRadiateTBParam_t *pParam, void *pBEDData0, void *pBEDData1);

effPCPRadiateTB_tag *effPCPRadiateTBCreatePCF(void *data);

effPCPRadiateTB_tag *effPCPRadiateTBCopy(effPCPRadiateTB_tag *src);

void effPCPRadiateTBDelete(effPCPRadiateTB_tag *hData);

void effPCPRadiateTBPartInit(effPCPRadiateTB_tag *pRadiateTB, s32 index);

void effPCPRadiateTBDraw(effPCPRadiateTB_tag *hData);

void effPCPRadiateTBSetPosition(effPCPRadiateTB_tag *hData, const void *pPos);

void effPCPRadiateTBSetColor(effPCPRadiateTB_tag *hData, u32 abgr);

effPCPSpiralBED_tag *effPCPSpiralBEDCreate(effPCPSpiralBEDParam_t *pParam, void *pBEDData);

effPCPSpiralBED_tag *effPCPSpiralBEDCreatePCF(void *data);

effPCPSpiralBED_tag *effPCPSpiralBEDCopy(effPCPSpiralBED_tag *src);

void effPCPSpiralBEDDelete(effPCPSpiralBED_tag *hData);

void effPCPSpiralBEDPartInit(effPCPSpiralBED_tag *pSpiralBED, s32 index);

void effPCPSpiralBEDDraw(effPCPSpiralBED_tag *hData);

void effPCPSpiralBEDSetPosition(effPCPSpiralBED_tag *hData, const void *pPos);

void effPCPSpiralBEDSetScale(effPCPSpiralBED_tag *hData, f32 scale);

void effPCPSpiralBEDSetColor(effPCPSpiralBED_tag *hData, u32 abgr);

void effPCPHelperInit(effPCPHelperBED_tag *pHelperBED);

effPCPHelperBED_tag *effPCPHelperBEDCreate(effPCPHelperBEDParam_t *pParam, void *pPBData, void *pBEDData);

effPCPHelperBED_tag *effPCPHelperBEDCreatePCF(void *data);

effPCPHelperBED_tag *effPCPHelperBEDCopy(effPCPHelperBED_tag *src);

void effPCPHelperBEDDelete(effPCPHelperBED_tag *hData);

void effPCPHelperBEDDraw(effPCPHelperBED_tag *hData);

void effPCPHelperBEDSetPosition(effPCPHelperBED_tag *hData, const void *pPos);

void effPCPHelperBEDSetScale(effPCPHelperBED_tag *hData, f32 scale);

void effPCPHelperBEDSetColor(effPCPHelperBED_tag *hData, u32 abgr);

void *effPCPHelperBEDGetModel(effPCPHelperBED_tag *pHelperBED);

void effPCPAnalyzeInit(effPCPAnalyzeBED_tag *pAnalyzeBED);

effPCPAnalyzeBED_tag *effPCPAnalyzeBEDCreate(void *pBEDData);

effPCPAnalyzeBED_tag *effPCPAnalyzeBEDCreatePCF(void *data);

effPCPAnalyzeBED_tag *effPCPAnalyzeBEDCopy(effPCPAnalyzeBED_tag *src);

void effPCPAnalyzeBEDDelete(effPCPAnalyzeBED_tag *hData);

void effPCPAnalyzeBEDDraw(effPCPAnalyzeBED_tag *hData);

void effPCPAnalyzeBEDSetScale(effPCPAnalyzeBED_tag *hData, f32 scale);

void effPCPAnalyzeBEDSetColor(effPCPAnalyzeBED_tag *hData, u32 abgr);

effPCPFLEACHBlur_tag *effPCPFLEACHBlurCreate(effPCPFLEACHBlurParam_t *pFLEACHBlurParam);

effPCPFLEACHBlur_tag *effPCPFLEACHBlurCreatePCF(void *data);

effPCPFLEACHBlur_tag *effPCPFLEACHBlurCopy(effPCPFLEACHBlur_tag *work);

void effPCPFLEACHBlurDelete(effPCPFLEACHBlur_tag *hData);

extern void dds3EffectPartialMultiBlurSetParam(dds3Effect_PartialMultiBlur_t *work,
    const dds3Effect_PartialMultiBlurParam_t *param);

void effPCPFLEACHBlurDraw(effPCPFLEACHBlur_tag *work);

void effPCPFLEACHBlurSetColor(effPCPFLEACHBlur_tag *hData, u32 abgr);

#endif
