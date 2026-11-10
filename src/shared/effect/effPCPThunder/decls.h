/* Declarations of smt3-decomp src/smt3/effect/effPCPThunder.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPCPTHUNDER_H
#define SHARED_EFFECT_EFFPCPTHUNDER_H

#include "common.h"

#include "effPCPData.h"

#include "effThunder.h"

#include "ps2asm.h"

#include "sdf_resource.h"

#include "sdf_types.h"

#include "sdfFrame.h"

struct effRandState_t;

struct effPCPThunderSpherePolyParam_t;

struct effPCP_ThunderRadiatePolyPart_t;

struct effPCP_ThunderRadiatePolyParam_t;

struct effPCP_ThunderRadiatePoly_t;

struct effPCP_ThunderPolyPart_t;

struct effPCP_ThunderPolyParam_t;

struct effPCP_ThunderPolyList_t;

struct effPCP_ThunderPolyListParam_t;

struct effPCP_ThunderColumnPolyLowPart_t;

struct effPCP_ThunderColumnPolyLowParam_t;

struct effPCP_ThunderColumnPolyLow_t;

struct effPCPThunderSpherePolyPart_t;

struct effPCPThunderSpherePoly_t;

struct parCell_t;

struct sdfVector3_t;

struct sdfMatPacket_t;

struct sdfGsMemSlice_t;

typedef struct effRandState_t effRandState_t;

typedef struct effPCP_ThunderRadiatePolyPart_t effPCP_ThunderRadiatePolyPart_t;

typedef struct effPCP_ThunderRadiatePolyParam_t effPCP_ThunderRadiatePolyParam_t;

typedef struct parCell_t parCell_t;

typedef struct sdfMatPacket_t sdfMatPacket_t;

typedef union sdfMatUvwParm_t sdfMatUvwParm_t;

typedef struct sdfTexPacket_t sdfTexPacket_t;

typedef struct parSystem_t parSystem_t;

typedef struct effPCP_ThunderRadiatePoly_t effPCP_ThunderRadiatePoly_t;

typedef struct effPCP_ThunderPolyPart_t effPCP_ThunderPolyPart_t;

typedef struct effPCP_ThunderPolyParam_t effPCP_ThunderPolyParam_t;

typedef struct effPCP_ThunderPolyListParam_t effPCP_ThunderPolyListParam_t;

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

/* Mid-grey fragment colour (0x80 in each channel). */
/* Four ordered 32-bit words in the shared effect random-number state. */
struct effRandState_t {
    u32 words[4];
};

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

struct parSystem_t;

struct parCell_t {
    u128 *history;   /* 0x00 */
    u32 *colors;     /* 0x04: packed per-history-vector colors */
    s32 vertexCount; /* 0x08: active vectors in the topology-specific history */
    s32 historyAdvanceCountdown; /* 0x0C: updates until the next history shift */
    u32 color;       /* 0x10 initialized to EFF_THUNDER_NEUTRAL_COLOR */
};

struct sdfMat_t;

/* A texture coordinate transform in a material packet. sdfMatSetUvwParm fills
 * it either as six floats or, for the identity case, as three quadword halves. */
