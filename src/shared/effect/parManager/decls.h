/* Declarations of smt3-decomp src/smt3/effect/parManager.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_PARMANAGER_H
#define SHARED_EFFECT_PARMANAGER_H

#include "common.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "effect.h"

#include "billboard.h"

#include "sdfFrame.h"

typedef struct dds3Particle_BlurBuffList_t dds3Particle_BlurBuffList_t;

typedef struct dds3Particle_BlurBuff_t dds3Particle_BlurBuff_t;

typedef struct dds3Particle_Effect_t dds3Particle_Effect_t;

typedef struct parDispatch_t parDispatch_t;

typedef struct effectBufferRecord_t effectBufferRecord_t;

typedef struct dds3Particle_Physics_t dds3Particle_Physics_t;

typedef struct effEmitterHead_t effEmitterHead_t;

typedef struct effTemplatePacketList_t effTemplatePacketList_t;

typedef struct dds3ParticleColorAnim_t dds3ParticleColorAnim_t;

typedef struct dds3Particle_Basic_t dds3Particle_Basic_t;

typedef struct parObjClientView_t parObjClientView_t;

typedef struct parKindResource_t parKindResource_t;

typedef struct dds3ParOffsetBuffParts_t dds3ParOffsetBuffParts_t;

typedef struct dds3ParOffsetBuff_t dds3ParOffsetBuff_t;

typedef struct effParticleRecord_t effParticleRecord_t;

typedef struct effParticleOverlay_t effParticleOverlay_t;

typedef struct effTemplateScaleRecord_t effTemplateScaleRecord_t;

typedef struct dds3Particle_Radiate_t3 dds3Particle_Radiate_t3;

typedef struct dds3Particle_Spiral_t3 dds3Particle_Spiral_t3;

typedef struct dds3Particle_Smoke_t3 dds3Particle_Smoke_t3;

typedef struct dds3Particle_Spark_t3 dds3Particle_Spark_t3;

typedef struct dds3Particle_SphereSpiral_t3 dds3Particle_SphereSpiral_t3;

typedef struct dds3Particle_SphereSmoke_t3 dds3Particle_SphereSmoke_t3;

typedef struct dds3Particle_SphereRing_t3 dds3Particle_SphereRing_t3;

typedef struct dds3Particle_Spark2_t3 dds3Particle_Spark2_t3;

typedef struct dds3Particle_SparkOffset_t3 dds3Particle_SparkOffset_t3;

typedef struct dds3Particle_SmokeOffset_t3 dds3Particle_SmokeOffset_t3;

typedef struct dds3BillResource_tag dds3BillResource_tag;

typedef struct billData_t billData_t;

typedef struct billOut_t billOut_t;

typedef struct effTrackPolyList_t effTrackPolyList_t;

typedef struct {
    struct dds3ModelHandle_t *model;
    s32 idA;
    s32 idB;
    f32 startFrame;
    f32 endFrame;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
} effTrackPolyParams_t;

typedef struct dds3Particle_BlurBuffList_t {
    u128 *pPos;
    u16 count;
    u8 pad06[2];
    u32 abgr;
    f32 scale;
} dds3Particle_BlurBuffList_t;

typedef struct dds3Particle_BlurBuff_t {
    u16 number;
    u16 point;
    dds3Particle_BlurBuffList_t *blurList;
    billHandle_t **pBill_Handle;
    sdfMemHandle_t *hMem;
} dds3Particle_BlurBuff_t;

typedef struct {
    struct dds3Particle_BlurBuff_t *table; /* +8: kind 1 history source */
    u8 opaque0C[0xC];       /* No other kind-1 payload words are read here. */
} parKindHistoryPayload_t;

typedef struct {
    f32 scale;                      /* +8: shared by kinds 2..4 */
    void *dispatchParameter;        /* +0xC: kinds 2 and 3 */
    struct parSystem_t *drawSystem;   /* +0x10: kind 2 */
    u8 opaque14[4];                 /* Kind 2's final word is unobserved. */
} parKindCrossStripPayload_t;

typedef struct {
    f32 scale;                      /* +8: shared by kinds 2..4 */
    void *dispatchParameter;        /* +0xC: kinds 2 and 3 */
    u8 opaque10[4];                 /* Kind 3's first system word is unobserved. */
    struct parSystem_t *drawSystem;   /* +0x14: kind 3 */
} parKindCrossTrianglePayload_t;

typedef struct {
    f32 scale;                          /* +8: shared by kinds 2..4 */
    u32 firstGradientColor;             /* +0xC: kind 4 */
    u32 secondGradientColor;            /* +0x10: kind 4 */
    struct effTrackPolyList_t *modelList; /* +0x14: kind 4 */
} parKindTrackPolygonPayload_t;

typedef union {
    parKindHistoryPayload_t history;
    parKindCrossStripPayload_t crossStrip;
    parKindCrossTrianglePayload_t crossTriangle;
    parKindTrackPolygonPayload_t trackPolygon;
    u8 opaque[0x10];
} parKindPayload_t;

typedef struct dds3Particle_Effect_t {
    u16 type; /* +0: ParKindStateKind tag */
    u8 pad02[2];
    /* The halfword bitfield preserves native int-alias dependencies; see
     * dds-decomp/docs/idioms.md, "Trace-justified alias sets: emitter subrecord count". */
    u32 templateEntryCount : 16; /* 0x04: subrecords per particle in template clones */
    u32 : 16;
    parKindPayload_t payload; /* +8: tag-specific pointer/scalar view */
} dds3Particle_Effect_t;

typedef struct parDispatch_t {
    void *(*func)(void *);
    void (*update)(struct dds3Particle_Basic_t *);
    void (*destroy)(void *);
} parDispatch_t;

typedef struct effectBufferRecord_t {
    f32 position[4];
    f32 velocity[4]; /* 0x10 */
    s32 age;        /* 0x20: live frame age; negative values encode delays or initialization states. */
    u32 color;      /* 0x24: packed color passed to billboard and subeffect drawing. */
    f32 scale;      /* 0x28: current billboard scale */
    f32 spin;       /* 0x2C: current billboard rotation */
    f32 motionMagnitude;       /* 0x30 */
    u8 pad34[0x0C];
} effectBufferRecord_t;

