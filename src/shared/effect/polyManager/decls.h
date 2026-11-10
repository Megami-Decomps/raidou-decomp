/* Declarations of smt3-decomp src/smt3/effect/polyManager.c, for its bodies in this directory. */
#ifndef SHARED_EFFECT_POLYMANAGER_H
#define SHARED_EFFECT_POLYMANAGER_H

#include "common.h"

#include "draw3d.h"

#include "ps2asm.h"

#include "sdfFrame.h"

typedef struct
{
    u128 *pVertex;
    u32 *pVC;
    s32 count;
    s32 rec;
    u32 color;
} dds3TrackPolyList_t;

typedef struct dds3TrackPoly_t
{
    u16 type;
    u16 mode;
    s32 number;
    s32 point;
    s32 rec;
    sdfMemHandle_t *hMem;
    dds3TrackPolyList_t *trackList;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    struct dds3TrackPoly_t *next;
    s32 d_next;
} dds3TrackPoly_t;

/* Polygon owners follow the variable-sized attribute arrays in one allocation. */
typedef struct dds3Polygon_t
{
    s16 flags;
    u8 unk02[2];
    s32 capacity;
    s32 strip_count;
    s32 drawCount;
    sdfMemHandle_t *hMem;
    sdf3DPrimPolyParm_t parm;
    sdfMat_t *pMat;
    u128 *pVertex;
    u32 (*texcoords)[2];
    u128 *pNormal;
    u32 *pVertexColor;
    struct dds3Polygon_t *next;
    s32 d_next;
} dds3Polygon_t;

typedef struct
{
    s32 number;
    u32 abgr;
    u128 *pVertex;
    u32 *pVC;
    sdfMat_t *pMat;
    sdfMemHandle_t *hMem;
} dds3TrianglePoly_t;

enum trackTopology
{
    TRACK_TOPOLOGY_PAIR,
    TRACK_TOPOLOGY_TRIANGLE,
    TRACK_TOPOLOGY_SIX_VECTOR,
    TRACK_TOPOLOGY_FOUR_VECTOR,
    TRACK_TOPOLOGY_FIVE_VECTOR
};

/* Packed color words written in the vertex order used by each cell geometry. */
typedef struct trackTriangleColors_t {
    s32 edge0;  /* 0x00 */
    s32 middle; /* 0x04 */
    s32 edge1;  /* 0x08 */
} trackTriangleColors_t;

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern s32 sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfMat_t *sdfMatNew(void);

extern void sdfMatFreeDelayed(sdfMat_t *aMat);

extern void sdfMatSetConstant(sdfMat_t *aMat, f32 aRatio);

extern void sdfMatSetTex(sdfMat_t *aMat, sdfTexHandle_t *aTex);

extern s32 sdfGrAllocPacketBuff(s32 aSize);

extern void sdfDrawTagInit(sdfDrawTag_t *a);

extern void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

extern void sdf3DPrimLinkResetTm(sdfDrawTag_t *aTag, s32 aAllocator);

extern void sdf3DPrimLinkMat(sdfDrawTag_t *aTag, sdfMat_t *aMat, s32 aAllocator);

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *aParm);

extern f32 dds3TrackPolyVCType1Param[4];

extern f32 dds3TrackPolyVCType2Param[4];

extern f32 dds3TrackPolyVCSetAcuteParam[4];

extern f32 dds3TrackPolyVCSetAcuteWParam[4];

extern f32 dds3TrackPolyVCAcuteParam[4];

extern void (*VCSetFunc[])(void *, void *, void *);

extern u32 dds3TrackPolyPairFaceIndex[];

extern u32 dds3TrackPolyTriangleFaceIndex[];

extern u32 dds3TrackPolyFiveVectorFaceIndex[];

extern u32 dds3TrackPolySixVectorFaceIndex[];

extern u32 dds3TrackPolyDefaultFaceIndex[];

extern sdfGrOt_t *dds3TrackPolyOtTable[];

extern sdfGrOt_t sdfGrOtTable[85];

void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number);

extern dds3TrackPoly_t *dds3TrackPolyTag;

