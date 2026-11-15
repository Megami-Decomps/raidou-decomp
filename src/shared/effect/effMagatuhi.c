#include "common.h"
#include "sdfDevCons.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "fpu.h"
#include "effect.h"
#include "effDebug.h"
#include "sdfFrame.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "billManager.h"
#include "sdfTex.h"
#include "sdfMath.h"
#include "sdfPacketBuff.h"
#include "effMath.h"
#include "sdfBillBoard.h"
#include "sdfMemory.h"
#include "effMagatuhi.h"

extern sdfGrOt_t *effMagaGrOtBuckets[4];

/* Creation-table entries return the newly allocated type-specific work. */
typedef struct effCreateHandler_t {
    void *(*handler)(void *);
    u8 unk4[0x14];
} effCreateHandler_t;

extern effCreateHandler_t dds3MagatuhiCreateTable[];

/* Effect callback dispatch entry (0x18). */
typedef struct effHandler_t {
    void (*handler)(void *);
    u8 unk4[0x14];
} effHandler_t;

extern effHandler_t dds3MagatuhiDrawTable[];
extern effHandler_t dds3MagatuhiDeleteTable[];

/* The set-vector, set-colour and set-matrix slots also take an argument. */
typedef struct effVectorHandler_t {
    void (*handler)(void *, void *);
    u8 unk4[0x14];
} effVectorHandler_t;

typedef struct effColorHandler_t {
    void (*handler)(void *, u32);
    u8 unk4[0x14];
} effColorHandler_t;

extern effVectorHandler_t dds3MagatuhiSetRGBATable[];
extern effColorHandler_t dds3MagatuhiSetRGBATable2[];
extern effVectorHandler_t dds3MagatuhiSetRGBATable3[];
extern s16 effMagaBillboardTexCoords[15][8];

#define EFF_BUILTIN_NAME_COUNT 0x2F

#define EFF_DIRECTORY_FLAG_CLEAR 0x1000

#define EFF_DIRECTORY_PATH_BYTES 0x70

#define EFF_MAGATUHI_ALPHA_SHIFT 24

#define EFF_MAGATUHI_BEZIER_GROUP_STRIDE 4

#define EFF_MAGATUHI_CALLBACK_TYPE 3

#define EFF_MAGATUHI_CONTROL_POINT_COUNT 4

#define EFF_MAGATUHI_DELAY_WORD_BYTES 4

#define EFF_MAGATUHI_FADE_ALPHA_SCALE 127.0f

#define EFF_MAGATUHI_FULL_TURN 6.2831853f

#define EFF_MAGATUHI_HALF_TURN 3.14159265f

#define EFF_MAGATUHI_HISTORY_MINIMUM 8

#define EFF_MAGATUHI_MATRIX_WORD_COUNT 16

#define EFF_MAGATUHI_NEUTRAL_COLOR 0x80808080

#define EFF_MAGATUHI_NEUTRAL_RGB 0x808080

#define EFF_MAGATUHI_RANDOM_MIDPOINT 0.5f

#define EFF_MAGATUHI_REPLAY_BEZIER 1

#define EFF_MAGATUHI_REPLAY_DRIFT 4

#define EFF_MAGATUHI_REPLAY_GROUP_STRIDE 3

#define EFF_MAGATUHI_REPLAY_NONE 0

#define EFF_MAGATUHI_REPLAY_ORBIT 3

#define EFF_MAGATUHI_REPLAY_RING 2

#define EFF_MAGATUHI_VECTOR_WORD_COUNT 4

#define EFF_MAGATUHI_XYZ_COMPONENT_COUNT 3

#define EFF_SUBWORK_PREFIX_BYTES 0x40

/* SDK directory entry filled by the directory-read provider. */
typedef struct effDirEnt_t {
    u32 flags;
    u8 opaque04[0x3C];
    char name[0x100];
    void *privateData;
} effDirEnt_t;

/* Drift parameters are not ring parameters despite their equal size (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 loop;
    u8 pad11[3];
    s32 lifetimeFrames;
    s32 delaySpread;
    s32 fadeIn;
    s32 fadeOut;
    f32 radius;
    f32 up_s;
    f32 up_s_r;
    f32 angleStep;
    f32 ampli;
    f32 scaleStep;
    f32 ampli_r;
    u32 particleCount;
    u32 reserve;
    u8 pad48[0x94];
} effMagaSmokeParam_t;

/* Shared 0x30-byte state: base position, planar drift, and accumulated lift. */
typedef struct {
    f32 pos[EFF_MAGATUHI_XYZ_COMPONENT_COUNT];
    u8 pad0C[4];
    f32 dirUp[EFF_MAGATUHI_XYZ_COMPONENT_COUNT]; /* XZ direction, Y accumulated lift */
    u8 pad1C[4];
    s32 age; /* negative: waiting to start; nonnegative: elapsed frames */
    f32 amplitude;
    f32 angle;
    f32 up_s;
} effMagaSmokePart_tag;

/* Seven copied words; the parent appends its independently allocated history. */
typedef struct effMagatuhiHistoryParams_t {
    s32 count;
    s32 historyCount;
    f32 historyDistanceScale;
    u32 colorA;
    u32 colorB;
    s32 drawBucket;
    f32 historyAngleStep;
} effMagatuhiHistoryParams_t;

/* History arrays precede this 0x38-byte owner in one retained allocation. */
/* One row of the position and angle tables, so an address can be cast to a
 * row pointer through a type name. */
typedef f32 effMagatuhiRow_t[4];

typedef struct dds3MagatuhiBill_tag {
    s32 count;
    u16 historyCount;
    u8 pad06[2];
    f32 historyDistanceScale; /* Segment length is clamped to three times this value. */
    f32 historyAngleStep; /* Radians per history update. */
    u32 drawBucket;
    f32 (*positions)[4];
    u32 *colorTable;
    f32 *historyScalars;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    f32 (*angleRows)[4];
    sdfTexHandle_t *texture;
    struct sdfMemHandle_t *allocationHandle;
} dds3MagatuhiBill_tag;

typedef struct effMagatuhiOwner_t {
    effMagatuhiHistoryParams_t params;
    dds3MagatuhiBill_tag *valueWork;
} effMagatuhiOwner_t;

/* The 0x30-byte states precede this 0x12C-byte work allocation. */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    effMagaSmokeParam_t param;
    effMagaSmokePart_tag *pPart;
    u32 abgr;
    effMagatuhiOwner_t *pMagatuhi;
    sdfMemHandle_t *hMem;
} effMagaSmoke_tag;

/* Parameter head of the first family: particles of 0x30 bytes precede the work. */
typedef struct {
    f32 originX, originY, originZ;
    u8 pad0C[4];
    f32 targetX, targetY, targetZ;
    u8 pad1C[4];
    u8 respawn;            /* 0x20 */
    u8 pad21[3];
    s32 life;    /* 0x24 */
    s32 timing;       /* 0x28 modulus of the particle delay */
    s32 fade_in;      /* 0x2C */
    s32 fade_out;     /* 0x30 */
    f32 initialRadius;
    f32 baseLiftStep;      /* 0x38 */
    f32 liftVariation;
    f32 s_angle_s;         /* 0x40 */
    f32 baseScale;         /* 0x44 */
    f32 s_ampli_s;         /* 0x48 */
    f32 scaleVariation;    /* 0x4C */
    f32 captureDistance;   /* 0x50: transition from drift to the path */
    f32 c_ampli;        /* 0x54 */
    f32 c_step;          /* 0x58 */
    f32 c_step_a;  /* 0x5C: multiplier applied to the path step */
    u32 reserve;     /* 0x60 */
    u8 pad64[0x118];
} effMagaSmokeConvParam_t; /* 0x17C */

/* Bezier parameter head: particle count and delay spread at their native offsets. */
typedef struct {
    f32 point[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 loop;            /* 0x40 restart finished particles */
    u8 pad41[3];
    s32 life;    /* 0x44 frames a particle lives */
    s32 timing;       /* 0x48 modulus of the particle delay */
    s32 fade_in;            /* 0x4C */
    s32 fade_out;           /* 0x50 */
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT]; /* 0x54 */
    u32 reserve;     /* 0x64 */
    u32 duration;          /* 0x68 */
    u8 pad6C[0x114];
} effMagaPoint4Param_t; /* 0x180 */

/* Orbit parameters copied into the work record; trailing bytes stay opaque. */
typedef struct effMagaSpiralParam_t {
    f32 origin[4];
    u8 loop;            /* 0x10 */
    u8 pad11[3];
    s32 life;    /* 0x14 */
    s32 timing;       /* 0x18 */
    s32 fade_in;            /* 0x1C */
    s32 fade_out;           /* 0x20 */
    f32 rotate_s;         /* 0x24 */
    f32 rotate_r;       /* 0x28 */
    f32 up_s;        /* 0x2C */
    f32 up_r;      /* 0x30 */
    f32 radius_s;       /* 0x34 */
    f32 radius_e;         /* 0x38 */
    f32 radius_s_r;       /* 0x3C */
    f32 endJitter;         /* 0x40 */
    u32 particleCount;     /* 0x44 */
    u32 reserve;          /* 0x48 */
    u8 pad4C[0x94];
} effMagaSpiralParam_t;

