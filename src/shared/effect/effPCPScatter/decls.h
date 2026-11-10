/* Declarations of smt3-decomp src/smt3/effect/effPCPScatter.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_EFFPCPSCATTER_H
#define SHARED_EFFECT_EFFPCPSCATTER_H

#include "common.h"

#include "sdf_types.h"

#include "effPCPData.h"

#include "ps2asm.h"

#include "sdf_resource.h"

#include "sdfFrame.h"

struct effDebugPolyParams_t;

struct effRandState_t;

struct dds3WhirlwindPolyList_tag;

struct effPCPWhirlwindPoly_tag;

struct effPCPWhirlwind2Poly_tag;

struct effPCPWhirlwind3Poly_tag;

struct effPCPWhirlwindPolyParam_t;

struct effPCPWhirlwind2PolyParam_t;

struct effPCPWhirlwind3PolyParam_t;

struct effPCPWhirlwindPolyPart_tag;

struct effPCPWwSpherePoly_tag;

struct effPCPWwSpherePolyParam_t;

struct effPCPWwSpherePolyPart_tag;

struct dds3ScatterPolyList_tag;

struct effPCPHamaCardSpiralParam_t;

struct effPCPHamaCardSpiralPart_tag;

struct effPCPHamaCardSpiral_tag;

struct effPCPHamaCardScatterParam_t;

struct effPCPHamaCardScatterPart_tag;

struct effPCPHamaCardScatter_tag;

struct effPCPHamaCardSphereParam_t;

struct effPCPHamaCardSpherePart_tag;

struct effPCPHamaCardSphere_tag;

struct sdfMatPacket_t;

struct sdfGsMemSlice_t;

typedef struct effDebugPolyParams_t effDebugPolyParams_t;

typedef struct sdfTexPacket_t sdfTexPacket_t;

typedef struct sdfMatPacket_t sdfMatPacket_t;

typedef union sdfMatUvwParm_t sdfMatUvwParm_t;

typedef struct dds3WhirlwindPolyList_tag dds3WhirlwindPolyList_tag;

typedef struct effPCPWhirlwindPolyParam_t effPCPWhirlwindPolyParam_t;

typedef struct effPCPWhirlwindPolyPart_tag effPCPWhirlwindPolyPart_tag;

typedef struct effPCPWhirlwindPoly_tag effPCPWhirlwindPoly_tag;

typedef struct effPCPWhirlwind2PolyParam_t effPCPWhirlwind2PolyParam_t;

typedef struct effPCPWhirlwind2Poly_tag effPCPWhirlwind2Poly_tag;

typedef struct effPCPWhirlwind3PolyParam_t effPCPWhirlwind3PolyParam_t;

typedef struct effPCPWhirlwind3Poly_tag effPCPWhirlwind3Poly_tag;

typedef struct effPCPWwSpherePolyParam_t effPCPWwSpherePolyParam_t;

typedef struct effPCPWwSpherePolyPart_tag effPCPWwSpherePolyPart_tag;

typedef struct effPCPWwSpherePoly_tag effPCPWwSpherePoly_tag;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct effPCPHamaCardSpiralParam_t effPCPHamaCardSpiralParam_t;

typedef struct effPCPHamaCardSpiralPart_tag effPCPHamaCardSpiralPart_tag;

typedef struct effPCPHamaCardSpiral_tag effPCPHamaCardSpiral_tag;

typedef struct effPCPHamaCardScatterPart_tag effPCPHamaCardScatterPart_tag;

typedef struct effPCPHamaCardScatterParam_t effPCPHamaCardScatterParam_t;

typedef struct effPCPHamaCardScatter_tag effPCPHamaCardScatter_tag;

typedef struct effPCPHamaCardSphereParam_t effPCPHamaCardSphereParam_t;

typedef struct effPCPHamaCardSpherePart_tag effPCPHamaCardSpherePart_tag;

typedef struct effPCPHamaCardSphere_tag effPCPHamaCardSphere_tag;

typedef struct sdfGrOt_t sdfGrOt_t;

typedef struct effRandState_t effRandState_t;

#define EFF_SCATTER_NEUTRAL_COLOR 0x80808080

#define EFF_SCATTER_FULL_TURN 6.2831852f

#define EFF_SCATTER_DEGREES_TO_RADIANS 0.017453292f

#define EFF_SCATTER_RIGHT_ANGLE_DEGREES 90.0f

#define EFF_SCATTER_DIAGONAL_ANGLE_DEGREES 45.0f

#define EFF_SCATTER_TILT_STEP_DEGREES 5.0f

#define EFF_SCATTER_INITIAL_RIBBON_RADIUS 200.0f

#define EFF_SCATTER_RANDOM_MIDPOINT 0.5f

#define EFF_SCATTER_RANDOM_SPAN 2.0f

#define EFF_SCATTER_POOL_SLOTS_PER_GROUP 3

#define EFF_SCATTER_POOL_RECORD_WORDS_PER_SLOT 8

#define EFF_SCATTER_POOL_AUX_WORDS_PER_SLOT 2

#define EFF_SCATTER_WORD_BYTES 4

#define EFF_SCATTER_POOL_CONTROL_BYTES 0x34

#define EFF_SCATTER_RECORD_BYTES 0x60

#define EFF_SCATTER_AUX_RECORD_BYTES 0x18

#define EFF_SCATTER_DRAW_TEMPLATE_BYTES 0x2C

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

struct dds3ScatterPolyListTexResource_tag;

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

/* Shared scatter texture reference, created and released independently of pools. */
typedef struct dds3ScatterPolyListTexResource_tag {
    sdfTexHandle_t *textureHandle;
    s32 refCount;
} dds3ScatterPolyListTexResource_tag; /* 0x08 */

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

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

