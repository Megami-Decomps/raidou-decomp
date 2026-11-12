#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_002B8FC8.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "newdata.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/fr_font.h"

#include "dds/mdl_flag_api.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdk_libc.h"

#include "dds/ui_color.h"

/* Return whether the selected flag bit is set, as EvtBitTestResult. */
EvtBitTestResult evtBitCheck(s32 flag);

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

void qsort(void *base, u32 count, u32 size, s32 (*compare)(const void *, const void *));

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

#include "dds/sdk_ps2.h"

#include "dds/fr_font.h"

#include "dds/evt_stage_test_api.h"

#include "dds/evt_stage_test_resource_work_result.h"

#include "dds/dsp_entry_api.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/ui_color.h"

#include "dds/fr_font_context.h"

#include "dds/sdf_chip.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/mnu_input.h"

#include "dds/kwln.h"

#include "dds/pty_skill_menu_api.h"

#include "dds/mnu_list.h"

#include "dds/sdf_heap_api.h"

#include "dds/mnu_staff.h"

#include "dds/fpu.h"

#include "dds/dat_state.h"

#include "dds/eff.h"

#include "dds/mnu_shop.h"

#include "dds/mnu_scroll_panel.h"

#include "dds/menu_fade_entry.h"

#include "dds/eff_random.h"

#include "dds/mdl_flag_api.h"

#include "dds/btl.h"

#include "dds/mnu_party_menu_data.h"

#include "dds/mnu_standard_partner_requirements_api.h"

struct StaffMenuRuntime;

extern void itfGridStorePosition(MenuGridSlot *, EffectSlotSet *, s32);

extern void mnuClearPageSelectionHandles(MenuPageWindow *);

#define MNU_ENTRY_SPRITE_COUNT 4

#define MNU_ENTRY_COLOR_COUNT 4

#define MNU_ENTRY_MARKED_COLOR 0x89BDC940

#define MNU_ENTRY_DEFAULT_COLOR 0x89BDC980

#define MNU_ENTRY_ALTERNATE_COLOR 0xBBEFAB80

#define MNU_NODE_FADE_STEP 0x10

#define MNU_FULL_FADE 0x100

#define MNU_PANEL_FADE_LIMIT 0x200

#define MNU_PANEL_FADE_THRESHOLD 0x101

#define MNU_WINDOW_FADE_STEP 0x20

#define MNU_WINDOW_TRANSITION_FLAG 4

#define MNU_WINDOW_TRANSITION_CLEAR_MASK 0xFFFFFFFB

#define MNU_LIST_SELECTION_FLAG 8

#define MNU_LIST_ALTERNATE_SELECTION_FLAG 0x10

#define MNU_WINDOW_CONTAINER_BYTES 0x98

#define MNU_PANEL_LAYOUT_BYTES 0x38

#define MNU_WINDOW_RESOURCE_SPRITES 3

#define MNU_PANEL_KIND_LIMIT 6

#define MNU_SORT_KEY_COUNT 3

#define MNU_SORT_COMPARATOR_COUNT 6

#define MNU_LIST_POINTER_BYTES 4

#define MNU_FADING_SLOT_COUNT 4

#define MNU_FADING_SLOT_BYTES 0x18

#define MNU_FADING_MUTATION_THRESHOLD 5

#define MNU_FADING_WORD_STEP 0x40

#define MNU_STAFF_PARTY_SLOT_COUNT 5

#define MNU_STAFF_PARTY_LAST_SLOT 4

#define MNU_STAFF_DISPLAY_OVERFLOW 4

#define MNU_STAFF_DISPLAY_LIMIT 3

#define MNU_STAFF_PARTY_ENTRY_BYTES 0x1C4

#define MNU_STAFF_PARTY_HALFWORD_STRIDE 0xE2

#define MNU_STAFF_PARTY_BASE 0xA60

#define MNU_STAFF_BACKUP_BYTES 0x8D4

#define MNU_STAFF_PARTY_ACTIVE_BIT 1

#define MNU_STAFF_NODE_UNAVAILABLE 1

#define MNU_STAFF_NODE_SELECTED 2

#define MNU_STAFF_PARTY_PANEL_BASE 0x284

#define MNU_STAFF_PARTY_SLOTS_BASE 0xA928

#define MNU_STAFF_FADE_STEP 0x10

#define MNU_STAFF_FADE_CLOSE_THRESHOLD 0x50

#define MNU_STAFF_FADE_OPEN_THRESHOLD 0xB0

#define MNU_STAFF_INPUT_CONFIRM 1

#define MNU_STAFF_INPUT_CANCEL 2

#define MNU_STAFF_POPUP_INPUT_MASK 3

#define MNU_STAFF_VIEW_INPUT_MASK 0xC2

#define MNU_STAFF_VIEW_TOGGLE_MASK 0xC0

#define MNU_STAFF_PAGE_INPUT_MASK 0x300

#define MNU_STAFF_INPUT_PREV_PAGE 0x100

#define MNU_STAFF_INPUT_NEXT_PAGE 0x200

#define MNU_STAFF_INPUT_PREVIOUS 0x10

#define MNU_STAFF_INPUT_NEXT 0x20

#define MNU_STAFF_INPUT_NAV_STATE_MASK 0x300000

#define MNU_STAFF_INPUT_REJECTED 0x8000

#define MNU_STAFF_SKILL_INPUT_MASK 0x33

#define MNU_STAFF_REORDER_INPUT_MASK 0x37

#define MNU_STAFF_REORDER_CANCEL_MASK 6

typedef struct MenuList MenuList;

typedef struct MenuIconState MenuIconState;

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);

extern s32 itfGridLookupValueOrDefault(EffectSlotSet *object, s32 key);

extern u8 *D_00435E5C;

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

extern EvtStageTestResourceWorkResult evtStageTestAdvanceResourceWork(void);

extern char mnuStaffCampPagePopupEntry[];

extern void mnuDrawWindowDecorations(s32, s32, s32, MenuWindowContainer *, s32);

extern void mnuReleasePartyIconBundles(MenuPageWindow *);

extern void mnuClearEntries(MenuPageWindow *);

extern u8 *D_00435E54;

extern u8 *datUnitNames;

extern void mnuDrawSlotIcons(s32 x, MenuPageWindow *page);

extern void frEnableMode();

extern void frDisableMode();

extern char mnuCampPartySelectionReturnPopupEntry[];

extern struct sdfGrOt_t *kwlnSceneSurfaceSets[13][4];

extern s8 evtStageTestUpdate(struct sdfGrOt_t **);

extern char mnuSkillMenuDetailPopupEntry[];

extern char mnuCampFieldSkillPopupEntry[];

extern char mnuSkillMenuFieldUsePopupEntry[];