typedef struct dds3Particle_Physics_t {
    sdfMemHandle_t *hMem;
    effectBufferRecord_t *pPhysics;
} dds3Particle_Physics_t;

typedef struct dds3ParticleColorAnim_t {
    s32 type;          /* 0x00: 1 = no ramp, 2 = color0-to-color1 only */
    u32 color0;        /* 0x04 */
    u32 color1;        /* 0x08 */
    u32 color2;        /* 0x0C */
    u32 alpha;         /* 0x10 */
    s32 alpha_s;  /* 0x14 */
    s32 alpha_e; /* 0x18 */
    s32 frame_end;        /* 0x1C */
    s32 frame_h;    /* 0x20 */
    f32 r01;           /* 0x24 */
    f32 r12;           /* 0x28 */
    f32 g01;           /* 0x2C */
    f32 g12;           /* 0x30 */
    f32 b01;           /* 0x34 */
    f32 b12;           /* 0x38 */
    u32 alpha_s_add;        /* 0x3C */
    u32 alpha_e_sub;       /* 0x40 */
} dds3ParticleColorAnim_t;

typedef struct effEmitterHead_t {
    f32 origin[4];         /* 0x00 */
    f32 scale[3];
    u8 pad1C[4];
    u32 particleCount;
    s32 lifetimeFrames;
    s32 billboardResource;
    s16 drawKind;
    u8 pad2E[2];
    dds3Particle_Effect_t sub;      /* 0x30: kind-tagged shared drawing owner */
    dds3ParticleColorAnim_t fade;   /* 0x48 */
    f32 scaleEnd;          /* 0x8C */
    f32 spinDegrees;       /* 0x90 */
    f32 speedJitter;       /* 0x94 */
    f32 spinJitter;        /* 0x98 */
    u8 pad9C[4];
    s32 templateSize;
    u32 restartStepCount;
    u8 padA8[8];
    f32 matrix[16];        /* 0xB0 */
    u32 colorMask;         /* 0xF0 */
    billHandle_t *billboard; /* 0xF4: owned billboard shared with the particle view */
    dds3Particle_Physics_t *buffer; /* 0xF8 */
    u32 pendingRestartSteps;
    f32 backupMatrix[16];
    u8 pad140[2];
    u16 active;            /* 0x142 */
    u8 pad144[0xC];
} effEmitterHead_t;

typedef struct effTemplatePacketList_t {
    effEmitterHead_t head; /* Common runtime prefix, 0x150 bytes. */
    u8 randomSphere; /* 0x150 */
    u8 restart; /* 0x151 */
    u8 pad152[2];
    union {
        s32 decayStep; /* 0x154: subtraction from each later packet tag */
        u32 delayRange; /* 0x154: bounded-radius spawn delay */
    };
    f32 recordScale; /* 0x158 */
    union {
        f32 tailValues[4]; /* 0x15C-0x168: variant-specific scaled values */
        u32 tailWords[4];
        struct {
            f32 targetRadius; /* 0x15C */
            f32 motionMagnitude; /* 0x160 */
            f32 accelerationPct; /* 0x164 */
            f32 unk168;
        };
    };
    dds3ParOffsetBuffParts_t *recordList; /* 0x16C: start of the history descriptors */
    s32 recordsPerPacket; /* 0x170 */
    sdfMemHandle_t *listAllocation; /* 0x174: allocation descriptor */
    void *auxiliaryData; /* 0x178: optional 16 bytes per packet */
    sdfMemHandle_t *auxiliaryAllocation; /* 0x17C: allocation descriptor */
} effTemplatePacketList_t;

typedef struct dds3Particle_Basic_t {
    f32 origin[4];                /* 0x00 */
    f32 billboardScale;           /* 0x10: radial packet's base scale */
    f32 billboardScaleY;          /* 0x14 */
    u8 pad18[8];
    s32 particleCount;            /* 0x20 */
    s32 lifetimeFrames;           /* 0x24 */
    s32 numberO;     /* 0x28: PAR_BILLBOARD_CLONE_FROM_RESOURCE */
    s16 billboardMode;            /* 0x2C */
    u8 pad2E[2];
    dds3Particle_Effect_t kindState;       /* 0x30 */
    dds3ParticleColorAnim_t colorRamp;       /* 0x48 */
    f32 scaleEnd;                /* 0x8C: rendered scale after interpolation. */
    f32 spinDegrees;             /* 0x90: converted to radians by the client update. */
    f32 billboardScaleJitter;     /* 0x94 */
    f32 initialPhaseJitter;       /* 0x98 */
    u8 alternateSpin;            /* 0x9C: selects the sign of particle rotation. */
    u8 pad9D[7];
    u32 d_frame;         /* 0xA4 */
    u8 padA8[8];
    f32 rot[16];               /* 0xB0 */
    u32 abgr;           /* 0xF0: set through dds3ParticleSetRGBA. */
    billHandle_t *hBillboard;           /* 0xF4 */
    dds3Particle_Physics_t *buffer;     /* 0xF8 */
    u32 pendingRestartSteps;      /* 0xFC: count, not a pointer */
    f32 rot_b[16];         /* 0x100 */
    u16 type;            /* 0x140: object dispatch, distinct from kind */
    u16 state;              /* 0x142 */
    u8 pad144[0xC];
} dds3Particle_Basic_t;

typedef struct parObjClientView_t {
    dds3Particle_Basic_t owner;
    u8 mode150; /* 0x150 */
    u8 mode151; /* 0x151 */
    u8 opaque152[0x26];
} parObjClientView_t;

typedef struct parKindResource_t {
    s32 type;
    s32 offset;
    u8 opaque08[8];
} parKindResource_t;

typedef struct dds3ParOffsetBuffParts_t {
    s32 point;
    s32 count;
    f32 (*points)[3];
} dds3ParOffsetBuffParts_t;

typedef struct dds3ParOffsetBuff_t {
    dds3ParOffsetBuffParts_t *pParts;
    s32 length;
} dds3ParOffsetBuff_t;

