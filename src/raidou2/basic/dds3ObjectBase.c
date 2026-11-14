#include "common.h"
#include "raidou_alloc.h"

/* Declarations from smt3-decomp src/smt3/basic/dds3ObjectBase.c */
#include "common.h"

#include "sdfFrame.h"

#include "basic.h"

#include "model.h"

#include "motion.h"

#include "event.h"

#include "field.h"

#include "ps2asm.h"

extern dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic);

extern dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

extern dds3ObjectBaseControlWorkData_t *dds3EventObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

extern dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic);

extern void dds3AreaBasicGetItemArray(dds3Basic_t *pAreaBasic, dds3Basic_t *itemType);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern void sdfModelRootFree(sdfModelRootNode_t *aRoot, s32 aReleaseNodes, s32 aReleaseMatList);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern void sdfChipFreeDirect(void *aPtr);

extern sdfModelRootNode_t *dds3ModelBasicMakeModel(dds3Basic_t *basic);

extern sdfMotionCtrlr_t *dds3ModelBasicSetMotion(dds3Basic_t *basic, sdfModelRootNode_t *model);

extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

extern void sdfModelEvalTm2(sdfModelRootNode_t *aRoot, s32 aFrameId);

extern void *sdfChipAllocDirect(u32 aSize);

extern dds3BasicArrayHandle_t *dds3GetBasicArrayHandle(s32 bufcnt);

extern s32 evtBitCheck(s32 no);

extern void rsetOtOfs(s32 otofs);

extern void rsetAlpha(s32 mode);

extern void rsetFrameMaskRgbOn(void);

extern void rsetFrameMaskRgbOff(void);

extern void rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte,
                     s32 ztst);

extern void aDrawBoxFZ(s32 x, s32 y, s32 w, s32 h, u32 z, u32 r, u32 g,
                       u32 b, u32 a);

extern void mdlLink(void *aHandle, sdfModelOt_t *aModelOt);

extern sdfModelOt_t sdfModelOtTable[];

extern s32 fldGetAnimeFlag(s32 area, s32 floor, s32 bit);

extern void sdfMotionSeek(sdfMotionCtrlr_t *aCtrlr, f32 aFrame);

extern void sdfMotionNext(sdfMotionCtrlr_t *aCtrlr);

extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

extern void sdfModelLinkOt(sdfModelOt_t *aOt, sdfModelRootNode_t *aRoot);

typedef struct
{
    char name[20];
    char parts[17][20];
} dds3UnitPartTable_t;

extern const dds3UnitPartTable_t dds3HijriParts;

extern const dds3UnitPartTable_t dds3HijBParts;

extern const dds3UnitPartTable_t dds3PlayerAParts;

extern const dds3UnitPartTable_t dds3PlayerBParts;

extern const dds3UnitPartTable_t dds3TiakiParts;

extern const dds3UnitPartTable_t dds3YuukoParts;

extern const dds3UnitPartTable_t dds3IsamuParts;

extern const dds3UnitPartTable_t dds3IsamuFParts;

extern const dds3UnitPartTable_t dds3IsamuFzParts;

extern const dds3UnitPartTable_t dds3HikawaParts;

extern const dds3UnitPartTable_t dds3Model113Parts;

extern const dds3UnitPartTable_t dds3TiakiBParts;

extern const dds3UnitPartTable_t dds3MajyoParts;

extern const dds3UnitPartTable_t dds3HutomimiParts;

extern char dds3ObjectBaseHijBNodeName[];

extern char dds3ObjectBaseIsamFNodeName[];

extern char dds3ObjectBaseAradiaNodeName[];

extern char dds3ObjectBaseYuukoPNodeName[];

extern char dds3ObjectBaseChiakiNodeName[];

extern char dds3ObjectBaseTiakiANodeName[];

extern mdlObj_t *mdlLoad(s32 aMajor, s32 aMinor);

extern void mdlGetScale(mdlObj_t *aHandle);

extern void mdlSetRotate(mdlObj_t *aHandle);

extern void mdlSetPosition(mdlObj_t *aHandle);

extern void dds3SetSclPoseSet(dds3Basic_t *pBasic, const sdfVector4_t *scl);

extern void Normalize(void);

extern struct evtUnit_t *evtCreateUnitHandle(dds3Basic_t *owner, mdlObj_t *model);

extern s32 sdfModelGetNodeIndexByName(sdfModelRootNode_t *aRoot, const char *aName);

