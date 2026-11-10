#ifndef DDS_EFF_BILL_H

#define DDS_EFF_BILL_H
#include "common.h"

#include "eff_expanded_list.h"

#include "sdf.h"

#include "eff_curve.h"

struct billHandle_t;

struct EffTrackSet;

struct FileSlotTable;

/* Lens-flare constructors copy 0x40 bytes into a 0x58-byte owner at +0x18
 * (DDS1 0029C620 / DDS2 002DE338). Draw callbacks read strength with LWC1,
 * compare frame/limit signed, and select the flare set with LBU at +0x54. */
typedef struct EffLensFlareParams {
    SdfColorTrack colorCurve;
    SdfAlphaTrack alphaCurve;
    f32 strength;
    s32 limit;
    u8 flareSet;
    u8 pad3D[3];
} EffLensFlareParams;

/* Lens-flare table row (16 bytes); effLensFlareSets holds sets of thirteen rows, selected by EffLensFlareParams.flareSet. */
typedef struct EffLensFlareElement {
    f32 positionScale; /* 0x00: distance along the line from the screen center */
    f32 sizeScale;     /* 0x04 */
    u8 textureIndex;   /* 0x08: dds3BillboardGetGeneralTex index */
    u8 pad09[3];
    u32 color;         /* 0x0C: RGBA; alpha 0 skips the element */
} EffLensFlareElement;

typedef struct EffFadeVectorWork {
    f32 vector[4];
    s32 frame;
    u32 color;
    EffLensFlareParams source;
} EffFadeVectorWork;

typedef char EffLensFlareParamsSizeCheck[(sizeof(EffLensFlareParams) == 0x40) ? 1 : -1];

typedef char EffLensFlareElementSizeCheck[(sizeof(EffLensFlareElement) == 0x10) ? 1 : -1];

typedef char EffFadeVectorWorkSizeCheck[(sizeof(EffFadeVectorWork) == 0x58) ? 1 : -1];

/* The 0x20-byte strip file header copied into EffectStripNode at +0x0C. */
typedef struct EffStripParams {
    u8 flag;
    u8 pad01[3];
    u32 innerColor;
    u32 outerColor;
    f32 crossSize;
    f32 longSize;
    f32 sideSize;
    f32 billSize;
    u32 tint;
} EffStripParams;

typedef char EffStripParamsSizeCheck[(sizeof(EffStripParams) == 0x20) ? 1 : -1];

/* DDS1 002AAF70 / DDS2 002EE348 allocate this 0x3C-byte strip owner.
 * File constructors copy a 0x20-byte source header into its +0x0C member. */
typedef struct EffectStripNode {
    u32 percent;
    u32 color;
    f32 opacity;
    EffStripParams copiedHeader;
    struct EffTrackSet *trackSet;
    struct billHandle_t *resource;
    struct FileSlotTable *active;
    u16 count;
} EffectStripNode;

typedef char EffectStripNodeSizeCheck[(sizeof(EffectStripNode) == 0x3C) ? 1 : -1];

/* DDS1 002A7B68 / DDS2 002EA120 allocate 0xD4 bytes and copy this
 * 0x98-byte parameter record at +0x30. Draws 002A8020 / 002EA5D8
 * scale the +0x64 track's result and use the +0x90 track as the angle. */
typedef struct EffQuadParams {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    EffScalarTrack sizeTrack;
    EffScalarTrack angleTrack;
    s32 duration;
    f32 sizeScale;
    u8 noSetup;
    u8 pad95[3];
} EffQuadParams;

typedef struct EffQuadWork {
    f32 position[4];
    f32 orientation[4];
    f32 scale;
    u32 color;
    u32 sourceKind;
    s32 frame;
    EffQuadParams source;
    struct billHandle_t *billHandle;
    struct EffExpandedList *reference; /* Kind-7 retained resource wrapper. */
    sdfMat_t *assetHandle;
} EffQuadWork;

typedef char EffQuadParamsSizeCheck[(sizeof(EffQuadParams) == 0x98) ? 1 : -1];

typedef char EffQuadWorkSizeCheck[(sizeof(EffQuadWork) == 0xD4) ? 1 : -1];

/* Class operations select the copied parameter format and own this 0x40-byte
 * header. Each resource kind owns the pointer returned by its factory. */
