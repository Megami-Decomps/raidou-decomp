#include "common.h"
#include "effPCPFlash.h"
#include "effMagatuhi.h"
#include "effPCPData.h"
#include "ps2asm.h"
#include "sdf_resource.h"
#include "sdf_types.h"
#include "sdfFrame.h"
#include "sdk_libc.h"
#include "sdfVu0.h"
#include "sdfMath.h"
#include "sdfPacketBuff.h"
#include "effMath.h"
#include "effect.h"
#include "libsdf.h"
#include "effPacket.h"
#include "sdfDraw.h"
#include "sdfMat.h"
#include "sdfMemory.h"


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


struct sdfGsMemSlice_t;



typedef struct effPCPFlashPolyRingExpHPart_tag effPCPFlashPolyRingExpHPart_tag;

typedef struct effPCPFlashPolyRingExpH_tag effPCPFlashPolyRingExpH_tag;

typedef struct effPCPFlashPolyCircleParam_t effPCPFlashPolyCircleParam_t;

typedef struct effPCPFlashPolySphere2Part_tag effPCPFlashPolySphere2Part_tag;

typedef struct effPCPFlashPolySphere2ExpParam_t effPCPFlashPolySphere2ExpParam_t;

typedef struct effPCPFlashPolySphereParam_t effPCPFlashPolySphereParam_t;

typedef struct effPCPFlashPolySphere2Param_t effPCPFlashPolySphere2Param_t;

typedef struct effPCPFlashPolySphere2_tag effPCPFlashPolySphere2_tag;


typedef struct effBezierDrawParam_t effBezierDrawParam_t;

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

struct sdfMat_t;


struct sdfTexHandle_t;


/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

struct sdfMemHandle_t;

/* Embedded command work at actor +0x20. Neither record is the whole owning
 * actor. */
struct sdfMemHandle_t;

/* Position/color arrays precede this render pool; constructors clear the full allocation. */
struct dds3FlashPolyList_t {
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
}; /* 0x70 */

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

extern effRandState_t dds3EffectBase[];
extern f32 effPCPFlashPolyPipeVertex[];

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(effRandState_t *name);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(effRandState_t *name);
extern sdfGrOt_t *effPCPFlashQuadSurfaces[];
extern u32 effPCPFlashQuadIndices[];
extern u32 effPCPFlashFanIndices[];
extern sdfGrOt_t *effPCPFlashFanSurfaces[];
extern sdfGrOt_t *effPCPFlashFanHSurfaces[];

extern sdf3DPrimPolyParm_t effPCPFlashPolyParams[];
extern sdfGrOt_t *effPCPFlashTriangleSurfaces[];

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */

/* Load axis-angle rotation into VU0 vf28-vf31. */

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat,
                              s32 (*aAllocator)(s32));

extern void sdf3DPrimLinkTm(sdfDrawTag_t *aTag,
                           s32 (*aAllocator)(s32));

extern sdfGrOt_t sdfGrOtTable[85];

/* Flash state surface: SMT3 45, Raidou 66, Raidou 2 67. */
#ifdef VERSION_RAIDOU2
#define effFlashOtNode (&sdfGrOtTable[67])
#else
#define effFlashOtNode (&sdfGrOtTable[66])
#endif

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

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern f32 aViewEye[], aViewTarget[];
extern f32 effPCPFlashPolySphere2Vertex[], effPCPFlashPolySphereVertex[];
extern f32 effPCPFlashPolyRingVertex[], effPCPFlashPolySphereExpVertex[];
extern f32 def_dir[], effPCPFlashPolyRingExp2Vertex[], effPCPFlashPolyCircleVertex[];
extern f32 effPCPFlashPolyRingExpHVertex[], effPCPFlashPolyRingExpVertex[];

typedef struct effFlashOrbitArcBlock_t {
    effPCPFlashPolyRingExp_tag header;
    effPCPFlashPolyRingExpPart_tag parts[1];
} effFlashOrbitArcBlock_t;

dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 face);
void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool);
void *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);
void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex);
dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 face);
void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool);
void *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex);
void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex);
dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 face);
void dds3FlashPolyList2Delete(dds3FlashPolyList_t *hPoly);
void dds3FlashPolyList3Delete(dds3FlashPolyList_t *hPoly);
void dds3FlashPolyListHDelete(dds3FlashPolyList_t *hPoly);
effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Create(effPCPFlashPolySphere2Param_t *pParam);
effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Copy(effPCPFlashPolySphere2Param_t *params);
void effPCPFlashPolySphere2Delete(effPCPFlashPolySphere2_tag *hData);
void effPCPFlashPolySphere2PartSetVC(effPCPFlashPolySphere2_tag *work, s32 index, s32 color);
void effPCPFlashPolySphere2PartSetVertex(effPCPFlashPolySphere2_tag *work, s32 index, void *view);
void effPCPFlashPolySphere2Draw(effPCPFlashPolySphere2_tag *hData);
effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreate(effPCPFlashPolySphereParam_t *src);
effPCPFlashPolySphere_tag *effPCPFlashPolySphereCopy(effPCPFlashPolySphereParam_t *src);
void effPCPFlashPolySpherePartSetVC(effPCPFlashPolySphere_tag *work, s32 index, s32 color);
void effPCPFlashPolySpherePartInit(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySpherePartSetVertex(effPCPFlashPolySphere_tag *work, s32 index, void *view);
void effPCPFlashPolySpherePartRotate(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySphereDraw(effPCPFlashPolySphere_tag *hData);
effPCPFlashPolyRing_tag *effPCPFlashPolyRingCreate(effPCPFlashPolyRingParam_t *pParam);
effPCPFlashPolyRing_tag *effPCPFlashPolyRingCopy(effPCPFlashPolyRingParam_t *params);
void effPCPFlashPolyRingDelete(effPCPFlashPolyRing_tag *hData);
void effPCPFlashPolyRingPartSetVC(effPCPFlashPolyRing_tag *work, s32 index, s32 color);
void effPCPFlashPolyRingPartSetVertex(effPCPFlashPolyRing_tag *work, s32 index);
void effPCPFlashPolyRingPartRotate(effPCPFlashPolyRing_tag *pFlashPoly, s32 number);
void effPCPFlashPolyRingDraw(effPCPFlashPolyRing_tag *hData);
effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCreate(effPCPFlashPolyPipeParam_t *params);
effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCopy(effPCPFlashPolyPipeParam_t *params);
void effPCPFlashPolyPipePartSetVC(effPCPFlashPolyPipe_tag *work, s32 index, u32 colorMultiplier);
void effPCPFlashPolyPipePartSetVertex(effPCPFlashPolyPipe_tag *work, s32 index);
void effPCPFlashPolyPipePartRotate(effPCPFlashPolyPipe_tag *pFlashPoly, s32 number);
void effPCPFlashPolyPipeDraw(effPCPFlashPolyPipe_tag *hData);
effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpHCreate(effPCPFlashPolyRingExpParam_t *pParam);
effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCopy(effPCPFlashPolyRingExpParam_t *params);
void effPCPFlashPolyRingExpPartSetVC(effPCPFlashPolyRingExp_tag *work, s32 index, s32 color);
void effPCPFlashPolyRingExpHPartSetVertex(effPCPFlashPolyRingExpH_tag *work, s32 index);
void effPCPFlashPolyRingExpPartRotate(effPCPFlashPolyRingExp_tag *pFlashPoly, s32 number);
effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreate(effPCPFlashPolySphereExpParam_t *pParam);
effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCopy(effPCPFlashPolySphereExpParam_t *params);
void effPCPFlashPolySphereExpDelete(effPCPFlashPolySphereExp_tag *hData);
void effPCPFlashPolySphereExpPartSetVC(effPCPFlashPolySphereExp_tag *work, s32 index, s32 color);
void effPCPFlashPolySphereExpPartInit(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySphereExpPartSetVertex(effPCPFlashPolySphereExp_tag *work, s32 index, void *view);
void effPCPFlashPolySphereExpPartRotate(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySphereExpDraw(effPCPFlashPolySphereExp_tag *hData);
effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreate(effPCPFlashPolySphere2ExpParam_t *pParam);
effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCopy(effPCPFlashPolySphere2ExpParam_t *params);
void effPCPFlashPolySphere2ExpDelete(effPCPFlashPolySphere2Exp_tag *hData);
void effPCPFlashPolySphere2ExpPartSetVC(effPCPFlashPolySphere2Exp_tag *work, s32 index, s32 color);
void effPCPFlashPolySphere2ExpPartSetVertex(effPCPFlashPolySphere2Exp_tag *work, s32 index, void *view);
void effPCPFlashPolySphere2ExpDraw(effPCPFlashPolySphere2Exp_tag *hData);
effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Create(effFlashRadialStripParam_t *pParam);
effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Copy(effFlashRadialStripParam_t *params);
void effPCPFlashPolySphere3Delete(effPCPFlashPolySphere3_tag *hData);
void effPCPFlashPolySphere3PartSetVC(effPCPFlashPolySphere3_tag *work, s32 index, u32 colorMultiplier);
void effPCPFlashPolySphere3PartInit(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySphere3PartSetVertex(effPCPFlashPolySphere3_tag *work, s32 index, void *view);
void effPCPFlashPolySphere3PartRotate(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook);
void effPCPFlashPolySphere3Draw(effPCPFlashPolySphere3_tag *hData);
effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Create(effPCPFlashPolyRingExp2Param_t *pParam);
effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Copy(effPCPFlashPolyRingExp2Param_t *params);
void effPCPFlashPolyRingExp2PartSetVC(effPCPFlashPolyRingExp2_tag *work, s32 index, s32 color);
void effPCPFlashPolyRingExp2PartSetVertex(effPCPFlashPolyRingExp2_tag *work, s32 index);
void effPCPFlashPolyRingExp2PartRotate(effPCPFlashPolyRingExp2_tag *pFlashPoly, s32 number);
void effPCPFlashPolyRingExp2Draw(effPCPFlashPolyRingExp2_tag *hData);
effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCreate(effPCPFlashPolyCircleParam_t *pParam);
effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCopy(effPCPFlashPolyCircleParam_t *params);
void effPCPFlashPolyCirclePartSetVC(effPCPFlashPolyCircle_tag *work, s32 index, s32 color);
void effPCPFlashPolyCirclePartSetVertex(effPCPFlashPolyCircle_tag *work, s32 index, void *view);
void effPCPFlashPolyCircleDraw(effPCPFlashPolyCircle_tag *hData);
effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpCreate(effPCPFlashPolyRingExpH_tag *pParam);
effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCopy(effPCPFlashPolyRingExpH_tag *params);
void effPCPFlashPolyRingExpHPartSetVC(effPCPFlashPolyRingExpH_tag *work, s32 index, s32 color);
void effPCPFlashPolyRingExpPartSetVertex(effPCPFlashPolyRingExp_tag *work, s32 index);
void effPCPFlashPolyRingExpHPartRotate(effPCPFlashPolyRingExpH_tag *pFlashPoly, s32 number);

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Create(effPCPFlashPolySphere2Param_t *pParam)
{
    sdfMemHandle_t *allocation = sdfMemAllocDirect(pParam->number * sizeof(effPCPFlashPolySphere2Part_tag) + sizeof(effPCPFlashPolySphere2_tag));
    effPCPFlashPolySphere2_tag *work = EE_ADDR_AS(effPCPFlashPolySphere2_tag *, sdfMemLock(allocation));
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, pParam, 0x30);
    work->pPart = (effPCPFlashPolySphere2Part_tag *)(work + 1);
    work->abgr = EFF_NEUTRAL_COLOR;
    work->allocationHandle = allocation;
    work->scale = 1.0f;
    work->frame = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->pPart[i].frame = 0;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1031, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2CreatePCF(void *table)
{
    return effPCPFlashPolySphere2Create((effPCPFlashPolySphere2Param_t *)dds3EffectPCPDataGetData(table, 0));
}

effPCPFlashPolySphere2_tag *effPCPFlashPolySphere2Copy(effPCPFlashPolySphere2Param_t *params)
{
    return effPCPFlashPolySphere2Create(params);
}

void effPCPFlashPolySphere2Delete(effPCPFlashPolySphere2_tag *hData)
{
    dds3FlashPolyList2Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere2SetPosition);

void effPCPFlashPolySphere2SetColor(effPCPFlashPolySphere2_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere2SetScale);

void effPCPFlashPolySphere2PartSetVC(effPCPFlashPolySphere2_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyList2GetVC(work->hPoly, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    vc[2] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
}

void effPCPFlashPolySphere2PartSetVertex(effPCPFlashPolySphere2_tag *work, s32 index, void *view)
{
    effPCPFlashPolySphere2Part_tag *part = &work->pPart[index];
    f32 *quad = (f32 *)dds3FlashPolyList2GetVertex(work->hPoly, index);
    sdfVector4_t base;
    sdfVector4_t size;
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->radius;
    VEC3_SPLAT(&size.x, radius);
    angle = step * (f32)index;
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_STOREVECTOR(&base);
    SetRotateMatrixAxis(angle, (const sdfVector3_t *)view);
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphere2Vertex);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const sdfVector3_t *)view);
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(effPCPFlashPolySphere2Vertex);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolySphere2Draw(effPCPFlashPolySphere2_tag *hData) {
    sdfVector4_t axis;
    s32 particleIndex;
    s32 lifetime;
    s32 particleCount;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    effPCPFlashPolySphere2Part_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    lifetime = SDF_FRAMES(hData->lifetime);
    particleCount = hData->particleCount;
    part = hData->pPart;
    half = lifetime >> 1;
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    tintColor = hData->abgr;
    for (particleIndex = 0; particleIndex < particleCount; particleIndex++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            effPCPFlashPolySphere2PartSetVertex(hData, particleIndex, &axis);
            effPCPFlashPolySphere2PartSetVC(hData, particleIndex, 0);
            if (ramp == 0) {
                part->radius = maxScale;
            } else {
                part->radius = 0.0f;
            }
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = 0;
                }
                color = 0;
                effPCPFlashPolySphere2PartSetVC(hData, particleIndex, color);
            } else if (part->frame > 0) {
                if (ramp == 0) {
                    part->radius = maxScale;
                } else {
                    part->radius = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->radius) {
                        part->radius = maxScale;
                    }
                }
                effPCPFlashPolySphere2PartSetVertex(hData, particleIndex, &axis);
                if (part->frame < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolySphere2PartSetVC(hData, particleIndex, color);
            }
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyList2Draw(handle);
}

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreate(effPCPFlashPolySphereParam_t *src) {
    sdfMemHandle_t *handle = sdfMemAllocDirect(src->number * sizeof(effPCPFlashPolySpherePart_tag) + sizeof(effPCPFlashPolySphere_tag));
    effPCPFlashPolySphere_tag *work = EE_ADDR_AS(effPCPFlashPolySphere_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 particleIndex;

    memcpy(work, src, 0x40);
    work->pPart = (effPCPFlashPolySpherePart_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->scale = 1.0f;
    work->frame = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    range = work->randomRange;
    for (particleIndex = 0; particleIndex < work->particleCount; particleIndex++) {
        work->pPart[particleIndex].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1219, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolySphere_tag *effPCPFlashPolySphereCreatePCF(void *data)
{
    return effPCPFlashPolySphereCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolySphere_tag *effPCPFlashPolySphereCopy(effPCPFlashPolySphereParam_t *src)
{
    return effPCPFlashPolySphereCreate(src);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1229, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCPFlashPolySphereDelete(effPCPFlashPolySphere_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphereSetPosition);

void effPCPFlashPolySphereSetColor(effPCPFlashPolySphere_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphereSetScale);

void effPCPFlashPolySpherePartSetVC(effPCPFlashPolySphere_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolySpherePartInit(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook) {
    effPCPFlashPolySpherePart_tag *part = pFlashPoly->pPart + number;
    sdfVector4_t direction;
    f32 factor;
    f32 scale;

    direction.x = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    direction.y = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    direction.z = (dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f;
    /* Two plain quadword loads, no memory clobber: retail keeps `direction`
     * and `orientation` CSE'd across them. */
    SDF_VU0_LOADVECTOR(&direction);
    SDF_VU0_LOADVECTOR_B(pLook);
    SDF_VU0_OUTERPRODUCT();
        SDF_VU0_NORMALIZE();;
    SDF_VU0_STOREVECTOR(&direction);
    part->position[0] = direction.x;
    part->position[1] = direction.y;
    part->position[2] = direction.z;
    factor = dds3GetRandFloatA(dds3EffectBase) * 0.3f + 0.7f;
    scale = pFlashPoly->maxScale * factor;
    part->radius_max = scale;
    part->radius = scale;
    factor = (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f) * 0.5f;
    part->width_in = pFlashPoly->upSpan * factor;
    part->width_out = pFlashPoly->acrossSpan * factor;
    part->rotate = pFlashPoly->rotationStepRange * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
}

void effPCPFlashPolySpherePartSetVertex(effPCPFlashPolySphere_tag *work, s32 index, void *view)
{
    effPCPFlashPolySpherePart_tag *part = &work->pPart[index];
    f32 *quad = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t center;
    sdfVector4_t scale;
    sdfVector4_t across;
    sdfVector4_t up;
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->radius;
    ratio = size / part->radius_max;
    VEC3_SPLAT(&scale.x, size);
    acrossLen = part->width_out * ratio;
    VEC3_SPLAT(&across.x, acrossLen);
    upLen = part->width_in * ratio;
    VEC3_SPLAT(&up.x, upLen);
    center.x = part->position[0];
    center.y = part->position[1];
    center.z = part->position[2];
    SDF_VU0_LOADVECTOR(&center);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&across);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&across);
    SDF_VU0_LOADVECTOR(&up);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&up);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(&up);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereVertex);
    SDF_VU0_LOADVECTOR_B(&across);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolySpherePartRotate(effPCPFlashPolySphere_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySpherePart_tag *part = &pFlashPoly->pPart[number];
    sdfVector4_t position;

    position.x = part->position[0];
    position.y = part->position[1];
    position.z = part->position[2];
    SetRotateMatrixAxis(SDF_FRAME_STEP(part->rotate), (const sdfVector3_t *)pLook);
    SDF_VU0_LOADVECTOR(&position);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_STOREVECTOR(&position);
    part->position[0] = position.x;
    part->position[1] = position.y;
    part->position[2] = position.z;
}

void effPCPFlashPolySphereDraw(effPCPFlashPolySphere_tag *hData) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    effPCPFlashPolySpherePart_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&axis);
    lifetime = SDF_FRAMES(hData->lifetime);
    count = hData->particleCount;
    part = hData->pPart;
    half = lifetime >> 1;
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            effPCPFlashPolySpherePartInit(hData, index, &axis);
            effPCPFlashPolySpherePartSetVertex(hData, index, &axis);
            effPCPFlashPolySpherePartSetVC(hData, index, 0);
            if (ramp == 0) {
                part->radius = maxScale;
            } else {
                part->radius = 0.0f;
            }
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                }
                color = 0;
                effPCPFlashPolySpherePartSetVC(hData, index, color);
            } else if (part->frame > 0) {
                if (ramp == 0) {
                    part->radius = maxScale;
                } else {
                    part->radius = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->radius) {
                        part->radius = maxScale;
                    }
                }
                effPCPFlashPolySpherePartRotate(hData, index, &axis);
                effPCPFlashPolySpherePartSetVertex(hData, index, &axis);
                if (part->frame < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolySpherePartSetVC(hData, index, color);
            }
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}

effPCPFlashPolyRing_tag *effPCPFlashPolyRingCreate(effPCPFlashPolyRingParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->number * sizeof(effPCPFlashPolyRingExp2Part_tag) + sizeof(effPCPFlashPolyRing_tag));
    effPCPFlashPolyRing_tag *work = EE_ADDR_AS(effPCPFlashPolyRing_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0x48);
    work->parts = (effPCPFlashPolyRingExp2Part_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->frame = 0;
    work->scale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyListCreate(work->particleCount);
    record->scale = 1.0f;
    record->blend = work->drawMode;
    work->hPoly = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
        work->parts[i].rotate = angle;
        angle += step;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1488, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyRing_tag *effPCPFlashPolyRingCreatePCF(void *data)
{
    return effPCPFlashPolyRingCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolyRing_tag *effPCPFlashPolyRingCopy(effPCPFlashPolyRingParam_t *params)
{
    return effPCPFlashPolyRingCreate(params);
}

void effPCPFlashPolyRingDelete(effPCPFlashPolyRing_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingSetPosition);

void effPCPFlashPolyRingSetColor(effPCPFlashPolyRing_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingSetScale);

void effPCPFlashPolyRingPartSetVC(effPCPFlashPolyRing_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolyRingPartSetVertex(effPCPFlashPolyRing_tag *work, s32 index)
{
    effPCPFlashPolyRingExp2Part_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t offset;
    sdfVector4_t unit;
    sdfVector4_t scaleA;
    sdfVector4_t scaleB;
    sdfVector4_t scaleC;
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->length;
    ratio = size / part->length_max;
    VEC3_SPLAT(&scaleA.x, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(&scaleB.x, widthB);
    VEC3_SPLAT(&scaleC.x, widthC);
    unit.x = sdfCos(part->rotate);
    unit.y = 0;
    sinv = sdfSin(part->rotate);
    unit.z = sinv;
    offset.x = unit.x * work->orbitRadius;
    offset.y = 0;
    offset.z = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyRingVertex[0] = unit.x * height;
    effPCPFlashPolyRingVertex[1] = height + -1.0f;
    effPCPFlashPolyRingVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleB);
    SDF_VU0_LOADVECTOR(&scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolyRingPartRotate(effPCPFlashPolyRing_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyRingExp2Part_tag *part;

    part = &pFlashPoly->parts[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->angularStep);
}

void effPCPFlashPolyRingDraw(effPCPFlashPolyRing_tag *hData) {
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    effPCPFlashPolyRingExp2Part_tag *part;
    dds3FlashPolyList_t *handle;

    lifetime = SDF_FRAMES(hData->lifetime);
    count = hData->particleCount;
    part = hData->parts;
    half = lifetime >> 1;
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            part->length_max = maxScale;
            if (ramp == 0) {
                part->length = maxScale;
            } else {
                part->length = 0.0f;
            }
            effPCPFlashPolyRingPartSetVertex(hData, index);
            effPCPFlashPolyRingPartSetVC(hData, index, 0);
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                }
                color = 0;
                effPCPFlashPolyRingPartSetVC(hData, index, color);
            } else if (part->frame > 0) {
                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->length) {
                        part->length = maxScale;
                    }
                }
                effPCPFlashPolyRingPartRotate(hData, index);
                effPCPFlashPolyRingPartSetVertex(hData, index);
                if (part->frame < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolyRingPartSetVC(hData, index, color);
            }
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}

effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCreate(effPCPFlashPolyPipeParam_t *params)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(params->particleCount * sizeof(effPCPFlashPolyPipePart_tag) + sizeof(effPCPFlashPolyPipe_tag));
    effPCPFlashPolyPipe_tag *work = EE_ADDR_AS(effPCPFlashPolyPipe_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, params, 0x50);
    work->parts = (effPCPFlashPolyPipePart_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->frame = 0;
    work->scale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyList3Create(work->particleCount * 2);
    record->scale = 1.0f;
    record->blend = work->drawMode;
    work->hPoly = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
        work->parts[i].rotate = angle;
        angle += step;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1727, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCreatePCF(void *data)
{
    return effPCPFlashPolyPipeCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolyPipe_tag *effPCPFlashPolyPipeCopy(effPCPFlashPolyPipeParam_t *params)
{
    return effPCPFlashPolyPipeCreate(params);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1737, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCPFlashPolyPipeDelete(effPCPFlashPolyPipe_tag *hData)
{
    dds3FlashPolyList3Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyPipeSetPosition);

void effPCPFlashPolyPipeSetColor(effPCPFlashPolyPipe_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyPipeSetScale);

void effPCPFlashPolyPipePartSetVC(effPCPFlashPolyPipe_tag *work, s32 index, u32 colorMultiplier) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    colors[0] = effMagaCylinderCalcColor(colorB | 0x80000000, colorMultiplier);
    colors[1] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    colors[2] = effMagaCylinderCalcColor(colorA | 0x80000000, colorMultiplier);
    colors[3] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    mirror = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

/* vu0 routine: the orbit offset plus a tilted disc of corner offsets for an
   accumulating particle, whose radius grows by the work record's radial step */
void effPCPFlashPolyPipePartSetVertex(effPCPFlashPolyPipe_tag *work, s32 index)
{
    effPCPFlashPolyPipePart_tag *part = &work->parts[index];
    f32 *quad = dds3FlashPolyList3GetVertex(work->hPoly, index * 2);
    sdfVector4_t offset;
    sdfVector4_t unit;
    sdfVector4_t middle;
    sdfVector4_t outer;
    sdfVector4_t inner;
    sdfVector4_t size;
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 sinv;
    f32 height;
    f32 *mirror;

    center = part->up;
    VEC3_SPLAT(&middle.x, center);
    outerEdge = center + part->length;
    VEC3_SPLAT(&outer.x, outerEdge);
    innerEdge = center - part->length;
    VEC3_SPLAT(&inner.x, innerEdge);
    part->up = part->up + work->radialStep;
    span = part->width_in;
    VEC3_SPLAT(&size.x, span);
    unit.x = sdfCos(part->rotate);
    unit.y = 0;
    sinv = sdfSin(part->rotate);
    unit.z = sinv;
    offset.x = unit.x * work->orbitRadius;
    offset.y = 0;
    offset.z = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyPipeVertex[0] = unit.x * height;
    effPCPFlashPolyPipeVertex[1] = height + -1.0f;
    effPCPFlashPolyPipeVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyPipeVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&size);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&outer);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&outer);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&inner);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&inner);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&middle);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&outer);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    mirror = dds3FlashPolyList3GetVertex(work->hPoly, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&inner);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(mirror);
}

void effPCPFlashPolyPipePartRotate(effPCPFlashPolyPipe_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyPipePart_tag *part;

    part = &pFlashPoly->parts[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->increment);
}

void effPCPFlashPolyPipeDraw(effPCPFlashPolyPipe_tag *hData)
{
    s32 index;
    s32 count;
    effPCPFlashPolyPipePart_tag *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;

    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    randomRange = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 factor;

        if (age == 0) {
            effPCPFlashPolyPipePartSetVertex(hData, index);
            effPCPFlashPolyPipePartSetVC(hData, index, 0);
            part->abgr = EFF_NEUTRAL_COLOR;
            factor = dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f;
            part->length = hData->radialHalfThicknessBase * factor;
            factor = dds3GetRandFloatA(dds3EffectBase) * 0.7f + 0.3f;
            part->width_in = hData->spanBase * factor;
            part->width_out = hData->unk34 * factor;
            part->up = 0;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % randomRange);
            }
            color = 0;
            effPCPFlashPolyPipePartSetVC(hData, index, color);
        } else if (age > 0) {
            effPCPFlashPolyPipePartRotate(hData, index);
            effPCPFlashPolyPipePartSetVertex(hData, index);
            if (part->frame < fadeIn && fadeIn != 0) {
                factor = (f32)part->frame / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->frame && fadeOut != 0) {
                factor = (f32)(lifetime - part->frame) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, factor), tintColor);
            effPCPFlashPolyPipePartSetVC(hData, index, color);
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyList3Draw(handle);
}

effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpHCreate(effPCPFlashPolyRingExpParam_t *pParam)
{
    sdfMemHandle_t *handle;
    effFlashOrbitArcBlock_t *block;
    effPCPFlashPolyRingExp_tag *ring;
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = sdfMemAllocDirect((u32)pParam->number * 16 + 0x80);
    block = EE_ADDR_AS(effFlashOrbitArcBlock_t *, sdfMemLock(handle));
    ring = &block->header;
    memcpy(ring, pParam, 0x58);
    ring->pPart = block->parts;
    ring->allocationHandle = handle;
    ring->abgr = EFF_NEUTRAL_COLOR;
    ring->radius = ring->orbitRadiusStart;
    ring->width_in = ring->upSpanStart;
    ring->width_out = ring->acrossSpanStart;
    ring->frame = 0;
    ring->scale = 1.0f;
    if (ring->randomRange == 0) {
        ring->randomRange = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    record = dds3FlashPolyListCreate(ring->particleCount);
    record->scale = 1.0f;
    record->blend = ring->drawMode;
    ring->hPoly = record;
    step = EFFECT_RING_FULL_TURN / (u32)ring->particleCount;
    spread = ring->randomRange;
    for (i = 0; i < (u32)ring->particleCount; i++) {
        ring->pPart[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % spread);
        ring->pPart[i].accumulator = angle;
        angle += step;
    }
    return ring;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1978, near, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCreatePCF(void *data)
{
    return effPCPFlashPolyRingExpHCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolyRingExp_tag *effPCPFlashPolyRingExpCopy(effPCPFlashPolyRingExpParam_t *params)
{
    return effPCPFlashPolyRingExpHCreate(params);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:1988, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpDelete(effPCPFlashPolyRingExp_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExpSetPosition);

void effPCPFlashPolyRingExpHSetColor(effPCPFlashPolyRingExpH_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExpSetScale);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2008, match, same function 1.00 (words 60/60, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpPartSetVC(effPCPFlashPolyRingExp_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolyRingExpHPartSetVertex(effPCPFlashPolyRingExpH_tag *work, s32 index) {
    effPCPFlashPolyRingExpHPart_tag *part = &work->pPart[index];
    f32 *positions = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t orbitOffset;
    sdfVector4_t radialDirection;
    sdfVector4_t axisOffset;
    sdfVector4_t sideOffsetA;
    sdfVector4_t sideOffsetB;
    f32 sine;
    f32 basisFactor;

    VEC3_SPLAT(&axisOffset.x, work->length);
    VEC3_SPLAT(&sideOffsetA.x, work->currentSideOffsetA);
    VEC3_SPLAT(&sideOffsetB.x, work->currentSideOffsetB);
    radialDirection.x = sdfCos(part->rotate);
    radialDirection.y = 0;
    sine = sdfSin(part->rotate);
    radialDirection.z = sine;
    orbitOffset.x = radialDirection.x * work->radius;
    orbitOffset.y = 0;
    orbitOffset.z = sine * work->radius;
    basisFactor = part->spread;
    effPCPFlashPolyRingExpHVertex[0] = radialDirection.x * basisFactor;
    effPCPFlashPolyRingExpHVertex[1] = basisFactor + -1.0f;
    effPCPFlashPolyRingExpHVertex[2] = sine * basisFactor;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExpHVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&radialDirection);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&sideOffsetA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&sideOffsetA);
    SDF_VU0_LOADVECTOR(&sideOffsetB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&sideOffsetB);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&axisOffset);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&orbitOffset);
    SDF_VU0_LOADVECTOR_B(&sideOffsetB);
    SDF_VU0_STOREVECTOR(positions + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(positions + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(positions + 16);
    SDF_VU0_LOADVECTOR(&orbitOffset);
    SDF_VU0_LOADVECTOR_B(&sideOffsetA);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(positions);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(positions + 4);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2093, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpPartRotate(effPCPFlashPolyRingExp_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyRingExpPart_tag *part;

    part = &pFlashPoly->pPart[number];
    part->accumulator += SDF_FRAME_STEP(pFlashPoly->angularStep);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2101, near, same function 0.88 (words 214/214, calls 0.62, shape 1.00) */
void effPCPFlashPolyRingExpHDraw(effPCPFlashPolyRingExp_tag *hData)
{
    s32 index;
    s32 count;
    effPCPFlashPolyRingExpPart_tag *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;
    f32 heightDelta;
    f32 delta;
    f32 progress;
    f32 factor;
    f32 start;
    f32 radiusEnd;
    f32 upStart;
    f32 acrossStart;
    f32 upEnd;
    f32 acrossEnd;
    f32 normalEnd;

    part = hData->pPart;
    count = hData->particleCount;
    lifetime = SDF_FRAMES(hData->lifetime);
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    randomRange = hData->randomRange;
    tintColor = hData->abgr;
    heightDelta = hData->heightEnd - hData->heightStart;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)hData->frame / (f32)lifetime;
    }
    start = hData->orbitRadiusStart;
    radiusEnd = hData->orbitRadiusEnd;
    delta = radiusEnd - start;
    hData->radius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = hData->upSpanStart;
    acrossStart = hData->acrossSpanStart;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = hData->normalSpanStart;
    normalEnd = hData->normalSpanEnd;
    delta = upEnd - upStart;
    hData->width_in = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    hData->width_out = acrossStart + delta * progress;
    delta = normalEnd - start;
    hData->length = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;

        if (age == 0) {
            part->stepSpeed = hData->heightStart;
            effPCPFlashPolyRingExpHPartSetVertex((effPCPFlashPolyRingExpH_tag *)hData, index);
            effPCPFlashPolyRingExpPartSetVC(hData, index, 0);
            part->color = EFF_NEUTRAL_COLOR;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->frame = ~(SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % randomRange + 1) - 1);
            }
            color = 0;
            effPCPFlashPolyRingExpPartSetVC(hData, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->stepSpeed = heightDelta * progress + hData->heightStart;
            effPCPFlashPolyRingExpPartRotate(hData, index);
            effPCPFlashPolyRingExpHPartSetVertex((effPCPFlashPolyRingExpH_tag *)hData, index);
            if (part->frame < fadeIn && fadeIn != 0) {
                factor = (f32)part->frame / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->frame && fadeOut != 0) {
                factor = (f32)(lifetime - part->frame) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->color, factor), tintColor);
            effPCPFlashPolyRingExpPartSetVC(hData, index, color);
        }
        part->frame = part->frame + 1;
    }
    if (hData->frame == lifetime) {
        hData->frame = 0;
    } else {
        hData->frame++;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreate(effPCPFlashPolySphereExpParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolySphereExpPart_tag) + sizeof(effPCPFlashPolySphereExp_tag));
    effPCPFlashPolySphereExp_tag *work = EE_ADDR_AS(effPCPFlashPolySphereExp_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0x4C);
    work->parts = (effPCPFlashPolySphereExpPart_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->scale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyListCreate(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2232, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCreatePCF(void *data)
{
    return effPCPFlashPolySphereExpCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolySphereExp_tag *effPCPFlashPolySphereExpCopy(effPCPFlashPolySphereExpParam_t *params)
{
    return effPCPFlashPolySphereExpCreate(params);
}

void effPCPFlashPolySphereExpDelete(effPCPFlashPolySphereExp_tag *hData)
{
    dds3FlashPolyListDelete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphereExpSetPosition);

void effPCPFlashPolySphereExpSetColor(effPCPFlashPolySphereExp_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphereExpSetScale);

void effPCPFlashPolySphereExpPartSetVC(effPCPFlashPolySphereExp_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolySphereExpPartInit(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook) {
    effPCPFlashPolySphereExpPart_tag *part = pFlashPoly->parts + number;
    f32 factor;
    f32 scale;

    part->look_r = dds3GetRandFloatA(dds3EffectBase) * 6.2831853f;
    factor = dds3GetRandFloatA(dds3EffectBase) * 0.3f + 0.7f;
    scale = pFlashPoly->maxScale * factor;
    part->radius_max = scale;
    part->radius = scale;
    factor = (dds3GetRandFloatA(dds3EffectBase) * 0.5f + 0.5f) * 0.5f;
    part->width_in = pFlashPoly->upSpan * factor;
    part->width_out = pFlashPoly->acrossSpan * factor;
    part->rotate = pFlashPoly->angularStepRange * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
}

void effPCPFlashPolySphereExpPartSetVertex(effPCPFlashPolySphereExp_tag *work, s32 index, void *view)
{
    effPCPFlashPolySphereExpPart_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t base;
    sdfVector4_t scale;
    sdfVector4_t across;
    sdfVector4_t up;
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->radius;
    ratio = size / part->radius_max;
    VEC3_SPLAT(&scale.x, size);
    acrossLen = part->width_out * ratio;
    VEC3_SPLAT(&across.x, acrossLen);
    upLen = part->width_in * ratio;
    VEC3_SPLAT(&up.x, upLen);
    SetRotateMatrixAxis(part->look_r, (const sdfVector3_t *)view);
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&across);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&across);
    SDF_VU0_LOADVECTOR(&up);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&up);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scale);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereExpVertex);
    SDF_VU0_LOADVECTOR_B(&up);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(effPCPFlashPolySphereExpVertex);
    SDF_VU0_LOADVECTOR_B(&across);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolySphereExpPartRotate(effPCPFlashPolySphereExp_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySphereExpPart_tag *part;

    part = &pFlashPoly->parts[number];
    part->look_r += SDF_FRAME_STEP(part->rotate);
}

void effPCPFlashPolySphereExpDraw(effPCPFlashPolySphereExp_tag *hData) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 ramp;
    s32 fadeIn;
    s32 fadeOut;
    f32 maxScale;
    s32 restart;
    s32 tintColor;
    u32 range;
    effPCPFlashPolySphereExpPart_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;
        f32 blend;

        if (part->frame == 0) {
            effPCPFlashPolySphereExpPartInit(hData, index, &axis);
            effPCPFlashPolySphereExpPartSetVertex(hData, index, &axis);
            effPCPFlashPolySphereExpPartSetVC(hData, index, 0);
            if (ramp == 0) {
                part->radius = maxScale;
            } else {
                part->radius = 0.0f;
            }
            part->abgr = EFF_NEUTRAL_COLOR;
        } else {
            if (part->frame >= lifetime) {
                if (restart != 0) {
                    part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                }
                color = 0;
                effPCPFlashPolySphereExpPartSetVC(hData, index, color);
            } else if (part->frame > 0) {
                s32 remain;

                if (ramp == 0) {
                    part->radius = maxScale;
                } else {
                    part->radius = (maxScale * (f32)part->frame) / (f32)ramp;
                    if (maxScale < part->radius) {
                        part->radius = maxScale;
                    }
                }
                effPCPFlashPolySphereExpPartRotate(hData, index, &axis);
                effPCPFlashPolySphereExpPartSetVertex(hData, index, &axis);
                age = part->frame;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor);
                effPCPFlashPolySphereExpPartSetVC(hData, index, color);
            }
        }
        part->frame = part->frame + 1;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->updateCount = hData->updateCount + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreate(effPCPFlashPolySphere2ExpParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolyRingExpPart_tag) + sizeof(effPCPFlashPolySphere2Exp_tag));
    effPCPFlashPolySphere2Exp_tag *work = EE_ADDR_AS(effPCPFlashPolySphere2Exp_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, pParam, 0x3C);
    work->parts = (effPCPFlashPolyRingExpPart_tag *)(work + 1);
    work->abgr = EFF_NEUTRAL_COLOR;
    work->allocationHandle = handle;
    work->scale = 1.0f;
    work->updateCount = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = 0;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2482, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCreatePCF(void *data)
{
    return effPCPFlashPolySphere2ExpCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolySphere2Exp_tag *effPCPFlashPolySphere2ExpCopy(effPCPFlashPolySphere2ExpParam_t *params)
{
    return effPCPFlashPolySphere2ExpCreate(params);
}

void effPCPFlashPolySphere2ExpDelete(effPCPFlashPolySphere2Exp_tag *hData)
{
    dds3FlashPolyList2Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere2ExpSetPosition);

void effPCPFlashPolySphere2ExpSetColor(effPCPFlashPolySphere2Exp_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere2ExpSetScale);

void effPCPFlashPolySphere2ExpPartSetVC(effPCPFlashPolySphere2Exp_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyList2GetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    vc[2] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
}

void effPCPFlashPolySphere2ExpPartSetVertex(effPCPFlashPolySphere2Exp_tag *work, s32 index, void *view)
{
    effPCPFlashPolyRingExpPart_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyList2GetVertex(work->hPoly, index);
    sdfVector4_t base;
    sdfVector4_t size;
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(&size.x, radius);
    angle = step * (f32)index;
    SetRotateMatrixAxis(angle, (const sdfVector3_t *)view);
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(def_dir);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const sdfVector3_t *)view);
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(def_dir);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolySphere2ExpDraw(effPCPFlashPolySphere2Exp_tag *hData) {
    s128 axis;
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    effPCPFlashPolyRingExpPart_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    startA = hData->initialRadius;
    startB = hData->initialRadialSpeed;
    decay = hData->radialDamping;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    tintColor = hData->abgr;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolySphere2ExpPartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere2ExpPartSetVertex(hData, index, &axis);
                effPCPFlashPolySphere2ExpPartSetVC(hData, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * SDF_FRAME_DECAY(decay);
                part->accumulator = part->accumulator + SDF_FRAME_STEP(speed);
                effPCPFlashPolySphere2ExpPartSetVertex(hData, index, &axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effPCPFlashPolySphere2ExpPartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = 0;
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->updateCount = hData->updateCount + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    if (active != 0) {
        dds3FlashPolyList2Draw(handle);
    }
}

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Create(effFlashRadialStripParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolySphere3Part_tag) + sizeof(effPCPFlashPolySphere3_tag));
    effPCPFlashPolySphere3_tag *work = EE_ADDR_AS(effPCPFlashPolySphere3_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0xD0);
    work->parts = (effPCPFlashPolySphere3Part_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->updateCount = 0;
    work->scale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = dds3FlashPolyList3Create(work->particleCount * 2);
    record->scale = 1.0f;
    record->blend = work->drawMode;
    work->hPoly = record;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2678, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3CreatePCF(void *data)
{
    return effPCPFlashPolySphere3Create(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolySphere3_tag *effPCPFlashPolySphere3Copy(effFlashRadialStripParam_t *params)
{
    return effPCPFlashPolySphere3Create(params);
}

void effPCPFlashPolySphere3Delete(effPCPFlashPolySphere3_tag *hData)
{
    dds3FlashPolyList3Delete(hData->hPoly);
    sdfMemFreeDirect(hData->allocationHandle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere3SetPosition);

void effPCPFlashPolySphere3SetColor(effPCPFlashPolySphere3_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolySphere3SetScale);

void effPCPFlashPolySphere3PartSetVC(effPCPFlashPolySphere3_tag *work, s32 index, u32 colorMultiplier) {
    u32 *colors;
    u32 colorA;
    u32 colorB;
    u32 *mirror;

    colors = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2);
    colorA = work->centerColor & 0xFFFFFF;
    colorB = work->edgeColor & 0xFFFFFF;
    colors[0] = effMagaCylinderCalcColor(colorB | 0x80000000, colorMultiplier);
    colors[1] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    colors[2] = effMagaCylinderCalcColor(colorA | 0x80000000, colorMultiplier);
    colors[3] = effMagaCylinderCalcColor(colorB, colorMultiplier);
    mirror = (u32 *)dds3FlashPolyList3GetVC(work->hPoly, index * 2 + 1);
    mirror[0] = colors[0];
    mirror[1] = colors[1];
    mirror[2] = colors[2];
    mirror[3] = colors[3];
}

void effPCPFlashPolySphere3PartInit(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook) {
    effPCPFlashPolySphere3Part_tag *part = pFlashPoly->parts + number;
    f32 factor;

    part->look_r = dds3GetRandFloatA(dds3EffectBase) * 6.2831853f;
    factor = dds3GetRandFloatA(dds3EffectBase) * 0.3f + 0.7f;
    part->radius = pFlashPoly->maxThickness * factor;
    factor = pFlashPoly->spanVariation;
    part->width = pFlashPoly->spanBase * (dds3GetRandFloatA(dds3EffectBase) * factor + (1.0f - factor));
    part->rotate = pFlashPoly->angularStepRange * ((dds3GetRandFloatA(dds3EffectBase) - 0.5f) * 2.0f);
}

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effPCPFlashPolySphere3PartSetVertex(effPCPFlashPolySphere3_tag *work, s32 index, void *view)
{
    effPCPFlashPolySphere3Part_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyList3GetVertex(work->hPoly, index * 2);
    sdfVector4_t base;
    sdfVector4_t size;
    sdfVector4_t spare;
    sdfVector4_t middle;
    sdfVector4_t outer;
    sdfVector4_t inner;
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 *mirror;

    center = part->move;
    VEC3_SPLAT(&middle.x, center);
    outerEdge = center + part->radius;
    VEC3_SPLAT(&outer.x, outerEdge);
    innerEdge = center - part->radius;
    VEC3_SPLAT(&inner.x, innerEdge);
    span = part->width;
    VEC3_SPLAT(&size.x, span);
    SetRotateMatrixAxis(part->look_r, (const sdfVector3_t *)view);
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_COPYVECTOR_A_TO_C();
    /* retail multiplies the (never written) `spare` slot here and stores it back */
    SDF_VU0_LOADVECTOR_B(&spare);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&spare);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&size);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&outer);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&outer);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&inner);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&inner);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&middle);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_LOADVECTOR(&outer);
    SDF_VU0_STOREVECTOR(quad);
    mirror = (f32 *)dds3FlashPolyList3GetVertex(work->hPoly, index * 2 + 1);
    SDF_VU0_COPYVECTOR(mirror + 8, quad + 8);
    SDF_VU0_COPYVECTOR(mirror + 4, quad + 4);
    SDF_VU0_COPYVECTOR(mirror + 12, quad + 12);
    SDF_VU0_LOADVECTOR(&inner);
    SDF_VU0_STOREVECTOR(mirror);
}

