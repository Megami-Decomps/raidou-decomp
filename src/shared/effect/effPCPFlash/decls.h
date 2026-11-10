/* Declarations of smt3-decomp src/smt3/effect/effPCPFlash.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPCPFLASH_H
#define SHARED_EFFECT_EFFPCPFLASH_H

#include "common.h"

#include "effPCPData.h"

#include "ps2asm.h"

#include "sdf_resource.h"

#include "sdf_types.h"

#include "sdfFrame.h"

struct effDebugPolyParams_t;

struct effRandState_t;

struct dds3FlashPolyList_t;

struct effPCPFlashPolyRingExpHPart_tag;

struct effPCPFlashPolyRingExpH_tag;

struct effBezierDrawParam_t;

struct effPCPFlashPolyPipePart_tag;

struct effPCPFlashPolyPipe_tag;

struct effPCPFlashPolyRingExp2_tag;

struct effPCPFlashPolyRingExpPart_tag;

struct effPCPFlashPolyCircleParam_t;

struct effPCPFlashPolyCircle_tag;

struct effPCPFlashPolyRingExp_tag;

struct effPCPFlashPolyRingExp2Part_tag;

struct effPCPFlashPolySphere2Part_tag;

struct effPCPFlashPolySphere3Part_tag;

struct effPCPFlashPolySphere3_tag;

struct effPCPFlashPolySphere2ExpParam_t;

struct effPCPFlashPolySphere2Exp_tag;

struct effPCPFlashPolySpherePart_tag;

struct effPCPFlashPolySphereExpPart_tag;

struct effPCPFlashPolySphereExp_tag;

struct effPCPFlashPolyRing_tag;

struct effPCPFlashPolySphereParam_t;

struct effPCPFlashPolySphere_tag;

struct effPCPFlashPolySphere2Param_t;

struct effPCPFlashPolySphere2_tag;

struct sdfVector3_t;

struct sdfMatPacket_t;

struct sdfGsMemSlice_t;

typedef struct effRandState_t effRandState_t;

typedef struct sdfMatPacket_t sdfMatPacket_t;

typedef union sdfMatUvwParm_t sdfMatUvwParm_t;

typedef struct sdfTexPacket_t sdfTexPacket_t;

typedef struct effPCPFlashPolyRingExpHPart_tag effPCPFlashPolyRingExpHPart_tag;

typedef struct effPCPFlashPolyRingExpH_tag effPCPFlashPolyRingExpH_tag;

typedef struct effPCPFlashPolyCircleParam_t effPCPFlashPolyCircleParam_t;

typedef struct effPCPFlashPolySphere2Part_tag effPCPFlashPolySphere2Part_tag;

typedef struct effPCPFlashPolySphere2ExpParam_t effPCPFlashPolySphere2ExpParam_t;

typedef struct effPCPFlashPolySphereParam_t effPCPFlashPolySphereParam_t;

typedef struct effPCPFlashPolySphere2Param_t effPCPFlashPolySphere2Param_t;

typedef struct effPCPFlashPolySphere2_tag effPCPFlashPolySphere2_tag;

typedef struct sdfGrOt_t sdfGrOt_t;

typedef struct effBezierDrawParam_t effBezierDrawParam_t;

typedef struct effDebugPolyParams_t effDebugPolyParams_t;

#define EFF_POOL_HEADER_BYTES 0x70

#define EFF_PACKET_PARAMS_BYTES 0x2C

#define EFF_PACKET_LIST_BYTES 0x20

#define EFF_STATE_PACKET_BYTES 0x30

#define EFF_MATRIX_WORD_COUNT 16

#define EFF_FAN_VERTEX_COUNT 5

#define EFF_FAN_BATCH_VERTICES 0x19

#define EFF_FAN_BATCH_PARAMS 0xF

#define EFF_TRIANGLE_VERTEX_COUNT 3

#define EFF_TRIANGLE_BATCH_VERTICES 0x30

#define EFF_QUAD_VERTEX_COUNT 4

#define EFF_QUAD_BATCH_VERTICES 0x20

#define EFF_TRIANGLE_POSITION_BYTES 0x30

#define EFF_TRIANGLE_COLOR_BYTES 0xc

#define EFF_QUAD_POSITION_BYTES 0x40

#define EFF_QUAD_COLOR_BYTES 0x10

#define EFF_FAN_POSITION_BYTES 0x50

#define EFF_FAN_COLOR_BYTES 0x14

#define EFF_NEUTRAL_COLOR 0x80808080

#define EFF_RGB_MASK 0xFFFFFF

#define EFF_DIRECT_SURFACE_COUNT 4

/* Four ordered 32-bit words in the shared effect random-number state. */
struct effRandState_t {
    u32 words[4];
};

