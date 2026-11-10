#include "common.h"

/* Declarations from smt3-decomp src/smt3/model/mdlEffect.c */
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

extern f32 sdfViewMatrix[16];

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

extern const char mdlDebugLabelAmbient[16];

extern const char softenBiasTitle[16];

extern char mdlPibLabelP2A[8];

extern char mdlPibLabelD3P[8];

extern char mdlColorLabelA[8];

extern char mdlColorLabelB[8];

extern char mdlColorLabelG[8];

extern char mdlColorLabelR[8];

extern char mdlColorByteFormat[8];

extern char mdlPibCursorMark[8];

extern char mdlDebugLabelLight2[8];

extern char mdlDebugLabelLight1[8];

extern char mdlDebugLabelLight0[8];

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

extern s32 dds3BitEditorRowBase;

extern s32 dds3BitEditorTable;

extern s32 dds3BitEditorTable2;

extern char dds3BitEditorGlyphTemplate[8];

extern char dds3BitEditorColumnFormat[8];

extern char dds3BitEditorRowFormat[8];

extern s32 evtNextEventNo;
/* End of declarations from src/smt3/model/mdlEffect.c */

/* Follow the relative links in a resource's record table to find an ID. */
pibHeaderChunk_t *mdlPibGetChunk(mdlObj_t *handle, s32 group) {
    pibHeaderChunk_t *recordTable = handle->sub->effectData;
    pibHeaderChunk_t *recordCursor;
    s32 remainingRecords;

    if (recordTable == NULL) {
        return NULL;
    }
    recordCursor = recordTable;
    remainingRecords = mdlPibGetChunkCount(recordCursor);
    do {
        remainingRecords--;
        recordCursor = EE_ADDR_AS(pibHeaderChunk_t *, (EE_ADDR_S(recordCursor) + recordCursor->nextOffset));
        if (remainingRecords == -1) {
            return NULL;
        }
    } while (recordCursor->group != group);
    return recordCursor;
}

/* The first eight bytes belong to the enclosing list, not its first record. */
pibHeaderChunk_t *mdlPibGetFirstChunkItem(pibHeaderChunk_t *data) {
    pibHeaderChunk_t *firstRecord = (pibHeaderChunk_t *)((u8 *)data + MDL_RECORD_LIST_HEADER_BYTES);

    if (firstRecord->group == MDL_RECORD_END_KIND) {
        firstRecord = NULL;
    }
    return firstRecord;
}

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
pibHeaderChunk_t *mdlPibGetNextChunkItem(pibHeaderChunk_t *data) {
    pibHeaderChunk_t *nextRecord = EE_ADDR_AS(pibHeaderChunk_t *, (EE_ADDR_S(data) + data->nextOffset));

    if (nextRecord->group == MDL_RECORD_END_KIND) {
        nextRecord = NULL;
    }
    return nextRecord;
}

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlPibGetChunkItemCount(pibHeaderChunk_t *data) {
    s32 recordCount;
    pibHeaderChunk_t *recordCursor;

    if (data == NULL) {
        return 0;
    }
    recordCursor = mdlPibGetFirstChunkItem(data);
    recordCount = 0;
    while (recordCursor != NULL) {
        recordCount++;
        recordCursor = mdlPibGetNextChunkItem(recordCursor);
    }
    return recordCount;
}

s32 mdlPibCheckItemType(pibHeaderChunk_t *data, s32 type) {
    return data->group == type;
}

u16 mdlPibGetEffectCount(pibHeaderChunk_t *record) {
    return record->parameter.part.partIndex;
}

/* Read the PIB motion count from its packed halfword. */
u16 mdlPibGetMotionCount(const pibHeaderChunk_t *record) {
    return record->tail.half.slotCount;
}

void mdlPibDrawEffectInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, s32 attr, pibHeaderChunk_t *data) {
    if (data->group >= PIB_CHUNK_TYPE_P2A &&
        data->group <= IPU) {
        switch (data->group) {
        case PIB_CHUNK_TYPE_P2A:
        case PIB_CHUNK_TYPE_D3P: {
            char *label = data->group == PIB_CHUNK_TYPE_P2A ? mdlPibLabelP2A : mdlPibLabelD3P;

            if (data->parameter.part.count == 1) {
                sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                                    "%s %d,%d", label, data->payload.word,
                                                                    data->parameter.part.partIndex)));
                return;
            }
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                                "%s %d~%d,%d", label, data->payload.word,
                                                                data->payload.word + data->parameter.part.count - 1,
                                                                data->parameter.part.partIndex)));
            return;
        }
        case PIB_CHUNK_TYPE_TRACK:
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                                "TRACK %d~%d", data->payload.word,
                                                                data->parameter.word)));
            return;
        case PIB_CHUNK_TYPE_BLUR:
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                                "BLUR %d,%d", data->payload.stream.selectorA,
                                                                data->payload.stream.selectorB)));
            return;
        case IPU:
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                                "IPU %08x", data->payload.word)));
            return;
        }
    } else {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y, z, attr,
                                                            "UNKNOWN[%3d]", data->group)));
    }
}

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlPibDrawTrackInfo(sdfDrawTag_t *tag, s32 x, s32 y, s32 z, mdlTrackParams_t *data, s32 cursor) {
    s32 colorY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 packedColor;
    s32 colorIndex;
    s32 channelIndex;

    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y, z, 0, "MARK0:%d", data->mark0)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0x60, z, 0, "MARK1:%d", data->mark1)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0xC0, z, 0, "START:%d", data->start)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0x120, z, 0, "END  :%d", data->end)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0x180, z, 0, "ITRVL:%d", data->interval)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0x1E0, z, 0, "FACE :%d", data->face)));
    sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, y + 0x240, z, 0, "BLEND:%d", data->blend)));
    for (colorIndex = 0; colorIndex != MDL_MARK_COLOR_COUNT; colorIndex++) {
        packedColor = data->colors[colorIndex];
        sdfDrawTagLink(tag, EE_ADDR(dds3PrimFrameRect(x + 0x480, colorY, z, 0x300, 0x180, (packedColor & 0xFFFFFF) | 0x80000000, 0x60404040)));
        for (channelIndex = 0; channelIndex != MDL_COLOR_CHANNEL_COUNT; channelIndex++) {
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(labelX, colorY, z, mdlColorChannelStyle[channelIndex], mdlColorChannelLabels[channelIndex])));
            sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x + 0x240, colorY, z, 0, mdlColorByteFormat, packedColor & MDL_COLOR_BYTE_MASK)));
            packedColor >>= MDL_COLOR_BYTE_BITS;
            colorY += 0x60;
        }
        colorY += 0x60;
    }
    if (cursor >= 0) {
        sdfDrawTagLink(tag, EE_ADDR(sdfDevPrintf(x, y + mdlPibTrackRowOffset[cursor], z, 0, mdlPibCursorMark)));
    }
}

INCLUDE_ASM(const s32, "model/mdlEffect", mdlPibEditTrackInfo);

/* Append a newly created billboard to the next part-list slot. */
void mdlCreateEffectResrc_P2A(sdfItemArray_t *partList, void *descriptor) {
    mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)partList->items)[partList->count];

    partEntry->count = 0;
    partEntry->type = MDL_PART_BILLBOARD;
    partEntry->handle = dds3BillboardCreate(1, EE_ADDR(descriptor));
    partList->count += 1;
}

/* Append a newly created effect to the next part-list slot. */
void mdlCreateEffectResrc_D3P(sdfItemArray_t *partList, void *descriptor) {
    mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)partList->items)[partList->count];

    partEntry->type = MDL_PART_EFFECT;
    partEntry->count = 0;
    partEntry->handle = dds3EffectLoad(descriptor);
    partList->count += 1;
}

void mdlCreateEffectResrc_IPU(sdfItemArray_t *list, u8 *sourceAddress, sdfMemHandle_t *backingAllocation, s32 sourceSize) {
    mdlEffectResrc_IPU_t *node = sdfChipAllocZeroDirect(sizeof(*node));
    mdlEffectResrcItem_t *entry = &((mdlEffectResrcItem_t *)list->items)[list->count];

    node->dataSize = sourceSize;
    node->data = EE_ADDR(sourceAddress);
    node->dataMemHandle = backingAllocation;
    entry->type = MDL_PART_OBJECT;
    entry->count = 0;
    entry->handle = node;
    list->count += 1;
}

void mdlDeleteEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu) {
    if (ipu->playing != 0) {
        sdfIpuFree(&ipu->ipuState);
    }
    sdfMemFreeDirect(ipu->dataMemHandle);
    sdfChipFreeDirect(ipu);
}

void mdlStartEffectResrc_IPU(mdlEffectResrc_IPU_t *ipu, sdfTexHandle_t *tex, mdlIpuParams_t *parm) {
    if (ipu->playing == 0) {
        ipu->playing = 1;
        sdfIpuSetOutputTex(&ipu->ipuState, parm, EE_ADDR_AS(u8 *, ipu->data), ipu->dataSize, tex);
    }
}