typedef struct EffClassWork {
    union {
        u8 transform[0x20];
        struct {
            f32 position[4];
            f32 orientation[4];
        } vectors;
    };
    f32 scale;
    u32 color;
    u32 frame;
    s32 kind;
    void *resource;
    void *payload;
    u8 pad38[8];
} EffClassWork;

/* Count-bearing serialized formats share the first 0x3C bytes. Native
 * point callbacks compare the time word signed; emitter callbacks compare it
 * unsigned. These are two real interpretations of the same serialized word. */
typedef struct EffBillTimedHeader {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    union {
        u32 progress;
        s32 duration;
    } time;
    u32 count;
} EffBillTimedHeader;

typedef struct EffBillOutputHeader {
    EffBillTimedHeader timed;
    u8 outputMode;
    u8 pad3D[3];
} EffBillOutputHeader;

typedef struct EffBillFrameHeader {
    EffBillOutputHeader output;
    u8 unk40[4];
    f32 fadeInFraction;
    f32 fadeOutFraction;
    s32 lifetime;
    s32 spawnCount;
    u8 respawn;
    u8 spawnAllOnStart;
    u8 mode;
    u8 pad57;
} EffBillFrameHeader;

/* DDS2 003E9950 / DDS1 0037E8A0 resource kinds 1..8 copy the following
 * record lengths. Unrecovered bytes remain serialized data. */
/* DDS2 billAdvanceFrameInstances (002E0900) consumes the kind-1 geometry tail. */
typedef struct EffBillFrameConfig {
    EffBillFrameHeader frame;
    u32 innerColor;
    u32 outerColor;
    f32 innerWidth;
    f32 outerWidth;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillFrameConfig; /* kind 1, 0x80 */

typedef struct EffBillCellConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 extent;
    u8 unk64[4];
    f32 extentJitter;
    f32 radius;
    f32 radiusJitter;
    f32 centerOffset;
    f32 centerOffsetJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad89[3];
} EffBillCellConfig; /* kind 2, 0x8C */

struct RefObj;

/* Track-set allocations end with this 0x30-byte header. Kind selects the
 * geometry and optional column strides; tail follows the geometry buffer. */
typedef struct EffTrackSet {
    u32 type;
    u32 color;
    s32 rows;
    u16 kind;
    u8 pad0E[2];
    s32 count;
    u8 flag;
    u8 pad15[3];
    struct RefObj *shared;
    u8 *buffer;
    u8 *columns;
    u8 *tail;
    sdfMat_t *handle;
    struct sdfMemHandle_t *allocation;
} EffTrackSet;

typedef char EffTrackSet_size_must_be_0x30[(sizeof(EffTrackSet) == 0x30) ? 1 : -1];

/* Resource kinds 1..8 allocate sixteen bytes before their entry array. */
typedef struct EffBillFrameState {
    u8 *entries;
    EffTrackSet *asset;
    struct sdfMemHandle_t *allocation;
    u32 unk0C;
} EffBillFrameState;

typedef struct EffBillCellEntry {
    s32 age;
    f32 velocity;
    f32 angle;
    f32 radiusA;
    f32 radiusB;
    f32 centerOffset;
    f32 extent;
} EffBillCellEntry;

/* Kind-3 particle entry (0x2C bytes), the array behind EffBillFrameState.entries. */
typedef struct EffBillParticleEntry {
    s32 age;        /* -1 waiting to spawn, -2 retired, otherwise frames alive */
    f32 rise;
    f32 spinRate;
    f32 angle;
    f32 radiusA;
    f32 radiusB;
    f32 offset;
    f32 offsetStep;
    f32 extent;
    u8 pad24[8];
} EffBillParticleEntry;

typedef struct EffBillParticleConfig {
    EffBillFrameHeader frame;
    u32 colorA;             /* 0x58 */
    u32 colorB;             /* 0x5C */
    f32 extent;             /* 0x60 */
    u8 pad64[4];
    f32 extentJitter;       /* 0x68 */
    f32 radius;             /* 0x6C */
    f32 radiusJitter;       /* 0x70 */
    f32 offset;             /* 0x74 */
    f32 offsetJitter;       /* 0x78 */
    f32 endOffset;          /* 0x7C */
    f32 endOffsetJitter;    /* 0x80 */
    f32 spinRate;           /* 0x84 */
    f32 spinRateJitter;     /* 0x88 */
    f32 acceleration;       /* 0x8C */
    f32 rise;               /* 0x90 */
    f32 riseJitter;         /* 0x94 */
    f32 gravity;            /* 0x98 */
} EffBillParticleConfig; /* kind 3, 0x9C */

typedef struct EffBillAnimatedFrameConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    u32 drawProgress; /* 0x70: frames over which the radius grows; at least 1 */
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillAnimatedFrameConfig; /* kind 4, 0x80 */

typedef struct EffBillEmitterFrameConfig {
    EffBillFrameHeader frame;
    u32 firstColor;     /* 0x58 */
    u32 secondColor;    /* 0x5C */
    f32 width;          /* 0x60 */
    f32 widthJitter;    /* 0x64 */
    f32 radius;         /* 0x68 */
    f32 radiusJitter;   /* 0x6C */
    f32 offset;         /* 0x70 */
    f32 offsetJitter;   /* 0x74 */
    f32 speed;          /* 0x78 */
    f32 speedJitter;    /* 0x7C */
    f32 acceleration;  /* 0x80 */
    u8 reverse;         /* 0x84 */
    u8 padding85[3];
} EffBillEmitterFrameConfig; /* kind 5, 0x88 */

/* Kind-6 radial strip simulation: copied 0xA4-byte configuration. */
typedef struct EffBillStripFrameConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 lengthStart;
    f32 lengthStartJitter;
    f32 lengthEnd;
    f32 lengthEndJitter;
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 tiltStart;
    f32 tiltStartJitter;
    f32 tiltEnd;
    f32 tiltEndJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillStripFrameConfig; /* kind 6, 0xA4 */

typedef struct EffBillStripEntry {
    s32 age;
    f32 angle;
    f32 spin;
    f32 length;
    f32 lengthStep;
    f32 radius;
    f32 radiusStep;
    f32 tilt;
    f32 tiltStep;
    f32 width;
} EffBillStripEntry;

typedef char EffBillStripEntrySizeCheck[(sizeof(EffBillStripEntry) == 0x28) ? 1 : -1];

/* Kind-7 trails share each randomized width factor across their two ends. */
typedef struct EffBillTrailFrameConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 widthA;
    f32 widthB;
    f32 widthJitter;
    f32 lengthStart;
    f32 lengthStartJitter;
    f32 lengthEnd;
    f32 lengthEndJitter;
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 tiltStart;
    f32 tiltStartJitter;
    f32 tiltEnd;
    f32 tiltEndJitter;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillTrailFrameConfig; /* kind 7, 0xA8 */

typedef struct EffBillTrailEntry {
    s32 age;
    f32 angle;
    f32 spin;
    f32 length;
    f32 lengthStep;
    f32 radius;
    f32 radiusStep;
    f32 tilt;
    f32 tiltStep;
    f32 widthA;
    f32 widthB;
} EffBillTrailEntry;

typedef char EffBillTrailEntrySizeCheck[(sizeof(EffBillTrailEntry) == 0x2C) ? 1 : -1];

/* Kind-8 quad updates (DDS1 002A2E18 / DDS2 002E4E80) read these
 * parameters from the copied 0x98-byte record and advance 0x20-byte entries. */
typedef struct EffBillQuadFrameConfig {
    EffBillFrameHeader frame;
    u32 colorA;
    u32 colorB;
    f32 width;
    f32 widthJitter;
    f32 radius;
    f32 radiusJitter;
    f32 heightStart;
    f32 heightStartJitter;
    f32 heightEnd;
    f32 heightEndJitter;
    f32 tilt;
    f32 tiltJitter;
    f32 velocity;
    f32 velocityJitter;
    f32 acceleration;
    u8 reverse;
    u8 pad95[3];
} EffBillQuadFrameConfig; /* kind 8, 0x98 */

typedef struct EffBillQuadEntry {
    s32 timer;
    f32 velocity;
    f32 angle;
    f32 radius;
    f32 height;
    f32 heightStep;
    f32 tilt;
    f32 width;
} EffBillQuadEntry;

