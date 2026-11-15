#ifndef CODE_R1_0034_D128_H
#define CODE_R1_0034_D128_H

#include "common.h"
#include "code_0027A1D0.h"
#include "code_r1_001CC1D0.h"

/* The occupied-party display and its five native 0x34-byte entries. */
typedef struct PartyPanelEntry {
    s32 unk0;
    s32 index;
    s32 unitIndex;
    s32 level;
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    s32 stats[DAT_BASE_STAT_COUNT];
} PartyPanelEntry;

typedef struct PartyPanel {
    s32 frontlineCount;
    s32 reserveCount;
    PartyPanelEntry slots[5];
} PartyPanel;

/* DDS1 allocates 0x50 bytes; DDS2's expanded sprite banks and byte flags use 0x78. */
typedef struct MenuSprites {
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
} MenuSprites;

/* The allocated icon bundle owns its draw fade and fade direction. */
typedef struct MenuIconBundle {
    u32 unk0[3];
    struct EffectSlotSet *sprite[3];
    s32 fade;
    s32 fadeOut;
} MenuIconBundle;

typedef char MenuIconBundle_size_must_be_0x20[
    sizeof(MenuIconBundle) == 0x20 ? 1 : -1];

/* Complete 0x50-byte texture/effect owner embedded in each page bank. */
typedef struct MenuPageBar {
    s32 variant;
    u8 pad04[0x0C];
    s32 quantizedSpan; /* 0x10 */
    s32 *settings; /* 0x14: four selectable effect settings */
    u8 settingIndex;
    s8 positionY;
    u8 pad1A[2];
    struct EffectSlotSet *textures[7]; /* 0x1C */
    struct EffMappedResource *effects[2]; /* 0x38 */
    s32 activeEffect;
    s32 fade;
    s32 fadeOut;
    s32 holdEffectUpdate;
} MenuPageBar;

/* One command owns both complete 128-entry record arrays. The second
 * initializer explicitly writes all 128 entries, independent of table spacing. */
typedef struct MenuCommandPosition {
    s32 x;
    s32 y;
    s32 targetX;
    s32 targetY;
} MenuCommandPosition;

typedef struct MenuCommandMotion {
    s32 value; /* Phase step for kind 0, opacity factor for kind 1. */
    s32 delay;
    s32 phase;
    s32 scale;
} MenuCommandMotion;

typedef struct MenuQueuedCommand {
    u32 keepActive; /* Nonzero keeps the command alive after its phase is spent. */
    s32 kind;
    s32 option;
    s32 highPhaseDecayStep;
    s32 lowPhaseDecayStep;
    s32 initialValue;
    s32 argument;
    struct EffectSlotSet *resources; /* 0x1C: the real draw resource instance. */
    s32 particleCount;
    MenuCommandPosition positions[128]; /* 0x24 */
    MenuCommandMotion motion[128];      /* 0x824 */
} MenuQueuedCommand;

/* One resource prefix followed by two complete command records. Resource
 * destructors visit this prefix once, while command routines use 0x1024 strides. */
typedef struct MenuPageSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    struct EffectSlotSet *icon[3]; /* 0x10 */
    MenuPageBar hp;               /* 0x1C */
    MenuPageBar mp;               /* 0x6C */
    struct EffectSlotSet *frame[8]; /* 0xBC */
    struct MenuSprites *windowSprites; /* 0xDC */
    MenuIconBundle *iconBundle;
    struct EffectSlotSet *panelResourceHandles[3]; /* 0xE4 */
    MenuQueuedCommand commands[2]; /* 0xF0 and 0x1114 */
} MenuPageSlot;

typedef char MenuCommandPosition_size_check[(sizeof(MenuCommandPosition) == 0x10) ? 1 : -1];

typedef char MenuCommandMotion_size_check[(sizeof(MenuCommandMotion) == 0x10) ? 1 : -1];

typedef char MenuQueuedCommand_size_check[(sizeof(MenuQueuedCommand) == 0x1024) ? 1 : -1];

typedef char MenuPageSlot_size_check[(sizeof(MenuPageSlot) == 0x2138) ? 1 : -1];

typedef struct MenuPageWindow {
    u32 flags;
    s32 transitionValue;
    PartyPanel *records;
    struct EffectSlotSet *resources; /* 0x0C: resolved base-resource handle */
    s32 slot;
    struct EffectSlotSet *secondaryResource;
    s32 secondarySlot;
    struct EffectSlotSet *alternateResource;
    s32 alternateSlot;
    /* One main resource bank, copied and consumed in two eight-entry halves. */
    struct EffectSlotSet *mainResources[16];
    struct EffectSlotSet *handlesC[5];
    MenuPageSlot slots[5];
    struct MenuList *lists[2];
    s32 selected;
    s32 scrollOffset;
    s32 fade;
} MenuPageWindow;

typedef char MenuPageWindow_size_must_be_0xA6A4[
    sizeof(MenuPageWindow) == 0xA6A4 ? 1 : -1];

typedef char MenuPageWindow_mainResources_extent_check[
    sizeof(((MenuPageWindow *)0)->mainResources) == 0x40 ? 1 : -1];

/* Signed row selector; -1 leaves the panel group without a selected row. */
typedef s32 MenuPanelGroupSelection;

/* Native group allocations own five panel items and retain their selection. */
typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    struct EffectSlotSet *texture; /* 0x0C */
    struct MenuPanelItem *entries[5]; /* 0x10 */
    MenuPanelGroupSelection selection; /* 0x24; -1 means no selected row. */
    s32 initialValue; /* 0x28 */
} MenuPanelGroup;

typedef char MenuPanelGroup_size_must_be_0x2C[(sizeof(MenuPanelGroup) == 0x2C) ? 1 : -1];

#endif