sdfItemArray_t *mdlEffectResrcListNew(u32 initCapa) {
    return sdfItemArrayNew(initCapa, 0x10, 4);
}

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlEffectResrcListFree(sdfItemArray_t *efcResList) {
    s32 partIndex;

    if (efcResList != NULL) {
        for (partIndex = 0; partIndex < efcResList->count; partIndex++) {
            mdlEffectResrcItem_t *partEntry = &((mdlEffectResrcItem_t *)efcResList->items)[partIndex];

            switch (partEntry->type) {
            case MDL_PART_BILLBOARD:
                dds3BillboardDelete(partEntry->handle);
                break;
            case MDL_PART_EFFECT:
                dds3EffectDelete(partEntry->handle);
                break;
            case MDL_PART_OBJECT:
                mdlDeleteEffectResrc_IPU(partEntry->handle);
                break;
            }
        }
        sdfItemArrayFree(efcResList);
    }
}

/* Prepend a cleared, typed resource item to the owner's list. */
dds3ModelEffectCtrlr_t *mdlCreateEffectCtrlr(mdlObj_t *aHandle, s32 aType, s32 aPlayGroup) {
    dds3ModelEffectCtrlr_t *item = sdfChipAllocZeroDirect(sizeof(dds3ModelEffectCtrlr_t));
    dds3ModelEffectCtrlr_t *previousHead = aHandle->resourceItems;
    item->type = aType;
    item->next = previousHead;
    item->playGroup = aPlayGroup;
    aHandle->resourceItems = item;
    return item;
}

void *mdlEffectDup_P2A(mdlEffectResrcItem_t *efcResPtr) {
    struct mdlBillboard_t *clone = dds3BillboardCopy(efcResPtr->handle);
    efcResPtr->count = efcResPtr->count + 1;
    return clone;
}

void *mdlEffectDup_D3P(mdlEffectResrcItem_t *efcResPtr) {
    mdlEffect_t *clone = dds3EffectCopy(efcResPtr->handle);
    efcResPtr->count = efcResPtr->count + 1;
    return clone;
}

/* Resolve a 16-byte part slot; retail checks only the upper index bound. */
mdlEffectResrcItem_t *mdlGetEffectResrc(mdlObj_t *handle, s32 effectIndex) {
    sdfItemArray_t *slotTable;

    slotTable = handle->sub->effectResrcList;
    if (slotTable == NULL) {
        return NULL;
    }
    if (effectIndex >= slotTable->count) {
        return NULL;
    }
    return (mdlEffectResrcItem_t *)((u8 *)slotTable->items + effectIndex * MDL_PART_SLOT_BYTES);
}

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlCreateEffect_P2A_D3P(mdlObj_t *owner, pibChunkItem_P2A_D3P_t *partRecord, s32 motionSlotIndex, s32 resourceType, void *(*createPart)(mdlEffectResrcItem_t *)) {
    mdlEffectResrcItem_t *partSlot = mdlGetEffectResrc(owner, partRecord->effectIndex);

    if (partSlot != NULL) {
        sdfModelRootNode_t *model = owner->inner;
        s32 recordId = partRecord->markId;
        s32 remainingRecords = partRecord->count;
        f32 optionalAnchorScale = 0.0f;

        /* Retain the native 17-byte size threshold for the optional anchor scale. */
        if (partRecord->size >= MDL_PART_VALUE_SIZE_THRESHOLD) {
            optionalAnchorScale = partRecord->zOffset;
        }
        do {
            mdlMarkPos_t *chunkRecord = sdfModelSearchMarkPosData(model, recordId++);

            if (chunkRecord != NULL) {
                dds3ModelEffectCtrlr_t *resourceItem = mdlCreateEffectCtrlr(owner, resourceType, motionSlotIndex);

                resourceItem->payload.part.instance = createPart(partSlot);
                resourceItem->payload.part.slot = partSlot;
                resourceItem->payload.part.mapPositionRecord = chunkRecord;
                resourceItem->payload.part.anchorScale = optionalAnchorScale;
            }
        } while (--remainingRecords != 0);
    }
}

/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateEffect_TRACK(mdlObj_t *owner, pibChunkItem_TRACK_t *record, s32 motionSlotIndex) {
    mdlTrackCreateParams_t effectParams;
    dds3ModelEffectCtrlr_t *resourceItem;

    effectParams.model = owner;
    effectParams.idA = record->mark;
    effectParams.idB = record->endPositionId;
    effectParams.startFrame = record->startFrame;
    effectParams.endFrame = record->endFrame;
    effectParams.sampleInterval = record->interval;
    effectParams.historyLength = record->face;
    effectParams.unk1C = 3;
    effectParams.kind = record->blendAlgo;
    effectParams.gradientColors[0] = record->colors[0];
    effectParams.gradientColors[1] = record->colors[1];
    effectParams.gradientColors[2] = record->colors[2];
    effectParams.gradientColors[3] = record->colors[3];
    resourceItem = mdlCreateEffectCtrlr(owner, dds3ModelEffectType_TRACK, motionSlotIndex);
    resourceItem->payload.part.track = Eff_ModelTrackPolyCreate(&effectParams);
}

