#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldCamera.c */
#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "kernel.h"

#include "ps2asm.h"

#include "basic.h"

#include "dds3obj.h"

#include "billboard.h"

#include "draw.h"

#include "scr.h"

#include "draw3d.h"

#include "field.h"

#include "model.h"

#include "motion.h"

#include "newdata.h"

#include "event.h"

#include "effect.h"

#include "sdfFrame.h"

#include "sdfAspect.h"

extern void *memset(void *, int, size_t);

f32 sdfSin(f32 aRadius);

f32 sdfCos(f32 aRadius);

fldFollowCameraControl_t *fldGetFollowCameraControl(void);

extern f32 fldCamPhokanSelX[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

extern f32 fldCamPhokanEyeX[];

extern f32 fldCamPhokanTgtX[];

extern f32 fldCamEye[];

extern f32 fldCamTarget[];

dds3Basic_t *fldGetUnitFollowCamera(void);

dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *pBasic);

dds3Basic_t *dds3GetActiveScene(void);

extern f32 fldCamElvState;

extern f32 fldElvCameraTarget[];

extern f32 fldElvCameraPos[];

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, f32 *tgtpos,
                                                      f32 *campos);

extern void dds3OnPoseSetFlag(dds3PoseSet_t *, u32);

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern s32 strcmp(const char *left, const char *right);

s32 evtBitCheck(s32 no);

extern s32 fldCamResetOn;

extern s32 fldCamMiwatasi;

extern s32 fldElvCameraScene;

extern void fldCamSetChaseCore(f32 height);

extern fldUnitControl_t *fldGetUnitControl(void);

extern f32 fldCamLength;

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

f32 fldCalcAngleBetween(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ);

f32 fldCamCalcSlopeOffset(f32 x, f32 z, f32 *xs, f32 *ys, f32 *zs);

void fldCamSetLeng(void);

void fldCamResetNpc(void);

extern f32 fldCamPitch;

typedef struct {
    f32 distance;
    f32 eyeHeight;
    f32 targetHeight;
    f32 fovy;
} fldCamParamRow_t;

extern fldCamParamRow_t fldCamParamTable[];

extern f32 fldCamDirLock;

extern s32 fldHitChkMatter;

extern s32 fldHitChkSlope;

extern collision_s *fldMatterTable;

/* Select a valid fallback hit only in the -1 arm. Keeping that observation
 * separate from the selected row preserves the native entry dataflow; an
 * eager copy followed by reusing the observation is folded by the first
 * CSE pass. This ordinary branch structure needs no register constraint. */
void fldCamCalcSaka(void);

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);

extern f32 fldCamDist;

extern f32 fldCamHeight;

extern f32 fldCamParamTable2;

extern f32 fldCamHeightTarget;

extern f32 fldCamPitchTarget;

extern s32 fldHitChkCamera(const f32 start[3], const f32 end[3], f32 hitPoint[3]);

extern void fldCamSetLeng(void);

/* Compiler reconstruction, not recovered source spelling: the initial angle
 * conversion uses its real scalar-field address rather than the later full
 * camera-state base. Qualified CSE dumps retain the distinct HIGH quantities
 * and the later base/constant loads. The three local homes die at the initial
 * store, before the pose call; none relies on a call-clobbered value surviving.
 * Vector copies name their actual four-float memory so unrelated camera state
 * remains available across them. Native scratch W lanes are left untouched. */
void resetCamNormal2(void);

void fldCamGetPhokanParam(void);

void fldCamUpdateFollow(void);

extern f32 R360_LR(f32 a, f32 b);

extern s32 fldEncountTestFlag(u32 flag);

extern s8 dds3PadButtonState[];

extern s32 fldCamFastFlag;

extern s32 l_UseCollision;

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, const sdfVector4_t *target);

extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

/* DDS player-follow camera reconstruction adapted to SMT3 native geometry,
 * smoothing and collision behavior; see docs/normal-camera-matching.md. */
void calcCamNormal(void);

extern f32 fldCamDefaultPitch;

