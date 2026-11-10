/* Declarations of smt3-decomp src/smt3/effect/effMagatuhi.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFMAGATUHI_H
#define SHARED_EFFECT_EFFMAGATUHI_H

#include "common.h"

#include "sdfDevCons.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "fpu.h"

#include "effect.h"

#include "effDebug.h"

#include "sdfFrame.h"

extern sdfGrOt_t *effMagaGrOtBuckets[4];

/* Creation-table entries return the newly allocated type-specific work. */
typedef struct effCreateHandler_t {
    void *(*handler)(void *);
    u8 unk4[0x14];
} effCreateHandler_t;

extern effCreateHandler_t dds3MagatuhiCreateTable[];

/* Effect callback dispatch entry (0x18); DDS1/2 game/code_0018CAC8/00194700.c. */
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

extern effVectorHandler_t dds3MagatuhiSetPositionTable[];

extern effColorHandler_t dds3MagatuhiSetRGBATable[];

extern effVectorHandler_t dds3MagatuhiSetRotateMatrixTable[];

extern char *dds3EffectFileNameTable[];

extern s8 effDebugEnabled;

extern char dds3EffectDirPathFormat[];

extern u32 effDebugAttr;

extern s16 effMagaBillboardTexCoords[15][8];

/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct dmaPacketHeader_t {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} QWORD_ALIGNED dmaPacketHeader_t;

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

/* The creation callback's returned payload is retained in this 8-byte dispatch owner. */
typedef struct dds3MagatuhiPar_tag {
    u32 type;
    void *pHandle;
} dds3MagatuhiPar_tag;

/* Callback input prefix; type 3 contains an effect-dispatch object at +8. */
typedef struct {
    s32 type;
    u8 pad04[4];
    dds3MagatuhiPar_tag *effect;
} effMagatuhiCallback_t;

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

/* Float source block read by effMagatuhiCopyFloatBlock. */
typedef struct {
    f32 originX, originY, originZ;
    u8 pad0C[4];
    f32 targetX, targetY, targetZ;
    u8 pad1C[4];
    f32 initialRadius, captureDistance;
} effMagatuhiFloatParams_t; /* 0x28 */

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

/* Four control points and their jitter scales; copy xyz but preserve each w. */
typedef struct {
    f32 controlPoints[EFF_MAGATUHI_CONTROL_POINT_COUNT][EFF_MAGATUHI_VECTOR_WORD_COUNT];
    f32 jitterScales[EFF_MAGATUHI_CONTROL_POINT_COUNT];
} effMagatuhiRowsSrc_t;

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

/* Effect subrecord with byte fields at +0x20/+0x40/+0x50 (0x51); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct effSub_t {
    u8 unk0[0x20];
    u8 type0SubSlot;
    u8 unk21[0x1F];
    u8 type1SubSlot;
    u8 unk41[0xF];
    u8 type2To4SubSlot;
} effSub_t;

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

extern void dds3AdminiCallSequence(s32, void *, u32, s32);

/* Float-input SDK form: retain the broad memory effect and let GCC transfer
 * the binary32 scale to the unpack's GPR operand. The exact surface-grid
 * gradient uses four such 16-byte carriers (DDS1 002A7288, DDS2 002E9840).
 * This is distinct from the integer-bit, read-only form above. */
static inline void UnpackRGBAWordFloat(u32 color, f32 unit) {
    u32 rgba[4];
    rgba[0] = color;
    MMI_RGBA_UNPACK(rgba, unit);
}

/* Allocate/retain slot storage and return its header after the slots, not the slot base.
   Only the two native slot defaults are initialized; count/allocation validity is unchecked. */
effArrHdr_t *dds3Bezier4PCreate(s32 number);

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern u32 dds3EffectBuiltinNameIndex;

extern u8 dds3EffectBase[];

sdfTexHandle_t *dds3BillboardGetGeneralTex(s32 number);

effCubicBezierSlot_t *dds3Bezier4PGetPart(struct effArrHdr_t *pEffBezier4P, s32 index);

void dds3Bezier4PDelete(struct effArrHdr_t *pEffBezier4P);

s32 dds3Bezier4PPartRun(struct effArrHdr_t *table, s32 slotIndex, f32 *out);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(u8 *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(u8 *name);

/* vu0 routine: multiply RGBA channels normalized by 128, then pack at the same scale.
   Native integer conversion/byte packing remains unclamped; quadword storage is unchanged. */
u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB);