void mdlCreateEffect_BLUR(mdlObj_t *owner, pibHeaderChunk_t *record) {
    mdlSetBlur(owner, record->payload.stream.selectorA,
                  record->payload.stream.selectorB,
                  record->parameter.word, record->tail.word10);
}

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlCreateEffect_IPU(mdlObj_t *aHandle, mdlPibIpu_t *aData, s32 aPlayGroup) {
    mdlEffectResrcItem_t *partSlot = mdlGetEffectResrc(aHandle, aData->index);

    if (partSlot != 0) {
        mdlEffectResrc_IPU_t *object = partSlot->handle;
        if (object->inUse == 0) {
            sdfTexHandle_t *texture = sdfTexSearch(aData->dataId);
            if (texture != 0) {
                dds3ModelEffectCtrlr_t *resourceItem;
                u8 *attributes;
                object->inUse = 1;
                resourceItem = mdlCreateEffectCtrlr(aHandle, dds3ModelEffectType_IPU, aPlayGroup);
                /* Fill the object and resource data before attaching the owner. */
                resourceItem->payload.object.object = object;
                attributes = resourceItem->payload.object.attributes;
                resourceItem->payload.object.texture = texture;
                resourceItem->payload.object.owner = aHandle;
                resourceItem->payload.object.minimumTime = aData->minimumTime;
                attributes[1] = 1;
                attributes[2] = aData->loopMode;
                attributes[3] = aData->playbackCadenceStep;
            }
        }
    }
}

/* Start the IPU once its owner reaches the stored motion-frame threshold. */
void mdlDrawEffect_IPU(dds3ModelEffectCtrlr_t *effect) {
    mdlEffectResrc_IPU_t *object = effect->payload.object.object;
    if (object->playing == 0) {
        s32 minimumTime = effect->payload.object.minimumTime;
        f32 motionTime = effect->payload.object.owner->first->frame;
        if ((u32)(s32)motionTime < (u32)minimumTime) {
            return;
        }
        mdlStartEffectResrc_IPU(object, effect->payload.object.texture,
                   (mdlIpuParams_t *)effect->payload.object.attributes);
    }
}

/* Dispatch the five record kinds, retaining the game's native return behavior. */
void mdlCreateEffect(mdlObj_t *owner, pibHeaderChunk_t *record, s32 motionSlotIndex) {
    switch (record->group) {
    case PIB_CHUNK_TYPE_P2A:
        mdlCreateEffect_P2A_D3P(owner, (pibChunkItem_P2A_D3P_t *)record, motionSlotIndex, dds3ModelEffectType_P2A, mdlEffectDup_P2A);
        return;
    case PIB_CHUNK_TYPE_D3P:
        mdlCreateEffect_P2A_D3P(owner, (pibChunkItem_P2A_D3P_t *)record, motionSlotIndex, dds3ModelEffectType_D3P, mdlEffectDup_D3P);
        return;
    case PIB_CHUNK_TYPE_TRACK:
        mdlCreateEffect_TRACK(owner, (pibChunkItem_TRACK_t *)record, motionSlotIndex);
        return;
    case PIB_CHUNK_TYPE_BLUR:
        mdlCreateEffect_BLUR(owner, record);
        return;
    case IPU:
        mdlCreateEffect_IPU(owner, (mdlPibIpu_t *)record, motionSlotIndex);
        break;
    }
}

/* Find the requested record list and associate each entry with this motion slot. */
void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup) {
    pibHeaderChunk_t *recordList = mdlPibGetChunk(aHandle, aGroup);
    if (recordList != NULL) {
        pibHeaderChunk_t *recordCursor = mdlPibGetFirstChunkItem(recordList);
        while (recordCursor != NULL) {
            mdlCreateEffect(aHandle, recordCursor, aPlayGroup);
            recordCursor = mdlPibGetNextChunkItem(recordCursor);
        }
    }
}

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlFreeEffect(dds3ModelEffectCtrlr_t *aCtrlr) {
    switch (aCtrlr->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardDelete((mdlBillboard_t *)aCtrlr->payload.part.instance);
        break;
    case dds3ModelEffectType_D3P:
        dds3EffectDelete((mdlEffect_t *)aCtrlr->payload.part.instance);
        break;
    case dds3ModelEffectType_TRACK:
        Eff_ModelTrackPolyDelete(aCtrlr->payload.part.track);
        break;
    }
    sdfChipFreeDirect((void *)aCtrlr);
}