/* One particle slot; replay visits the first slot in each triplet. */
typedef struct effMagaSpiralPart_tag {
    s32 frame;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 up;     /* 0x04 */
    f32 up_s; /* 0x08 */
    f32 rotate;      /* 0x0C */
    f32 rotate_s;  /* 0x10 */
    f32 radius;     /* 0x14 */
    f32 radius_s; /* 0x18 */
} effMagaSpiralPart_tag;

/* Work is followed by the orbit slots in the same retained allocation. */
typedef struct effMagaSpiral_tag {
    f32 matrix[16];
    effMagaSpiralParam_t param;
    effMagaSpiralPart_tag *pPart;
    effMagatuhiOwner_t *pMagatuhi;
    u32 abgr;
    sdfMemHandle_t *hMem;
} effMagaSpiral_tag;

/* Ring parameters copied verbatim into the work at +0x40 (0xDC). */
typedef struct {
    f32 origin[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    u8 loop;
    u8 pad11[3];
    s32 life;
    s32 timing;
    s32 fade_in;
    s32 fade_out;
    f32 rotate_s;
    f32 rotate_r;
    f32 height;
    f32 radius_s;
    f32 radius_e;
    f32 radius_s_r;
    f32 endJitter;
    u32 particleCount;
    u32 reserve;
    u8 pad48[0x94];
} effMagaCylinderParam_t;

/* Per-particle state allocated after EffMagatuhiRingWork by both titles. */
typedef struct effMagaCylinderPart_tag {
    s32 frame;        /* 0x00: negative while waiting; otherwise elapsed frames */
    f32 height;     /* 0x04 */
    f32 rotate;      /* 0x08 */
    f32 rotate_s;  /* 0x0C */
    f32 radius;     /* 0x10 */
    f32 radius_s; /* 0x14 */
} effMagaCylinderPart_tag;

/* Work precedes its array of 0x18-byte ring states (0x12C). */
typedef struct {
    f32 matrix[EFF_MAGATUHI_MATRIX_WORD_COUNT];
    effMagaCylinderParam_t param;
    effMagaCylinderPart_tag *pPart;
    effMagatuhiOwner_t *pMagatuhi;
    u32 abgr;
    sdfMemHandle_t *hMem;
} effMagaCylinder_tag;

typedef struct {
    effMagaSmokeConvParam_t head;
    effMagaSmokePart_tag *pPart; /* 0x17C */
    effArrHdr_t *pBezier4P;    /* 0x180 */
    u8 pad184[8];
    effMagatuhiOwner_t *pMagatuhi; /* 0x18C */
    sdfMemHandle_t *hMem; /* 0x190 backing allocation, not the particle address */
} effMagaSmokeConv_tag;

typedef struct {
    effMagaPoint4Param_t param;
    s32 *pPart;           /* 0x180 */
    effArrHdr_t *pBezier4P;    /* 0x184 */
    effMagatuhiOwner_t *pMagatuhi; /* 0x188 */
    sdfMemHandle_t *hMem; /* 0x18C backing allocation, not the work address */
} effMagaPoint4_tag;

/* Effect subrecord with byte fields at +0x20/+0x40/+0x50 (0x51). */
typedef struct effSub_t {
    u8 unk0[0x20];
    u8 type0SubSlot;
    u8 unk21[0x1F];
    u8 type1SubSlot;
    u8 unk41[0xF];
    u8 type2To4SubSlot;
} effSub_t;

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* Float-input SDK form: retain the broad memory effect and let GCC transfer
 * the binary32 scale to the unpack's GPR operand. The exact surface-grid
 * gradient uses four such 16-byte carriers.
 * This is distinct from the integer-bit, read-only form above. */
static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    MMI_RGBA_UNPACK(rgba, unit);
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
extern u8 dds3EffectBase[];
void dds3Bezier4PDelete(struct effArrHdr_t *pEffBezier4P);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *name);

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */

/* Fixed-scale form of the SDK's four-word RGBA packing scratch buffer. */
static inline u32 PackMagatuhiTrailColor(void) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}


/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Build XYZ and attribute VIF streams; return the packet address.
 * vertexCount must be positive; n retains its native count/byte-size reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */

/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */

/* Requires a non-NULL texture; returns its primary buffer as a borrowed view. */
struct sdfTexPacket_t *sdfTexGetPacket(struct sdfTexHandle_t *aHandle);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];
extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

#include "sdk_libc.h"
#include "sdf3D.h"

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source);
void effMagaCylinderFree(effMagatuhiOwner_t *resource);
void effMagaCylinderDrawTrack(effMagatuhiOwner_t *resource);
void effMagaCylinderSetPartPtrs(dds3MagatuhiBill_tag *work, s32 slotIndex);
dds3MagatuhiBill_tag *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep);
void effMagaCylinderFreeParts(dds3MagatuhiBill_tag *work);
void effSwordTrackDrawTex(dds3MagatuhiBill_tag *work);
void effMagaCylinderSetPartPos(dds3MagatuhiBill_tag *work, s32 index, const sdfVector4_t *vector);
void effMagaCylinderSetPart(dds3MagatuhiBill_tag *work, s32 index, u32 color);
void effMagaCylinderBlendColor(dds3MagatuhiBill_tag *work, u32 colorA, u32 colorB);
void effMagaSmokeInitPart(dds3MagatuhiBill_tag *valueWork, s32 index);
effMagaSmokeConv_tag *effMagaSmokeConvCreate(effMagaSmokeConvParam_t *src);
void effMagaSmokeConvDelete(effMagaSmokeConv_tag *pData);
void effMagaSmokeConvPartInit(effMagaSmokeConv_tag *pMagaSmokeConv, s32 index);
void effMagaSmokeConvDraw(effMagaSmokeConv_tag *pData);
effMagaPoint4_tag *effMagaPoint4Create(effMagaPoint4Param_t *src);
void effMagaPoint4Delete(effMagaPoint4_tag *pData);
void effMagaPoint4PartInit(effMagaPoint4_tag *pMagaPoint4, s32 index);
void effMagaPoint4Draw(effMagaPoint4_tag *pData);
effMagaCylinder_tag *effMagaCylinderCreate(effMagaCylinderParam_t *src);
void effMagaCylinderDelete(effMagaCylinder_tag *pData);
void effMagaCylinderPartInit(effMagaCylinder_tag *pMagaCylinder, s32 index);

/* Sample the current fixed-height ring before advancing angle and radius.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaCylinderDraw(effMagaCylinder_tag *pData);
void effMagaCylinderSetRGBA(effMagaCylinder_tag *pData, u32 abgr);
void effMagaCylinderRandomize(effMagatuhiCallback_t *hEffect);
effMagaSpiral_tag *effMagaSpiralCreate(effMagaSpiralParam_t *src);
void effMagaSpiralDelete(effMagaSpiral_tag *pData);
void effMagaSpiralPartInit(effMagaSpiral_tag *pMagaSpiral, s32 index);

/* Sample and transform the current orbit before advancing its three rates.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSpiralDraw(effMagaSpiral_tag *pData);
void effMagaSpiralSetRGBA(effMagaSpiral_tag *pData, u32 abgr);
void effMagaSpiralRandomize(effMagatuhiCallback_t *hEffect);
effMagaSmoke_tag *effMagaSmokeCreate(effMagaSmokeParam_t *src);
void effMagaSmokeDelete(effMagaSmoke_tag *pData);
void effMagaSmokePartInit(effMagaSmoke_tag *pMagaSmoke, s32 index);

/* Accumulate lift, transform the drift sample, then advance scale and angle.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSmokeDraw(effMagaSmoke_tag *pData);
void effMagaSmokeSetRGBA(effMagaSmoke_tag *pData, u32 abgr);
void effMagaSmokeRandomize(effMagatuhiCallback_t *hEffect);
dds3MagatuhiPar_tag *dds3MagatuhiParCreate(s32 type, void *params);
void dds3MagatuhiParDraw(dds3MagatuhiPar_tag *hData);
void dds3MagatuhiParDelete(dds3MagatuhiPar_tag *hData);

/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(dds3MagatuhiPar_tag *hMagaPar);

/* Read the dispatch owner's type word; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(dds3MagatuhiPar_tag *hMagaPar);
u32 dds3MagatuhiParIsActive(void);

/* Node set-matrix slot: invoke optional subtype callback B; preserve the one-formal surface. */
void dds3MagatuhiParSetRotateMatrix(dds3MagatuhiPar_tag *work, void *pMatrix);
void dds3MagatuhiParSetByType(dds3MagatuhiPar_tag *work, s32 slotValue);
dds3MagatuhiPar_tag *dds3MagatuhiParCopy(dds3MagatuhiPar_tag *work);
extern u32 effDebugStopTimer, effDebugRunTimer;
extern char effDebugTitleFormat[], effDebugLabelFormat[], effDebugTrueText[], effDebugFalseText[], effDebugHeaderFormat[];
extern u32 effDebugStopTimer;
extern char effDebugTitleFormat[8];
extern char effDebugLabelFormat[8];
extern char effDebugTrueText[8];
extern char effDebugFalseText[8];

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source) {
    effMagatuhiOwner_t *effect;

    effect = sdfChipAllocDirect(sizeof(*effect));
    memcpy(&effect->params, source, sizeof(effect->params));
    effect->valueWork = effMagaCylinderAllocTex(effect->params.count, effect->params.historyCount, effect->params.historyDistanceScale, effect->params.drawBucket, effect->params.historyAngleStep);
    effMagaCylinderBlendColor(effect->valueWork, effect->params.colorA, effect->params.colorB);
    return effect;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effBTLSys_Nego_Stone_Delete);

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSmokeConvDrawSub);