typedef struct dds3FlashPolyList_t dds3FlashPolyList_t;

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

/* Position/color arrays precede this render pool; constructors clear the full allocation. */
typedef struct dds3FlashPolyList_t {
    f32 matrix[16];
    f32 pos[3];
    u8 pad4C[4];
    u32 blend; /* 0x50: packet submission surface */
    u32 abgr;
    s32 point; /* Positions/abgr words, not group count. */
    f32 scale;
    u8 *pVertex;
    u8 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3FlashPolyList_t; /* 0x70 */

struct effPCPFlashPolyRingExpHPart_tag {
    u32 abgr;
    s32 frame;
    f32 rotate;
    f32 spread;
};

/* The ring allocator copies 0x58 bytes of parameters into this 0x80-byte
 * owner. Its update passes this same work to the phase, color and fan helpers. */
struct effPCPFlashPolyRingExpH_tag {
    f32 origin[4];
    u32 count;
    u8 respawn;
    u8 pad15[3];
    s32 duration;
    s32 spread;
    u32 fadeIn;
    u32 fadeOut;
    u32 firstColor;
    u32 secondColor;
    f32 param30;
    f32 param34;
    f32 param38;
    f32 param3C;
    f32 param40;
    f32 param44;
    f32 param48;
    f32 param4C;
    f32 increment;
    u32 drawMode;
    effPCPFlashPolyRingExpHPart_tag *pPart;
    u32 frame;
    u32 abgr;
    f32 scale;
    f32 radius;
    f32 length;
    f32 currentSideOffsetB;
    f32 currentSideOffsetA;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

typedef struct effPCPFlashPolyPipePart_tag effPCPFlashPolyPipePart_tag;

struct effPCPFlashPolyPipePart_tag {
    u32 abgr;
    s32 frame;
    f32 length;
    f32 width_in;
    f32 width_out;
    f32 up;
    f32 rotate;
};

typedef struct effPCPFlashPolyPipe_tag effPCPFlashPolyPipe_tag;

struct effPCPFlashPolyPipe_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 spanBase;
    f32 unk34;
    f32 orbitRadius;
    f32 radialHalfThicknessBase;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 drawMode;
    effPCPFlashPolyPipePart_tag *parts;
    u32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

typedef struct effPCPFlashPolyRingExp2_tag effPCPFlashPolyRingExp2_tag;

typedef struct effPCPFlashPolyRingExp2Part_tag effPCPFlashPolyRingExp2Part_tag;

struct effPCPFlashPolyRingExp2Part_tag {
    u32 abgr;
    s32 frame;
    f32 length;
    f32 length_max;
    f32 rotate;
};

struct effPCPFlashPolyRingExp2_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
    effPCPFlashPolyRingExp2Part_tag *pPart;
    u32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

/* Shared 0x10-byte motion state: accumulator is an angle for orbit arcs and
   a radius for radial triangles; stepSpeed is radial speed or arc height. */
typedef struct effPCPFlashPolyRingExpPart_tag effPCPFlashPolyRingExpPart_tag;

struct effPCPFlashPolyRingExpPart_tag {
    u32 color;
    s32 frame;
    f32 accumulator;
    f32 stepSpeed; /* 0x0C: multiplied by decay each step, then added to
                       * accumulator; also read as the particle's height */
};

struct effPCPFlashPolyCircleParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 drawMode;
};

typedef struct effPCPFlashPolyCircle_tag effPCPFlashPolyCircle_tag;

struct effPCPFlashPolyCircle_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 drawMode;
    effPCPFlashPolyRingExpPart_tag *parts;
    s32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

typedef struct effPCPFlashPolyRingExp_tag effPCPFlashPolyRingExp_tag;

/* The orbit constructor and renderer share this 0x80-byte work record.
   The 0x58-byte copied parameters are followed by the motion-particle array
   pointer and draw/resource state. */
struct effPCPFlashPolyRingExp_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpanStart;
    f32 acrossSpanStart;
    f32 orbitRadiusStart;
    f32 orbitRadiusEnd;
    f32 normalSpanStart;
    f32 normalSpanEnd;
    f32 heightStart;
    f32 heightEnd;
    f32 angularStep;
    u32 drawMode;
    effPCPFlashPolyRingExpPart_tag *pPart;
    u32 frame;
    u32 abgr;
    f32 scale;
    f32 radius;
    f32 length;
    f32 width_in;
    f32 width_out;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

struct effPCPFlashPolySphere2Part_tag {
    u32 abgr;
    s32 frame;
    f32 radius;
    u8 radius_max[4];
};

typedef struct effPCPFlashPolySphere3Part_tag effPCPFlashPolySphere3Part_tag;

struct effPCPFlashPolySphere3Part_tag {
    u32 abgr;
    s32 frame;
    f32 rotate;
    f32 radius;
    f32 move;
    f32 move_s;
    f32 look_r;
    f32 width;
};

typedef struct effPCPFlashPolySphere3_tag effPCPFlashPolySphere3_tag;

struct effPCPFlashPolySphere3_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 spanBase;
    f32 spanVariation;
    f32 maxThickness;
    f32 angularStepRange;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    u8 pad50[0x80];
    effPCPFlashPolySphere3Part_tag *parts;
    u32 updateCount;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

struct effPCPFlashPolySphere2ExpParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
};