/* The drawable owns its geometry allocation and asset independently of the
 * effect instance that supplies its transform and scale. */
struct dds3WhirlwindPolyList_tag {
    f32 origin[4];             // 0x00
    f32 matrix[16];            // 0x10
    u32 packetQueueIndex;     // 0x50
    u32 color;                // 0x54
    u32 particleCount;        // 0x58
    s32 vectorsPerParticle;   // 0x5C: two coordinate vectors per vertex pair
    f32 scale;                // 0x60
    f32 *points;              // 0x64
    f32 *uv;                  // 0x68
    u32 *vertexColors;        // 0x6C
    u32 *colors;              // 0x70
    struct sdfMat_t *asset;    // 0x74
    struct sdfMemHandle_t *allocation; // 0x78
    struct dds3ScatterPolyListTexResource_tag *sharedResource; // 0x7C
};

/* The first ring variant retains its complete copied parameter head in the
 * runtime owner. Its particle array follows the fixed 0x18C-byte owner. */
struct effPCPWhirlwindPolyParam_t {
    f32 origin[4];
    f32 matrix[16];
    u32 packetQueueIndex;
    u8 loop;
    u8 pad55[3];
    s32 duration;
    u32 particleCount;
    u32 segmentsPerParticle;
    u32 randomDelayRange;
    s32 fadeIn;
    s32 fadeRange;
    f32 angleStepBase;
    f32 angleStepJitter;
    f32 riseStep;
    f32 tiltScale;
    f32 heightOffsetBase;
    f32 heightOffsetJitter;
    f32 initialRise;
    f32 riseDecay;
    u8 pad90[4];
    f32 initialTiltSpeed;
    f32 tiltDamping;
    f32 radiusBase;
    f32 radiusJitter;
    f32 targetRadius;
    f32 targetRadiusJitter;
    s32 baseColor;
    u32 uSpan; /* Horizontal UV extent. */
    u32 vSpan; /* Vertical UV extent. */
    u8 padB8[0x80];
};

/* Shared particle record used by B/C and the related copied-parameter ring. */
struct effPCPWhirlwindPolyPart_tag {
    f32 rot_x;
    f32 rot_y;
    s32 frame;
    f32 up_s;
    f32 radian;
    f32 radian_s;
    f32 radian_add;
    f32 radius;
    f32 radius_s;
    f32 width;
};

struct effPCPWhirlwindPoly_tag {
    f32 matrix[16];
    effPCPWhirlwindPolyParam_t param;
    effPCPWhirlwindPolyPart_tag *pPart;
    f32 scale;
    u32 abgr;
    dds3WhirlwindPolyList_tag *hPoly;
    sdfMemHandle_t *allocationHandle;
};

/* B's copied parameter block and runtime owner. Its particle pointer follows
 * the copied parameters, and the variable particle array follows the owner. */