/* Native direction-locked sibling of calcCamNormal; see
 * docs/dir-lock-camera-matching.md for its retained reference-vector stores. */
void calcCamDirLock(void);

extern s32 fldNpcCamTarget;

extern f32 fldCamNpcTarget;

extern f32 fldCamNpcTarget2;

extern f32 fldNpcCamPosX;

extern f32 fldNpcCamPosY;

extern f32 fldNpcCamPosZ;

extern f32 fldNpcCamAngle;

extern f32 fldNpcCamTargetY;

extern void dds3CameraObjectBasicGetCameraTM(dds3Basic_t *camera, const sdfVector4_t *target);

extern void dds3GetTMPoseSet(dds3Basic_t *pBasic);

void calcCamNormalNPC(void);

extern void dds3GetRotPoseSet(dds3Basic_t *pBasic);

extern f32 EulerAngleAxisY(void);

extern f32 fldCamPitch;

void resetCamTgtPlayer(void);

void fldCamSetChase(void);

void fldCamUpdateDefault(void);

extern u8 dds3PadStickState[2][2][2];

extern f32 fldCamPitch;

extern f32 fldCamOrbitAngle;

extern f32 fldCamPadHeight;

/* Existing camera-vector storage and construction/smoothing phases; see
 * docs/camera-dispatch-matching.md for the Chase/Main family qualification. */
void fldCamUpdateChase(void);

dds3Basic_t *fldCamNowEveCamera(void);

/* First-person sibling of the normal follow camera. See
 * docs/first-person-camera-matching.md for native geometry and storage. */
void calcCamSyukan(void);

void fldSetElvCameraScene(u32 mode);

void fldCamElvTask(void);

void fldClearElvCameraScene(void);

extern void fldPlayerAlpOn(void);

extern void fldPlayerSvsOn(void);

extern void fldPlayerAlpOff(void);

extern void fldPlayerSdwOff(void);

extern void fldPlayerSvsOff(void);

extern void fldPlayerOff(void);

extern void fldCamCalcSaka(void);

extern void fldCamUpdateChase(void);

extern void fldEventUpdateEx_Unity_2(void);

extern void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frame);

extern const char fldCamElvStartLabel[];

extern s32 D_0054EC94;

extern f32 D_0054EC98;

extern f32 D_0054EC9C;

/* Six-mode camera dispatcher; native returns NULL on every path. */
extern const char fldCamElvStartLabel[15];

void *fldCamMain(void);

extern void *dds3SearchProcessName(const char *namestr);

extern void dds3SetScreenFovy(f32 fovy);

extern const char fldCamElvStartLabel[];

void fldCamSyukanSuriawase(void);

s32 fldCamMiwatasiChk(void);

s32 fldCamNpcCamChk(char *pName);

extern f32 fldCamDefaultPitch;

extern f32 fldCamDist;

extern s32 fldCamMiwatasi;

extern s32 D_0054EC94;

extern f32 D_0054EC98;

extern f32 D_0054EC9C;

extern f32 fldCamHeightTarget;

extern f32 fldCamHeight;

extern f32 fldCamParamTable2;

extern s32 fldCamResetOn;

extern s32 fldCamFastFlag;

extern f32 fldCamOrbitAngle;

extern f32 fldCamPadHeight;

extern s32 fldElvCameraScene;

extern f32 fldCamElvState;

extern s32 D_0054ECC4;

extern s32 fldHitChkMatter;

extern s32 fldHitChkSlope;

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

extern s32 fldCamSvsState;

extern f32 fldCamPitchTarget;
/* End of declarations from src/smt3/field/fldCamera.c */

f32 GetSphericalPosMove(f32 fromX, f32 fromZ, f32 toX, f32 toZ) {
    s32 angle = (s32)(360.0f - fldCalcAngleBetween(fromX, fromZ, toX, toZ) + 90.0f);
    return (f32)(angle % 360);
}

INCLUDE_ASM(const s32, "field/fldCamera", func_00127E78);
