#include "common.h"
#include "libsdf.h"
#include "model.h"
#include "mdlEffect.h"
#include "kernel.h"
#include "newdata.h"
#include "ps2asm.h"
#include "motion.h"
#include "sdfFrame.h"
#include "sdfQuat.h"
#include "dds3DebugStuff.h"

typedef struct mdlBillboard_t mdlBillboard_t;

typedef struct mdlEffect_t mdlEffect_t;

typedef struct mdlTrackWork_t mdlTrackWork_t;

typedef struct mdlEffectResrc_IPU_t mdlEffectResrc_IPU_t;

typedef struct mdlEffectResrcItem_t mdlEffectResrcItem_t;

typedef struct mdlMarkPos_t mdlMarkPos_t;

typedef struct mdlEffectResrcItem_t {
    u32 type;       /* 0x00 */
    s32 count;      /* 0x04 */
    void *handle;   /* 0x08 */
    u8 opaque0C[4]; /* 0x0C */
} mdlEffectResrcItem_t;

typedef struct pibChunkItem_P2A_D3P_t {
    u8 type[4];
    u32 size;          /* 0x04: checked against the optional anchor-scale extent */
    s32 markId;       /* 0x08 */
    u16 count;         /* 0x0C */
    u16 effectIndex;     /* 0x0E */
    f32 zOffset;   /* 0x10: read when size >= 0x11 */
} pibChunkItem_P2A_D3P_t;

typedef struct mdlPibIpu_t {
    u8 opaque00[8];
    s32 dataId;        /* 0x08: resource-key lookup */
    s32 minimumTime;   /* 0x0C: deferred initialization threshold */
    u8 playbackCadenceStep; /* 0x10: mdlIpuParams_t byte 3 */
    u8 loopMode;            /* 0x11: mdlIpuParams_t byte 2 */
    u16 index;         /* 0x12: part-slot index */
} mdlPibIpu_t;

typedef struct pibChunkItem_TRACK_t {
    u8 pad00[8];
    s32 mark;   /* +0x08: first sampled endpoint lookup */
    s32 endPositionId;     /* +0x0C: second sampled endpoint lookup */
    u16 startFrame;        /* +0x10: inclusive update start */
    u16 endFrame;          /* +0x12: inclusive update end */
    u16 interval;    /* +0x14: update-count modulus */
    u8 face;      /* +0x16: history allocation input */
    u8 blendAlgo;           /* +0x17: selected by the track's draw data */
    u32 colors[4]; /* +0x18: copied to mdlTrackCreateParams_t */
} pibChunkItem_TRACK_t;

typedef struct mdlMarkPos_t {
    u32 nodeId;      /* 0x00: draw-node lookup key */
    s32 id;          /* 0x04: map-position record lookup key */
    u8 reserved08[8];
    u128 position;   /* 0x10 */
    u128 up;         /* 0x20 */
    u128 direction;  /* 0x30 */
} mdlMarkPos_t;

typedef struct mdlDebugLight_t {
    f32 (**lights)[4];
    f32 *ambient;
    struct mdlDebugPad_t *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    struct sdfDrawTag_t *packetList;
} mdlDebugLight_t;

typedef struct mdlDebugLabel_t {
    u16 x;              /* 0x00 */
    u16 y;              /* 0x02 */
    s32 style;          /* 0x04 */
    const char *format; /* 0x08 */
} mdlDebugLabel_t;

typedef struct mdlDebugPad_t {
    u8 pad00;
    s8 confirm;
    u8 pad02;
    s8 cancel;
    s8 stepDownA;  /* 0x04 */
    s8 stepUpA;    /* 0x05 */
    s8 stepDownB;  /* 0x06 */
    s8 stepUpB;    /* 0x07 */
} mdlDebugPad_t;

typedef struct mdlDebugScale_t {
    mdlDebugPad_t *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    sdfDrawTag_t *packetList;
} mdlDebugScale_t;

typedef struct mdlDebugEntry_t {
    s32 y;
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    const char *label;  /* 0x10 */
    const char *format; /* 0x14 */
} mdlDebugEntry_t;