typedef union {
    f32 m[6];
    u64 q[3];
} sdfMatTexTm_t;

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
struct sdfMatPacket_t {
    u32 pad00;             /* 0x00 */
    u32 ambient;             /* 0x04 */
    u32 diffuse;             /* 0x08 */
    u32 multiTexDiffuse;    /* 0x0C: packed color applied by the secondary draw state */
    u32 specular; /* 0x10: copied from the owning asset, or forced to zero. */
    u32 refmapColor;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 constant;             /* 0x1C */
    u32 alphaState;        /* 0x20: packed GS ALPHA_2 state emitted by the VU transfer builder */
    u32 multiTexMethod;     /* 0x24: secondary packet mode copied from SdfAsset */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    u64 tex1Data;          /* 0x38 */
    u64 tex0Data;
    u64 clampData;
    u64 mtexTex1Data;      /* 0x50 */
    u64 mtexTex0Data;
    u64 mtexClampData;
    /* sdfMatSetUvwParm writes one transform per texture: a 2x2 then an offset. */
    sdfMatTexTm_t texTm;   /* 0x68 */
    sdfMatTexTm_t mtexTexTm;
    u32 vifItopCommand;    /* 0x98: VIF ITOP with immediate 0xC */
    u32 vifMscalCommand;   /* 0x9C: VIF MSCAL with microprogram index zero */
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

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

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

/* Header followed by a variable-length effPCP_ThunderRadiatePolyPart_t array in one allocation. */
struct effPCP_ThunderRadiatePoly_t {
    effPCP_ThunderRadiatePolyParam_t head;
    effPCP_ThunderRadiatePolyPart_t *pPart; /* 0x48 */
    u32 abgr; /* 0x4C: settable, otherwise unobserved */
    parSystem_t *hPoly; /* 0x50 */
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

struct effPCP_ThunderPoly_t {
    effPCP_ThunderPolyParam_t head;
    effPCP_ThunderPolyPart_t *pPart; /* 0x54 */
    u32 color;               /* 0x58 */
    union {
        u32 updateCount;     /* single-hPoly variant */
        parSystem_t *secondarySystem; /* dual-hPoly variant */
    } state;                 /* 0x5C */
    parSystem_t *hPoly;            /* 0x60 */
    sdfMemHandle_t *allocationHandle; /* 0x64 */
}; /* 0x68 */

typedef struct effPCP_ThunderPolyList_t effPCP_ThunderPolyList_t;

/* Shared paired-chain parameter and resource owners. Points are filled before rendering. */
struct effPCP_ThunderPolyListParam_t {
    f32 points[10][4];
    s32 count;
    effPCP_ThunderPolyParam_t TPParam;
};

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
    parSystem_t *hPoly;    /* 0x1C */
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
    struct parSystem_t *hPoly;
    struct sdfMemHandle_t *allocationHandle;
};

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

extern effRandState_t dds3EffectBase[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);

extern s32 effMagaCylinderCalcColor(s32 color, s32 param);

extern void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number);

extern void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

extern void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

extern void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed);

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData);

void dds3TrackPolyListInit(struct parSystem_t *hTrackPoly, s32 number);

void dds3TrackPolyDraw(struct parSystem_t *system);

void dds3TrackPolyDelete(struct parSystem_t *hTrackPoly);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

extern f32 aViewTarget[4], aViewEye[4];

extern void SetRotateMatrixXYZ(const struct sdfVector3_t *);

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

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void *sdfChipAllocDirect(u32 aSize);

extern void *memset(void *a, s32 b, size_t c);

extern parSystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);

extern void dds3TrackPolySetMode(parSystem_t *hTrackPoly, u16 mode);

extern void dds3TrackPolyVCSet(parSystem_t *hTrackPoly, s32 type, u32 abgr_in, u32 abgr_out);

