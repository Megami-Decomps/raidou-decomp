#include "common.h"
#include "raidou_alloc.h"
#include "libsdf.h"
#include "effBattle.h"
#include "event.h"
#include "model.h"
#include "ps2asm.h"
#include "effDebug.h"
#include "sdfFrame.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "sdfChip.h"
#include "sdfVu0.h"
#include "sdfMath.h"
#include "sdfQuat.h"
#include "effBlur_Filter.h"
#include "effMath.h"
#include "effEvent.h"
#include "billManager.h"

typedef struct effEveBEDParams_t {
    f32 center[4];
    f32 rot[4];
    f32 r;
    f32 h;
    f32 scale;
    u32 abgr;
} effEveBEDParams_t;

/* The constructor reserves 0x38 bytes; only its init prefix is serialized. */
typedef struct effEveBEDCut_t {
    effEveBEDParams_t init;
    evtUnit_t *actor;
    effBTLCut_t *effect;
} effEveBEDCut_t;

typedef struct effSimpleBED_t {
    effBTL_t *hBED;
    effEveBEDCut_t *hCut;
    effEveBEDParams_t pParams;
    u8 original;
    u8 unk39[3];
} effSimpleBED_t;

/* Serialized records permit unaligned sources and destinations. */
typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) effEveBEDRecord_t;

extern void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut);

#define EFF_EVENT_NEUTRAL_COLOR 0x80808080

#define EFF_EVENT_BLUR_SOURCE_BYTES 0x28

#define EFF_EVENT_BLUR_RESOURCE_INDEX 2

#define EFF_EVENT_STAGGERED_RESOURCE_INDEX 3

#define EFF_EVENT_SQUARE_RESOURCE_INDEX 0

#define EFF_EVENT_STAGGERED_SLOT_COUNT 4

#define EFF_EVENT_VECTOR_COMPONENTS 4

#define EFF_EVENT_HALF_TURN 3.14159265f

#define EFF_EVENT_JITTER_RANGE 0.3f

#define EFF_EVENT_JITTER_BASE 0.7f

#define EFF_EVENT_RANDOM_MIDPOINT 0.5f

#define EFF_CHANNEL_ALPHA_SCALE 127.0f

#define EFF_CHANNEL_ALPHA_SHIFT 24

#define EFF_CHANNEL_RGB_COLOR 0x00808080

#define EFF_CURVE_SCALAR_SOURCE_OFFSET 16

#define EFF_CURVE_ROW_FLOATS 4

#define EFF_CURVE_SOURCE_ROW_COUNT 4

extern effBlurTemplate_t *hEve_PartialMultiBlur;
extern effBlurScatterWork_t *hEve_DistortionBlur;
extern effBlurScaleWork_t *hEve_RippleBlur;
extern dds3Effect_PartialMultiFilter_t *hEve_PartialMultiFilter;
extern u8 mblurDefaultParam[];
extern u8 dblurDefaultParam[];

/* Factories accept the serialized parameter prefix used by effect callbacks. */
extern u8 mfilterDefaultParam[];
extern u8 rblurDefaultParam[];
extern effScreenDrawParams_t BlurEditorGetParamsData;
extern dds3Effect_PartialMultiBlurParam_t rblurParam;
extern dds3Effect_DistortionBlurParam_t rblurParam2;
extern dds3Effect_Filter_t dblurData;
extern dds3Effect_PartialMultiFilterParam_t mfilterParam;
extern dds3Effect_PartialMultiFilterParam_t dblurParam2;
extern dds3Effect_RippleBlurParam_t rblurParam3;
extern sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

typedef billHandle_t billHandle_t;

/* Shared billboard-emitter parameters copied into each live set. Creation and
 * parameter copies clamp startDelaySpread, fadeIn, and fadeOut to at least
 * one; motionDelaySpread is consumed unchanged. */
typedef struct effEVECombParSmokeParam_t {
    f32 position[4];
    u32 number;
    s32 duration;
    s32 wait;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Forwarded to the billboard mode setter in direct draw. */
    u8 pad2A[2];
    s32 fade_in;
    s32 fade_out;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent; /* Speeds multiply by 1 + this / 100 per step. */
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge; /* Batched path fades primary texture RGB from black. */
    s32 colorTransitionFrames;
} effEVECombParSmokeParam_t;

typedef struct effEVECombParSmokePart_t {
    f32 pos[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z direction; Y accumulates vertical offset. */
    u8 pad1C[4];
    s32 frame;
    s32 wait;
    f32 scale;
    f32 s_w_r;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 s_h_r;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 roll;
    f32 roll_s;
    f32 amplitude;
    f32 angle;
    f32 up_s;
    f32 shift_s;
    u8 pad5C[4];
} effEVECombParSmokePart_t;

/* Particle storage is followed by this owner in the same allocation. */
typedef struct effEVECombParSmoke_t {
    effEVECombParSmokeParam_t head;
    effEVECombParSmokePart_t *pPart;
    u8 Ztest;
    u8 pad81[3];
    sdfMemHandle_t *hMem;
} effEVECombParSmoke_t;

/* Shared channel parameter block copied into each allocated channel work. */
typedef struct effAPPoint4Param_t {
    f32 controlPoints[4][4];
    u8 enabled;
    u8 pad41[3];
    u32 count;
    s32 steps;
    s32 spread;
    s32 fade_in;
    s32 fade_out;
    f32 jitter[4];
    u8 pad68[0x100];
} effAPPoint4Param_t;

typedef struct effAPPoint4Part_tag {
    s32 frame;
    struct dds3EffectPart_t *hPart; /* Retained compact effect owner, cloned per record. */
} effAPPoint4Part_tag;

/* Allocated channel owner shared by the event factory and channel consumers. */
typedef struct effAPPoint4_tag {
    effAPPoint4Param_t param;
    effAPPoint4Part_tag *pPart;
    struct effArrHdr_t *pBezier4P; /* Trailing allocation header, not the slot base. */
    sdfMemHandle_t *hMem;
} effAPPoint4_tag;

/* effCopyVertRows writes XYZ at a four-float stride, leaves +0x40..+0x57
 * untouched, then copies four scalars at +0x58. */
typedef struct effFloatRows_t {
    f32 primary[16];
    u8 opaque40[0x18];
    f32 secondary[4];
} effFloatRows_t;

/* Allocated interpolation cursor; effCreateChannel requests exactly 0x18 bytes. */
typedef struct effChan_t {
    sdfMemHandle_t *allocation;
    u32 recordCount;
    effFloatRows_t *rows;
    u32 cursorIndex;
    f32 cursorPosition;
    f32 cursorStep;
} effChan_t;

/* Emitter view: the first eight bytes are not used by this consumer. */
typedef struct effEmit_t {
    u8 opaque00[8];
    effAPPoint4_tag *primitive;
} effEmit_t;

/* Parameter template owner; the four-byte prefix is not read by channel work. */
typedef struct effChanSourceOwner_t {
    u8 opaque00[4];
    dds3EffectPart_t *param;
} effChanSourceOwner_t;

/* Source header followed by the owner used to duplicate each record parameter. */
typedef struct effChanSource_t {
    effAPPoint4Param_t head;
    effChanSourceOwner_t *owner;
} effChanSource_t;

extern f32 effEveBEDModeScale[];
extern f32 aViewEye[4];
extern u8 aViewDown[];

/* Read vector inputs; VU0 forms first minus second and stores second in vf31. */

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31. */
extern f32 effEveBEDDirAngle[];

/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. */
extern s32 EffEveCPSmokeCount;
extern billHandle_t *dds3BillboardFileLoad(s32 kind, const char *path);
extern addr_t hEffEveCPSmokeBill01Tex;
extern addr_t hEffEveCPSmokeBill02Tex;
extern effRandState_t dds3EffectBase[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);
extern f32 aViewTarget[4];

/* Blend packed RGBA8888 colors; t >= 1 returns colorB unchanged. */

/* Copy one 128-bit vector from the read-only source to destination storage. */
void dds3BillboardSetPosition(void *hBillboard, const void *pPos);
void dds3BillboardSetScale(billHandle_t *hBillboard, f32 x, f32 y);
void dds3BillboardSetRGBA(billHandle_t *hBillboard, u32 abgr);

/* Radian rotation of the billboard quad in its plane. */
void dds3BillboardSetRotate(billHandle_t *hBillboard, f32 rot);
extern effEVECombParSmokeParam_t effEVECombParSmokeCreate2Data;
dds3EffectPart_t *dds3EffectPartCreate(u16 type, void *pData);
dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *hEffPartSrc);

