#include "common.h"
#include "effPCPThunder.h"
#include "effPCPData.h"
#include "effThunder.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#include "sdf_types.h"
#include "sdfFrame.h"
#include "sdk_libc.h"
#include "sdfMath.h"
#include "sdfVu0.h"
#include "effect.h"
#include "libsdf.h"
#include "sdfChip.h"
#include "sdfMemory.h"
#include "polyManager.h"
#include "effMagatuhi.h"
#include "dds3KernelMain.h"


struct effPCPThunderSpherePolyParam_t;

struct effPCP_ThunderRadiatePolyPart_t;

struct effPCP_ThunderRadiatePolyParam_t;

struct effPCP_ThunderRadiatePoly_t;

struct effPCP_ThunderPolyPart_t;

struct effPCP_ThunderPolyList_t;

struct effPCP_ThunderColumnPolyLowPart_t;

struct effPCP_ThunderColumnPolyLowParam_t;

struct effPCP_ThunderColumnPolyLow_t;

struct effPCPThunderSpherePolyPart_t;

struct effPCPThunderSpherePoly_t;


struct sdfVector3_t;


struct sdfGsMemSlice_t;


typedef struct effPCP_ThunderRadiatePolyPart_t effPCP_ThunderRadiatePolyPart_t;

typedef struct effPCP_ThunderRadiatePolyParam_t effPCP_ThunderRadiatePolyParam_t;



typedef struct effPCP_ThunderRadiatePoly_t effPCP_ThunderRadiatePoly_t;

typedef struct effPCP_ThunderPolyPart_t effPCP_ThunderPolyPart_t;

typedef struct effPCP_ThunderColumnPolyLowPart_t effPCP_ThunderColumnPolyLowPart_t;

typedef struct effPCP_ThunderColumnPolyLowParam_t effPCP_ThunderColumnPolyLowParam_t;

typedef struct effPCP_ThunderColumnPolyLow_t effPCP_ThunderColumnPolyLow_t;

typedef struct effPCPThunderSpherePolyPart_t effPCPThunderSpherePolyPart_t;

typedef struct effPCPThunderSpherePolyParam_t effPCPThunderSpherePolyParam_t;

typedef struct effPCPThunderSpherePoly_t effPCPThunderSpherePoly_t;

#define EFF_THUNDER_VECTOR_COMPONENTS 4

#define EFF_THUNDER_NEUTRAL_COLOR 0x80808080

#define EFF_THUNDER_ALPHA_MASK 0xFF000000

#define EFF_THUNDER_ALPHA_WRAP_ADD 0xE0000000

#define EFF_THUNDER_ALPHA_FADE_STEP 0x20000000

#define EFF_THUNDER_RANDOM_MIDPOINT 0.5f

#define EFF_THUNDER_RANDOM_SPAN 2.0f

#define EFF_THUNDER_AXIS_JITTER 0.5f

#define EFF_THUNDER_ROTATION_JITTER 0.4f

#define EFF_THUNDER_RADIUS_JITTER 0.3f

#define EFF_THUNDER_HALF_SCALE 0.5f

#define EFF_THUNDER_HALF_TURN 3.14159265f

#define EFF_THUNDER_MIN_ACTIVE_FRAMES 1

/* Per-particle countdown state in the paired thunder-cell implementations. */
struct effPCP_ThunderRadiatePolyPart_t {
    f32 directionX; /* 0x00 */
    f32 directionY; /* 0x04 */
    f32 directionZ; /* 0x08 */
    u32 delayFrames; /* 0x0C */
    u32 activeFrames; /* 0x10 */
};

/* Copied parameter prefix for the counted-down cell effect. */
struct effPCP_ThunderRadiatePolyParam_t {
    f32 origin[4];
    u16 blend;
    u8 pad12[2];
    f32 length;
    u32 number;
    u8 opaque1C[8];
    u32 startDelayRange;
    u32 activeFrameRange;
    u16 split;
    u8 pad2E[2];
    f32 coreWidth;
    u32 color_center;
    f32 bandWidth;
    u32 color_border;
    f32 edgeWidth;
    u32 color_edge;
};


struct sdfMat_t;


struct sdfTexHandle_t;


/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

/* Header followed by a variable-length effPCP_ThunderRadiatePolyPart_t array in one allocation. */
struct effPCP_ThunderRadiatePoly_t {
    effPCP_ThunderRadiatePolyParam_t head;
    effPCP_ThunderRadiatePolyPart_t *pPart; /* 0x48 */
    u32 abgr; /* 0x4C: settable, otherwise unobserved */
    dds3TrackPoly_t *hPoly; /* 0x50 */
    sdfMemHandle_t *hMem; /* 0x54 */
};

/* Per-fragment countdowns and the packed color consumed by thunder updates. */
struct effPCP_ThunderPolyPart_t {
    u32 wait;  /* 0x00 */
    u32 refresh; /* 0x04 */
    u32 abgr;        /* 0x08 */
};

struct effPCP_ThunderPoly_t;

typedef struct effPCP_ThunderPoly_t effPCP_ThunderPoly_t;

struct effPCP_ThunderPoly_t {
    effPCP_ThunderPolyParam_t head;
    effPCP_ThunderPolyPart_t *pPart; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-hPoly variant */
        dds3TrackPoly_t *secondarySystem; /* dual-hPoly variant */
    } state;                 /* 0x5C */
    dds3TrackPoly_t *hPoly;            /* 0x60 */
    sdfMemHandle_t *allocationHandle; /* 0x64 */
}; /* 0x68 */

struct effPCP_ThunderPolyList_t {
    effPCP_ThunderPolyListParam_t head;
    effPCP_ThunderPoly_t *handles[10];
    u32 color;
};

/* One orbiting spark and its independently owned particle cell system. */
struct effPCP_ThunderColumnPolyLowPart_t {
    s32 frame;              /* 0x00 */
    f32 up;   /* 0x04 */
    f32 up_s;    /* 0x08 */
    f32 rotate;       /* 0x0C */
    f32 rotate_s;     /* 0x10 */
    f32 radius;      /* 0x14 */
    f32 height;     /* 0x18 */
    dds3TrackPoly_t *hPoly;    /* 0x1C */
};

