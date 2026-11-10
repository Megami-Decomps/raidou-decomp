/* Declarations of smt3-decomp src/smt3/basic/dds3EffectObjectBasic.c, for its bodies in this directory. */
#ifndef SHARED_BASIC_DDS3EFFECTOBJECTBASIC_H
#define SHARED_BASIC_DDS3EFFECTOBJECTBASIC_H

#include "sdk_libc.h"

#include "common.h"

#include "sdf_resource.h"

#include "basic.h"

#include "dds3obj.h"

#include "ps2asm.h"

#include "field.h"

#include "effBattle.h"

#include "model.h"

#include "billboard.h"

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
    f32 range;
    f32 height;
    f32 parameter;
    u32 color;
} dds3EffectEventParameters_t;

typedef struct
{
    sdfVector4_t position[2];
    f32 radius[2];
} effMagaSCArea_t;

typedef struct
{
    sdfVector4Unaligned_t position[4];
    f32 radius[4];
} effMagaP4Point_t;

extern const dds3EffectEventParameters_t dds3EffectEventDefaultParameters;

extern const effMagaSCArea_t dds3EffectMagaSCAreaDefault;

extern const effMagaP4Point_t dds3EffectMagaP4PointDefault;

extern void *memcpy(void *a, const void *b, size_t c);

extern void dds3BillboardSetPosition(void *hBillboard, const void *pPos);

extern void dds3BillboardSetAnimNumber(void *hBillboard, u32 number);

extern void dds3EffectSetPosition(void *pData, const void *pPos);

extern struct effHandle_t *dds3EffectLoad(void *pD3P);

extern struct effHandle_t *dds3EffectFileLoad(u32 pFilename);

extern  effBTL_t *effEveBEDCreate( effBTL_t *pData);

extern void effMagaSCSetArea(void *hEffect, const void *pMagaSCArea);

extern void effMagaP4SetPoint(void *hEffect, const void *pP4Point);

extern sdfVector4_t aViewEye;

extern s32 fldGetModelFlag2(s32 map, u32 floor, s32 group);

extern s32 fldGetAnimeId2(s32 map, u32 floor, s32 group);

extern void dds3BillboardSetRGBA(void *hBillboard, u32 abgr);

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
} dds3EffectOwnerPose_t;

extern dds3Basic_t *dds3GetActiveScene(void);

extern s32 dds3SceneBasicExistBasicFromType(dds3Basic_t *scene, s32 type, dds3Basic_t *target);

extern void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);

extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void ToMatrix(void);

extern void MultMatrixRevNoArg(void);

extern void Normalize(void);

extern void dds3EffectSetRotateMatrix(void *pData, const sdfMatrix4_t *pMatrix);

extern s32 dds3EffectGetState(void *hEffect);

extern u16 dds3BillboardGetAnimState(void *hBillboard);

extern void effEveBEDCutSetParams(void *hEveBEDCut, const void *pParams);

typedef struct
{
    u32 flag;
    u32 type;
    u32 noEffect;
    f32 size[3];
    u32 offsetZ;
    s32 groupID;
    u32 param0;
    u32 param1;
} fldResourceEffect_t;

typedef struct
{
    dds3ObjectBaseControlWorkData_t *pObjectBaseControlWorkData;
    u32 stat;
    s32 type;
    void *handle;
    u32 linkpos[4]; /* sdfVector4_t in the IDB, written as words here */
    dds3Basic_t *linkbasic;
    u16 linkindex;
    u16 linkbasictype;
    void *para;
    void *cuth;
    s32 reserve1;
    s32 reserve2;
    s32 reserve3;
    fldResourceEffect_t *pFldEx;
    sdfVector4_t FldExBasePos;
} dds3EffectObjectBasicWorkData_t;

struct billHandle_t *dds3BillboardCopy(struct billHandle_t *hBillboard);

struct billHandle_t *dds3BillboardCreate(s32 kind, addr_t id);

void effMagaP4Randomize(void *hEffect);