extern char mnuSkillMenuEquippedSlotsPopupEntry[];

extern char mnuSkillMenuPagePopupEntry[];

extern s32 ptySkillMenuApplyFieldUseAndCost();

extern void mnuPlayInputSound(s32, s32, u32 *);

extern u32 mnuCampBackdropFrameMap[];

extern const CampEffectRows mnuCampBackdropEffectRows;

extern char *mnuCampBackdropSpriteResourceNames[];

extern char *mnuCampBackdropAnimationResourceNames[];

extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern void mnuFreeWindowSprites();

extern s8 evtGetCapturedWindowPanelValue(void);

extern s32 mnuApplyRandomStatItemEffect();

extern void mnuPrepareStaffValueChangeDialog(MenuStaffContext *, dds_datUnitWork_t *, s32, s32);

extern char mnuCampRingCustomizationPopupEntry[];

extern void mnuDrawCampGridAndLabel();

extern u8 *datSkillMessages;

extern void mnuDrawCampIconBackdropByKind();

extern void mnuDrawCampGridAndLabelDefault();

extern char mnuCampCommandHelpTextBank[];

extern void mnuIdleVoiceTimer(struct StaffMenuRuntime *object);

extern void mnuDrawPartySelectionPreview();

extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);

extern void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *);

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

extern char mnuStaffPartyBrowsePopupEntry[];

extern char mnuStaffFieldSkillDisplayPopupEntry[];

extern void mnuSelectPage(MenuPageWindow *window, s32 selected);

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[0x54];
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    s32 displayHandle;     /* 0x60 */
    s32 resourceHandle;    /* 0x64 */
    void *displayResource; /* 0x68 */
    s32 alternateResource; /* 0x6C: used when swapping the staff panel view */
    u8 pad70[0x58];
    s32 labelHandle;       /* 0xC8 */
    u8 padCC[0x38];
    s32 imageHandle;       /* 0x104 */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    s32 partyPanelActive;  /* 0xA928 */
    s32 partyPanelLast;    /* 0xA92C */
    u8 padA930[0x104];
    MenuPanelGroup *panelGroup; /* 0xAA34 */
    s32 panelRequest;      /* 0xAA38 */
    MenuSpriteState *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    s32 resourceList;      /* 0xAA5C */
} MenuContext;

typedef struct MenuListNode MenuListNode;

#define PARTY_BACKUP_OFFSET ((s32)&((PartyMenuData *)0)->backup)

/* Staff/skill-menu work prefix, separate from the party-selection allocation. */
typedef struct MenuPartyRuntime {
    u8 pad00[0x10];
    s32 staffMode; /* 0x10 */
    s32 staffView; /* 0x14: selects the input mask and panel view */
    s32 staffSelection; /* 0x18: chosen list entry */
    s32 staffExit; /* 0x1C: signals the close transition */
    u8 pad20[4];
    MenuWindowContainer *selectedWindow; /* 0x24 */
    s32 activeMark; /* 0x28 */
    u8 pad2C[8];
    u32 selectedIndex; /* 0x34: compared with MenuListNode.index */
} MenuPartyRuntime;

/* The idle-motion timer shares the party work area with other menu states. */
typedef struct MenuVoiceState {
    u8 pad00[0x28];
    s32 idleFrames;
    s32 motionSelection; /* -1 enables periodic motion */
} MenuVoiceState;

extern void ptyRecomputeMaxHpMp(dds_datUnitWork_t *unit);

extern void scrClearSecondaryScriptFlag(dds_datUnitWork_t *unit, u16 flagId);

#include "dds/fr_font_draw_glyph.h"

extern void dspRequestMessageWindowModeOne(void);

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

extern MenuListDefaults D_0042AF00;

extern void mnuDrawStaffPartySelectionPanel(s32);

s32 mnuAdvanceStaffValuePopup(s32 callback);

s32 mnuFinishStaffValuePopup(s32 callback);

/* Initialize the menu display using a value from the resource chain. */
u32 mnuEnterSelectedResourceLabel(dds3ProcessID_t *task);

u32 mnuPartySlotAssignmentLeaveNoOp(void);

s32 mnuUpdatePartySlotAssignmentPopup(s32 callback);

s32 mnuStartPanelDispatch(s32 callback);

s32 mnuStartPanelExit(s32 callback);

/* Clear the selected item's five stat bonuses and its requirement count,
 * then recalculate the party record's maximum HP and MP. */
void mnuClearPartySelectionValues(dds_datUnitWork_t *entry, s32 selection);

u32 mnuEnterSlotLabel(dds3ProcessID_t *task);

u32 mnuPartySlotClearLeaveNoOp(void);

s32 mnuPartySlotConfirmClearUpdate(s32 callback);

s32 mnuAdvancePartyClearPopup(s32 callback);

s32 mnuFinishPartyClearPopup(s32 callback);

u32 mnuAllowPartySelectionReturn(void);

void mnuSelectPartySelectionVisuals(u32 context);

void mnuNoOpPartySelectionCleanup(u32 callback);

s32 mnuIsFinalItemIndex(s32 index, const struct MenuList *list);

void mnuDestroyPartySelectionWindow(MenuContext *context);

/* Snapshot the five party entries and cap the menu's displayed slot count. */
void mnuCopyPartyEntries(MenuContext *context);

extern void mnuAssignSelectedPartyEntry();

/* Notify active snapshot entries, restore the backup, then refresh panel resources.
 * The second loop counts down while the backup byte offset advances forward. */
void mnuRestorePartyEntriesAndRefresh(MenuContext *context);

/* Count active entries in the five-slot party array, capped at three. */
s32 mnuCountActiveSlots(void);

/* Reset selection/backup state, activate all panel slots, and enable list nodes.
 * Keep the native short-arity snapshot call unchanged. */
void mnuClearPartySelectionAndActivateSlots(MenuContext *context);

/* Release panel textures before reinitializing slots and updating handle state. */
void mnuRefreshPartyPanelSlots(MenuContext *context);

u32 mnuReleasePartySelectionResources(dds3ProcessID_t *task);

void mnuPreparePartyPanelTransition(MenuContext *menu);

/* Two-stage fade: B rises first when opening, A falls first when closing.
 * The second ramp begins strictly past its threshold; clamps follow each step. */
void mnuUpdateStaffFade(s32 opening, PartyMenuData *menuWork);

/* Update the final-row flag, draw the panel, and suppress its contents update
 * once the party transition has frozen it. */
s32 mnuOpenStaffPartySelectionPanel(s32 callback);

s32 mnuStepPartySelectionControl(s32 callback);

u8 mnuIsStateNotOne(void);

void mnuSelectStaffSelectionVisuals(u32 context);

