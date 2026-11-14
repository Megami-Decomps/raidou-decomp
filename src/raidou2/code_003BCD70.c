#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_002B0278.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "common.h"

#include "newdata.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dsp_entry_api.h"

#include "dds/dsp_message_window_api.h"

#include "dds/eff_random.h"

#include "dds/evt_message_window_api.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdk_libc.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/ui_color.h"

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Advance caller-owned state, returning its next 32-bit value. NULL selects
 * the shared effect RNG state. */
u32 dds3GetRandA(EffRandState *state);

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

/* Return the signed control-state byte as a query result, not a boolean or a
 * stored phase. NO_ACTIVE_CONTROL (0) covers a missing window and control state
 * two hidden by the gate; stored zero also passes through. Visible stored states
 * one, two and three report ACTIVE, GATED and CLEANUP. Other stored values pass
 * through unchanged. */
EvtMessageWindowControlResult fclChkMessage(void);

/* Start a display entry; return zero if no window is open, one if it starts. */
s32 fclStartMessage(s32 entry);

/* Request the existing message-window worker's mode-one transition. */
void fclUpdateMessage(void);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

void frSetPriority(struct _FRQ *glyph, u32 unshiftedValue);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

void *sdfChipAllocDirect(s32 size);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

#include "dds/sdk_ps2.h"

#include "dds/sdf_gs_register_submit_api.h"

#include "dds/dsp_message_window_api.h"

#include "dds/evt_message_window_api.h"

#include "dds/evt_stage_test_api.h"

#include "dds/evt_stage_test_resource_work_result.h"

#include "dds/dsp_entry_api.h"

#include "dds/sdf_chip.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/ui_color.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/mnu_input.h"

#include "dds/kwln.h"

#include "dds/pty_skill_menu_api.h"

#include "dds/mnu.h"

#include "dds/mnu_item_requirement_api.h"

#include "dds/mnu_party_panel_api.h"

#include "dds/mnu_party_menu_data.h"

#include "dds/brs_level_progress_api.h"

#include "dds/mnu_panel_state.h"

#include "dds/sdf_heap_api.h"

#include "dds/mnu_staff.h"

#include "dds/mnu_ability_target_category.h"

#include "dds/mnu_list.h"

#include "dds/mnu_entry_use_status_api.h"

#include "dds/eff.h"

#include "dds/mnu_shop.h"

#include "dds/mnu_scroll_panel.h"

#include "dds/dat_state.h"

#include "dds/dat_assigned_item_id.h"

#include "dds/mnu_standard_partner_requirements_api.h"

#include "dds/pty_skill_nibble_state.h"

#include "dds/pty_roster_api.h"

#include "dds/prf_skill_list_buffer.h"

#include "dds/eff_random.h"

#include "dds/fr_font.h"

#include "dds/pty_adjust_item_quantity_api.h"

/* Declared before first use (were implicit int). */
extern void evtStageTestInit(s32 mode);

extern void evtStageTestStop(void);

extern s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex);

extern void mnuResetWorkFloats(void);

extern void mnuSetStateValue(u32 *out, u32 value);

extern void mnuSwitchCampVisualCategory(MnuStaffVisualCategory nextCategory, MenuStaffContext *menu);

extern void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

struct MenuListNode;

struct StaffMenuRuntime;

#include "dds/fpu.h"

#include "dds/sdf.h"

#include "dds/itf.h"

#include "dds/menu_fade_entry.h"

extern void fclSetMessageVar(s32, const char *);

extern DatAssignedItemId mnuGetPartyEntryCurrentId(dds_datUnitWork_t *);

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

#define MNU_STAFF_BACKUP_BYTES 0x8D4

#define MNU_STAFF_PARTY_ACTIVE_BIT 1

#define MNU_STAFF_NODE_SELECTED 2

#define MNU_STAFF_PARTY_PANEL_BASE 0x284

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

extern s32 mnuLookupRangeEntry(u16);

extern u16 mnuGetAdjustedEntryValue(s32, dds_datUnitWork_t *);

extern s32 mnuGetRangeEntryFlatValue(s32);

extern u8 mnuGetRangeEntryKind(u32);

extern _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, _FRQ *);

extern void mnuDrawRepeatedPanelSprites(s32, s32, s32, s32, s32, EffectSlotSet *, s32, s32);

extern char D_00437BF8[];

extern u8 *D_00435E5C;

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

extern EvtStageTestResourceWorkResult evtStageTestAdvanceResourceWork(void);

extern s32 mnuIsStaffMenuInputBlocked();

extern MenuPopupEntry mnuStaffCampPagePopupEntry[];

extern void mnuDrawWindowDecorations(s32, s32, s32, s32, s32);

extern void mnuReleasePartyIconBundles(MenuPageWindow *);

extern void mnuClearEntries(MenuPageWindow *);

extern _FRQ *itfDrawUnderscoreTextSegment(s32, s32, s32, u32, const u8 *, s32);

extern _FRQ *itfDrawTextWithSelectedFontMode(s32, s32, s32, s8, u16, s32);

extern u8 *D_00435E54;

extern u8 *datUnitNames;

extern void mnuDrawSlotIcons(s32 x, MenuPageWindow *page);

extern void frEnableMode(s32);

extern u8 frDisableMode(u8);