typedef struct dds3SpPoly_Basic_t {
    f32 origin[4];
    u32 number; /* 0x10: number of trailing per-entry records. */
    s32 frame;
    u32 templateSize; /* Byte length copied from the source template. */
    u8 pad1C[4];
    f32 matrix[16];
    u32 abgr;
    u8 loop;
    u8 pad65;
    u16 mode; /* 0x66: selected particle draw bucket. */
    u32 alpha_s;
    u32 alpha_e;
    f32 matrixCopy[16]; /* Initialized from matrix when the owner is created. */
    u16 type;
    u16 state; /* Cleared when every entry finishes. */
    u8 padB4[0xC];
} dds3SpPoly_Basic_t;

typedef struct dds3SpPoly_RingPhysics_t {
    s32 frame;
    f32 spread_s;
} dds3SpPoly_RingPhysics_t;

typedef struct dds3SpPoly_SphereRingPhysics_t {
    s32 frame;
    u32 radius; /* Native initializer converts a float to u32. */
} dds3SpPoly_SphereRingPhysics_t;

typedef struct dds3SpPoly_RollRingPhysics_t {
    s32 frame;
    f32 radius;
    f32 spread_s;
    f32 rot_x;
    f32 rot_y;
} dds3SpPoly_RollRingPhysics_t;

typedef struct dds3SpPoly_Ring_Old_t3 {
    f32 origin[4];
    u16 entryCount;
    u8 pad12[2];
    s32 duration;
    s32 headerSize;
    u8 pad1C[4];
    f32 matrix[16];
    u32 color;
    u8 templateLoop;
    u8 pad65;
    u16 drawBucket;
    u32 startColorRampFrames;
    u32 endColorRampFrames;
    f32 matrixCopy[16];
    u16 handler;
    u16 active;
    u8 padB4[0xC];
    u8 loop;
    u8 padC1[3];
    s32 spawnDelayStep;
    u16 segments;
    u8 padCA[2];
    f32 radius;
    f32 pairDisplacement;
    u8 padD4[8];
    dds3TrackPoly_t *hTrackPoly;
    s32 *ages;
} dds3SpPoly_Ring_Old_t3;

typedef struct dds3SpPoly_Ring_t3 {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 radialWidth;
    f32 initialRadius;
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    f32 liftStep;
    u8 padE0[4];
    void *dispatchArgB;             /* +0xE4 */
    u8 padE8[4];
    void *dispatchArgA;             /* +0xEC */
    struct dds3TrackPoly_t *hTrackPoly;         /* +0xF0 */
    dds3SpPoly_RingPhysics_t *pPhysics;         /* +0xF4 */
    struct sdfMemHandle_t *hMem;  /* +0xF8 */
    u8 padFC[4];
} dds3SpPoly_Ring_t3;

typedef struct dds3SpPolyOldCellParam_t {
    u8 pad00[8];
    u16 verticesPerCell; /* 0x08 */
    u8 pad0A[0xA];
    void *dispatchArgA;
    void *dispatchArgB;          /* 0x14: forwarded as two opaque callback arguments */
    struct dds3TrackPoly_t *cellSystem; /* 0x1C: allocated cell system */
    u32 *cellWords;       /* 0x20: inline per-cell word array */
    u8 pad24[0xC];
} dds3SpPolyOldCellParam_t;

/* Fixed 0xF0 descriptor prefix; variable per-cell words begin at +0xF0. */
typedef struct dds3SpPolyOldDesc_t {
    u8 pad00[0x10];
    s32 count;            /* 0x10 */
    u8 pad14[4];
    s32 headerSize;       /* 0x18: serialized-header byte count */
    u8 pad1C[0xA4];
    dds3SpPolyOldCellParam_t cells; /* 0xC0 */
} dds3SpPolyOldDesc_t;

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

typedef struct {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 radialWidth;
    f32 radius;
    f32 radius_r;
    void *dispatchArgB;            /* 0xD4 */
    u8 padD8[4];
    void *dispatchArgA;            /* 0xDC */
    dds3TrackPoly_t *hTrackPoly;              /* 0xE0 */
    dds3SpPoly_SphereRingPhysics_t *pPhysics;        /* 0xE4 */
    struct sdfMemHandle_t *hMem; /* 0xE8 */
    u8 padEC[4];
} dds3SpPoly_SphereRing_t3; /* 0xF0, followed by eight-byte pPhysics */