void effPCPFlashPolySphere3PartRotate(effPCPFlashPolySphere3_tag *pFlashPoly, s32 number, void *pLook)
{
    effPCPFlashPolySphere3Part_tag *part;

    part = &pFlashPoly->parts[number];
    part->look_r += SDF_FRAME_STEP(part->rotate);
}

void effPCPFlashPolySphere3Draw(effPCPFlashPolySphere3_tag *hData) {
    s128 axis;
    s32 index;
    s32 count;
    s32 lifetime;
    f32 maxThickness;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    s32 restart;
    u32 range;
    s32 tintColor;
    effPCPFlashPolySphere3Part_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    maxThickness = hData->maxThickness;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    startA = hData->initialRadius;
    startB = hData->initialRadialSpeed;
    decay = hData->radialDamping;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolySphere3PartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolySphere3PartInit(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVC(hData, index, 0);
                part->radius = maxThickness;
                part->move = startA;
                part->move_s = startB;
                part->abgr = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                f32 speed = part->move_s;
                s32 remain;
                f32 blend;

                part->move_s = speed * SDF_FRAME_DECAY(decay);
                part->move = part->move + SDF_FRAME_STEP(speed);
                effPCPFlashPolySphere3PartRotate(hData, index, &axis);
                effPCPFlashPolySphere3PartSetVertex(hData, index, &axis);
                age = part->frame;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effPCPFlashPolySphere3PartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                effPCPFlashPolySphere3PartSetVC(hData, index, 0);
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->updateCount = hData->updateCount + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyList3Draw(handle);
}

effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Create(effPCPFlashPolyRingExp2Param_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->number * sizeof(effPCPFlashPolyRingExp2Part_tag) + sizeof(effPCPFlashPolyRingExp2_tag));
    effPCPFlashPolyRingExp2_tag *work = EE_ADDR_AS(effPCPFlashPolyRingExp2_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, pParam, 0x50);
    work->pPart = (effPCPFlashPolyRingExp2Part_tag *)(work + 1);
    work->allocationHandle = handle;
    work->abgr = EFF_NEUTRAL_COLOR;
    work->frame = 0;
    work->scale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = dds3FlashPolyListCreate(work->particleCount);
    record->scale = 1.0f;
    record->blend = work->drawMode;
    work->hPoly = record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->pPart[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
        work->pPart[i].rotate = angle;
        angle += step;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:2943, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2CreatePCF(void *data)
{
    return effPCPFlashPolyRingExp2Create(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolyRingExp2_tag *effPCPFlashPolyRingExp2Copy(effPCPFlashPolyRingExp2Param_t *params)
{
    return effPCPFlashPolyRingExp2Create(params);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFunc9);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExp2SetPosition);

void effPCPFlashPolyRingExp2SetColor(effPCPFlashPolyRingExp2_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExp2SetScale);

void effPCPFlashPolyRingExp2PartSetVC(effPCPFlashPolyRingExp2_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyListGetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
        vc[4] = effMagaCylinderCalcColor(0x80000000, color);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, color);
        vc[3] = effMagaCylinderCalcColor(rgb1 | 0x40000000, color);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, color);
    }
}

