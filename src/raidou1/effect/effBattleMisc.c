#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effBattleMisc.c */
#include "common.h"

#include "effBattle.h"

#include "nb.h"

#include "event.h"

#include "model.h"

#include "draw3d.h"

#include "datCalc.h"

#include "ps2asm.h"

extern void nbSetFormPos(nbFormation_t *form, sdfVector4_t *pos);

extern void nbSetFormRot(nbFormation_t *form, sdfVector4_t *rot);

extern u32 effBTLScaleFrameTable[];

extern void (*GetPositionPtrTable[])(void *owner, effBTLAimParams_t *request);

extern u32 effBTLSkillGetInvokeParty(void);

extern u32 effBTLSkillGetTargetParty(void);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern u32 effBTLSkillGetTargetNumber(void);

extern u32 effBTLSkillGetTargetBit(void);

void effBTLModelInvokePartyShow(void);

extern void effBTLPartGetPartyPositionPtr(u32 group, effBTLAimParams_t *pPosPtr);

extern void effBTLPartPtrModelGroupGetCenterAndHeight(u32 group, sdfVector4_t *center, f32 *heightAverage, f32 *heightMax, f32 *frontMax);

extern f32 aViewTarget[4];

extern f32 aViewEye[4];

extern f32 aViewDown[4];

extern f32 effBTL_Ptr_D[];

extern f32 effBTLAimVerticalScale[];

extern void MultMatrixNoArg(void);

extern u8 effBTLModelShowFlag[];

extern f32 effBTLModelDirFwd[];

extern f32 effBTLModelDirBack[];

extern f32 effBTLModelLocalZ[];

extern f32 effBTLCircumferenceAxisZ[];

extern f32 effBTLCircumferenceAngleTable[];

extern f32 effBTLZeroVector[];

extern f32 effBTLGroupDirFwd[];

extern f32 effBTLGroupDirBack[];

extern sdfVector4_t effBTLPartCenterOffset[32];

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern s32 effBTLSkillCheckRun(void);

extern s32 effBTLSkillGetID(void);

extern void ToMatrix(void);

extern void InversMatrix(void);

extern f32 effBTLModelGroupGetMaxLength(sdfVector4_t *group, u32 model);

extern void nbLoadUnitPos(nbFormation_t *form);

extern void effBTLModelLoadRotate(nbFormation_t *form);

extern void mdlGetPosition(mdlObj_t *aHandle);

extern void sdfModelEvalTm(sdfModelRootNode_t *aRoot);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *aRoot, s32 aId);

extern s32 sdfModelGetMarkPosMatrix(sdfModelRootNode_t *aRoot, s32 aId);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern void effDebugPointDraw(sdfVector4_t *pPos);

extern void *memset(void *a, s32 b, size_t c);

extern void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);

extern void LookAtMatrix(const void *aTarget, const void *aEye, const void *aDown);

extern void ReverseMatrix(void);

extern void MultMatrixRev(sdfMatrix4_t *aMatrix);

extern sdf3DScreenParam_t dds3ScreenParam;

extern s32 sdfModelCheckBounds(f32 *aMinMax);

extern void SetRotateMatrixX(f32 aAngle);

extern void SetRotateMatrixY_B(f32 aAngle);

extern void MultMatrixRevNoArg(void);

extern f32 effBTLModelGroupGetSphere(u32 group);

f32 effBTLModelGroupGetSphere2(u32 group);

f32 effBTLModelGroupGetSphereExp(u32 group, u32 model);

extern f32 effLinePointLength(sdfVector4_t *pDir, sdfVector4_t *pLinePos, sdfVector4_t *pPos);

extern void SetRotateMatrixY(f32 aAngle);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern f32 fabsf(f32 value);

extern void FromEuler(f32 aRotateX, f32 aRotateY, f32 aRotateZ);

extern f32 sdfAtan2(f32 aY, f32 aX);

extern void nbFormDispOn2(nbFormation_t *form);

extern void nbFormDispOff2(nbFormation_t *form);

extern void effBTLModelGetCircumference(nbFormation_t *pFormation, sdfVector4_t *pDirection);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pScene);

extern void dds3CameraObjectBasicGetTargetPos(dds3Basic_t *pCamera);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

void effBTLModelGetSphereCenter(nbFormation_t *pFormation);

/* Return the larger unscaled extent multiplied by the unit scale. */
f32 effBTLModelGetRadius(nbFormation_t *pFormation);

void effBTLModelGetDirection(nbFormation_t *pFormation, sdfVector4_t *pDirection);

void effBTLModelGetDirection2(nbFormation_t *pFormation);

/* vu0 routine: the point on the formation sphere in the given direction. */
void effBTLModelGetCircumference(nbFormation_t *pFormation, sdfVector4_t *pDirection);

void effBTLModelGetCircumferencePtr(nbFormation_t *pFormation, u16 ptr_y, u16 ptr_x);

/* Upper Y bound: scaled half-height minus the sampled position's Y. */
f32 effBTLModelGetHeight(nbFormation_t *pFormation);

f32 func_0016C898(nbFormation_t *unit);

s32 effBTLModelGetMarkPosition(nbFormation_t *pFormation, s32 mark);

/* Fall back to the bounding-sphere center when the model has no such mark. */
void effBTLModelGetMarkPosition2(nbFormation_t *pFormation, s32 mark);