typedef struct {
    dds3SpPoly_Basic_t head;
    u32 spawnDelayStep;
    u32 vertex;
    f32 width;
    f32 radius_s;
    f32 radius_e;
    f32 radius_s_r;
    f32 radius_e_r;
    u32 simu_num;
    f32 incline;
    f32 roll_s;
    void *color_edge_in;            /* 0xE8 */
    u8 color[4];
    void *color_edge_out;            /* 0xF0 */
    dds3TrackPoly_t *hTrackPoly;              /* 0xF4 */
    dds3SpPoly_RollRingPhysics_t *pPhysics; /* 0xF8 */
    struct sdfMemHandle_t *allocation; /* 0xFC */
} dds3SpPoly_RollRing_t3; /* 0x100, followed by 20-byte pPhysics */

/* The ring kinds share the template prefix and their leading parameter words;
 * the creation paths clamp the segment count through this shared view. */
typedef union {
    dds3SpPoly_Ring_t3 band;
    dds3SpPoly_SphereRing_t3 arc;
    dds3SpPoly_RollRing_t3 rotating;
} dds3SpPolyRingUnion_t;

typedef struct
{
    void *(*func)(void *params);
    void (*callback)(void *work);
    void (*destroy)(void *work);
} dds3SpPolyEntry_t;

typedef struct
{
    u8 unk00[0x64];
    u8 loop;
    u8 unk65[0x4B];
    u16 handler;
    u16 active;
    u8 unkB4[0xC];
    u8 oldLoop;
} dds3SpPolyLoopWork_t;

extern void *memcpy(void *a, const void *b, size_t c);

extern void *memset(void *a, s32 b, size_t c);

extern dds3SpPolyEntry_t dds3SpPolyTable[];

extern void (*dds3SpPolyResetTable[])(void *work);

extern void (*dds3SpPolySetScaleTable[])(void *work, f32 scale);

extern f32 dds3SpPolyColorTo[4];

extern f32 dds3SpPolyColorFrom[4];

extern f32 dds3PolygonTriangleParam[4];

extern u8 dds3EffectBase[];

extern f32 dds3GetRandFloatA(void *name);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern f32 sdfCos(f32 aRadius);

extern f32 sdfSin(f32 aRadius);

extern void SetRotateMatrixX(f32 aAngle);

extern void SetRotateMatrixY_B(f32 aAngle);

extern void MultMatrixRevNoArg(void);

extern void MultMatrixNoArg(void);

extern void dds3SpPoly_RingBuffInit(dds3SpPoly_Ring_t3 *pRing, s32 number);

/* Separate geometry builder reached by the initializer's native tail call. */
extern void func_00168A50(dds3SpPoly_Ring_t3 *ring, s32 number, f32 radius);

dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(dds3SpPolyOldDesc_t *src);

