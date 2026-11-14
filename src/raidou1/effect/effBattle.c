#include "common.h"

/* Declarations from smt3-decomp src/smt3/effect/effBattle.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "effBattle.h"

#include "nb.h"

#include "basic.h"

#include "ps2asm.h"

#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 60,
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0
};

extern void dds3EffectPartDelete(dds3EffectPart_t *hEffPart);

extern dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *hEffPartSrc);

extern dds3EffectPart_t *dds3EffectPartCreate(u16 type, void *pData);

extern void dds3EffectPartSetScale(dds3EffectPart_t *hEffPart, f32 scale);

extern void sdfChipFreeDirect(void *aPtr);

#include "sdk_libc.h"

extern f32 effBTLFadeColorFrom[4];

extern f32 effBTLFadeColorTo[4];

/* Non-GP BSS words: incomplete extern arrays preserve the native lui/lw
 * accesses (DDS idioms.md, "Declaring non-GP data"). */
extern s8 effBTLSkillRun;

extern s32 effBTLSkillState;

extern s32 effBTLSkillID;

extern s32 effBTLFieldColorProcess;

extern f32 effBTLFieldLightAnimAxis[4];

extern s32 effBTLFieldLightAnimFrame;

extern s32 effBTLFieldLightAnimFrameEnd;

extern f32 effBTLFieldLightAnimRotate;

extern s32 effBTLSkyFilterAlphaBase;

extern s32 effBTLSkyFilterAlphaTarget;

extern u16 effBTLSkyFilterFrame;

extern u16 effBTLSkyFilterFrameMax;

extern void effBTLFieldColorEnd(void);

extern void effBTLFieldColorOFF(void);

extern void dds3EffectFilter(dds3Effect_Filter_t *params);

extern void effBTLFieldColorRun(void);

extern void effBTLPointLightRun(void);

extern void effBTLPointLightOFF(void);

extern void effBTLSkillClearActive(void);

extern void effBTLPartPtrResetAllModelCenterOffset(void);

extern void effBTLSystemPartHandleDelete(void);

extern void nbCameraSetState(s32 state);

extern void sdfMemFreeDirect(void *aHandle);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfChipAllocDirect(s32 aSize);

extern f32 effBTLPartGetScalePtr(nbFormation_t *pFormation);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

typedef struct effBTLImage_t {
    s32 words[0xC38 / 4];
} effBTLImage_t;

extern u32 effColorBlend(u32 abgr0, u32 abgr1, f32 rate);

extern void nbStageFilterAlp(s32 alp);

extern void effBTLPointLightON(void);

extern void effBTLPointLightInit(void);

extern void effBTLSkillSetActive(void);

extern void effBTLCutReset(void);

extern void effBTLPartPtrGetAllModelCenterOffset(void);

extern void effBTLModelShowInit(void);

extern void effBTLSystemPartHandleCreate(void);

extern void effBTLFieldColorON(u32 id);