extern s8 effDebugActive;

extern s8 effDebugCameraAllowed;

extern s8 dds3PadBitTable[];

extern f32 aViewDown[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 effMagaSavedViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 effMagaSavedViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 effMagaSavedViewDown[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern s32 D_002F56C0[2][2][16];

extern s32 D_002F57C0[2][2];

extern u8 dds3PadButtonPrev[2][2][16];

extern u8 dds3PadButtonState[2][2][16];

extern s8 dds3PadStickPrev[2][2][2];

extern s8 dds3PadStickState[2][2][2];

extern s8 dds3PadStickStatusPrev[2][2];

extern s8 dds3PadStickStatusState[2][2];

extern effDebugMenu_t *effDebugMenu;

extern sdfGrOt_t sdfGrOtTable[];

SYMBOL_AT(sdfGrOt_t, sdfDebugGrOt, sdfGrOtTable, 0xA60);

extern struct sdfDrawTag_t *effDebugDrawTag;

extern void *dds3PrimFrameRect(s32 aX, s32 aY, u32 aZ, s32 aW, s32 aH, u32 aBgCol, u32 aFrameCol);

extern void *dds3PrimLine(s32 aX0, s32 aY0, u32 aZ0, u32 aColor0, s32 aX1, s32 aY1, u32 aZ1, u32 aColor1, s32 aMode);

extern s32 effDebugPosX;

extern u32 effDebugPosY;

extern void *memcpy(void *a, const void *b, size_t c);

/* Fixed-scale form of the SDK's four-word RGBA packing scratch buffer. */
static inline u32 PackMagatuhiTrailColor(void) {
    u32 rgba[4];
    MMI_RGBA_PACK_UNIT(rgba[0], 128.0f);
    return rgba[0];
}

extern s32 sceDclose(s32 directory);

extern s32 sceDopen(void *path);

extern s32 sceDread(s32 directory, effDirEnt_t *entry);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void *sdfChipAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* Build XYZ and attribute VIF streams; return the packet address.
 * vertexCount must be positive; n retains its native count/byte-size reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *aCenter, const void *aColor, const void *aUVList, const void *aXYOffsetList, s32 aNum, void *(*aAllocator)(s32));

/* Round payloadBytes up to quadwords and pair FLUSHA with VIF DIRECT. */
void sdf3DMakeDirectRefPacket(dmaPacketHeader_t *aPacket, addr_t aData, s32 aLeng);

f32 sdfCos(f32 aRadius);

void sdfBillBoardMakeTMPacket(u8 *aPacket, const f32 *aTM);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

extern u8 sdfHddValid;

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

f32 sdfSin(f32 aRadius);

/* Requires a non-NULL texture; returns its primary buffer as a borrowed view. */
struct sdfTexPacket_t *sdfTexGetPacket(struct sdfTexHandle_t *aHandle);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
s32 sdfTexGetPacketLength(const struct sdfTexHandle_t *aHandle);

extern f32 aViewTarget[EFF_MAGATUHI_VECTOR_WORD_COUNT];

extern f32 aViewEye[EFF_MAGATUHI_VECTOR_WORD_COUNT];

#include "sdk_libc.h"

extern char * strcpy(char *, const char *);

extern u32 strlen(const char *text);

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source);

void effMagaCylinderFree(effMagatuhiOwner_t *resource);

void effMagaCylinderDrawTrack(effMagatuhiOwner_t *resource);

void effMagaCylinderSetPartPtrs(dds3MagatuhiBill_tag *work, s32 slotIndex);

dds3MagatuhiBill_tag *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep);

/* Release the allocation retained by the render/history owner. */
void effMagaCylinderFreeParts(dds3MagatuhiBill_tag *work);

void effSwordTrackDrawTex(dds3MagatuhiBill_tag *work);

void effMagaCylinderSetPartPos(dds3MagatuhiBill_tag *work, s32 index, const sdfVector4_t *vector);

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(dds3MagatuhiBill_tag *work, s32 index, u32 color);

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(dds3MagatuhiBill_tag *work, u32 colorA, u32 colorB);