void *memset(void *a, s32 b, size_t c);

struct dds3BillResource_tag;

typedef struct dds3BillResource_tag dds3BillResource_tag;

struct dds3BillAnimResource_tag;

typedef struct dds3BillAnimResource_tag dds3BillAnimResource_tag;

struct dds3BillAnim_tag;

typedef struct dds3BillAnim_tag dds3BillAnim_tag;

struct EvtUnit;

typedef struct EvtUnit EvtUnit;

struct sdfItemArray_t;

typedef struct sdfItemArray_t sdfItemArray_t;

struct sdfModelExtraChunk_t;

typedef struct sdfModelExtraChunk_t sdfModelExtraChunk_t;

struct SdfDmaTagHeader;

typedef struct SdfDmaTagHeader SdfDmaTagHeader;

struct sdfModelNode_t;

typedef struct sdfModelNode_t sdfModelNode_t;

struct effPartHandle_t;

typedef struct effPartHandle_t effPartHandle_t;

typedef struct effEveBEDCutHandle_t effEveBEDCutHandle_t;

typedef struct BattleEffect BattleEffect;

typedef void (*FileRequestCallback)(void *request, u32 arg);

typedef struct effHandle_t {
    u32 type;
    s32 arg;
    s32 instance;
    f32 unkC;
} effHandle_t;

enum {
    EFF_WORLD_KIND_WORLD_INFO = 0,
    EFF_WORLD_KIND_EVENT_TABLE = 1,
    EFF_WORLD_KIND_WORLD_INDEX_OWNER = 2,
    EFF_WORLD_KIND_SLOT_RING = 3,
    EFF_WORLD_KIND_CAMERA = 4,
    EFF_WORLD_KIND_FOLLOW_MODEL = 5,
    EFF_WORLD_KIND_EFFECT_TRANSFORM = 6,
    EFF_WORLD_KIND_EFFECT_OBJECT = 7,
    EFF_WORLD_KIND_RESOURCE_OWNER = 8,
    EFF_WORLD_KIND_LIGHT = 9,
    EFF_WORLD_KIND_SCRIPT_OBJECT = 10,
    EFF_WORLD_KIND_ACTION_B = 11,
    EFF_WORLD_KIND_ACTION_D = 13,
    EFF_WORLD_KIND_ACTION_10 = 16,
    EFF_WORLD_KIND_TRANSFORM_SOURCE = 17,
    EFF_WORLD_KIND_COUNT = 18
};

#define EFF_OBJ_STATE_EVENT_NODE 5

#define EFF_OBJ_ENTRY_ID_MASK 0xFFFF

#define EFF_OBJ_STATE_PARAMETERS_READY 6

#define EFF_OBJ_KIND EFF_WORLD_KIND_EFFECT_OBJECT

#define EFF_OBJ_STATE_MAGATUHI_TWO_ROWS 7

#define EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS 8

#define EFF_OBJ_STATE_BOUND_BILL 1

#define EFF_OBJ_OWNER_KIND_FIRST 4

#define EFF_OBJ_OWNER_KIND_END 10

#define EFF_OBJ_OWNER_KIND_EXTRA 0x11

#define EFF_OBJ_FLAG_OWNER_LINK 4

#define EFF_OBJ_FLAG_OWNER_BILL_ENTRY 8

typedef struct effOwnerBillPayload_t {
    u8 pad00[8];
    u32 state;
    void *params;
} effOwnerBillPayload_t;

#define EFF_OBJ_OWNER_BILL_KIND 5

typedef struct effOwnerBillParam_t {
    u8 pad00[0x18];
     sdfModelRootNode_t *model;
} effOwnerBillParam_t;

void dds3EffectDelete(effHandle_t *pData);

void dds3BillboardDelete(struct billHandle_t *hData);

void effEveBEDCutDelete(effEveBEDCutHandle_t *hEveBEDCut);

void effEveBEDDelete( effBTL_t *hEveBED);

void sdfChipFreeDirect(void *aPtr);