extern MenuPopupEntry mnuCampPartySelectionReturnPopupEntry[];

extern struct sdfGrOt_t *D_dds1_00325788[13][4];

extern s8 evtStageTestUpdate(struct sdfGrOt_t **);

extern MenuPopupEntry mnuSkillMenuDetailPopupEntry[];

extern MenuPopupEntry mnuCampFieldSkillPopupEntry[];

extern MenuPopupEntry mnuSkillMenuFieldUsePopupEntry[];

extern MenuPopupEntry mnuSkillMenuEquippedSlotsPopupEntry[];

extern MenuPopupEntry mnuSkillMenuPagePopupEntry[];

extern s32 ptySkillMenuApplyFieldUseAndCost();

extern void mnuPlayInputSound(s32, s32, u32 *);

extern u32 mnuCampBackdropFrameMap[];

extern const CampEffectRows mnuCampBackdropEffectRows;

extern char *mnuCampBackdropSpriteResourceNames[];

extern char *mnuCampBackdropAnimationResourceNames[];

extern void effRequestResourceByMode(const char *, const char *, s32, u32 *);

extern void mnuFreeWindowSprites();

extern s8 evtGetCapturedWindowPanelValue(void);

extern s32 mnuApplyRandomStatItemEffect(u8 *, dds_datUnitWork_t *, MnuRequirementItemId);

extern void mnuPrepareStaffValueChangeDialog(MenuStaffContext *, dds_datUnitWork_t *, s32, s32);

extern MenuPopupEntry mnuCampRingCustomizationPopupEntry[];

extern void mnuDrawCampGridAndLabel();

extern u8 *datSkillMessages;

extern void mnuDrawCampIconBackdropByKind(s32 kind, dds3ProcessID_t *task);

extern void mnuSelectPage(MenuPageWindow *, s32);

extern void mnuDrawCampGridAndLabelDefault();

extern char mnuCampCommandHelpTextBank[];

extern void mnuIdleVoiceTimer(struct StaffMenuRuntime *object);

extern void mnuDrawIconPanel(s32, s32, s32, s32, MenuIconState *, s32, s32);

extern void mnuUpdateWindowPanelHandleStatesKindFourFive(MenuIconState *);

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

extern MenuPopupEntry mnuStaffPartyBrowsePopupEntry[];

extern MenuPopupEntry mnuStaffFieldSkillDisplayPopupEntry[];

typedef struct MenuPanelDrawEntry {
    s32 slotIndex;
    s32 x;
    s32 y;
} MenuPanelDrawEntry;

/* The draw routine copies five entries; the data label has a trailing zero entry. */
typedef struct MenuPanelDrawLayout {
    MenuPanelDrawEntry entry[5];
} MenuPanelDrawLayout;

extern const MenuPanelDrawLayout D_0042AD38;

extern const u8 D_00437BF0[];

extern s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *);

extern void cmpDrawTriG(const u32 *, const u32 *, u32, const u32 *, u32);

extern void mnuDrawAndAdvanceRatioPanel(s32, s32, s32, u32, s32, s32, MenuPageBar *, u32);

extern u32 scrGetIndexedRecordAddress(u32, const u8 **);

extern void mnuDrawMenuTextById(s32, s32, s32, s32, s32, s32);

/* Menu runtime fields shared by the party, panel and resource handlers. */
typedef struct MenuContext {
    u8 pad00[8];
    MenuPopupState transitionWork; /* +0x08: native saved-entry transition state */
    s32 popupState[3];     /* 0x54: passed to the popup state handlers */
    struct EffectSlotSet *displayHandle;     /* 0x60 */
    struct EffectSlotSet *resourceHandle; /* 0x64: second base resource owner. */
    void *displayResource; /* 0x68 */
    struct EffectSlotSet *alternateResources[10]; /* +0x6C: indexed by party unit ID */
    u8 pad94[0x1C];
    s32 selectionResources[6];
    s32 labelHandle;       /* 0xC8 */
    struct EffectSlotSet *panelModels[10]; /* +0xCC: indexed by party unit ID */
    const void *partySelectionLayout; /* 0xF4: layout copied into the party window panel. */
    const void *skillCategoryLayout; /* 0xF8 */
    const void *equippedSkillLayout; /* 0xFC */
    s32 skillPanelResource; /* 0x100 */
    MenuWindowContainer *imageHandle; /* 0x104: passed to the window fade owner. */
    u8 pad108[4];
    s32 listHandle;        /* 0x10C */
    u8 pad110[8];
    s32 panelHandle;       /* 0x118 */
    u8 pad11C[0x168];
    MenuPageWindow partyWindow; /* 0x284: lists, page slots and selection */
    PartyPanel partyPanel; /* 0xA928: counters and five native 0x34-byte entries */
    MenuPanelGroup *panelGroup; /* 0xAA34 */
    MenuSpriteState *panelRequest; /* 0xAA38 */
    MenuSpriteState *panelEffects; /* 0xAA3C */
    u8 padAA40[8];
    s32 party;             /* 0xAA48 */
    u8 padAA4C[0x10];
    MenuProfilePanel *profilePanel; /* 0xAA5C: retained profile-panel allocation */
    u16 slotOfA[0x2A0]; /* 0xAA60 */
    u16 slotOfB[0x40]; /* 0xAFA0 */
    u16 skillSlots[0x75]; /* 0xB020 */
    u8 padB10A[2];
    MenuFadeFields transition; /* 0xB10C: complete native fade owner. */
    s32 titleFadingOut;
    s32 titleOpacity;
    s32 titleSlide;
    s32 highlightOpacity;
} MenuContext;