typedef union mdlDebugOptionCtrl_t {
    u32 packed;
    struct {
        u8 active;
        u8 blink;
        s16 cursor;
    } fields;
} mdlDebugOptionCtrl_t;

typedef struct mdlDebugOption_t {
    s8 *pad;
    mdlDebugOptionCtrl_t control;
    u8 pad08[0x20];
    f32 value;
    f32 adjustment;
    sdfDrawTag_t *packetList;
    u8 pad34[4];
} mdlDebugOption_t;

typedef struct { u8 hasAudio, stereo, loopMode, playbackCadenceStep; } mdlIpuParams_t;

struct mdlEffectResrc_IPU_t
{
    addr_t data; /* EE byte address handed to the IPU stream initializer. */
    sdfMemHandle_t *dataMemHandle;
    u8 inUse, playing;
    u8 unk0A[6];
    s32 dataSize;
    u8 unk14[0xC];
    u8 ipuState[0x8C];
};

typedef struct
{
    mdlObj_t *model;
    s32 idA, idB;
    f32 startFrame, endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
} mdlTrackCreateParams_t;

typedef struct
{
    f32 (*lights[4])[4];
    f32 ambient[4];
    u8 unk20[0x10];
    u32 fogColor;
} mdlLightParams_t;

typedef void (*mdlOtRegFunc_t)(sdfGrOt_t *, sdfDrawTag_t *);
extern f32 sdfViewMatrix[16];
extern mdlDebugScale_t mdlDebugScaleWork;
extern mdlDebugEntry_t mdlDebugEntryTable[];
extern mdlDebugLabel_t mdlDebugLightLabels[4], mdlDebugScaleFogLabel, softenBiasLabels[4];
extern u16 mdlDebugLightCursorY[], softenBiasCursorY[4] __attribute__((section(".data")));
extern f32 sdfFaceDivSoften, sdfFaceDivBias;

/* dds3ModelEffectCtrlr_t.type: which effect a controller drives. */
typedef enum dds3ModelEffectType {
    dds3ModelEffectType_P2A = 0,
    dds3ModelEffectType_D3P = 1,
    dds3ModelEffectType_TRACK = 2,
    dds3ModelEffectType_IPU = 3
} dds3ModelEffectType;

/* pibHeaderChunk_t.kind, as mdlCreateEffect dispatches on it. */
typedef enum PIB_CHUNK_TYPE {
    PIB_CHUNK_TYPE_P2A = 1,
    PIB_CHUNK_TYPE_D3P = 2,
    PIB_CHUNK_TYPE_TRACK = 3,
    PIB_CHUNK_TYPE_BLUR = 4,
    IPU = 5,
    PIB_CHUNK_TYPE_END = 0xFFFF
} PIB_CHUNK_TYPE;

extern char mdlDebugLightCursor[];
extern char mdlDebugLightFormat[];
extern char mdlDebugScaleCursor[];
extern char softenBiasCursor[];
extern char softenBiasValueFormat[];
extern char softenBiasAdjustFormat[];
extern char softenBiasLabelOn[];
extern char softenBiasLabelOff[];

#define MDL_RECORD_LIST_HEADER_BYTES 8

#define MDL_RECORD_END_KIND 0xFFFF

#define MDL_MARK_COLOR_COUNT 4

#define MDL_COLOR_CHANNEL_COUNT 4

#define MDL_COLOR_BYTE_MASK 0xFF

#define MDL_COLOR_BYTE_BITS 8

#define MDL_MARK_LAST_FIELD 0x16

#define MDL_MARK_COLOR_FIRST_FIELD 7

#define MDL_PAD_REPEAT_FLAG 2

#define MDL_PART_SLOT_BYTES 0x10

#define MDL_PART_VALUE_SIZE_THRESHOLD 0x11

#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

#define MDL_PART_OBJECT 3

