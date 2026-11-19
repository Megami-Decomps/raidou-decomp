#include "common.h"
#include "raidou_alloc.h"
#include "sdk_libgcc.h"
#include "effBattle.h"
#include "nb.h"
#include "basic.h"
#include "ps2asm.h"
#include "sdfFrame.h"
#include "sdk_libc.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "effPart.h"
#include "code_001EA090.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 60,
    EFF_PARAM_WORK_KIND_EFFECT_NODE = 0
};



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

#include "include_asm.h"
#include "newdata.h"

extern u32 effFieldColorFinalSelector;
extern s32 effData24;
extern s32 effData25;
u32 effBTLFieldColorGetOverrideSelector(void);
u32 effBTLFieldColorGetFinalSelector(void);

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:186, near, same function 0.93 (words 80/83, calls 0.86, shape 0.97) */
effBTL_t *effBTLCreate(void *pData) {
    sdfMemHandle_t *handle;
    effBTL_t *work;

    handle = sdfMemAllocDirect(0xC44);
    work = sdfMemLock(handle);
    work->resource = handle;
    *(effBTLImage_t *)work = *(effBTLImage_t *)pData;
    effBTLPartCreate(work, pData);
    work->CutCount = 0;
    work->hCutListTop = NULL;
    return work;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:205, near, same function 0.87 (words 21/24, calls 0.80, shape 0.93) */
void effBTLDelete(effBTL_t *hBTLEffect) {
    effBTLCut_t *cut;
    effBTLCut_t *next;

    cut = hBTLEffect->hCutListTop;
    while (cut != NULL) {
        next = cut->next;
        effBTLCutDelete(cut);
        cut = next;
    }
    effBTLPartDelete(hBTLEffect);
    sdfMemFreeDirect(hBTLEffect->resource);
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:220, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
s32 effBTLGetMotionChangeFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    return hBTLEffect->banks[bank].list.callbackFrame;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:225, match, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
s32 effBTLGetCutEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    return hBTLEffect->banks[bank].list.frame;
}

s32 effBTLGetDrawEndFrame(effBTL_t *hBTLEffect, u16 CutNum) {
    s32 bank = CutNum >= 2;
    if (CutNum == 2) {
        return hBTLEffect->banks[bank].list.frame;
    }
    return hBTLEffect->banks[bank].list.colorRampEndFrame;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:238, match, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
void effBTLCutListRelease(effBTLCut_t *hBTLCutEffect) {
    effBTLCut_t **link = &hBTLCutEffect->state->hCutListTop;
    effBTLCut_t *cur;

    while ((cur = *link) != NULL) {
        if (cur == hBTLCutEffect) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

/* NON_MATCHING: 100 of 105 words match.  */
#ifdef NON_MATCHING
effBTLCut_t *effBTL(effBTL_t *hBTLEffect, u16 CutNum, void *pFormation, s32 stat) {
    effBTLCut_t *cut;
    effBTL_SkillCutTable_t *list;
    effBTL_SkillCutTable_t *cutList;
    effBTL_SkillPartsList_t *entry;
    effPartHandle_t **work;
    u8 actorScaleEnabled;
    s32 remaining;
    u16 count;

    cut = sdfChipAllocDirect(0x128);
    memset(cut, 0, 0x128);
    cut->color = 0x80808080;
    list = (CutNum < 2) ? &hBTLEffect->banks[0].list : &hBTLEffect->banks[1].list;
    cut->next = hBTLEffect->hCutListTop;
    cut->flags = 14;
    cut->triggerFrame = list->frame;
    cut->state = hBTLEffect;
    hBTLEffect->hCutListTop = cut;
    cut->kind = CutNum;
    cut->list = list;
    if (CutNum == 2) {
        cut->colorRampEndFrame = list->frame;
    } else {
        cut->colorRampEndFrame = list->colorRampEndFrame;
    }
    cutList = cut->list;
    work = cut->parameterWorks;
    cut->stat = stat;
    cut->formation = pFormation;
    entry = cutList->entries;
    actorScaleEnabled = cutList->actorScaleEnabled;
    count = cutList->count;
    if (count != 0) {
        remaining = count;
        do {
            if (entry->type != 0xFFFE) {
                *work = dds3EffectPartCopy(entry->value.work);
                if (actorScaleEnabled != 0 && pFormation != NULL) {
                    if (entry->pos_ptr.scaleKind == 0 || entry->pos_ptr.scaleKind == 4) {
                        effBTLCutSetScaleSub(*work, effLeaf35(cut->formation));
                    }
                }
            } else {
                *work = NULL;
            }
            remaining--;
            work++;
            entry++;
        } while (remaining != 0);
    }
    cut->currentFrame = 0;
    printf("effBTL:cut %d handle create(%p)[%d]\n", cut->kind, cut->state, cut->state->CutCount);
    cut->state->CutCount++;
    return cut;
}
#else
INCLUDE_ASM(const s32, "effect/effBattle", effBTL);
#endif

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:308, near, same function 1.00 (words 36/36, calls 1.00, shape 1.00) */
void effBTLCutDelete(effBTLCut_t *hBTLCutEffect) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_WORK_CAPACITY; workIndex++) {
        if (hBTLCutEffect->parameterWorks[workIndex] != 0) {
            dds3EffectPartDelete(hBTLCutEffect->parameterWorks[workIndex]);
        }
    }
    effBTLCutListRelease(hBTLCutEffect);
    hBTLCutEffect->state->CutCount--;
    printf("effBTL:cut %d handle delete(%p)[%d]\n", hBTLCutEffect->kind, hBTLCutEffect->state,
           hBTLCutEffect->state->CutCount);
    sdfChipFreeDirect(hBTLCutEffect);
}

u16 effBTLCutGetCutNumber(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->kind;
}

u32 effBTLCutGetFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->currentFrame;
}

void *effBTLCutGetFormation(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->formation;
}

#ifdef VERSION_RAIDOU1
void effBTLCutSetMotionChangeCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallback) {
    hBTLCutEffect->sourceFrameCallback = pCallback;
}
#else
INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetMotionChangeCallbackFunc);
#endif