void effMagaCylinderSetPartPtrs(dds3MagatuhiBill_tag *work, s32 slotIndex) {
    f32 *angleRow;

    work->writeIndices[slotIndex] = 0;
    work->validCounts[slotIndex] = 0;
    work->slotColors[slotIndex] = EFF_MAGATUHI_NEUTRAL_COLOR;
    angleRow = work->angleRows[slotIndex];
    angleRow[0] = 6.2831853f;
    angleRow[1] = 0.0f;
    angleRow[2] = 3.1415926f;
    angleRow[3] = 0.0f;
}

dds3MagatuhiBill_tag *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep) {
    s32 countFrames;
    s32 frameTerm;
    s32 countTerm;
    s32 allocationSize;
    struct sdfMemHandle_t *allocation;
    s32 positions;
    s32 colorTable;
    s32 historyScalars;
    s32 values;
    s32 writeIndices;
    s32 validCounts;
    s32 angleValues;
    dds3MagatuhiBill_tag *resource;
    s16 *defaults;
    u32 i;

    countFrames = count * historyCount;
    frameTerm = countFrames + historyCount;
    countTerm = (countFrames << 2) + count;
    allocationSize = (((count << 3) + ((countTerm + frameTerm) << 1) + (count << 1)) << 1) + sizeof(dds3MagatuhiBill_tag);
    allocation = sdfMemAllocDirect(allocationSize);
    positions = EE_ADDR_S(sdfMemLock(allocation));
    colorTable = positions + (countFrames << 4);
    historyScalars = colorTable + (historyCount << 2);
    values = historyScalars + (countFrames << 2);
    writeIndices = values + (count << 2);
    validCounts = writeIndices + (count << 1);
    angleValues = validCounts + (count << 1);
    resource = EE_ADDR_AS(dds3MagatuhiBill_tag *, (angleValues + (count << 4)));

    resource->count = count;
    resource->historyCount = historyCount;
    resource->historyAngleStep = historyAngleStep;
    resource->historyDistanceScale = historyDistanceScale;
    resource->drawBucket = param;
    resource->positions = EE_ADDR_AS(effMagatuhiRow_t *, positions);
    resource->colorTable = EE_ADDR_AS(u32 *, colorTable);
    resource->historyScalars = EE_ADDR_AS(f32 *, historyScalars);
    resource->slotColors = EE_ADDR_AS(u32 *, values);
    resource->writeIndices = EE_ADDR_AS(u16 *, writeIndices);
    resource->validCounts = EE_ADDR_AS(u16 *, validCounts);
    resource->angleRows = EE_ADDR_AS(effMagatuhiRow_t *, angleValues);
    resource->allocationHandle = allocation;
    resource->texture = dds3BillboardGetGeneralTex(0);

    defaults = &effMagaBillboardTexCoords[0][0];
    i = 0;
    do {
        i++;
        defaults[0] = 0;
        defaults[1] = 0;
        defaults[2] = 0x400;
        defaults[3] = 0;
        defaults[4] = 0x400;
        defaults[5] = 0x400;
        defaults[6] = 0;
        defaults[7] = 0x400;
        defaults += 8;
    } while (i < 0xF);

    if (count != 0) {
        i = 0;
        do {
            effMagaCylinderSetPartPtrs(resource, i);
            i++;
        } while (i < count);
    }
    return resource;
}

void effMagaCylinderFreeParts(dds3MagatuhiBill_tag *work) {
    sdfMemFreeDirect(work->allocationHandle);
}

void effSwordTrackDrawTex(dds3MagatuhiBill_tag *work) {
    sdfVector4_t positions[15];
    u32 colors[15];
    f32 offsets[15][8];
    f32 matrix[16];
    sdfVector4_t point;
    sdfDrawTag_t *list;
    u32 packet;
    u32 *vertexPacket;
    f32 (*historyPositions)[4];
    f32 *widthFactors;
    u32 *palette;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    u32 historyOffset;
    u32 particleIndex;
    u32 particleCount;
    u32 slotColor;
    u32 validCount;
    u32 historyCount;
    s32 pendingCount;
    f32 baseSize;
    u16 index, historyIndex;
    f32 size, widthScale;
    u32 packed;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(0x20));
    sdfDrawTagInit(list);
    packet = sdfGrAllocPacketBuff(0x20);
    sdf3DMakeDirectRefPacket(EE_ADDR_AS(DmaPacketHeader *, packet),
        EE_ADDR(sdfTexGetPacket(work->texture)),
        sdfTexGetPacketLength((sdfTexHandle_t *)work->texture));
    sdfDrawTagLinkRef(list, packet);
    SDF_VU0_STOREIDENTITY(matrix);
    historyOffset = 0;
    packet = sdfGrAllocPacketBuff(0x38);
    sdfBillBoardMakeTMPacket(EE_ADDR_AS(u8 *, packet), matrix);
    sdfDrawTagLink(list, packet);
    pendingCount = 0;
    particleCount = work->count;
    historyCount = work->historyCount;
    baseSize = work->historyDistanceScale;
    palette = work->colorTable;
    slotColors = work->slotColors;
    writeIndices = work->writeIndices;
    validCounts = work->validCounts;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++) {
        slotColor = *slotColors++;
        historyPositions = work->positions + historyOffset;
        widthFactors = work->historyScalars + historyOffset;
        validCount = *validCounts++;
        index = *writeIndices++;
        historyOffset += historyCount;
        if (slotColor & 0xFF000000) {
            SDF_VU0_COPYVECTOR(&point, historyPositions[index]);
            historyIndex = 0;
            if (validCount != 0) {
                widthScale = baseSize * 4.0f;
                do {
                    f32 factor = widthFactors[index];
                    widthFactors[index] = factor * 0.75f + 0.25f;
                    size = widthScale * factor;
                    if (index == 0) {
                        index = historyCount - 1;
                    } else {
                        index--;
                    }
                    SDF_VU0_LOADVECTOR(&point);
                    SDF_VU0_STOREVECTOR(&positions[pendingCount]);
                    SDF_VU0_LOADVECTOR_B(historyPositions[index]);
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&point);
                    UnpackRGBAWordFloat(slotColor, 0.0078125f);
                    SDF_VU0_COPYVECTOR_A_TO_B();
                    UnpackRGBAWordFloat(palette[historyIndex], 0.0078125f);
                    SDF_VU0_MULVECTOR();
                    packed = PackMagatuhiTrailColor();
                    colors[pendingCount] = packed;
                    offsets[pendingCount][0] = -size;
                    offsets[pendingCount][1] = -size;
                    offsets[pendingCount][2] = size;
                    offsets[pendingCount][3] = -size;
                    offsets[pendingCount][4] = size;
                    offsets[pendingCount][5] = size;
                    offsets[pendingCount][6] = -size;
                    offsets[pendingCount][7] = size;
                    pendingCount++;
                    if (pendingCount == 15) {
                        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
                            colors, effMagaBillboardTexCoords, offsets, 15, 0);
                        sdfDrawTagLink(list, EE_ADDR(vertexPacket));
                        pendingCount = 0;
                    }
                    historyIndex++;
                } while (historyIndex < validCount);
            }
        }
    }
    if (pendingCount != 0) {
        vertexPacket = sdfBillBoardMakeN((u128 *)positions,
            colors, effMagaBillboardTexCoords, offsets, pendingCount, 0);
        sdfDrawTagLink(list, EE_ADDR(vertexPacket));
    }
    ((void (*)(sdfGrOt_t *, sdfDrawTag_t *))effMagaGrOtBuckets[work->drawBucket]->regFunc)(effMagaGrOtBuckets[work->drawBucket], list);
}