typedef struct {
    dds3Particle_Basic_t head;
    u8 axisMode;                 /* 0x150 */
    u8 loop;                     /* 0x151: rearm expired particles */
    u8 pad152[2];
    u32 spawnDelayFrames;        /* 0x154 */
    f32 initialRadius;           /* 0x158 */
    f32 rotationStepDegrees;     /* 0x15C: native updater converts to radians */
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    dds3ParOffsetBuff_t history;
    sdfMemHandle_t *childHandle;     /* 0x174: additional owned allocation */
    f32 previousOrigin[3];       /* 0x178: native updater caches origin here */
} dds3Particle_SphereSpiral2_t3;

typedef struct {
    f32 position[4];
    f32 radialOffset[3];
    u8 pad1C[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 initialPhaseRadians; /* initialized as a full-turn phase; later use is not established */
    f32 rotationAxis[3];
    f32 radiusStep;
} parBurstPacket_t;

typedef struct effParticleRecord_t {
    f32 x;
    f32 y;
    f32 z;
    u8 opaque0C[0x14];
    s32 frame;
    u32 color;
    f32 speed;
    f32 angle;
    u8 opaque30[0x10];
} effParticleRecord_t;

typedef struct effParticleOverlay_t {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 speed;
    u8 pad14[0x10];
    u32 drawEndFrame;
    u8 pad28[0x24];
    u32 color;
    u8 pad50[8];
    u32 alpha;
    u8 pad5C[0x30];
    f32 lastSpeed;
    u8 pad90[4];
    f32 speedJitter;
    f32 angleJitter;
    u8 pad9C[0x54];
    u32 colorMask;
    u8 padF4[4];
    dds3Particle_Physics_t *buffer;
} effParticleOverlay_t;

typedef struct effTemplateScaleRecord_t {
    u8 opaque00[0x20];
    u32 initialTag;
    u8 opaque24[0x10];
    f32 scale;
    u8 opaque38[8];
} effTemplateScaleRecord_t;

typedef struct dds3Particle_Radiate_t3 {
    effEmitterHead_t head; /* shared emitter prefix, 0x150 bytes */
    u8 mode;             /* 0x150 */
    u8 loop;             /* 0x151 */
    u8 opaque152[10];
    u32 period;          /* 0x15C */
    f32 rotationStepDegrees;            /* 0x160 */
    f32 stepLength;            /* 0x164 */
} dds3Particle_Radiate_t3;

/* Each emitter uses a 64-byte packet, with its own four-word motion state. */
typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 phase;
    f32 radialDistance;
    f32 verticalSpeed;
    f32 verticalOffset;
} parSpiralPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 waveAmplitude;
    f32 wavePhase;
    f32 previousWaveValue;
    f32 verticalVelocity;
} parSmokePacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    u8 unk30[8];
    f32 verticalAcceleration;
    u8 unk3C[4];
} parSparkPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    u8 unk30[4];
    f32 phaseAngle;
    u8 unk38[8];
} parRadiatePacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 axisX;
    f32 axisY;
    f32 axisZ;
    f32 radiusStep;
} parSphereSpiralPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 radius;
    u8 unk34[12];
} parSphereRingPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 motionMagnitude;
    f32 boundaryRadius;
    u8 unk38[8];
} parSphereAtomPacket_t;

typedef struct {
    f32 pos[4];
    f32 vel[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 spin;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 offsetAcceleration;
} parSparkOffsetPacket_t;

typedef struct dds3Particle_Spiral_t3 {
    effEmitterHead_t head;
    u8 flag150;
    u8 loop;
    u8 timing[6];
    f32 radius;      /* 0x158 */
    u32 period;      /* 0x15C */
    f32 verticalSpeed;
    f32 rotationStepDegrees;
    f32 targetRadius;
    f32 verticalAccelerationPct;
    f32 radiusRange; /* 0x170 */
    f32 targetRadiusJitter;  /* 0x174 */
} dds3Particle_Spiral_t3;

typedef struct dds3Particle_Smoke_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 verticalSpeed;
    f32 phaseStepDegrees;
    f32 waveAmplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 verticalSpeedJitter;     /* 0x170 */
    f32 waveAmplitudeJitter;     /* 0x174 */
} dds3Particle_Smoke_t3;

typedef struct dds3Particle_Spark_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170 */
} dds3Particle_Spark_t3;

typedef struct dds3Particle_SphereSpiral_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 speed;       /* 0x158 */
    f32 spinRate;    /* 0x15C */
    f32 targetRadius;
    f32 jitterA;     /* 0x164 */
    f32 jitterB;     /* 0x168 */
} dds3Particle_SphereSpiral_t3;

typedef struct dds3Particle_SphereSmoke_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 pad151[3];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 up_s;
    f32 phaseStepDegrees;
    f32 amplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 up_s_r;
    f32 ampli_r;
    f32 radius_s_r;
} dds3Particle_SphereSmoke_t3;

typedef struct dds3Particle_SphereRing_t3 {
    effEmitterHead_t head;
    u8 pad150;
    u8 loop;         /* 0x151 */
    u8 timing[6];
    f32 radius;      /* 0x158 */
    f32 radius_r;      /* 0x15C */
    u32 simu_num;      /* 0x160 */
    f32 roll_s;    /* 0x164 */
} dds3Particle_SphereRing_t3;

typedef struct dds3Particle_Spark2_t3 {
    effEmitterHead_t basic;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    u8 pad158[4];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160 */
    f32 gravity;     /* 0x164 */
    f32 speed;       /* 0x168 */
    f32 decayPct;    /* 0x16C */
    u8 pad170[4];
    f32 degrees;     /* 0x174 */
} dds3Particle_Spark2_t3;

typedef struct dds3Particle_SparkOffset_t3 {
    effEmitterHead_t head;
    u8 mode;         /* 0x150 */
    u8 loop;         /* 0x151 */
    u8 pad152[2];
    u32 spread;      /* 0x154: random activation delay bound */
    u8 randomDir;    /* 0x158 */
    u8 pad159[3];
    f32 radius;      /* 0x15C */
    f32 cone;        /* 0x160: horizontal direction weight */
    f32 gravity;     /* 0x164: divided by 100 for acceleration */
    f32 speed;       /* 0x168: launch magnitude */
    f32 decayPct;    /* 0x16C */
    f32 jitter;      /* 0x170: launch magnitude jitter */
} dds3Particle_SparkOffset_t3;