struct effPCPWhirlwind2PolyParam_t {
    f32 pos[4];
    f32 matrix[16];
    u32 blend;
    u8 loop;
    u8 pad55[3];
    s32 life;
    u32 number;
    u32 split;
    u32 timing;
    s32 fade_in;
    s32 fade_out;
    f32 radian_w;
    f32 radian_w_r;
    f32 radian_w_u;
    f32 incline;
    f32 width;
    f32 width_r;
    f32 up_s;
    f32 up_a;
    u8 up_r[4];
    f32 roll_s;
    f32 roll_a;
    f32 radius;
    f32 radius_r;
    f32 radius_s;
    f32 radius_s_r;
    f32 radius_a;
    s32 abgr;
    u32 repeat_u; /* Horizontal UV extent. */
    u32 repeat_v; /* Vertical UV extent. */
    u8 padBC[0x80];
};

struct effPCPWhirlwind2Poly_tag {
    f32 matrix[16];
    effPCPWhirlwind2PolyParam_t param;
    effPCPWhirlwindPolyPart_tag *pPart;
    f32 scale;
    u32 abgr;
    s32 frame_all;
    dds3WhirlwindPolyList_tag *hPoly;
    sdfMemHandle_t *allocationHandle;
};

/* C adds staggered ring motion and two color keys to the copied parameters. */
struct effPCPWhirlwind3PolyParam_t {
    f32 pos[4];
    f32 matrix[16];
    u32 blend;
    u8 loop;
    u8 pad55[3];
    s32 life;
    u32 number;
    u32 split;
    u32 timing;
    s32 fade_in;
    s32 fade_out;
    f32 radian_w;
    f32 radian_w_r;
    f32 incline;
    f32 height;
    f32 slant;
    f32 width;
    f32 width_r;
    f32 up_s;
    f32 up_a;
    u8 up_r[4];
    f32 roll_s;
    f32 roll_a;
    f32 radius;
    f32 radius_r;
    f32 radius_s;
    f32 radius_s_r;
    f32 radius_a;
    s32 abgr_s;
    s32 abgr_e;
    u32 repeat_u; /* Horizontal UV extent; this variant reverses the legacy names. */
    u32 repeat_v;
    u8 padC4[0x80];
};

struct effPCPWhirlwind3Poly_tag {
    f32 matrix[16];
    effPCPWhirlwind3PolyParam_t param;
    effPCPWhirlwindPolyPart_tag *pPart;
    f32 scale;
    u32 abgr;
    s32 frame_all;
    dds3WhirlwindPolyList_tag *hPoly;
    sdfMemHandle_t *hMem;
};

/* Plain is a distinct 0xE8-byte parameter and 0x28-byte particle variant. */
struct effPCPWwSpherePolyParam_t {
    f32 pos[4];
    u32 blend;
    u8 loop;
    u8 pad15[3];
    s32 life;
    u32 number;
    u32 split;
    u32 timing;
    s32 fade_in;
    s32 fade_out;
    f32 radian_w;
    f32 radian_w_r;
    f32 width;
    f32 width_r;
    f32 roll_s;
    f32 roll_a;
    f32 radius;
    f32 radius_r;
    f32 radius_s;
    f32 radius_a;
    s32 radius_aw;
    s32 abgr;
    u32 repeat_u; /* Horizontal UV extent. */
    u32 repeat_v; /* Vertical UV extent. */
    u8 pad68[0x80];
};

/* The flat particle has three rotation angles before its signed age; it is
 * distinct from the other variants' particle even though both records are 0x28. */
struct effPCPWwSpherePolyPart_tag {
    f32 rot[3];
    s32 frame;
    f32 radian;
    f32 radian_s;
    f32 radian_add;
    f32 radius;
    f32 radius_s;
    f32 width;
};

struct effPCPWwSpherePoly_tag {
    f32 matrix[16];
    effPCPWwSpherePolyParam_t param;
    effPCPWwSpherePolyPart_tag *pPart;
    f32 scale;
    u32 abgr;
    dds3WhirlwindPolyList_tag *hPoly;
    sdfMemHandle_t *hMem;
};

/* Appended after the two record arrays; resource helpers own this same control block. */
typedef struct dds3ScatterPolyList_tag {
    f32 origin[4];
    u32 poolMode; /* 0x10: selects the surface node used when drawing the pool */
    u32 color;
    s32 secondWordCount;
    f32 scale;
    u8 *recordBase;
    u8 *auxRecordBase;
    sdfMat_t *drawAsset;
    sdfMemHandle_t *allocation;
    dds3ScatterPolyListTexResource_tag *sharedResource;
} dds3ScatterPolyList_tag; /* 0x34 */