void effMagaSmokeInitPart(dds3MagatuhiBill_tag *valueWork, s32 index);

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagaSmokeConv_tag *effMagaSmokeConvCreate(effMagaSmokeConvParam_t *src);

/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagaSmokeConv_tag *pData);

/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w remains unwritten, as in the retail routine. */
void effMagaSmokeConvPartInit(effMagaSmokeConv_tag *pMagaSmokeConv, s32 index);

/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void effMagaSmokeConvDraw(effMagaSmokeConv_tag *pData);

/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *hEffect, effMagatuhiFloatParams_t *pMagaSCArea);

/* Clone history parameters; signed delays follow the returned work block. */
effMagaPoint4_tag *effMagaPoint4Create(effMagaPoint4Param_t *src);

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagaPoint4_tag *pData);

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagaPoint4_tag *pMagaPoint4, s32 index);

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagaPoint4_tag *pData);

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *hEffect, effMagatuhiRowsSrc_t *pP4Point);

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *hEffect);

/* Clone ring parameters; the returned work precedes its individual slots. */
effMagaCylinder_tag *effMagaCylinderCreate(effMagaCylinderParam_t *src);

/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagaCylinder_tag *pData);

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagaCylinderPartInit(effMagaCylinder_tag *pMagaCylinder, s32 index);

/* Sample the current fixed-height ring before advancing angle and radius.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaCylinderDraw(effMagaCylinder_tag *pData);

/* Copy the ring origin as one quadword, including its fourth component. */
void effMagaCylinderSetPosition(effMagaCylinder_tag *pData, void *pPos);

/* The historical symbol names a resource, but this slot is packed color. */
void effMagaCylinderSetRGBA(effMagaCylinder_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaCylinderSetRotateMatrix(effMagaCylinder_tag *pData, void *pMatrix);

/* Replay the first slot of each three-slot group from a random age. */
void effMagaCylinderRandomize(effMagatuhiCallback_t *hEffect);

/* Clone orbit parameters; the returned work precedes its individual slots. */
effMagaSpiral_tag *effMagaSpiralCreate(effMagaSpiralParam_t *src);

/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagaSpiral_tag *pData);

/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagaSpiralPartInit(effMagaSpiral_tag *pMagaSpiral, s32 index);

/* Sample and transform the current orbit before advancing its three rates.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSpiralDraw(effMagaSpiral_tag *pData);

/* Copy the orbit origin as one quadword, preserving the source w. */
void effMagaSpiralSetPosition(effMagaSpiral_tag *pData, void *pPos);

/* This callback sets the packed orbit color, not an allocation handle. */
void effMagaSpiralSetRGBA(effMagaSpiral_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSpiralSetRotateMatrix(effMagaSpiral_tag *pData, void *pMatrix);

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *hEffect);

/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagaSmoke_tag *effMagaSmokeCreate(effMagaSmokeParam_t *src);

/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagaSmoke_tag *pData);

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagaSmokePartInit(effMagaSmoke_tag *pMagaSmoke, s32 index);

/* Accumulate lift, transform the drift sample, then advance scale and angle.
 * Keep the cached age across initialization and preserve fade-in priority. */
void effMagaSmokeDraw(effMagaSmoke_tag *pData);

/* Copy the drift origin as one quadword, preserving the source w. */
void effMagaSmokeSetPosition(effMagaSmoke_tag *pData, void *pPos);

/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagaSmokeSetRGBA(effMagaSmoke_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSmokeSetRotateMatrix(effMagaSmoke_tag *pData, void *pMatrix);

/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *hEffect);

/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagaRandomize(effMagatuhiCallback_t *hEffect);

/* Allocate the dispatch owner before calling the selected payload constructor. */
dds3MagatuhiPar_tag *dds3MagatuhiParCreate(s32 type, void *params);

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(dds3MagatuhiPar_tag *hData);

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(dds3MagatuhiPar_tag *hData);

/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(dds3MagatuhiPar_tag *hMagaPar);

/* Read the dispatch owner's type word; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(dds3MagatuhiPar_tag *hMagaPar);

/* Native node type-three condition slot: accept with the constant result one. */
u32 dds3MagatuhiParIsActive(void);

/* Node set-vector slot: invoke optional subtype callback A; preserve the one-formal surface. */
void dds3MagatuhiParSetPosition(dds3MagatuhiPar_tag *work, void *pPos);