/* Copied spark constructor input; gaps remain opaque without stronger evidence. */
struct effPCP_ThunderColumnPolyLowParam_t {
    f32 origin[4];                   /* 0x00 */
    f32 loweredPosition[4];          /* 0x10 */
    f32 position[4];                 /* 0x20 */
    u16 systemParam;                 /* 0x30 */
    u8 pad32[0x0A];
    f32 waveAmplitude;               /* 0x3C */
    u8 pad40[8];
    u16 halfLife;                    /* 0x48 */
    u8 pad4A[2];
    f32 width;                       /* 0x4C */
    void *dispatchArg;               /* 0x50 */
    u8 pad54[0x10];
    f32 heightOffset;                /* 0x64 */
    u32 sparkCount;                  /* 0x68 */
    u8 loop;                         /* 0x6C */
    u8 pad6D[3];
    s32 duration;                    /* 0x70 */
    s32 startDelaySpread;            /* 0x74 */
    s32 fadeInTime;                  /* 0x78 */
    s32 fadeOutTime;                 /* 0x7C */
    f32 orbitRadius;                 /* 0x80 */
    f32 radiusRandomness;            /* 0x84 */
    f32 heightOffsetRange;           /* 0x88 */
    f32 angularSpeed;                /* 0x8C */
    f32 angularSpeedRandomness;      /* 0x90 */
    f32 angularDamping;              /* 0x94 */
    f32 verticalSpeed;               /* 0x98 */
    f32 verticalSpeedRandomness;     /* 0x9C */
    f32 verticalDamping;             /* 0xA0 */
};

/* Allocation header followed by sparkCount records of effPCP_ThunderColumnPolyLowPart_t. */
struct effPCP_ThunderColumnPolyLow_t {
    effPCP_ThunderColumnPolyLowParam_t param;      /* 0x00 */
    effPCP_ThunderColumnPolyLowPart_t *pPart;         /* 0xA4 */
    u32 abgr;                   /* 0xA8 */
    sdfMemHandle_t *hMem;   /* 0xAC */
};

struct effPCPThunderSpherePolyPart_t {
    u32 wait;
    u32 refresh;
    f32 dir[3];
    f32 axis[3];
    f32 rotate;
    f32 radius;
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
/* The complete work is followed by cellCount cells in the same allocation. */
struct effPCPThunderSpherePoly_t {
    effPCPThunderSpherePolyParam_t head;
    effPCPThunderSpherePolyPart_t *pPart;
    u32 abgr;
    f32 width_base;
    f32 height_base;
    struct dds3TrackPoly_t *hPoly;
    struct sdfMemHandle_t *allocationHandle;
};

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

extern effRandState_t dds3EffectBase[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);
extern void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number);
extern void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);
extern void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);
extern void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed);
void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);
void sdfChipFreeDirect(void *aPtr);
extern f32 aViewTarget[4], aViewEye[4];

#define EFF_THUNDER_PARAMETER_BLOCK 0

#define EFF_THUNDER_MIN_DELAY_SPREAD 1

#define EFF_THUNDER_SINGLE_CELL 1

#define EFF_THUNDER_ENDPOINT_VECTOR_BYTES 0x10

enum trackPolyTopology {
    PAR_CELL_TOPOLOGY_PAIR = 0,
    PAR_CELL_TOPOLOGY_TRIANGLE = 1,
    PAR_CELL_TOPOLOGY_SIX_VECTOR = 2,
    PAR_CELL_TOPOLOGY_FIVE_VECTOR = 4
};

extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern void dds3TrackPolySetMode(dds3TrackPoly_t *hTrackPoly, u16 mode);
void func_00177AC8(effPCP_ThunderColumnPolyLow_t *work, s32 index);
void effPCP_ThunderPolyDrawOff(effPCP_ThunderPoly_t *work, s32 index);
void effPCP_ThunderPolyLowDrawOff(effPCP_ThunderPoly_t *work, s32 index);
void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index);
effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *source);
void *effPCPThunderSpherePolyCreatePCF(void *parameterTable);
void *effPCPThunderSpherePolyCopy(void *source);
void effPCPThunderSphereRotate(effPCPThunderSpherePoly_t *work, s32 index);
void effPCPThunderSphereRotateAxis(effPCPThunderSpherePoly_t *work, s32 index);
effPCPThunderSpherePoly_t *effBTLBindCreateTrack(effPCPThunderSpherePolyParam_t *parameters);
u32 effPCPThunderSpherePolyLowGetParam(u32 pThunderPoly);
void effPCPThunderSphereLowRotate(effPCPThunderSpherePoly_t *work, s32 index);
void effPCPThunderSphereLowRotateAxis(effPCPThunderSpherePoly_t *work, s32 index);
effPCP_ThunderColumnPolyLow_t *effThunderSparkCreate(effPCP_ThunderColumnPolyLowParam_t *parameters);
void effThunderReleaseSparkWork(effPCP_ThunderColumnPolyLow_t *work);
void func_00177BC0(effPCP_ThunderColumnPolyLow_t *work, s32 index);
void effThunderUpdateOrbitingSparks(effPCP_ThunderColumnPolyLow_t *work);
effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters);
void effPCP_ThunderPolySetVCAcute(effPCP_ThunderPoly_t *pThunderPoly);
void effPCP_ThunderPolySetVCAcute2(effPCP_ThunderPoly_t *pThunderPoly);
void effPCP_ThunderPolySetVCNormal(effPCP_ThunderPoly_t *pThunderPoly);
void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);
void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData);
effPCP_ThunderPoly_t *effPCP_ThunderPolyLowCreate(effPCP_ThunderPolyParam_t *parameters);
void effPCP_ThunderPolyLowDelete(effPCP_ThunderPoly_t *hData);
void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);
void effPCP_ThunderPolyLowDraw(effPCP_ThunderPoly_t *hData);
effPCP_ThunderRadiatePoly_t *effPCP_ThunderRadiatePolyCreate(effPCP_ThunderRadiatePolyParam_t *parameters);
void effPCP_ThunderRadiatePolyDelete(effPCP_ThunderRadiatePoly_t *hData);
void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number);
void effPCP_ThunderRadiatePolyDraw(effPCP_ThunderRadiatePoly_t *hData);
void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed);

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *source) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(source->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocation);
    u32 cellIndex;

    work->head = *source;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->width_base = source->width;
    work->height_base = source->height;
    work->allocationHandle = allocation;
    work->hPoly = dds3TrackPolyCreate(work->head.cellCount, work->head.perCell, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->abgr = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void *effPCPThunderSpherePolyCreatePCF(void *pParam) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(pParam, EFF_THUNDER_PARAMETER_BLOCK);
    return effPCPThunderSpherePolyCreate(parameters);
}