void mdlSetDebugMatrix(mdlDebugLight_t *context, f32 *position);
void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant);
void mdlGetDebugEntry(s32 valueEditIndex);
extern char mdlDebugLightCursor[8];
extern char mdlDebugLightFormat[8];
extern char mdlDebugLabelFog[8];
extern char mdlDebugLabelFar[8];
extern char mdlDebugLabelMin[8];
extern char mdlDebugLabelNear[8];
extern char mdlDebugIntFormat[8];
extern char mdlDebugLabelMax[8];
extern char mdlDebugScaleCursor[8];
extern char softenBiasLabelDivide[8];
extern char softenBiasLabelBias[8];
extern char softenBiasLabelSoften[8];
extern char softenBiasCursor[8];
extern char softenBiasValueFormat[8];
extern char softenBiasAdjustFormat[8];
extern char softenBiasLabelOn[8];
extern char softenBiasLabelOff[8];

/* vu0 routine: rotate the edited light direction about the view's X/Y axes. */
void mdlSetDebugMatrix(mdlDebugLight_t *context, f32 *position)
{
    if (context->pad->stepUpA != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR( position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR( position);
    } else if (context->pad->stepDownA != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR( position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR( position);
    }
    if (context->pad->stepUpB != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR( position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR( position);
    } else if (context->pad->stepDownB != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        ToMatrix();
        SDF_VU0_LOADVECTOR( position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR( position);
    }
}

INCLUDE_ASM(const s32, "model/mdlEffect_001A1780", mdlDebugPrintEffectScale);

void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    dds3PrimDebugTextBoxRGB(mdlDebugScaleWork.packetList, first, second,
                  (mdlDebugScaleWork.editing == 0) ? -1 : variant, packedColor | 0x80000000, 1);
}

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlGetDebugEntry(s32 valueEditIndex) {
    mdlDebugEntry_t *edit = &mdlDebugEntryTable[valueEditIndex];
    f32 *target = edit->target;
    s32 value = (s32)(*target + 0.5f);
    s32 stepA;
    s32 stepB;
    mdlDebugPad_t *pad;
    s32 min;
    s32 max;

    if (value < 1000) {
        stepA = 10;
        stepB = 1;
    } else if (value < 10000) {
        stepA = 100;
        stepB = 10;
    } else {
        stepA = 1000;
        stepB = 100;
    }
    min = edit->min;
    pad = mdlDebugScaleWork.pad;
    max = edit->max;
    if (pad->stepUpA & 0x80) {
        if (value < max) {
            value += stepA;
            if (value > max) {
                value = max;
            }
        } else {
            value = min;
        }
    } else if (pad->stepUpA & 2) {
        value += stepA;
        if (value > max) {
            value = max;
        }
    } else if (pad->stepDownA & 0x80) {
        if (min < value) {
            value -= stepA;
            if (value < min) {
                value = min;
            }
        } else {
            value = max;
        }
    } else if (pad->stepDownA & 2) {
        value -= stepA;
        if (value < min) {
            value = min;
        }
    } else if (pad->stepUpB & 0x80) {
        if (value < max) {
            value += stepB;
            if (value > max) {
                value = max;
            }
        } else {
            value = min;
        }
    } else if (pad->stepUpB & 2) {
        value += stepB;
        if (value > max) {
            value = max;
        }
    } else if (pad->stepDownB & 0x80) {
        if (min < value) {
            value -= stepB;
            if (value < min) {
                value = min;
            }
        } else {
            value = max;
        }
    } else if (pad->stepDownB & 2) {
        value -= stepB;
        if (value < min) {
            value = min;
        }
    } else {
        return;
    }
    *target = value;
}

INCLUDE_ASM(const s32, "model/mdlEffect_001A1780", mdlDebugPrintEffectScale2);

INCLUDE_ASM(const s32, "model/mdlEffect_001A1780", soften_bias_Process);
INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLightCursor);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLightFormat);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLabelFog);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLabelFar);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLabelMin);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLabelNear);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugIntFormat);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugLabelMax);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", mdlDebugScaleCursor);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasLabelDivide);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasLabelBias);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasLabelSoften);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasCursor);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasValueFormat);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasAdjustFormat);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasLabelOn);

INCLUDE_SDATA(const s32, "model/mdlEffect_001A1780", softenBiasLabelOff);