s32 dds3InitPoseSet(dds3Basic_t *basic);

void *sdfChipAllocDirect(u32 aSize);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

extern u8 opaque40[0x60];

void dds3FreePoseSet(dds3Basic_t *basic);

void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *basic);

u8 dds3ObjectBaseCheckFlag(struct dds3Basic_t *pObjectBasic, u32 flag);

void dds3EffectDraw(effHandle_t *pData);

extern void dds3BillboardDraw(struct billHandle_t *hData);

void effEveBEDCutDraw(effEveBEDCutHandle_t *hEveBEDCut);

dds3Basic_t *dds3AddSceneBasicData(s32 type);

void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

extern void dds3SetRadiusPoseSet(dds3PoseSet_t *node);

dds3Basic_t *AddP2AEffect(void *p2a, void *pos, s32 rot);

extern billHandle_t *dds3BillboardFileLoad(s32 kind, const char *path);

dds3Basic_t *AddTMXEffect(void *tmx, void *pos, s32 rot);

dds3Basic_t *AddBEDEffect();

void sdfMemFreeDirect( sdfMemHandle_t *aHandle);

effEveBEDCutHandle_t *effEveBEDCutCreate( effBTL_t *hEveBED, u16 CutNum, const void *pParams);

u32 dds3AllocObjectId(void);

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

extern dds3Basic_t *dds3AreaBasicGetFastArea(void);

void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *AddMG12Effect();

extern void effMagaRandomize(void *hEffect);

extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);

void dds3BillboardSetXYOffset(struct billHandle_t *hBillboard, f32 x, f32 y);

void dds3BillboardSetDrawType(struct billHandle_t *hBillboard, s32 type);

void dds3BillboardSetMode(struct billHandle_t *hBillboard, s32 mode);

void FreeEffectData(dds3EffectObjectBasicWorkData_t *work);

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *aPtr);

void dds3EffectObjectBasicDestructor(dds3Basic_t *aPtr);

/* Resolve the owner transform, then forward it to the live effect backend. */
s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *aPtr);

s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *aPtr);

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

dds3Basic_t *dds3AddEffectObjectBasic(u32 serialID, void *aPos, void *aRot);

dds3Basic_t *AddP2AEffect(void *p2a, void *pos, s32 rot);

dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot);

void dds3AddP2AEffectObjectBasic2(u32 effdata, void *pos, s32 rot);

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, saddr_t secondVectorAddress);

void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 animeID);

dds3Basic_t *AddTMXEffect(void *tmx, void *pos, s32 rot);

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot);

void dds3AddTMXEffectObjectBasic2(u32 effdata, void *pos, s32 rot);

dds3Basic_t *dds3AddTMXEffectObjectBasic(const char *path, void *firstVector, saddr_t secondVectorAddress);

dds3Basic_t *AddD3PEffect(void *eff, void *pos, void *rot);

dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, void *pos, void *rot);

dds3Basic_t *dds3AddD3PEffectObjectBasic(u32 file, void *pos, void *rot);

dds3Basic_t *AddBEDEffect();

void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);

void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot);

void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno);

dds3Basic_t *AddESPEffect(const char *name, void *pos, void *rot, f32 r);

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, void *pos, void *rot, f32 r);

/* Keep the legacy positional (kind, descriptor) ABI used by the file loader. */
dds3Basic_t *AddMG12Effect();

dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);

void *dds3AddMG12EffectObjectBasic(s32 type, void *file);

dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *b);

s32 GetESPPos(dds3Basic_t *b);

s32 GetESPR(dds3Basic_t *b);

s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);

/* Missing parameter sources leave their existing position and radius intact. */
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                              dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s);

void dds3EffectObjectBasicStatOff(dds3Basic_t *basic, s32 s);

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);

s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);

void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);

void dds3EffectObjectBasicSetFldEx(dds3Basic_t *basic, fldResourceEffect_t *pResEff);

/* Apply field visibility/animation selectors and the view-distance fade. */
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *basic);

#endif