extern dds3ProcessID_t *dds3StartProcess(const char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern u16 effBTLCutCameraState[];

extern sdfVector4_t effBTLStageColorTable[];

extern sdfVector4_t effBTLDefaultColor[];

extern s32 fldSkyGetAlpha(void);

extern void *memset(void *a, s32 b, size_t c);

typedef struct effBTLSkillInfo_t {
    nbFormation_t *invokeForm;
    nbFormation_t *targetForm[32];
    u32 targetNumber;
    u32 targetBit;
} effBTLSkillInfo_t;

extern effBTLSkillInfo_t D_005698E0;

typedef struct effBTLFieldFilter_t {
    dds3Effect_Filter_t rect;
    u32 baseColor;
    u32 targetColor;
    u16 frame;
    u16 frameMax;
} effBTLFieldFilter_t;

extern effBTLFieldFilter_t effBTLFieldColorFilter;

typedef struct effBTLLightParams_t {
    f32 *sources[4];
    f32 ambient[4];
} effBTLLightParams_t;

typedef struct fldResourceLight_t {
    u32 resv;
    u32 Flag;
    u32 AnimeType;
    f32 InsideR;
    f32 OutsideR;
    f32 Soften;
    f32 Bias;
    f32 DiffuseR;
    f32 DiffuseG;
    f32 DiffuseB;
    f32 AmbientR;
    f32 AmbientG;
    f32 AmbientB;
} fldResourceLight_t;

typedef struct effBTLLightWork_t {
    u8 unk00[0x70];
    u16 switchFrame;
} effBTLLightWork_t;

extern f32 effBTLFieldColor[4];

extern f32 effBTLSavedLight1[4];

extern f32 effBTLSavedLight0[4];

extern f32 effBTLSavedAmbient[4];

extern fldResourceLight_t effBTLFieldLight;

extern u32 abgr;

extern u32 effBTLFieldColorId;

extern u32 effBTLLightColor;

extern u32 D_0054F190;

extern u32 effBTLFieldFilter[];

extern s32 effBTLFieldColorIsOn(void);

extern void effBTLFieldColorGetBaseColor(void);

extern void effBTLFieldColorStart(void);

typedef struct effBTLColor_t {
    f32 a;
    f32 r;
    f32 g;
    f32 b;
} effBTLColor_t;

/* Native 20-byte table row: alpha, red, green, blue, then packed color.
 * Array components preserve the indexed address shape; see
 * docs/levers/addressing-and-types.md. */
typedef struct effBTLColorRow_t {
    f32 color[4];
    u32 word;
} effBTLColorRow_t;

extern effBTLColorRow_t effBTLFieldColorTable[];

extern const f32 effBTLCutBaseRotate[4];

extern const f32 effBTLCutRotateA[4];

extern const f32 effBTLCutRotateB[4];

extern void effBTLPointLightSet(void *pLight, void *pFormation);

extern void effBTLModelColorSet(void *pModel, void *pFormation);

extern void effBTLPartGetPositionPtr(void *pFormation, effBTLAimParams_t *pPosPtr, sdfVector4_t *pPos);

extern void effBTLPartGetRotateMatrixPtr(effBTLAimParams_t *pPosPtr, sdfMatrix4_t *pMatrix);

extern f32 effBTLPartGetRadiusPtr(void *pFormation, effBTLAimParams_t *pPosPtr);

extern void effBTLModelGetMarkMatrix2(void *pFormation, s32 mark);

extern void effBTLCutSetMax(u8 kind, u32 frame);

extern void dds3SetQuake(s32 frame, s32 range);

extern void dds3EffectPartSetPosition(dds3EffectPart_t *hEffPart, sdfVector4_t *pPos);

extern void dds3EffectPartSetRotateMatrix(dds3EffectPart_t *hEffPart, sdfMatrix4_t *pMatrix);

extern void dds3EffectPartSetDirection(dds3EffectPart_t *hEffPart, sdfMatrix4_t *pMatrix);

extern void dds3EffectPartSetArea(dds3EffectPart_t *hEffPart, f32 area);

extern void dds3EffectPartSetColor(dds3EffectPart_t *hEffPart, u32 abgr);

extern void dds3EffectPartDraw(dds3EffectPart_t *hEffPart);

extern void Mult(void);

extern void ToMatrix(void);

extern void MultMatrixNoArg(void);

extern void SetRotateMatrixAxis(f32 aAngle, const f32 *aAxis);

extern void dds3SetLightObjectResoruce(dds3Basic_t *basic, fldResourceLight_t *resource);

extern void dds3LinkSceneLight(dds3Basic_t *basic, s32 frame);

extern void dds3UnlinkSceneLight(dds3Basic_t *basic);

extern effBTLLightParams_t dds3LightParam;

typedef f32 effBTLVector4_t[4];

extern effBTLVector4_t effBTLSceneAmbient;

effBTL_t *effBTLCreate(void *pData);

void effBTLDelete(effBTL_t *hBTLEffect);

s32 effBTLGetMotionChangeFrame(effBTL_t *hBTLEffect, u16 CutNum);

s32 effBTLGetCutEndFrame(effBTL_t *hBTLEffect, u16 CutNum);

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum);

void effBTLCutListRelease(effBTLCut_t *hBTLCutEffect);

effBTLCut_t *effBTLCutCreate(effBTL_t *hBTLEffect, u16 CutNum, void *pFormation, s32 stat);

/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
void effBTLCutDelete(effBTLCut_t *hBTLCutEffect);

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBTLCutGetCutNumber(effBTLCut_t *hBTLCutEffect);

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBTLCutGetFrame(effBTLCut_t *hBTLCutEffect);

void *effBTLCutGetFormation(effBTLCut_t *hBTLCutEffect);

void effBTLCutSetMotionChangeCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallbackFunc);

/* Store the callback used when the current frame reaches triggerFrame. */
void effBTLCutSetCutEndCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallbackFunc);

/* Read the callback frame from the linked source. */
s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *hBTLCutEffect);

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBTLCutSetCutEndFrame(effBTLCut_t *hBTLCutEffect, s32 frame);

/* Return the frame tested by the native trigger callback. */
u32 effBTLCutGetCutEndFrame(effBTLCut_t *hBTLCutEffect);

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBTLCutSetDrawEndFrame(effBTLCut_t *hBTLCutEffect, u32 frame);

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBTLCutGetDrawEndFrame(effBTLCut_t *hBTLCutEffect);