/* Return a borrowed block at the selected record's signed relative offset. */
void dds3Bezier4PDelete(struct effArrHdr_t *pEffBezier4P);
void dds3EffectPartDelete(dds3EffectPart_t *hEffPart);
effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *pEffBezier4P, s32 index);
void dds3EffectPartSetColor(dds3EffectPart_t *hEffPart, u32 abgr);
s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);

EXTERN_LATE_CONST sdfVector4_t effUnitY;

void effEveBEDPartGetPositionPtr(effEveBEDParams_t *pParams, effBTLAimParams_t *pPosPtr);
void dds3EffectPartSetPosition(dds3EffectPart_t *hEffPart, void *pPos);
void dds3EffectPartDraw(dds3EffectPart_t *hEffPart);
extern effArrHdr_t *dds3Bezier4PCreate(s32 number);
void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *hTex);

/* Return the current staggered-blur work without retaining it. */
effBlurScaleWork_t *Eve_RippleBlurGetParams(void);
void Eve_RippleBlurSetTex(sdfTexHandle_t *hTex);
void Eve_RippleBlurSetDefTex(void);
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam);
void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *hTex);
void Eve_PartialMultiFilterSetDefTex(void);
void Eve_EffectInit(void);
dds3Effect_PartialMultiBlurParam_t *mblurGetParam(void);
dds3Effect_DistortionBlurParam_t *dblurGetParam(void);
dds3Effect_Filter_t *dblurGetParam2(void);

/* Expose the resource-template setup body by address. */
dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void);
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters);

/* Expose the scale-blur setup parameters by address. */
dds3Effect_RippleBlurParam_t *rblurGetParam(void);
effBTL_t *effEveBEDCreate(effBTL_t *pData);
effEveBEDCut_t *effEveBEDCutCreate(effBTL_t *hEveBED, u16 CutNum, const void *pParams);
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams);
void effEveBEDCutGetParams(const effEveBEDRecord_t *hEveBEDCut, effEveBEDRecord_t *pParams);

/* The real mark-kind comparison value has a local register binding to keep
 * the native constant/byte-load order; see docs/matching/effect/effEvent.md. */
void effEveBEDCutDraw(effEveBEDCut_t *hEveBEDCut);
effSimpleBED_t *effSimpleBEDCreate(effBTL_t *pData, f32 scale);
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *hSimpleBEDSrc);

/* Apply the native owner operation to the pointer stored in the record prefix. */
void effSimpleBEDDraw(effSimpleBED_t *hSimpleBED);
void effSimpleBEDSetPosition(effSimpleBED_t *hSimpleBED, f32 *pPos);
void effSimpleBEDSetRGBA(effSimpleBED_t *hSimpleBED, u32 abgr);
extern const sdfVector4_t effUnitY;
effEVECombParSmoke_t *effEVECombParSmokeCreate(effEVECombParSmokeParam_t *pParam);
void effEVECombParSmokeDelete(effEVECombParSmoke_t *pCombParSmoke);
void effEVECombParSmokePartInit(effEVECombParSmoke_t *pCombParSmoke, s32 index);
void effCallBillboardSetMode(effEVECombParSmoke_t *work);

/* The 15-quad batch sdfBillBoardMakeN takes as four parallel arrays, with the
 * texture coordinates and the corner offsets as flat rows. */
typedef struct effEveBillBatch_t {
    sdfVector4_t centre[15];
    u32 color[15];
    s16 uv[15][8];
    f32 corner[15][8];
    s32 count;
} effEveBillBatch_t;

void effEVECombParSmokeGetParams(const effEVECombParSmokeParam_t *pCombParSmoke, effEVECombParSmokeParam_t *pParam);
void effEVECombParSmokeSetParams(effEVECombParSmokeParam_t *pCombParSmoke, const effEVECombParSmokeParam_t *pParam);

/* Create from the installed default emitter block; the returned pointer is unused. */
void effEVECombParSmokeCreate2(void);
effAPPoint4_tag *effAPPoint4Create(const effAPPoint4Param_t *pParams, u16 type, void *pData);
effAPPoint4_tag *effAPPoint4CreatePCF(void *data);
effAPPoint4_tag *effAPPoint4Copy(effChanSource_t *source);
void effAPPoint4Delete(effAPPoint4_tag *hData);
void effAPPoint4PartInit(effAPPoint4_tag *pAPPoint4, u32 index);
void effAPPoint4Draw(effAPPoint4_tag *hData);
void effEveAPP4Randomize(effEmit_t *hEffect);
extern char effLabelBlend[8];
extern char effLabelDistor[8];
extern char effLabelDistS[7];

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "basic.h"
#include "effBattleMisc.h"