typedef struct MenuListNode MenuListNode;

/* Native selection-state allocation is 0x30 bytes, distinct from item state. */
typedef struct StaffMenuRuntime {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    u32 resources[2];
    s32 staffMode;
    s32 staffView;
    s32 staffSelection;
    s32 staffExit;
    MenuIconState *iconPanel; /* 0x20: created and released with the staff panels. */
    s32 active;
    s32 idleFrames;
    s32 motionSelection;
} StaffMenuRuntime;

/* The item/skill-state constructor allocates and clears this complete 0x3C. */
typedef struct SkillMenuRuntime {
    sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 skillMenuActive;
    MenuList *categoryList;
    MenuWindowContainer *skillWindows[4];
    struct MenuPanelState *skillPanel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    u32 unk2C; /* 2B6FE8 clears this after rebuilding selection. */
    u32 selectionFlags; /* 0x30: resets the selected skill entry. */
    u32 selectedIndex;
    s32 fade; /* 0x38: shared skill-category marker fade. */
} SkillMenuRuntime;

extern void ptyRecomputeMaxHpMp(dds_datUnitWork_t *unit);

extern void scrClearSecondaryScriptFlag(dds_datUnitWork_t *unit, u16 flagId);

extern s32 frFontDrawGlyphChain(_FRQ *, s8, u32);

typedef struct MenuListDefaults {
    s32 indices[3];
} MenuListDefaults;

extern MenuListDefaults D_r1_00463EC0;

extern const MenuListDefaults D_0042AE18;

extern char D_00437C00[];

extern void ptySkillMenuDrawEquippedSkillEntry(s32, s32, s32, MenuList *, MenuListNode *, s32);

/* Allocate a zeroed window and its list; the last two arguments configure list rows. */
extern void mnuDrawStaffPartySelectionPanel(dds3ProcessID_t *task);

extern const char *D_003E78D0[];

extern u8 *datItemMessages;

extern void mnuDrawStaffValueGrid(s32, s32, s32, MenuContext *);

extern void mnuSetPanelItemsFromRow(MenuPanelGroup *, s32);

extern void mnuClearStaffSceneConfigEntries(MenuPanelGroup *);

void mnuDrawStaffPartySelectionPanel(dds3ProcessID_t *task);

s32 mnuAdvanceStaffValuePopup(dds3ProcessID_t *callback);

s32 mnuFinishStaffValuePopup(dds3ProcessID_t *callback);

/* Initialize the menu display using a value from the resource chain. */
u32 mnuEnterSelectedResourceLabel(dds3ProcessID_t *task);

/* Registered party-slot assignment leave hook: no side effects; returns one. */
u32 mnuPartySlotAssignmentLeaveNoOp(void);

extern char mnuRingCustomizationStatEligibility[];

extern char mnuRingCustomizationBonusWeights[20];

s32 mnuApplyRandomStatItemEffect(u8 *menu, dds_datUnitWork_t *entry,
                                 MnuRequirementItemId itemId);

s32 mnuUpdatePartySlotAssignmentPopup(dds3ProcessID_t *callback);

s32 mnuStartPanelDispatch(dds3ProcessID_t *callback);

s32 mnuStartPanelExit(dds3ProcessID_t *callback);

/* Clear the selected item's five stat bonuses and its requirement count,
 * then recalculate the party record's maximum HP and MP. */
void mnuClearPartySelectionValues(dds_datUnitWork_t *entry, MnuRequirementItemId selection);

u32 mnuEnterSlotLabel(dds3ProcessID_t *task);

/* Registered party-slot clearing leave hook: no side effects; returns one. */
u32 mnuPartySlotClearLeaveNoOp(void);

s32 mnuPartySlotConfirmClearUpdate(dds3ProcessID_t *callback);

s32 mnuAdvancePartyClearPopup(dds3ProcessID_t *callback);

s32 mnuFinishPartyClearPopup(dds3ProcessID_t *callback);

/* Always allow entry to the registered party-selection return popup. */
u32 mnuAllowPartySelectionReturn(void);

/* Select the visual category used by the party-selection window. */
void mnuSelectPartySelectionVisuals(u32 context);

void mnuNoOpPartySelectionCleanup(u32 callback);

s32 mnuIsFinalItemIndex(s32 nodeIndex, const MenuList *list);

extern void mnuDrawPartySelectionListEntry(s32, s32, s32, MenuList *, MenuListNode *, s32);

extern void mnuCreatePartySelectionWindow(MenuContext *);

extern char D_00437BE8[];

void mnuCreatePartySelectionWindow(MenuContext *context);

void mnuDestroyPartySelectionWindow(MenuContext *context);

/* Snapshot the five party entries and cap the menu's displayed slot count. */
extern void mnuCopyPartyEntries(MenuContext *context);

/* Snapshot five party entries; at most three active slots are displayed. */
void mnuCopyPartyEntries(MenuContext *context);

extern void mnuPopulatePartyPanelSlot(u8 *entry, s32 unused, u32 index, PartyPanel *panel);