typedef struct effPCPFlashPolySphere2Exp_tag effPCPFlashPolySphere2Exp_tag;

struct effPCPFlashPolySphere2Exp_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    effPCPFlashPolyRingExpPart_tag *parts;
    s32 updateCount;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

typedef struct effPCPFlashPolySpherePart_tag effPCPFlashPolySpherePart_tag;

struct effPCPFlashPolySpherePart_tag {
    u32 abgr;
    s32 frame;
    f32 rotate;
    f32 radius;
    f32 position[3];
    f32 unk1C;
    f32 width_in;
    f32 width_out;
    f32 radius_max;
};

typedef struct effPCPFlashPolySphereExpPart_tag effPCPFlashPolySphereExpPart_tag;

struct effPCPFlashPolySphereExpPart_tag {
    u32 abgr;
    s32 frame;
    f32 rotate;
    f32 radius;
    f32 look_r;
    f32 width_in;
    f32 width_out;
    f32 radius_max;
};

typedef struct effPCPFlashPolySphereExp_tag effPCPFlashPolySphereExp_tag;

struct effPCPFlashPolySphereExp_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u8 pad24[0x04];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularStepRange;
    u32 drawMode;
    effPCPFlashPolySphereExpPart_tag *parts;
    u32 updateCount;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

typedef struct effPCPFlashPolyRing_tag effPCPFlashPolyRing_tag;

struct effPCPFlashPolyRing_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
    effPCPFlashPolyRingExp2Part_tag *parts;
    u32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

/* The rotating-streak factory copies this 0x40-byte parameter prefix into
 * its larger work allocation before appending particle and runtime state. */
struct effPCPFlashPolySphereParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 interval;
    u32 abgr_in;
    u32 abgr_out;
    f32 width_in;
    f32 width_out;
    f32 radius;
    f32 rotate;
    u32 blend;
};