void *effPCPThunderSpherePolyCopy(void *source) {
    return effPCPThunderSpherePolyCreate(source);
}

void effPCPFlashPolyCircleDelete(effPCPThunderSpherePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:515, match, same function 1.00 (words 4/4, calls 1.00, shape 1.00) */
void effPCPShockWave01DrawSub(void *hData, void *pPos) {
    SDF_VU0_COPYVECTOR(hData, pPos);
}

void effPCPShockWave01DrawSub2(effPCPThunderSpherePoly_t *hData, u32 value) {
    hData->abgr = value;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:523, match, same function 1.00 (words 7/7, calls 1.00, shape 1.00) */
void effPCPThunderSphereExpDrawSub(effPCPThunderSpherePoly_t *hData, f32 scale) {
    hData->head.width = hData->width_base * scale;
    hData->head.height = hData->height_base * scale;
}

void *effPCPShockWave01DrawSub3(void *pParam) {
    return pParam;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:802, match, same function 1.00 (words 142/142, calls 1.00, shape 1.00) */
void effPCPThunderSphereLowInit(effPCPThunderSpherePoly_t *work, s32 index) {
    effPCPThunderSpherePolyPart_t *cell = work->pPart + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    cell->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.height * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->dir[0] = scratchVector[0];
    cell->dir[1] = scratchVector[1];
    cell->dir[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->axis[0] = scratchVector[0];
    cell->axis[1] = scratchVector[1];
    cell->axis[2] = scratchVector[2];
    cell->rotate = work->head.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.width * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereRotate(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    sdfVector3_t rotation __attribute__((aligned(16)));
    dds3TrackPoly_t *system = work->hPoly;
    dds3TrackPolyList_t *cell = &system->trackList[index];
    effPCPThunderSpherePolyPart_t *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 radius;

    count = system->point / 5;
    cell->count = system->point;
    source = &work->pPart[index];
    vertices = cell->pVertex;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.2f + 1.0f;
    VEC3_SPLAT(width, work->head.width_border * widthScale);
    VEC3_SPLAT(outerWidth, width[0] + work->head.width_edge * widthScale);
    originalAxis[0] = source->axis[0];
    originalAxis[1] = source->axis[1];
    originalAxis[2] = source->axis[2];
    originalAxis[3] = 0.0f;
    rotationScale = -source->rotate;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->dir[0];
    placement[1] = 0.0f;
    placement[2] = source->dir[2];
    SDF_VU0_COPYVECTOR(origin, work->head.origin);
    origin[1] += source->dir[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation.x = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation.y = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        rotation.z = dds3GetRandFloatA(dds3EffectBase) * (75.0f * (EFF_THUNDER_HALF_TURN / 180.0f));
        SetRotateMatrixXYZ(&rotation);
        SDF_VU0_LOADVECTOR(axis);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.7f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(side);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
    }
}

void effPCPThunderSphereRotateAxis(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *system = work->hPoly;
    effPCPThunderSpherePolyPart_t *source = &work->pPart[index];
    dds3TrackPolyList_t *cell = &system->trackList[index];
    u128 *vertices = cell->pVertex;
    s32 rows = system->point / 5;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotate * sample * 3.0f;
    direction[0] = source->dir[0];
    direction[1] = 0.0f;
    direction[2] = source->dir[2];
    axis[0] = source->axis[0];
    axis[1] = source->axis[1];
    axis[2] = source->axis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    source->dir[0] = direction[0];
    source->dir[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 1);
        SDF_VU0_LOADVECTOR(vertices + 2);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 2);
        SDF_VU0_LOADVECTOR(vertices + 3);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 3);
        SDF_VU0_LOADVECTOR(vertices + 4);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 4);
        vertices += 5;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowDraw);

effPCPThunderSpherePoly_t *effBTLBindCreateTrack(effPCPThunderSpherePolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->cellCount * sizeof(effPCPThunderSpherePolyPart_t) + sizeof(effPCPThunderSpherePoly_t));
    effPCPThunderSpherePoly_t *work = (effPCPThunderSpherePoly_t *)sdfMemLock(allocationHandle);
    u32 cellIndex;

    work->head = *parameters;
    work->pPart = (effPCPThunderSpherePolyPart_t *)(work + 1);
    work->width_base = parameters->width;
    work->height_base = parameters->height;
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.cellCount, work->head.perCell, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, work->head.color_border, work->head.color_border);
    dds3TrackPolySetMode(work->hPoly, work->head.systemParam);
    for (cellIndex = 0; cellIndex < work->head.cellCount; cellIndex++) {
        work->pPart[cellIndex].wait = 0;
        work->pPart[cellIndex].refresh = 0;
        work->pPart[cellIndex].abgr = 0;
    }
    work->abgr = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCPFlashPolyRingExp2Delete(effPCPThunderSpherePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

void *effCallEffectPCPDataGetData(void *pParam) {
    void *parameters;

    parameters = dds3EffectPCPDataGetData(pParam, EFF_THUNDER_PARAMETER_BLOCK);
    return effBTLBindCreateTrack(parameters);
}

void *effPCPThunderSpherePolyLowCopy(void *parameters) {
    return effBTLBindCreateTrack(parameters);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowSetPosition);

void effPCPThunderSpherePolyLowSetColor(effPCPThunderSpherePoly_t *hData, u32 value) {
    hData->abgr = value;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCPThunderSpherePolyLowSetScale);

u32 effPCPThunderSpherePolyLowGetParam(u32 pThunderPoly) {
    return pThunderPoly;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:532, match, same function 1.00 (words 142/142, calls 1.00, shape 1.00) */
void effPCPThunderSphereInit(effPCPThunderSpherePoly_t *work, s32 index) {
    effPCPThunderSpherePolyPart_t *cell = work->pPart + index;
    f32 scratchVector[EFF_THUNDER_VECTOR_COMPONENTS];

    cell->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    cell->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    scratchVector[1] = 0;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    scratchVector[1] = work->head.height * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN);
    cell->dir[0] = scratchVector[0];
    cell->dir[1] = scratchVector[1];
    cell->dir[2] = scratchVector[2];
    scratchVector[0] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    scratchVector[1] = 1.0f;
    scratchVector[2] = (dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_AXIS_JITTER;
    SDF_VU0_LOADVECTOR(scratchVector);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(scratchVector);
    cell->axis[0] = scratchVector[0];
    cell->axis[1] = scratchVector[1];
    cell->axis[2] = scratchVector[2];
    cell->rotate = work->head.rotationScale * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_ROTATION_JITTER + 1.0f);
    cell->radius = work->head.width * EFF_THUNDER_HALF_SCALE * ((dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT) * EFF_THUNDER_RANDOM_SPAN * EFF_THUNDER_RADIUS_JITTER + 1.0f);
    cell->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCPThunderSphereLowRotate(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 position[4] __attribute__((aligned(16)));
    f32 placement[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 originalAxis[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 origin[4] __attribute__((aligned(16)));
    f32 width[4] __attribute__((aligned(16)));
    sdfVector3_t rotation __attribute__((aligned(16)));
    dds3TrackPoly_t *system = work->hPoly;
    dds3TrackPolyList_t *cell = &system->trackList[index];
    effPCPThunderSpherePolyPart_t *source;
    u128 *vertices;
    s32 count;
    s32 i;
    f32 widthScale;
    f32 rotationScale;
    f32 angleJitter;
    f32 radius;

    count = system->point >> 1;
    cell->count = system->point;
    source = &work->pPart[index];
    vertices = cell->pVertex;
    widthScale = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f * 0.3f + 1.0f;
    VEC3_SPLAT(width, work->head.width_border * widthScale);
    originalAxis[0] = source->axis[0];
    originalAxis[1] = source->axis[1];
    originalAxis[2] = source->axis[2];
    originalAxis[3] = 0.0f;
    radius = source->radius;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    placement[0] = source->dir[0];
    placement[1] = 0.0f;
    placement[2] = source->dir[2];
    SDF_VU0_COPYVECTOR(origin, work->head.origin);
    origin[1] += source->dir[1];

    for (i = 0; i < count; i++) {
        position[0] = origin[0] + placement[0] * radius;
        position[1] = origin[1] + placement[1] * radius;
        position[2] = origin[2] + placement[2] * radius;
        SDF_VU0_COPYVECTOR(axis, originalAxis);
        rotation.x = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation.y = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        rotation.z = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 0.5f);
        SetRotateMatrixXYZ(&rotation);
        SDF_VU0_LOADVECTOR(axis);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(axis);
        angleJitter = dds3GetRandFloatA(dds3EffectBase);
        rotationScale = -source->rotate;
        SetRotateMatrixAxis(((angleJitter - 0.5f) * 2.0f * 0.8f) * rotationScale + rotationScale, (const struct sdfVector3_t *)axis);
        SDF_VU0_LOADVECTOR(placement);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(placement);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 1);
        vertices += 2;
    }
}

void effPCPThunderSphereLowRotateAxis(effPCPThunderSpherePoly_t *work, s32 index) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *system = work->hPoly;
    effPCPThunderSpherePolyPart_t *source = &work->pPart[index];
    dds3TrackPolyList_t *cell = &system->trackList[index];
    u128 *vertices = cell->pVertex;
    s32 rows = system->point >> 1;
    s32 i;
    f32 sample;
    f32 angle;

    sample = dds3GetRandFloatA(dds3EffectBase);
    angle = source->rotate * sample * 3.0f;
    direction[0] = source->dir[0];
    direction[1] = 0.0f;
    direction[2] = source->dir[2];
    axis[0] = source->axis[0];
    axis[1] = source->axis[1];
    axis[2] = source->axis[2];
    SetRotateMatrixAxis(angle, (const struct sdfVector3_t *)axis);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(direction);
    source->dir[0] = direction[0];
    source->dir[2] = direction[2];
    SDF_VU0_LOADVECTOR_B(work->head.origin);
    for (i = 0; i < rows; i++) {
        SDF_VU0_LOADVECTOR(vertices);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices);
        SDF_VU0_LOADVECTOR(vertices + 1);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(vertices + 1);
        vertices += 2;
    }
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effCallPCPThunderSphereLowRotate);

effPCP_ThunderColumnPolyLow_t *effThunderSparkCreate(effPCP_ThunderColumnPolyLowParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->sparkCount * sizeof(effPCP_ThunderColumnPolyLowPart_t) + sizeof(effPCP_ThunderColumnPolyLow_t));
    effPCP_ThunderColumnPolyLow_t *work = (effPCP_ThunderColumnPolyLow_t *)sdfMemLock(allocationHandle);
    s32 delaySpread;
    u32 sparkIndex;

    work->param = *parameters;
    work->pPart = (effPCP_ThunderColumnPolyLowPart_t *)(work + 1);
    work->abgr = EFF_THUNDER_NEUTRAL_COLOR;
    work->hMem = allocationHandle;
    if (work->param.startDelaySpread <= 0) {
        work->param.startDelaySpread = EFF_THUNDER_MIN_DELAY_SPREAD;
    }
    delaySpread = work->param.startDelaySpread;
    for (sparkIndex = 0; sparkIndex < work->param.sparkCount; sparkIndex++) {
        work->pPart[sparkIndex].hPoly = dds3TrackPolyCreate(EFF_THUNDER_SINGLE_CELL, work->param.halfLife * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
        dds3TrackPolyVCSet(work->pPart[sparkIndex].hPoly, 2, EE_ADDR(work->param.dispatchArg), EE_ADDR(work->param.dispatchArg));
        dds3TrackPolySetMode(work->pPart[sparkIndex].hPoly, work->param.systemParam);
        func_00177AC8(work, sparkIndex);
        work->pPart[sparkIndex].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delaySpread);
    }
    return work;
}