typedef struct dds3Particle_SmokeOffset_t3 {
    effEmitterHead_t head;
    u8 loop;
    u8 flag151;
    u8 pad152[2];
    u32 spread;      /* 0x154 */
    f32 radius;      /* 0x158 */
    f32 verticalSpeed;
    f32 phaseStepDegrees;
    f32 waveAmplitude;
    f32 waveGrowth;
    f32 verticalAccelerationPct;
    f32 verticalSpeedJitter;
    f32 waveAmplitudeJitter;
    f32 (*aux)[4];   /* 0x178: 16 bytes per packet */
} dds3Particle_SmokeOffset_t3;

enum ParKindStateKind {
    PAR_KIND_STATE_HISTORY = 1,
    PAR_KIND_STATE_BILLBOARD_CROSS_STRIP = 2,
    PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE = 3,
    PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES = 4
};

enum { PAR_BILLBOARD_CLONE_FROM_RESOURCE = -1 };

enum {
    PAR_BURST_RANDOM_AXIS = 1
};

#define EFF_TEMPLATE_TAIL_OFFSET 0x150

#define EFF_PACKET_INITIAL_TAG 0xF0000001

#define PAR_PACKET_INITIAL_TAG 0xF0000001

extern f32 parTrack_w[];

extern f32 parLook[];

extern u8 aViewTarget[], aViewEye[];

extern parDispatch_t dds3ParticleFuncTable[];

extern effRandState_t dds3EffectBase[];

extern s32 ParRand[];

extern f32 dds3ParticleSparkDirTable[][4];

extern f32 dds3ParticleSparkOffsetDirTable[][4];

extern f32 dds3ParticleNegOneVec[4];

extern u8 aViewDown[];

extern void (*dds3ParticleAllBuffInitTable[])(void *);

extern void (*dds3ParticleSetScaleTable[])(dds3Particle_Basic_t *, f32);

extern void *memset(void *a, s32 b, size_t c);

extern void *memcpy(void *a, const void *b, size_t c);

void dds3ParticleManagerInit(void);

dds3Particle_Physics_t *dds3Particle_PhysicsCreate(s32 number);

void dds3Particle_PhysicsDelete(dds3Particle_Physics_t *hPhysics);

void dds3Particle_BasicCreate(void *effect);

void dds3Particle_BasicDelete(void *pParBasic);

void dds3Particle_SpiralAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SpiralSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SpiralCreate(effTemplatePacketList_t *source);

void dds3Particle_SpiralDelete(void *hData);

void dds3Particle_SpiralBuffInit(dds3Particle_Spiral_t3 *pSpiral, u32 number);

void dds3Particle_SpiralDraw(dds3Particle_Spiral_t3 *hData);

void dds3Particle_SmokeAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SmokeSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SmokeCreate(effTemplatePacketList_t *source);

void dds3Particle_SmokeDelete(void *hData);

void dds3Particle_SmokeBuffInit(dds3Particle_Smoke_t3 *pSmoke, u32 number);

void dds3Particle_SmokeDraw(dds3Particle_Smoke_t3 *hData);

void dds3Particle_SparkAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SparkSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SparkCreate(effTemplatePacketList_t *source);

void dds3Particle_SparkDelete(void *hData);

void dds3Particle_SparkBuffInit(dds3Particle_Spark_t3 *pSpark, s32 number);

void dds3Particle_SparkDraw(dds3Particle_Spark_t3 *hData);

void dds3Particle_RadiateAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_RadiateSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_RadiateCreate(effTemplatePacketList_t *source);

void dds3Particle_RadiateDelete(void *hData);

void dds3Particle_RadiateBuffInit(dds3Particle_Radiate_t3 *pRadiate, u32 number);

void dds3Particle_RadiateDraw(dds3Particle_Radiate_t3 *hData);

void dds3Particle_SphereSpiralAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SphereSpiralSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SphereSpiralCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereSpiralDelete(void *hData);

void dds3Particle_SphereSpiralBuffInit(dds3Particle_SphereSpiral_t3 *pSphereSpiral, u32 number);

void dds3Particle_SphereSpiralDraw(dds3Particle_SphereSpiral_t3 *hData);

void dds3Particle_SphereSmokeAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SphereSmokeSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SphereSmokeCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereSmokeDelete(void *hData);

void dds3Particle_SphereSmokeBuffInit(dds3Particle_SphereSmoke_t3 *pSphereSmoke, u32 number);

void dds3Particle_SphereSmokeDraw(dds3Particle_SphereSmoke_t3 *hData);

void dds3Particle_SphereRingAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SphereRingSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SphereRingCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereRingDelete(void *hData);

void dds3Particle_SphereRingBuffInit(dds3Particle_SphereRing_t3 *pSphereRing, u32 number);

void dds3Particle_SphereRingDraw(dds3Particle_SphereRing_t3 *hData);

void dds3Particle_Spark2AllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_Spark2SetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_Spark2Create(effTemplatePacketList_t *source);

void dds3Particle_Spark2Delete(void *hData);

void dds3Particle_Spark2BuffInit(dds3Particle_Spark2_t3 *pSpark2, s32 number);

void dds3Particle_Spark2Draw(dds3Particle_Spark2_t3 *hData);

void dds3Particle_SphereAtomAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SphereAtomSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SphereAtomCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereAtomDelete(void *hData);

void dds3Particle_SphereAtomBuffInit(effTemplatePacketList_t *pSphereAtom, s32 number);

void dds3Particle_SphereAtomDraw(effTemplatePacketList_t *hData);

void dds3Particle_SpiralBlendColor(effParticleOverlay_t *effect);

void dds3Particle_SparkOffsetAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SparkOffsetSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SparkOffsetCreate(effTemplatePacketList_t *source);