void effPCPFlashPolyRingExp2PartSetVertex(effPCPFlashPolyRingExp2_tag *work, s32 index)
{
    effPCPFlashPolyRingExp2Part_tag *part = &work->pPart[index];
    f32 *quad = (f32 *)dds3FlashPolyListGetVertex(work->hPoly, index);
    sdfVector4_t offset;
    sdfVector4_t unit;
    sdfVector4_t scaleA;
    sdfVector4_t scaleB;
    sdfVector4_t scaleC;
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->length;
    ratio = size / part->length_max;
    VEC3_SPLAT(&scaleA.x, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(&scaleB.x, widthB);
    VEC3_SPLAT(&scaleC.x, widthC);
    unit.x = sdfCos(part->rotate);
    unit.y = 0;
    sinv = sdfSin(part->rotate);
    unit.z = sinv;
    offset.x = unit.x * work->orbitRadius;
    offset.y = 0;
    offset.z = sinv * work->orbitRadius;
    height = work->tilt;
    effPCPFlashPolyRingExp2Vertex[0] = unit.x * height;
    effPCPFlashPolyRingExp2Vertex[1] = height + -1.0f;
    effPCPFlashPolyRingExp2Vertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExp2Vertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleB);
    SDF_VU0_LOADVECTOR(&scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolyRingExp2PartRotate(effPCPFlashPolyRingExp2_tag *pFlashPoly, s32 number)
{
    effPCPFlashPolyRingExp2Part_tag *part;

    part = &pFlashPoly->pPart[number];
    part->rotate += SDF_FRAME_STEP(pFlashPoly->angularStep);
}

void effPCPFlashPolyRingExp2Draw(effPCPFlashPolyRingExp2_tag *hData) {
    s32 restart;
    s32 tintColor;
    s32 count;
    s32 index;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    f32 maxScale;
    u32 range;
    effPCPFlashPolyRingExp2Part_tag *part;
    dds3FlashPolyList_t *handle;

    count = hData->particleCount;
    part = hData->pPart;
    lifetime = SDF_FRAMES(hData->lifetime);
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    ramp = SDF_FRAMES(hData->scaleRampTime);
    maxScale = hData->maxScale;
    restart = hData->restartRandomly;
    range = hData->randomRange;
    tintColor = hData->abgr;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                part->length_max = maxScale;
                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = 0.0f;
                }
                effPCPFlashPolyRingExp2PartSetVertex(hData, index);
                effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
                part->abgr = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                s32 remain;
                f32 blend;

                if (ramp == 0) {
                    part->length = maxScale;
                } else {
                    part->length = (maxScale * (f32)age) / (f32)ramp;
                    if (maxScale < part->length) {
                        part->length = maxScale;
                    }
                }
                effPCPFlashPolyRingExp2PartRotate(hData, index);
                effPCPFlashPolyRingExp2PartSetVertex(hData, index);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effPCPFlashPolyRingExp2PartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->abgr, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = ~SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % range);
                effPCPFlashPolyRingExp2PartSetVC(hData, index, 0);
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    hData->frame = hData->frame + 1;
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListDraw(handle);
}

effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCreate(effPCPFlashPolyCircleParam_t *pParam)
{
    sdfMemHandle_t *handle = sdfMemAllocDirect(pParam->particleCount * sizeof(effPCPFlashPolyRingExpPart_tag) + sizeof(effPCPFlashPolyCircle_tag));
    effPCPFlashPolyCircle_tag *work = EE_ADDR_AS(effPCPFlashPolyCircle_tag *, sdfMemLock(handle));
    dds3FlashPolyList_t *record;
    u32 i;

    memcpy(work, pParam, 0x40);
    work->parts = (effPCPFlashPolyRingExpPart_tag *)(work + 1);
    work->abgr = EFF_NEUTRAL_COLOR;
    work->allocationHandle = handle;
    work->scale = 1.0f;
    work->frame = 0;
    record = dds3FlashPolyList2Create(work->particleCount);
    work->hPoly = record;
    record->blend = work->drawMode;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].frame = 0;
    }
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3178, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCreatePCF(void *data)
{
    return effPCPFlashPolyCircleCreate(dds3EffectPCPDataGetData(data, 0));
}

effPCPFlashPolyCircle_tag *effPCPFlashPolyCircleCopy(effPCPFlashPolyCircleParam_t *params)
{
    return effPCPFlashPolyCircleCreate(params);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFunc10);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyCircleSetPosition);

void effPCPFlashPolyCircleSetColor(effPCPFlashPolyCircle_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyCircleSetScale);

void effPCPFlashPolyCirclePartSetVC(effPCPFlashPolyCircle_tag *work, s32 index, s32 color)
{
    u32 *vc;
    s32 rgb1;
    s32 rgb2;

    vc = (u32 *)dds3FlashPolyList2GetVC(work->hPoly, index);
    rgb1 = work->centerColor & 0xFFFFFF;
    rgb2 = work->edgeColor & 0xFFFFFF;
    vc[0] = effMagaCylinderCalcColor(rgb2, color);
    vc[1] = effMagaCylinderCalcColor(rgb2, color);
    vc[2] = effMagaCylinderCalcColor(rgb1 | 0xFF000000, color);
}