void mnuReleaseMenuWindowHandles(s32 context);

u32 mnuCreateSelectState(u32 unused, s32 staffView);

s32 mnuStaffCloseSelectionState(dds3ProcessID_t *task);

u32 mnuOpenStaffSelectionEntry(u32 context);

s32 mnuFinishStaffSelectionEntry(dds3ProcessID_t *task);

u32 mnuOpenPartySelectionEntry(u32 context);

s32 mnuFinishPartySelectionEntry(dds3ProcessID_t *task);

/* Update the popup first; accept confirm/cancel only while its state is zero.
 * Return a nonzero popup-update word unchanged, or zero after handling input. */
s32 mnuStaffPopupUpdate(s32 callback);

s32 mnuDrawStaffCampPageWithImage(s32 callback);

s32 mnuStepStaffCampPageControl(s32 callback);

extern MenuProfilePanel *mnuCreateProfilePanel(dds_datUnitWork_t *selectionState);

extern void mnuConfigurePartyPanelPage(s32, s32, s32, s32);

s32 mnuCreatePanels(s32 callback);

s32 mnuDestroyPanels(s32 callback);

void mnuResetSelectedPanelOpacity(s32 context);

/* Switch the party page, rebuilding its panels; previous takes priority.
 * The opaque request argument is forwarded unchanged. Return one if switched. */
s32 mnuStaffSwitchPartyPage(s32 requestArgument);

/* Update popup state before page navigation, view toggles, and exit requests.
 * stateWord holds the update result first, then the stored popup state. */
s32 mnuStaffBrowsePartyUpdate(s32 callback);

void mnuDrawSlotIcons(s32 x, MenuPageWindow *page);

void mnuDrawSelectedPartySlotMarkers(s32 context, StaffSlots *resources);

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, const char *text, s32 flags);

void mnuDrawPartySkillAndStatusPanel(dds_datUnitWork_t *entry, MenuPageWindow *page, MenuPanelGroup *packedGroup, MenuSpriteState *spriteState, s32 unused, s32 spriteFlags);

void mnuDrawProfilePanelAndSprite(dds_datUnitWork_t *entry, u32 unused1, MenuSpriteState *spriteState,
                                    MenuProfilePanel *resource,
                                    u32 unused4, u32 spriteFlags);

void mnuIdleVoiceTimer(struct StaffMenuRuntime *object);

s32 mnuStaffIdlePartyUpdate(s32 callback);

u32 mnuAllowSkillMenuReturn(void);

u32 mnuDestroySelectedPartyWindow(u32 callback);

/* Seek the first node with a nonzero sort key and no unavailable flag.
 * Leave the cursor unchanged if no such node exists. */
void mnuSeekFirstAvailableStaffListNode(dds3ProcessID_t *task);

MenuWindowContainer *mnuSeekSelectedWindowCursor(s32 selectionMode, s32 callback);

void mnuSelectSkillSelectionVisuals(u32 context);

void mnuNoOpItemSelectionTeardown(s32 context);

u32 *mnuBuildOwnedSkillBits(void);

void mnuReleaseSkillCodeBitset(u32 *bits);

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits);

void mnuDestroySkillMenuWindows(s32 context);

u32 mnuCreateItemState(s32 callback);

s32 mnuCloseItemSelectionState(s32 selection);

s32 mnuCampMenuDrawSlotLabel(s32 callback);

s32 mnuStepSkillSlotControl(s32 callback);

/* Set the selected flag only on nodes whose index matches the saved selection. */
void mnuHighlightSelectedListNode();

u32 mnuClearSkillSelectionImageState(dds3ProcessID_t *task);

/* Narrow the ID to its native 16-bit skill code before duplicate detection.
 * Insert only missing skills, then recompute maxima and clear the script flag. */
void mnuAddPartySkillIfMissing(dds_datUnitWork_t *partyEntry, s32 skillId, s32 skillSlot);

/* Clear one skill slot, retaining the native short-arity maxima recomputation. */
void mnuClearPartySkillSlot(dds_datUnitWork_t *partyEntry, s32 skillSlot);

/* Open the selected skill's popup or cancel, then process list navigation.
 * Native list reads precede the late window guard; preserve that ordering. */
void mnuCampMenuHandleInput(s32 callback);

void mnuSwapPartySkillSlots(dds_datUnitWork_t *party, s32 firstSlot, s32 secondSlot);

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
typedef struct MenuPanelBlock {
    u32 word[14];
} MenuPanelBlock;

typedef struct MenuPanelWindow {
    u8 pad0[4];
    s32 field4;
    u8 pad8[0x10];
    MenuList *list; /* 0x18 */
    u8 pad1C[0x3C];
    MenuPanelBlock block; /* 0x58 */
    u8 pad90[4];
    s32 field94;
} MenuPanelWindow;

void ptySkillMenuCopyPageState(s32 context);

s32 ptySkillMenuEnterPage(s32 callback);

s32 ptySkillMenuDispatchPageRequest(s32 callback);

s32 ptySkillMenuApplyFieldUseAndCost();

/* Mark entries whose field-use cost cannot be paid by the selected party member. */
void mnuFlagMatchingEntries(s32 context);

s32 ptySkillMenuHandleFieldUse(s32 callback);

s32 ptySkillMenuOpenSelectedSkillPage(s32 callback);

s32 ptySkillMenuDispatchConfirmRequest(s32 callback);

u32 ptySkillMenuClosePartyPage(u32 callback);

s32 ptySkillMenuHandlePageSwitch(s32 callback);

