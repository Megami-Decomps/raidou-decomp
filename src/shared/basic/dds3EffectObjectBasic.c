#include "common.h"
#include "raidou_alloc.h"
#include "sdk_libc.h"
#include "sdf_resource.h"
#include "basic.h"
#include "dds3obj.h"
#include "ps2asm.h"
#include "field.h"
#include "effBattle.h"
#include "model.h"
#include "billboard.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "billManager.h"
#include "dds3SceneBasic.h"
#include "dds3Basic.h"
#include "dds3EffectObjectBasic.h"
#include "dds3ObjectBase.h"
#include "effManager.h"
#include "effMagatuhi.h"
#include "effEvent.h"
#include "dds3AreaBasic.h"

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
    f32 range;
    f32 height;
    f32 parameter;
    u32 color;
} dds3EffectEventParameters_t;

extern const dds3EffectEventParameters_t dds3EffectEventDefaultParameters;
extern const effMagaSCArea_t dds3EffectMagaSCAreaDefault;
extern const effMagaP4Point_t dds3EffectMagaP4PointDefault;

typedef struct
{
    sdfVector4_t position;
    sdfVector4_t rotation;
} dds3EffectOwnerPose_t;

extern void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);
extern void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);

typedef struct fldResourceEffect_t
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

typedef struct BattleEffect BattleEffect;

typedef void (*FileRequestCallback)(void *request, u32 arg);

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

#define EFF_OBJ_OWNER_KIND_EXTRA EFF_WORLD_KIND_ACTION_10

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
void dds3EffectObjectBasicCalcFldEx(dds3Basic_t *basic);
void dds3EffectDraw(effHandle_t *pData);
dds3Basic_t *AddP2AEffect(void *p2a, void *pos, s32 rot);
extern billHandle_t *dds3BillboardFileLoad(s32 kind, const char *path);
dds3Basic_t *AddTMXEffect(void *tmx, void *pos, s32 rot);
dds3Basic_t *AddBEDEffect();
dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);
dds3Basic_t *AddMG12Effect();
extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);
void FreeEffectData(dds3EffectObjectBasicWorkData_t *work);
s32 dds3EffectObjectBasicConstructor(dds3Basic_t *aPtr);
void dds3EffectObjectBasicDestructor(dds3Basic_t *aPtr);
s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *aPtr);
dds3Basic_t *dds3AddEffectObjectBasic(u32 serialID, const sdfVector4_t *aPos, const sdfVector4_t *aRot);
dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot);
void dds3AddP2AEffectObjectBasic2(u32 effdata, void *pos, s32 rot);
dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, saddr_t secondVectorAddress);
void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 animeID);
dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot);
void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot);
void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot);
void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno);
dds3Basic_t *AddESPEffect(const char *name, void *pos, void *rot, f32 r);
dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);
void *dds3AddMG12EffectObjectBasic(s32 type, void *file);
dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *b);
s32 GetESPPos(dds3Basic_t *b);
s32 GetESPR(dds3Basic_t *b);
s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);

s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                              dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s);
s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);
void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic);
s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);
void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic);
void dds3EffectObjectBasicSetFldEx(dds3Basic_t *basic, fldResourceEffect_t *pResEff);

void FreeEffectData(dds3EffectObjectBasicWorkData_t *work) {
    dds3EffectObjectBasicWorkData_t *data = work;

    switch (data->type) {
    case 1:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            dds3BillboardDelete(data->handle);
            data->handle = NULL;
        }
        break;
    case 4:
        if (data->handle != NULL) {
            data->handle = NULL;
        }
        break;
    case 5:
        if (data->cuth != NULL) {
            effEveBEDCutDelete(data->cuth);
            data->cuth = NULL;
        }
        if (data->handle != NULL) {
            effEveBEDDelete(data->handle);
            data->handle = NULL;
        }
        if (data->para != NULL) {
            sdfChipFreeDirect(data->para);
            data->para = NULL;
        }
        break;
    case 7:
    case 8:
        if (data->handle != NULL) {
            dds3EffectDelete(data->handle);
            data->handle = NULL;
        }
        if (data->cuth != NULL) {
            sdfChipFreeDirect(data->cuth);
            data->cuth = NULL;
        }
        if (data->para != NULL) {
            sdfChipFreeDirect(data->para);
            data->para = NULL;
        }
        break;
    }
    data->type = 0;
    data->handle = NULL;
}