struct effPartHandle_t;

/* One randomized channel record. */
struct effPartHandle_t;

/* Compact effect owner: its halfword kind selects the opaque payload's operations. */
struct effPartHandle_t {
    u16 kind;
    u8 pad02[2];
    void *payload;
};

/* Serialized radial scatter parameters copied into each runtime work block. */
struct effPCPHamaCardSpiralParam_t {
    f32 pos[4];
    f32 tex_w;
    f32 tex_h;
    u32 blend;
    u8 loop;
    u8 pad1D[3];
    u32 number;
    u32 simu_num;
    s32 timing;
    u32 fade_in;
    u32 fade_out;
    s32 life;
    s32 wait;
    f32 up_s;
    f32 up_a;
    f32 roll_s;
    f32 roll_a;
    f32 radius_s;
    f32 radius_e;
    f32 radius_s_r;
    f32 radius_e_r;
    f32 up_r;
    u8 vibrate;
    u8 pad61[3];
    f32 vib_power;
    u8 attach;
    u8 pad69[3];
    s32 attach_f;
    u32 attach_i;
};

/* Per-particle age and radial/vertical motion state. */
struct effPCPHamaCardSpiralPart_tag {
    s32 frame;
    f32 radius_s;
    f32 roll_s;
    f32 up_s;
    f32 radius;
    f32 radian;
    f32 height;
};

/* Runtime owner followed inline by its variable-length particle array. */
struct effPCPHamaCardSpiral_tag {
    effPCPHamaCardSpiralParam_t param;
    effPCPHamaCardSpiralPart_tag *pPart;
    f32 scale;
    u32 abgr;
    dds3ScatterPolyList_tag *hPoly;
    sdfMemHandle_t *hMem;
    u32 AttachNum;
    effPartHandle_t **pAttach;
    sdfMemHandle_t *hAttachMem;
};

struct effPCPHamaCardScatterPart_tag {
    s32 frame;
    f32 height;
    f32 height_s;
    f32 radian;
    f32 radian_w;
    f32 radian_s;
    f32 radius;
    f32 roll;
    f32 unk20;
    f32 unk24;
};

/* Serialized parameters copied into each ribbon-scatter work owner. */
struct effPCPHamaCardScatterParam_t {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    s32 fadeDuration;
    f32 heightStep;
    u8 pad3C[4];
    u8 duplicateParticles;
    u8 pad41[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
};

/* Runtime owner followed inline by its variable-length particle array. */
struct effPCPHamaCardScatter_tag {
    effPCPHamaCardScatterParam_t param;
    effPCPHamaCardScatterPart_tag *pPart;
    f32 scale;
    u32 abgr;
    dds3ScatterPolyList_tag *hPoly;
    sdfMemHandle_t *hMem;
    u32 AttachNum;
    effPartHandle_t **pAttach;
    sdfMemHandle_t *hAttachMem;
};

/* Serialized parameters copied into each spin-scatter work owner. */
struct effPCPHamaCardSphereParam_t {
    f32 pos[4];
    f32 tex_w;
    f32 tex_h;
    u32 blend;
    u8 loop;
    u8 pad1D[3];
    u32 number;
    s32 timing;
    u32 fade_in;
    u32 fade_out;
    s32 life;
    s32 wait;
    f32 roll_s;
    f32 roll_a;
    f32 radius_s;
    f32 radius_e;
    f32 radius_s_r;
    f32 radius_e_r;
    u8 vibrate;
    u8 pad51[3];
    f32 vib_power;
    u8 attach;
    u8 pad59[3];
    s32 attach_f;
    u32 attach_i;
};

struct effPCPHamaCardSpherePart_tag {
    s32 frame;
    f32 radius_s;
    f32 roll_s;
    f32 radius;
    f32 radian;
    f32 axis[3];
};

/* Runtime owner followed inline by its variable-length particle array. */
struct effPCPHamaCardSphere_tag {
    effPCPHamaCardSphereParam_t param;
    effPCPHamaCardSpherePart_tag *pPart;
    f32 scale;
    u32 abgr;
    dds3ScatterPolyList_tag *hPoly;
    sdfMemHandle_t *hMem;
    u32 AttachNum;
    effPartHandle_t **pAttach;
    sdfMemHandle_t *hAttachMem;
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

struct TMX_HEADER_t;

extern void *effPkt3DPrimMakePoly(effDebugPolyParams_t *aParm);

/* Four ordered 32-bit words in the shared effect random-number state. */
struct effRandState_t {
    u32 words[4];
};

extern effRandState_t dds3EffectBase[];

void dds3EffectPartDelete(effPartHandle_t *hEffPart);

extern u32 dds3WhirlwindPolyListGetABGR(dds3WhirlwindPolyList_tag *hPoly, s32 index);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);

/* Constructors copy one serialized parameter variant into its runtime owner. */
effPCPWhirlwindPoly_tag *effPCPWhirlwindPolyCreate(
    effPCPWhirlwindPolyParam_t *pParam, addr_t pTex);

effPCPWwSpherePoly_tag *effPCPWwSpherePolyCreate(
    effPCPWwSpherePolyParam_t *pParam, addr_t pTex);

extern dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCopy(dds3ScatterPolyListTexResource_tag *hSplTexResSrc);

effPCPWhirlwind2Poly_tag *effPCPWhirlwind2PolyCreate(
    effPCPWhirlwind2PolyParam_t *pParam, addr_t pTex);

effPCPWhirlwind3Poly_tag *effPCPWhirlwind3PolyCreate(
    effPCPWhirlwind3PolyParam_t *pParam, addr_t pTex);

extern effDebugPolyParams_t effDebugPoly;

extern sdfGrOt_t *effPCPScatterSurfaceByQueue[];

extern effDebugPolyParams_t effPCPScatterDrawParams[];

extern sdfGrOt_t *effPCPScatterSurfaceByPool[];

extern u32 effPCPScatterTexcoords[];

extern u32 effPCPScatterParams[];

extern u8 D_0031F3E0[];

extern void *memset(void *a, s32 b, size_t c);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 aSize);