typedef struct effPCPFlashPolySphere_tag effPCPFlashPolySphere_tag;

/* Spawn, rotation and draw passes share this 0x58-byte streak work.
   The position-rotation routine uses the same parts pointer at 0x40. */
struct effPCPFlashPolySphere_tag {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 rotationStepRange;
    u32 drawMode;
    effPCPFlashPolySpherePart_tag *pPart;
    s32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

/* The factory copies this 0x30-byte parameter prefix into its larger work
 * allocation before appending particle and resource state. */
struct effPCPFlashPolySphere2Param_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 loop;
    u8 pad15[3];
    s32 life;
    s32 scale_f;
    u32 abgr_in;
    u32 abgr_out;
    f32 radius;
    u32 blend;
};

/* effPCPFlashPolySphere2Create copies the 0x30-byte parameter prefix, then places
 * these 0x10-byte particles immediately after the 0x48-byte work. */
struct effPCPFlashPolySphere2_tag {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 colorA;
    u32 colorB;
    f32 maxScale;
    u32 drawMode;
    effPCPFlashPolySphere2Part_tag *pPart;
    s32 frame;
    u32 abgr;
    f32 scale;
    sdfMemHandle_t *allocationHandle;
    dds3FlashPolyList_t *hPoly;
};

struct sdfDrawTag_t;

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

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* Vertex-draw descriptor used by the thunder history and end-cap packets.
 * Both title providers clear the first 0x2C bytes and keep these globals in
 * 0x30-byte slots; the final four bytes remain opaque. */
struct effBezierDrawParam_t {
    s16 primitiveCount; /* 0x00 */
    s16 pointCount;     /* 0x02 */
    u16 flags;          /* 0x04 */
    u8 opaque_06[2];
    u32 color;          /* 0x08 */
    const u32 *primitiveIndices; /* 0x0C */
    u128 *points;       /* 0x10 */
    u8 opaque_14[0xC];
    u32 *colors;        /* 0x20 */
    u8 opaque_24[0xC];
};

extern void *effPkt3DPrimMakePoly(effBezierDrawParam_t *);

extern effRandState_t dds3EffectBase[];

extern u32 effPCPFlashFanIndices[];

extern f32 effPCPFlashPolyPipeVertex[];

extern f32 effPCPFlashPolyRingExpVertex[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);

extern u32 effMagaCylinderCalcColor(u32 color, u32 colorB);

extern sdfGrOt_t *effPCPFlashQuadSurfaces[];

extern u32 effPCPFlashQuadIndices[];

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

extern effDebugPolyParams_t effPCPFlashPolyParams[];

extern f32 effPCPFlashPolyRingExpHVertex[];

extern sdfGrOt_t *effPCPFlashFanSurfaces[];

extern sdfGrOt_t *effPCPFlashFanHSurfaces[];

extern sdfGrOt_t *effPCPFlashTriangleSurfaces[];

extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *, s32, size_t);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* Load axis-angle rotation into VU0 vf28-vf31. */
void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

struct sdfMat_t *sdfMatNew(void);

f32 sdfCos(f32 aRadius);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);

f32 sdfSin(f32 aRadius);

extern sdfGrOt_t sdfGrOtTable[85];

#define effFlashOtNode ((sdfGrOt_t *)&sdfGrOtTable[45])

/* Parameter prefixes retained by the flash particle constructors. */
typedef struct effPCPFlashPolyRingParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scale_f;
    u32 interval;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 drawMode;
} effPCPFlashPolyRingParam_t;

typedef struct effPCPFlashPolyPipeParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 spanBase;
    f32 width_out;
    f32 orbitRadius;
    f32 radialHalfThicknessBase;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 drawMode;
} effPCPFlashPolyPipeParam_t;

typedef struct effPCPFlashPolyRingExpParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 interval;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpanStart;
    f32 acrossSpanStart;
    f32 orbitRadiusStart;
    f32 orbitRadiusEnd;
    f32 normalSpanStart;
    f32 normalSpanEnd;
    f32 heightStart;
    f32 heightEnd;
    f32 angularStep;
    u32 drawMode;
} effPCPFlashPolyRingExpParam_t;