s32 dds3EffectObjectBasicConstructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    dds3InitPoseSet(aPtr);
    aPtr->workdata = sdfChipAllocDirect(0x50);
    memset(aPtr->workdata, 0, 0x50);
    data = aPtr->workdata;
    data->pObjectBaseControlWorkData = dds3ObjectBaseInitControlWorkData(aPtr);
    dds3ObjectBaseOnFlag(aPtr, 0x60);
    return 1;
}

void dds3EffectObjectBasicDestructor(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    data = aPtr->workdata;
    FreeEffectData(data);
    dds3FreePoseSet(aPtr);
    dds3ObjectBaseFreeControlWorkData(data->pObjectBaseControlWorkData);
    sdfChipFreeDirect(aPtr->workdata);
    aPtr->workdata = NULL;
}

s32 dds3EffectObjectBasicMainProcedure(dds3Basic_t *aPtr)
{
    sdfMatrix4_t matrix;
    sdfVector4_t position;
    sdfVector4_t rotation = {0, 0, 0, 1.0f};
    dds3EffectObjectBasicWorkData_t *work;
    s32 moved = 0;
    effHandle_t *effect;
    billHandle_t *bill;
    s32 result;

    work = aPtr->workdata;
    if (work->stat & EFF_OBJ_FLAG_OWNER_LINK) {
        if (dds3SceneBasicExistBasicFromType(dds3GetActiveScene(), work->linkbasictype,
                                            work->linkbasic) != 0) {
            if (work->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
                dds3EffectObjectBasicGetObjectIndex(aPtr);
            } else {
                switch (work->linkbasictype) {
                case BASIC_TYPE_CAMERAOBJECT: case BASIC_TYPE_UNITOBJECT: case BASIC_TYPE_MATTEROBJECT:
                case BASIC_TYPE_EFFECTOBJECT: case BASIC_TYPE_EVENTOBJECT: case BASIC_TYPE_LIGHTOBJECT:
                    SDF_VU0_LOADVECTOR(&work->linkbasic->poseset->rot);
                    SDF_VU0_STOREVECTOR(&rotation);
                    ToMatrix();
                    break;
                case BASIC_TYPE_POINT:
                    SDF_VU0_LOADVECTOR(&((dds3EffectOwnerPose_t *)work->linkbasic->workdata)->rotation);
                    SDF_VU0_STOREVECTOR(&rotation);
                    ToMatrix();
                    break;
                default:
                    SDF_VU0_LOADIDENTITY();
                    break;
                }
            }
            SDF_VU0_COPYMATRIX_A_TO_B();
            SDF_VU0_LOADIDENTITY();
            SDF_VU0_LOADVECTOR(work->linkpos);
            SDF_VU0_VECTOR_W_SET_ONE();
            SDF_VU0_TRANSLATE();
            MultMatrixRevNoArg();
            SDF_VU0_LOADIDENTITYVECTOR();
            SDF_VU0_APPLYMATRIX();
            if ((work->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) == 0) {
                switch (work->linkbasictype) {
                case BASIC_TYPE_CAMERAOBJECT: case BASIC_TYPE_UNITOBJECT: case BASIC_TYPE_MATTEROBJECT:
                case BASIC_TYPE_EFFECTOBJECT: case BASIC_TYPE_EVENTOBJECT: case BASIC_TYPE_LIGHTOBJECT:
                    SDF_VU0_LOADVECTOR_B(&work->linkbasic->poseset->pos);
                    SDF_VU0_ADDVECTOR();
                    break;
                case BASIC_TYPE_POINT:
                    SDF_VU0_LOADVECTOR_B(&((dds3EffectOwnerPose_t *)work->linkbasic->workdata)->position);
                    SDF_VU0_ADDVECTOR();
                    break;
                }
            }
            SDF_VU0_STOREVECTOR(&position);
            dds3SetRadiusPoseSet(aPtr->poseset);
            SDF_VU0_STOREVECTOR(&aPtr->poseset->pos);
            moved = 1;
            dds3OffPoseSetFlag(aPtr->poseset, POSESET_FLAG_UPDATEPOSE);
        } else {
            dds3EffectObjectBasicUnlinkObject(aPtr);
        }
    } else {
        dds3OffPoseSetFlag(aPtr->poseset, POSESET_FLAG_UPDATEPOSE);
        SDF_VU0_LOADVECTOR(&aPtr->poseset->pos);
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(&position);
        dds3SetRadiusPoseSet(aPtr->poseset);
        SDF_VU0_LOADVECTOR(&aPtr->poseset->rot);
        Normalize();
        SDF_VU0_STOREVECTOR(&rotation);
        moved = 1;
    }
    switch (work->type) {
    case EFF_OBJ_STATE_BOUND_BILL:
        effect = work->handle;
        if (effect != NULL && moved) {
            dds3EffectSetPosition(effect, &position);
            SDF_VU0_LOADVECTOR(&rotation);
            ToMatrix();
            SDF_VU0_STOREMATRIX(&matrix);
            dds3EffectSetRotateMatrix(effect, &matrix);
        }
        result = dds3EffectGetState(effect);
    expired:
        if (result == 0) {
            u32 flags = work->stat;

            work->stat = flags & ~1;
            if (flags & 2) {
                dds3DeleteSceneBasicData(aPtr);
            }
        }
        return 1;
    case 2:
        bill = work->handle;
        if (bill != NULL && moved) {
            dds3BillboardSetPosition(bill, &position);
        }
        result = dds3BillboardGetAnimState(bill);
        goto expired;
    case 3:
        if (work->handle != NULL && moved) {
            dds3BillboardSetPosition(work->handle, &position);
        }
        return 1;
    case 4:
        return 1;
    case EFF_OBJ_STATE_EVENT_NODE: {
        void *cut = work->cuth;

        if (cut != NULL) {
            if (moved) {
                dds3EffectEventParameters_t *parameters = work->para;

                if (parameters != NULL) {
                    SDF_VU0_COPYVECTOR(&parameters->position, &position);
                    SDF_VU0_COPYVECTOR(&parameters->rotation, &rotation);
                    effEveBEDCutSetParams(cut, parameters);
                }
            }
        }
        return 1;
    }
    }
    return 1;
}