void *sdfChipAllocDirect(s32 aSize);

void sdfDrawTagLink(struct sdfDrawTag_t *aTag, addr_t aPacket);

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. */
void MultMatrixNoArg(void);

void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

struct sdfMat_t *sdfMatNew(void);

f32 sdfCos(f32 aRadius);

void sdfDrawTagInit(struct sdfDrawTag_t *a);

/* Set vf28-vf31 to vf28-vf31 * vf24-vf27 in place. */
void MultMatrixRevNoArg(void);

/* Queue an asset owner for the deferred SdfAsset release callback. */
void sdfMatFreeDelayed(struct sdfMat_t *aMat);

void sdfChipFreeDirect(void *aPtr);

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

/* Store the primary or secondary texture owner and mark its draw state dirty. */
void sdfMatSetTex(struct sdfMat_t *aMat, struct sdfTexHandle_t *aTex);

void sdfMatSetConstant(struct sdfMat_t *aMat, f32 aRatio);

f32 sdfSin(f32 aRadius);

/* Acquire a texture from a serialized texture-file header. */
struct sdfTexHandle_t *sdfTexSet(const TMX_HEADER_t *aTex);

void sdfTexFreeDelayed(sdfTexHandle_t *aHandle);

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixX(f32 aAngle);

/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void SetRotateMatrixY_B(f32 aAngle);

extern void SetRotateMatrixXYZ(f32 *aAngles);

/* Dependencies for the restored DDS effect implementations. */
extern effPartHandle_t *dds3EffectPartCreate(s32 type, void *pData);

extern effPartHandle_t *dds3EffectPartCopy(effPartHandle_t *hEffPartSrc);

extern void dds3EffectPartDraw(effPartHandle_t *hEffPart);

extern void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, f32 *pPos);

extern void dds3EffectPartSetColor(effPartHandle_t *hEffPart, u32 abgr);

extern u32 dds3GetRandA(effRandState_t *name);

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern s32 effMagaCylinderCalcColor(s32 color, s32 tint);

extern f32 sdfAtan(f32 aX);

extern void SetRotateMatrixAxis(f32 aAngle, const void *aAxis);

extern void *memcpy(void *a, const void *b, size_t c);

effPCPHamaCardSpiral_tag *effPCPHamaCardSpiralCreate(const effPCPHamaCardSpiralParam_t *pParam, addr_t pTex, void *pData);