typedef struct EffBattleUnitRgbCommand {
    u32 color;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitRgbCommand;

typedef struct EffBattleUnitColorCommand {
    EffectVectorRequest request;
    u32 firstColor;
    u32 secondColor;
    u32 startFrame;
    s32 startDurationIndex;
    u32 endFrame;
    s32 endDurationIndex;
} EffBattleUnitColorCommand;

typedef char EffBattleUnitRgbCommandSizeCheck[(sizeof(EffBattleUnitRgbCommand) == 0x14) ? 1 : -1];

typedef char EffBattleUnitColorCommandSizeCheck[(sizeof(EffBattleUnitColorCommand) == 0x20) ? 1 : -1];

/* Each effect bank addresses this many parameter rows when flattening a
 * bank/entry pair into the copy-source index stored in an entry. */
enum { EFF_BATTLE_PARAMETER_ENTRY_CAPACITY = 0x3C };

/* Serialized parameter selector: an effect-work kind or one of the row states
 * below. Keep the wire/storage width at 16 bits. */
typedef u16 EffBattleParameterKind;

typedef union EffBattleParameterValue {
    u32 mixerOffset; /* Effect-work payload offset from the source mixer. */
    u32 copySourceIndex; /* Bank * EFF_BATTLE_PARAMETER_ENTRY_CAPACITY + entry. */
    struct effPartHandle_t *work;
} EffBattleParameterValue;

typedef struct effBTLCutHandle_t effBTLCutHandle_t;

typedef void (*BattleEffectCallback)(effBTLCutHandle_t *effect);

/* Per-entry actor eligibility selector; preserve the stored byte width. */
typedef u8 EffBattleEntryActorFilterMode;

enum EffBattleEntryActorFilterMode {
    EFF_BATTLE_ENTRY_ACTOR_FILTER_BYPASS = 0,
    EFF_BATTLE_ENTRY_ACTOR_FILTER_LINK_FLAG_4_WITHOUT_FLAG_1 = 1,
    EFF_BATTLE_ENTRY_ACTOR_FILTER_LINK_FLAGS_4_AND_1 = 2,
    EFF_BATTLE_ENTRY_ACTOR_FILTER_LINK_FLAG_2 = 3,
};

typedef struct EffBattleEntry {
    EffBattleParameterKind parameterKind; /* 0x00: work kind or row-state sentinel. */
    u8 pad02[2];
    EffBattleParameterValue value; /* 0x04 */
    EffectVectorRequest aimParams;        /* 0x08 */
    u32 zeroKindCount;             /* 0x10 */
    EffBattleEntryActorFilterMode kind; /* 0x14: actor eligibility filter mode. */
    u8 pad15[3];
} EffBattleEntry; /* 0x18 */

typedef struct EffBattleEntryList {
    s32 triggerFrame;              /* 0x00 */
    u8 pad04[2];
    u16 shake;                     /* 0x06: screen-shake amount; 31 or more logs and caps at 300 frames */
    u16 shakeFrame;                /* 0x08: frame at which the shake starts */
    u16 shakeFade;                 /* 0x0A: fade applied with the shake */
    EffBattleUnitColorCommand colorCommand; /* 0x0C: single command read by the voice updater */
    u8 opaque2C[4];                /* 0x2C: retained bytes not read by the update routine */
    EffBattleUnitRgbCommand rgbCommand;     /* 0x30: single command read by the voice updater */
    u8 opaque44[4];                /* 0x44: retained bytes not read by the update routine */
    s32 callbackFrame;             /* 0x48 */
    u8 pad4C[0x20];
    u16 count;                     /* 0x6C */
    u8 pad6E[2];
    s32 colorRampEndFrame;         /* 0x70 */
    u8 actorScaleEnabled;          /* 0x74 */
    u8 pad75[3];
    EffBattleEntry entries[1];     /* 0x78 */
} EffBattleEntryList;

typedef struct SoundBank {
    u8 pad00[8];
    EffBattleEntryList list;       /* 0x08 */
    u8 pad98[0x580];
} SoundBank;

typedef char EffBattleEntry_size_must_be_0x18[
    (sizeof(EffBattleEntry) == 0x18) ? 1 : -1];

typedef char EffBattleEntryList_size_must_be_0x90[
    (sizeof(EffBattleEntryList) == 0x90) ? 1 : -1];

typedef char SoundBank_size_must_be_0x618[
    (sizeof(SoundBank) == 0x618) ? 1 : -1];

typedef struct SoundMixer {
    SoundBank banks[2];
    u8 pad0C30[8];
    struct sdfMemHandle_t *resource; /* 0xC38: owns the mixer allocation. */
    u32 activeVoiceCount; /* Effect voices currently linked to this mixer. */
    effBTLCutHandle_t *voiceList;
} SoundMixer;

/* DDS1 00160958 / DDS2 00168548 allocate and clear exactly 0x128 bytes. */
struct effBTLCutHandle_t {
    struct BtlUnit *owner; /* 0x00: borrowed voice owner used for actor scaling. */
    SoundMixer *state; /* 0x04 */
    u32 color; /* 0x08 */
    u16 flags; /* 0x0C */
    u8 pad0E[2];
    u32 currentFrame; /* 0x10 */
    u32 triggerFrame; /* 0x14 */
    u32 colorRampEndFrame; /* 0x18 */
    u16 kind; /* 0x1C */
    u8 pad1E[2];
    s32 unk20;
    struct EffBattleEntryList *list; /* 0x24 */
    struct effPartHandle_t *parameterWorks[EFF_BATTLE_PARAMETER_ENTRY_CAPACITY]; /* 0x28 */
    BattleEffectCallback sourceFrameCallback; /* 0x118 */
    BattleEffectCallback triggerFrameCallback; /* 0x11C */
    u32 unk120;
    effBTLCutHandle_t *next; /* 0x124 */
};

/* The stored screen record keeps eight bytes beyond the copied blur quad. */
typedef struct EffScreenDrawParams {
    dds3Effect_Blur_t source;
    u8 pad28[8];
} EffScreenDrawParams;

typedef char EffScreenDrawParams_size_must_be_0x30[(sizeof(EffScreenDrawParams) == 0x30) ? 1 : -1];

/* Event-light init block copied into the retained event owner (0x30 bytes). */
typedef struct EffEventInit {
    f32 position[4];
    f32 orientation[4];
    f32 range;
    f32 height;
    f32 param;
    u32 color;
} EffEventInit;

typedef char EffEventInit_size_must_be_0x30[(sizeof(EffEventInit) == 0x30) ? 1 : -1];

typedef char EffEventLight_size_must_be_0x3C[
    (sizeof(effSimpleBED_t) == 0x3C) ? 1 : -1];

#define EFF_EVENT_EVENT_RECORD_BYTES 0x30

extern EffScreenDrawParams effData4;
extern dds3Effect_Filter_t D_00355F88;
extern s8 EffectDrawData2;
void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source);

/* Compact event owner: the allocator reserves exactly 0x38 bytes. */
struct effEveBEDCutHandle_t {
    EffEventInit init;
    struct evtUnitHandle_t *actor; /* Linked event unit supplies its model owner. */
    effBTLCutHandle_t *effect;
};

struct BtlUnit;

struct DmaPacketHeader;

INCLUDE_ASM(const s32, "effect/effEvent", effBlur2OnSub);

INCLUDE_ASM(const s32, "effect/effEvent", effBlur2SwapSub);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub3);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub4);