void mnuAssignSelectedPartyEntry(s32 entryIndex, s32 mode, s32 skipRefresh, MenuContext *context);

/* Notify active snapshot entries, restore the backup, then refresh panel resources. */
extern void mnuRestorePartyEntriesAndRefresh(MenuContext *context);

/* Notify active snapshot entries, restore the backup, then refresh panel resources.
 * The final loop counts down while copying backup entries forward. */
void mnuRestorePartyEntriesAndRefresh(MenuContext *context);

/* Count active entries in the five-slot party array, capped at three. */
s32 mnuCountActiveSlots(void);

/* Reset selection/backup state, activate all panel slots, and enable list nodes. */
void mnuClearPartySelectionAndActivateSlots(MenuContext *context);

/* Release panel textures before reinitializing slots and updating handle state. */
void mnuRefreshPartyPanelSlots(MenuContext *context);

s32 mnuInitializePartySelectionState(dds3ProcessID_t *task);

u32 mnuReleasePartySelectionResources(dds3ProcessID_t *task);

void mnuPreparePartyPanelTransition(MenuContext *menu);

void mnuClearWindowListSelectionFlags(MenuWindowContainer *menu);

s32 mnuHandlePartySelectionInput(dds3ProcessID_t *callback);

void mnuDrawPartySelectionPreviewPanel(MenuContext *context, MenuPageBar *hpBar, MenuPageBar *mpBar,
                   dds_datUnitWork_t *entry, EffectSlotSet *portrait, u32 fade,
                   u32 frameFade, s32 dimmed);

/* Two-stage fade: B rises first when opening, A falls first when closing.
 * The second ramp begins strictly past its threshold; clamps follow each step. */
void mnuUpdateStaffFade(s32 opening, PartyMenuData *menuWork);

extern MenuProfilePanel *mnuCreateProfilePanel(dds_datUnitWork_t *selectionState);

extern void mnuSetGroupProperties(MenuProfilePanel *, EffectSlotSet *, EffectSlotSet *, s32, s32);

extern void mnuDrawAndAdvanceProfilePanel(s32, s32, s32, MenuProfilePanel *, s32);

extern void mnuFreeProfilePanelWork(MenuProfilePanel *);

extern void mnuDrawPartySelectionPreview(MenuContext *);

extern void mnuDrawPartySelectionPreviewPanel(MenuContext *, MenuPageBar *, MenuPageBar *, dds_datUnitWork_t *, EffectSlotSet *, u32, u32, s32);

/* Refresh the selected party member's effect pair and profile panel. */
void mnuDrawPartySelectionPreview(MenuContext *context);

/* Update the final-row flag, draw the panel, and suppress its contents update
 * once the party transition has frozen it. */
s32 mnuOpenStaffPartySelectionPanel(dds3ProcessID_t *callback);

s32 mnuStepPartySelectionControl(dds3ProcessID_t *callback);

u8 mnuIsStateNotOne(void);

/* Select the visual category used while the staff/party view is active. */
void mnuSelectStaffSelectionVisuals(u32 context);

void mnuReleaseMenuWindowHandles(s32 context);

/* Release both staff resource slots; their menu indices differ between games. */
void mnuReleaseStaffMenuResources(struct EffectSlotSet **baseResources);

/* Reset texture handles for the same two resource slots. */
void mnuReleaseStaffMenuTextureHandles(struct EffectSlotSet **baseResources);

u32 mnuCreateSelectState(dds3ProcessID_t *task, s32 staffView);

s32 mnuStaffCloseSelectionState(dds3ProcessID_t *task);

/* Open selection work in staff-view mode; the task argument is forwarded unchanged. */
u32 mnuOpenStaffSelectionEntry(dds3ProcessID_t *context);

s32 mnuFinishStaffSelectionEntry(dds3ProcessID_t *task);

/* Open the same selection work in party-view mode. */
u32 mnuOpenPartySelectionEntry(dds3ProcessID_t *context);

s32 mnuFinishPartySelectionEntry(dds3ProcessID_t *task);

/* Update the popup first; accept confirm/cancel only while its state is zero.
 * Return a nonzero popup-update word unchanged, or zero after handling input. */
s32 mnuStaffPopupUpdate(dds3ProcessID_t *callback);

s32 mnuDrawStaffCampPageWithImage(dds3ProcessID_t *callback);

s32 mnuStepStaffCampPageControl(dds3ProcessID_t *callback);

extern void mnuResetPartyPanelFade(MenuPageWindow *, s32, u32, u32);

extern void mnuSetPageParams(MenuSprites *, s32);

void mnuConfigurePartyPanelPage(MenuPageWindow *window, s32 mode, u32 staffImage, s32 staffMode);

extern void mnuConfigurePartyPanelPage(MenuPageWindow *, s32, u32, s32);

s32 mnuCreatePanels(dds3ProcessID_t *callback);

s32 mnuDestroyPanels(dds3ProcessID_t *callback);

void mnuResetSelectedPanelOpacity(s32 context);

/* Switch the party page, rebuilding its panels; previous takes priority.
 * The opaque request argument is forwarded unchanged. Return one if switched. */
s32 mnuStaffSwitchPartyPage(dds3ProcessID_t *requestArgument);

/* Update popup state before page navigation, view toggles, and exit requests.
 * stateWord holds the update result first, then the stored popup state. */