typedef struct effPCPFlashPolyRingExp2Param_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 number;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 blend;
} effPCPFlashPolyRingExp2Param_t;

typedef struct effPCPFlashPolySphereExpParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u8 pad24[4];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularStepRange;
    u32 drawMode;
} effPCPFlashPolySphereExpParam_t;

typedef struct effFlashRadialStripParam_t {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 centerColor;
    u32 edgeColor;
    f32 spanBase;
    f32 spanVariation;
    f32 maxThickness;
    f32 angularStepRange;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 drawMode;
    u8 pad50[0x80];
} effFlashRadialStripParam_t;

typedef struct effRingBlock_t {
    effPCPFlashPolyRingExpH_tag header;
    effPCPFlashPolyRingExpHPart_tag vertices[1];
} effRingBlock_t;

#define EFF_RING_PARAM_BYTES 0x58

#define EFF_RING_HEADER_BYTES 0x80

#define EFF_RING_VERTEX_BYTES 16

#define EFFECT_RING_START_ANGLE (-1.5707963f)

#define EFFECT_RING_FULL_TURN (6.2831853f)

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern f32 aViewEye[], aViewTarget[];

extern f32 effPCPFlashPolySphere2Vertex[], effPCPFlashPolySphereVertex[];

extern f32 effPCPFlashPolyRingVertex[], effPCPFlashPolySphereExpVertex[];

extern f32 def_dir[], effPCPFlashPolyRingExp2Vertex[], effPCPFlashPolyCircleVertex[];

typedef struct effFlashOrbitArcBlock_t {
    effPCPFlashPolyRingExp_tag header;
    effPCPFlashPolyRingExpPart_tag parts[1];
} effFlashOrbitArcBlock_t;

dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 face);

void dds3FlashPolyListDraw(dds3FlashPolyList_t *work);

void *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *group, s32 groupIndex);

dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 face);

void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool);

void *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex);

dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 face);

void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool);

void *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex);

dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 face);

void dds3FlashPolyListHDraw(dds3FlashPolyList_t *work);

void *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *group, s32 groupIndex);

void dds3FlashPolyListDelete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *hPoly, u32 blend);

void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *hPoly, u32 abgr);

void dds3FlashPolyListSetScale(dds3FlashPolyList_t *hPoly, f32 scale);

void dds3FlashPolyList2Delete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyList3Delete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyListHDelete(dds3FlashPolyList_t *hPoly);

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Create(effPCPFlashPolySphere2Param_t *pParam);

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2CreatePCF(void *table);

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Copy(effPCPFlashPolySphere2Param_t *params);

void effPCPFlashPolySphere2Delete(effPCPFlashPolySphere2_tag *hData);

void effPCPFlashPolySphere2SetPosition(void *hData, void *pPos);

void effPCPFlashPolySphere2SetColor(effPCPFlashPolySphere2_tag *hData, u32 abgr);

void effPCPFlashPolySphere2SetScale(effPCPFlashPolySphere2_tag *hData, f32 scale);

void effPCPFlashPolySphere2PartSetVC(effPCPFlashPolySphere2_tag *work, s32 index, s32 color);

void effPCPFlashPolySphere2PartSetVertex(effPCPFlashPolySphere2_tag *work, s32 index, void *view);

void effPCPFlashPolySphere2Draw(effPCPFlashPolySphere2_tag *hData);

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreate(effPCPFlashPolySphereParam_t *src);

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreatePCF(void *data);

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCopy(effPCPFlashPolySphereParam_t *src);

void effPCPFlashPolySphereDelete(effPCPFlashPolySphere_tag *hData);

void effPCPFlashPolySphereSetPosition(void *hData, void *pPos);

void effPCPFlashPolySphereSetColor(effPCPFlashPolySphere_tag *hData, u32 abgr);

void effPCPFlashPolySphereSetScale(effPCPFlashPolySphere_tag *hData, f32 scale);

