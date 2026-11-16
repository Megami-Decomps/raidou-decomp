#ifndef CODE_R1_0034_D128_H
#define CODE_R1_0034_D128_H

#include "common.h"
#include "code_0027A1D0.h"
#include "code_r1_001CC1D0.h"

/* The occupied-party display and its five native 0x34-byte entries. */
typedef struct mnuPartyPanelEntry_t {
    s32 unk0;
    s32 index;
    s32 unitIndex;
    s32 level;
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    s32 stats[DAT_BASE_STAT_COUNT];
} mnuPartyPanelEntry_t;

typedef struct mnuPartyPanel_t {
    s32 frontlineCount;
    s32 reserveCount;
    mnuPartyPanelEntry_t slots[5];
} mnuPartyPanel_t;

typedef struct mnuSprites_t {
    u32 flags;
    u8 pad04[8];
    s32 profileLabelOverride;
    void *icon[5];
    void *item[11];
    void *cursor[4];
    s32 fade;
    s32 drawAlpha;
    s32 fadeOut;
    s32 slideOffset;
    s32 slideSpeed;
    s8 menuValueStyleOverride;
    s8 currentValueStyle;
} mnuSprites_t;

/* The allocated icon bundle owns its draw fade and fade direction. */
typedef struct mnuIconBundle_t {
    u32 unk0[3];
    struct sprHandle_t *sprite[3];
    s32 fade;
    s32 fadeOut;
} mnuIconBundle_t;

typedef char mnuIconBundle_size_must_be_0x20[
    sizeof(mnuIconBundle_t) == 0x20 ? 1 : -1];

/* Complete 0x50-byte texture/effect owner embedded in each page bank. */
typedef struct mnuPageBar_t {
    s32 variant;
    u8 pad04[0x0C];
    s32 quantizedSpan; /* 0x10 */
    s32 *settings; /* 0x14: four selectable effect settings */
    u8 settingIndex;
    s8 positionY;
    u8 pad1A[2];
    struct sprHandle_t *textures[7]; /* 0x1C */
    struct effMappedResource_t *effects[2]; /* 0x38 */
    s32 activeEffect;
    s32 fade;
    s32 fadeOut;
    s32 holdEffectUpdate;
} mnuPageBar_t;

/* One command owns both complete 128-entry record arrays. The second
 * initializer explicitly writes all 128 entries, independent of table spacing. */
typedef struct mnuCommandPosition_t {
    s32 x;
    s32 y;
    s32 targetX;
    s32 targetY;
} mnuCommandPosition_t;

typedef struct mnuCommandMotion_t {
    s32 value; /* Phase step for kind 0, opacity factor for kind 1. */
    s32 delay;
    s32 phase;
    s32 scale;
} mnuCommandMotion_t;

typedef struct mnuQueuedCommand_t {
    u32 keepActive; /* Nonzero keeps the command alive after its phase is spent. */
    s32 kind;
    s32 option;
    s32 highPhaseDecayStep;
    s32 lowPhaseDecayStep;
    s32 initialValue;
    s32 argument;
    struct sprHandle_t *resources; /* 0x1C: the real draw resource instance. */
    s32 particleCount;
    mnuCommandPosition_t positions[128]; /* 0x24 */
    mnuCommandMotion_t motion[128];      /* 0x824 */
} mnuQueuedCommand_t;

/* One resource prefix followed by two complete command records. Resource
 * destructors visit this prefix once, while command routines use 0x1024 strides. */
typedef struct mnuPageSlot_t {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    struct sprHandle_t *icon[3]; /* 0x10 */
    mnuPageBar_t hp;               /* 0x1C */
    mnuPageBar_t mp;               /* 0x6C */
    struct sprHandle_t *frame[8]; /* 0xBC */
    struct mnuSprites_t *windowSprites; /* 0xDC */
    mnuIconBundle_t *iconBundle;
    struct sprHandle_t *panelResourceHandles[3]; /* 0xE4 */
    mnuQueuedCommand_t commands[2]; /* 0xF0 and 0x1114 */
} mnuPageSlot_t;

typedef char mnuCommandPosition_size_check[(sizeof(mnuCommandPosition_t) == 0x10) ? 1 : -1];

typedef char mnuCommandMotion_size_check[(sizeof(mnuCommandMotion_t) == 0x10) ? 1 : -1];

typedef char mnuQueuedCommand_size_check[(sizeof(mnuQueuedCommand_t) == 0x1024) ? 1 : -1];

typedef char mnuPageSlot_size_check[(sizeof(mnuPageSlot_t) == 0x2138) ? 1 : -1];

typedef struct mnuPageWindow_t {
    u32 flags;
    s32 transitionValue;
    mnuPartyPanel_t *records;
    struct sprHandle_t *resources; /* 0x0C: resolved base-resource handle */
    s32 slot;
    struct sprHandle_t *secondaryResource;
    s32 secondarySlot;
    struct sprHandle_t *alternateResource;
    s32 alternateSlot;
    /* One main resource bank, copied and consumed in two eight-entry halves. */
    struct sprHandle_t *mainResources[16];
    struct sprHandle_t *handlesC[5];
    mnuPageSlot_t slots[5];
    struct mnuList_t *lists[2];
    s32 selected;
    s32 scrollOffset;
    s32 fade;
} mnuPageWindow_t;

typedef char mnuPageWindow_size_must_be_0xA6A4[
    sizeof(mnuPageWindow_t) == 0xA6A4 ? 1 : -1];

typedef char MenuPageWindow_mainResources_extent_check[
    sizeof(((mnuPageWindow_t *)0)->mainResources) == 0x40 ? 1 : -1];

/* Signed row selector; -1 leaves the panel group without a selected row. */
typedef s32 MenuPanelGroupSelection;

/* Native group allocations own five panel items and retain their selection. */
typedef struct mnuPanelGroup_t {
    u8 pad00[0x0C];
    struct sprHandle_t *texture; /* 0x0C */
    struct mnuPanelItem_t *entries[5]; /* 0x10 */
    MenuPanelGroupSelection selection; /* 0x24; -1 means no selected row. */
    s32 initialValue; /* 0x28 */
} mnuPanelGroup_t;

typedef char mnuPanelGroup_size_must_be_0x2C[(sizeof(mnuPanelGroup_t) == 0x2C) ? 1 : -1];

void CmdMenuCursorDrawUnlock(void *pCmdMenu);

#endif