/* Node set-matrix slot: invoke optional subtype callback B; preserve the one-formal surface. */
void dds3MagatuhiParSetRotateMatrix(dds3MagatuhiPar_tag *work, void *pMatrix);

/* Node parameter-word slot: invoke optional subtype callback C; preserve the one-formal surface. */
void dds3MagatuhiParSetRGBA(dds3MagatuhiPar_tag *work, u32 abgr);

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(dds3MagatuhiPar_tag *work, s32 slotValue);

/* Read the type-specific subslot, or zero for other types.
   The callback's second argument is unused. */
u32 func_001A8DF8(dds3MagatuhiPar_tag *work, s32 unusedSlotValue);

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
dds3MagatuhiPar_tag *dds3MagatuhiParCopy(dds3MagatuhiPar_tag *work);

void InitMagatuhiEditorProcess(void);

void effDebugDrawRectLine(s32 x, s32 y, s32 radius, s32 drawCross);

effDebugMenu_t *func_001AA310(effDebugMenu_t *viewer);

s32 func_001AA5A0(void);

effDebugMenu_t *dblurUpdateHandle(effDebugMenu_t *pMenu);

void effDebugDrawMenu(effDebugMenu_t *menu);

void func_001AA6D0(void);

s32 eff_1AA3B0(void);

void effDebugAllocDrawTag(void);

void effDebugLinkDrawTag(void);

void effDebugDrawRect3(s32 x, s32 y, u32 fillColor);

void effDebugDrawRect2(s32 x, s32 y);

void effDebugDrawRect4(s32 x, s32 y, s32 width, s32 height);

void effDebugDrawRectFull(void);

void effDebugPrintf(const char *text);

s32 eff_1AA550(void);

void effDebugSetPos(s32 x, s32 y);

void effDebugSetAttr(u32 attributes);

s32 dds3_1AACF8(void);

/* Debug filesystem mode opens the formatted pfs0 directory; otherwise reset built-in iteration.
   Return the native directory-open result, or zero for built-in mode. */
s32 dds3EffectDirOpen(void *name);

/* Run the native directory callback only in debug filesystem mode; its result is ignored. */
void dds3EffectDirClose(s32 directory);

/* Debug mode delegates to the native iterator. Built-in mode copies the next name,
   clears flag mask 0x1000, and returns name length; zero also marks table exhaustion. */
s32 dds3EffectDirRead(s32 directory, effDirEnt_t *entry);

effMagatuhiOwner_t *effMagaCylinderAllocPart(const u32 *source);

void effMagaCylinderFree(effMagatuhiOwner_t *resource);

void effMagaCylinderDrawTrack(effMagatuhiOwner_t *resource);

void effMagaCylinderSetPartPtrs(dds3MagatuhiBill_tag *work, s32 slotIndex);

dds3MagatuhiBill_tag *effMagaCylinderAllocTex(s32 count, s32 historyCount, f32 historyDistanceScale, s32 param, f32 historyAngleStep);

/* Release the allocation retained by the render/history owner. */
void effMagaCylinderFreeParts(dds3MagatuhiBill_tag *work);

void effSwordTrackDrawTex(dds3MagatuhiBill_tag *work);

void effMagaCylinderSetPartPos(dds3MagatuhiBill_tag *work, s32 index, const sdfVector4_t *vector);

/* Store a per-slot packed color; the native path does not validate the index. */
void effMagaCylinderSetPart(dds3MagatuhiBill_tag *work, s32 index, u32 color);

/* Accumulate 1/historyCount per entry; the nominal last fraction is below 1. */
void effMagaCylinderBlendColor(dds3MagatuhiBill_tag *work, u32 colorA, u32 colorB);

void effMagaSmokeInitPart(dds3MagatuhiBill_tag *valueWork, s32 index);

/* Clone the first-family parameters; return the work after its state array.
 * Clamp only the copied delay modulus, leaving the caller's parameters intact. */
effMagaSmokeConv_tag *effMagaSmokeConvCreate(effMagaSmokeConvParam_t *src);

/* Release math state, the history owner, then the backing allocation. */
void effMagaSmokeConvDelete(effMagaSmokeConv_tag *pData);

/* Seed independent normalized XZ position/drift vectors and clear slot history.
 * The vector scratch w remains unwritten, as in the retail routine. */