#ifdef VERSION_RAIDOU1
void effBTLCutSetCutEndCallbackFunc(effBTLCut_t *hBTLCutEffect, effBTLCallback_t pCallback) {
    hBTLCutEffect->triggerFrameCallback = pCallback;
}
#else
INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetCutEndCallbackFunc);
#endif

s32 effBTLCutGetMotionChangeFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->list->callbackFrame;
}

INCLUDE_ASM(const s32, "effect/effBattle", effBTLCutSetCutEndFrame);

u32 effBTLCutGetCutEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->triggerFrame;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:365, match, same function 1.00 (words 12/12, calls 1.00, shape 1.00) */
void effBTLCutSetDrawEndFrame(effBTLCut_t *hBTLCutEffect, u32 frame) {
    if (hBTLCutEffect->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (frame < hBTLCutEffect->colorRampEndFrame) {
            hBTLCutEffect->colorRampEndFrame = frame;
        }
        return;
    }
    hBTLCutEffect->colorRampEndFrame = frame;
}

u32 effBTLCutGetDrawEndFrame(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->colorRampEndFrame;
}

void effBTLCutSetUserData(effBTLCut_t *hBTLCutEffect, void *userData) {
    hBTLCutEffect->userData = userData;
}

void *effBTLCutGetUserData(effBTLCut_t *hBTLCutEffect) {
    return hBTLCutEffect->userData;
}

void effLeaf16(void) {
}

/* body: matched in smt3 @ 9401f79e: src/smt3/effect/effBattle.c:572, near, same function 1.00 (words 30/30, calls 1.00, shape 1.00) */
void effBTLCutSetScale(effBTLCut_t *hBTLCutEffect, f32 scale) {
    s32 i;
    s32 count;

    if (scale != 1.0f) {
        count = hBTLCutEffect->list->count;
        for (i = 0; i < count; i++) {
            effBTLCutSetScaleSub(hBTLCutEffect->parameterWorks[i], scale);
        }
    }
}

s32 effBattleHasActiveKind(effBTLCut_t *hBTLCutEffect) {
    effBTL_SkillCutTable_t *list = hBTLCutEffect->list;
    s32 count = list->count;
    effBTL_SkillPartsList_t *entry = list->entries;
    s32 i;

    for (i = 0; i < count; i++, entry++) {
        if (entry->reverberations == 3) {
            return 1;
        }
        if (entry->reverberations == 2) {
            return 1;
        }
    }
    return 0;
}

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
                        effPartHandle_t *duplicatedWork = dds3EffectPartCopy(sourceEntry->value.work);
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

void effBTLSkillSetState(s32 state, s32 id, s32 overrideSelector, s32 finalSelector) {
    effData24 = state;
    effData25 = id;
    effBTLSkillID = overrideSelector ? overrideSelector : state;
    effFieldColorFinalSelector = finalSelector ? finalSelector : state;
}


s32 effBTLSkillGetState(void) {
    return effData24;
}


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

void effBTLFieldColorGetBaseColor2(s32 colorId, s16 variant, s16 kind, f32 *output) {
    f32 scaled[4];
    s16 index = variant - 200;

    if ((u16)index >= 40 || kind <= 0 || kind >= 11) {
        printf("effBTLFieldColorGetBaseColor:DummyColorLoad\n");
        SDF_VU0_LOADVECTOR(effBTLDefaultColor);
    } else {
        SDF_VU0_LOADVECTOR(effBTLStageColorTable[(index * 10) + kind - 1]);
    }
    scaled[1] = effBTLFieldColorGetBaseColor2Data[colorId][1];
    scaled[2] = effBTLFieldColorGetBaseColor2Data[colorId][2];
    scaled[3] = effBTLFieldColorGetBaseColor2Data[colorId][3];
    SDF_VU0_LOADVECTOR_B(scaled);
    SDF_VU0_MULVECTOR();
    SDF_VU0_STOREVECTOR(scaled);
    output[0] = scaled[1];
    output[1] = scaled[2];
    output[2] = scaled[3];
}

void effLeaf19(s32 colorId, s32 variant, s32 kind, f32 *output) {
    effBTLFieldColorGetBaseColor2(colorId, (s16)variant, (s16)kind, output);
}

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "effect/effBattle", effStrEffBtlEmpty);

#endif
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