void effBTLCutSetUserData(effBTLCut_t *hBTLCutEffect, void *pData);

void *effBTLCutGetUserData(effBTLCut_t *hBTLCutEffect);

typedef struct effBTLColorWork_t {
    u32 word;
    u8 pad04[12];
} effBTLColorWork_t;

u32 effBTLCutGetAllPartColor(u32 frame, u32 frame_end);

void effBTLCutSetCameraState(effBTLCut_t *hBTLCutEffect);

/* Canonical declaration of the provider defined below. */
extern const char effBTLShakeFormat[];

u32 effBTLSkillGetInvokeParty(void);

extern const f32 effBTLCutBaseRotate[4];

extern const f32 effBTLCutRotateA[4];

extern const f32 effBTLCutRotateB[4];

extern const char effBTLShakeFormat[28];

void effBTLCutDraw(effBTLCut_t *hBTLCutEffect);

void effBTLCutSetScale(effBTLCut_t *hBTLCutEffect, f32 scale);

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData);

/* Release each initialized parameter-work entry in the handle's two banks. */
void effBTLPartDelete(effBTL_t *pSkill);

void effBTLSkillInit(void);

void effBTLSkillStart(u32 id, u32 stat);

void effBTLSkillEnd(void);

void effBTLSkillSetFormationInfo(s32 InvokeID, u32 TargetBit);

nbFormation_t *effBTLSkillGetInvokeFormation(void);

u32 effBTLSkillGetInvokeParty(void);

nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

u32 effBTLSkillGetTargetParty(void);

u32 effBTLSkillGetTargetNumber(void);

u32 effBTLSkillGetTargetBit(void);

s32 effBTLSkillCheckRun(void);

void effBTLSkillSetState(s32 state);

s32 effBTLSkillGetState(void);

s32 effBTLSkillGetID(void);

void effBTLFieldColorInit(void);

void effBTLFieldColorON(u32 id);

void effBTLFieldColorOFF(void);

void effBTLFieldColorStart(void);

void effBTLFieldColorEnd(void);

void effBTLFieldLightDirectionSet(const f32 *pDir);

void effBTLFieldLightAxisSet(const f32 *pAxis);

void effBTLFieldLightAxisSet(const f32 *pAxis);

void effBTLFieldLightAnimationSet(s32 frame, f32 angle);

void effBTLSkyFilterON(s32 alpha, u16 frame);

void effBTLSkyFilterOFF(u16 frame);

void effBTLSkyFilterRun(void);

void effBTLFieldFilterON(u32 abgr, u16 frame);

void effBTLFieldFilterOFF(u16 frame);

void effBTLFieldFilterRun(void);

void effBTLFieldFilterDraw(void);

void effBTLFieldColorRun(void);

void effBTLFieldColorDraw(void);

void effBTLFieldColorGetBaseColor(void);

s32 nbEffectProcess(void);

s32 nbEffectDraw(void);

void nbEffectShutdown(void);

void nbInitEffectProcess(nbMainProcessData_t *data);

extern s8 effBTLSkillRun;

extern s32 effBTLFieldColorProcess;

extern s32 effBTLSkyFilterAlphaTarget;

extern s32 effBTLSkyFilterAlphaBase;

extern u16 effBTLSkyFilterFrame;

extern u16 effBTLSkyFilterFrameMax;
/* End of declarations from src/smt3/effect/effBattle.c */

/* Declarations from dds-decomp src/dds1/game/code_00161838.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_EFF_BATTLE_COPY_TRIGGER_FRAME 1

#define SMT3_DEFINES_EFF_BATTLE_MINIMUM_RAMP_END_FRAME 1

#define SMT3_DEFINES_EFF_BATTLE_PARAMETER_WORK_CAPACITY 1

#define SMT3_DEFINES_EFF_PARAM_WORK_KIND_EFFECT_NODE 1

#define SMT3_DEFINES_effBTLColorRow_t 1

#define SMT3_DEFINES_effBTLColorWork_t 1

#define SMT3_DEFINES_effBTLColor_t 1

#define SMT3_DEFINES_effBTLFieldFilter_t 1

#define SMT3_DEFINES_effBTLImage_t 1

#define SMT3_DEFINES_effBTLLightParams_t 1

#define SMT3_DEFINES_effBTLLightWork_t 1

#define SMT3_DEFINES_effBTLSkillInfo_t 1

#define SMT3_DEFINES_effBTLVector4_t 1

#define SMT3_DEFINES_fldResourceLight_t 1

#include "dds/common.h"

#include "dds/sdf_vu0.h"

#include "dds/btl.h"

#include "dds/eff_field_color.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_debug_api.h"

extern u32 effFieldColorFlags;

extern u32 effFieldColorOriginalSelector;

extern u32 effFieldColorFinalSelector;

/* Keep the original selector and optional overrides separately for field-color lookups. */
void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId);