extern void dds3TrackPolyVC3Set_Acute(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

extern void dds3TrackPolyVC5Set_Acute(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

extern void dds3TrackPolyVC5Set_AcuteR(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

extern void dds3TrackPolyVC5Set_AcuteW(parSystem_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

extern void dds3TrackPolyVC5Set_Normal(parSystem_t *hTrackPoly, s32 abgr_0, s32 abgr_1, s32 abgr_2);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern u32 dds3GetKernelCount(void);

void func_00177AC8(effPCP_ThunderColumnPolyLow_t *work, s32 index);

void effPCP_ThunderPolyDrawOff(effPCP_ThunderPoly_t *work, s32 index);

void effPCP_ThunderPolyLowDrawOff(effPCP_ThunderPoly_t *work, s32 index);

void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index);

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyCreate(effPCPThunderSpherePolyParam_t *source);

void *effPCPThunderSpherePolyCreatePCF(void *parameterTable);

void *effPCPThunderSpherePolyCopy(void *source);

void effPCPThunderSpherePolyDelete(effPCPThunderSpherePoly_t *hData);

void effPCPThunderSpherePolySetPosition(void *hData, void *pPos);

void effPCPThunderSpherePolySetColor(effPCPThunderSpherePoly_t *hData, u32 abgr);

void effPCPThunderSpherePolySetScale(effPCPThunderSpherePoly_t *hData, f32 scale);

effPCPThunderSpherePolyParam_t *effPCPThunderSpherePolyGetParam(effPCPThunderSpherePoly_t *pThunderPoly);

void effPCPThunderSphereInit(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSphereRotate(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSphereRotateAxis(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSpherePolyDraw(effPCPThunderSpherePoly_t *hData);

effPCPThunderSpherePoly_t *effBTLBindCreateTrack(effPCPThunderSpherePolyParam_t *parameters);

void effPCPThunderSpherePolyLowDelete(effPCPThunderSpherePoly_t *hData);

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyLowCreate(void *pParam);

effPCPThunderSpherePoly_t *effPCPThunderSpherePolyLowCopy(void *parameters);

void effPCPThunderSpherePolyLowSetPosition(void *hData, void *pPos);

void effPCPThunderSpherePolyLowSetColor(effPCPThunderSpherePoly_t *hData, u32 abgr);

void effPCPThunderSpherePolyLowSetScale(effPCPThunderSpherePoly_t *work, f32 scale);

u32 effPCPThunderSpherePolyLowGetParam(u32 pThunderPoly);

void effPCPThunderSphereLowInit(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSphereLowRotate(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSphereLowRotateAxis(effPCPThunderSpherePoly_t *work, s32 index);

void effPCPThunderSpherePolyLowDraw(effPCPThunderSpherePoly_t *hData);

effPCP_ThunderColumnPolyLow_t *eff_177850(effPCP_ThunderColumnPolyLowParam_t *parameters);

void eff_177A48(effPCP_ThunderColumnPolyLow_t *work);

void func_00177AB0(void *destination, void *source);

void func_00177AC0(effPCP_ThunderColumnPolyLow_t *work, u32 tintColor);

void func_00177AC8(effPCP_ThunderColumnPolyLow_t *work, s32 index);

void func_00177BC0(effPCP_ThunderColumnPolyLow_t *work, s32 index);

void eff_178128(effPCP_ThunderColumnPolyLow_t *work);

effPCP_ThunderPoly_t *effPCP_ThunderPolyCreate(effPCP_ThunderPolyParam_t *parameters);

void effPCP_ThunderPolyDelete(effPCP_ThunderPoly_t *hData);

void effPCP_ThunderPolySetPosition(u8 *hData, void *pPos);

void effPCP_ThunderPolySetColor(effPCP_ThunderPoly_t *hData, u32 abgr);

effPCP_ThunderPolyParam_t *effPCP_ThunderPolyGetParam(effPCP_ThunderPoly_t *pThunderPoly);

void effPCP_ThunderPolySetVCAcute(effPCP_ThunderPoly_t *pThunderPoly);

void effPCP_ThunderPolySetVCAcute2(effPCP_ThunderPoly_t *pThunderPoly);

void effPCP_ThunderPolySetVCNormal(effPCP_ThunderPoly_t *pThunderPoly);

void effPCP_ThunderPolyDrawOff(effPCP_ThunderPoly_t *work, s32 index);

void effPCP_ThunderPolyPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

void effPCP_ThunderPolyDraw(effPCP_ThunderPoly_t *hData);

effPCP_ThunderPoly_t *effPCP_ThunderPolyLowCreate(effPCP_ThunderPolyParam_t *parameters);

void effPCP_ThunderPolyLowDelete(effPCP_ThunderPoly_t *hData);

void effPCP_ThunderPolyLowSetPosition(u8 *hData, void *pPos);

void effPCP_ThunderPolyLowSetColor(effPCP_ThunderPoly_t *hData, u32 abgr);

void effPCP_ThunderPolyLowDrawOff(effPCP_ThunderPoly_t *work, s32 index);

void effPCP_ThunderPolyLowPartRefresh(effPCP_ThunderPoly_t *pThunderPoly, s32 number);

void effPCP_ThunderPolyLowDraw(effPCP_ThunderPoly_t *hData);

effPCP_ThunderRadiatePoly_t *effPCP_ThunderRadiatePolyCreate(effPCP_ThunderRadiatePolyParam_t *parameters);

void effPCP_ThunderRadiatePolyDelete(effPCP_ThunderRadiatePoly_t *hData);

void effPCP_ThunderRadiatePolySetPosition(void *hData, void *pPos);

void effPCP_ThunderRadiatePolySetColor(effPCP_ThunderRadiatePoly_t *hData, u32 abgr);

void effPCP_ThunderRadiatePolyDrawOff(effPCP_ThunderRadiatePoly_t *work, s32 index);

void effPCP_ThunderRadiatePolyPartRefresh(effPCP_ThunderRadiatePoly_t *pThunderRadiatePoly, s32 number);

void effPCP_ThunderRadiatePolyDraw(effPCP_ThunderRadiatePoly_t *hData);

effPCP_ThunderPolyList_t *effPCP_ThunderPolyListCreate(effPCP_ThunderPolyListParam_t *src);

void effPCP_ThunderPolyListDelete(effPCP_ThunderPolyList_t *hData);

effPCP_ThunderPolyListParam_t *effPCP_ThunderPolyListGetParam(effPCP_ThunderPolyList_t *pTPolyList);

void effPCP_ThunderPolyListSetColor(effPCP_ThunderPolyList_t *hData, u32 abgr);

void effPCP_ThunderPolyListPartRefresh(effPCP_ThunderPoly_t *work, s32 index, const u128 *seed);

void effPCP_ThunderPolyListDraw(effPCP_ThunderPolyList_t *hData);

#endif