typedef struct EffBillPointConfig {
    EffBillTimedHeader timed;
    /* Bolt/range updaters (DDS2 002EB058/002EC370) interpret the
     * layer word as an unsigned division count for floating-point math. */
    s32 layers;
    s32 respawnDelay;
    /* Initial lateral-wave amplitude of alternating five-point ribbons. */
    f32 alternatingAmplitude; /* 0x44 */
    u8 pad48[4];
    u32 periods;
    f32 amplitude;
    f32 speed;
    f32 speedJitter;
    u8 drawFlag;
    u8 pad5D[0xB];
    f32 rangeFadeInEnd;
    f32 rangeFadeOutStart;
    u32 colorA;
    u32 colorB;
    u32 colorC;
    f32 halfWidth;
    f32 edgeWidth;
    f32 totalLength; /* 0x84: divided by layer count to size each mesh segment. */
} EffBillPointConfig;

typedef struct EffBillRangeConfig {
    EffBillPointConfig point;
    u8 tilted;
    u8 pad89[3];
    f32 startBase;
    f32 startRand;
    f32 endBase;
    f32 endRand;
    f32 heightScale;
    f32 angularBase;
    f32 angularRand;
    f32 angularAcceleration;
} EffBillRangeConfig;

/* DDS2 resource-op rows at 003E9DF4/003E9E10/003E9E2C copy
 * 0xF8/0xF4/0xD8 bytes for vortex/column/spiral. The row at 003E9E7C
 * copies 0x10C bytes for the animated flame. Kind is local to its operation
 * table; EffClassWork.kind together with that table selects the record. */
typedef struct EffBillEmitterHeader {
    EffBillTimedHeader timed;
    SdfColorTrack emitterColorTrack;
    SdfAlphaTrack emitterAlphaTrack;
    u8 pad70[8];
    f32 fadeIn;
    f32 fadeOut;
    u32 life;
    u32 spawnRate;
    u8 respawn;
    u8 pad89[3];
    u32 segments;
} EffBillEmitterHeader;