effPCPHamaCardSphere_tag *effPCPHamaCardSphereCreate(const effPCPHamaCardSphereParam_t *pParam, addr_t pTex, void *pData);

effPCPHamaCardScatter_tag *effPCPHamaCardScatterCreate(const effPCPHamaCardScatterParam_t *pParam, addr_t pTex, void *pData);

dds3WhirlwindPolyList_tag *dds3WhirlwindPolyListCreate(u32 number, u32 face);

void dds3WhirlwindPolyListSetRotateMatrix(dds3WhirlwindPolyList_tag *hPoly, const void *pMatrix);

dds3ScatterPolyList_tag *dds3ScatterPolyListCreate(s32 face);

void dds3ScatterPolyListDelete(dds3ScatterPolyList_tag *hPoly);

void dds3ScatterPolyListDraw(dds3ScatterPolyList_tag *hPoly);

void dds3ScatterPolyListSetTex(dds3ScatterPolyList_tag *hPoly, u32 pTex);

void dds3ScatterPolyListCopyTex(dds3ScatterPolyList_tag *hPoly, dds3ScatterPolyList_tag *hPolySrc);

u128 *dds3ScatterPolyListGetVertex(dds3ScatterPolyList_tag *hPoly, s32 index);

u32 *dds3ScatterPolyListGetVC(dds3ScatterPolyList_tag *hPoly, s32 index);

dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCreate(u32 pTex);

void dds3ScatterPolyListTexResourceDelete(dds3ScatterPolyListTexResource_tag *hSplTexRes);

void dds3WhirlwindPolyListDelete(dds3WhirlwindPolyList_tag *hPoly);

void dds3WhirlwindPolyListDraw(dds3WhirlwindPolyList_tag *object);

void dds3WhirlwindPolyListSetTex(dds3WhirlwindPolyList_tag *hPoly, u32 pTex);

void dds3WhirlwindPolyListCopyTex(dds3WhirlwindPolyList_tag *hPoly, dds3WhirlwindPolyList_tag *hPolySrc);

f32 *dds3WhirlwindPolyListGetVertex(dds3WhirlwindPolyList_tag *hPoly, s32 index);

f32 *dds3WhirlwindPolyListGetUV(dds3WhirlwindPolyList_tag *hPoly, s32 index);

#define EFF_SCATTER_PARAM_BLOCK 0

#define EFF_SCATTER_RESOURCE_BLOCK 1

#define EFF_SCATTER_CHILD_BLOCK 2

#define EFF_SCATTER_RGB_MASK 0xFFFFFF

#define EFF_SCATTER_HALF_TURN 3.14159265f

#define EFF_SCATTER_HALF_SEGMENT 0.5f

#define EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK 6

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

effPCPHamaCardSpiral_tag *effPCPHamaCardSpiralCreate();

effPCPHamaCardSpiral_tag *effPCPHamaCardSpiralCreatePCF(void *parameterTable);

effPCPHamaCardSpiral_tag *effPCPHamaCardSpiralCopy();

void effPCPHamaCardSpiralDelete(effPCPHamaCardSpiral_tag *hData);

void effPCPHamaCardSpiralPartInit(effPCPHamaCardSpiral_tag *pHCardSpiral, u32 number);

void effPCPHamaCardSpiralDraw(effPCPHamaCardSpiral_tag *hData);

void effPCPHamaCardSpiralSetPosition(void *hData, void *pPos);

void effPCPHamaCardSpiralSetColor(effPCPHamaCardSpiral_tag *hData, u32 abgr);

void effPCPHamaCardSpiralSetScale(effPCPHamaCardSpiral_tag *hData, f32 scale);

effPCPHamaCardSphere_tag *effPCPHamaCardSphereCreate();

effPCPHamaCardSphere_tag *effPCPHamaCardSphereCreatePCF(void *parameterTable);

effPCPHamaCardSphere_tag *effPCPHamaCardSphereCopy();

void effPCPHamaCardSphereDelete(effPCPHamaCardSphere_tag *hData);

void effPCPHamaCardSpherePartInit(effPCPHamaCardSphere_tag *pHCardSphere, s32 number);

void effPCPHamaCardSphereDraw(effPCPHamaCardSphere_tag *hData);

void effPCPHamaCardSphereSetPosition(void *hData, void *pPos);

void effPCPHamaCardSphereSetColor(effPCPHamaCardSphere_tag *hData, u32 abgr);