u32 effBTLFieldColorGetVariantSelector(void);

u32 effBTLFieldColorGetOverrideSelector(void);

u32 effBTLFieldColorGetFinalSelector(void);

/* Default RGB adjustment used when no field-specific color is supplied. */
void effBTLFieldColorGetFixedVector(u32 unused, f32 *color);

extern f32 D_00350F60[][4];

extern f32 D_00352880[4];

extern EffFieldColorRecord D_0034E730[];

/* Variant and kind are narrowed to their stored widths before the lookup. */
void effBTLFieldColorLookupNarrowSelectors(s32 colorId, s16 variant, s16 kind, f32 *output);

u32 effBTLFieldColorGetEntryWord(s32 index);

void effBTLFieldColorSetFlags(u32 flags);

void effBTLFieldColorClearFlags(u32 flags);

void effBTLFieldColorResetFlags(void);

f32 effBTLFieldColorGetActorScale(BtlUnit *unit);

f32 effGetActorScaleForParameterWork(BtlUnit *unit);
/* End of declarations from src/dds1/game/code_00161838.c */

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4A08);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4B48);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4BA0);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4BD0);

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    if (CutNum == 2) {
        return hBTLEffect->banks[bank].list.frame;
    }
    return hBTLEffect->banks[bank].list.colorRampEndFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4C58);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4CA0);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4E48);

u16 func_r1_001E4ED8(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->kind;
}

u32 func_r1_001E4EE0(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->currentFrame;
}

void *func_r1_001E4EE8(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->formation;
}

void func_r1_001E4EF0(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallback) {
    hBTLCutEffect->sourceFrameCallback = pCallback;
}

void func_r1_001E4EF8(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallback) {
    hBTLCutEffect->triggerFrameCallback = pCallback;
}

s32 func_r1_001E4F00(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->list->callbackFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4F10);

u32 func_r1_001E4F28(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->triggerFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4F30);

u32 func_r1_001E4F60(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->colorRampEndFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4F68);

void *func_r1_001E4F70(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->userData;
}

void func_r1_001E4F78(void) {
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4F80);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E4FF8);

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData) {
    u8 *sourceBytes = (u8 *)pData;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;
        u16 entryCountForBank = pSkill->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->type != 0xFFFE) {
                    if (entry->type == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 sourceBank = index / 0x3C;
                        u32 sourceEntryIndex = index % 0x3C;
                        effBTL_SkillPartsList_t *sourceEntry = &pSkill->banks[sourceBank].list.entries[sourceEntryIndex];
                        dds3EffectPart_t *duplicatedWork = dds3EffectPartCopy(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->type;

                        entry->value.work = duplicatedWork;
                        entry->type = sourceKind;
                    } else {
                        entry->value.work = dds3EffectPartCreate(entry->type, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->type == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
                        entry->timing++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
    }
}

/* Release each initialized parameter-work entry in the handle's two banks. */
void effBTLPartDelete(effBTL_t *pSkill) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = pSkill->banks[bankIndex].list.count;
        effBTL_SkillPartsList_t *entry = pSkill->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->type != 0xFFFE) {
                    dds3EffectPartDelete(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E5230);

extern s32 D_r1_0046CF84;

s32 func_r1_001E5250(void) {
    return D_r1_0046CF84;
}

extern s32 D_r1_0046CF88;

s32 func_r1_001E5258(void) {
    return D_r1_0046CF88;
}

u32 effBTLFieldColorGetOverrideSelector(void) {
    return effBTLSkillID;
}

u32 effBTLFieldColorGetFinalSelector(void) {
    return effFieldColorFinalSelector;
}

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E5270);

INCLUDE_ASM(const s32, "effect/effBattle", func_r1_001E5290);

void func_r1_001E5398(void *a0, s32 a1, s32 a2) {
    func_r1_001E5290(a0, (s16)a1, (s16)a2);
}

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455700);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455710);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455720);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455730);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455740);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455750);

INCLUDE_RODATA(const s32, "effect/effBattle", D_r1_00455760);

INCLUDE_SDATA(const s32, "effect/effBattle", D_r1_0046CF84);

INCLUDE_SDATA(const s32, "effect/effBattle", D_r1_0046CF88);

INCLUDE_SDATA(const s32, "effect/effBattle", D_r1_0046CF8C);

INCLUDE_SDATA(const s32, "effect/effBattle", D_r1_0046CF90);