INCLUDE_ASM(const s32, "effect/effEvent", effMultiBlurOnSub);

INCLUDE_ASM(const s32, "effect/effEvent", effMultiBlurSwapSub);

void Eve_PartialMultiBlurSetTex(sdfTexHandle_t *hTex) {
    hEve_PartialMultiBlur->texture = hTex;
}

INCLUDE_ASM(const s32, "effect/effEvent", effCallBillboardGetGeneralTex);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub5);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub6);

INCLUDE_ASM(const s32, "effect/effEvent", effDistBlurOnSub);

effBlurScatterWork_t *effDistBlurSwapSub(void) {
    return hEve_DistortionBlur;
}

INCLUDE_ASM(const s32, "effect/effEvent", Eve_DistortionBlurSetTex);

INCLUDE_ASM(const s32, "effect/effEvent", effCallBillboardGetGeneralTex2);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub7);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub8);

INCLUDE_ASM(const s32, "effect/effEvent", effRipBlurOnSub);

INCLUDE_ASM(const s32, "effect/effEvent", effRipBlurSwapSub);

void Eve_RippleBlurSetTex(sdfTexHandle_t *hTex) {
    hEve_RippleBlur->texture = hTex;
}

/* Install the staggered-blur resource-table texture without changing its slots. */
void Eve_RippleBlurSetDefTex(void) {
    hEve_RippleBlur->texture = dds3BillboardGetGeneralTex(EFF_EVENT_STAGGERED_RESOURCE_INDEX);
}

void effEnableFramebufferQuad(void) {
    EffectDrawData2 = 1;
}

void effDisableFramebufferQuad(void) {
    EffectDrawData2 = 0;
}

/* Copy only the framebuffer source prefix, not the trailing screen-record bytes. */
void effCopyFramebufferQuadParameters(const dds3Effect_Blur_t *source) {
    memcpy(&effData4, source, EFF_EVENT_BLUR_SOURCE_BYTES);
}

/* Expose the framebuffer-quad parameter block by address. */
EffScreenDrawParams *effGetCh73Params(void) {
    return &effData4;
}

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub9);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub10);

INCLUDE_ASM(const s32, "effect/effEvent", dblurSetParam2);

INCLUDE_ASM(const s32, "effect/effEvent", effFilter2SwapSub);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub11);

INCLUDE_ASM(const s32, "effect/effEvent", effBlurUpdateSub12);

/* Copy the resource template body while preserving its selected resource. */
void Eve_PartialMultiFilterSetParam(dds3Effect_PartialMultiFilterParam_t *pParam) {
    hEve_PartialMultiFilter->param = *pParam;
}

INCLUDE_ASM(const s32, "effect/effEvent", effMFilterSwapSub);

void Eve_PartialMultiFilterSetTex(sdfTexHandle_t *hTex) {
    hEve_PartialMultiFilter->hAlphaTex = hTex;
}

/* Install the textured-square resource-table texture while retaining its body. */
void Eve_PartialMultiFilterSetDefTex(void) {
    hEve_PartialMultiFilter->hAlphaTex = dds3BillboardGetGeneralTex(EFF_EVENT_SQUARE_RESOURCE_INDEX);
}

/* Clone the four default work templates; only the staggered slot count is overridden. */
void Eve_EffectInit(void) {
    hEve_PartialMultiBlur = dds3EffectPartialMultiBlurCreate((dds3Effect_PartialMultiBlurParam_t *)mblurDefaultParam);
    hEve_DistortionBlur = dds3EffectDistortionBlurCreate((dds3Effect_DistortionBlurParam_t *)dblurDefaultParam);
    hEve_PartialMultiFilter = dds3EffectPartialMultiFilterCreate((dds3Effect_PartialMultiFilterParam_t *)mfilterDefaultParam);
    hEve_RippleBlur = dds3EffectRippleBlurCreate((dds3Effect_RippleBlurParam_t *)rblurDefaultParam);
    Eve_RippleBlurGetParams()->params.number = EFF_EVENT_STAGGERED_SLOT_COUNT;
}

INCLUDE_ASM(const s32, "effect/effEvent", EffectDraw);

INCLUDE_ASM(const s32, "effect/effEvent", effCallMemcpy);

effScreenDrawParams_t *BlurEditorGetParams(void) {
    return &BlurEditorGetParamsData;
}

INCLUDE_ASM(const s32, "effect/effEvent", BlurSetParam2);

INCLUDE_ASM(const s32, "effect/effEvent", mblurProcess);

dds3Effect_PartialMultiBlurParam_t *mblurGetParam(void) {
    return &rblurParam;
}

INCLUDE_ASM(const s32, "effect/effEvent", rblurSetParam);

INCLUDE_ASM(const s32, "effect/effEvent", dblurProcess);

dds3Effect_DistortionBlurParam_t *dblurGetParam(void) {
    return &rblurParam2;
}

INCLUDE_ASM(const s32, "effect/effEvent", dblurSetParam);

INCLUDE_ASM(const s32, "effect/effEvent", effCallMemcpy2);

dds3Effect_Filter_t *dblurGetParam2(void) {
    return &dblurData;
}

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf29);

INCLUDE_ASM(const s32, "effect/effEvent", mfilterRun);

dds3Effect_PartialMultiFilterParam_t *mfilterGetParam(void) {
    return &dblurParam2;
}

/* Replace the setup block rather than the active solid-rectangle parameters. */
void effEventSetSolidRectangleParameters(dds3Effect_Filter_t *parameters) {
    D_00355F88 = *parameters;
}

INCLUDE_ASM(const s32, "effect/effEvent", effCallMemcpy3);

INCLUDE_ASM(const s32, "effect/effEvent", effEnterDataSub);

/* Replace the setup body without replacing the active textured-square work. */
void mfilterSetParam(dds3Effect_PartialMultiFilterParam_t *parameters) {
    mfilterParam = *parameters;
}

INCLUDE_ASM(const s32, "effect/effEvent", rblurProcess);

dds3Effect_RippleBlurParam_t *rblurGetParam(void) {
    return &rblurParam3;
}

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf30);

INCLUDE_ASM(const s32, "effect/effEvent", effAddBEDEffectSub);

INCLUDE_ASM(const s32, "effect/effEvent", effFreeEffectDataSub);

/* Allocate the compact record, copy its init prefix, then attach the new effect. */
effEveBEDCut_t *effEveBEDCutCreate(effBTL_t *hEveBED, u16 CutNum, const void *pParams) {
    const effEveBEDParams_t *init = (const effEveBEDParams_t *)pParams;
    effEveBEDCut_t *work = sdfChipAllocDirect(sizeof(effEveBEDCut_t));

    memcpy(work, init, sizeof(*init));
    work->actor = NULL;
    work->effect = effBTLCutCreate(hEveBED, CutNum, 0, 0);
    effBTLCutSetScale(work->effect, init->scale);
    return work;
}