void dds3Particle_SparkOffsetDelete(void *hData);

void dds3Particle_SparkOffsetBuffInit(dds3Particle_SparkOffset_t3 *pSparkOffset, s32 number);

void dds3Particle_SparkOffsetDraw(dds3Particle_SparkOffset_t3 *hData);

void dds3Particle_SmokeOffsetAllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SmokeOffsetSetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SmokeOffsetCreate(effTemplatePacketList_t *source);

void dds3Particle_SmokeOffsetDelete(effTemplatePacketList_t *hData);

void dds3Particle_SmokeOffsetBuffInit(dds3Particle_SmokeOffset_t3 *pSmokeOffset, u32 number);

void dds3Particle_SmokeOffsetDraw(dds3Particle_SmokeOffset_t3 *hData);

void dds3Particle_SphereSpiral2AllBuffInit(effTemplatePacketList_t *hData);

void dds3Particle_SphereSpiral2SetScale(effTemplatePacketList_t *hData, float scale);

effTemplatePacketList_t *dds3Particle_SphereSpiral2Create(effTemplatePacketList_t *source);

void dds3Particle_SphereSpiral2Delete(dds3Particle_SphereSpiral2_t3 *hData);

void dds3Particle_SphereSpiral2BuffInit(dds3Particle_SphereSpiral2_t3 *pSphereSpiral2, u32 number);

void dds3Particle_SphereSpiral2Draw(dds3Particle_SphereSpiral2_t3 *hData);

void dds3ParticleSetColorTable(dds3ParticleColorAnim_t *pColAnim, s32 frame);

dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 number, u16 length, billHandle_t **pBill_Handle);

void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *hBlur);

void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale);

extern u32 dds3GetKernelCount(void);

void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *hBlur, s32 index, const f32 *pPosBase, u32 abgr,
                   dds3ParOffsetBuff_t *pOffBuff, f32 scale);

void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *hBlur);

void dds3Particle_BlurClearItem(dds3Particle_BlurBuff_t *table, s32 slotIndex);

void dds3Particle_TrackSetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);

void dds3Particle_Track2SetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);

u32 dds3ParticleColorBlend(u32 color0, u32 color1);

dds3Particle_Basic_t *dds3ParticleCreate(s32 dispatchIndex, void *creationData);

void dds3ParticleDelete(dds3Particle_Basic_t *hData);

void dds3ParticleDraw(dds3Particle_Basic_t *hData);

dds3Particle_Basic_t *dds3ParticleCopy(dds3Particle_Basic_t *obj);

void dds3ParticleReset(dds3Particle_Basic_t *hData);

void dds3ParticleSetScale(dds3Particle_Basic_t *hData, f32 scale);

u16 dds3ParticleGetState(dds3Particle_Basic_t *hData);

void dds3ParticleSetPosition(void *hData, void *pPos);

void dds3ParticleSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);

void dds3ParticleSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);

void dds3ParticleSetTypeFlag(dds3Particle_Basic_t *object, s32 mode);

u8 dds3ParticleGetLoop(dds3Particle_Basic_t *hData);

dds3Particle_Basic_t *dds3ParticleExpCreate(s32 kind, parKindResource_t *resource);

dds3Particle_Basic_t *dds3ParticleExpCopy(dds3Particle_Basic_t *source);

void dds3ParticleExpDelete(dds3Particle_Basic_t *hData);

void dds3ParticleExpDraw(dds3Particle_Basic_t *hData);

void dds3ParticleExpReset(dds3Particle_Basic_t *hData);

void dds3ParticleExpSetScale(dds3Particle_Basic_t *hData, float scale);

u16 dds3ParticleExpGetState(dds3Particle_Basic_t *hData);

void dds3ParticleExpSetPosition(void *hData, void *pPos);

void dds3ParticleExpSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);

void dds3ParticleExpSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);

void dds3ParticleExpSetLoop(dds3Particle_Basic_t *hData, u8 loop);

u8 dds3ParticleExpGetLoop(dds3Particle_Basic_t *hData);

void dds3Particle_EffectInit(dds3Particle_Effect_t *obj);

void dds3Particle_EffectSet(dds3Particle_Effect_t *pEffect, s32 index, u32 abgr, f32 scale);

void dds3Particle_EffectReset(dds3Particle_Effect_t *pEffect, s32 number);

void dds3Particle_TrackSet(parSystem_t *hTrackPoly, s32 i, u32 abgr);

void dds3Particle_Track2Set(parSystem_t *hTrackPoly, s32 i, u32 abgr);

void dds3Particle_Track3Set(effTrackPolyList_t *hTrackPoly, s32 i, u32 abgr);

u32 dds3Particle_TrackPolyGetRecTiming();

void sdfRandomizeByTime(effRandState_t *aCB);

sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

void *sdfMemLock(sdfMemHandle_t *aHandle);

void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

billHandle_t *dds3BillboardGeneral(s32 number);

void dds3BillboardSetScale(billHandle_t *hBillboard, float x, float y);

void dds3BillboardSetMode(billHandle_t *hBillboard, s32 mode);

parSystem_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);

void dds3TrackPolyVCSet(void *hTrackPoly, s32 type, void *abgr_in, void *abgr_out);

void dds3TrackPolySetMode(parSystem_t *hTrackPoly, u16 mode);

effTrackPolyList_t *Eff_ModelTrackPolyListCreate(effTrackPolyParams_t *pParam, u32 number);

void dds3TrackPolyDelete(parSystem_t *hTrackPoly);

void Eff_ModelTrackPolyListDelete(effTrackPolyList_t *list);

void dds3BillboardDelete(struct billHandle_t *hData);

void *sdfChipAllocDirect(s32 aSize);

void sdfChipFreeDirect(void *aPtr);

f32 dds3GetRandFloatA(effRandState_t *name);

f32 sdfCos(f32 aRadius);

f32 sdfSin(f32 aRadius);

u32 dds3ParticleColorAnim(dds3ParticleColorAnim_t *pColAnim, u32 color_now, s32 frame);

u32 dds3GetRandA(effRandState_t *name);

void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

void ReverseMatrix(void);