void effBTLModelGetMarkPosition3(mdlObj_t *hModel, s32 mark);

s32 effBTLModelGetMarkPositionEvalTm(nbFormation_t *pFormation, s32 mark);

s32 effBTLModelGetMarkMatrix(nbFormation_t *pFormation, s32 mark);

void effBTLModelGetMarkMatrix2(nbFormation_t *pFormation, s32 mark);

void effBTLModelGroupGetCenter(u32 group);

f32 effBTLModelGroupGetMaxLength(sdfVector4_t *group, u32 model);

f32 effBTLModelGroupGetLineMaxLength(sdfVector4_t *pVec, sdfVector4_t *pPos, u32 group);

extern void effBTLGetGroupPos(nbFormation_t *form, f32 *out);

f32 effBTLModelGroupGetWidthX(u32 group);

f32 effBTLModelGroupGetSphere(u32 group);

f32 effBTLModelGroupGetSphereOld(u32 group);

f32 effBTLModelGroupGetSphere2(u32 group);

void effBTLModelGroupGetDirection(u32 group, sdfVector4_t *pDirection);

void effBTLModelLook(nbFormation_t *pFormation, sdfVector4_t *pTargetPos);

void effBTLModelGroupLook(u32 group, sdfVector4_t *pPos);

u32 effBTLModelGetGroupNumber(u32 group);

f32 effBTLModelGetGroupMaxHeight(u32 group);

void effBTLModelSetPosition(nbFormation_t *pFormation, sdfVector4_t *pPos);

void effBTLGetGroupPos(nbFormation_t *form, f32 *out);

void nbGetUnitModelPos(nbFormation_t *form, f32 *out);

void effBTLModelSetRotate(void);

void effBTLModelGetRotate(nbFormation_t *pFormation, f32 *pRot);

void effBTLModelAllPushPosition(void);

void effBTLModelAllPopPosition(void);

void effBTLModelAllPushRotate(void);

void effBTLModelAllPopRotate(void);

void effBTLModelGroupPopPosition(u32 group);

void effBTLModelGroupRotate(u32 group, f32 angle);

void effBTLModelGroupDiameterDisposition(u32 group);

void effBTLModelShowInit(void);

void effBTLModelShow(nbFormation_t *pFormation);

void effBTLModelHide(nbFormation_t *pFormation);

void effBTLModelShowRun(void);

void effBTLModelAllShow(void);

void effBTLModelAllHide(void);

void effBTLModelInvokeShow(void);

void effBTLModelTargetShow(void);

void effBTLModelInvokeHide(void);

void effBTLModelTargetHide(void);

void effBTLModelInvokeGroupHide(void);

void effBTLModelTargetGroupHide(void);

void effBTLModelInvokePartyShow(void);

void effBTLModelTargetPartyShow(void);

void effBTLModelInvokePartyHide(void);

void effBTLModelTargetPartyHide(void);

u32 effBTLModelGetInvokeGroupNumber(void);

u32 effBTLModelGetTargetGroupNumber(void);

s32 effBTLModelGetID(nbFormation_t *pFormation);

s32 effBTLModelCheckBossID(nbFormation_t *pFormation);

u16 effBTLModelGetBadStatus(nbFormation_t *pFormation);

/* Native point-light/color callers use this four-entry table as frame durations. */
u32 effBTLPartGetScalePtrSub(s32 pFormation);

f32 effBTLPartCalcScale(nbFormation_t *form);

f32 effBTLPartGetScalePtr(nbFormation_t *form);

void effBTLPartPtrGetAllModelCenterOffset(void);

void effBTLPartPtrResetAllModelCenterOffset(void);

void effBTLPartPtrGetModelCenter(nbFormation_t *pFormation);

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos);

void effBTLPartGetModelPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);

void effBTLPartGetPartyPositionPtr(u32 group, effBTLAimParams_t *pPosPtr);

void effBTLPartGetInvokePartyPositionPtr(u32 pFormation, void *pPosPtr);

void effBTLPartGetTargetPartyPositionPtr(u32 pFormation, void *pPosPtr);

void effBTLPartGetWorldPositionPtr(nbFormation_t *formation, effBTLAimParams_t *request);

void effBTLPartGetModelMarkPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr);

f32 effBTLPartGetRadiusPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr);

void effBTLPartGetPointLightPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pDir);

/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix);

void effBTLPartDebugPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr);

void effBTLModelCheckBoundsHide(nbFormation_t *pFormation, sdfVector4_t *w, sdfVector4_t *h,
                                sdfVector4_t *pTarget, f32 pEye, f32 pUp);
/* End of declarations from src/smt3/effect/effBattleMisc.c */

/* The selected position provider leaves its vector in vf10. */
void effBTLPartGetPositionPtr(nbFormation_t *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos) {
    GetPositionPtrTable[pPosPtr->scaleKind](pFormation, pPosPtr);
    SDF_VU0_STOREVECTOR(pPos);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA150);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA180);

void func_001EA1B0(void) {
}

void func_001EA1B8(void) {
}

void func_001EA1C0(void) {
}

void func_001EA1C8(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA1D0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA200);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA230);

void func_001EA400(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001EA408);

void func_001EA418(void) {
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", effBTLPartGetRotateMatrixPtr);