void effPCPHamaCardSphereSetScale(effPCPHamaCardSphere_tag *hData, f32 scale);

effPCPHamaCardScatter_tag *effPCPHamaCardScatterCreate();

void effPCPHamaCardScatterCreatePCF(void *parameterTable);

effPCPHamaCardScatter_tag *effPCPHamaCardScatterCopy();

void effPCPHamaCardScatterDelete(effPCPHamaCardScatter_tag *hData);

void effPCPHamaCardScatterPartInit(effPCPHamaCardScatter_tag *pHCardScatter, s32 number);

/* vu0 routine: each particle sweeps three ribbon quads around its tilt angle,
 * and the tilt reverses once it passes half its stored span. */
void effPCPHamaCardScatterDraw(effPCPHamaCardScatter_tag *hData);

void effPCPHamaCardScatterSetPosition(void *pHCardScatter, void *pPos);

void effPCPHamaCardScatterSetColor(effPCPHamaCardScatter_tag *pHCardScatter, u32 abgr);

void effPCPHamaCardScatterSetScale(void);

dds3ScatterPolyList_tag *dds3ScatterPolyListCreate(s32 face);

void dds3ScatterPolyListDelete(dds3ScatterPolyList_tag *hPoly);

void dds3ScatterPolyListDraw(dds3ScatterPolyList_tag *hPoly);

void dds3ScatterPolyListSetTex(dds3ScatterPolyList_tag *hPoly, u32 pTex);

void dds3ScatterPolyListCopyTex(dds3ScatterPolyList_tag *hPoly, dds3ScatterPolyList_tag *hPolySrc);

u128 *dds3ScatterPolyListGetVertex(dds3ScatterPolyList_tag *hPoly, s32 index);

u32 *dds3ScatterPolyListGetVC(dds3ScatterPolyList_tag *hPoly, s32 index);

dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCreate(u32 pTex);

void dds3ScatterPolyListTexResourceDelete(dds3ScatterPolyListTexResource_tag *hSplTexRes);

dds3ScatterPolyListTexResource_tag *dds3ScatterPolyListTexResourceCopy(dds3ScatterPolyListTexResource_tag *hSplTexResSrc);

effPCPWhirlwindPoly_tag *effPCPWhirlwindPolyCreate();

effPCPWhirlwindPoly_tag *effPCPWhirlwindPolyCreatePCF(void *parameterTable);

effPCPWhirlwindPoly_tag *effPCPWhirlwindPolyCopy(effPCPWhirlwindPoly_tag *work);

void effPCPWhirlwindPolyDelete(effPCPWhirlwindPoly_tag *hData);

void effPCPWhirlwindPolyPartInit(effPCPWhirlwindPoly_tag *pWhirlwindPoly, s32 index);

void effPCPWhirlwindPolyPartMove(effPCPWhirlwindPoly_tag *pWhirlwindPoly, s32 index);

void effPCPWhirlwindPolyDraw(effPCPWhirlwindPoly_tag *hData);

void effPCPWhirlwindPolySetPosition(effPCPWhirlwindPoly_tag *hData, void *pPos);

void effPCPWhirlwindPolySetScale(effPCPWhirlwindPoly_tag *hData, f32 scale);

void effPCPWhirlwindPolySetColor(effPCPWhirlwindPoly_tag *hData, u32 abgr);

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effPCPWhirlwindPolySetRotateMatrix(effPCPWhirlwindPoly_tag *hData, void *pMatrix);

effPCPWhirlwind2Poly_tag *effPCPWhirlwind2PolyCreate();

effPCPWhirlwind2Poly_tag *effPCPWhirlwind2PolyCreatePCF(void *parameterTable);

effPCPWhirlwind2Poly_tag *effPCPWhirlwind2PolyCopy(effPCPWhirlwind2Poly_tag *work);

void effPCPWhirlwind2PolyDelete(effPCPWhirlwind2Poly_tag *hData);

void effPCPWhirlwind2PolyPartInit(effPCPWhirlwind2Poly_tag *pWhirlwind2Poly, s32 index);

void effPCPWhirlwind2PolyPartMove(effPCPWhirlwind2Poly_tag *pWhirlwind2Poly, s32 index);

void effPCPWhirlwind2PolyDraw(effPCPWhirlwind2Poly_tag *hData);