/* Release all items for this slot without losing the link across consecutive matches. */
void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup) {
    dds3ModelEffectCtrlr_t **itemLink = &aHandle->resourceItems;
    dds3ModelEffectCtrlr_t *item = *itemLink;
    while (item != 0) {
        if (item->playGroup == aPlayGroup) {
            dds3ModelEffectCtrlr_t *nextItem = item->next;
            mdlFreeEffect(item);
            *itemLink = nextItem;
            item = nextItem;
        } else {
            itemLink = &item->next;
            item = item->next;
        }
    }
}

/* vu0 routine: transform an anchor, then offset it away from the eye. */
void mdlEffect_P2A_D3P_GetPos(sdfModelRootNode_t *model, dds3ModelEffectCtrlr_t *effect, f32 *vect) {
    mdlMarkPos_t *mapPosition = effect->payload.part.mapPositionRecord;
    sdfModelNode_t *drawNode = sdfModelSearchNode(model, mapPosition->nodeId);
    f32 scale = effect->payload.part.anchorScale;

    SDF_VU0_LOADMATRIX(&drawNode->tm);
        SDF_VU0_LOADVECTOR( &mapPosition->position);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR_C( aViewEye);
    SDF_VU0_SUBVECTOR_A_C();
    SDF_VU0_NORMALIZE();
    SDF_VU0_SCALEVECTOR_BITS(scale);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM( vect);
}

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlLinkEffect(mdlObj_t *owner, dds3ModelEffectCtrlr_t *anchorRecord) {
    sdfModelRootNode_t *model = owner->inner;
    sdfVector4_t position;
    void *resourceInstance;

    switch (anchorRecord->type) {
    case dds3ModelEffectType_P2A:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, &position.x);
        resourceInstance = anchorRecord->payload.part.instance;
        dds3BillboardSetPosition(resourceInstance, &position);
        dds3BillboardDraw((mdlBillboard_t *)resourceInstance);
        break;
    case dds3ModelEffectType_D3P:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, &position.x);
        resourceInstance = anchorRecord->payload.part.instance;
        dds3EffectSetPosition((mdlEffect_t *)resourceInstance, &position);
        dds3EffectDraw((mdlEffect_t *)resourceInstance);
        break;
    case dds3ModelEffectType_TRACK:
        Eff_ModelTrackPolyDraw(anchorRecord->payload.part.track);
        break;
    case dds3ModelEffectType_IPU:
        mdlDrawEffect_IPU(anchorRecord);
        break;
    }
}

/* Set billboard/effect color; other controller kinds have no RGBA dispatch. */
void mdlSetEffectColorSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, u32 aColor) {
    switch (aEffect->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardSetRGBA((mdlBillboard_t *)aEffect->payload.part.instance, aColor);
        return;
    case dds3ModelEffectType_D3P:
        dds3EffectSetRGBA((mdlEffect_t *)aEffect->payload.part.instance, aColor);
        break;
    }
}

/* Set billboard/effect scale; other controller kinds have no scale dispatch. */
void mdlSetEffectScaleSub(mdlObj_t *aHandle, dds3ModelEffectCtrlr_t *aEffect, float aScale) {
    switch (aEffect->type) {
    case dds3ModelEffectType_P2A:
        dds3BillboardSetScale((struct mdlBillboard_t *)aEffect->payload.part.instance, aScale, aScale);
        return;
    case dds3ModelEffectType_D3P:
        dds3EffectSetScale((mdlEffect_t *)aEffect->payload.part.instance, aScale);
        break;
    }
}

char mdlPibLabelP2A[8] = "P2A";

char mdlPibLabelD3P[8] = "D3P";

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF608);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF610);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF618);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF620);

char mdlColorByteFormat[8] = "%02X";

char mdlPibCursorMark[8] = ">";

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F50);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F60);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F70);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F80);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3F90);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FA0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FB0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FC0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FD0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FE0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B3FF0);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4000);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4010);

INCLUDE_RODATA(const s32, "model/mdlEffect", D_004B4020);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF638);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF640);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF648);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF650);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF658);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF660);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF668);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF670);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF678);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF680);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF688);

INCLUDE_SDATA(const s32, "model/mdlEffect", D_004DF690);