void effPCPFlashPolySpherePartSetVC(effPCPFlashPolySphere_tag *work, s32 index, s32 color);

void effPCPFlashPolySpherePartInit(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook);

void effPCPFlashPolySpherePartSetVertex(effPCPFlashPolySphere_tag *work, s32 index, void *view);

void effPCPFlashPolySpherePartRotate(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook);

void effPCPFlashPolySphereDraw(effPCPFlashPolySphere_tag *hData);

effPCPFlashPolyRing_tag *effPCPFlashPolyRingCreate(effPCPFlashPolyRingParam_t *pParam);

effPCPFlashPolyRing_tag *effPCPFlashPolyRingCreatePCF(void *data);

effPCPFlashPolyRing_tag *effPCPFlashPolyRingCopy(effPCPFlashPolyRingParam_t *params);

void effPCPFlashPolyRingDelete(effPCPFlashPolyRing_tag *hData);

void effPCPFlashPolyRingSetPosition(void *hData, void *pPos);

void effPCPFlashPolyRingSetColor(effPCPFlashPolyRing_tag *hData, u32 abgr);

void effPCPFlashPolyRingSetScale(effPCPFlashPolyRing_tag *hData, f32 scale);

void effPCPFlashPolyRingPartSetVC(effPCPFlashPolyRing_tag *work, s32 index, s32 color);

void effPCPFlashPolyRingPartSetVertex(effPCPFlashPolyRing_tag *work, s32 index);

void effPCPFlashPolyRingPartRotate(effPCPFlashPolyRing_tag *pFlashPoly, s32 number);

void effPCPFlashPolyRingDraw(effPCPFlashPolyRing_tag *hData);

effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCreate(effPCPFlashPolyPipeParam_t *params);

effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCreatePCF(void *data);

effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCopy(effPCPFlashPolyPipeParam_t *params);

void effPCPFlashPolyPipeDelete(effPCPFlashPolyPipe_tag *hData);

void effPCPFlashPolyPipeSetPosition(void *hData, void *pPos);

void effPCPFlashPolyPipeSetColor(effPCPFlashPolyPipe_tag *hData, u32 abgr);

void effPCPFlashPolyPipeSetScale(effPCPFlashPolyPipe_tag *hData, f32 scale);

void effPCPFlashPolyPipePartSetVC(effPCPFlashPolyPipe_tag *work, s32 index, u32 colorMultiplier);

/* vu0 routine: the orbit offset plus a tilted disc of corner offsets for an
   accumulating particle, whose radius grows by the work record's radial step */
void effPCPFlashPolyPipePartSetVertex(effPCPFlashPolyPipe_tag *work, s32 index);

void effPCPFlashPolyPipePartRotate(effPCPFlashPolyPipe_tag *pFlashPoly, s32 number);

void effPCPFlashPolyPipeDraw(effPCPFlashPolyPipe_tag *hData);

effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCreate(effPCPFlashPolyRingExpParam_t *pParam);

effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCreatePCF(void *data);

effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCopy(effPCPFlashPolyRingExpParam_t *params);

void effPCPFlashPolyRingExpDelete(effPCPFlashPolyRingExp_tag *hData);

void effPCPFlashPolyRingExpSetPosition(void *hData, void *pPos);

void effPCPFlashPolyRingExpSetColor(effPCPFlashPolyRingExp_tag *hData, u32 abgr);

void effPCPFlashPolyRingExpSetScale(effPCPFlashPolyRingExp_tag *hData, f32 scale);

void effPCPFlashPolyRingExpPartSetVC(effPCPFlashPolyRingExp_tag *work, s32 index, s32 color);

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void effPCPFlashPolyRingExpPartSetVertex(effPCPFlashPolyRingExp_tag *work, s32 index);

void effPCPFlashPolyRingExpPartRotate(effPCPFlashPolyRingExp_tag *pFlashPoly, s32 number);