void effMagaSmokeConvPartInit(effMagaSmokeConv_tag *pMagaSmokeConv, s32 index);

/* Drift until the particle enters the capture radius, then follow a randomized
 * cubic path to the target. Fade-in takes priority over an overlapping fade-out.
 * Preserve the native unwritten point w and unchecked fade divisors. */
void effMagaSmokeConvDraw(effMagaSmokeConv_tag *pData);

/* Update the first family's scattered float parameters without touching gaps. */
void effMagaSCSetArea(effMagatuhiCallback_t *hEffect, effMagatuhiFloatParams_t *pMagaSCArea);

/* Clone history parameters; signed delays follow the returned work block. */
effMagaPoint4_tag *effMagaPoint4Create(effMagaPoint4Param_t *src);

/* Release Bezier slots, their history owner, then the backing allocation. */
void effMagaPoint4Delete(effMagaPoint4_tag *pData);

/* Jitter control points perpendicular to the path and camera viewing direction. */
void effMagaPoint4PartInit(effMagaPoint4_tag *pMagaPoint4, s32 index);

/* Advance delayed Bezier particles, pack their fade, and submit the owner.
 * Fade-in wins when its interval overlaps fade-out; divisors are unchecked. */
void effMagaPoint4Draw(effMagaPoint4_tag *pData);

/* Replace control-point xyz and jitter amplitudes without changing point w. */
void effMagaP4SetPoint(effMagatuhiCallback_t *hEffect, effMagatuhiRowsSrc_t *pP4Point);

/* Seed every fourth slot with interpolated samples between two curve positions.
 * Keep the asymmetric duration/age subtraction and unchecked span divisor. */
void effMagaP4Randomize(effMagatuhiCallback_t *hEffect);

/* Clone ring parameters; the returned work precedes its individual slots. */
effMagaCylinder_tag *effMagaCylinderCreate(effMagaCylinderParam_t *src);

/* Release the ring history owner before the work/state allocation. */
void effMagaCylinderDelete(effMagaCylinder_tag *pData);

/* Initialize a ring slot and reset the history/value owned by that slot. */
void effMagaCylinderPartInit(effMagaCylinder_tag *pMagaCylinder, s32 index);

void effMagaCylinderDraw(effMagaCylinder_tag *pData);

/* Copy the ring origin as one quadword, including its fourth component. */
void effMagaCylinderSetPosition(effMagaCylinder_tag *pData, void *pPos);