void effMagaCylinderSetPartPos(dds3MagatuhiBill_tag *work, s32 index, const sdfVector4_t *vector) {
    f32 (*newSlot)[4];
    f32 (*replayPosition)[4];
    f32 (*historyPosition)[4];
    f32 *angleRow;
    f32 *scalarOutput;
    sdfVector4_t viewDirection;
    f32 baseLimit = work->historyDistanceScale;
    f32 limit;
    f32 phaseScalar;
    f32 radial;
    u32 historyCount = work->historyCount;
    u32 writeIndex = work->writeIndices[index];
    u32 validCount = work->validCounts[index];
    u32 rowBase = index * historyCount;
    u32 linearIndex;
    u32 historyIndex;

    historyPosition = &work->positions[rowBase + writeIndex];
    writeIndex++;
    if (writeIndex >= historyCount) {
        writeIndex = 0;
    }

    linearIndex = rowBase + writeIndex;
    newSlot = work->positions + linearIndex;
    scalarOutput = work->historyScalars + linearIndex;
    SDF_VU0_COPYVECTOR(newSlot, vector);
    angleRow = work->angleRows[index];

    angleRow[2] += 0.21816613f;
    if (3.14159265f <= angleRow[2]) {
        angleRow[2] -= 3.14159265f;
        angleRow[3] = dds3GetRandFloatA(dds3EffectBase) * 1.5f;
    }

    phaseScalar = sdfSin(angleRow[2]);
    *scalarOutput = phaseScalar * angleRow[3] + 1.0f;

    if (validCount >= EFF_MAGATUHI_HISTORY_MINIMUM) {
        historyIndex = writeIndex < (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT)
                           ? historyCount + writeIndex - (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT)
                           : writeIndex - (EFF_MAGATUHI_REPLAY_GROUP_STRIDE + EFF_MAGATUHI_CONTROL_POINT_COUNT);
        replayPosition = &work->positions[rowBase + historyIndex];
        angleRow = work->angleRows[index];
        angleRow[0] += work->historyAngleStep;
        if (6.2831853f <= angleRow[0]) {
            angleRow[0] -= 6.2831853f;
            if (angleRow[1] < 1.0f) {
                angleRow[1] += (dds3GetRandFloatA(dds3EffectBase) * 0.4f + 0.60000003f) * 0.25f;
            } else if (0.5f < angleRow[1]) {
                angleRow[1] -= (dds3GetRandFloatA(dds3EffectBase) * 0.4f + 0.60000003f) * 0.25f;
            }
        }

        SDF_VU0_LOADVECTOR(aViewEye);
        SDF_VU0_LOADVECTOR_B(aViewTarget);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(vector);
        SDF_VU0_LOADVECTOR_B(historyPosition);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR(&viewDirection);
        radial = sdfSin(angleRow[0]) * angleRow[1];
        (*replayPosition)[0] += viewDirection.x * radial;
        (*replayPosition)[1] += viewDirection.y * radial;
        (*replayPosition)[2] += viewDirection.z * radial;
    }

    SDF_VU0_LOADVECTOR_B(vector);
    SDF_VU0_LOADVECTOR(historyPosition);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(radial);
    limit = baseLimit * 3.0f;
    if (limit < radial) {
        VEC3_SPLAT(&viewDirection.x, limit);
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(&viewDirection);
        SDF_VU0_MULVECTOR();
    }
    SDF_VU0_STOREVECTOR(historyPosition);

    work->writeIndices[index] = writeIndex;
    validCount++;
    if (validCount > historyCount) {
        validCount = historyCount;
    }
    work->validCounts[index] = validCount;
}

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(dds3MagatuhiBill_tag *work, s32 index, u32 color) {
    work->slotColors[index] = color;
}

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(dds3MagatuhiBill_tag *work, u32 colorA, u32 colorB) {
    f32 blendFraction = 0.0f;
    u32 historyCount = work->historyCount;
    u32 *paletteEntry = work->colorTable;
    f32 blendStep = 1.0f / historyCount;
    u32 paletteEntryIndex;

    for (paletteEntryIndex = 0; paletteEntryIndex < historyCount; paletteEntryIndex++) {
        *paletteEntry++ = effColorBlend(colorA, colorB, blendFraction);
        blendFraction += blendStep;
    }
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSmokeConvPartInitSub);

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagaSmokeConv_tag *effMagaSmokeConvCreate(effMagaSmokeConvParam_t *src) {
    u32 count = src->reserve;
    u32 size = count * sizeof(effMagaSmokePart_tag);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagaSmokeConv_tag));
    effMagaSmokePart_tag *particle = EE_ADDR_AS(effMagaSmokePart_tag *, sdfMemLock(allocation));
    effMagaSmokeConv_tag *work = (effMagaSmokeConv_tag *)((u8 *)particle + size);
    s32 spread;
    u32 particleIndex;

    work->head = *src;
    work->hMem = allocation;
    work->pPart = particle;
    if (work->head.timing <= 0) {
        work->head.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->head.reserve);
    work->pBezier4P = dds3Bezier4PCreate(count);
    spread = work->head.timing;
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagaSmokeConv_tag *pData) {
    dds3Bezier4PDelete(pData->pBezier4P);
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}

/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w remains unwritten, as in the retail routine. */
void effMagaSmokeConvPartInit(effMagaSmokeConv_tag *pMagaSmokeConv, s32 index) {
    effMagaSmokePart_tag *particle = &pMagaSmokeConv->pPart[index];
    sdfVector4_t direction;
    f32 radius;
    f32 random;
    effCubicBezierSlot_t *slot;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = pMagaSmokeConv->head.initialRadius * (random + random);
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction.x = random + random;
    direction.y = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction.z = random + random;
    SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)&direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_MEMOPS(*(u128 *)&direction);
    particle->pos[0] = radius * direction.x;
    particle->pos[1] = 0.0f;
    particle->pos[2] = radius * direction.z;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->dirUp[0] = random + random;
    particle->dirUp[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->dirUp[2] = random + random;
    SDF_VU0_LOADVECTOR(particle->dirUp);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->dirUp);
    particle->amplitude = pMagaSmokeConv->head.baseScale *
        (dds3GetRandFloatA(dds3EffectBase) * pMagaSmokeConv->head.scaleVariation +
         (1.0f - pMagaSmokeConv->head.scaleVariation));
    particle->angle = dds3GetRandFloatA(dds3EffectBase) * EFF_MAGATUHI_FULL_TURN;
    particle->up_s = pMagaSmokeConv->head.baseLiftStep *
        (dds3GetRandFloatA(dds3EffectBase) * pMagaSmokeConv->head.liftVariation +
         (1.0f - pMagaSmokeConv->head.liftVariation));
    slot = dds3Bezier4PGetPart(pMagaSmokeConv->pBezier4P, index);
    slot->t = 0.0f;
    slot->step = 0.0f;
    effMagaSmokeInitPart(pMagaSmokeConv->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaSmokeConv->pMagatuhi->valueWork, index, 0);
}