void SetRotateMatrixAxis(f32 aAngle, const sdfVector3_t *aAxis);

extern void dds3BillboardDraw(billHandle_t *hData);

void dds3TrackPolyVCRefresh_ParType1(parSystem_t *hTrackPoly, s32 index);

extern void dds3TrackPolyColorSet(parSystem_t *hTrackPoly, s32 number, u32 color);

void dds3TrackPolyVCRefresh_ParType2(parSystem_t *hTrackPoly, s32 index);

void dds3TrackPolyDraw(parSystem_t *node);

void Eff_ModelTrackPolyListDrawOnly(effTrackPolyList_t *hMTPolyList);

void MultMatrixNoArg(void);

struct billHandle_t *dds3BillboardCreate(s32 kind, addr_t data);

void dds3BillboardSetAnimColorAdvMode(billHandle_t *hBillboard);

struct billHandle_t *dds3BillboardCopy(const struct billHandle_t *hBillboard);

void dds3BillboardSetAnimKey(billHandle_t *hBillboard, u32 frame);

void dds3TrackPolyListInit(parSystem_t *hTrackPoly, s32 number);

void Eff_ModelTrackPolyListReset(effTrackPolyList_t *hMTPolyList, s32 index);

void dds3TrackPolySet(parSystem_t *hTrackPoly, s32 number, const u128 *pV);

void dds3TrackPolySet2(parSystem_t *hTrackPoly, s32 number, const u128 *pV);

void Eff_ModelTrackPolyListVertexSet(effTrackPolyList_t *hMTPolyList, s32 index, const u128 *pVertex);

void Eff_ModelTrackPolyListColorSet(effTrackPolyList_t *hMTPolyList, s32 index, u32 abgr);

void func_00162270(effParticleOverlay_t *effect);

/* Both draw paths submit the resource owned by the selected particle kind. */
static __inline__ void SubmitKindDrawing(dds3Particle_Effect_t *kind) {
    switch (kind->type) {
    case PAR_KIND_STATE_HISTORY:
        dds3Particle_BlurDraw(kind->payload.history.table);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_STRIP:
        dds3TrackPolyDraw(kind->payload.crossStrip.drawSystem);
        break;
    case PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE:
        dds3TrackPolyDraw(kind->payload.crossTriangle.drawSystem);
        break;
    case PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES:
        Eff_ModelTrackPolyListDrawOnly(kind->payload.trackPolygon.modelList);
        break;
    }
}

/* Seed the generator used for per-particle launch delays. */
void dds3ParticleManagerInit(void);

void dds3ParticleManagerExit(void);

/* Allocate records followed by their owner tail; clear only two native state words per record. */
dds3Particle_Physics_t *dds3Particle_PhysicsCreate(s32 number);

void dds3Particle_PhysicsDelete(dds3Particle_Physics_t *hPhysics);

/* Allocate drawing resources and reset the emitter's common runtime state. */
/* The subrecord bitfield keeps native int-alias dependencies; see
 * dds-decomp/docs/idioms.md, "Trace-justified alias sets: emitter subrecord count". */
void dds3Particle_BasicCreate(void *effect);

/* Release the selected sub-effect resource, billboard and packet-buffer
 * allocation described by the common emitter prefix. */
void dds3Particle_BasicDelete(void *pParBasic);

/* Stagger native tags in period-sized groups; a nonempty buffer requires a nonzero period. */
void dds3Particle_SpiralAllBuffInit(effTemplatePacketList_t *hData);

/* Scale the position, record scale, and this variant's two tail floats. */
void dds3Particle_SpiralSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the template prefix and appended tail, then allocate its packet records. */
effTemplatePacketList_t *dds3Particle_SpiralCreate(effTemplatePacketList_t *source);

void dds3Particle_SpiralDelete(void *hData);

/* Seed one ring packet's phase, radius and vertical motion, then its subeffect. */
void dds3Particle_SpiralBuffInit(dds3Particle_Spiral_t3 *pSpiral, u32 number);

/* Advance ring motion and ages; repeat by tagging packets for the next update.
 * completedCount counts expired nonrepeating packets in this update only. */
/* vf12 keeps the packet's previous position and vf10 the new one for
 * dds3Particle_EffectSet, which reads them as implicit arguments. */