void effPCPFlashPolyRingExpDraw(effPCPFlashPolyRingExp_tag *hData);

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreate(effPCPFlashPolySphereExpParam_t *pParam);

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreatePCF(void *data);

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCopy(effPCPFlashPolySphereExpParam_t *params);

void effPCPFlashPolySphereExpDelete(effPCPFlashPolySphereExp_tag *hData);

void effPCPFlashPolySphereExpSetPosition(void *hData, void *pPos);

void effPCPFlashPolySphereExpSetColor(effPCPFlashPolySphereExp_tag *hData, u32 abgr);

void effPCPFlashPolySphereExpSetScale(effPCPFlashPolySphereExp_tag *hData, f32 scale);

void effPCPFlashPolySphereExpPartSetVC(effPCPFlashPolySphereExp_tag *work, s32 index, s32 color);

void effPCPFlashPolySphereExpPartInit(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook);

void effPCPFlashPolySphereExpPartSetVertex(effPCPFlashPolySphereExp_tag *work, s32 index, void *view);

void effPCPFlashPolySphereExpPartRotate(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook);

void effPCPFlashPolySphereExpDraw(effPCPFlashPolySphereExp_tag *hData);

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreate(effPCPFlashPolySphere2ExpParam_t *pParam);

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreatePCF(void *data);

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCopy(effPCPFlashPolySphere2ExpParam_t *params);

void effPCPFlashPolySphere2ExpDelete(effPCPFlashPolySphere2Exp_tag *hData);

void effPCPFlashPolySphere2ExpSetPosition(void *hData, void *pPos);

void effPCPFlashPolySphere2ExpSetColor(effPCPFlashPolySphere2Exp_tag *hData, u32 abgr);

void effPCPFlashPolySphere2ExpSetScale(effPCPFlashPolySphere2Exp_tag *hData, f32 scale);

void effPCPFlashPolySphere2ExpPartSetVC(effPCPFlashPolySphere2Exp_tag *work, s32 index, s32 color);

void effPCPFlashPolySphere2ExpPartSetVertex(effPCPFlashPolySphere2Exp_tag *work, s32 index, void *view);

void effPCPFlashPolySphere2ExpDraw(effPCPFlashPolySphere2Exp_tag *hData);

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Create(effFlashRadialStripParam_t *pParam);

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3CreatePCF(void *data);

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Copy(effFlashRadialStripParam_t *params);

void effPCPFlashPolySphere3Delete(effPCPFlashPolySphere3_tag *hData);

void effPCPFlashPolySphere3SetPosition(void *hData, void *pPos);

void effPCPFlashPolySphere3SetColor(effPCPFlashPolySphere3_tag *hData, u32 abgr);

void effPCPFlashPolySphere3SetScale(effPCPFlashPolySphere3_tag *hData, f32 scale);

void effPCPFlashPolySphere3PartSetVC(effPCPFlashPolySphere3_tag *work, s32 index, u32 colorMultiplier);

void effPCPFlashPolySphere3PartInit(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook);

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effPCPFlashPolySphere3PartSetVertex(effPCPFlashPolySphere3_tag *work, s32 index, void *view);

void effPCPFlashPolySphere3PartRotate(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook);

void effPCPFlashPolySphere3Draw(effPCPFlashPolySphere3_tag *hData);

effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Create(effPCPFlashPolyRingExp2Param_t *pParam);

effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2CreatePCF(void *data);

effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Copy(effPCPFlashPolyRingExp2Param_t *params);

void effPCPFlashPolyRingExp2Delete(effPCPFlashPolyRingExp2_tag *hData);

void effPCPFlashPolyRingExp2SetPosition(void *hData, void *pPos);

void effPCPFlashPolyRingExp2SetColor(effPCPFlashPolyRingExp2_tag *hData, u32 abgr);

void effPCPFlashPolyRingExp2SetScale(effPCPFlashPolyRingExp2_tag *hData, f32 scale);

void effPCPFlashPolyRingExp2PartSetVC(effPCPFlashPolyRingExp2_tag *work, s32 index, s32 color);