/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void effMagaSmokeConvDraw(effMagaSmokeConv_tag *pData) {
    sdfVector4_t point;
    sdfVector4_t origin;
    sdfVector4_t basePosition;
    sdfVector4_t target;
    sdfVector4_t viewDirection;
    sdfVector4_t sideAxis;
    sdfVector4_t towardTarget;
    sdfVector4_t startTangent;
    effArrHdr_t *mathResource;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSmokePart_tag *particle;
    effCubicBezierSlot_t *slot;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    f32 angleStep, scaleStep;
    f32 captureDistance, pathJitter, pathStep, pathAcceleration;
    f32 radial, distance, segmentDistance, offset, random, fade;
    s32 age;
    u32 i;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&viewDirection);
    mathResource = pData->pBezier4P;
    valueWork = pData->pMagatuhi->valueWork;
    particle = pData->pPart;
    count = pData->head.reserve;
    respawn = pData->head.respawn;
    life = SDF_FRAMES(pData->head.life);
    spread = pData->head.timing;
    angleStep = SDF_FRAME_STEP(pData->head.s_angle_s);
    scaleStep = SDF_FRAME_STEP(pData->head.s_ampli_s);
    fadeIn = SDF_FRAMES(pData->head.fade_in);
    fadeOut = SDF_FRAMES(pData->head.fade_out);
    slot = dds3Bezier4PGetPart(mathResource, 0);
    SDF_VU0_COPYVECTOR(&origin, &pData->head.originX);
    captureDistance = pData->head.captureDistance;
    pathJitter = pData->head.c_ampli;
    pathStep = SDF_FRAME_STEP(pData->head.c_step);
    pathAcceleration = SDF_FRAME_DECAY(pData->head.c_step_a);
    SDF_VU0_COPYVECTOR(&target, &pData->head.targetX);
    for (i = 0; i < count; i++, particle++, slot++) {
        age = particle->age;
        if (age == 0) {
            effMagaSmokeConvPartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            if (slot->step == 0.0f) {
                SDF_VU0_COPYVECTOR(&basePosition, particle->pos);
                radial = particle->amplitude * sdfSin(particle->angle);
                particle->dirUp[1] += SDF_FRAME_STEP(particle->up_s);
                point.x = origin.x + particle->pos[0] + particle->dirUp[0] * radial;
                point.y = origin.y + particle->pos[1] + particle->dirUp[1];
                point.z = origin.z + particle->pos[2] + particle->dirUp[2] * radial;
                particle->amplitude += scaleStep;
                particle->angle += angleStep;
                SDF_VU0_LOADVECTOR(&target);
                SDF_VU0_LOADVECTOR_B(&point);
                SDF_VU0_SUBVECTOR();
                SDF_VU0_GETLENGTH(distance);
                if (distance <= captureDistance) {
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&towardTarget);
                    SDF_VU0_LOADVECTOR_B(&viewDirection);
                    SDF_VU0_OUTERPRODUCT();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&sideAxis);
                    SDF_VU0_LOADVECTOR(&point);
                    SDF_VU0_LOADVECTOR_B(&basePosition);
                    SDF_VU0_SUBVECTOR();
                    SDF_VU0_NORMALIZE();
                    SDF_VU0_STOREVECTOR(&startTangent);
                    slot->controlPoints[0][0] = point.x;
                    slot->controlPoints[0][1] = point.y;
                    slot->controlPoints[0][2] = point.z;
                    segmentDistance = captureDistance * 0.3999999762f;
                    offset = pathJitter * (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f);
                    slot->controlPoints[1][0] = point.x + towardTarget.x * segmentDistance + startTangent.x * offset;
                    slot->controlPoints[1][1] = point.y + towardTarget.y * segmentDistance + startTangent.y * offset;
                    slot->controlPoints[1][2] = point.z + towardTarget.z * segmentDistance + startTangent.z * offset;
                    segmentDistance = captureDistance * 0.6499999762f;
                    random = dds3GetRandFloatA(dds3EffectBase) - 0.5f;
                    offset = pathJitter * (random + random);
                    slot->controlPoints[2][0] = point.x + towardTarget.x * segmentDistance + sideAxis.x * offset;
                    slot->controlPoints[2][1] = point.y + towardTarget.y * segmentDistance + sideAxis.y * offset;
                    slot->controlPoints[2][2] = point.z + towardTarget.z * segmentDistance + sideAxis.z * offset;
                    slot->controlPoints[3][0] = target.x;
                    slot->controlPoints[3][1] = target.y;
                    slot->controlPoints[3][2] = target.z;
                    slot->step = pathStep;
                }
            } else {
                dds3Bezier4PPartRun(mathResource, i, &point.x);
                slot->step *= pathAcceleration;
            }
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i, ((u32)(fade * 127.0f) << 24) | 0x808080);
            if (SDF_FRAME_DUE(age)) effMagaCylinderSetPartPos(valueWork, i, &point);
        }
        if (age >= life && respawn) {
            particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->age++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}

/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *hEffect, effMagaSCArea_t *pMagaSCArea) {
    effMagaSmokeConv_tag *dst = dds3MagatuhiGetHandle(hEffect->effect);

    dst->head.originX = pMagaSCArea->position[0].x;
    dst->head.originY = pMagaSCArea->position[0].y;
    dst->head.originZ = pMagaSCArea->position[0].z;
    dst->head.initialRadius = pMagaSCArea->radius[0];
    dst->head.targetX = pMagaSCArea->position[1].x;
    dst->head.targetY = pMagaSCArea->position[1].y;
    dst->head.targetZ = pMagaSCArea->position[1].z;
    dst->head.captureDistance = pMagaSCArea->radius[1];
}