/* Shift a selected child window's list to the requested row. Keep the
 * double-dereferenced cursor load raw: the typed form does not match. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target);

s32 ptySkillMenuBrowseCandidatePages(s32 callback);

s32 mnuOpenSkillDetailPanel(s32 callback);

s32 mnuStepSkillDetailControl(s32 callback);

void mnuDrawSelectionLabel(u16 id);

/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(const dds_datUnitWork_t **out, s32 max);

u32 mnuDestroySkillSelectionWindow(u32 callback);

u32 mnuResetSelection(u32 callback);

u32 mnuCloseSkillSelection(u32 callback);

s32 mnuUpdateSkillListInput(s32 callback);

void mnuDrawListFrames(s32 context);

s32 mnuCampMenuDrawStatus(s32 callback);

s32 mnuStepCampStatusControl(s32 callback);

void mnuInitializeMapPacket(u32 value, u32 *values, s32 count, MapPacket *packet);

void mnuOrEntryFlags(u32 flags, u32 *entryFlags);

void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);

void mnuBindCampEffectAnimation(MenuEffectResources *);

void mnuLoadEffectResources(MenuEffectResources *);

void mnuRequestEffectResources(MenuEffectResources *);

u32 mnuBindCampEffectWhenLoaded(MenuEffectResources *);

void mnuDestroyEffectResources(MenuEffectResources *);

void mnuSpawnSpark(MenuCampEffect *fx);

void mnuRetireCampSpark(MenuCampEffect *, s32 index);

void mnuDrawAndAdvanceCampSparks(MenuCampEffect *fx, s32 surface);

typedef struct MenuBadgePlace {
    s32 slot;
    s32 x;
    s32 y;
} MenuBadgePlace;

typedef struct MenuBadgeLayout {
    MenuBadgePlace place[2];
} MenuBadgeLayout;

extern MenuBadgeLayout D_0042AED0;

#define MNU_ADVANCE_FADE(value, amount, maximum) { \
    if ((value) < (maximum)) { \
        (value) += (amount); \
    } \
    if ((value) > (maximum)) { \
        (value) = (maximum); \
    } \
}

void mnuDrawBadgeFade(MenuCampEffect *set, s32 surface);

extern MenuBadgeLayout D_0042AEE8;

void mnuDrawCampIconBackdrop(MenuCampEffect *set, s32 surface);

void mnuEnableCampBadgeFade(u32 *flags);

MenuList *mnuCreateListState(u32 owner, u32 visibleCount, s32 rowSpacing);

void mnuUpdateListScrollFlags(MenuList *list);

s32 mnuListContainsFinalNode(MenuList *list);

void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect);

void mnuSelectFirstListNode(MenuList *list);

void mnuSelectLastListNode(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

u32 mnuTestListFlagTwo(u32 *flags);

/* Return the stored row step times the visible row count, in native units. */
s32 mnuGetListViewportHeight(const MenuList *list);

/* Cancel the pending animation on every node in this list. */
void mnuResetListNodeFadeCounters(MenuList *list);

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(MenuList *list);

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg);

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry);

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 entryIndex, MenuListNode *entry);

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

extern const char D_00437C28[];

/* Draw visible rows, preserving each row's glyph chain and callback state. */
void mnuDrawWindowListEntries(s32 x, s32 y, s32 depth, s32 xOffset, s32 yOffset,
                   s32 fadeScale, u32 unusedFlags, MenuList *list, s32 drawArg);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
MenuWindowContainer *mnuCreateWindowContainer(s32 id, s32 width, s32 height,
                                                s32 visibleCount, s32 rowSpacing);

/* Destroy the owned list and optional sprite resources before freeing the window. */
void mnuDestroyWindowContainer(MenuWindowContainer *menu);

void mnuSetWindowOverlaySprite(MenuWindowContainer *menu, struct EffectSlotSet *sprite);

void mnuSetWindowFadeScale(MenuWindowContainer *menu, u32 fadeScale);

void mnuSetWindowContainerLayout(MenuWindowContainer *menu, struct EffectSlotSet *layout2C, u32 layout30, struct EffectSlotSet *layout34,
                                    u32 layout48, u32 layout38, struct EffectSlotSet *layout3C, u32 layout40,
                                    u32 layout4C);

void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu, struct EffectSlotSet *first, u32 second);

void mnuSetWindowEntryParameters(u32 first, MenuWindowContainer *menu, EffectSlotSet *spriteResource, u32 third, u32 fourth);

/* Copy the native panel layout, override its bounds, and mark its transition flag. */
void mnuSetWindowPanelBounds(MenuWindowContainer *panel, const struct MenuIconState *layout,
                             u32 left, u32 top,
                   u32 right, u32 bottom);

void mnuCreateListWithDefaults(MenuWindowContainer *menu, u32 first, u32 second,
                               u32 third, EffectSlotSet *spriteResource);

/* Clear only the window's panel-transition bit. */
void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *window);

MenuListNode *mnuAppendWindowListNode(MenuWindowContainer *menu, const void *text);

/* Insert `value` beside anchor in the window's list (see mnuInsertListNodeRelativeToAnchor). */
MenuListNode *mnuInsertWindowListNodeRelativeToAnchor(MenuWindowContainer *menu, MenuListNode *anchor, const void *text, s32 mode, u32 options);

void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu);

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step);

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step);

MenuListNode *mnuAdvanceWindowListSelection(MenuWindowContainer *menu);

MenuListNode *mnuRetreatWindowListSelection(MenuWindowContainer *menu);

void mnuClearWindowListSelectionFlags(MenuWindowContainer *menu);

void mnuCheckWindowListStatus(MenuWindowContainer *menu);

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value,
                        EffectSlotSet *resource, s32 *idx, s32 unused);

/* Create an owned three-sprite bundle using the caller's slot-index array. */
MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    EffectSlotSet *resourceHandle, s32 *indices, u32 unused);

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(MenuIconSprites *menu);

void func_002B9A38(void);

void mnuDrawListScrollbar(s32 x, s32 y, s32 depth, u32 opacity, MenuList *list,
                   MenuIconSprites *resource, s32 drawArg);

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option);

void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, s32 count, s32 option);

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option);

/* Draw the three optional decoration sprites at their individual X offsets. */
void mnuDrawWindowDecorations(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 option);

/* Draw at the selected row before advancing the panel's transition value.
 * DDS2's alternate selection flag overrides its ordinary selection flag. */
void mnuDrawWindowSelectionPanel(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 drawArg);

/* Draw the window, then advance its fade scale without a post-addition clamp. */
void mnuDrawWindowContainer(s32 x, s32 y, s32 depth, MenuWindowContainer *menu, s32 drawArg);

void mnuSpinForWindowListCount(MenuWindowContainer *window);

MenuIconState *mnuCreatePanelIconState(u32 mode, s32 resource, s32 material);

/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuClearEntryFlags(MenuIconState *group);

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(MenuIconState *list);

typedef struct MenuPos {
    s32 x;
    s32 y;
} MenuPos;

typedef struct MenuPosTable {
    MenuPos pos[6];
} MenuPosTable;

typedef struct MenuPosTable3 {
    MenuPos pos[3];
} MenuPosTable3;

extern const MenuPosTable3 D_0042AF48;

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 surface);

extern const MenuPosTable D_0042AF60;

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface);

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

extern const MenuOffsets D_0042AF90;

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface);

extern void mnuDrawIconPanelFade();

extern void mnuDrawIconRow6();

extern void mnuDrawIconPair();

/* Dispatch six DDS2 panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg);

void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg);

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg);

void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *obj);

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(MenuList *list);

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list);

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(MenuList *list, s32 restoreCursor);

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(MenuListNode **nodeArray, s32 count);

/* Three-way comparison of unsigned primary keys, descending without subtraction. */
s32 mnuComparePrimaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned primary keys, ascending without subtraction. */
s32 mnuComparePrimaryKeyAscending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned secondary keys, descending without subtraction. */
s32 mnuCompareSecondaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned secondary keys, ascending without subtraction. */
s32 mnuCompareSecondaryKeyAscending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned tertiary keys, descending without subtraction. */
s32 mnuCompareTertiaryKeyDescending(MenuListNode **left, MenuListNode **right);