typedef struct EffBillEmitterCommon {
    EffBillEmitterHeader header;
    f32 rowRadius;
    u8 pad94[4];
    f32 height;
    f32 heightJitter;
    u8 padA0[4];
    f32 rise;
    /* Angular sweep for vortex/spiral; longitudinal extent for column. */
    f32 extent;
    f32 extentJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 drawMode;
    u8 padBA[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
} EffBillEmitterCommon;

typedef struct EffBillVortexConfig {
    EffBillEmitterCommon common;
    f32 climbStart;
    f32 climbStartJitter;
    f32 climbEnd;
    f32 climbEndJitter;
    u8 padDC[4];
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillVortexConfig;

typedef struct EffBillColumnConfig {
    EffBillEmitterCommon common;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
    f32 flareJitter;
    f32 flareCenter;
    f32 flare;
} EffBillColumnConfig;

typedef struct EffBillSpiralConfig {
    EffBillEmitterCommon common;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
} EffBillSpiralConfig;

typedef struct EffBillFlameConfig {
    EffBillEmitterHeader header;
    EffScalarCurve scaleCurve;
    u8 padB4[5];
    u8 drawMode;
    u8 padBA[2];
    f32 spreadA;
    f32 spreadB;
    f32 driftA;
    f32 driftB;
    f32 length;
    f32 lengthJitter;
    f32 width;
    f32 widthJitter;
    u8 burst;
    u8 meshMode;
    u8 padDE[2];
    f32 radiusStart;
    f32 radiusStartJitter;
    f32 radiusEnd;
    f32 radiusEndJitter;
    f32 velocity;
    f32 spin;
    f32 spinJitter;
    f32 spinAccel;
    f32 drag;
    f32 dragJitter;
    f32 gravity;
} EffBillFlameConfig;

/* Shared strip resource owner used by the flame updater and the existing
 * quantized-row producers in both games. */
typedef struct EffStripWork {
    u32 count;          // 0x00
    u32 type;           // 0x04
    u32 color;          // 0x08
    s32 rowStride;      // 0x0C
    s32 repeat;         // 0x10
    u8 flag;            // 0x14
    u8 pad_15[3];
    u32 *colors;        // 0x18
    u8 *positions;      // 0x1C
    u8 *uvsA;           // 0x20
    u8 *uvsB;           // 0x24
    u8 *extra;          // 0x28
    sdfMat_t *handle;   // 0x2C
    struct sdfMemHandle_t *allocation; // 0x30
} EffStripWork;

/* The class operation table selects the interpretation of each 0x30-byte
 * entry. Vortex (002ABDE0/002EF1C0), column (002ACA40/002EFE20) and flame
 * (002AEC60/002F2050) share the timer/radius/spin prefix, but use the
 * +0x10/+0x14 and +0x24..+0x2C words differently. Their allocators and
 * reset callbacks use the same record stride and timer sentinel. */
typedef struct EffBillEmitterEntry {
    s32 timer;
    u8 unk04[4];
    f32 radius;
    f32 radiusStep;
    union {
        struct {
            f32 climb;
            f32 climbStep;
        } vortex;
        struct {
            f32 drag;
            f32 positionY;
        } column;
        struct {
            f32 drag;
            f32 velocity;
        } flame;
    } vertical;
    f32 spin;
    f32 angle;
    f32 width;
    union {
        struct {
            f32 sweep;
            f32 flare;
            f32 height;
        } vortex;
        struct {
            f32 length;
            f32 flare;
            f32 height;
        } column;
        struct {
            f32 length;
            f32 baseA;
            f32 baseB;
        } flame;
    } geometry;
} EffBillEmitterEntry;

typedef struct EffBillRadialConfig {
    EffBillPointConfig point;
    f32 radius;
} EffBillRadialConfig;

/* Animation resource kind 4 (DDS2 003E9ED0) copies this 0x98 record.
 * Samples are consumed as an unsigned allocation count and a signed row bound. */
typedef struct EffBillQuantizedConfig {
    SdfColorTrack colorTrack;
    SdfAlphaTrack alphaTrack;
    EffScalarCurve scaleCurve;
    u8 pad58[0x10];
    f32 alphaFadeInFraction;
    f32 alphaFadeOutFraction;
    u32 drawProgress;
    union {
        u32 quantizedSamples;
        s32 signedRows;
    } samples;
    f32 fadeInEnd;
    f32 fadeOutStart;
    f32 uvARowScale;
    f32 uvBVertexScale;
    f32 rowOffset;
    f32 radius;
    f32 angularSpanDegrees;
    u8 meshMode;
    u8 pad95[3];
} EffBillQuantizedConfig;

/* Other class records selected by DDS2 003E9B80 / DDS1 0037EAD0.
 * Their count/time header and output byte are recovered; tails remain unknown. */
typedef struct EffBillClass1Config {
    EffBillOutputHeader output;
    u8 unk40[0x2C];
} EffBillClass1Config;

typedef struct EffBillClass2Config {
    EffBillOutputHeader output;
    u8 unk40[0x90];
} EffBillClass2Config;

typedef struct EffBillClass3Config {
    EffBillOutputHeader output;
    u8 unk40[0x28];
} EffBillClass3Config;

typedef union EffBillConfig {
    EffBillFrameConfig frame;
    EffBillCellConfig cell;
    EffBillParticleConfig particle;
    EffBillAnimatedFrameConfig animatedFrame;
    EffBillEmitterFrameConfig emitterFrame;
    EffBillStripFrameConfig stripFrame;
    EffBillTrailFrameConfig trailFrame;
    EffBillQuadFrameConfig quadFrame;
    EffBillClass1Config class1;
    EffBillClass2Config class2;
    EffBillClass3Config class3;
    EffBillPointConfig point;
    EffBillRadialConfig radial;
    EffBillRangeConfig range;
    EffBillVortexConfig vortex;
    EffBillColumnConfig column;
    EffBillSpiralConfig spiral;
    EffBillFlameConfig flame;
    EffBillQuantizedConfig quantized;
} EffBillConfig;

struct EffPointSet;

typedef struct EffScaleRangeEntry {
    struct EffPointSet *set;
    f32 position[3];
    u8 pad10[4];
    s32 negativeSeed;
    u32 color;
    f32 yaw;
    f32 pitch;
    f32 angularRate;
    f32 phase;
    f32 height;
} EffScaleRangeEntry;

typedef char EffClassWorkSizeCheck[(sizeof(EffClassWork) == 0x40) ? 1 : -1];

typedef char EffBillTimedHeaderSizeCheck[(sizeof(EffBillTimedHeader) == 0x3C) ? 1 : -1];

typedef char EffBillOutputHeaderSizeCheck[(sizeof(EffBillOutputHeader) == 0x40) ? 1 : -1];

typedef char EffBillFrameHeaderSizeCheck[(sizeof(EffBillFrameHeader) == 0x58) ? 1 : -1];

typedef char EffBillFrameConfigSizeCheck[(sizeof(EffBillFrameConfig) == 0x80) ? 1 : -1];

typedef char EffBillCellConfigSizeCheck[(sizeof(EffBillCellConfig) == 0x8C) ? 1 : -1];

typedef char EffBillParticleConfigSizeCheck[(sizeof(EffBillParticleConfig) == 0x9C) ? 1 : -1];

typedef char EffBillParticleEntrySizeCheck[(sizeof(EffBillParticleEntry) == 0x2C) ? 1 : -1];

typedef char EffBillAnimatedFrameConfigSizeCheck[(sizeof(EffBillAnimatedFrameConfig) == 0x80) ? 1 : -1];

typedef char EffBillEmitterFrameConfigSizeCheck[(sizeof(EffBillEmitterFrameConfig) == 0x88) ? 1 : -1];

typedef char EffBillStripFrameConfigSizeCheck[(sizeof(EffBillStripFrameConfig) == 0xA4) ? 1 : -1];

typedef char EffBillTrailFrameConfigSizeCheck[(sizeof(EffBillTrailFrameConfig) == 0xA8) ? 1 : -1];

typedef char EffBillQuadFrameConfigSizeCheck[(sizeof(EffBillQuadFrameConfig) == 0x98) ? 1 : -1];

typedef char EffBillClass1ConfigSizeCheck[(sizeof(EffBillClass1Config) == 0x6C) ? 1 : -1];

typedef char EffBillClass2ConfigSizeCheck[(sizeof(EffBillClass2Config) == 0xD0) ? 1 : -1];

typedef char EffBillClass3ConfigSizeCheck[(sizeof(EffBillClass3Config) == 0x68) ? 1 : -1];

typedef char EffBillQuantizedConfigSizeCheck[(sizeof(EffBillQuantizedConfig) == 0x98) ? 1 : -1];

typedef char EffBillPointConfigSizeCheck[(sizeof(EffBillPointConfig) == 0x88) ? 1 : -1];

typedef char EffBillRangeConfigSizeCheck[(sizeof(EffBillRangeConfig) == 0xAC) ? 1 : -1];

typedef char EffBillEmitterHeaderSizeCheck[(sizeof(EffBillEmitterHeader) == 0x90) ? 1 : -1];

typedef char EffBillEmitterCommonSizeCheck[(sizeof(EffBillEmitterCommon) == 0xCC) ? 1 : -1];

typedef char EffBillVortexConfigSizeCheck[(sizeof(EffBillVortexConfig) == 0xF8) ? 1 : -1];

typedef char EffBillColumnConfigSizeCheck[(sizeof(EffBillColumnConfig) == 0xF4) ? 1 : -1];

typedef char EffBillSpiralConfigSizeCheck[(sizeof(EffBillSpiralConfig) == 0xD8) ? 1 : -1];

typedef char EffBillFlameConfigSizeCheck[(sizeof(EffBillFlameConfig) == 0x10C) ? 1 : -1];

typedef char EffBillRadialConfigSizeCheck[(sizeof(EffBillRadialConfig) == 0x8C) ? 1 : -1];

typedef char EffBillConfigSizeCheck[(sizeof(EffBillConfig) == 0x10C) ? 1 : -1];

typedef char EffScaleRangeEntrySizeCheck[(sizeof(EffScaleRangeEntry) == 0x30) ? 1 : -1];

typedef char EffBillQuadEntrySizeCheck[(sizeof(EffBillQuadEntry) == 0x20) ? 1 : -1];

typedef char EffBillEmitterEntrySizeCheck[(sizeof(EffBillEmitterEntry) == 0x30) ? 1 : -1];

typedef char EffStripWorkSizeCheck[(sizeof(EffStripWork) == 0x34) ? 1 : -1];

#endif