INCLUDE_ASM(const s32, "effect/effEvent", effBTLSys_Nego_Stone_Delete2);

/* Copy the packed 0x30-byte record while retaining its native packed layout. */
void effEveBEDCutSetParams(effEveBEDCut_t *hEveBEDCut, const void *pParams) {
    *(effEveBEDRecord_t *)hEveBEDCut = *(const effEveBEDRecord_t *)pParams;
}

/* This copies a packed 0x30-byte record, not a complete 0x60-byte billboard state. */
void effEveBEDCutGetParams(const effEveBEDRecord_t *hEveBEDCut, effEveBEDRecord_t *pParams) {
    *pParams = *hEveBEDCut;
}

INCLUDE_ASM(const s32, "effect/effEvent", effCommandEFFBEDSTARTEVOSub);

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf31);

INCLUDE_ASM(const s32, "effect/effEvent", effCallMult);

/* Initialize a holder and its effect record; the teardown flag starts set. */
effSimpleBED_t *effSimpleBEDCreate(effBTL_t *pData, f32 scale) {
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->pParams.rot[3] = 1.0f;
    work->pParams.r = 50.0f;
    work->pParams.h = 180.0f;
    work->pParams.center[1] = -90.0f;
    work->pParams.scale = scale;
    work->pParams.abgr = EFF_EVENT_NEUTRAL_COLOR;
    work->pParams.rot[0] = 0;
    work->pParams.rot[1] = 0;
    work->pParams.rot[2] = 0;
    work->pParams.center[0] = 0;
    work->pParams.center[2] = 0;
    work->pParams.center[3] = 0;
    work->hBED = effEveBEDCreate(pData);
    work->hCut = effEveBEDCutCreate(work->hBED, 0, &work->pParams);
    work->original = 1;
    return work;
}

INCLUDE_ASM(const s32, "effect/effEvent", effSimpleBEDDelete);

/* Clone the init block/effect using the same handle; clear the shared-teardown flag. */
effSimpleBED_t *effSimpleBEDCopy(effSimpleBED_t *hSimpleBEDSrc) {
    effEveBEDParams_t *block = &hSimpleBEDSrc->pParams;
    effSimpleBED_t *work = sdfChipAllocDirect(sizeof(effSimpleBED_t));

    work->hCut = effEveBEDCutCreate(hSimpleBEDSrc->hBED, 0, block);
    work->hBED = hSimpleBEDSrc->hBED;
    memcpy(&work->pParams, block, sizeof(work->pParams));
    work->original = 0;
    return work;
}

void effSimpleBEDDraw(effSimpleBED_t *hSimpleBED) {
    effEveBEDCutDraw(hSimpleBED->hCut);
}

/* Copy xyz, lower y by half the aim height, clear w and publish the record. */
void effSimpleBEDSetPosition(effSimpleBED_t *hSimpleBED, f32 *pPos) {
    hSimpleBED->pParams.center[0] = pPos[0];
    hSimpleBED->pParams.center[1] = pPos[1] - hSimpleBED->pParams.h * 0.5f;
    hSimpleBED->pParams.center[2] = pPos[2];
    hSimpleBED->pParams.center[3] = 0;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}

/* Copy the holder's updated packed tint to its event record. */
void effSimpleBEDSetRGBA(effSimpleBED_t *hSimpleBED, u32 abgr) {
    hSimpleBED->pParams.abgr = abgr;
    effEveBEDCutSetParams(hSimpleBED->hCut, &hSimpleBED->pParams);
}