/* Three-way comparison of unsigned tertiary keys, ascending without subtraction. */
s32 mnuCompareTertiaryKeyAscending(MenuListNode **left, MenuListNode **right);

/* Sort the walked node pointers and rebuild the list from the cursor.
 * Nonzero ascending selects the last three comparators, not descending order.
 * Allocation uses the stored count; key bounds and the relinker's minimum count remain unchecked. */
extern const MenuPosTable3 D_0042AF48;

extern const MenuPosTable D_0042AF60;

extern const MenuOffsets D_0042AF90;

void mnuSortItems(MenuList *menu, s32 keyIndex, s32 ascending);

/* Allocate four native fade records into pointer slots after the list header. */
void mnuAllocateListEntries(s32 *list);

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list);

/* Store the active word and window address in the header-selected slot.
 * Preserve the native below-five early return; it is not an upper capacity bound. */
void mnuAppendFadingWindowEntry(s32 activeValue, s32 windowAddress, s32 *list);

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex);

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg);

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list);

/* Reset the original 0x98-byte prefix, then initialize the later fade fields. */
void mnuInitializeWindowFadeState(MenuFadeFields *menu);

/* Snapshot the outgoing window and its resources before starting the next fade. */
void mnuBeginWindowFadeTransition(MenuWindowContainer *windowAddress, MenuFadeFields *work);

void mnuResetWindowFadeParameters(MenuFadeFields *menu);

void mnuUpdateAndDrawWindowTransition(s32 x, s32 y, s32 depth, MenuFadeFields *menu, s32 option);

void mnuInitScrollHandles(MenuScrollPanel *menu);

void mnuReleaseScrollPanelAnimations();

MenuScrollPanel *mnuCreateScrollPanel(struct EffectSlotSet *owner);

void mnuDestroyScrollPanel(MenuScrollPanel *menu);

void mnuActivatePendingPanelResource(MenuScrollPanel *context);

void mnuConfigurePanelResource(MenuScrollPanel *menu, struct EffectSlotSet *model,
                               u32 resourceIndex, u32 retract);

u8 mnuHasActivePanelResource(MenuScrollPanel *resources);

void mnuLoadPanelSectionResources(MenuPageSlot *slot,
                                  EffectSlotSet *model, u32 firstValue,
                                  u32 secondValue, s32 thirdValue);

void mnuReleasePartyPanelTextures(MenuPageWindow *window);

void mnuResetPartyPanelFade(MenuPageWindow *window, s32 slotIndex, u32 unused, u32 preserve);

void mnuSetMenuPageFlags(MenuSprites *page, u32 flags);

void mnuSetPageParams(MenuSprites *page, s32 pageTransitionMode);

typedef struct MenuSpritePlacement {
    EffectSlotSet *source;
    u32 index;
    s32 x;
    s32 y;
} MenuSpritePlacement;

typedef struct MenuGatedSpritePlacement {
    EffectSlotSet *source;
    u32 index;
    s32 x;
    s32 y;
    s8 requiresFlag;
} MenuGatedSpritePlacement;

MenuSprites *mnuCreatePageSpriteSet(s32 kind, EffectSlotSet *mainResource,
                                    EffectSlotSet *itemResource,
                                    EffectSlotSet *iconResource,
                                    EffectSlotSet *cursorResource,
                                    EffectSlotSet *alternateResource);

void mnuFreeIconSprites(MenuSprites *menu);

void mnuDrawIconRow(s32 unusedA, s32 unusedB, s32 depth, s32 skip, MenuSprites *set, s32 drawArg);

void mnuSetWindowResource(s32 pageIndex, MenuPageWindow *menu,
                          EffectSlotSet *mainResource,
                          EffectSlotSet *itemResource,
                          EffectSlotSet *iconResource,
                          EffectSlotSet *cursorResource,
                          EffectSlotSet *alternateResource);

void mnuSetIndexedWindowPageSpriteFlags(s32 index, MenuPageWindow *menu, u32 first, u32 second);

void mnuClearEntries(MenuPageWindow *menu);

typedef struct MenuIconEntry {
    u32 id;
    s32 x;
    s32 y;
} MenuIconEntry;

typedef struct MenuIconLayout {
    MenuIconEntry entry[3];
} MenuIconLayout;

extern const MenuIconLayout D_0042AFD8;

MenuIconBundle *mnuCreateIconBundle(EffectSlotSet *resource);

void mnuReleaseIconBundleAndSprites(MenuIconBundle *menu);

void mnuDrawFadeIcons(s32 x, s32 y, s32 depth, s32 unused, MenuIconBundle *obj, s32 drawArg);

void mnuAttachPartyIconBundle(s32 slotIndex, MenuPageWindow *menu, EffectSlotSet *resource);

void mnuReleasePartyIconBundles(MenuPageWindow *menu);

s32 mnuPercentOrHundred(s32 currentAmount, s32 maximumAmount);

typedef struct MenuPanelSequenceSettings {
    s32 rows[5][4];
    const char *captions[2];
} MenuPanelSequenceSettings;

extern MenuPanelSequenceSettings D_003E7878;

extern void mnuDrawPanelSequenceByRow(MenuPageBar *, s32, s32 *, s32, s32, EffectSlotSet *);

void mnuDrawPartyVitalPanelSequences(MenuPageWindow *window, EffectSlotSet *texture);

void mnuReleasePartyPanelSpriteTextures(MenuPageWindow *window);

/* Copy eight resource handles into the window's primary handle bank. */
void mnuCopyPrimaryWindowHandles(MenuPageWindow *menu, struct EffectSlotSet **source);

/* Copy eight resource handles into the window's secondary handle bank. */
void mnuCopySecondaryWindowHandles(MenuPageWindow *menu, struct EffectSlotSet **source);

void mnuRegisterResourceHandles(MenuPageWindow *destination, struct EffectSlotSet **source);

void mnuRebuildPartySlotResources(MenuPageWindow *menu, s32 partySlotIndex, s32 kind);

void mnuRefreshWindowSlots(MenuPageWindow *menu, s32 preserveResourceHandles);

void mnuReleaseAndRefreshWindowSlots(MenuPageWindow *menu);

extern void mnuUpdatePairedEffectBounds(MenuPageBar *bar);

void mnuRefreshPartyPanelBars(MenuPageWindow *page);

extern char D_00437C30[];