void dds3Particle_SpiralDraw(dds3Particle_Spiral_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeAllBuffInit(effTemplatePacketList_t *hData);

/* Scale template position, record scale, and this variant's two tail values. */
void dds3Particle_SmokeSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the disc prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SmokeCreate(effTemplatePacketList_t *source);

void dds3Particle_SmokeDelete(void *hData);

/* Seed a random planar position and direction; radialDistance may be negative. */
void dds3Particle_SmokeBuffInit(dds3Particle_Smoke_t3 *pSmoke, u32 number);

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SmokeDraw(dds3Particle_Smoke_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkAllBuffInit(effTemplatePacketList_t *hData);

/* Scale the position and three variant-specific tail values. */
void dds3Particle_SparkSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the ballistic prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SparkCreate(effTemplatePacketList_t *source);

void dds3Particle_SparkDelete(void *hData);

/* Spark launch initialization retains an s4/s5 RNG-address allocation residual. */
/* Scale velocity, add its stored vertical acceleration, and advance position.
 * Only mode zero transforms the displacement through the emitter matrix. */
void dds3Particle_SparkDraw(dds3Particle_Spark_t3 *hData);

/* Spawn each packet, then overwrite its tag with a zero-based, period-grouped schedule. */
void dds3Particle_RadiateAllBuffInit(effTemplatePacketList_t *hData);

/* Scale position and the third variant-specific tail value. */
void dds3Particle_RadiateSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the look-at ring prefix/tail and spawn its staggered packet schedule. */
effTemplatePacketList_t *dds3Particle_RadiateCreate(effTemplatePacketList_t *source);

void dds3Particle_RadiateDelete(void *hData);

/* Seed a view-oriented ring packet and initialize its subeffect. */
void dds3Particle_RadiateBuffInit(dds3Particle_Radiate_t3 *pRadiate, u32 number);

/* Advance view-oriented motion and ages; repeated packets respawn immediately. */
void dds3Particle_RadiateDraw(dds3Particle_Radiate_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiralAllBuffInit(effTemplatePacketList_t *hData);

/* Scale position, record scale, and the second tail value. */
void dds3Particle_SphereSpiralSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the burst prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SphereSpiralCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereSpiralDelete(void *hData);

/* Seed burst velocity and an optional random axis; retain the native spawn delay. */
void dds3Particle_SphereSpiralBuffInit(dds3Particle_SphereSpiral_t3 *pSphereSpiral, u32 number);

/* Adjust velocity length, rotate around the packet axis, and rebuild position. */
void dds3Particle_SphereSpiralDraw(dds3Particle_SphereSpiral_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSmokeAllBuffInit(effTemplatePacketList_t *hData);

/* Scale template position, record scale, and this variant's tail values. */
void dds3Particle_SphereSmokeSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the sphere prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SphereSmokeCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereSmokeDelete(void *hData);

/* Seed a jittered spherical position and planar direction.
 * Native delay uses bitwise complement, not unary negation. */
void dds3Particle_SphereSmokeBuffInit(dds3Particle_SphereSmoke_t3 *pSphereSmoke, u32 number);

/* Advance sinusoidal planar displacement and scaled vertical steps, then ages. */
void dds3Particle_SphereSmokeDraw(dds3Particle_SphereSmoke_t3 *hData);

/* Stagger native tags using this variant's second tail word as the group period. */
void dds3Particle_SphereRingAllBuffInit(effTemplatePacketList_t *hData);

/* Scale the position and record scale of a short template. */
void dds3Particle_SphereRingSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the expanding-ring prefix/tail and initialize its period-grouped packet tags. */
effTemplatePacketList_t *dds3Particle_SphereRingCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereRingDelete(void *hData);

/* Seed radius and ring direction, placing the packet at its rotated vertical offset. */
void dds3Particle_SphereRingBuffInit(dds3Particle_SphereRing_t3 *pSphereRing, u32 number);

/* Build age-phase ring positions and rotate the stored X/Z directions.
 * vector is reused for the position offset and old direction components. */
void dds3Particle_SphereRingDraw(dds3Particle_SphereRing_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_Spark2AllBuffInit(effTemplatePacketList_t *hData);

/* Scale template position and three variant-specific tail values. */
void dds3Particle_Spark2SetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the cone prefix/tail, allocate packet records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_Spark2Create(effTemplatePacketList_t *source);

void dds3Particle_Spark2Delete(void *hData);

/* Seed mode-zero cone position/velocity; every mode initializes age and subeffect. */
void dds3Particle_Spark2BuffInit(dds3Particle_Spark2_t3 *pSpark2, s32 number);

/* Scale velocity, apply stored gravity, and transform each motion step before aging. */
void dds3Particle_Spark2Draw(dds3Particle_Spark2_t3 *hData);

/* Initialize each packet's native tag and its template-selected uniform scale. */
void dds3Particle_SphereAtomAllBuffInit(effTemplatePacketList_t *hData);

/* Scale the position, record scale, and first two tail values. */
void dds3Particle_SphereAtomSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the prefix/tail and initialize each packet record with the template's uniform scale. */
effTemplatePacketList_t *dds3Particle_SphereAtomCreate(effTemplatePacketList_t *source);

void dds3Particle_SphereAtomDelete(void *hData);

/* vu0 routine: seed radial placement, normalized velocity and activation age. */
void dds3Particle_SphereAtomBuffInit(effTemplatePacketList_t *pSphereAtom, s32 number);

/* Reflect moving packets at the frame's entry radius. The first record's
 * updated radius becomes the boundary on the following frame. */
void dds3Particle_SphereAtomDraw(effTemplatePacketList_t *hData);

void dds3Particle_SpiralRandomize(effParticleOverlay_t *effect);

void func_001621A0(effEmitterHead_t *head, f32 factor);

effEmitterHead_t *func_001621C8(effEmitterHead_t *source);

void func_00162248(effEmitterHead_t *object);

void func_00162270(effParticleOverlay_t *effect);

/* Frame zero reinitializes to -1 before incrementing; refresh position and modulated packed color. */
void dds3Particle_SpiralBlendColor(effParticleOverlay_t *effect);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SparkOffsetAllBuffInit(effTemplatePacketList_t *hData);

/* Scale the position and three variant-specific tail values. */
void dds3Particle_SparkOffsetSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the offset-gravity prefix/tail, allocate records, and initialize their native age tags. */
effTemplatePacketList_t *dds3Particle_SparkOffsetCreate(effTemplatePacketList_t *source);

void dds3Particle_SparkOffsetDelete(void *hData);

/* Accumulate motion in the stored offset, then add the emitter origin.
 * Only mode zero transforms the per-frame displacement through the matrix. */
void dds3Particle_SparkOffsetDraw(dds3Particle_SparkOffset_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SmokeOffsetAllBuffInit(effTemplatePacketList_t *hData);

/* Scale position, record scale, and two tail values. */
void dds3Particle_SmokeOffsetSetScale(effTemplatePacketList_t *hData, float scale);

/* Clone the disc prefix/tail plus a separate 16-byte-per-packet auxiliary allocation. */
effTemplatePacketList_t *dds3Particle_SmokeOffsetCreate(effTemplatePacketList_t *source);

void dds3Particle_SmokeOffsetDelete(effTemplatePacketList_t *hData);

/* Seed the auxiliary position and packet direction separately, then the subeffect. */
void dds3Particle_SmokeOffsetBuffInit(dds3Particle_SmokeOffset_t3 *pSmokeOffset, u32 number);

/* Advance auxiliary positions, then transform/store packet positions with origin.
 * Auxiliary accumulation remains separate from the transformed packet position. */
void dds3Particle_SmokeOffsetDraw(dds3Particle_SmokeOffset_t3 *hData);

/* Initialize only the native age/tag word of each packet record. */
void dds3Particle_SphereSpiral2AllBuffInit(effTemplatePacketList_t *hData);

/* Scale template position, record scale, and the second tail value. */
void dds3Particle_SphereSpiral2SetScale(effTemplatePacketList_t *hData, float scale);

/* Allocate one 12-byte descriptor per packet and its attached 16-byte records. */
effTemplatePacketList_t *dds3Particle_SphereSpiral2Create(effTemplatePacketList_t *source);

/* Release the radial emitter's extra allocation, shared resources, and block. */
void dds3Particle_SphereSpiral2Delete(dds3Particle_SphereSpiral2_t3 *hData);

/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void dds3Particle_SphereSpiral2BuffInit(dds3Particle_SphereSpiral2_t3 *pSphereSpiral2, u32 number);

/* Grow each radial offset toward its target radius, spin it about the particle axis,
 * rebuild position from the origin, and feed the kind's point history. */
void dds3Particle_SphereSpiral2Draw(dds3Particle_SphereSpiral2_t3 *hData);

void dds3ParticleSetColorTable(dds3ParticleColorAnim_t *pColAnim, s32 frame);

u32 dds3ParticleColorAnim(dds3ParticleColorAnim_t *pColAnim, u32 color_now, s32 frame);

/* Allocate contiguous point slabs, slot records and the trailing table header. */
dds3Particle_BlurBuff_t *dds3Particle_BlurCreate(u32 number, u16 length, billHandle_t **pBill_Handle);

/* Release the slot table's allocation handle, not a separate node object. */
void dds3Particle_BlurDelete(dds3Particle_BlurBuff_t *hBlur);

void dds3Particle_BlurSet(dds3Particle_BlurBuff_t *table, s32 slotIndex, u32 color, f32 billboardScale);

void dds3Particle_BlurSetExp(dds3Particle_BlurBuff_t *hBlur, s32 index, const f32 *pPosBase, u32 abgr,
                   dds3ParOffsetBuff_t *pOffBuff, f32 scale);

void dds3Particle_BlurDraw(dds3Particle_BlurBuff_t *hBlur);

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void dds3Particle_BlurClearItem(dds3Particle_BlurBuff_t *table, s32 slotIndex);

/* Rebuild one slot's two-vertex strip cells from its point history: each cell is
   the offset point +/- the cross product of the segment direction with the view axis,
   scaled by the strip width. */
void dds3Particle_TrackSetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);

/* Triangle-cell variant of dds3Particle_TrackSetExp: three vertices per history segment. */
void dds3Particle_Track2SetExp(parSystem_t *hTrackPoly, s32 index, const f32 *pPos, u32 abgr, dds3ParOffsetBuff_t *pOffBuff);

/* The colour inlines read and write a 16-byte slot, one word of it used. */
typedef struct {
    u32 word;
    u8 pad04[12];
} parColorWork_t;

/* vu0 routine: unpack two colours, multiply the lanes and pack */
u32 dds3ParticleColorBlend(u32 color0, u32 color1);

/* Create through the selected dispatch entry and record its index; returns the
 * new instance, which dds3EffectCreate keeps. */
dds3Particle_Basic_t *dds3ParticleCreate(s32 dispatchIndex, void *creationData);

/* Invoke the selected destructor on the particle. */
void dds3ParticleDelete(dds3Particle_Basic_t *hData);

/* Draw live particles after applying the requested number of emitter updates. */
void dds3ParticleDraw(dds3Particle_Basic_t *hData);

/* Create another object of the same dispatch index and return the new instance. */
dds3Particle_Basic_t *dds3ParticleCopy(dds3Particle_Basic_t *obj);

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void dds3ParticleReset(dds3Particle_Basic_t *hData);

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void dds3ParticleSetScale(dds3Particle_Basic_t *hData, f32 scale);

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 dds3ParticleGetState(dds3Particle_Basic_t *hData);

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void dds3ParticleSetPosition(void *hData, void *pPos);

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void dds3ParticleSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);

void dds3ParticleSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);

void dds3ParticleSetTypeFlag(dds3Particle_Basic_t *object, s32 mode);

u8 dds3ParticleGetLoop(dds3Particle_Basic_t *hData);

dds3Particle_Basic_t *dds3ParticleExpCreate(s32 kind, parKindResource_t *resource);

dds3Particle_Basic_t *dds3ParticleExpCopy(dds3Particle_Basic_t *source);

void dds3ParticleExpDelete(dds3Particle_Basic_t *hData);

/* Apply queued updates, then draw live records and submit the kind's resource. */
void dds3ParticleExpDraw(dds3Particle_Basic_t *hData);

void dds3ParticleExpReset(dds3Particle_Basic_t *hData);

void dds3ParticleExpSetScale(dds3Particle_Basic_t *hData, float scale);

/* Descriptor query: return the complete 16-bit restart state. */
u16 dds3ParticleExpGetState(dds3Particle_Basic_t *hData);

void dds3ParticleExpSetPosition(void *hData, void *pPos);

void dds3ParticleExpSetRotateMatrix(dds3Particle_Basic_t *hData, void *pMatrix);

void dds3ParticleExpSetRGBA(dds3Particle_Basic_t *hData, u32 abgr);

void dds3ParticleExpSetLoop(dds3Particle_Basic_t *hData, u8 loop);

/* Descriptor query: select the kind's current mode byte. */
u8 dds3ParticleExpGetLoop(dds3Particle_Basic_t *hData);

/* Kinds 2-4 keep the scale at +8 of their own record; copy it into the
 * shared vector and store the (vf10 - vf11) difference. */
void dds3Particle_EffectInit(dds3Particle_Effect_t *obj);

void dds3Particle_EffectSet(dds3Particle_Effect_t *pEffect, s32 index, u32 abgr, f32 scale);

void dds3Particle_EffectReset(dds3Particle_Effect_t *pEffect, s32 number);

void dds3Particle_TrackSet(parSystem_t *hTrackPoly, s32 i, u32 abgr);

void dds3Particle_Track2Set(parSystem_t *hTrackPoly, s32 i, u32 abgr);

void dds3Particle_Track3Set(effTrackPolyList_t *hTrackPoly, s32 i, u32 abgr);

u32 dds3Particle_TrackPolyGetRecTiming(void);

#endif