extern char *sdfModelGetNodeName(sdfModelRootNode_t *aRoot, s32 aIndex);

extern s32 strcmp(const char *a, const char *b);

#include "sdk_libc.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

typedef struct
{
    dds3ObjectBaseControlWorkData_t *control;
    s32 hojiID;
} dds3ObjectBaseMatterWork_t;

void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic);

void *dds3ObjectBaseGetUserValue(dds3Basic_t *pObjectBasic);

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);

extern void mdlFree(void *aHandle);

extern void evtDeleteUnitHandle(void *h);

extern void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr);

u32 BasicTypeToItemType(u32 type);

void dds3AreaBasicRegistItem(dds3Basic_t *pAreaBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic);

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func);

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic);

void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic);

void dds3MoverBasicFreePath(dds3Basic_t *pMoverBasic);

dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic);

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

dds3ObjectBaseControlWorkData_t *dds3ObjectBaseInitControlWorkData(dds3Basic_t *pObjectBasic);

void dds3ObjectBaseFreeControlWorkData(dds3ObjectBaseControlWorkData_t *pControl);

void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag);

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);

void dds3ObjectBaseSetUserValue(dds3Basic_t *pObjectBasic, void *pUserValue);

void *dds3ObjectBaseGetUserValue(dds3Basic_t *pObjectBasic);

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic);

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType);

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType);

dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic);

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

/* modelType selects the model-context or raw model/motion ownership path. */
void dds3ObjectBaseFreeModelHandle(dds3Basic_t *pObjectBasic);

/* The loaded model is identified by its root node name, and the matching table
 * supplies the 17 part names whose node indices the control keeps. */
extern const dds3UnitPartTable_t dds3HijriParts;

extern const dds3UnitPartTable_t dds3HijBParts;

extern const dds3UnitPartTable_t dds3PlayerAParts;

extern const dds3UnitPartTable_t dds3PlayerBParts;

extern const dds3UnitPartTable_t dds3TiakiParts;

extern const dds3UnitPartTable_t dds3YuukoParts;

extern const dds3UnitPartTable_t dds3IsamuParts;

extern const dds3UnitPartTable_t dds3IsamuFParts;

extern const dds3UnitPartTable_t dds3IsamuFzParts;

extern const dds3UnitPartTable_t dds3HikawaParts;

extern const dds3UnitPartTable_t dds3Model113Parts;

extern const dds3UnitPartTable_t dds3TiakiBParts;

extern const dds3UnitPartTable_t dds3MajyoParts;

extern const dds3UnitPartTable_t dds3HutomimiParts;

void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32 major, s32 minor);

void dds3ObjectBaseMakeMatter(dds3Basic_t *pObjectBasic);

/* Modes 0, 4 and 5 play at unit speed; the remaining modes start stopped. */
void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *pObjectBasic, u32 mode);

s32 dds3ObjectBaseRegistArea(dds3Basic_t *pObjectBasic);

/* The save-screen variant: the model is drawn between two full-screen boxes, the
 * second only while event bit 0xAF0 is clear. */
void dds3ObjectBaseModelLinkOtSvs(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic);

/* The matter path ramps animeSpeed up or down depending on whether the block's
 * anime flag is set, then seeks to the last frame in mode 6. */
void dds3ObjectBaseModelLinkOt(sdfModelOt_t *pModelOt, dds3Basic_t *pObjectBasic);

void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic);

void dds3ObjectBaseSetPadControl(dds3Basic_t *pObjectBasic, dds3BasicControlProc_t pFuncPadControl);

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func);

void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic);

void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic);

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic);

dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);

void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *pObjectBasic, u32 svs);

extern char dds3ObjectBaseHijBNodeName[6];

extern char dds3ObjectBaseIsamFNodeName[7];

extern char dds3ObjectBaseAradiaNodeName[7];

extern char dds3ObjectBaseYuukoPNodeName[8];

extern char dds3ObjectBaseChiakiNodeName[7];

extern char dds3ObjectBaseTiakiANodeName[8];

extern u16 frMessageCode;
/* End of declarations from src/smt3/basic/dds3ObjectBase.c */

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseInitControlWorkData_dds3ObjectBase);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", dds3ObjectBaseFreeControlWorkData_dds3ObjectBase);

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

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_0011C048);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_0011C150);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_0011C1F8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_r1_001197E0);

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