/* Create both party-panel lists and append their shared row labels. */
void mnuInitScrollLists(MenuPageWindow *menu, PartyPanel *records);

void mnuDestroyWindowOwnedLists();

void mnuRebuildScrollLists(MenuPageWindow *context, PartyPanel *records);

void mnuClearPageSelection(MenuPageWindow *menu);

void mnuInitPageWindow(MenuPageWindow *window, PartyPanel *records,
                       EffectSlotSet *resources, s32 slot,
                       EffectSlotSet *secondaryResource, s32 secondarySlot,
                       EffectSlotSet *alternateResource, s32 alternateSlot);

void mnuShutdownContext(MenuPageWindow *window);

void mnuResolveUnselectedPageHandles(MenuPageWindow *window);

void mnuRefreshPageHandles(MenuPageWindow *window);

void mnuSelectPage(MenuPageWindow *window, s32 selected);

void mnuClearPageSelectionHandles(MenuPageWindow *window);

void mnuFlagActiveWindows(MenuPageWindow *window);

void mnuClearPartyPanelActiveFlags(MenuPageWindow *window);

void mnuClearActionFlags(s32 kind, MenuPageWindow *page);

void mnuStepPartyPanelListFromInput(s32 mode, MenuPageWindow *window);

extern const MenuIconLayout D_0042AFD8;
/* End of declarations from src/dds2/game/code_002B8FC8.c */

/* Subtract the fade step only when positive, then clamp any negative result to zero. */
void mnuDecreaseListNodeFadeCounters(MenuList *list) {
    MenuListNode *node = list->first;
    if (node != NULL) {
        do {
            s32 timer = node->animationTimer;
            s32 reduced = timer - MNU_NODE_FADE_STEP;
            if (timer > 0) {
                node->animationTimer = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                node->animationTimer = 0;
            }
            node = node->next;
        } while (node != NULL);
    }
}

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg) {
    u32 spriteIndex = 0;
    do {
        s32 selected = node == list->cursor;
        s32 index = selected * MNU_ENTRY_SPRITE_COUNT + spriteIndex;
        struct EffectSlotSet *sprite = node->sprites[index].sprite;
        if (sprite != 0) {
            itfDrawGridWithResolvedSlot(x, y, depth, 0, sprite, node->sprites[index].effect, drawArg);
        }
        spriteIndex++;
    } while (spriteIndex < MNU_ENTRY_SPRITE_COUNT);
}

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry) {
    u32 flags = entry->flags48;
    u32 color = MNU_ENTRY_MARKED_COLOR;
    if (!(flags & 1)) {
        color = (flags & 4) ? MNU_ENTRY_ALTERNATE_COLOR : MNU_ENTRY_DEFAULT_COLOR;
    }
    return fclBlendColor(color, previousColor, entry->animationTimer);
}

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 entryIndex, MenuListNode *entry) {
    s32 colorIndex;

    for (colorIndex = 0; colorIndex < MNU_ENTRY_COLOR_COUNT; colorIndex++) {
        u32 previousColor = menu->workEntries[entryIndex].geometry.cornerColors[colorIndex];

        menu->workEntries[entryIndex].geometry.cornerColors[colorIndex] = mnuBlendListNodeColorByFlags(previousColor, entry);
    }
}

INCLUDE_ASM(const s32, "code_003BD050", mnuDrawWindowListEntries);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg) {
    mnuDrawWindowListEntries(x, y, depth, 0, 0, MNU_FULL_FADE, 0, (MenuList *)(u32)menu, drawArg);
}

INCLUDE_ASM(const s32, "code_003BD050", mnuCreateWindowContainer);

INCLUDE_ASM(const s32, "code_003BD050", mnuDestroyWindowContainer);

INCLUDE_ASM(const s32, "code_003BD050", func_003BD5A8);

INCLUDE_ASM(const s32, "code_003BD050", func_003BD5B0);

void mnuSetWindowContainerLayout(MenuWindowContainer *menu, struct EffectSlotSet *layout2C, u32 layout30, struct EffectSlotSet *layout34,
                                    u32 layout48, u32 layout38, struct EffectSlotSet *layout3C, u32 layout40,
                                    u32 layout4C) {
    menu->decorations[0].sprite = layout2C;
    menu->decorationX[2] = layout4C;
    menu->decorations[0].parameter = layout30;
    menu->decorations[1].sprite = layout34;
    menu->decorations[1].parameter = layout38;
    menu->decorationX[1] = layout48;
    menu->decorations[2].sprite = layout3C;
    menu->decorations[2].parameter = layout40;
    menu->decorationX[0] = 0;
}

void mnuInitializeBasicWindowLayout(MenuWindowContainer *menu, struct EffectSlotSet *first, u32 second) {
    mnuSetWindowContainerLayout(menu, first, second, 0, 0, 0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "code_003BD050", func_003BD618);

INCLUDE_ASM(const s32, "code_003BD050", mnuSetWindowPanelBounds);

void mnuCreateListWithDefaults(MenuWindowContainer *menu, u32 first, u32 second,
                               u32 third, EffectSlotSet *spriteResource) {
    MenuListDefaults defaults = D_0042AF00;
    menu->resource =
        mnuCreateWindowSpriteResources(first, second, third, spriteResource,
                                       defaults.indices, 3);
}

INCLUDE_ASM(const s32, "code_003BD050", func_003BD720);

MenuListNode *mnuAppendWindowListNode(MenuWindowContainer *menu, const void *text) {
    return mnuListAppendNode(menu->list, text);
}

/* Insert `value` beside anchor in the window's list (see mnuInsertListNodeRelativeToAnchor). */
MenuListNode *mnuInsertWindowListNodeRelativeToAnchor(MenuWindowContainer *menu, MenuListNode *anchor, const void *text, s32 mode, u32 options) {
    return mnuInsertListNodeRelativeToAnchor(menu->list, anchor, text, mode, options);
}

void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu) {
    mnuRemoveListCursorNode(menu->list);
}

/* Advance selection; clear its byte and panel sprite flags only when a node is returned. */
MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListAdvanceCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

/* Retreat selection with the same conditional byte/panel cleanup as advancement. */
MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step) {
    MenuListNode *selected = mnuListRetreatCursor(menu->list, step, 0);
    if (selected != NULL) {
        selected->selectionByte54 = 0;
        mnuClearEntryFlags(&menu->panel);
    }
    return selected;
}

MenuListNode *mnuAdvanceWindowListSelection(MenuWindowContainer *menu) {
    return mnuAdvanceListSelection(menu, 0);
}

MenuListNode *mnuRetreatWindowListSelection(MenuWindowContainer *menu) {
    return mnuReverseListSelection(menu, 0);
}

INCLUDE_ASM(const s32, "code_003BD050", func_003BD850);

