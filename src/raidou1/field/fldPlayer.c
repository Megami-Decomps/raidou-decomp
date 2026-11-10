#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldPlayer.c */
#include "fpu.h"

#include "common.h"

#include "libsdf.h"

#include "kernel.h"

#include "ps2asm.h"

#include "sdfFrame.h"

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

dds3Basic_t *dds3GetActiveScene(void);

extern f32 R360_LR(f32 a, f32 b);

extern f32 fabsf(f32);

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void dds3ObjectBaseOffFlag(void *pObjectBasic, s32 flag);

extern void dds3ObjectBaseOnFlag(void *object, s32 mask);

extern s32 fldAutoMapChkMode(void);

s32 evtBitCheck(s32 no);

void fldPlayerOff(void);

void fldPlayerSvsOff(void);

void fldPlayerSdwOff(void);

extern f32 fldWmFastMove;

extern s32 gfldPlayerHasiCnt;

extern s32 gfldPlayerDamegeCnt;

extern s32 fldPlayerStepDist;

extern s32 fldWmWarpMode;

extern fldUnitControl_t *fldGetUnitControl(void);

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *pObjectBasic, u32 svs);

extern dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);

extern dds3ObjectBaseControlWorkData_t *dds3UnitObjectBasicGetControlWorkData(dds3Basic_t *pBasic);

extern void fldPlayerMotion(s32 hisMotion, s32 upMotion, f32 motHokan);

extern void fldPlayerMotReset2(void);

extern s32 fldPlayerFastMode;

extern s32 l_UseCollision;

extern void fldHitChkUnitElv(f32 *phys_pos);

extern void fldHitChkUnit(f32 *phys_pos);

extern void dds3OffPoseSetFlag(dds3PoseSet_t *pPoseSet, u32 flag);

extern void dds3SetSclPoseSet(dds3Basic_t *pBasic, f32 *scl);

extern f32 fldWmapPlayerState;

extern const sdfVector4Unaligned_t fldWm3Scale;

extern const sdfVector4Unaligned_t fldWm4Scale;

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, s32 flag);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

f32 R360_LR(f32 a, f32 b);

f32 fldPlayerCalc360_8(f32 aKakudo);

f32 fldPlayerCalc360_4(f32 aKakudo);

s32 fldPlayerMirChk(void);

extern s32 fldChkVortex(void);

extern void mdlLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void mdlSmoothLoopMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber,
                                f32 aBeforeLeng, f32 aSmoothLeng);

void fldPlayerMotion(s32 hisMotion, s32 upMotion, f32 motHokan);

extern f32 fldCamTarget[];

extern f32 fldCamEye[];

extern void fldSePlay(s32 id);

extern void fldSeVol(s32 id, s32 volume);

extern u32 dds3GetRandA(void *name);

/* Native surface-category branch is retained by the localized operand below. */
/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
void fldPlayerMotionSe(s32 hisMotion, s32 upMotion);

void fldPlayerCalc_Hit(void);

extern const sdfVector4_t fldWarpRunAxis;

extern const sdfVector4_t fldNmlAxis;

void fldPlayerCalc_WarpStart(void);

void fldPlayerCalc_FastStart(void);

void fldPlayerCalc_Wm3(void);

void fldPlayerCalc_Wm4(void);

extern const char fldNoroiDamageOnMsg[];

extern const char fldNoroiDamageOffMsg[];

extern s32 fldEncountTestFlag(u32 mask);

extern void fldEncountSetFlag(s32 mask);

extern void mdlSingleMotion(mdlObj_t *aHandle, s32 aGroup, s32 aNumber);

extern void fldDmgStart(f32 x, f32 y, f32 z);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frames);

extern void fldStatusSet(void);

extern void fldPlayerCalc_WarpRun(void);

extern void fldPlayerMotionSe(s32 hisMotion, s32 upMotion);

extern s32 fldPlayerCalc_Wm2(void);

extern s32 fldPlayerCalc_Nml(void);

extern s32 fldPlayerCalc_Nml_syukan(void);

/* Keep the mode-dispatch mirror check opaque to same-unit optimization.
 * The direct view merges its tail with the default arm; this canonical
 * same-symbol view retains the separate native call without changing ABI. */
extern s32 fldPlayerMirChkDispatch(void) FAKEMATCH_ASM_NAME("fldPlayerMirChk");

s32 fldPlayerCalc(void);

s32 fldPlayerChkAna(void);

void fldPlayerOff(void);

void fldPlayerOn(void);

void fldPlayerSvsOff(void);

void fldPlayerSvsOn(void);

void fldPlayerAlpOff(void);

void fldPlayerAlpOn(void);

void fldPlayerSdwOn(void);

void fldPlayerSdwOff(void);

s32 fldPlayerFastSet(void);

void fldPlayerMotReset(void);

void fldPlayerMotReset2(void);

extern const sdfVector4Unaligned_t fldWm3Scale;

extern const sdfVector4Unaligned_t fldWm4Scale;

extern const sdfVector4Unaligned_t fldWm2Scale;

extern const sdfVector4Unaligned_t fldWm2WorkInit;

extern const sdfVector4Unaligned_t fldNmlSyukanAxis;

extern const char fldNoroiDamageOnMsg[24];

extern const char fldNoroiDamageOffMsg[32];

extern f32 fldPlayerWarpState;

extern s32 fldPlayerFastMode;

extern s32 gfldPlayerHasiCnt;

extern s32 gfldPlayerDamegeCnt;

extern s32 fldPlayerStepDist;

extern s32 fldPlayerMoveState;

extern s32 fldPlayerSyukanState;

extern u32 fldPlayerMoveStateSaved;

extern s32 fldWmapMatterLast;

extern s32 fldWmapBlendStep;

extern s32 fldWmWarpMode;

extern f32 fldWmapPlayerState;

extern f32 fldWmFastMove;

extern s32 fldSyukanState;

extern s32 fldSkyEnable;
/* End of declarations from src/smt3/field/fldPlayer.c */

INCLUDE_ASM(const s32, "field/fldPlayer", func_001274A8);

INCLUDE_ASM(const s32, "field/fldPlayer", fldSnapAngleToCompassPoint);

f32 fldPlayerCalc360_8(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(45.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 1;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(135.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 3;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(225.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 5;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 6;
    }
    diff = fabsf(R360_LR(315.0f, aKakudo));
    if (diff < best) {
        index = 7;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 1:
        aKakudo = 45.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 3:
        aKakudo = 135.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 5:
        aKakudo = 225.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    case 7:
        aKakudo = 315.0f;
        break;
    }
    return aKakudo;
}

f32 fldPlayerCalc360_4(f32 aKakudo) {
    f32 best;
    s32 index;
    f32 diff;
    while (aKakudo >= 360.0f) {
        aKakudo -= 360.0f;
    }
    while (aKakudo < 0.0f) {
        aKakudo += 360.0f;
    }
    best = 900.0f;
    index = -1;
    diff = fabsf(R360_LR(0.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 0;
    }
    diff = fabsf(R360_LR(90.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 2;
    }
    diff = fabsf(R360_LR(180.0f, aKakudo));
    if (diff < best) {
        best = diff;
        index = 4;
    }
    diff = fabsf(R360_LR(270.0f, aKakudo));
    if (diff < best) {
        index = 6;
    }
    switch (index) {
    case 0:
        aKakudo = 0.0f;
        break;
    case 2:
        aKakudo = 90.0f;
        break;
    case 4:
        aKakudo = 180.0f;
        break;
    case 6:
        aKakudo = 270.0f;
        break;
    }
    return aKakudo;
}