void effPCPFlashPolyRingExp2PartSetVertex(effPCPFlashPolyRingExp2_tag *work, s32 index);

void effPCPFlashPolyRingExp2PartRotate(effPCPFlashPolyRingExp2_tag *pFlashPoly, s32 number);

void effPCPFlashPolyRingExp2Draw(effPCPFlashPolyRingExp2_tag *hData);

effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCreate(effPCPFlashPolyCircleParam_t *pParam);

effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCreatePCF(void *data);

effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCopy(effPCPFlashPolyCircleParam_t *params);

void effPCPFlashPolyCircleDelete(effPCPFlashPolyCircle_tag *hData);

void effPCPFlashPolyCircleSetPosition(void *hData, void *pPos);

void effPCPFlashPolyCircleSetColor(effPCPFlashPolyCircle_tag *hData, u32 abgr);

void effPCPFlashPolyCircleSetScale(effPCPFlashPolyCircle_tag *hData, f32 scale);

void effPCPFlashPolyCirclePartSetVC(effPCPFlashPolyCircle_tag *work, s32 index, s32 color);

void effPCPFlashPolyCirclePartSetVertex(effPCPFlashPolyCircle_tag *work, s32 index, void *view);

void effPCPFlashPolyCircleDraw(effPCPFlashPolyCircle_tag *hData);

effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCreate(effPCPFlashPolyRingExpH_tag *pParam);

effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCreatePCF(void *table);

effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCopy(effPCPFlashPolyRingExpH_tag *params);

void effPCPFlashPolyRingExpHDelete(effPCPFlashPolyRingExpH_tag *hData);

void effPCPFlashPolyRingExpHSetPosition(void *hData, void *pPos);

void effPCPFlashPolyRingExpHSetColor(effPCPFlashPolyRingExpH_tag *hData, u32 abgr);

void effPCPFlashPolyRingExpHSetScale(effPCPFlashPolyRingExpH_tag *hData, f32 scale);

/* vu0 routine: copy the four matrix rows through vf28-vf31. */
void effPCPFlashPolyRingExpHSetRotateMatrix(effPCPFlashPolyRingExpH_tag *hData, void *pMatrix);

void effPCPFlashPolyRingExpHPartSetVC(effPCPFlashPolyRingExpH_tag *work, s32 index, s32 modulationColor);

/* vu0 routine: build five orbiting fan positions from a phase-derived basis.
 * No camera matrix is read. Quadword loads retain the native scratch W values. */
void effPCPFlashPolyRingExpHPartSetVertex(effPCPFlashPolyRingExpH_tag *work, s32 index);

void effPCPFlashPolyRingExpHPartRotate(effPCPFlashPolyRingExpH_tag *pFlashPoly, s32 number);

void effPCPFlashPolyRingExpHDraw(effPCPFlashPolyRingExpH_tag *hData);

dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 face);

void dds3FlashPolyListDelete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyListDraw(dds3FlashPolyList_t *work);

void *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *group, s32 groupIndex);

void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *hPoly, u32 blend);

void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *hPoly, u32 abgr);

void dds3FlashPolyListSetScale(dds3FlashPolyList_t *hPoly, f32 scale);

dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 face);

void dds3FlashPolyList2Delete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool);

void *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex);

dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 face);

void dds3FlashPolyList3Delete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool);

void *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex);

dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 face);

void dds3FlashPolyListHDelete(dds3FlashPolyList_t *hPoly);

void dds3FlashPolyListHDraw(dds3FlashPolyList_t *work);

void *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *group, s32 groupIndex);

u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *group, s32 groupIndex);

void dds3FlashPolyListHSetBlend(dds3FlashPolyList_t *hPoly, u32 blend);

void dds3FlashPolyListHSetABGR(dds3FlashPolyList_t *hPoly, u32 abgr);

void dds3FlashPolyListHSetScale(dds3FlashPolyList_t *hPoly, f32 scale);

#endif