void mnuClearWindowListSelectionFlags(MenuWindowContainer *menu) {
    mnuClearListFlagsOneAndTwo(&menu->list->stateFlags);
}

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value,
                        EffectSlotSet *resource, s32 *idx, s32 unused) {
    obj->value = value;
    obj->sprite[0] = effCreateResourceSlotSet(resource, idx[0], 1);
    obj->sprite[1] = effCreateResourceSlotSet(resource, idx[1], 1);
    obj->sprite[2] = effCreateResourceSlotSet(resource, idx[2], 1);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[0], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[1], 0, w, h, w, h);
    itfSetGridEntryQuantizedAndRefresh(obj->sprite[2], 0, w, h, w, h);
}

/* Create an owned three-sprite bundle using the caller's slot-index array. */
MenuIconSprites *mnuCreateWindowSpriteResources(u32 width, u32 height, u32 value,
                    EffectSlotSet *resourceHandle, s32 *indices, u32 unused) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x18);
    MenuIconSprites *bundle = (void *)sdfMemLock(allocation);
    memset(bundle, 0, 0x18);
    bundle->allocation = allocation;
    mnuInitIconSprites(bundle, width, height, value, resourceHandle, indices, unused);
    return bundle;
}

/* Destroy every native sprite slot, then release the bundle's allocation handle. */
void mnuReleaseWindowTextures(MenuIconSprites *menu) {
    u32 spriteIndex = 0;
    do {
        effDestroyResourceSlotSet(menu->sprite[spriteIndex]);
        spriteIndex++;
    } while (spriteIndex < MNU_WINDOW_RESOURCE_SPRITES);
    sdfMemFreeDirect(menu->allocation);
}

void func_003BDA80(void) {
}

void mnuDrawListScrollbar(s32 x, s32 y, s32 depth, u32 opacity, MenuList *list,
                   MenuIconSprites *resource, s32 drawArg) {
    s32 top;
    s32 trackExtent;
    s32 bottom;
    s32 count;
    s32 visible;
    s32 rowOffset;
    EffectSlotSet *firstSprite;
    EffectSlotDescription *firstDescription;
    BdWork *firstWork;
    BdWork *lastWork;

    if (resource == NULL) {
        return;
    }

    firstSprite = resource->sprite[0];
    firstDescription = firstSprite->descriptions;
    firstWork = firstSprite->workEntries;
    lastWork = resource->sprite[2]->workEntries;
    trackExtent = firstDescription->rect[1] * 8 - 0x28;
    top = firstWork->geometry.bounds[3];
    top += trackExtent;
    bottom = lastWork->geometry.bounds[3];
    count = list->count;
    visible = list->visibleCount;
    if (count == 0 || visible == 0 || count <= visible) {
        return;
    }

    rowOffset = (top - bottom) * list->head->index / (count - visible);
    effDrawTextureSlotWithBlendedCornerColors(x, y, depth, opacity, 1,
                  firstSprite, 0, drawArg);
    effDrawTextureSlotWithBlendedCornerColors(x, y + top, depth, opacity, 1,
                  resource->sprite[1], 0, drawArg);
    effDrawTextureSlotWithBlendedCornerColors(x, y + rowOffset + 0x28, depth, opacity, 1,
                  resource->sprite[2], 0, drawArg);
}

INCLUDE_ASM(const s32, "code_003BD050", mnuDrawWindowResourceSpriteRows);

INCLUDE_ASM(const s32, "code_003BD050", func_003BDC28);

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option) {
    mnuDrawWindowIconRows(x, y, flags, window, window->list->visibleCount, option);
}

INCLUDE_ASM(const s32, "code_003BD050", func_003BDD40);

INCLUDE_ASM(const s32, "code_003BD050", mnuDrawWindowSelectionPanel);

INCLUDE_ASM(const s32, "code_003BD050", func_003BDEE8);

INCLUDE_ASM(const s32, "code_003BD050", mnuSpinForWindowListCount);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6180);

INCLUDE_ASM(const s32, "code_003BD050", mnuCreatePanelIconState);

/* Reset low sprite flags only for a present first sprite and a supported panel kind. */
void mnuClearEntryFlags(MenuIconState *group) {
    s32 spriteIndex;
    if (group->sprite[0] != NULL && group->kind < MNU_PANEL_KIND_LIMIT) {
        for (spriteIndex = 0; spriteIndex < group->count; spriteIndex++) {
            EffectSlotSet *obj = group->sprite[spriteIndex];
            u32 *flags = &obj->workEntries->states[0].flags;
            *flags &= ~1;
            effInitializeSlotWork(obj, 0);
        }
    }
}

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
void mnuReleaseResourceList(MenuIconState *list) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < list->count; slotIndex++) {
        if (list->sprite[slotIndex] != 0) {
            effDestroyResourceSlotSet(list->sprite[slotIndex]);
        }
    }
    sdfChipFreeDirect(list);
}

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 surface) {
    MenuPosTable3 table = D_0042AF48;
    s32 width = state->sprite[0]->workEntries->sourceWidth << 4;
    s32 i;
    s32 a;
    if (state->fade > 0x100) {
        alpha = 0x200 - state->fade;
    }
    for (i = 0; i < state->count; i++) {
        if (i == 0 && mode != 1) {
            a = state->fade < 0x100 ? alpha : 0x100;
        } else {
            a = alpha;
        }
        a = i == 2 ? 0x100 : a;
        if (i == 2 && mode == 2) {
            continue;
        }
        effDrawTextureSlotWithBlendedCornerColors(x + table.pos[i].x, y + table.pos[i].y, z, a, 1, state->sprite[i], 0, surface);
    }
    state->sprite[0]->workEntries->geometry.bounds[2] = width;
    if (state->count >= 3) {
        state->sprite[2]->workEntries->geometry.bounds[2] = width;
    }
}

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface) {
    MenuPosTable table = D_0042AF60;
    s32 i;
    for (i = 0; i < state->count; i++) {
        if (i != 3 && i != 5) {
            effDrawTextureSlotWithBlendedCornerColors(x + table.pos[i].x, y + table.pos[i].y, z, w, 1, state->sprite[i], 0, surface);
        }
    }
}

void mnuDrawIconPair(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface) {
    MenuOffsets offset = D_0042AF90;
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x0, y + offset.y0, z, w, 1, state->sprite[0], 0, surface);
    effDrawTextureSlotWithBlendedCornerColors(x + offset.x1, y + offset.y1, z, w, 1, state->sprite[1], 0, surface);
}