s32 dds3EffectObjectBasicDrawProcedure(dds3Basic_t *aPtr) {
    dds3EffectObjectBasicWorkData_t *data;

    data = aPtr->workdata;
    dds3EffectObjectBasicCalcFldEx((dds3Basic_t *)aPtr);
    if ((data->stat & 1) == 0) {
        return 1;
    }
    if (dds3ObjectBaseCheckFlag(aPtr, 1)) {
        return 1;
    }

    switch (data->type) {
    case 1:
    case 7:
    case 8:
        if (data->handle != NULL) {
            dds3EffectDraw(data->handle);
        }
        break;
    case 2:
    case 3:
        if (data->handle != NULL) {
            dds3BillboardDraw(data->handle);
        }
        break;
    case 5:
        if (data->cuth != NULL) {
            effEveBEDCutDraw(data->cuth);
        }
        break;
    }
    return 1;
}

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic)
{
    return ((dds3EffectObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}

dds3Basic_t *dds3AddEffectObjectBasic(u32 serialID, const sdfVector4_t *aPos, const sdfVector4_t *aRot) {
    dds3Basic_t *obj;
    dds3EffectObjectBasicWorkData_t *data;

    obj = (dds3Basic_t *)dds3AddSceneBasicData(EFF_WORLD_KIND_EFFECT_OBJECT);
    if (obj == NULL) {
        return NULL;
    }
    obj->serialID = serialID;
    dds3ObjectBaseCreateMover(obj);
    dds3SetPosPoseSet(obj, aPos);
    dds3SetRotPoseSet(obj, aRot);
    dds3SetRadiusPoseSet(obj->poseset);
    data = obj->workdata;
    data->stat = 0;
    data->type = 0;
    data->handle = NULL;
    data->linkbasic = NULL;
    data->linkindex = 0;
    data->linkbasictype = 0;
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddP2AEffect);

dds3Basic_t *dds3AddP2AEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((dds3EffectObjectBasicWorkData_t *)ebasic->workdata)->handle);
    return AddP2AEffect(bill, pos, rot);
}

void dds3AddP2AEffectObjectBasic2(u32 effdata, void *pos, s32 rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(1, effdata);
    AddP2AEffect(bill, pos, rot);
}

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, saddr_t secondVectorAddress) {
    void *bill;

    bill = dds3BillboardFileLoad(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}

void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 animeID)
{
    dds3BillboardSetAnimNumber(((dds3EffectObjectBasicWorkData_t *)basic->workdata)->handle, animeID);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddTMXEffect);

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((dds3EffectObjectBasicWorkData_t *)ebasic->workdata)->handle);
    return AddTMXEffect(bill, pos, rot);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3AddTMXEffectObjectBasic2);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", effObjCreateIndexedKindZero);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddD3PEffect);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3EffectObjectBasicAddD3P2);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", dds3CallDevLoadTexList);

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0011C888.c:1, match */
dds3Basic_t *AddEPLEffect(effHandle_t *eff, const sdfVector4_t *pos, const sdfVector4_t *rot)
{
    sdfVector4_t vector;
    dds3Basic_t *basic;
    dds3EffectObjectBasicWorkData_t *work;
    dds3ObjectBaseControlWorkData_t *control;
    dds3Basic_t *area;

    memset(&vector, 0, sizeof(vector));
    basic = dds3AddEffectObjectBasic(dds3AllocObjectId(), pos, rot);
    if (basic == NULL) {
        return NULL;
    }
    SDF_VU0_LOADVECTOR(pos);
    SDF_VU0_STOREVECTOR(&vector);
    dds3EffectSetPosition(eff, &vector);
    work = basic->workdata;
    work->type = EFF_OBJ_STATE_BOUND_BILL;
    work->handle = eff;
    work->stat = 0;
    work->linkbasic = NULL;
    work->linkindex = 0;
    work->linkbasictype = 0;
    control = dds3EffectObjectBasicGetControlWorkData(basic);
    control->modelType = 2;
    area = dds3AreaBasicGetFastArea();
    if (area != NULL) {
        control->pBasic[4] = area;
        dds3AreaBasicRegistItem(area, basic);
    }
    return basic;
}

