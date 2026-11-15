#include "common.h"
#include "raidou_alloc.h"
#include "sdfFrame.h"
#include "basic.h"
#include "model.h"
#include "motion.h"
#include "event.h"
#include "field.h"
#include "ps2asm.h"
#include "dds3EffectObjectBasic.h"
#include "dds3LightObjectBasic.h"
#include "dds3CameraObjectBasic.h"

extern dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

typedef struct
{
    char name[20];
    char parts[17][20];
} dds3UnitPartTable_t;

#include "sdk_libc.h"

typedef struct
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 hojiID;
} dds3ObjectBaseMatterWork_t;

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);
u32 BasicTypeToItemType(u32 type);
dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);
void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);
void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);
void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);
void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);
dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);
dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);
void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);
void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);
u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);
dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic);
void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);
void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl);
void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic);
void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic);
dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseInitControlWorkData);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseFreeControlWorkData);

void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->flag |= mask;
}

void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    control->flag &= ~flag;
}

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    return (control->flag & flag) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseSetUserValue);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseGetUserValue);

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic)
{
    if (pItemBasic == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(pObjectBasic, pItemBasic,
                                             BasicTypeToItemType(pItemBasic->type >> 24));
}

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType)
{
    dds3Basic_t *previous;

    previous = dds3ObjectBaseGetControlBasic(pObjectBasic, itemType);
    dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType] = pItemBasic;
    return previous;
}

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType];
}

dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->hEventBasicArray;
}

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pModel;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseFreeModelHandle);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMakeUnit);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMakeMatter);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseMatterAnimeSpeed);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseRegistArea);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3CallMemset);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseModelLinkOt);

void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic)
{
    if (dds3ObjectBaseGetControlBasic(pObjectBasic, 1) == NULL) {
        dds3ObjectBaseSetControlBasic(pObjectBasic, dds3AddMoverBasic(pObjectBasic));
    }
}

void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl)
{
    dds3ObjectBaseSetPrimaryControl(pObjectBasic, pFuncPadControl);
    /* Retail dds3ObjectBaseSetPadControl and DDS1 001127A8 keep JAL plus epilogue.
     * DDS cc1probe: this consumed-callback reset is removed in 03.cse,
     * after the sibling-call decision (01.sibling / 02.jump). */
    pFuncPadControl = NULL;
}

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3Basic_t *mover;

    mover = dds3ObjectBaseGetControlBasic(basic, 1);
    if (mover == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(mover, func);
    return 1;
}

void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicSetPath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1), pPathBasic);
    dds3MoverBasicMakePath(pObjectBasic);
}

void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic)
{
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic)
{
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}

dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = NULL;
    switch ((pObjectBasic->type >> 24) - BASIC_TYPE_CAMERAOBJECT) {
    case 0: control = dds3GetCameraObjectBaseControlWorkData(pObjectBasic); break;
    case 1: control = dds3UnitObjectBasicGetControlWorkData(pObjectBasic); break;
    case 2: control = dds3MatterObjectBasicGetControlWorkData(pObjectBasic); break;
    case 3: control = dds3EffectObjectBasicGetControlWorkData(pObjectBasic); break;
    case 4: control = dds3EventObjectBasicGetControlWorkData(pObjectBasic); break;
    case 5: control = dds3LightObjectBasicGetControlWorkData(pObjectBasic); break;
    }
    return control;
}