/* Clone history parameters; signed delays follow the returned work block. */
effMagaPoint4_tag *effMagaPoint4Create(effMagaPoint4Param_t *src) {
    u32 count = src->reserve;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * EFF_MAGATUHI_DELAY_WORD_BYTES + sizeof(effMagaPoint4_tag));
    effMagaPoint4_tag *work = EE_ADDR_AS(effMagaPoint4_tag *, sdfMemLock(allocation));
    s32 *delays = (s32 *)(work + 1);
    s32 spread;
    u32 i;

    work->param = *src;
    work->hMem = allocation;
    work->pPart = delays;
    if (work->param.timing <= 0) {
        work->param.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.reserve);
    work->pBezier4P = dds3Bezier4PCreate(count);
    spread = work->param.timing;
    for (i = 0; i < count; i++) {
        *delays++ = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
    }
    return work;
}

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagaPoint4_tag *pData) {
    dds3Bezier4PDelete(pData->pBezier4P);
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagaPoint4_tag *pMagaPoint4, s32 index) {
    f32 scale[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 lastNormal[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 viewDirection[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 point[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    effCubicBezierSlot_t *slot;
    f32 step;
    f32 random;

    SDF_VU0_LOADVECTOR(aViewEye);
    SDF_VU0_LOADVECTOR_B(aViewTarget);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(viewDirection);
    step = 1.0f / (f32)SDF_FRAMES(pMagaPoint4->param.life);
    slot = dds3Bezier4PGetPart(pMagaPoint4->pBezier4P, index);
    slot->t = 0;
    slot->step = step;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[0] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[0]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[1]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[0][0] = point[0];
    slot->controlPoints[0][1] = point[1];
    slot->controlPoints[0][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[1] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[1]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[2]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[1][0] = point[0];
    slot->controlPoints[1][1] = point[1];
    slot->controlPoints[1][2] = point[2];

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[2] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(pMagaPoint4->param.point[2]);
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[3]);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_LOADVECTOR_B(viewDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[2][0] = point[0];
    slot->controlPoints[2][1] = point[1];
    slot->controlPoints[2][2] = point[2];

    /* The fourth point reuses the third segment's normalized side direction. */
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    scale[0] = pMagaPoint4->param.jitterScales[3] * (random + random);
    scale[1] = scale[0];
    scale[2] = scale[0];
    SDF_VU0_LOADVECTOR(lastNormal);
    SDF_VU0_LOADVECTOR_B(scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(pMagaPoint4->param.point[3]);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(point);
    slot->controlPoints[3][0] = point[0];
    slot->controlPoints[3][1] = point[1];
    slot->controlPoints[3][2] = point[2];
    effMagaSmokeInitPart(pMagaPoint4->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaPoint4->pMagatuhi->valueWork, index, 0);
}

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagaPoint4_tag *pData) {
    effArrHdr_t *slots = pData->pBezier4P;
    dds3MagatuhiBill_tag *valueWork = pData->pMagatuhi->valueWork;
    s32 *delays = pData->pPart;
    u8 respawn = pData->param.loop;
    u32 count = pData->param.reserve;
    s32 life = SDF_FRAMES(pData->param.life);
    s32 spread = pData->param.timing;
    s32 fadeIn = SDF_FRAMES(pData->param.fade_in);
    s32 fadeOut = SDF_FRAMES(pData->param.fade_out);
    sdfVector4_t out;
    f32 fade;
    s32 frame;
    u32 particleIndex;

    /* Retain the native slot lookup even though its returned address is unused. */
    dds3Bezier4PGetPart(slots, 0);
    for (particleIndex = 0; particleIndex < count; particleIndex++) {
        frame = *delays;
        if (frame == 0) {
            effMagaPoint4PartInit(pData, particleIndex);
        }
        if (frame > 0 && frame <= life) {
            dds3Bezier4PPartRun(slots, particleIndex, &out.x);
            if (fadeIn > frame) {
                fade = (f32)frame / (f32)fadeIn;
            } else if (life - frame <= fadeOut) {
                fade = (f32)(life - frame) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, particleIndex, ((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB);
            if (SDF_FRAME_DUE(frame)) effMagaCylinderSetPartPos(valueWork, particleIndex, &out);
        }
        if (frame >= life && respawn) {
            *delays = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            (*delays)++;
        }
        delays++;
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *hEffect, effMagaP4Point_t *pP4Point) {
    effMagaPoint4_tag *dst = dds3MagatuhiGetHandle(hEffect->effect);
    u32 controlPointIndex;

    for (controlPointIndex = 0; controlPointIndex < EFF_MAGATUHI_CONTROL_POINT_COUNT; controlPointIndex++) {
        dst->param.point[controlPointIndex][0] = pP4Point->controlPoints[controlPointIndex][0];
        dst->param.point[controlPointIndex][1] = pP4Point->controlPoints[controlPointIndex][1];
        dst->param.point[controlPointIndex][2] = pP4Point->controlPoints[controlPointIndex][2];
        dst->param.jitterScales[controlPointIndex] = pP4Point->jitterScales[controlPointIndex];
    }
}

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *hEffect) {
    effMagaPoint4_tag *work = dds3MagatuhiGetHandle(hEffect->effect);
    dds3MagatuhiBill_tag *valueWork = work->pMagatuhi->valueWork;
    u32 historyFrames = work->param.duration;
    s32 lifetimeFrames = work->param.life;
    u32 count = work->param.reserve;
    s32 *ages = work->pPart;
    sdfVector4_t samplePosition;
    f32 startPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 endPosition[EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 blendFraction;
    f32 blendStep;
    u32 particleIndex;
    u32 sampleIndex;
    s32 sampleSpan;
    effCubicBezierSlot_t *slot;

    for (particleIndex = 0; particleIndex < count; particleIndex += EFF_MAGATUHI_BEZIER_GROUP_STRIDE) {
        effMagaPoint4PartInit(work, particleIndex);
        *ages = dds3GetRandA((u8 *)dds3EffectBase) % lifetimeFrames;
        slot = dds3Bezier4PGetPart(work->pBezier4P, particleIndex);
        if (historyFrames < *ages) {
            slot->t = slot->step * SDF_FRAMES_F((f32)(*ages - historyFrames));
            sampleSpan = historyFrames;
        } else {
            slot->t = 0.0f;
            sampleSpan = historyFrames - *ages;
        }
        blendFraction = 0.0f;
        dds3Bezier4PPartRun(work->pBezier4P, particleIndex, startPosition);
        slot->t = slot->step * SDF_FRAMES_F((f32)*ages);
        dds3Bezier4PPartRun(work->pBezier4P, particleIndex, endPosition);
        blendStep = 1.0f / (f32)sampleSpan;
        for (sampleIndex = 0; sampleIndex < sampleSpan; sampleIndex++) {
            SDF_VU0_LOADVECTOR(startPosition);
            SDF_VU0_LOADVECTOR_B(endPosition);
            SDF_VU0_INTERPOLATE(blendFraction);
            SDF_VU0_STOREVECTOR(&samplePosition);
            effMagaCylinderSetPartPos(valueWork, particleIndex, &samplePosition);
            blendFraction += blendStep;
        }
        *ages = SDF_FRAMES(*ages) + 1;
        ages += EFF_MAGATUHI_BEZIER_GROUP_STRIDE;
    }
}

/* Clone ring parameters; the returned work precedes its individual slots. */
effMagaCylinder_tag *effMagaCylinderCreate(effMagaCylinderParam_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effMagaCylinderPart_tag) + sizeof(effMagaCylinder_tag));
    effMagaCylinder_tag *work = EE_ADDR_AS(effMagaCylinder_tag *, sdfMemLock(allocation));
    effMagaCylinderPart_tag *particle = (effMagaCylinderPart_tag *)(work + 1);
    s32 spread;
    u32 i;

    work->param = *src;
    work->pPart = particle;
    work->abgr = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->hMem = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->param.timing <= 0) {
        work->param.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.particleCount);
    spread = work->param.timing;
    for (i = 0; i < count; i++) {
        particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagaCylinder_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagaCylinderPartInit(effMagaCylinder_tag *pMagaCylinder, s32 index) {
    effMagaCylinderPart_tag *elem = &pMagaCylinder->pPart[index];
    f32 blend;
    f32 t;

    elem->frame = 0;
    elem->height = -pMagaCylinder->param.height * dds3GetRandFloatA(dds3EffectBase);
    elem->rotate = dds3GetRandFloatA(dds3EffectBase) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = pMagaCylinder->param.rotate_r;
    elem->rotate_s = pMagaCylinder->param.rotate_s * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = pMagaCylinder->param.radius_s_r;
    elem->radius = pMagaCylinder->param.radius_s * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = pMagaCylinder->param.endJitter;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    elem->radius_s = (pMagaCylinder->param.radius_e * t - elem->radius) / (f32)pMagaCylinder->param.life;
    effMagaSmokeInitPart(pMagaCylinder->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaCylinder->pMagatuhi->valueWork, index, 0);
}

void effMagaCylinderDraw(effMagaCylinder_tag *pData) {
    sdfVector4_t out;
    sdfVector4_t origin;
    dds3MagatuhiBill_tag *valueWork;
    effMagaCylinderPart_tag *particle;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    u32 tintColor;
    f32 angle, radius, fade;
    s32 age;
    u32 i;

    particle = pData->pPart;
    count = pData->param.particleCount;
    respawn = pData->param.loop;
    life = SDF_FRAMES(pData->param.life);
    spread = pData->param.timing;
    fadeIn = SDF_FRAMES(pData->param.fade_in);
    fadeOut = SDF_FRAMES(pData->param.fade_out);
    tintColor = pData->abgr;
    valueWork = pData->pMagatuhi->valueWork;
    SDF_VU0_COPYVECTOR_MEMOPS(&origin, pData->param.origin);
    SDF_VU0_LOADMATRIX(pData->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->frame;
        if (age == 0) {
            effMagaCylinderPartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            angle = particle->rotate;
            radius = particle->radius;
            out.x = sdfCos(angle) * radius;
            out.y = particle->height;
            out.z = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_LOADVECTOR(&out);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&out);
            particle->rotate += SDF_FRAME_STEP(particle->rotate_s);
            particle->radius += SDF_FRAME_STEP(particle->radius_s);
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i,
                                   effMagaCylinderCalcColor(((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            if (SDF_FRAME_DUE(age)) effMagaCylinderSetPartPos(valueWork, i, &out);
        }
        if (age >= life && respawn) {
            particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->frame++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaCylinderSetPosition);

void effMagaCylinderSetRGBA(effMagaCylinder_tag *pData, u32 abgr) {
    pData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaCylinderSetRotateMatrix);

/* Replay the first slot of each three-slot group from a random age. */
void effMagaCylinderRandomize(effMagatuhiCallback_t *hEffect) {
    u32 k;
    effMagaCylinder_tag *work;
    dds3MagatuhiBill_tag *valueWork;
    effMagaCylinderPart_tag *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    sdfVector4_t out;
    sdfVector4_t origin;
    f32 height;
    f32 angle;
    f32 radius;
    f32 angleStep;
    f32 radiusStep;

    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(hEffect->effect) == EFF_MAGATUHI_REPLAY_RING) {
            work = dds3MagatuhiGetHandle(hEffect->effect);
            count = work->param.particleCount;
            valueWork = work->pMagatuhi->valueWork;
            maxSteps = work->param.reserve;
            frames = work->param.life;
            particle = work->pPart;
            SDF_VU0_COPYVECTOR(&origin, work->param.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaCylinderPartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->frame = SDF_FRAMES(frameOffset);
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->rotate_s;
                radiusStep = particle->radius_s;
                height = particle->height;
                angle = particle->rotate + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                out.w = 0;
                for (k = 0; k < steps; k++) {
                    out.x = sdfCos(angle) * radius;
                    out.y = height;
                    out.z = sdfSin(angle) * radius;
                    SDF_VU0_LOADVECTOR_B(&origin);
                    SDF_VU0_LOADVECTOR(&out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&out);
                    effMagaCylinderSetPartPos(valueWork, i, &out);
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->radius = radius;
                particle->rotate = angle;
            }
        }
    }
}

/* Clone orbit parameters; the returned work precedes its individual slots. */
effMagaSpiral_tag *effMagaSpiralCreate(effMagaSpiralParam_t *src) {
    u32 count = src->particleCount;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(count * sizeof(effMagaSpiralPart_tag) + sizeof(effMagaSpiral_tag));
    effMagaSpiral_tag *work = EE_ADDR_AS(effMagaSpiral_tag *, sdfMemLock(allocation));
    effMagaSpiralPart_tag *particle = (effMagaSpiralPart_tag *)(work + 1);
    s32 spread;
    u32 i;

    work->param = *src;
    work->pPart = particle;
    work->abgr = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->hMem = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->param.timing <= 0) {
        work->param.timing = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.particleCount);
    spread = work->param.timing;
    for (i = 0; i < count; i++) {
        particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagaSpiral_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}

/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagaSpiralPartInit(effMagaSpiral_tag *pMagaSpiral, s32 index) {
    effMagaSpiralPart_tag *elem = &pMagaSpiral->pPart[index];
    f32 blend;
    f32 t;

    elem->frame = 0;
    elem->up = 0;
    blend = pMagaSpiral->param.up_r;
    elem->up_s = pMagaSpiral->param.up_s * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    elem->rotate = dds3GetRandFloatA(dds3EffectBase) * (EFF_MAGATUHI_HALF_TURN * 2.0f);
    blend = pMagaSpiral->param.rotate_r;
    elem->rotate_s = pMagaSpiral->param.rotate_s * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = pMagaSpiral->param.radius_s_r;
    elem->radius = pMagaSpiral->param.radius_s * (dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend));
    blend = pMagaSpiral->param.endJitter;
    t = dds3GetRandFloatA(dds3EffectBase) * blend + (1.0f - blend);
    elem->radius_s = (pMagaSpiral->param.radius_e * t - elem->radius) / (f32)pMagaSpiral->param.life;
    effMagaSmokeInitPart(pMagaSpiral->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaSpiral->pMagatuhi->valueWork, index, 0);
}

void effMagaSpiralDraw(effMagaSpiral_tag *pData) {
    sdfVector4_t out;
    sdfVector4_t origin;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSpiralPart_tag *particle;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    u32 tintColor;
    f32 angle, radius, fade;
    s32 age;
    u32 i;

    particle = pData->pPart;
    count = pData->param.particleCount;
    respawn = pData->param.loop;
    life = SDF_FRAMES(pData->param.life);
    spread = pData->param.timing;
    fadeIn = SDF_FRAMES(pData->param.fade_in);
    fadeOut = SDF_FRAMES(pData->param.fade_out);
    tintColor = pData->abgr;
    valueWork = pData->pMagatuhi->valueWork;
    SDF_VU0_COPYVECTOR_MEMOPS(&origin, pData->param.origin);
    SDF_VU0_LOADMATRIX(pData->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->frame;
        if (age == 0) {
            effMagaSpiralPartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            angle = particle->rotate;
            radius = particle->radius;
            out.x = sdfCos(angle) * radius;
            out.y = particle->up;
            out.z = sdfSin(angle) * radius;
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_LOADVECTOR(&out);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&out);
            particle->up += SDF_FRAME_STEP(particle->up_s);
            particle->rotate += SDF_FRAME_STEP(particle->rotate_s);
            particle->radius += SDF_FRAME_STEP(particle->radius_s);
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i,
                                   effMagaCylinderCalcColor(((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            if (SDF_FRAME_DUE(age)) effMagaCylinderSetPartPos(valueWork, i, &out);
        }
        if (age >= life && respawn) {
            particle->frame = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->frame++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSpiralSetPosition);

void effMagaSpiralSetRGBA(effMagaSpiral_tag *pData, u32 abgr) {
    pData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSpiralSetRotateMatrix);

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *hEffect) {
    u32 k;
    effMagaSpiral_tag *work;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSpiralPart_tag *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    sdfVector4_t out;
    sdfVector4_t origin;
    f32 height;
    f32 angle;
    f32 radius;
    f32 heightStep;
    f32 angleStep;
    f32 radiusStep;

    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(hEffect->effect) == EFF_MAGATUHI_REPLAY_ORBIT) {
            work = dds3MagatuhiGetHandle(hEffect->effect);
            count = work->param.particleCount;
            valueWork = work->pMagatuhi->valueWork;
            maxSteps = work->param.reserve;
            frames = work->param.life;
            particle = work->pPart;
            SDF_VU0_COPYVECTOR(&origin, work->param.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSpiralPartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->frame = SDF_FRAMES(frameOffset);
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = particle->rotate_s;
                radiusStep = particle->radius_s;
                heightStep = particle->up_s;
                angle = particle->rotate + angleStep * (f32)frameOffset;
                radius = particle->radius + radiusStep * (f32)frameOffset;
                height = particle->up + heightStep * (f32)frameOffset;
                out.w = 0;
                for (k = 0; k < steps; k++) {
                    out.x = sdfCos(angle) * radius;
                    out.y = height;
                    out.z = sdfSin(angle) * radius;
                    SDF_VU0_LOADVECTOR_B(&origin);
                    SDF_VU0_LOADVECTOR(&out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&out);
                    effMagaCylinderSetPartPos(valueWork, i, &out);
                    height += heightStep;
                    angle += angleStep;
                    radius += radiusStep;
                }
                particle->up = height;
                particle->radius = radius;
                particle->rotate = angle;
            }
        }
    }
}

/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagaSmoke_tag *effMagaSmokeCreate(effMagaSmokeParam_t *src) {
    u32 count = src->particleCount;
    u32 size = count * sizeof(effMagaSmokePart_tag);
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size + sizeof(effMagaSmoke_tag));
    effMagaSmokePart_tag *particle = EE_ADDR_AS(effMagaSmokePart_tag *, sdfMemLock(allocation));
    effMagaSmoke_tag *work = (effMagaSmoke_tag *)((u8 *)particle + size);
    s32 spread;
    u32 i;

    work->param = *src;
    work->pPart = particle;
    work->abgr = EFF_MAGATUHI_NEUTRAL_COLOR;
    work->hMem = allocation;
    SDF_VU0_STOREIDENTITY(work->matrix);
    if (work->param.delaySpread <= 0) {
        work->param.delaySpread = 1;
    }
    work->pMagatuhi = effMagaCylinderAllocPart(&work->param.particleCount);
    spread = work->param.delaySpread;
    for (i = 0; i < count; i++) {
        particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        particle++;
    }
    return work;
}

/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagaSmoke_tag *pData) {
    effMagaCylinderFree(pData->pMagatuhi);
    sdfMemFreeDirect(pData->hMem);
}

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagaSmokePartInit(effMagaSmoke_tag *pMagaSmoke, s32 index) {
    effMagaSmokePart_tag *particle = &pMagaSmoke->pPart[index];
    sdfVector4_t direction;
    f32 radius;
    f32 random;

    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    radius = pMagaSmoke->param.radius * (random + random);
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction.x = random + random;
    direction.y = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    direction.z = random + random;
    SDF_VU0_LOADVECTOR_MEMOPS(*(u128 *)&direction);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_MEMOPS(*(u128 *)&direction);
    particle->pos[0] = radius * direction.x;
    particle->pos[1] = 0.0f;
    particle->pos[2] = radius * direction.z;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->dirUp[0] = random + random;
    particle->dirUp[1] = 0.0f;
    random = dds3GetRandFloatA(dds3EffectBase) - EFF_MAGATUHI_RANDOM_MIDPOINT;
    particle->dirUp[2] = random + random;
    SDF_VU0_LOADVECTOR(particle->dirUp);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(particle->dirUp);
    particle->amplitude = pMagaSmoke->param.ampli *
        (dds3GetRandFloatA(dds3EffectBase) * pMagaSmoke->param.ampli_r +
         (1.0f - pMagaSmoke->param.ampli_r));
    particle->angle = dds3GetRandFloatA(dds3EffectBase) * EFF_MAGATUHI_FULL_TURN;
    particle->up_s = pMagaSmoke->param.up_s *
        (dds3GetRandFloatA(dds3EffectBase) * pMagaSmoke->param.up_s_r +
         (1.0f - pMagaSmoke->param.up_s_r));
    effMagaSmokeInitPart(pMagaSmoke->pMagatuhi->valueWork, index);
    effMagaCylinderSetPart(pMagaSmoke->pMagatuhi->valueWork, index, 0);
}

void effMagaSmokeDraw(effMagaSmoke_tag *pData) {
    sdfVector4_t out;
    sdfVector4_t origin;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSmokePart_tag *particle;
    u8 respawn;
    u32 count;
    s32 life, spread, fadeIn, fadeOut;
    u32 tintColor;
    f32 radial, fade;
    f32 angleStep, scaleStep;
    s32 age;
    u32 i;

    particle = pData->pPart;
    tintColor = pData->abgr;
    count = pData->param.particleCount;
    respawn = pData->param.loop;
    life = SDF_FRAMES(pData->param.lifetimeFrames);
    spread = pData->param.delaySpread;
    angleStep = SDF_FRAME_STEP(pData->param.angleStep);
    scaleStep = SDF_FRAME_STEP(pData->param.scaleStep);
    fadeIn = SDF_FRAMES(pData->param.fadeIn);
    fadeOut = SDF_FRAMES(pData->param.fadeOut);
    valueWork = pData->pMagatuhi->valueWork;
    SDF_VU0_COPYVECTOR_MEMOPS(&origin, pData->param.origin);
    SDF_VU0_LOADMATRIX(pData->matrix);
    for (i = 0; i < count; i++, particle++) {
        age = particle->age;
        if (age == 0) {
            effMagaSmokePartInit(pData, i);
        }
        if (age > 0 && age <= life) {
            radial = particle->amplitude * sdfSin(particle->angle);
            particle->dirUp[1] += SDF_FRAME_STEP(particle->up_s);
            out.x = particle->pos[0] + particle->dirUp[0] * radial;
            out.y = particle->pos[1] + particle->dirUp[1];
            out.z = particle->pos[2] + particle->dirUp[2] * radial;
            SDF_VU0_LOADVECTOR_B(&origin);
            SDF_VU0_LOADVECTOR(&out);
            SDF_VU0_APPLYMATRIX();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&out);
            particle->amplitude += scaleStep;
            particle->angle += angleStep;
            if (age < fadeIn) {
                fade = (f32)age / (f32)fadeIn;
            } else if (life - age <= fadeOut) {
                fade = (f32)(life - age) / (f32)fadeOut;
            } else {
                fade = 1.0f;
            }
            effMagaCylinderSetPart(valueWork, i,
                                   effMagaCylinderCalcColor(((u32)(fade * EFF_MAGATUHI_FADE_ALPHA_SCALE) << EFF_MAGATUHI_ALPHA_SHIFT) | EFF_MAGATUHI_NEUTRAL_RGB, tintColor));
            if (SDF_FRAME_DUE(age)) effMagaCylinderSetPartPos(valueWork, i, &out);
        }
        if (age >= life && respawn) {
            particle->age = -SDF_FRAMES(dds3GetRandA((u8 *)dds3EffectBase) % spread);
        } else {
            particle->age++;
        }
    }
    effMagaCylinderDrawTrack(pData->pMagatuhi);
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSmokeSetPosition);

void effMagaSmokeSetRGBA(effMagaSmoke_tag *pData, u32 abgr) {
    pData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effMagaSmokeSetRotateMatrix);

/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *hEffect) {
    u32 k;
    effMagaSmoke_tag *work;
    dds3MagatuhiBill_tag *valueWork;
    effMagaSmokePart_tag *particle;
    u32 count;
    s32 frames;
    u32 maxSteps;
    u32 i;
    u32 steps;
    s32 frameOffset; /* random age, then the initial state offset skipped before replay */
    sdfVector4_t out;
    sdfVector4_t origin;
    f32 lift;
    f32 angle;
    f32 scale;
    f32 liftStep;
    f32 angleStep;
    f32 scaleStep;

    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        if (dds3MagatuhiGetType(hEffect->effect) == EFF_MAGATUHI_REPLAY_DRIFT) {
            work = dds3MagatuhiGetHandle(hEffect->effect);
            count = work->param.particleCount;
            valueWork = work->pMagatuhi->valueWork;
            maxSteps = work->param.reserve;
            frames = work->param.lifetimeFrames;
            particle = work->pPart;
            SDF_VU0_COPYVECTOR(&origin, work->param.origin);
            SDF_VU0_LOADMATRIX(work->matrix);
            for (i = 0; i < count; i += EFF_MAGATUHI_REPLAY_GROUP_STRIDE, particle += EFF_MAGATUHI_REPLAY_GROUP_STRIDE) {
                effMagaSmokePartInit(work, i);
                frameOffset = dds3GetRandA((u8 *)dds3EffectBase) % frames;
                particle->age = SDF_FRAMES(frameOffset);
                /* Keep at most maxSteps samples: skip older state only when
                 * the random age exceeds that window; otherwise start at zero. */
                if (maxSteps < frameOffset) {
                    steps = maxSteps;
                    frameOffset -= steps;
                } else {
                    steps = maxSteps - frameOffset;
                    frameOffset = 0;
                }
                angleStep = work->param.angleStep;
                scaleStep = work->param.scaleStep;
                liftStep = particle->up_s;
                angle = particle->angle + angleStep * (f32)frameOffset;
                scale = particle->amplitude + scaleStep * (f32)frameOffset;
                lift = particle->dirUp[1] + liftStep * (f32)frameOffset;
                out.w = 0;
                for (k = 0; k < steps; k++) {
                    /* Native ordering: lift advances before sampling, scale/angle after. */
                    lift += liftStep;
                    /* Keep the sine evaluation even though its result is unused here. */
                    sdfSin(angle);
                    out.x = particle->pos[0] + particle->dirUp[0] * scale;
                    out.y = particle->pos[1] + lift;
                    out.z = particle->pos[2] + particle->dirUp[2] * scale;
                    SDF_VU0_LOADVECTOR_B(&origin);
                    SDF_VU0_LOADVECTOR(&out);
                    SDF_VU0_APPLYMATRIX();
                    SDF_VU0_ADDVECTOR();
                    SDF_VU0_STOREVECTOR(&out);
                    effMagaCylinderSetPartPos(valueWork, i, &out);
                    scale += scaleStep;
                    angle += angleStep;
                }
                particle->amplitude = scale;
                particle->angle = angle;
                particle->dirUp[1] = lift;
            }
        }
    }
}

/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagaRandomize(effMagatuhiCallback_t *hEffect) {
    if (hEffect->type == EFF_MAGATUHI_CALLBACK_TYPE) {
        switch (dds3MagatuhiGetType(hEffect->effect)) {
        case EFF_MAGATUHI_REPLAY_NONE:
            break;
        case EFF_MAGATUHI_REPLAY_BEZIER:
            effMagaP4Randomize(hEffect);
            break;
        case EFF_MAGATUHI_REPLAY_RING:
            effMagaCylinderRandomize(hEffect);
            break;
        case EFF_MAGATUHI_REPLAY_ORBIT:
            effMagaSpiralRandomize(hEffect);
            break;
        case EFF_MAGATUHI_REPLAY_DRIFT:
            effMagaSmokeRandomize(hEffect);
            break;
        }
    }
}

/* Allocate the dispatch owner before calling the selected payload constructor. */
dds3MagatuhiPar_tag *dds3MagatuhiParCreate(s32 type, void *params) {
    dds3MagatuhiPar_tag *work = sdfChipAllocDirect(sizeof(*work));
    void *payload = dds3MagatuhiCreateTable[type].handler(params);

    work->type = type;
    work->pHandle = payload;
    return work;
}

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(dds3MagatuhiPar_tag *hData) {
    dds3MagatuhiDrawTable[hData->type].handler(hData->pHandle);
}

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(dds3MagatuhiPar_tag *hData) {
    dds3MagatuhiDeleteTable[hData->type].handler(hData->pHandle);
    sdfChipFreeDirect(hData);
}

void *effMagaSCSetAreaSub(dds3MagatuhiPar_tag *work) {
    return work->pHandle;
}

u32 effMagaCylinderRandomizeSub(dds3MagatuhiPar_tag *work) {
    return work->type;
}

void effLeaf43(void) {
}

u32 dds3MagatuhiParIsActive(void) {
    return 1;
}

void dds3MagatuhiParSetRotateMatrix(dds3MagatuhiPar_tag *work, void *pPos) {
    void (*handler)(void *, void *) = dds3MagatuhiSetRGBATable[work->type].handler;

    if (handler != NULL) {
        handler(work->pHandle, pPos);
    }
}

void effLeaf44(dds3MagatuhiPar_tag *work, void *pMatrix) {
    void (*handler)(void *, void *) = dds3MagatuhiSetRGBATable3[work->type].handler;

    if (handler != NULL) {
        handler(work->pHandle, pMatrix);
    }
}

void effLeaf45(dds3MagatuhiPar_tag *work, u32 abgr) {
    void (*handler)(void *, u32) = dds3MagatuhiSetRGBATable2[work->type].handler;

    if (handler != NULL) {
        handler(work->pHandle, abgr);
    }
}

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(dds3MagatuhiPar_tag *work, s32 slotValue) {
    u8 slotByte = slotValue;
    u32 type = work->type;

    switch (type) {
    case 0:
        ((effSub_t *)work->pHandle)->type0SubSlot = slotByte;
        return;
    case 1:
        ((effSub_t *)work->pHandle)->type1SubSlot = slotByte;
        return;
    case 2:
        ((effSub_t *)work->pHandle)->type2To4SubSlot = slotByte;
        return;
    case 3:
        ((effSub_t *)work->pHandle)->type2To4SubSlot = slotByte;
        return;
    case 4:
        ((effSub_t *)work->pHandle)->type2To4SubSlot = slotByte;
        return;
    default:
        return;
    }
}

INCLUDE_ASM(const s32, "effect/effMagatuhi", effGetSubSlot);

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
dds3MagatuhiPar_tag *dds3MagatuhiParCopy(dds3MagatuhiPar_tag *work) {
    u32 type = work->type;
    void *handlerArg = NULL;

    switch (type) {
    case 0:
        handlerArg = work->pHandle;
        break;
    case 1:
        handlerArg = work->pHandle;
        break;
    case 2:
        handlerArg = (u8 *)work->pHandle + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 3:
        handlerArg = (u8 *)work->pHandle + EFF_SUBWORK_PREFIX_BYTES;
        break;
    case 4:
        handlerArg = (u8 *)work->pHandle + EFF_SUBWORK_PREFIX_BYTES;
        break;
    default:
        break;
    }
    return dds3MagatuhiParCreate(type, handlerArg);
}
