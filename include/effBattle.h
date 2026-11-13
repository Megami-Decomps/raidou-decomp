#ifndef EFF_BATTLE_H
#define EFF_BATTLE_H

#include "common.h"
#include "libsdf.h"
#include "effect.h"

typedef struct effBTLCut_t effBTLCut_t;
typedef struct dds3EffectPart_t dds3EffectPart_t;
typedef void (*effBTLCallback_t)(effBTLCut_t *cut);

/* Native eight-byte position/rotation selector shared by BED entries. */
typedef struct effBTLAimParams_t {
    u8 scaleKind;
    u8 aimMode;
    u8 directionMode;
    u8 unk03;
    s32 rangeOverride;
} effBTLAimParams_t;

typedef union effBTLPartValue_t {
    u32 sourceOffset;
    dds3EffectPart_t *work;
} effBTLPartValue_t;

typedef struct effBTL_SkillPartsList_t {
    u16 type;
    u8 pHandle[2];
    effBTLPartValue_t value;
    effBTLAimParams_t pos_ptr;
    u32 timing;
    u8 reverberations;
    u8 unk15[3];
} effBTL_SkillPartsList_t;

typedef struct effBTL_SkillCutTable_t {
    s32 frame;              /* 0x00 */
    u8 pad04[2];
    u16 shake;                     /* 0x06: screen-shake amount; 31 or more logs and caps at 300 frames */
    u16 shakeFrame;                /* 0x08: frame at which the shake starts */
    u16 shakeFade;                 /* 0x0A: fade applied with the shake */
    u8 colorKeys[0x24];            /* 0x0C: colour keyframes read by the unit colour keyframe routine */
    u8 rgbKeys[0x18];              /* 0x30: RGB keyframes read by the unit RGB keyframe routine */
    s32 callbackFrame;             /* 0x48 */
    u8 pad4C[0x20];
    u16 count;                     /* 0x6C */
    u8 pad6E[2];
    s32 colorRampEndFrame;         /* 0x70 */
    u8 actorScaleEnabled;          /* 0x74 */
    u8 pad75[3];
    effBTL_SkillPartsList_t entries[1];     /* 0x78 */
} effBTL_SkillCutTable_t;

/* Each BED bank reserves sixty entries, for a native stride of 0x618. */
typedef struct effBTLBank_t {
    u8 pad00[8];
    effBTL_SkillCutTable_t list;       /* 0x08 */
    u8 pad98[0x580];
} effBTLBank_t;

/* effBTLCreate allocates 0xC44 bytes; its two banks occupy 0xC30. */
typedef struct effBTL_t {
    effBTLBank_t banks[2];
    u8 unkC30[8];
    sdfMemHandle_t *resource;
    u32 CutCount;
    effBTLCut_t *hCutListTop;
} effBTL_t;

/* effBTLCutCreate allocates and clears 0x128 bytes in both Raidou games.
 * smt3-decomp's layout lacks the color and flags words at 0x08. */
struct effBTLCut_t {
    void *formation; /* 0x00 */
    effBTL_t *state; /* 0x04 */
    u32 color; /* 0x08 */
    u16 flags; /* 0x0C */
    u8 unk0E[2];
    u32 currentFrame; /* 0x10 */
    u32 triggerFrame; /* 0x14 */
    u32 colorRampEndFrame; /* 0x18 */
    u16 kind; /* 0x1C */
    u8 unk1E[2];
    s32 stat; /* 0x20 */
    effBTL_SkillCutTable_t *list; /* 0x24 */
    dds3EffectPart_t *parameterWorks[60]; /* 0x28 */
    effBTLCallback_t sourceFrameCallback; /* 0x118 */
    effBTLCallback_t triggerFrameCallback; /* 0x11C */
    void *userData; /* 0x120 */
    effBTLCut_t *next; /* 0x124 */
};


extern effBTL_t *effBTLCreate(void *pData);
extern void effBTLDelete(effBTL_t *hBTLEffect);
extern effBTLCut_t *effBTLCutCreate(effBTL_t *hBTLEffect, u16 CutNum, void *pFormation, s32 stat);
extern void effBTLCutDelete(effBTLCut_t *hBTLCutEffect);
extern void effBTLCutSetScale(effBTLCut_t *hBTLCutEffect, f32 scale);

#endif