/* vu0 routine: place one part relative to the record's position and facing. */
void effEveBEDPartGetPositionPtr(effEveBEDParams_t *pParams, effBTLAimParams_t *pPosPtr) {
    sdfVector4_t out;
    sdfVector4_t offset;
    sdfVector4_t position;
    s32 range = pPosPtr->rangeOverride;
    u8 dir = pPosPtr->directionMode;
    u8 mode = pPosPtr->aimMode;
    f32 radius;
    f32 half;
    f32 y;
    f32 zero;

    if (range == 0) {
        radius = pParams->r;
    } else {
        radius = (f32)range;
    }
    half = pParams->h * 0.5f;
    SDF_VU0_COPYVECTOR(&position, pParams);
    if (mode == 5) {
        if (dir == 8 || dir == 0xA) {
            y = -1.0f;
            if (range != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = position.y - effEveBEDModeScale[mode] * half;
        if (dir == 8 || dir == 0xA) {
            if (range != 0) {
                y -= radius;
            }
        }
    }
    if (dir == 9 || mode == 4) {
        if (half < radius) {
            offset.z = -radius;
        } else {
            offset.z = -half;
        }
        zero = 0.0f;
        offset.y = zero;
        offset.x = zero;
        position.y = y;
        LookAtMatrix(&position, aViewEye, aViewDown);
        ReverseMatrix();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(&position);
        SDF_VU0_ADDVECTOR();
        return;
    }
    if (dir == 8 || dir == 0xA) {
        out.x = position.x;
        out.y = y;
        out.z = position.z;
    } else {
        offset.x = 0.0f;
        offset.z = 1.0f;
        offset.y = 0.0f;
        SDF_VU0_LOADVECTOR(pParams->rot);
        ToMatrix();
        SetRotateMatrixY_B(effEveBEDDirAngle[dir]);
        MultMatrixNoArg();
        SDF_VU0_LOADVECTOR(&offset);
        SDF_VU0_VECTOR_W_SET_ZERO();
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR_NOMEM(&offset);
        out.x = position.x + radius * offset.x;
        out.y = y + radius * offset.y;
        out.z = position.z + radius * offset.z;
    }
    SDF_VU0_LOADVECTOR(&out);
}

INCLUDE_RODATA(const s32, "effect/effEvent", effCallMultTable);

effEVECombParSmoke_t *effEVECombParSmokeCreate(effEVECombParSmokeParam_t *pParam) {
    u32 particleCount = pParam->number;
    u32 particleBytes = particleCount * sizeof(effEVECombParSmokePart_t);
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(particleBytes + sizeof(effEVECombParSmoke_t));
    effEVECombParSmokePart_t *particle = EE_ADDR_AS(effEVECombParSmokePart_t *, sdfMemLock(allocationHandle));
    effEVECombParSmoke_t *work = (effEVECombParSmoke_t *)((u8 *)particle + particleBytes);
    u32 particleIndex;

    work->head = *pParam;
    work->hMem = allocationHandle;
    work->pPart = particle;
    work->Ztest = 0;
    if (work->head.fade_in <= 0) {
        work->head.fade_in = 1;
    }
    if (work->head.fade_out <= 0) {
        work->head.fade_out = 1;
    }
    if (work->head.wait <= 0) {
        work->head.wait = 1;
    }
    EffEveCPSmokeCount = EffEveCPSmokeCount + 1;
    if (EffEveCPSmokeCount == 1) {
        hEffEveCPSmokeBill01Tex = EE_ADDR(dds3BillboardFileLoad(0, "/efftool/bill/dbball01.tmx"));
        hEffEveCPSmokeBill02Tex = EE_ADDR(dds3BillboardFileLoad(0, "/efftool/bill/dbball02.tmx"));
    }
    particleCount = work->head.number;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        particle->frame = 0;
        particle++;
    }
    return work;
}

/* Drop the shared texture reference count; zero dispatches both resource handles.
 * The retained allocation is released afterwards, regardless of that count. */
void effEVECombParSmokeDelete(effEVECombParSmoke_t *pCombParSmoke) {
    EffEveCPSmokeCount = EffEveCPSmokeCount - 1;
    if (EffEveCPSmokeCount == 0) {
        dds3BillboardDelete(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex));
        dds3BillboardDelete(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex));
    }
    sdfMemFreeDirect(pCombParSmoke->hMem);
}

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEVECombParSmokePartInit(effEVECombParSmoke_t *pCombParSmoke, s32 index) {
    effEVECombParSmokePart_t *particle = &pCombParSmoke->pPart[index];
    f32 direction[EFF_EVENT_VECTOR_COMPONENTS];
    f32 scaleFactor;
    f32 spawnRadius;
    s32 startDelaySpread = pCombParSmoke->head.wait;
    s32 motionDelaySpread = pCombParSmoke->head.motionDelaySpread;

    particle->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % startDelaySpread);
    particle->wait = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % motionDelaySpread);
    scaleFactor = dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.scaleRandomness + (1.0f - pCombParSmoke->head.scaleRandomness);
    particle->scale = pCombParSmoke->head.baseScale * scaleFactor;
    particle->s_w_r = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->s_h_r = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    particle->scalePhaseStepX = pCombParSmoke->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scalePhaseStepY = pCombParSmoke->head.scalePhaseStep * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
    particle->scaleAmplitudeX = pCombParSmoke->head.scaleAmplitudeX * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->scaleAmplitudeY = pCombParSmoke->head.scaleAmplitudeY * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE) * scaleFactor;
    particle->roll = dds3GetRandFloatA(dds3EffectBase) * (EFF_EVENT_HALF_TURN * 2.0f);
    if (dds3GetRandA((effRandState_t *)dds3EffectBase) & 1) {
        particle->roll_s = pCombParSmoke->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE);
    } else {
        particle->roll_s = -(pCombParSmoke->head.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * EFF_EVENT_JITTER_RANGE + EFF_EVENT_JITTER_BASE));
    }
    spawnRadius = pCombParSmoke->head.spawnRadius;
    direction[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[1] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    direction[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(direction);
    /* Each coordinate consumes its own radial random factor, not one shared radius. */
    particle->pos[0] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[0];
    particle->pos[1] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[1];
    particle->pos[2] = spawnRadius * dds3GetRandFloatA(dds3EffectBase) * direction[2];
    particle->motion[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    particle->motion[1] = 0;
    particle->motion[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_EVENT_RANDOM_MIDPOINT) * 2.0f;
    SDF_VU0_LOADVECTOR(particle->motion);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->motion);
    particle->amplitude = pCombParSmoke->head.swayAmplitude * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.swayRandomness + (1.0f - pCombParSmoke->head.swayRandomness));
    particle->angle = 0;
    particle->up_s = pCombParSmoke->head.verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.verticalSpeedRandomness + (1.0f - pCombParSmoke->head.verticalSpeedRandomness));
    particle->shift_s = pCombParSmoke->head.lateralSpeed * (dds3GetRandFloatA(dds3EffectBase) * pCombParSmoke->head.lateralSpeedRandomness + (1.0f - pCombParSmoke->head.lateralSpeedRandomness));
}

/* vu0 routine: advance and draw every live particle of one smoke set. */
void effCallBillboardSetMode(effEVECombParSmoke_t *work) {
    sdfVector4_t offset;
    sdfVector4_t origin;
    f32 side[EFF_EVENT_VECTOR_COMPONENTS];
    effEVECombParSmokePart_t *particle;
    u8 repeat;
    u32 count;
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    f32 growth;
    f32 swayPhaseStep;
    f32 swayAmplitudeStep;
    f32 drift;
    f32 wave;
    f32 ratio;
    f32 scaleX;
    f32 scaleY;
    u32 rgba;
    s32 age;
    s32 remaining;
    u32 i;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(aViewDown);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(side);
    growth = SDF_FRAME_DECAY(work->head.accelerationPercent / 100.0f + 1.0f);
    particle = work->pPart;
    count = work->head.number;
    repeat = work->head.repeat;
    duration = SDF_FRAMES(work->head.duration);
    fadeIn = SDF_FRAMES(work->head.fade_in);
    fadeOut = SDF_FRAMES(work->head.fade_out);
    color = work->head.color;
    swayPhaseStep = SDF_FRAME_STEP(work->head.swayPhaseStep);
    swayAmplitudeStep = SDF_FRAME_STEP(work->head.swayAmplitudeStep);
    SDF_VU0_COPYVECTOR(&origin, work);
    i = 0;
    dds3BillboardSetMode(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex), work->head.drawMode);
    dds3BillboardSetMode(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex), 3);
    for (; i < count; i++, particle++) {
        age = particle->frame;
        if (age == 0) {
            effEVECombParSmokePartInit(work, i);
        }
        if (age > 0 && age <= duration) {
            if (particle->wait >= 0) {
                drift = SDF_FRAME_STEP(particle->shift_s);
                particle->pos[0] += side[0] * drift;
                particle->pos[1] += side[1] * drift;
                particle->pos[2] += side[2] * drift;
                wave = particle->amplitude * sdfSin(particle->angle);
                particle->motion[1] += SDF_FRAME_STEP(particle->up_s);
                offset.x = particle->pos[0] + particle->motion[0] * wave;
                offset.y = particle->pos[1] + particle->motion[1];
                offset.z = particle->pos[2] + particle->motion[2] * wave;
                particle->up_s *= growth;
                particle->shift_s *= growth;
                particle->amplitude += swayAmplitudeStep;
                particle->angle += swayPhaseStep;
            } else {
                particle->wait++;
                offset.x = particle->pos[0];
                offset.y = particle->pos[1];
                offset.z = particle->pos[2];
            }
            if (age < fadeIn) {
                ratio = (f32)age / (f32)fadeIn;
            } else {
                remaining = duration - age;
                if (remaining <= fadeOut) {
                    ratio = (f32)remaining / (f32)fadeOut;
                } else {
                    ratio = 1.0f;
                }
            }
            rgba = effColorBlend(color & 0xFFFFFF, color, ratio);
            scaleY = particle->scale;
            scaleX = scaleY + particle->scaleAmplitudeX * sdfCos(particle->s_w_r);
            scaleY = scaleY + particle->scaleAmplitudeY * sdfCos(particle->s_h_r);
            SDF_VU0_LOADVECTOR_MEMOPS(offset.x);
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR_MEMOPS(offset.x);
            dds3BillboardSetPosition(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex), &offset);
            dds3BillboardSetScale(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex), scaleX, scaleY);
            dds3BillboardSetRGBA(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex), rgba);
            dds3BillboardSetRotate(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex), particle->roll);
            dds3BillboardDraw(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill01Tex));
            dds3BillboardSetPosition(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex), &offset);
            dds3BillboardSetScale(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex), scaleX, scaleY);
            dds3BillboardSetRGBA(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex), rgba);
            dds3BillboardSetRotate(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex), particle->roll);
            dds3BillboardDraw(EE_ADDR_AS(billHandle_t *, hEffEveCPSmokeBill02Tex));
            particle->s_w_r += SDF_FRAME_STEP(particle->scalePhaseStepX);
            particle->s_h_r += SDF_FRAME_STEP(particle->scalePhaseStepY);
            particle->roll += SDF_FRAME_STEP(particle->roll_s);
        }
        if (duration == age && repeat != 0) {
            particle->frame = 0;
        } else {
            particle->frame++;
        }
    }
}

