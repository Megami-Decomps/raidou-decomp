/* Declarations of smt3-decomp src/smt3/model/mdlEffect.c, for its bodies in this directory. */
#ifndef SHARED_MODEL_MDLEFFECT_H
#define SHARED_MODEL_MDLEFFECT_H

#include "common.h"

#include "libsdf.h"

#include "model.h"

#include "kernel.h"

#include "newdata.h"

#include "ps2asm.h"

#include "motion.h"

#include "sdfFrame.h"

/* Declared here because no header carries these. */
extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern u32 dds3GetKernelCount(void);

typedef struct mdlBillboard_t mdlBillboard_t;

typedef struct mdlEffect_t mdlEffect_t;

typedef struct mdlTrackWork_t mdlTrackWork_t;

typedef struct mdlEffectResrc_IPU_t mdlEffectResrc_IPU_t;

typedef struct mdlEffectResrcItem_t mdlEffectResrcItem_t;

typedef struct mdlMarkPos_t mdlMarkPos_t;

typedef struct pibHeaderChunk_t {
    s32 group;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
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
        u32 word10; /* 0x10: group-four stream parameter */
        struct {
            u16 slotCount; /* Kind 1/2 group records: following entity-slot packets. */
            u16 unk12;
        } half;
    } tail;
} pibHeaderChunk_t;

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

typedef struct mdlTrackParams_t {
    u8 opaque00[8];
    s32 mark0;
    s32 mark1;
    u16 start;
    u16 end;
    u16 interval;
    u8 face;
    u8 blend;
    u32 colors[4];
} mdlTrackParams_t;

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

extern sdfGrOt_t sdfGrOtTable[];

extern s8 dds3PadButtonState[];

extern s8 dds3PadBitTable[];

extern f32 D_002FA240[16];

extern u8 aViewEye[];

extern mdlLightParams_t dds3LightParam;

extern mdlDebugLight_t mdlDebugLightWork;

extern mdlDebugScale_t mdlDebugScaleWork;

extern mdlDebugEntry_t mdlDebugEntryTable[];

extern mdlDebugOption_t softenBiasWork;

extern mdlDebugLabel_t mdlDebugLightLabels[4], mdlDebugScaleFogLabel, softenBiasLabels[4];

extern u16 mdlDebugLightCursorY[], softenBiasCursorY[4] __attribute__((section(".data")));

extern u8 corePktSoftenBias __attribute__((section(".sdata")));

extern f32 sdfFaceDivSoften, sdfFaceDivBias;

extern char *mdlColorChannelLabels[];

extern u8 mdlColorChannelStyle[];

extern u16 mdlPibTrackRowOffset[];

extern void *sdfChipAllocZeroDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void *dds3BillboardCreate(s32, addr_t);

extern void dds3BillboardDelete(mdlBillboard_t *);

extern mdlBillboard_t *dds3BillboardCopy(mdlBillboard_t *);

extern void *dds3EffectLoad(void *pD3P);

extern void dds3EffectDelete(mdlEffect_t *);

extern mdlEffect_t *dds3EffectCopy(mdlEffect_t *);

extern void sdfIpuFree(void *cb);

extern void sdfIpuSetOutputTex(void *, mdlIpuParams_t *, u8 *, s32, sdfTexHandle_t *);

extern sdfItemArray_t *sdfItemArrayNew(u32, s32, s32);

extern void sdfItemArrayFree(sdfItemArray_t *);

extern mdlMarkPos_t *sdfModelSearchMarkPosData(sdfModelRootNode_t *, s32);

extern mdlTrackWork_t *Eff_ModelTrackPolyCreate(mdlTrackCreateParams_t *);

extern void Eff_ModelTrackPolyDelete(mdlTrackWork_t *);

extern void Eff_ModelTrackPolyDraw(mdlTrackWork_t *);

extern void mdlSetBlur(mdlObj_t *, s32, s32, u32, u32);

extern sdfTexHandle_t *sdfTexSearch(s32 id);

extern sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *, u32);

extern void dds3BillboardSetPosition(void *, const void *);

extern void dds3BillboardDraw(mdlBillboard_t *);

extern void dds3EffectSetPosition(mdlEffect_t *, const void *);

extern void dds3EffectDraw(mdlEffect_t *);

extern void dds3BillboardSetRGBA(mdlBillboard_t *, s32);

extern void dds3EffectSetRGBA(mdlEffect_t *, s32);

extern void dds3BillboardSetScale(mdlBillboard_t *, f32, f32);

extern void dds3EffectSetScale(mdlEffect_t *, f32);

extern void *sdfDevPrintf(s32, s32, s32, s32, const char *, ...);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t);

extern void *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void dds3DebugIntEditEx(void *, s32, s64, s64, s64, s64, s8 *);

extern void *dds3FontReqAllocDrawTag(void);

extern void FromRotation(f32);

extern void ToMatrix(void);

extern s32 dds3DebugFloatEditEx(f32 *, u8 *, f32, f32, f32, f32);

extern s32 dds3DebugColorEditEx(u32 *, s32, s8 *);

extern void dds3PrimDebugTextBox(sdfDrawTag_t *, s32, s32, s32, f32 *);

extern void dds3PrimDebugTextBoxRGB(void *, s32, s32, s32, u32, s32);