s32 mnuStaffBrowsePartyUpdate(dds3ProcessID_t *callback);

void mnuDrawSlotIcons(s32 x, MenuPageWindow *page);

void mnuDrawSelectedPartySlotMarkers(s32 context, StaffSlots *resources);

void mnuDrawTextSprite(s32 x, s32 y, s32 width, u32 color, const char *text, s32 flags);

void mnuDrawPartySkillAndStatusPanel(dds_datUnitWork_t *entry, MenuPageWindow *page, MenuPanelGroup *packedGroup, MenuSpriteState *spriteState, s32 unused, s32 spriteFlags);

void mnuDrawProfilePanelAndSprite(dds_datUnitWork_t *entry, u32 unused1, MenuSpriteState *spriteState,
                                    MenuProfilePanel *resource,
                                    u32 unused4, u32 spriteFlags);

void mnuDrawIconPanelFullFade(u32 x, u32 y, u32 depth, MenuIconState *panel, s32 drawArg);

s32 mnuDrawSelectedStaffPartyView(dds3ProcessID_t *callback);

void mnuIdleVoiceTimer(StaffMenuRuntime *object);

s32 mnuStaffIdlePartyUpdate(dds3ProcessID_t *callback);

/* Always allow entry to the registered skill-menu return popup. */
u32 mnuAllowSkillMenuReturn(void);

void mnuDrawRangeCostAndIcon(s32 x, s32 y, s32 depth, s32 xOffset, u32 fade,
                   dds_datUnitWork_t *actor, u16 rangeId, s32 style, s32 dim,
                   EffectSlotSet *costResource, u32 texture);

extern void mnuDispatchEntryWords(EffectSlotSet *, s32, MenuListNode *);

extern void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);

/* Draw one equipped-skill row: the skill's range icon and cost, or an empty
 * placeholder bar. The row is highlighted under an unlocked cursor or when
 * the node is flagged. */
void ptySkillMenuDrawEquippedSkillEntry(s32 x, s32 y, s32 depth, MenuList *list,
                   MenuListNode *node, s32 texture);

s32 ptySkillMenuBuildEquippedSlots(
    PtySkillUnavailableMarking markUnavailableSkills, dds3ProcessID_t *callback);

u32 mnuDestroySelectedPartyWindow(dds3ProcessID_t *callback);

/* Seek the first node with a nonzero sort key and no unavailable flag.
 * Leave the cursor unchanged if no such node exists. */
void mnuSeekFirstAvailableStaffListNode(dds3ProcessID_t *task);

MenuWindowContainer *mnuSeekSelectedWindowCursor(
    PtySkillUnavailableMarking markUnavailableSkills, dds3ProcessID_t *callback);

/* Select the visual category used by the skill-selection windows. */
void mnuSelectSkillSelectionVisuals(u32 context);

void mnuNoOpItemSelectionTeardown(s32 context);

extern void mnuDrawCampGridResourceSlot(s32, s32, s32, struct MenuList *,
                                        struct MenuListNode *, s32);

extern void mnuDispatchEntryWords(EffectSlotSet *, s32, MenuListNode *);

extern void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);

typedef struct MnuCategoryPositions {
    s32 entries[4][2];
} MnuCategoryPositions;

extern const MnuCategoryPositions D_0042AE28;

extern u32 scrGetSecondaryScriptFlag(dds_datUnitWork_t *, u16);

void mnuDrawSkillCategoryEntry(s32 x, s32 y, s32 depth, MenuList *list,
                   MenuListNode *node, s32 texture);

extern s32 ptyHasSkill(dds_datUnitWork_t *, s32);

extern u32 scrGetSecondaryScriptFlag(dds_datUnitWork_t *, u16);

/* Secondary script bits enable the category marker independently of learned skills. */
void mnuRefreshSkillWindowOwnershipFlags(MenuContext *context);

u32 *mnuBuildOwnedSkillBits(void);

/* Release the temporary skill-code bitmap allocated for page construction. */
void mnuReleaseSkillCodeBitset(u32 *bits);

s32 mnuIsSkillCodeInBitset(s32 skillCode, u32 *bits);

/* DDS2 page setup uses the same full owners as the surrounding menu code. */
extern struct MenuPanelState *mnuCreatePanelState(s32 width, s32 height);

extern void mnuSetPanelFirstGridSlot(struct MenuPanelState *, u32, u32, EffectSlotSet *, s32);

extern void mnuSetPanelSecondGridSlot(struct MenuPanelState *, s32, s32, EffectSlotSet *, s32);

extern void mnuSetPanelThirdGridSlot(struct MenuPanelState *, s32, s32, EffectSlotSet *, const s32 *);

extern void mnuSetPanelCornerGeometry(struct MenuPanelState *, s32, s32, EffectSlotSet *, s32, s32);

extern void mnuInitializePanelResource(struct MenuPanelState *, s32, s32);

extern MenuList *mnuCreateListState(u32, u32, s32);

extern void mnuSortItems(MenuList *, s32, s32);

extern s32 mnuGetIndexedNonzeroEffect(s32);

extern u16 mnuLookupPartyTableValue(u32, s32, s32);

extern const s32 mnuCampSkillHeadingFrameIndices[];

void ptySkillMenuInitPages(MenuContext *context);