INCLUDE_ASM(const s32, "effect/effEvent", effEVECombParSmokeDraw);

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf32);

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf33);

/* Copy the serialized emitter parameters without changing their values. */
void effEVECombParSmokeGetParams(const effEVECombParSmokeParam_t *pCombParSmoke, effEVECombParSmokeParam_t *pParam) {
    *pParam = *pCombParSmoke;
}

/* Copy parameters and clamp only start-delay spread and fade divisors;
 * motionDelaySpread remains unvalidated on this native path. */
void effEVECombParSmokeSetParams(effEVECombParSmokeParam_t *pCombParSmoke, const effEVECombParSmokeParam_t *pParam) {
    *pCombParSmoke = *pParam;
    if (pCombParSmoke->fade_in <= 0) {
        pCombParSmoke->fade_in = 1;
    }
    if (pCombParSmoke->fade_out <= 0) {
        pCombParSmoke->fade_out = 1;
    }
    if (pCombParSmoke->wait <= 0) {
        pCombParSmoke->wait = 1;
    }
}

void effEVECombParSmokeCreate2(void) {
    effEVECombParSmokeCreate(&effEVECombParSmokeCreate2Data);
}

effAPPoint4_tag *effAPPoint4Create(const effAPPoint4Param_t *pParams, u16 type, void *pData) {
    const effAPPoint4Param_t *head = pParams;
    u32 recordCount = head->count;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(recordCount * sizeof(effAPPoint4Part_tag) + sizeof(effAPPoint4_tag));
    effAPPoint4_tag *work = EE_ADDR_AS(effAPPoint4_tag *, sdfMemLock(allocation));
    effAPPoint4Part_tag *record = (effAPPoint4Part_tag *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->param = *head;
    work->hMem = allocation;
    work->pPart = record;
    if (work->param.spread <= 0) {
        work->param.spread = 1;
    }
    work->pBezier4P = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        delayModulus = work->param.spread;
        parameterTemplate = dds3EffectPartCreate(type, pData);
        record->hPart = parameterTemplate;
        record->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        record++;
        for (recordIndex = 1; recordIndex < recordCount; recordIndex++) {
            record->hPart = dds3EffectPartCopy(parameterTemplate);
            record->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            record++;
        }
    }
    return work;
}

/* Forward block 0, block 1's word-2 kind (narrowed to u16), and block 1 unchanged. */
effAPPoint4_tag *effAPPoint4CreatePCF(void *data) {
    void *block0 = dds3EffectPCPDataGetData(data, 0);
    void *block1 = dds3EffectPCPDataGetData(data, 1);

    return effAPPoint4Create(block0, dds3EffectPCPDataGetType(data, 1), block1);
}

/* Clone channel work, allocate slots and duplicate a parameter template per record.
 * The record count is captured before allocation; a nonpositive delay modulus becomes one. */
effAPPoint4_tag *effAPPoint4Copy(effChanSource_t *source) {
    u32 recordCount = source->head.count;
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(recordCount * sizeof(effAPPoint4Part_tag) + sizeof(effAPPoint4_tag));
    effAPPoint4_tag *work = EE_ADDR_AS(effAPPoint4_tag *, sdfMemLock(allocationHandle));
    effAPPoint4Part_tag *recordCursor = (effAPPoint4Part_tag *)(work + 1);
    dds3EffectPart_t *parameterTemplate;
    s32 delayModulus;
    u32 recordIndex;

    work->param = source->head;
    work->hMem = allocationHandle;
    work->pPart = recordCursor;
    if (work->param.spread <= 0) {
        work->param.spread = 1;
    }
    work->pBezier4P = dds3Bezier4PCreate(recordCount);
    if (recordCount != 0) {
        parameterTemplate = source->owner->param;
        delayModulus = work->param.spread;
        for (recordIndex = 0; recordIndex < recordCount; recordIndex++) {
            recordCursor->hPart = dds3EffectPartCopy(parameterTemplate);
            recordCursor->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
            recordCursor++;
        }
    }
    return work;
}

/* Release slot storage, every duplicated parameter and the backing allocation in native order. */
void effAPPoint4Delete(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor;
    u32 recordIndex = 0;
    u32 recordCount;

    dds3Bezier4PDelete(hData->pBezier4P);
    recordCount = hData->param.count;
    recordCursor = hData->pPart;
    if (recordCount != 0) {
        do {
            dds3EffectPartDelete(recordCursor->hPart);
            recordCursor++;
            recordIndex++;
        } while (recordIndex < recordCount);
    }
    sdfMemFreeDirect(hData->hMem);
}

/* vu0 routine: Jitter four control points normal to the path and viewing direction.
 * Reuse the final edge normal for the endpoint; retain XYZ-only scale initialization. */
