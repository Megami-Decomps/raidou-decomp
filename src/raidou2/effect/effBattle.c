#include "common.h"
#include "raidou_alloc.h"
#include "sdk_libgcc.h"
#include "effBattle.h"
#include "nb.h"
#include "basic.h"
#include "ps2asm.h"
#include "sdfFrame.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 60,
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0
};

extern void dds3EffectPartDelete(dds3EffectPart_t *hEffPart);
extern dds3EffectPart_t *dds3EffectPartCopy(dds3EffectPart_t *hEffPartSrc);
extern dds3EffectPart_t *dds3EffectPartCreate(u16 type, void *pData);

#include "sdk_libc.h"

extern s32 effBTLSkillID;

typedef struct effBTLImage_t {
    s32 words[0xC38 / 4];
} effBTLImage_t;

typedef struct effBTLSkillInfo_t {
    nbFormation_t *invokeForm;
    nbFormation_t *targetForm[32];
    u32 targetNumber;
    u32 targetBit;
} effBTLSkillInfo_t;

typedef struct effBTLFieldFilter_t {
    dds3Effect_Filter_t rect;
    u32 baseColor;
    u32 targetColor;
    u16 frame;
    u16 frameMax;
} effBTLFieldFilter_t;

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

extern u32 abgr;

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

typedef f32 effBTLVector4_t[4];

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum);

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBTLCutGetCutNumber(effBTLCut_t *hBTLCutEffect);

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBTLCutGetFrame(effBTLCut_t *hBTLCutEffect);
void *effBTLCutGetFormation(effBTLCut_t *hBTLCutEffect);

/* Read the callback frame from the linked source. */
s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *hBTLCutEffect);

/* Return the frame tested by the native trigger callback. */
u32 effBTLCutGetCutEndFrame(effBTLCut_t *hBTLCutEffect);

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBTLCutGetDrawEndFrame(effBTLCut_t *hBTLCutEffect);
void *effBTLCutGetUserData(effBTLCut_t *hBTLCutEffect);

typedef struct effBTLColorWork_t {
    u32 word;
    u8 pad04[12];
} effBTLColorWork_t;

void effBTLPartCreate(effBTL_t *pSkill, effBTL_t *pData);
void effBTLPartDelete(effBTL_t *pSkill);
s32 effBTLSkillGetState(void);
s32 effBTLSkillGetID(void);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/btl.h"

extern u32 effFieldColorFinalSelector;
u32 effBTLFieldColorGetOverrideSelector(void);
u32 effBTLFieldColorGetFinalSelector(void);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCreate);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLDelete);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLGetMotionChangeFrame);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLGetCutEndFrame);

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    if (CutNum == 2) {
        return hBTLEffect->banks[bank].list.frame;
    }
    return hBTLEffect->banks[bank].list.colorRampEndFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutListRelease);

INCLUDE_ASM(const s32, "effect/effBattle", effBTL);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutDelete);

u16 effBTLCutGetCutNumber(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->kind;
}

u32 effBTLCutGetFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->currentFrame;
}

void *effBTLCutGetFormation(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->formation;
}

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf12);

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf13);

s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->list->callbackFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf14);

u32 effBTLCutGetCutEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->triggerFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetDrawEndFrame);

u32 effBTLCutGetDrawEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->colorRampEndFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf15);

void *effBTLCutGetUserData(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->userData;
}

void effLeaf16(void) {
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetScale);

INCLUDE_ASM(const s32, "effect/effBattle", effBattleHasActiveKind);

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

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf17);

extern s32 effData24;

s32 effBTLSkillGetState(void) {
    return effData24;
}

extern s32 effData25;

s32 effBTLSkillGetID(void) {
    return effData25;
}

u32 effBTLFieldColorGetOverrideSelector(void) {
    return effBTLSkillID;
}

u32 effBTLFieldColorGetFinalSelector(void) {
    return effFieldColorFinalSelector;
}

INCLUDE_ASM(const s32, "effect/effBattle", effLeaf18);

INCLUDE_ASM(const s32, "effect/effBattle", effBTLFieldColorGetBaseColor2);

void effLeaf19(void *a0, s32 a1, s32 a2) {
    effBTLFieldColorGetBaseColor2(a0, (s16)a1, (s16)a2);
}

INCLUDE_RODATA(const s32, "effect/effBattle", filterStrParameterInit);

INCLUDE_RODATA(const s32, "effect/effBattle", mfilterDebugMenuStrCenterMove);

INCLUDE_RODATA(const s32, "effect/effBattle", mblurStrMultiBlur);

INCLUDE_RODATA(const s32, "effect/effBattle", dblurStrDistortionBlur);

INCLUDE_RODATA(const s32, "effect/effBattle", filterStrParallelBlur);

INCLUDE_RODATA(const s32, "effect/effBattle", mfilterDebugMenuStrMultiFilter);

INCLUDE_RODATA(const s32, "effect/effBattle", rblurStrRippleBlur);

INCLUDE_SDATA(const s32, "effect/effBattle", effData24);

INCLUDE_SDATA(const s32, "effect/effBattle", effData25);

INCLUDE_SDATA(const s32, "effect/effBattle", effBTLSkillID);

INCLUDE_SDATA(const s32, "effect/effBattle", effData26);