void mnuDestroySkillMenuWindows(s32 context);

u32 mnuCreateItemState(dds3ProcessID_t *callback);

s32 mnuCloseItemSelectionState(dds3ProcessID_t *selection);

s32 ptySkillMenuShellUpdate(dds3ProcessID_t *callback);

s32 mnuCampMenuDrawSlotLabel(dds3ProcessID_t *callback);

s32 mnuStepSkillSlotControl(dds3ProcessID_t *callback);

void mnuClearSkillSlotReorderSelection(dds3ProcessID_t *task);

MnuSkillSlotReorderState mnuGetSkillSlotReorderState(dds3ProcessID_t *callback);

/* Set the selected flag only on nodes whose index matches the saved selection. */
void mnuHighlightSelectedListNode(dds3ProcessID_t *task);

s32 mnuUpdateEmptySkillSelectionCursor(dds3ProcessID_t *callback);

u32 mnuClearSkillSelectionImageState(dds3ProcessID_t *task);

/* Narrow the ID to its native 16-bit skill code before duplicate detection.
 * Insert only missing skills, then recompute maxima and clear the script flag. */
void mnuAddPartySkillIfMissing(dds_datUnitWork_t *partyEntry, s32 skillId, s32 skillSlot);

/* Clear one skill slot and recompute the owning party record's maxima. */
void mnuClearPartySkillSlot(dds_datUnitWork_t *partyEntry, s32 skillSlot);

/* Open the selected skill's popup or cancel, then process list navigation.
 * Native list reads precede the late window guard; preserve that ordering. */
void mnuCampMenuHandleInput(dds3ProcessID_t *callback);

/* Apply a selected skill to the current slot, then process window input. */
void ptySkillMenuHandleSelection(dds3ProcessID_t *callback);

void mnuSwapPartySkillSlots(dds_datUnitWork_t *party, s32 firstSlot, s32 secondSlot);

/* First confirm stores a slot; a different second slot swaps and rebuilds.
 * Cancel clears the saved slot, opening the exit popup only if none was saved. */
void ptySkillMenuHandleSlotReorder(dds3ProcessID_t *callback);

s32 ptySkillMenuUpdate(dds3ProcessID_t *callback);

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

extern void mnuDrawPanelState();

void ptySkillMenuCopyPageState(s32 context);

s32 ptySkillMenuEnterPage(dds3ProcessID_t *callback);

s32 ptySkillMenuDispatchPageRequest(dds3ProcessID_t *callback);

s32 ptySkillMenuApplyFieldUseAndCost();

/* Mark entries whose field-use cost cannot be paid by the selected party member. */
void mnuFlagMatchingEntries(s32 context);

s32 ptySkillMenuHandleFieldUse(dds3ProcessID_t *callback);

s32 ptySkillMenuOpenSelectedSkillPage(dds3ProcessID_t *callback);

s32 ptySkillMenuDispatchConfirmRequest(dds3ProcessID_t *callback);

s32 ptySkillMenuOpenPartyPage(dds3ProcessID_t *callback);

void mnuClearPageSelectionHandles(MenuPageWindow *window);

u32 ptySkillMenuClosePartyPage(dds3ProcessID_t *callback);

s32 ptySkillMenuHandlePageSwitch(dds3ProcessID_t *callback);

MenuListNode *mnuReverseListSelection(MenuWindowContainer *menu, s32 step);

MenuListNode *mnuAdvanceListSelection(MenuWindowContainer *menu, s32 step);

/* Shift a selected child window's list to the requested row. Keep the
 * double-dereferenced cursor load raw: the typed form does not match. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target);

s32 ptySkillMenuBrowseCandidatePages(dds3ProcessID_t *callback);

s32 mnuOpenSkillDetailPanel(dds3ProcessID_t *callback);

/* Run the skill-detail panel's phase-two control and return its result. */
s32 mnuStepSkillDetailControl(dds3ProcessID_t *callback);

void mnuDrawSelectionLabel(u16 id);

extern MenuPoint D_00437C08[];

extern MenuPoint D_00437C10[];

extern MenuPoint D_00437C18[];

extern const char *mnuComboGunRequirementTexts[16];

extern s32 ptyGetAffinityKind(s32 affinityId, s32 index);

extern s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index);

/* Draw each of the command's three partner requirements. */
void mnuDrawAffinityParticipantLabels(u16 affinity, s32 resource, s32 labels);

/* Draw one skill-menu row: its grid slot, then the skill's range, cost and icon,
 * or the empty-row panel when the row holds no skill. */
void ptySkillMenuDrawEntry(s32 x, s32 y, s32 depth, MenuList *list,
                           MenuListNode *node, s32 texture);

/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(const dds_datUnitWork_t **out, s32 max);

u8 mnuHasStandardPartnerRequirements(BtlCommandId commandId);

extern s32 btlAccessRecordedLinkageSkillFlag(s32 id, s32 operation);

extern u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);

extern void ptySkillMenuDrawEntry(s32, s32, s32, MenuList *, MenuListNode *, s32);

/* Construct the linkage-skill list, retaining blank rows for available commands
 * that have not yet been recorded. Sort by the per-command catalog ordinal. */
void ptySkillMenuBuildLinkageSkills(dds3ProcessID_t *task);