void effPCPWhirlwind2PolySetPosition(effPCPWhirlwind2Poly_tag *hData, void *pPos);

void effPCPWhirlwind2PolySetScale(effPCPWhirlwind2Poly_tag *hData, f32 scale);

void effPCPWhirlwind2PolySetColor(effPCPWhirlwind2Poly_tag *hData, u32 abgr);

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effPCPWhirlwind2PolySetRotateMatrix(effPCPWhirlwind2Poly_tag *hData, void *pMatrix);

effPCPWhirlwind3Poly_tag *effPCPWhirlwind3PolyCreate();

effPCPWhirlwind3Poly_tag *effPCPWhirlwind3PolyCreatePCF(void *parameterTable);

effPCPWhirlwind3Poly_tag *effPCPWhirlwind3PolyCopy(effPCPWhirlwind3Poly_tag *work);

void effPCPWhirlwind3PolyDelete(effPCPWhirlwind3Poly_tag *hData);

void effPCPWhirlwind3PolyPartInit(effPCPWhirlwind3Poly_tag *pWhirlwind3Poly, s32 index);

void effPCPWhirlwind3PolyPartMove(effPCPWhirlwind3Poly_tag *pWhirlwind3Poly, s32 index);

void effPCPWhirlwind3PolyDraw(effPCPWhirlwind3Poly_tag *hData);

void effPCPWhirlwind3PolySetPosition(effPCPWhirlwind3Poly_tag *hData, void *pPos);

void effPCPWhirlwind3PolySetScale(effPCPWhirlwind3Poly_tag *hData, f32 scale);

void effPCPWhirlwind3PolySetColor(effPCPWhirlwind3Poly_tag *hData, u32 abgr);

/* vu0 routine: compose params.matrix and source via the native product into work->matrix. */
void effPCPWhirlwind3PolySetRotateMatrix(effPCPWhirlwind3Poly_tag *hData, void *pMatrix);

effPCPWwSpherePoly_tag *effPCPWwSpherePolyCreate();

effPCPWwSpherePoly_tag *effPCPWwSpherePolyCreatePCF(void *parameterTable);

effPCPWwSpherePoly_tag *effPCPWwSpherePolyCopy(effPCPWwSpherePoly_tag *work);

void effPCPWwSpherePolyDelete(effPCPWwSpherePoly_tag *hData);

void effPCPWwSpherePolyPartInit(effPCPWwSpherePoly_tag *pWwSpherePoly, s32 index);

void effPCPWwSpherePolyPartMove(effPCPWwSpherePoly_tag *pWwSpherePoly, s32 index);

void effPCPWwSpherePolyDraw(effPCPWwSpherePoly_tag *hData);

void effPCPWwSpherePolySetPosition(effPCPWwSpherePoly_tag *hData, void *pPos);

void effPCPWwSpherePolySetScale(effPCPWwSpherePoly_tag *hData, f32 scale);

void effPCPWwSpherePolySetColor(effPCPWwSpherePoly_tag *hData, u32 abgr);

/* vu0 routine: copy four transform rows through the matrix bank. */
void effPCPWwSpherePolySetRotateMatrix(effPCPWwSpherePoly_tag *hData, void *pMatrix);

dds3WhirlwindPolyList_tag *dds3WhirlwindPolyListCreate(u32 number, u32 face);

void dds3WhirlwindPolyListDelete(dds3WhirlwindPolyList_tag *hPoly);

void dds3WhirlwindPolyListDraw(dds3WhirlwindPolyList_tag *object);

void dds3WhirlwindPolyListSetTex(dds3WhirlwindPolyList_tag *hPoly, u32 pTex);

void dds3WhirlwindPolyListCopyTex(dds3WhirlwindPolyList_tag *hPoly, dds3WhirlwindPolyList_tag *hPolySrc);

f32 *dds3WhirlwindPolyListGetVertex(dds3WhirlwindPolyList_tag *hPoly, s32 index);

f32 *dds3WhirlwindPolyListGetUV(dds3WhirlwindPolyList_tag *hPoly, s32 index);

u32 dds3WhirlwindPolyListGetABGR(dds3WhirlwindPolyList_tag *hPoly, s32 index);

/* vu0 routine: copy four transform rows through the matrix bank. */
void dds3WhirlwindPolyListSetRotateMatrix(dds3WhirlwindPolyList_tag *hPoly, const void *pMatrix);

#endif