dds3Basic_t *dds3EffectObjectBasicAddEPL(void *effdata, const sdfVector4_t *pos, const sdfVector4_t *rot) {
    effHandle_t *effect;

    effect = dds3EffectLoadEPL(effdata);
    return AddEPLEffect(effect, pos, rot);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddBEDEffect);

void *dds3AddBEDEffectObjectBasic2(void *data, void *pos, void *rot)
{
    return AddBEDEffect(data, pos, rot);
}

void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddBEDEffect(EE_ADDR_AS(effBTL_t *, resolvedId), pos, rot);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno) {
    dds3EffectObjectBasicWorkData_t *data;

    data = basic->workdata;
    if (data->type != EFF_OBJ_STATE_EVENT_NODE) {
        return;
    }
    if (data->para == NULL) {
        return;
    }
    if (data->handle == NULL) {
        return;
    }
    if (data->cuth != NULL) {
        effEveBEDCutDelete(data->cuth);
        data->cuth = NULL;
    }
    data->cuth = effEveBEDCutCreate(data->handle, (u16)(cutno & EFF_OBJ_ENTRY_ID_MASK), data->para);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddESPEffect);

dds3Basic_t *dds3AddESPEffectObjectBasic(const char *name, void *pos, void *rot, f32 r)
{
    return AddESPEffect(name, pos, rot, r);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", AddMG12Effect);

dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata)
{
    return AddMG12Effect(type, effdata);
}

void *dds3AddMG12EffectObjectBasic(s32 type, void *file) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddMG12Effect(type, resolvedId);
    sdfMemFreeDirect(resourceHandle);
    return created;
}

dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *b) {
    dds3EffectObjectBasicWorkData_t *data;

    data = NULL;
    if (b == NULL) {
        return data;
    }
    if (((u8 *)&b->type)[3] != EFF_OBJ_KIND) {
        return data;
    }
    data = b->workdata;
    if (data->type != EFF_OBJ_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}

s32 GetESPPos(dds3Basic_t *b)
{
    if (CheckESP(b) == NULL) {
        return 0;
    }
    SDF_VU0_LOADVECTOR(&b->poseset->pos);
    return 1;
}

s32 GetESPR(dds3Basic_t *b) {
    dds3PoseSet_t *parameters;

    if (CheckESP(b) == NULL) {
        return 0;
    }
    parameters = b->poseset;
    return (s32)parameters->scl.x;
}

s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff) {
    if (((u8 *)&eff->type)[3] == EFF_OBJ_KIND) {
        dds3EffectObjectBasicWorkData_t *data = eff->workdata;

        switch (data->type) {
        case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
            break;
        case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
            effMagaP4Randomize(data->handle);
            break;
        case EFF_OBJ_STATE_BOUND_BILL:
            effMagaRandomize(data->handle);
            break;
        }
    }
}

/* Missing parameter sources leave their existing position and radius intact. */
s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                              dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4)
{
    dds3EffectObjectBasicWorkData_t *work;
    effMagaSCArea_t *area;
    effMagaP4Point_t *points;

    if (eff->type >> 24 != EFF_OBJ_KIND) {
        return 0;
    }
    work = eff->workdata;
    switch (work->type) {
    case EFF_OBJ_STATE_MAGATUHI_TWO_ROWS:
        area = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(&area->position[0]);
            area->radius[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(&area->position[1]);
            area->radius[1] = GetESPR(p2);
        }
        effMagaSCSetArea(work->handle, area);
        return 1;
    case EFF_OBJ_STATE_MAGATUHI_FOUR_ROWS:
        points = work->para;
        if (GetESPPos(p1)) {
            SDF_VU0_STOREVECTOR(points->controlPoints[0]);
            points->jitterScales[0] = GetESPR(p1);
        }
        if (GetESPPos(p2)) {
            SDF_VU0_STOREVECTOR(points->controlPoints[1]);
            points->jitterScales[1] = GetESPR(p2);
        }
        if (GetESPPos(p3)) {
            SDF_VU0_STOREVECTOR(points->controlPoints[2]);
            points->jitterScales[2] = GetESPR(p3);
        }
        if (GetESPPos(p4)) {
            SDF_VU0_STOREVECTOR(points->controlPoints[3]);
            points->jitterScales[3] = GetESPR(p4);
        }
        effMagaP4SetPoint(work->handle, points);
        return 1;
    }
    return 0;
}