u32 mnuDestroySkillSelectionWindow(dds3ProcessID_t *callback);

u32 mnuResetSelection(dds3ProcessID_t *callback);

u32 mnuCloseSkillSelection(dds3ProcessID_t *callback);

s32 mnuUpdateSkillListInput(dds3ProcessID_t *callback);

typedef struct MnuCampPanelDrawRecord {
    EffectSlotSet *resource;
    s32 sprite;
    s32 xOffset;
    s32 yOffset;
} MnuCampPanelDrawRecord;

typedef struct MnuCampPanelOrigins {
    MenuPoint primary;
    MenuPoint secondary;
} MnuCampPanelOrigins;

extern const MnuCampPanelOrigins D_0042AE48;

void mnuDrawSkillMenuFrameIcons(MenuContext *menuContext);

/* DDS2 retains the iteration but the frame drawing is compiled out.
 * DDS1 mnuDrawListFrames draws the frames at the same call site. */
void mnuDrawListFrames(s32 context);

s32 mnuCampMenuDrawStatus(dds3ProcessID_t *callback);

/* Run the camp-status panel's phase-two control and return its result. */
s32 mnuStepCampStatusControl(dds3ProcessID_t *callback);

void mnuInitializeMapPacket(u32 value, u32 *values, s32 count, MapPacket *packet);

void mnuOrEntryFlags(u32 flags, u32 *entryFlags);

void mnuCopyCampEffectRowData(const CampEffectRows *, MenuEffectResources *);

/* Copy two four-word effect rows from their serialized representation. */
void mnuCopyCampEffectRowData(const CampEffectRows *source, MenuEffectResources *resources);

void mnuSetCampEffectResourceHandles(struct EffectSlotSet *sheet,
                                    struct EffMappedResource *animation,
                                    MenuEffectResources *resources);

void mnuBindCampEffectAnimation(MenuEffectResources *resources);

void mnuLoadEffectResources(MenuEffectResources *resources);

void mnuRequestEffectResources(MenuEffectResources *resources);

u32 mnuBindCampEffectWhenLoaded(MenuEffectResources *resources);

void mnuDestroyEffectResources(MenuEffectResources *resources);

void mnuSpawnSpark(MenuCampEffect *fx);

void mnuRetireCampSpark(MenuCampEffect *effects, s32 index);

void mnuDrawAndAdvanceCampSparks(MenuCampEffect *fx, s32 surface);

typedef struct MenuBadgePlace {
    s32 slot;
    s32 x;
    s32 y;
} MenuBadgePlace;

typedef struct MenuBadgeLayout {
    MenuBadgePlace place[2];
} MenuBadgeLayout;

typedef struct MenuCampBackdropLayout {
    MenuBadgeLayout layer[2];
} MenuCampBackdropLayout;

extern MenuBadgePlace D_0042AE90;

extern MenuCampBackdropLayout D_0042AEA0;

extern void uiDrawActiveSurfaceWithTestMode(u32);

extern void uiConfigureSurfaceAlphaState(s32);

extern void uiDrawSurfaceAtNearDepth(u32);

void mnuDrawLayeredCampGridAndSparks(MenuCampEffect *set, s32 surface);

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

u32 mnuDestroyListState(MenuList *list);

void mnuUpdateListScrollFlags(MenuList *list);

MenuListNode *mnuListAppendNode();

s32 mnuListContainsFinalNode(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *, s32, s32);

MenuListNode *mnuListRetreatCursor(MenuList *, s32, s32);

/* Insert a node holding `value` before (or, with options & MNU_LIST_INSERT_AFTER_ANCHOR, after) anchor, keeping the visible window and
 * cursor in place. */
MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
                         const void *text, s32 mode, u32 options);

MenuListNode *mnuRemoveListCursorNode(MenuList *list);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;

extern void itfSetGridEntryQuantizedAndRefresh();

void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect);

MenuListNode *mnuWalkNodeList(s32 targetIndex, MenuList *list);

s32 mnuSeekListNode(s32 targetIndex, MenuList *list);

void mnuSelectFirstListNode(MenuList *list);

void mnuSelectLastListNode(MenuList *list);

MenuListNode *mnuAdvanceListWindowStart(MenuList *list);

/* Step the visible head back one node when a full window follows it. */
MenuListNode *mnuRetreatListWindowStart(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuAdvanceListCursorDefault(MenuList *list);

MenuListNode *mnuRetreatListCursorDefault(MenuList *list);

MenuListNode *mnuScrollListToEnd(MenuList *list);

/* Retreat the window by up to one page; at the first page, select the first
 * node, otherwise re-centre a cursor left on the window's final row. */
MenuListNode *mnuScrollListToPreviousPage(MenuList *list);

void mnuClearListFlagsOneAndTwo(u32 *flags);

u32 mnuTestListFlagTwo(u32 *flags);

/* Draw one four-sprite bank; the cursor entry selects the second bank. */
void mnuDrawFourEntries(s32 x, s32 y, s32 depth, MenuList *list, MenuListNode *node, s32 drawArg);

/* Blend the flag-selected packed color with the caller's previous color.
 * Flag one takes precedence over DDS2's additional flag-four color choice. */
u32 mnuBlendListNodeColorByFlags(u32 previousColor, MenuListNode *entry);

/* Apply the same entry-state blend to all four packed colors in one slot. */
void mnuDispatchEntryWords(EffectSlotSet *menu, s32 index, MenuListNode *entry);

/* Invoke the native window renderer at full fade with its size/flag arguments zeroed. */
void mnuCallInitWide(s32 x, s32 y, s32 depth, s32 menu, s32 drawArg);

void mnuRemoveWindowListCursorNode(MenuWindowContainer *menu);

void mnuCheckWindowListStatus(MenuWindowContainer *menu);

void mnuInitIconSprites(MenuIconSprites *obj, s32 w, s32 h, u32 value,
                        struct EffectSlotSet *resource, s32 *idx, s32 unused);

void func_002B9A38(void);

void mnuDrawWindowResourceSpriteRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, u32 option);