void dds3SpPoly_Ring_Old_Delete(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_BuffInit(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_SetScale(dds3SpPoly_Ring_Old_t3 *hData, f32 scale);

void dds3SpPoly_Ring_Old_Calc(dds3SpPoly_Ring_Old_t3 *pRing);

void dds3SpPoly_Ring_Old_Draw(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_New(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);

void dds3SpPoly_Ring_Old_Move(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);

u32 dds3SpPoly_Color(u32 frame, u32 frame_end, u32 abgr);

dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(dds3SpPoly_Basic_t *templateHead);

void dds3SpPoly_RingDelete(dds3SpPoly_Ring_t3 *hData);

void dds3SpPoly_RingAllBuffInit(dds3SpPoly_Ring_t3 *band);

void dds3SpPoly_RingMove(dds3SpPoly_Ring_t3 *pRing, s32 number);

void dds3SpPoly_RingSetScale(dds3SpPoly_Ring_t3 *hData, f32 scale);

void dds3SpPoly_RingDraw(dds3SpPoly_Ring_t3 *hData);

dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(dds3SpPoly_SphereRing_t3 *source);

void dds3SpPoly_SphereRingDelete(dds3SpPoly_SphereRing_t3 *hData);

void dds3SpPoly_SphereRingAllBuffInit(dds3SpPoly_SphereRing_t3 *arc);

void dds3SpPoly_SphereRingBuffInit(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);

void dds3SpPoly_SphereRingMove(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);

void dds3SpPoly_SphereRingSetScale(dds3SpPoly_SphereRing_t3 *hData, f32 scale);

void dds3SpPoly_SphereRingDraw(dds3SpPoly_SphereRing_t3 *hData);

dds3SpPoly_RollRing_t3 *dds3SpPoly_RollRingCreate(dds3SpPoly_Basic_t *templateHead);

void dds3SpPoly_RollRingDelete(dds3SpPoly_RollRing_t3 *hData);

void dds3SpPoly_RollRingAllBuffInit(dds3SpPoly_RollRing_t3 *rotatingBand);

void dds3SpPoly_RollRingBuffInit(dds3SpPoly_RollRing_t3 *pRollRing, u32 number);

void dds3SpPoly_RollRingMove(dds3SpPoly_RollRing_t3 *pRollRing, s32 number);

void dds3SpPoly_RollRingSetScale(dds3SpPoly_RollRing_t3 *hData, f32 scale);

void dds3SpPoly_RollRingSetEndless(dds3SpPoly_RollRing_t3 *pRollRing);

void *dds3SpPolyCreate(s32 index, void *arg);

void *dds3SpPolyCopy(dds3SpPoly_Basic_t *effect);

void dds3SpPolyDelete(dds3SpPoly_Basic_t *hData);

void dds3SpPolyDraw(dds3SpPoly_Basic_t *hData);

void dds3SpPolyReset(dds3SpPoly_Basic_t *hData);

void dds3SpPolySetScale(dds3SpPoly_Basic_t *hData, f32 scale);

u16 dds3SpPolyGetState(dds3SpPoly_Basic_t *hData);

void dds3SpPolySetPosition(void *hData, void *pPos);

void dds3SpPolySetRotateMatrix(dds3SpPoly_Basic_t *hData, void *pMatrix);

void dds3SpPolySetRGBA(dds3SpPoly_Basic_t *hData, u32 abgr);

void dds3SpPolySetLoop(dds3SpPolyLoopWork_t *hData, u8 loop);

u8 dds3SpPolyGetLoop(dds3SpPolyLoopWork_t *hData);

void dds3PolygonSetColor(dds3Polygon_t *hPolygon, u32 color);

void dds3PolygonTriangleSet(dds3Polygon_t *hPolygon, const f32 *center);

void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color);

extern dds3Polygon_t *hDrawListTop;

extern u8 dds3PolygonRandState[];

extern sdf3DPrimPolyParm_t dds3PolygonPrimParm;

extern void sdfRandomizeByTime(void *aCB);

void dds3TrackPolyInit(void);

void dds3PolygonManagerInit(void);

void dds3PolygonManagerExit(void);

dds3Polygon_t *dds3PolygonCreate(s32 face, s16 type, s16 attr);

void dds3PolygonDelete(dds3Polygon_t *hPolygon);

void dds3PolygonDraw(dds3Polygon_t *hPolygon);

extern sdfGrOt_t sdfGrOtTable[85];

SYMBOL_AT(sdfGrOt_t, D_002F64B8, sdfGrOtTable, 0x5A0);

static inline void parAppendDrawSurfacePool(sdfGrOt_t *pool, sdfDrawTag_t *packet) {
    pool->regFunc(pool, packet);
}

void dds3PolygonLinkOt(void);

void dds3PolygonSetTex(dds3Polygon_t *hPolygon, sdfTexHandle_t *hTex);

void dds3TrackPolyInit(void);

dds3TrackPoly_t *dds3TrackPolyCreate(s32 number, s32 face, s32 rec, u32 type);

void dds3TrackPolyDelete(dds3TrackPoly_t *hTrackPoly);

void dds3TrackPolyListInit(dds3TrackPoly_t *hTrackPoly, s32 number);

void dds3TrackPolyDraw(dds3TrackPoly_t *node);

void dds3TrackPolySet(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV);

void dds3TrackPolyMove(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pMove);

void dds3TrackPolySet2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pV);

void dds3TrackPolyMove2(dds3TrackPoly_t *hTrackPoly, s32 number, const u128 *pMove);

void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCRefresh_ParType1(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCRefresh_ParType2(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCRefresh_ParType2(dds3TrackPoly_t *hTrackPoly, s32 index);

void dds3TrackPolyVCSet_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 color);

void dds3TrackPolyVCSet2_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet2_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet2_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVC3Set_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVC3Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVC3Set_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVCSet4_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet4_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVCSet4_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_in, u32 abgr_out);

void dds3TrackPolyVC5Set_Normal(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVC5Set_Acute(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVC5Set_AcuteR(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolyVC5Set_AcuteW(dds3TrackPoly_t *hTrackPoly, u32 abgr_0, u32 abgr_1, u32 abgr_2);

void dds3TrackPolySetMode(dds3TrackPoly_t *hTrackPoly, u16 mode);

void dds3TrackPolyVCSet(dds3TrackPoly_t *hTrackPoly, s32 type, void *abgr_in, void *abgr_out);

void dds3TrackPolyLinkOt(void);

dds3TrianglePoly_t *dds3TrianglePolyCreate(s32 number);

void dds3TrianglePolyDelete(dds3TrianglePoly_t *hTriPoly);

void dds3TrianglePolyLinkOt(sdfGrOt_t *pOt, dds3TrianglePoly_t *hTriPoly);

dds3SpPolyOldDesc_t *dds3SpPoly_Ring_Old_Create(dds3SpPolyOldDesc_t *src);

void dds3SpPoly_Ring_Old_Delete(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_BuffInit(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_SetScale(dds3SpPoly_Ring_Old_t3 *hData, f32 scale);

void dds3SpPoly_Ring_Old_Calc(dds3SpPoly_Ring_Old_t3 *pRing);

void dds3SpPoly_Ring_Old_Draw(dds3SpPoly_Ring_Old_t3 *hData);

void dds3SpPoly_Ring_Old_New(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);

void dds3SpPoly_Ring_Old_Move(dds3SpPoly_Ring_Old_t3 *pRing, s32 number);

u32 dds3SpPoly_Color(u32 frame, u32 frame_end, u32 abgr);

dds3SpPoly_Ring_t3 *dds3SpPoly_RingCreate(dds3SpPoly_Basic_t *templateHead);

void dds3SpPoly_RingDelete(dds3SpPoly_Ring_t3 *hData);

void dds3SpPoly_RingAllBuffInit(dds3SpPoly_Ring_t3 *band);

void dds3SpPoly_RingBuffInit(dds3SpPoly_Ring_t3 *ring, s32 number);

void func_00168A50(dds3SpPoly_Ring_t3 *ring, s32 number, f32 radius);

void dds3SpPoly_RingMove(dds3SpPoly_Ring_t3 *pRing, s32 number);

void dds3SpPoly_RingSetScale(dds3SpPoly_Ring_t3 *hData, f32 scale);

void dds3SpPoly_RingDraw(dds3SpPoly_Ring_t3 *hData);

dds3SpPoly_SphereRing_t3 *dds3SpPoly_SphereRingCreate(dds3SpPoly_SphereRing_t3 *source);

void dds3SpPoly_SphereRingDelete(dds3SpPoly_SphereRing_t3 *hData);

void dds3SpPoly_SphereRingAllBuffInit(dds3SpPoly_SphereRing_t3 *arc);

void dds3SpPoly_SphereRingBuffInit(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);

void dds3SpPoly_SphereRingMove(dds3SpPoly_SphereRing_t3 *pSphereRing, s32 number);

void dds3SpPoly_SphereRingSetScale(dds3SpPoly_SphereRing_t3 *hData, f32 scale);

void dds3SpPoly_SphereRingDraw(dds3SpPoly_SphereRing_t3 *hData);

dds3SpPoly_RollRing_t3 *dds3SpPoly_RollRingCreate(dds3SpPoly_Basic_t *templateHead);

void dds3SpPoly_RollRingDelete(dds3SpPoly_RollRing_t3 *hData);

void dds3SpPoly_RollRingAllBuffInit(dds3SpPoly_RollRing_t3 *rotatingBand);

void dds3SpPoly_RollRingBuffInit(dds3SpPoly_RollRing_t3 *pRollRing, u32 number);

void dds3SpPoly_RollRingMove(dds3SpPoly_RollRing_t3 *pRollRing, s32 number);

void dds3SpPoly_RollRingSetScale(dds3SpPoly_RollRing_t3 *hData, f32 scale);

void dds3SpPoly_RollRingDraw(dds3SpPoly_RollRing_t3 *hData);

void dds3SpPoly_RollRingSetEndless(dds3SpPoly_RollRing_t3 *pRollRing);

void *dds3SpPolyCreate(s32 index, void *arg);

void *dds3SpPolyCopy(dds3SpPoly_Basic_t *effect);

void dds3SpPolyDelete(dds3SpPoly_Basic_t *hData);

void dds3SpPolyDraw(dds3SpPoly_Basic_t *hData);

void dds3SpPolyReset(dds3SpPoly_Basic_t *hData);

void dds3SpPolySetScale(dds3SpPoly_Basic_t *hData, f32 scale);

u16 dds3SpPolyGetState(dds3SpPoly_Basic_t *hData);

void dds3SpPolySetPosition(void *hData, void *pPos);

/* vu0 routine: compose the new rotation with the retained source matrix. */
void dds3SpPolySetRotateMatrix(dds3SpPoly_Basic_t *hData, void *pMatrix);

void dds3SpPolySetRGBA(dds3SpPoly_Basic_t *hData, u32 abgr);

void dds3SpPolySetLoop(dds3SpPolyLoopWork_t *hData, u8 loop);

u8 dds3SpPolyGetLoop(dds3SpPolyLoopWork_t *hData);

void dds3PolygonSetColor(dds3Polygon_t *hPolygon, u32 color);

void dds3PolygonTriangleSet(dds3Polygon_t *hPolygon, const f32 *center);

void dds3TrackPolyColorSet(dds3TrackPoly_t *hTrackPoly, s32 number, u32 color);

typedef void (*dds3TrackPolyVCSetFunc_t)(void *, void *, void *);

typedef void *(*dds3SpPolyCreateFunc_t)(void *);

typedef void (*dds3SpPolyWorkFunc_t)(void *);

typedef void (*dds3SpPolyScaleFunc_t)(void *, f32);

extern u32 dds3TrackPolyPairFaceIndex[32];

extern u32 dds3TrackPolyTriangleFaceIndex[28];

extern u32 dds3TrackPolySixVectorFaceIndex[32];

extern u32 dds3TrackPolyDefaultFaceIndex[20];

extern u32 dds3TrackPolyFiveVectorFaceIndex[24];

extern f32 dds3PolygonTriangleParam[4];

extern f32 dds3TrackPolyVCType1Param[4];

extern f32 dds3TrackPolyVCType2Param[4];

extern f32 dds3TrackPolyVCSetAcuteParam[4];

extern f32 dds3TrackPolyVCSetAcuteWParam[4];

extern f32 dds3TrackPolyVCAcuteParam[4];

/* Indexed by track type * 3 + colour mode. Types 0, 1 and 3 are populated;
 * the IDB's dds3TrackPolyVCSet4Table is entry 9. */
extern void (*VCSetFunc[16])(void *, void *, void *);

extern sdfGrOt_t *dds3TrackPolyOtTable[4];

extern f32 dds3SpPolyColorTo[4];

extern f32 dds3SpPolyColorFrom[4];

extern dds3SpPolyEntry_t dds3SpPolyTable[4];

extern void (*dds3SpPolyResetTable[4])(void *work);

extern void (*dds3SpPolySetScaleTable[4])(void *work, f32 scale);

#endif