void effThunderReleaseSparkWork(effPCP_ThunderColumnPolyLow_t *work) {
    s32 sparkCount = (s32)work->param.sparkCount;
    s32 sparkIndex = 0;

    if (sparkCount > 0) {
        do {
            dds3TrackPolyDelete(work->pPart[sparkIndex].hPoly);
            sparkIndex++;
        } while (sparkIndex < sparkCount);
    }
    sdfMemFreeDirect(work->hMem);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effLeaf199);

void effLeaf200(effPCP_ThunderColumnPolyLow_t *hData, u32 value) {
    hData->abgr = value;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1033, match, same function 1.00 (words 61/61, calls 1.00, shape 1.00) */
void effThunderSparkInit(effPCP_ThunderColumnPolyLow_t *work, s32 index) {
    effPCP_ThunderColumnPolyLowPart_t *spark = work->pPart + index;
    f32 randomness;

    spark->up = 0;
    randomness = work->param.verticalSpeedRandomness;
    spark->up_s = work->param.verticalSpeed * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
    spark->height = work->param.heightOffsetRange * dds3GetRandFloatA(dds3EffectBase);
    spark->rotate = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    randomness = work->param.angularSpeedRandomness;
    spark->rotate_s = work->param.angularSpeed * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
    randomness = work->param.radiusRandomness;
    spark->radius = work->param.orbitRadius * (dds3GetRandFloatA(dds3EffectBase) * randomness + (1.0f - randomness));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effThunderBuildSparkStripCell);

void effThunderUpdateOrbitingSparks(effPCP_ThunderColumnPolyLow_t *work) {
    f32 position[3];
    f32 origin[4] __attribute__((aligned(16)));
    u32 tintColor;
    s32 count;
    s32 loop = work->param.loop;
    s32 duration = SDF_FRAMES(work->param.duration);
    s32 delaySpread = work->param.startDelaySpread;
    s32 fadeInTime = SDF_FRAMES(work->param.fadeInTime);
    s32 fadeOutTime = SDF_FRAMES(work->param.fadeOutTime);
    f32 heightOffset = work->param.heightOffset;
    f32 verticalDamping = SDF_FRAME_DECAY(work->param.verticalDamping);
    f32 angularDamping = SDF_FRAME_DECAY(work->param.angularDamping);
    effPCP_ThunderColumnPolyLowPart_t *spark;
    dds3TrackPolyList_t *renderCell;
    s32 i;
    s32 age;
    f32 angle;
    f32 radius;
    f32 alpha;
    f32 lowerHeight;
    u32 color;

    tintColor = work->abgr;
    SDF_VU0_COPYVECTOR(origin, work->param.origin);
    count = work->param.sparkCount;
    spark = work->pPart;
    for (i = 0; i < count; i++, spark++) {
        age = spark->frame;
        if (age == 0) {
            func_00177AC8(work, i);
            dds3TrackPolyListInit(spark->hPoly, 0);
        } else if (age > 0 && age <= duration) {
            angle = spark->rotate;
            radius = spark->radius;
            position[0] = origin[0] + sdfCos(angle) * radius;
            position[1] = origin[1] + spark->up - spark->height;
            position[2] = origin[2] + sdfSin(angle) * radius;
            spark->up += SDF_FRAME_STEP(spark->up_s);
            spark->rotate += SDF_FRAME_STEP(spark->rotate_s);
            spark->up_s *= verticalDamping;
            spark->rotate_s *= angularDamping;
            if (age < fadeInTime) {
                alpha = (f32)age / fadeInTime;
            } else if (duration - age <= fadeOutTime) {
                alpha = (f32)(duration - age) / fadeOutTime;
            } else {
                alpha = 1.0f;
            }
            color = effMagaCylinderCalcColor(((u32)(alpha * 127.0f) << 24) | 0x808080, tintColor);
            renderCell = spark->hPoly->trackList;
            lowerHeight = position[1] - heightOffset;
            work->param.loweredPosition[0] = position[0];
            work->param.loweredPosition[1] = lowerHeight;
            work->param.loweredPosition[2] = position[2];
            work->param.position[0] = position[0];
            work->param.position[1] = position[1];
            work->param.position[2] = position[2];
            func_00177BC0(work, i);
            renderCell->color = color;
            dds3TrackPolyDraw(spark->hPoly);
        }
        if (age >= duration && loop) {
            spark->frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % delaySpread);
        } else {
            spark->frame++;
        }
    }
}

effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_FIVE_VECTOR);
    dds3TrackPolyVC5Set_AcuteW(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyDrawOff(work, i);
    }
    work->state.updateCount = 0;
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1610, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCP_ThunderPolyLowSetPosition(u8 *hData, void *pPos) {
        SDF_VU0_LOADVECTOR(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(pPos);
        SDF_VU0_STOREVECTOR_B(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(hData);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(hData);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1320, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPCPRadiateThunderDrawSub(effPCP_ThunderPoly_t *hData, u32 abgr) {
    hData->color = abgr;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1324, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
effPCP_ThunderPolyParam_t *effPCPZiodin01DrawSub(effPCP_ThunderPoly_t *pThunderPoly) {
    return &pThunderPoly->head;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1328, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
void effThunderApplyFragmentColorBands(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_Acute(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1332, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
void effPCPThunderPolyListCreateSub(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_AcuteR(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1336, match, same function 1.00 (words 10/10, calls 1.00, shape 1.00) */
void effPCPThunderPolyListCreateSub2(effPCP_ThunderPoly_t *pThunderPoly) {
    dds3TrackPolyVC5Set_Normal(pThunderPoly->hPoly, pThunderPoly->head.color_center, pThunderPoly->head.color_border, pThunderPoly->head.color_edge);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1340, match, same function 1.00 (words 39/39, calls 1.00, shape 1.00) */
void effCallGetRandA(effPCP_ThunderPoly_t *work, s32 index) {
    effPCP_ThunderPolyPart_t *fragment = &work->pPart[index];

    fragment->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    fragment->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *system = pThunderPoly->hPoly;
    dds3TrackPolyList_t *cell = &system->trackList[number];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, pThunderPoly->head.width_border);
    VEC3_SPLAT(outerWidth, width[0] + pThunderPoly->head.width_edge);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->count = system->point;
    vertices = cell->pVertex;
    segments = pThunderPoly->head.split;
    SDF_VU0_LOADVECTOR(pThunderPoly->head.end);
    SDF_VU0_LOADVECTOR_B(pThunderPoly->head.start);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= pThunderPoly->head.split;
    amplitude = pThunderPoly->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, pThunderPoly->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_B_TO_A();
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = pThunderPoly->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData) {
    s32 i = 0;
    dds3TrackPoly_t *system = hData->hPoly;
    s32 count = hData->head.number;
    u32 restartLimit = SDF_FRAMES(hData->head.restartFrameLimit);
    u32 tintColor = hData->color;
    u32 updateCount = hData->state.updateCount;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    dds3TrackPolyList_t *renderCells = system->trackList;
    s32 canRestart;

    if (count > 0) {
        canRestart = updateCount < restartLimit;
        do {
            if (fragment->wait == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyPartRefresh(hData, i);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr += EFF_THUNDER_ALPHA_WRAP_ADD;
                } else if (restartLimit == 0 || canRestart) {
                    effPCP_ThunderPolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                } else {
                    renderCells[i].count = 0;
                }
            } else {
                fragment->wait -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            renderCells[i].color = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            i++;
            fragment++;
        } while (i < count);
        updateCount = hData->state.updateCount;
    }
    hData->state.updateCount = updateCount + 1;
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderPoly_t *effPCP_ThunderPolyLowCreate(effPCP_ThunderPolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderPolyPart_t) + sizeof(effPCP_ThunderPoly_t));
    effPCP_ThunderPoly_t *work = (effPCP_ThunderPoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderPolyPart_t *)(work + 1);
    work->allocationHandle = allocationHandle;
    work->state.secondarySystem = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_TRIANGLE);
    dds3TrackPolyVCSet(work->state.secondarySystem, 2, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->state.secondarySystem, work->head.blend);
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(work->hPoly, 2, work->head.color_center, work->head.color_center);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderPolyLowDrawOff(work, i);
    }
    work->color = EFF_THUNDER_NEUTRAL_COLOR;
    return work;
}

void effPCP_ThunderPolyLowDelete(effPCP_ThunderPoly_t *hData) {
    dds3TrackPolyDelete(hData->state.secondarySystem);
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1310, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCP_ThunderPolySetPosition(u8 *hData, void *pPos) {
        SDF_VU0_LOADVECTOR(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_LOADVECTOR_B(pPos);
        SDF_VU0_STOREVECTOR_B(hData + EFF_THUNDER_ENDPOINT_VECTOR_BYTES);
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_LOADVECTOR(hData);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(hData);
}

void effPCP_ThunderPolyLowSetColor(effPCP_ThunderPoly_t *hData, u32 color) {
    hData->color = color;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPThunder.c:1624, match, same function 1.00 (words 39/39, calls 1.00, shape 1.00) */
void effCallGetRandA2(effPCP_ThunderPoly_t *work, s32 index) {
    effPCP_ThunderPolyPart_t *fragment = &work->pPart[index];

    fragment->wait = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    fragment->refresh = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
    fragment->abgr = EFF_THUNDER_NEUTRAL_COLOR;
}

void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *secondarySystem = pThunderPoly->state.secondarySystem;
    dds3TrackPoly_t *primarySystem = pThunderPoly->hPoly;
    dds3TrackPolyList_t *secondaryCell = &secondarySystem->trackList[number];
    dds3TrackPolyList_t *primaryCell = &primarySystem->trackList[number];
    u128 *vertices;
    u128 *coreVertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, pThunderPoly->head.coreWidth);
    VEC3_SPLAT(outerWidth, (width[0] + pThunderPoly->head.width_edge) + pThunderPoly->head.width_border);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    secondaryCell->count = secondarySystem->point;
    primaryCell->count = primarySystem->point;
    vertices = secondaryCell->pVertex;
    coreVertices = primaryCell->pVertex;
    segments = pThunderPoly->head.split;
    SDF_VU0_LOADVECTOR(pThunderPoly->head.end);
    SDF_VU0_LOADVECTOR_B(pThunderPoly->head.start);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= pThunderPoly->head.split;
    amplitude = pThunderPoly->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, pThunderPoly->head.start);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(coreVertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(coreVertices + 1);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 2);
        coreVertices += 2;
        vertices += 3;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = pThunderPoly->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 3;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyLowDraw(effPCP_ThunderPoly_t *hData) {
    s32 index = 0;
    dds3TrackPoly_t *secondarySystem = hData->state.secondarySystem;
    s32 fragmentCount = hData->head.number;
    dds3TrackPoly_t *primarySystem = hData->hPoly;
    dds3TrackPolyList_t *primaryCells = primarySystem->trackList;
    u32 tintColor = hData->color;
    effPCP_ThunderPolyPart_t *fragment = hData->pPart;
    dds3TrackPolyList_t *secondaryCells = secondarySystem->trackList;
    s32 packedColor;

    if (fragmentCount > 0) {
        do {
            if (fragment->wait == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (fragment->refresh != 0) {
                    effPCP_ThunderPolyLowPartRefresh(hData, index);
                    fragment->refresh--;
                } else if (fragment->abgr & EFF_THUNDER_ALPHA_MASK) {
                    fragment->abgr -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderPolyLowDrawOff(hData, index);
                    dds3TrackPolyListInit(hData->state.secondarySystem, index);
                    dds3TrackPolyListInit(hData->hPoly, index);
                }
            } else {
                fragment->wait -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            packedColor = effMagaCylinderCalcColor(fragment->abgr, tintColor);
            secondaryCells[index].color = packedColor;
            primaryCells[index].color = packedColor;
            fragment++;
            index++;
        } while (index < fragmentCount);
    }
    dds3TrackPolyDraw(hData->state.secondarySystem);
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderRadiatePoly_t *effPCP_ThunderRadiatePolyCreate(effPCP_ThunderRadiatePolyParam_t *parameters) {
    sdfMemHandle_t *allocationHandle = sdfMemAllocDirect(parameters->number * sizeof(effPCP_ThunderRadiatePolyPart_t) + sizeof(effPCP_ThunderRadiatePoly_t));
    effPCP_ThunderRadiatePoly_t *work = (effPCP_ThunderRadiatePoly_t *)sdfMemLock(allocationHandle);
    u32 i;

    work->head = *parameters;
    work->pPart = (effPCP_ThunderRadiatePolyPart_t *)(work + 1);
    work->hMem = allocationHandle;
    work->hPoly = dds3TrackPolyCreate(work->head.number, work->head.split * 2 - 1, 0, PAR_CELL_TOPOLOGY_SIX_VECTOR);
    dds3TrackPolyVC3Set_Acute(work->hPoly, work->head.color_center, work->head.color_border, work->head.color_edge);
    dds3TrackPolySetMode(work->hPoly, work->head.blend);
    for (i = 0; i < work->head.number; i++) {
        effPCP_ThunderRadiatePolyDrawOff(work, i);
    }
    return work;
}

void effPCP_ThunderRadiatePolyDelete(effPCP_ThunderRadiatePoly_t *hData) {
    dds3TrackPolyDelete(hData->hPoly);
    sdfMemFreeDirect(hData->hMem);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", effPCP_ThunderRadiatePolySetPosition);

void effPCP_ThunderRadiatePolySetColor(effPCP_ThunderRadiatePoly_t *hData, u32 abgr) {
    hData->abgr = abgr;
}

void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index) {
    effPCP_ThunderRadiatePolyPart_t *cell = work->pPart + index;
    f32 centeredRandom;

    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionX = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionY = centeredRandom + centeredRandom;
    centeredRandom = dds3GetRandFloatA(dds3EffectBase) - EFF_THUNDER_RANDOM_MIDPOINT;
    cell->directionZ = centeredRandom + centeredRandom;
    cell->delayFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.startDelayRange;
    cell->activeFrames = dds3GetRandA((effRandState_t *)dds3EffectBase) % work->head.activeFrameRange + EFF_THUNDER_MIN_ACTIVE_FRAMES;
}

void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number) {
    f32 width[4] __attribute__((aligned(16)));
    f32 bandWidth[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *system = pThunderRadiatePoly->hPoly;
    effPCP_ThunderRadiatePolyPart_t *source = &pThunderRadiatePoly->pPart[number];
    dds3TrackPolyList_t *cell = &system->trackList[number];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 facing;
    f32 stepLength;

    VEC3_SPLAT(width, pThunderRadiatePoly->head.coreWidth);
    VEC3_SPLAT(bandWidth, width[0] + pThunderRadiatePoly->head.bandWidth);
    VEC3_SPLAT(outerWidth, bandWidth[0] + pThunderRadiatePoly->head.edgeWidth);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->count = system->point;
    vertices = cell->pVertex;
    segments = pThunderRadiatePoly->head.split;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    direction[0] = source->directionX;
    direction[1] = source->directionY;
    direction[2] = source->directionZ;
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    distance = pThunderRadiatePoly->head.length / pThunderRadiatePoly->head.split;
    SDF_VU0_COPYVECTOR(position, pThunderRadiatePoly->head.origin);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        stepLength = distance * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.9f);
        VEC3_SPLAT(stepScale, stepLength);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(bandWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 5);
        vertices += 6;
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (70.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_STOREVECTOR_NOMEM(direction);
        SDF_VU0_LOADVECTOR_B(basis);
        SDF_VU0_INNERPRODUCT(facing);
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 6;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderRadiatePolyDraw(effPCP_ThunderRadiatePoly_t *hData) {
    s32 i = 0;
    s32 cellCount = hData->head.number;
    effPCP_ThunderRadiatePolyPart_t *cell = hData->pPart;
    dds3TrackPolyList_t *renderCells = hData->hPoly->trackList;

    if (cellCount > 0) {
        do {
            if (cell->delayFrames == 0 && SDF_FRAME_DUE(dds3GetKernelCount())) {
                if (cell->activeFrames != 0) {
                    effPCP_ThunderRadiatePolyPartRefresh(hData, i);
                    cell->activeFrames--;
                } else if (renderCells[i].color & EFF_THUNDER_ALPHA_MASK) {
                    renderCells[i].color -= EFF_THUNDER_ALPHA_FADE_STEP;
                } else {
                    effPCP_ThunderRadiatePolyDrawOff(hData, i);
                    dds3TrackPolyListInit(hData->hPoly, i);
                }
            } else {
                cell->delayFrames -= SDF_FRAME_DUE(dds3GetKernelCount());
            }
            cell++;
            i++;
        } while (i < cellCount);
    }
    dds3TrackPolyDraw(hData->hPoly);
}

effPCP_ThunderPolyList_t *effPCP_ThunderPolyListCreate(effPCP_ThunderPolyListParam_t *src) {
    effPCP_ThunderPolyList_t *group = sdfChipAllocDirect(sizeof(effPCP_ThunderPolyList_t));
    s32 i;

    memset(group->handles, 0, sizeof(group->handles));
    group->head = *src;
    group->handles[0] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute2(group->handles[0]);
    for (i = 1; i < src->count - 2; i++) {
        group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
        effPCP_ThunderPolySetVCNormal(group->handles[i]);
    }
    group->handles[i] = effPCP_ThunderPolyCreate(&group->head.TPParam);
    effPCP_ThunderPolySetVCAcute(group->handles[i]);
    group->color = EFF_THUNDER_NEUTRAL_COLOR;
    return group;
}

void effPCP_ThunderPolyListDelete(effPCP_ThunderPolyList_t *hData) {
    s32 fragmentIndex;

    for (fragmentIndex = 0; fragmentIndex < hData->head.count - 1; fragmentIndex++) {
        effPCP_ThunderPolyDelete(hData->handles[fragmentIndex]);
    }
    sdfChipFreeDirect(hData);
}

void *effPCP_ThunderPolyListGetParam(void *pTPolyList) {
    return pTPolyList;
}

void effPCP_ThunderPolyListSetColor(effPCP_ThunderPolyList_t *hData, u32 color) {
    hData->color = color;
}

void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed) {
    f32 width[4] __attribute__((aligned(16)));
    f32 outerWidth[4] __attribute__((aligned(16)));
    f32 negativeOuterWidth[4] __attribute__((aligned(16)));
    f32 position[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 basis[4] __attribute__((aligned(16)));
    f32 direction[4] __attribute__((aligned(16)));
    f32 side[4] __attribute__((aligned(16)));
    f32 cameraDirection[4] __attribute__((aligned(16)));
    f32 waveScale[4] __attribute__((aligned(16)));
    f32 stepScale[4] __attribute__((aligned(16)));
    f32 previousSide[4] __attribute__((aligned(16)));
    f32 delta[4] __attribute__((aligned(16)));
    dds3TrackPoly_t *system = work->hPoly;
    dds3TrackPolyList_t *cell = &system->trackList[index];
    u128 *vertices;
    u128 *join;
    s32 i;
    s32 segments;
    f32 distance;
    f32 amplitude;
    f32 phase;
    f32 cycle;
    f32 currentWave;
    f32 previousWave;
    f32 phaseStep;
    f32 facing;

    VEC3_SPLAT(width, work->head.width_border);
    VEC3_SPLAT(outerWidth, width[0] + work->head.width_edge);
    VEC3_SPLAT(negativeOuterWidth, -outerWidth[0]);
    cell->count = system->point;
    vertices = cell->pVertex;
    segments = work->head.split;
    SDF_VU0_LOADVECTOR(work->head.end);
    SDF_VU0_LOADVECTOR_B(seed + 3);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_GETLENGTH(distance);
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(direction);
    distance /= work->head.split;
    amplitude = work->head.waveAmplitude;
    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(cameraDirection);
    SDF_VU0_LOADVECTOR_B(direction);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(side);
    phase = dds3GetRandFloatA(dds3EffectBase) * (EFF_THUNDER_HALF_TURN * 2.0f);
    cycle = phase - (EFF_THUNDER_HALF_TURN * 2.0f);
    SDF_VU0_COPYVECTOR(position, seed + 3);
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave);
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(position);
    VEC3_SPLAT(stepScale, distance);
    previousWave = currentWave;
    currentWave = amplitude * sdfSin(phase);
    VEC3_SPLAT(waveScale, currentWave - previousWave);
    SDF_VU0_LOADVECTOR(direction);
    SDF_VU0_LOADVECTOR_B(stepScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(side);
    SDF_VU0_LOADVECTOR_B(waveScale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR_B(position);
    SDF_VU0_ADDVECTOR_B_A();
    SDF_VU0_STOREVECTOR_B_NOMEM(next);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR_NOMEM(basis);

    for (i = 0; i < segments; i++) {
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        if (i > 0) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
            SDF_VU0_COPYVECTOR_B_TO_A();
            SDF_VU0_LOADVECTOR_B(width);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
            SDF_VU0_LOADVECTOR(basis);
            SDF_VU0_LOADVECTOR_B(outerWidth);
            SDF_VU0_MULVECTOR();
            SDF_VU0_COPYVECTOR_A_TO_C();
            SDF_VU0_LOADVECTOR(position);
            SDF_VU0_COPYVECTOR_A_TO_B();
            SDF_VU0_ADDVECTOR_A_C();
            SDF_VU0_STOREVECTOR_NOMEM(vertices);
            SDF_VU0_COPYVECTOR_C_TO_A();
            SDF_VU0_SUBVECTOR_B_A();
            SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
            vertices += 5;
        } else {
            SDF_VU0_COPYVECTOR(vertices, seed);
            SDF_VU0_COPYVECTOR(vertices + 1, seed + 1);
            SDF_VU0_COPYVECTOR(vertices + 2, seed + 2);
            SDF_VU0_COPYVECTOR(vertices + 3, seed + 3);
            SDF_VU0_COPYVECTOR(vertices + 4, seed + 4);
            vertices += 5;
        }
        SDF_VU0_COPYVECTOR(previousSide, basis);
        SDF_VU0_COPYVECTOR(position, next);
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 2);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(width);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices + 1);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 3);
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(outerWidth);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(vertices);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_SUBVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(vertices + 4);
        vertices += 5;
        phaseStep = (EFF_THUNDER_HALF_TURN / segments) * 4.0f;
        cycle += phaseStep;
        phase += phaseStep;
        if (cycle > EFF_THUNDER_HALF_TURN) {
            cycle -= EFF_THUNDER_HALF_TURN;
            amplitude = work->head.waveAmplitude * (1.0f - dds3GetRandFloatA(dds3EffectBase) * 0.95f);
        }
        previousWave = currentWave;
        currentWave = amplitude * sdfSin(phase);
        VEC3_SPLAT(waveScale, currentWave - previousWave);
        SDF_VU0_LOADVECTOR(direction);
        SDF_VU0_LOADVECTOR_B(stepScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR(side);
        SDF_VU0_LOADVECTOR_B(waveScale);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR_NOMEM(delta);
        SetRotateMatrixAxis(((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f) * (20.0f * (EFF_THUNDER_HALF_TURN / 180.0f)), (const struct sdfVector3_t *)cameraDirection);
        SDF_VU0_LOADVECTOR(delta);
        SDF_VU0_APPLYMATRIX_XYZ();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_ADDVECTOR_B_A();
        SDF_VU0_STOREVECTOR_B_NOMEM(next);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_NORMALIZE();
        SDF_VU0_STOREVECTOR_NOMEM(basis);
        SDF_VU0_LOADVECTOR(next);
        SDF_VU0_LOADVECTOR_B(position);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_LOADVECTOR_B(previousSide);
        SDF_VU0_INNERPRODUCT(facing);
        /* Choose the join edge from the new direction relative to the prior side. */
        if (facing < 0.0f) {
            join = vertices - 1;
            SDF_VU0_LOADVECTOR_C(outerWidth);
        } else {
            join = vertices - 5;
            SDF_VU0_LOADVECTOR_C(negativeOuterWidth);
        }
        SDF_VU0_LOADVECTOR(basis);
        SDF_VU0_LOADVECTOR_B(cameraDirection);
        SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(join);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR_NOMEM(position);
    }
}

void effPCP_ThunderPolyListDraw(effPCP_ThunderPolyList_t *hData) {
    s32 i = 0;
    s32 segmentCount = hData->head.count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = hData->head.points;
    effPCP_ThunderPoly_t *work;
    effPCP_ThunderPoly_t *previous;
    dds3TrackPolyList_t *cell;

    for (; i < segmentCount; i++) {
        work = hData->handles[i];
        SDF_VU0_COPYVECTOR(work->head.start, points[0]);
        SDF_VU0_COPYVECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.number;
        color = effMagaCylinderCalcColor(hData->color, work->color);
        cell = work->hPoly->trackList;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = hData->handles[i - 1];
                SDF_FRAME_WHEN(dds3GetKernelCount(), effPCP_ThunderPolyListPartRefresh(work, j,
                    previous->hPoly->trackList[j].pVertex + previous->hPoly->trackList[j].count - 5));
            } else {
                SDF_FRAME_WHEN(dds3GetKernelCount(), effPCP_ThunderPolyPartRefresh(work, j));
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        dds3TrackPolyDraw(work->hPoly);
    }
}