extern void *dds3PrimLine3D(s32, s32, s32, s32);

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

extern char mdlPibLabelP2A[];

extern char mdlPibLabelD3P[];

extern char mdlColorByteFormat[];

extern char mdlPibCursorMark[];

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

/* Read the PIB file version without following its relative link. */
u32 mdlPibGetVersion(pibHeaderChunk_t *record);

/* The PIB chunk count is the low half of the packed parameter word. */
u16 mdlPibGetChunkCount(pibHeaderChunk_t *record);

/* Follow the relative links in a resource's record table to find an ID. */
pibHeaderChunk_t *mdlPibGetChunk(mdlObj_t *handle, s32 group);

/* The first eight bytes belong to the enclosing list, not its first record. */
pibHeaderChunk_t *mdlPibGetFirstChunkItem(pibHeaderChunk_t *data);

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
pibHeaderChunk_t *mdlPibGetNextChunkItem(pibHeaderChunk_t *data);

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlPibGetChunkItemCount(pibHeaderChunk_t *data);

s32 mdlPibCheckItemType(pibHeaderChunk_t *data, s32 type);

u16 mdlPibGetEffectCount(pibHeaderChunk_t *record);

/* Read the PIB motion count from its packed halfword. */
u16 mdlPibGetMotionCount(const pibHeaderChunk_t *record);

void mdlPibDrawEffectInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, s32 attr, pibHeaderChunk_t *data);

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlPibDrawTrackInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, mdlTrackParams_t *data, s32 cursor);

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlPibEditTrackInfo(mdlTrackParams_t *data, s16 *cursor_p);

/* Append a newly created billboard to the next part-list slot. */
void mdlCreateEffectResrc_P2A(sdfItemArray_t *partList, void *descriptor);

/* Append a newly created effect to the next part-list slot. */
void mdlCreateEffectResrc_D3P(sdfItemArray_t *partList, void *descriptor);

void mdlCreateEffectResrc_IPU(sdfItemArray_t *list, u8 *sourceAddress, sdfMemHandle_t *backingAllocation, s32 sourceSize);

void mdlDeleteEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu);

void mdlStartEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu, sdfTexHandle_t *tex, mdlIpuParams_t *parm);

sdfItemArray_t *mdlEffectResrcListNew(u32 initCapa);

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlEffectResrcListFree(sdfItemArray_t *efcResList);

/* Prepend a cleared, typed resource item to the owner's list. */
dds3ModelEffectCtrlr_t *mdlCreateEffectCtrlr(mdlObj_t *aHandle, s32 aType, s32 aPlayGroup);

void *mdlEffectDup_P2A(mdlEffectResrcItem_t *efcResPtr);

void *mdlEffectDup_D3P(mdlEffectResrcItem_t *efcResPtr);

/* Resolve a 16-byte part slot; retail checks only the upper index bound. */
mdlEffectResrcItem_t *mdlGetEffectResrc(mdlObj_t *handle, s32 effectIndex);

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlCreateEffect_P2A_D3P(mdlObj_t *owner, pibChunkItem_P2A_D3P_t *partRecord, s32 motionSlotIndex, s32 resourceType, void *(*createPart)(mdlEffectResrcItem_t *));

/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateEffect_TRACK(mdlObj_t *owner, pibChunkItem_TRACK_t *record, s32 motionSlotIndex);

void mdlCreateEffect_BLUR(mdlObj_t *owner, pibHeaderChunk_t *record);

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlCreateEffect_IPU(mdlObj_t *aHandle, mdlPibIpu_t *aData, s32 aPlayGroup);

/* Start the IPU once its owner reaches the stored motion-frame threshold. */
void mdlDrawEffect_IPU(dds3ModelEffectCtrlr_t *effect);

/* Dispatch the five record kinds, retaining the game's native return behavior. */
void mdlCreateEffect(mdlObj_t *owner, pibHeaderChunk_t *record, s32 motionSlotIndex);

/* Find the requested record list and associate each entry with this motion slot. */
void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup);

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr);

/* Release all items for this slot without losing the link across consecutive matches. */
void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup);

/* vu0 routine: transform an anchor, then offset it away from the eye. */
void mdlEffect_P2A_D3P_GetPos(sdfModelRootNode_t *model, dds3ModelEffectCtrlr_t *effect, f32 *vect);

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlLinkEffect(mdlObj_t *owner, dds3ModelEffectCtrlr_t *anchorRecord);

/* Set billboard/effect color; other controller kinds have no RGBA dispatch. */
void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor);

/* Set billboard/effect scale; other controller kinds have no scale dispatch. */
void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, float aScale);

/* vu0 routine: rotate the edited light direction about the view's X/Y axes. */
void mdlSetDebugMatrix(mdlDebugLight_t *context, f32 *position);

/* Update/draw the task-selected light and ambient editor; return zero. */
s32 mdlDebugPrintEffectScale(dds3ProcessID_t *task);

void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant);

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlGetDebugEntry(s32 valueEditIndex);

/* Edit the draw-vector target: four numeric rows and three packed-colour channels. */
s32 mdlDebugPrintEffectScale2(dds3ProcessID_t *task);

/* Update/draw the FACE-DIV soften, bias and divide controls; return zero. */
s32 soften_bias_Process(dds3ProcessID_t *task);

#endif