void effPCPFlashPolyCirclePartSetVertex(effPCPFlashPolyCircle_tag *work, s32 index, void *view)
{
    effPCPFlashPolyRingExpPart_tag *part = &work->parts[index];
    f32 *quad = (f32 *)dds3FlashPolyList2GetVertex(work->hPoly, index);
    sdfVector4_t base;
    sdfVector4_t size;
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(&size.x, radius);
    angle = step * (f32)index;
    SetRotateMatrixAxis(angle, (const sdfVector3_t *)view);
    base.x = 0;
    base.y = 1.0f;
    base.z = 0;
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_LOADVECTOR_B(view);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(effPCPFlashPolyCircleVertex);
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SetRotateMatrixAxis(angle + step, (const sdfVector3_t *)view);
    SDF_VU0_LOADVECTOR(&base);
    SDF_VU0_APPLYMATRIX_XYZ();
    SDF_VU0_LOADVECTOR_B(&size);
    SDF_VU0_MULVECTOR();
    SDF_VU0_LOADVECTOR_B(effPCPFlashPolyCircleVertex);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

void effPCPFlashPolyCircleDraw(effPCPFlashPolyCircle_tag *hData) {
    sdfVector4_t axis;
    s32 restart;
    s32 tintColor;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    f32 scale;
    effPCPFlashPolyRingExpPart_tag *part;
    dds3FlashPolyList_t *handle;

    SDF_VU0_LOADVECTOR(aViewTarget);
    SDF_VU0_LOADVECTOR_B(aViewEye);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
    SDF_VU0_STOREVECTOR(&axis);
    count = hData->particleCount;
    part = hData->parts;
    lifetime = SDF_FRAMES(hData->lifetime);
    startA = hData->initialRadius;
    startB = hData->initialRadialSpeed;
    decay = hData->radialDamping;
    fadeIn = SDF_FRAMES(hData->fadeInTime);
    fadeOut = SDF_FRAMES(hData->fadeOutTime);
    restart = hData->restartRandomly;
    tintColor = hData->abgr;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;

        if (lifetime < age) {
            effPCPFlashPolyCirclePartSetVC(hData, index, 0);
        } else {
            if (age == 0) {
                effPCPFlashPolyCirclePartSetVertex(hData, index, &axis);
                effPCPFlashPolyCirclePartSetVC(hData, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = EFF_NEUTRAL_COLOR;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * SDF_FRAME_DECAY(decay);
                part->accumulator = part->accumulator + SDF_FRAME_STEP(speed);
                effPCPFlashPolyCirclePartSetVertex(hData, index, &axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effPCPFlashPolyCirclePartSetVC(hData, index, effMagaCylinderCalcColor(effColorBlend(0, part->color, blend), tintColor));
            }
            if (age == lifetime && restart != 0) {
                part->frame = 0;
            } else {
                part->frame = part->frame + 1;
            }
        }
    }
    scale = hData->originOffset * hData->scale;
    handle = hData->hPoly;
    hData->frame = hData->frame + 1;
    handle->pos[0] = hData->origin[0] + axis.x * scale;
    handle->pos[1] = hData->origin[1] + axis.y * scale;
    handle->pos[2] = hData->origin[2] + axis.z * scale;
    handle->scale = hData->scale;
    if (active != 0) {
        dds3FlashPolyList2Draw(handle);
    }
}

effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpCreate(effPCPFlashPolyRingExpH_tag *pParam)
{
    sdfMemHandle_t *allocation;
    effPCPFlashPolyRingExpH_tag *ring;
    effRingBlock_t *block;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    allocation = sdfMemAllocDirect(pParam->count * EFF_RING_VERTEX_BYTES + EFF_RING_HEADER_BYTES);
    block = EE_ADDR_AS(effRingBlock_t *, sdfMemLock(allocation));
    ring = &block->header;
    memcpy(ring, pParam, EFF_RING_PARAM_BYTES);
    ring->pPart = &block->vertices[0];
    ring->allocationHandle = allocation;
    ring->abgr = EFF_NEUTRAL_COLOR;
    ring->radius = ring->param38;
    ring->currentSideOffsetB = ring->param30;
    ring->currentSideOffsetA = ring->param34;
    ring->frame = 0;
    ring->scale = 1.0f;
    if (ring->spread == 0) {
        ring->spread = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    ring->hPoly = dds3FlashPolyListHCreate(ring->count);
    ring->hPoly->scale = 1.0f;
    ring->hPoly->blend = ring->drawMode;
    step = EFFECT_RING_FULL_TURN / ring->count;
    spread = ring->spread;
    for (i = 0; i < ring->count; i++) {
        ring->pPart[i].frame = -SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % spread);
        ring->pPart[i].rotate = angle;
        angle += step;
    }
    return ring;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3387, near, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCreatePCF(void *table) {
    void *parameterBlock;

    parameterBlock = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingExpCreate(parameterBlock);
}

effPCPFlashPolyRingExpH_tag *effPCPFlashPolyRingExpHCopy(effPCPFlashPolyRingExpH_tag *params)
{
    return effPCPFlashPolyRingExpCreate(params);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effCallFlashPolyListHDelete);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExpHSetPosition);

void effPCPFlashPolyRingExpSetColor(effPCPFlashPolyRingExp_tag *hData, u32 abgr)
{
    hData->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effPCPFlashPolyRingExpHSetScale);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3418, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpHSetRotateMatrix(effPCPFlashPolyRingExpH_tag *hData, void *pMatrix) {
    SDF_VU0_LOADMATRIX(pMatrix);
    SDF_VU0_STOREMATRIX(hData->hPoly->matrix);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3423, match, same function 1.00 (words 60/60, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpHPartSetVC(effPCPFlashPolyRingExpH_tag *work, s32 index, s32 modulationColor) {
    u32 *vc;
    s32 firstRgb;
    s32 secondRgb;

    vc = (u32 *)dds3FlashPolyListHGetVC(work->hPoly, index);
    firstRgb = work->firstColor & EFF_RGB_MASK;
    secondRgb = work->secondColor & EFF_RGB_MASK;
    vc[0] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    vc[1] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    if (index & 1) {
        vc[2] = effMagaCylinderCalcColor(0x80000000, modulationColor);
        vc[3] = effMagaCylinderCalcColor(firstRgb | 0xFF000000, modulationColor);
        vc[4] = effMagaCylinderCalcColor(0x80000000, modulationColor);
    } else {
        vc[2] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
        vc[3] = effMagaCylinderCalcColor(firstRgb | 0x40000000, modulationColor);
        vc[4] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
    }
}

void effPCPFlashPolyRingExpPartSetVertex(effPCPFlashPolyRingExp_tag *work, s32 index)
{
    effPCPFlashPolyRingExpPart_tag *part = &work->pPart[index];
    f32 *quad = (f32 *)dds3FlashPolyListHGetVertex(work->hPoly, index);
    sdfVector4_t offset;
    sdfVector4_t unit;
    sdfVector4_t scaleA;
    sdfVector4_t scaleB;
    sdfVector4_t scaleC;
    f32 sinv;
    f32 height;

    VEC3_SPLAT(&scaleA.x, work->length);
    VEC3_SPLAT(&scaleB.x, work->width_out);
    VEC3_SPLAT(&scaleC.x, work->width_in);
    unit.x = sdfCos(part->accumulator);
    unit.y = 0;
    sinv = sdfSin(part->accumulator);
    unit.z = sinv;
    offset.x = unit.x * work->radius;
    offset.y = 0;
    offset.z = sinv * work->radius;
    height = part->stepSpeed;
    effPCPFlashPolyRingExpVertex[0] = unit.x * height;
    effPCPFlashPolyRingExpVertex[1] = height + -1.0f;
    effPCPFlashPolyRingExpVertex[2] = sinv * height;
    SDF_VU0_LOADVECTOR(effPCPFlashPolyRingExpVertex);
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&unit);
    SDF_VU0_OUTERPRODUCT();
    SDF_VU0_NORMALIZE();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(&scaleB);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleB);
    SDF_VU0_LOADVECTOR(&scaleC);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(&scaleC);
    SDF_VU0_COPYVECTOR_C_TO_A();
    SDF_VU0_LOADVECTOR_B(&scaleA);
    SDF_VU0_MULVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_C();
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleC);
    SDF_VU0_STOREVECTOR(quad + 12);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad + 8);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 16);
    SDF_VU0_LOADVECTOR(&offset);
    SDF_VU0_LOADVECTOR_B(&scaleB);
    SDF_VU0_ADDVECTOR_A_C();
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR(quad);
    SDF_VU0_SUBVECTOR();
    SDF_VU0_SUBVECTOR();
    SDF_VU0_STOREVECTOR(quad + 4);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3507, match, same function 1.00 (words 8/8, calls 1.00, shape 1.00) */
void effPCPFlashPolyRingExpHPartRotate(effPCPFlashPolyRingExpH_tag *pFlashPoly, s32 number) {
    effPCPFlashPolyRingExpHPart_tag *particle;

    particle = &pFlashPoly->pPart[number];
    particle->rotate = particle->rotate + SDF_FRAME_STEP(pFlashPoly->increment);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3514, near, same function 0.88 (words 214/214, calls 0.62, shape 1.00) */
void effPCPFlashPolyRingExpDraw(effPCPFlashPolyRingExpH_tag *hData)
{
    s32 index;
    s32 count;
    effPCPFlashPolyRingExpHPart_tag *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;
    f32 heightDelta;
    f32 delta;
    f32 progress;
    f32 factor;
    f32 start;
    f32 radiusEnd;
    f32 upStart;
    f32 acrossStart;
    f32 upEnd;
    f32 acrossEnd;
    f32 normalEnd;

    part = hData->pPart;
    count = hData->count;
    lifetime = SDF_FRAMES(hData->duration);
    fadeIn = SDF_FRAMES(hData->fadeIn);
    fadeOut = SDF_FRAMES(hData->fadeOut);
    restart = hData->respawn;
    randomRange = hData->spread;
    tintColor = hData->abgr;
    heightDelta = hData->param4C - hData->param48;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)hData->frame / (f32)lifetime;
    }
    start = hData->param38;
    radiusEnd = hData->param3C;
    delta = radiusEnd - start;
    hData->radius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = hData->param30;
    acrossStart = hData->param34;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = hData->param40;
    normalEnd = hData->param44;
    delta = upEnd - upStart;
    hData->currentSideOffsetB = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    hData->currentSideOffsetA = acrossStart + delta * progress;
    delta = normalEnd - start;
    hData->length = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->frame;
        s32 color;

        if (age == 0) {
            part->spread = hData->param48;
            effPCPFlashPolyRingExpPartSetVertex((effPCPFlashPolyRingExp_tag *)hData, index);
            effPCPFlashPolyRingExpHPartSetVC(hData, index, 0);
            part->abgr = EFF_NEUTRAL_COLOR;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->frame = ~(SDF_FRAMES(dds3GetRandA((effRandState_t *)dds3EffectBase) % randomRange + 1) - 1);
            }
            color = 0;
            effPCPFlashPolyRingExpHPartSetVC(hData, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->spread = heightDelta * progress + hData->param48;
            effPCPFlashPolyRingExpHPartRotate(hData, index);
            effPCPFlashPolyRingExpPartSetVertex((effPCPFlashPolyRingExp_tag *)hData, index);
            if (part->frame < fadeIn && fadeIn != 0) {
                factor = (f32)part->frame / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->frame && fadeOut != 0) {
                factor = (f32)(lifetime - part->frame) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->abgr, factor), tintColor);
            effPCPFlashPolyRingExpHPartSetVC(hData, index, color);
        }
        part->frame = part->frame + 1;
    }
    if (hData->frame == lifetime) {
        hData->frame = 0;
    } else {
        hData->frame++;
    }
    handle = hData->hPoly;
    handle->pos[0] = hData->origin[0];
    handle->pos[1] = hData->origin[1];
    handle->pos[2] = hData->origin[2];
    handle->scale = hData->scale;
    dds3FlashPolyListHDraw(handle);
}

dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u8 *block;
    s32 vertexCount;
    s32 recordBytes;
    s32 size;
    s32 quads;

    vertexCount = face * 5;
    recordBytes = vertexCount * 5 * 4;
    quads = recordBytes >> 4;
    recordBytes = ((recordBytes & 0xF) != 0 ? quads + 1 : quads) << 4;
    size = recordBytes + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u8 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + recordBytes);
    pool->pVertex = (u8 *)block;
    pool->pVC = (u8 *)(block + vertexCount * 16);
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->blend = 2;
    pool->point = vertexCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->attr = 0x4000;
    return pool;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3650, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effCallMatFreeDelayed2(dds3FlashPolyList_t *hPoly) {
    sdfMatFreeDelayed(hPoly->pMat);
    sdfMemFreeDirect(hPoly->hMem);
}