void effAPPoint4PartInit(effAPPoint4_tag *pAPPoint4, u32 index) {
    sdfVector4_t jitterScale;
    sdfVector4_t lastNormal;
    sdfVector4_t viewDirection;
    sdfVector4_t jitteredPoint;
    effAPPoint4Part_tag *record = pAPPoint4->pPart + index;
    effCubicBezierSlot_t *bezierSlot;
    f32 cursorStep;
    f32 centeredRandom;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&viewDirection);
    cursorStep = 1.0f / (f32)SDF_FRAMES(pAPPoint4->param.steps);
    bezierSlot = dds3Bezier4PGetPart(pAPPoint4->pBezier4P, index);
    bezierSlot->t = 0;
    bezierSlot->step = cursorStep;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[0] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[0]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[0][0] = jitteredPoint.x;
    bezierSlot->controlPoints[0][1] = jitteredPoint.y;
    bezierSlot->controlPoints[0][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[1] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[1]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[1][0] = jitteredPoint.x;
    bezierSlot->controlPoints[1][1] = jitteredPoint.y;
    bezierSlot->controlPoints[1][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[2] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(pAPPoint4->param.controlPoints[2]);
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(&viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&lastNormal);
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[2][0] = jitteredPoint.x;
    bezierSlot->controlPoints[2][1] = jitteredPoint.y;
    bezierSlot->controlPoints[2][2] = jitteredPoint.z;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
    jitterScale.x = pAPPoint4->param.jitter[3] * (centeredRandom + centeredRandom);
    jitterScale.y = jitterScale.x;
    jitterScale.z = jitterScale.x;
    SDF_VU0_LOADVECTOR(&lastNormal);
    SDF_VU0_LOADVECTOR_B(&jitterScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pAPPoint4->param.controlPoints[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(&jitteredPoint);
    bezierSlot->controlPoints[3][0] = jitteredPoint.x;
    bezierSlot->controlPoints[3][1] = jitteredPoint.y;
    bezierSlot->controlPoints[3][2] = jitteredPoint.z;
    dds3EffectPartSetColor(record->hPart, 0);
}

/* Step active records, update their packed color/position callbacks, and optionally recycle.
 * The slot-zero lookup precedes the empty-count check; retain the time snapshot and later stored increments. */
void effAPPoint4Draw(effAPPoint4_tag *hData) {
    effAPPoint4Part_tag *recordCursor = hData->pPart;
    effArrHdr_t *bezierSlots = hData->pBezier4P;
    u32 recordCount = hData->param.count;
    u32 recordIndex = 0;
    u8 cycleEnabled = hData->param.enabled;
    s32 activeSteps = SDF_FRAMES(hData->param.steps);
    s32 delayModulus = hData->param.spread;
    s32 fadeInSteps = SDF_FRAMES(hData->param.fade_in);
    s32 fadeOutSteps = SDF_FRAMES(hData->param.fade_out);
    f32 position[3] QWORD_ALIGNED;

    (void)dds3Bezier4PGetPart(bezierSlots, 0);
    if (recordCount == 0) {
        return;
    }
    do {
        s32 recordTime = recordCursor->frame;

        if (recordTime == 0) {
            effAPPoint4PartInit(hData, recordIndex);
        }
        if (recordTime > 0 && recordTime <= activeSteps) {
            f32 opacity;
            s32 fadePhase;
            u32 alpha;
            u32 color;

            dds3Bezier4PPartRun(bezierSlots, recordIndex, position);
            fadePhase = fadeInSteps > recordTime;
            if (fadePhase) {
                opacity = (f32)recordTime / (f32)fadeInSteps;
            } else {
                fadePhase = activeSteps - recordTime;
                if (fadePhase <= fadeOutSteps) {
                    opacity = (f32)fadePhase / (f32)fadeOutSteps;
                } else {
                    opacity = 1.0f;
                }
            }
            alpha = (u32)(opacity * EFF_CHANNEL_ALPHA_SCALE);
            color = (alpha << EFF_CHANNEL_ALPHA_SHIFT) | EFF_CHANNEL_RGB_COLOR;
            dds3EffectPartSetColor(recordCursor->hPart, color);
            dds3EffectPartSetPosition(recordCursor->hPart, position);
            dds3EffectPartDraw(recordCursor->hPart);
        }
        if (recordTime < activeSteps) {
            recordCursor->frame++;
        } else if (cycleEnabled != 0) {
            recordCursor->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delayModulus);
        } else {
            recordCursor->frame++;
        }
        recordCursor++;
        recordIndex++;
    } while (recordIndex < recordCount);
}

INCLUDE_ASM(const s32, "effect/effEvent", effLeaf34);

/* Jitter each slot and choose its initial unsigned-modulo step.
 * Stored record time is one ahead of the Bezier cursor parameter's sampled step. */
void effEveAPP4Randomize(effEmit_t *hEffect) {
    effAPPoint4_tag *channelWork = hEffect->primitive;
    u32 stepModulus = channelWork->param.steps;
    u32 recordCount = channelWork->param.count;
    effAPPoint4Part_tag *recordCursor = channelWork->pPart;
    u32 recordIndex = 0;
    effCubicBezierSlot_t *bezierSlot;
    s32 randomStep;
    f32 cursorStep;

    if (recordCount == 0) {
        return;
    }
    do {
        effAPPoint4PartInit(channelWork, recordIndex);
        recordCursor->frame = SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % stepModulus);
        bezierSlot = dds3Bezier4PGetPart(channelWork->pBezier4P, recordIndex);
        recordIndex++;
        randomStep = recordCursor->frame;
        cursorStep = bezierSlot->step;
        recordCursor->frame = randomStep + 1;
        recordCursor++;
        bezierSlot->t = cursorStep * (f32)randomStep;
    } while (recordIndex < recordCount);
}
INCLUDE_SDATA(const s32, "effect/effEvent", EffectDrawData3);

INCLUDE_SDATA(const s32, "effect/effEvent", EffectDrawData4);

INCLUDE_SDATA(const s32, "effect/effEvent", EffectDrawData5);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry2);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry3);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry4);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry5);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry6);

INCLUDE_SDATA(const s32, "effect/effEvent", mblurProcessDataEntry2);

INCLUDE_SDATA(const s32, "effect/effEvent", dblurProcessDataEntry2);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenuEntry2);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenuEntry3);

INCLUDE_SDATA(const s32, "effect/effEvent", evtUpdateEffectViewerGroupData);

INCLUDE_SDATA(const s32, "effect/effEvent", dblurProcessDataEntry3);

INCLUDE_SDATA(const s32, "effect/effEvent", effLabelBlend);

INCLUDE_SDATA(const s32, "effect/effEvent", effLabelDistor);

INCLUDE_SDATA(const s32, "effect/effEvent", rblurDebugMenuEntry2);

INCLUDE_SDATA(const s32, "effect/effEvent", rblurDebugMenuEntry3);

INCLUDE_SDATA(const s32, "effect/effEvent", dblurProcessDataEntry4);

INCLUDE_SDATA(const s32, "effect/effEvent", rblurDebugMenuEntry4);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry7);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry8);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu2Entry9);

INCLUDE_SDATA(const s32, "effect/effEvent", evtViewerDirty2);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu3Entry2);

INCLUDE_SDATA(const s32, "effect/effEvent", filterDebugMenu3Entry3);

INCLUDE_SDATA(const s32, "effect/effEvent", evtCallGetProcessData19Data);

INCLUDE_SDATA(const s32, "effect/effEvent", effCallMemcpy3Data);

INCLUDE_SDATA(const s32, "effect/effEvent", evtUpdateEffectViewerGroupData2);

INCLUDE_SDATA(const s32, "effect/effEvent", effLabelDistS);

s32 EffEveCPSmokeCount = 0;

addr_t hEffEveCPSmokeBill01Tex = 0;

addr_t hEffEveCPSmokeBill02Tex = 0;

INCLUDE_SDATA(const s32, "effect/effEvent", evtUpdateEffectViewerGroupData3);