void dds3EffectObjectBasicStatOn(dds3Basic_t *basic, s32 s) {
    ((dds3EffectObjectBasicWorkData_t *)basic->workdata)->stat |= s;
}

void dds3EffectObjectBasicStatOff(dds3Basic_t *basic, s32 s) {
    ((dds3EffectObjectBasicWorkData_t *)basic->workdata)->stat &= ~s;
}

s32 dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target) {
    dds3EffectObjectBasicWorkData_t *data;
    u8 kind;

    kind = ((u8 *)&target->type)[3];
    if (kind < EFF_OBJ_OWNER_KIND_FIRST || (kind >= EFF_OBJ_OWNER_KIND_END && kind != EFF_OBJ_OWNER_KIND_EXTRA)) {
        return 0;
    }
    data = basic->workdata;
    data->linkpos[2] = 0;
    data->linkbasic = target;
    data->linkindex = 0;
    data->stat |= EFF_OBJ_FLAG_OWNER_LINK;
    data->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    data->linkpos[0] = 0;
    data->linkbasictype = ((u8 *)&target->type)[3];
    data->linkpos[1] = 0;
    return 1;
}

void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic) {
    dds3EffectObjectBasicWorkData_t *data;
    dds3Basic_t *owner;
    effOwnerBillPayload_t *bill;

    data = basic->workdata;
    if (data->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->linkbasic;
        if (((u8 *)&owner->type)[3] == EFF_OBJ_OWNER_BILL_KIND) {
            bill = (effOwnerBillPayload_t *)((dds3EffectObjectBasicWorkData_t *)owner->workdata)->handle;
            if (bill->state != 0) {
                return;
            }
            sdfModelGetMarkPosMatrix(((effOwnerBillParam_t *)bill->params)->model, data->linkindex);
        }
    }
}

s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index) {
    dds3EffectObjectBasicWorkData_t *data;

    if (dds3EffectObjectBasicLinkObject(basic, target) == 0) {
        return 1;
    }
    data = basic->workdata;
    data->linkindex = index;
    data->stat |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    return 1;
}

void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic)
{
    dds3EffectObjectBasicWorkData_t *work = basic->workdata;

    work->linkindex = 0;
    work->linkbasic = NULL;
    work->linkbasictype = 0;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_LINK;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
}

void dds3EffectObjectBasicSetFldEx(dds3Basic_t *basic, fldResourceEffect_t *pResEff) {
    dds3EffectObjectBasicWorkData_t *resource = basic->workdata;
    dds3PoseSet_t *source = basic->poseset;

    resource->pFldEx = pResEff;
    resource->FldExBasePos.x = source->pos.x;
    resource->FldExBasePos.y = source->pos.y;
    resource->FldExBasePos.z = source->pos.z;
    resource->FldExBasePos.w = source->pos.w;
    dds3EffectObjectBasicStatOn((struct dds3Basic_t *)basic, 0x10);
    switch (resource->pFldEx->type) {
    case 0:
    case 3:
        dds3BillboardSetXYOffset((struct billHandle_t *)resource->handle, resource->pFldEx->size[0], resource->pFldEx->size[1]);
        if (resource->pFldEx->type == 3) {
            dds3BillboardSetDrawType(resource->handle, 1);
        }
        if (resource->pFldEx->flag & 1) {
            dds3BillboardSetMode(resource->handle, 2);
            return;
        }
        if (resource->pFldEx->flag & 2) {
            dds3BillboardSetMode(resource->handle, 3);
            return;
        }
        break;
    case 2:
        break;
    case 1:
        dds3BillboardSetAnimNumber(resource->handle, resource->pFldEx->param0);
        break;
    }
}

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", dds3EffectEventDefaultParameters);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", dds3EffectMagaSCAreaDefault);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", dds3EffectMagaP4PointDefault);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", sdfStrSys);