void dds3FlashPolyListDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
    sdfDrawTagInit(list);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = work->scale;
    matrix[5] = work->scale;
    matrix[10] = work->scale;
    matrix[12] = work->pos[0];
    matrix[13] = work->pos[1];
    matrix[14] = work->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->pMat, 0);
    remaining = work->point;
    effPCPFlashPolyParams->vertexColor = (u32 *)work->pVC;
    effPCPFlashPolyParams->vertex = (sdfVector4_t *)work->pVertex;
    effPCPFlashPolyParams->color = work->abgr;
    effPCPFlashPolyParams->faceCount = EFF_FAN_BATCH_PARAMS;
    effPCPFlashPolyParams->pointCount = EFF_FAN_BATCH_VERTICES;
    effPCPFlashPolyParams->faceIndex = effPCPFlashFanIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
        effPCPFlashPolyParams->vertex += EFF_FAN_BATCH_VERTICES;
        effPCPFlashPolyParams->vertexColor += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effPCPFlashPolyParams->pointCount = remaining;
        effPCPFlashPolyParams->faceCount = fanCount * 3;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
    }
    if (work->blend < EFF_DIRECT_SURFACE_COUNT) {
        effPCPFlashFanSurfaces[work->blend]->regFunc(effPCPFlashFanSurfaces[work->blend], list);
    } else {
        stateList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
        sdfDrawTagInit(stateList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES));
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, EE_ADDR(packet));
        effFlashOtNode->regFunc(effFlashOtNode, stateList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES));
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(packet));
        effFlashOtNode->regFunc(effFlashOtNode, list);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3721, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3725, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVC + groupIndex * EFF_FAN_COLOR_BYTES;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3729, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPCPCrackInitSub(dds3FlashPolyList_t *hPoly, u32 blend) {
    hPoly->blend = blend;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3733, match, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
void effPCPCrackDrawSub(dds3FlashPolyList_t *hPoly, u32 abgr) {
    hPoly->abgr = abgr;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effLeaf242);

dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 vertexCount;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    vertexCount = face * EFF_TRIANGLE_VERTEX_COUNT;
    positionWordCount = vertexCount * 4;
    colorWordCount = vertexCount;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u32 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->pVertex = (u8 *)block;
    pool->pVC = (u8 *)(block + positionWordCount);
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->blend = 2;
    pool->point = colorWordCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->attr = 0x4000;
    return pool;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3772, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effCallMatFreeDelayed3(dds3FlashPolyList_t *hPoly) {
    sdfMatFreeDelayed(hPoly->pMat);
    sdfMemFreeDirect(hPoly->hMem);
}

void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
    s32 remainingVertices;
    sdfGrOt_t *surface;

    sdfDrawTagInit(packet);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->pos[0];
    matrix[13] = pool->pos[1];
    matrix[14] = pool->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->pMat, 0);
    remainingVertices = pool->point;
    effPCPFlashPolyParams->vertexColor = (u32 *)pool->pVC;
    effPCPFlashPolyParams->vertex = (sdfVector4_t *)pool->pVertex;
    effPCPFlashPolyParams->color = pool->abgr;
    effPCPFlashPolyParams->faceCount = 0x10;
    effPCPFlashPolyParams->pointCount = EFF_TRIANGLE_BATCH_VERTICES;
    effPCPFlashPolyParams->faceIndex = NULL;
    while (remainingVertices >= EFF_TRIANGLE_BATCH_VERTICES) {
        remainingVertices -= EFF_TRIANGLE_BATCH_VERTICES;
        sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
        effPCPFlashPolyParams->vertex += EFF_TRIANGLE_BATCH_VERTICES;
        effPCPFlashPolyParams->vertexColor += EFF_TRIANGLE_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_TRIANGLE_VERTEX_COUNT) {
        effPCPFlashPolyParams->faceCount = remainingVertices / EFF_TRIANGLE_VERTEX_COUNT;
        effPCPFlashPolyParams->pointCount = remainingVertices;
        sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
    }
    surface = effPCPFlashTriangleSurfaces[pool->blend];
    surface->regFunc(surface, packet);
}

void *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_TRIANGLE_POSITION_BYTES;
}

void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVC + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}

dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 face) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    positionWordCount = face * 16;
    colorWordCount = face * EFF_QUAD_VERTEX_COUNT;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = EE_ADDR_AS(u32 *, sdfMemLock(handle));
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->pVertex = (u8 *)block;
    pool->blend = 2;
    pool->pVC = (u8 *)(block + positionWordCount);
    pool->point = colorWordCount;
    pool->hMem = handle;
    pool->scale = 1.0f;
    pool->abgr = EFF_NEUTRAL_COLOR;
    pool->pMat = sdfMatNew();
    sdfMatSetConstant(pool->pMat, 1.0f);
    memset(effPCPFlashPolyParams, 0, EFF_PACKET_PARAMS_BYTES);
    effPCPFlashPolyParams->attr = 0x4000;
    return pool;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3854, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
void effCallMatFreeDelayed4(dds3FlashPolyList_t *hPoly) {
    sdfMatFreeDelayed(hPoly->pMat);
    sdfMemFreeDirect(hPoly->hMem);
}

void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
    s32 remainingVertices;
    sdfGrOt_t *surface;

    sdfDrawTagInit(packet);
    SDF_VU0_STOREIDENTITY(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->pos[0];
    matrix[13] = pool->pos[1];
    matrix[14] = pool->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->pMat, 0);
    remainingVertices = pool->point;
    effPCPFlashPolyParams->vertexColor = (u32 *)pool->pVC;
    effPCPFlashPolyParams->vertex = (sdfVector4_t *)pool->pVertex;
    effPCPFlashPolyParams->color = pool->abgr;
    effPCPFlashPolyParams->faceCount = 0x10;
    effPCPFlashPolyParams->pointCount = EFF_QUAD_BATCH_VERTICES;
    effPCPFlashPolyParams->faceIndex = effPCPFlashQuadIndices;
    while (remainingVertices >= EFF_QUAD_BATCH_VERTICES) {
        remainingVertices -= EFF_QUAD_BATCH_VERTICES;
        sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
        effPCPFlashPolyParams->vertex += EFF_QUAD_BATCH_VERTICES;
        effPCPFlashPolyParams->vertexColor += EFF_QUAD_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_QUAD_VERTEX_COUNT) {
        effPCPFlashPolyParams->faceCount = remainingVertices / EFF_QUAD_VERTEX_COUNT * 2;
        effPCPFlashPolyParams->pointCount = remainingVertices;
        sdfDrawTagLink(packet, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
    }
    surface = effPCPFlashQuadSurfaces[pool->blend];
    surface->regFunc(surface, packet);
}

void *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_QUAD_POSITION_BYTES;
}

void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVC + groupIndex * EFF_QUAD_COLOR_BYTES;
}

dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 face) {
    dds3FlashPolyList_t *pool = dds3FlashPolyListCreate(face);

    SDF_VU0_STOREIDENTITY(pool->matrix);
    return pool;
}

void dds3FlashPolyListHDelete(dds3FlashPolyList_t *hPoly)
{
    dds3FlashPolyListDelete(hPoly);
}

void dds3FlashPolyListHDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
    sdfDrawTagInit(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->pos[0];
    matrix[13] = work->pos[1];
    matrix[14] = work->pos[2];
    SDF_VU0_LOADMATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->pMat, 0);
    remaining = work->point;
    effPCPFlashPolyParams->vertexColor = (u32 *)work->pVC;
    effPCPFlashPolyParams->vertex = (sdfVector4_t *)work->pVertex;
    effPCPFlashPolyParams->color = work->abgr;
    effPCPFlashPolyParams->faceCount = EFF_FAN_BATCH_PARAMS;
    effPCPFlashPolyParams->pointCount = EFF_FAN_BATCH_VERTICES;
    effPCPFlashPolyParams->faceIndex = effPCPFlashFanIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
        effPCPFlashPolyParams->vertex += EFF_FAN_BATCH_VERTICES;
        effPCPFlashPolyParams->vertexColor += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effPCPFlashPolyParams->pointCount = remaining;
        effPCPFlashPolyParams->faceCount = fanCount * 3;
        sdfDrawTagLink(list, EE_ADDR(effPkt3DPrimMakePoly(effPCPFlashPolyParams)));
    }
    if (work->blend < EFF_DIRECT_SURFACE_COUNT) {
        effPCPFlashFanHSurfaces[work->blend]->regFunc(effPCPFlashFanHSurfaces[work->blend], list);
    } else {
        stateList = EE_ADDR_AS(sdfDrawTag_t *, sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES));
        sdfDrawTagInit(stateList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES));
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, EE_ADDR(packet));
        effFlashOtNode->regFunc(effFlashOtNode, stateList);
        packet = EE_ADDR_AS(u64 *, sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES));
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, EE_ADDR(packet));
        effFlashOtNode->regFunc(effFlashOtNode, list);
    }
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3981, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
void *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *group, s32 groupIndex) {
    return group->pVertex + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effPCPFlash.c:3985, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *group, s32 groupIndex) {
    return (u32 *)(group->pVC + groupIndex * EFF_FAN_COLOR_BYTES);
}