/* Dispatch six DDS2 panel kinds; only kind four forces full fade. */
void mnuDrawIconPanel(s32 x, s32 y, s32 depth, s32 fade, MenuIconState *panel, s32 selectionMode, s32 drawArg) {
    switch (panel->kind) {
    case 0:
    case 1:
    case 2:
    case 3:
        mnuDrawIconPanelFade(x, y, depth, fade, panel, selectionMode, drawArg);
        return;
    case 4:
        mnuDrawIconRow6(x, y, depth, MNU_FULL_FADE, panel, drawArg);
        return;
    case 5:
        mnuDrawIconPair(x, y, depth, fade, panel, drawArg);
        break;
    }
}

void mnuDrawIconState(u32 x, u32 y, u32 depth, u32 fade, MenuIconState *panel, u32 selectionMode, u32 drawArg) {
    mnuDrawIconPanel(x, y, depth, fade, panel, selectionMode, drawArg);
}

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg) {
    mnuDrawIconState(x, y, depth, MNU_FULL_FADE, panel, 0, drawArg);
}

void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *obj) {
    switch (obj->kind) {
    case 4:
        itfGridLookupValueOrDefault(obj->sprite[2], 0);
        itfGridLookupValueOrDefault(obj->sprite[3], 0);
        itfGridLookupValueOrDefault(obj->sprite[4], 0);
        itfGridLookupValueOrDefault(obj->sprite[5], 0);
        return;
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 5:
        itfGridLookupValueOrDefault(obj->sprite[0], 0);
        itfGridLookupValueOrDefault(obj->sprite[1], 0);
        break;
    }
}

/* Rebuild the first-node pointer by walking backward from the cursor. */
void mnuRebuildListFirstFromCursor(MenuList *list) {
    s32 node;
    s32 first;
    s32 previous;

    previous = (s32)list->cursor;
    first = (s32)list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = (s32)((MenuListNode *)node)->prev;
    }
    list->first = (MenuListNode *)first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void mnuRebuildListLastFromCursor(MenuList *list) {
    s32 node;
    s32 last;
    s32 next;

    next = (s32)list->cursor;
    last = (s32)list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = (s32)((MenuListNode *)node)->next;
    }
    list->last = (MenuListNode *)last;
}

/* Reset the viewport/cursor to the first node; exactly one requests replay
 * toward the saved cursor, rather than treating every nonzero value as true. */
void mnuResetNodeLinks(MenuList *list, s32 restoreCursor) {
    MenuListNode *first;
    MenuListNode *oldCursor;

    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreCursor == 1) {
        MenuListNode *node = first;
        if (node == NULL) {
            return;
        }
        do {
            if (node == oldCursor) {
                return;
            }
            mnuAdvanceListCursorDefault(list);
            node = node->next;
        } while (node != NULL);
    }
}

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(MenuListNode **nodeArray, s32 count) {
    s32 itemIndex;

    nodeArray[0]->prev = NULL;
    nodeArray[0]->next = nodeArray[1];
    for (itemIndex = 1; itemIndex < count - 1; itemIndex++) {
        nodeArray[itemIndex]->prev = nodeArray[itemIndex - 1];
        nodeArray[itemIndex]->next = nodeArray[itemIndex + 1];
    }
    nodeArray[count - 1]->prev = nodeArray[count - 2];
    nodeArray[count - 1]->next = NULL;
    for (itemIndex = 0; itemIndex < count; itemIndex++) {
        nodeArray[itemIndex]->index = itemIndex;
    }
}

INCLUDE_ASM(const s32, "code_003BD050", mnuCompareTertiaryKeyDescending);

INCLUDE_ASM(const s32, "code_003BD050", mnuCompareTertiaryKeyAscending);

INCLUDE_ASM(const s32, "code_003BD050", func_003BEB00);

INCLUDE_ASM(const s32, "code_003BD050", func_003BEB28);

INCLUDE_ASM(const s32, "code_003BD050", func_003BEB58);

INCLUDE_ASM(const s32, "code_003BD050", func_003BEB80);

INCLUDE_ASM(const s32, "code_003BD050", mnuSortItems);

/* Allocate four native fade records into pointer slots after the list header. */
void mnuAllocateListEntries(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        list[slotIndex + 1] = sdfChipAllocZeroDirect(MNU_FADING_SLOT_BYTES);
    }
}

/* Free the four record blocks, not their nested window pointers. */
void mnuFreeListEntries(s32 *list) {
    s32 *entries = list + 1;
    u32 slotIndex = 0;
    do {
        sdfChipFreeDirect(*entries++);
        slotIndex++;
    } while (slotIndex < MNU_FADING_SLOT_COUNT);
}

INCLUDE_ASM(const s32, "code_003BD050", mnuAppendFadingWindowEntry);

/* Native removal only accepts indices at least five. Propagate the tail
 * record backward to the target, clearing each source's window pointer. */
void mnuRemoveFadingWindowEntry(s32 *list, u32 slotIndex) {
    s32 *entries;
    s32 *slot;
    u32 tailIndex;

    if (slotIndex >= MNU_FADING_MUTATION_THRESHOLD) {
        entries = list + 1;
        slot = entries + slotIndex;
        if (((MenuFadeEntry *)*slot)->active != 0) {
            mnuDestroyWindowContainer(((MenuFadeEntry *)*slot)->window);
        }
        tailIndex = list[0] - 1;
        ((MenuFadeEntry *)*slot)->window = 0;
        for (; slotIndex < tailIndex; tailIndex--) {
            *(MenuFadeEntry *)entries[tailIndex - 1] = *(MenuFadeEntry *)entries[tailIndex];
            ((MenuFadeEntry *)entries[tailIndex])->window = 0;
        }
        list[0]--;
    }
}

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < (u32)list[0]; slotIndex++) {
        MenuFadeEntry *entry = (MenuFadeEntry *)list[slotIndex + 1];
        mnuDrawWindowContainer(entry->x, entry->y, depth, entry->window, drawArg);
    }
}

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list) {
    u32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_FADING_SLOT_COUNT; slotIndex++) {
        MenuFadeEntry *entry = (MenuFadeEntry *)list[slotIndex + 1];
        if (entry->fade != 0) {
            entry->fade -= MNU_FADING_WORD_STEP;
        } else {
            mnuRemoveFadingWindowEntry(list, slotIndex);
        }
    }
}

INCLUDE_RODATA(const s32, "code_003BD050", D_004D61C8);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D61E0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6210);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6220);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6240);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6250);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6260);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6270);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6280);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6290);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62A0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62B0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62C0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62D0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62E0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D62F0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6300);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6310);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6320);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6330);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6340);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6350);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6360);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6370);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6380);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6390);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63A0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63B0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63C0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63D0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63E0);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D63F8);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6408);

INCLUDE_RODATA(const s32, "code_003BD050", D_004D6418);

INCLUDE_SDATA(const s32, "code_003BD050", D_004E4148);