/* The historical symbol names a resource, but this slot is packed color. */
void effMagaCylinderSetRGBA(effMagaCylinder_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaCylinderSetRotateMatrix(effMagaCylinder_tag *pData, void *pMatrix);

/* Replay the first slot of each three-slot group from a random age. */
void effMagaCylinderRandomize(effMagatuhiCallback_t *hEffect);

/* Clone orbit parameters; the returned work precedes its individual slots. */
effMagaSpiral_tag *effMagaSpiralCreate(effMagaSpiralParam_t *src);

/* Release the orbit history owner before the work/state allocation. */
void effMagaSpiralDelete(effMagaSpiral_tag *pData);

/* Seed randomized height/angular/radial increments and clear the slot history. */
void effMagaSpiralPartInit(effMagaSpiral_tag *pMagaSpiral, s32 index);

void effMagaSpiralDraw(effMagaSpiral_tag *pData);

/* Copy the orbit origin as one quadword, preserving the source w. */
void effMagaSpiralSetPosition(effMagaSpiral_tag *pData, void *pPos);

/* This callback sets the packed orbit color, not an allocation handle. */
void effMagaSpiralSetRGBA(effMagaSpiral_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSpiralSetRotateMatrix(effMagaSpiral_tag *pData, void *pMatrix);

/* Advance the orbiting-particle family: each group of three slots is replayed from a random delay, one orbit step at a time, into the value table. */
void effMagaSpiralRandomize(effMagatuhiCallback_t *hEffect);

/* Clone drift parameters; return the work after its 0x30-byte state array. */
effMagaSmoke_tag *effMagaSmokeCreate(effMagaSmokeParam_t *src);

/* Release the drift history owner before its states-plus-work allocation. */
void effMagaSmokeDelete(effMagaSmoke_tag *pData);

/* vu0 routine: initialize a planar particle with normalized position/drift. */
void effMagaSmokePartInit(effMagaSmoke_tag *pMagaSmoke, s32 index);

void effMagaSmokeDraw(effMagaSmoke_tag *pData);

/* Copy the drift origin as one quadword, preserving the source w. */
void effMagaSmokeSetPosition(effMagaSmoke_tag *pData, void *pPos);

/* This callback sets packed drift color; +0x120 is not an owner pointer. */
void effMagaSmokeSetRGBA(effMagaSmoke_tag *pData, u32 abgr);

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effMagaSmokeSetRotateMatrix(effMagaSmoke_tag *pData, void *pMatrix);

/* Replay the first slot of each three-slot drift group from a random age. */
void effMagaSmokeRandomize(effMagatuhiCallback_t *hEffect);

/* Dispatch replay side effects; retain the native s32 definition's fall-through. */
s32 effMagaRandomize(effMagatuhiCallback_t *hEffect);

/* Allocate the dispatch owner before calling the selected payload constructor. */
dds3MagatuhiPar_tag *dds3MagatuhiParCreate(s32 type, void *params);

/* Invoke the primary type callback with the payload; no NULL or type bounds check. */
void dds3MagatuhiParDraw(dds3MagatuhiPar_tag *hData);

/* Invoke the release callback, then free the outer work; no callback NULL check. */
void dds3MagatuhiParDelete(dds3MagatuhiPar_tag *hData);

/* Return the retained payload passed to this type's callbacks. */
void *dds3MagatuhiGetHandle(dds3MagatuhiPar_tag *hMagaPar);

/* Read the dispatch owner's type word; the pointer remains unchecked. */
u32 dds3MagatuhiGetType(dds3MagatuhiPar_tag *hMagaPar);

void func_001A8CD0(void);

/* Native node type-three condition slot: accept with the constant result one. */
u32 dds3MagatuhiParIsActive(void);

/* Node set-vector slot: invoke optional subtype callback A with the position. */
void dds3MagatuhiParSetPosition(dds3MagatuhiPar_tag *work, void *pPos);

/* Node set-matrix slot: invoke optional subtype callback B with the matrix. */
void dds3MagatuhiParSetRotateMatrix(dds3MagatuhiPar_tag *work, void *pMatrix);

/* Node parameter-word slot: invoke optional subtype callback C with the colour. */
void dds3MagatuhiParSetRGBA(dds3MagatuhiPar_tag *work, u32 abgr);

/* Store the low byte of slotValue at the type-specific subslot.
   Types two through four share the same location; other types are left unchanged. */
void dds3MagatuhiParSetByType(dds3MagatuhiPar_tag *work, s32 slotValue);

/* Read the type-specific subslot, or zero for other types.
   The callback's second argument is unused. */
u32 func_001A8DF8(dds3MagatuhiPar_tag *work, s32 unusedSlotValue);

/* Types zero/one dispatch from the payload base; two through four skip its prefix.
   Other types still dispatch with a zero argument; no type bounds check is added. */
dds3MagatuhiPar_tag *dds3MagatuhiParCopy(dds3MagatuhiPar_tag *work);

void func_001A8EA8(void);

void InitMagatuhiEditorProcess(void);

void effDebugDrawRect3(s32 x, s32 y, u32 fillColor);

void effDebugDrawRect2(s32 x, s32 y);

extern u32 effDebugStopTimer, effDebugRunTimer;

extern char effDebugTitleFormat[], effDebugLabelFormat[], effDebugTrueText[], effDebugFalseText[], effDebugHeaderFormat[];

void effDebugDrawMenu(effDebugMenu_t *menu);

void effDebugAllocDrawTag(void);

void effDebugLinkDrawTag(void);

void effDebugSetPos(s32 x, s32 y);

void effDebugSetAttr(u32 attributes);

void effDebugPrintf(const char *text);

void effDebugDrawRectFull(void);

void effDebugDrawRect4(s32 x, s32 y, s32 width, s32 height);

effDebugMenu_t *func_001AA310(effDebugMenu_t *viewer);

void effDebugInit(void);

s32 eff_1AA3B0(void);

s32 eff_1AA550(void);

void func_001AA598(void);

s32 func_001AA5A0(void);

s32 dds3_1AACF8(void);

#endif