void mnuDrawWindowIconRows(s32 x, s32 y, u32 flags, MenuWindowContainer *window, s32 count, s32 option);

void mnuDrawVisibleWindowIconRows(u32 x, u32 y, u32 flags, MenuWindowContainer *window, u32 option);

/* Draw at the selected row before advancing the panel's transition value.
 * DDS2's alternate selection flag overrides its ordinary selection flag. */
void mnuDrawWindowSelectionPanel(s32 x, s32 y, s32 depth, MenuWindowContainer *window, s32 drawArg);

void mnuSpinForWindowListCount(MenuWindowContainer *window);

/* Destroy nonzero resource slots, retaining the native per-iteration count read, then free. */
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

extern MenuPosTable3 D_r1_00463F08;

void mnuDrawIconPanelFade(s32 x, s32 y, s32 z, s32 alpha, MenuIconState *state, s32 mode, s32 surface);

extern MenuPosTable D_r1_00463F20;

void mnuDrawIconRow6(s32 x, s32 y, s32 z, s32 w, MenuIconState *state, s32 surface);

typedef struct MenuOffsets {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} MenuOffsets;

extern MenuOffsets D_r1_00463F50;

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

/* Relink and reindex the pointer array. Native endpoint writes require at
 * least two entries; zero/one-entry calls are not guarded here. */
void mnuLinkItemList(MenuListNode **items, s32 count);

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
void mnuRemoveFadingWindowEntry(s32 *list, u32 index);

/* Use each record's stored position/window, with the caller supplying draw depth. */
void mnuDrawFadingWindows(s32 depth, s32 *list, s32 drawArg);

/* Subtract from every nonzero fade word without clamping. An already-zero
 * word calls the native remover, whose at-least-five index guard is retained. */
void mnuUpdateFade(s32 *list);

void mnuResetWindowFadeParameters(MenuFadeFields *menu);

void mnuSetMenuPageFlags(MenuSprites *page, u32 flags);

void mnuFreeIconSprites(MenuSprites *menu);

extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

void mnuDrawIconRow(s32 unusedA, s32 unusedB, s32 depth, s32 skip, MenuSprites *set, s32 drawArg);

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

extern MenuIconLayout D_0042AFD8;

void mnuReleaseIconBundleAndSprites(MenuIconBundle *menu);

void mnuDrawFadeIcons(s32 x, s32 y, s32 depth, s32 unused, MenuIconBundle *obj, s32 drawArg);

void mnuReleasePartyIconBundles(MenuPageWindow *menu);

s32 mnuPercentOrHundred(s32 value, s32 total);

/* Copy eight resource handles into the window's primary handle bank. */
void mnuCopyPrimaryWindowHandles(MenuPageWindow *menu, struct EffectSlotSet **source);

/* Copy eight resource handles into the window's secondary handle bank. */
void mnuCopySecondaryWindowHandles(MenuPageWindow *menu, struct EffectSlotSet **source);

void mnuRegisterResourceHandles(MenuPageWindow *destination, struct EffectSlotSet **source);

extern char D_00437C30[];

/* Create both party-panel lists and append their shared row labels. */
void mnuInitScrollLists(MenuPageWindow *menu, s32 *counts);

void mnuDestroyWindowOwnedLists();

void mnuClearPageSelection(MenuPageWindow *menu);

void mnuResolveUnselectedPageHandles(MenuPageWindow *window);

void mnuRefreshPageHandles(MenuPageWindow *window);
/* End of declarations from src/dds2/game/code_002B0278.c */

MenuListNode *mnuScrollListToEnd(MenuList *list) {
    s32 i;

    if (list->cursor == NULL) {
        return 0;
    }
    i = 0;
    while (i < list->visibleCount) {
        if (mnuListContainsFinalNode(list)) {
            if (i == 0) {
                if (list->cursor == list->last) {
                    return 0;
                }
                list->cursor = list->last;
                list->windowOffset = list->visibleCount - 1;
                return list->last;
            }
            break;
        }
        mnuAdvanceListWindowStart(list);
        i++;
        list->cursor = list->cursor->next;
        list->windowOffset += 1;
    }
    if (mnuListContainsFinalNode(list)) {
        list->windowOffset = list->visibleCount - 1;
        list->cursor = list->last;
        return list->last;
    }
    if (list->cursor == list->head) {
        list->cursor = list->cursor->next;
        list->windowOffset = 1;
    }
    mnuUpdateListScrollFlags((u8 *)list);
    return list->cursor;
}

INCLUDE_ASM(const s32, "code_003BCD70", mnuScrollListToPreviousPage);
