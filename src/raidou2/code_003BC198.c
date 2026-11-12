#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00279CC0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "newdata.h"

#include "dds/fr_font.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

void *sdfChipAllocZeroDirect(s32 size);

void sdfChipFreeDirect(void *memory);

#include "dds/camp_menu_context.h"

#include "dds/sdf_chip.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_source_lifetime.h"

#include "dds/mnu_input.h"

#include "dds/kwln.h"

#include "dds/mnu.h"

#include "dds/mnu_list.h"

#include "dds/mnu_shop.h"

#include "dds/mnu_scroll_panel.h"

#include "dds/dat_state.h"

#include "dds/pty_roster_api.h"

#include "dds/mnu_standard_partner_requirements_api.h"

#include "dds/eff.h"

#include "dds/eff_resource_records.h"

#include "dds/itf.h"

#include "dds/fr_font.h"

#include "dds/mnu_staff.h"

/* Declared before first use (were implicit int). */
extern void effRequestResourceByMode(const char *base, const char *name, u32 mode, u32 *outInstance);

extern s32 mnuCloseItemSelectionState(dds3ProcessID_t *contextArg);

extern void uiDrawTexturedSurfaceAtFarDepth(s32 surface);

extern void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

extern void uiPrepareMenuBackgroundSurface(s32 unused, s32 surface);

extern EffPayload *effCreatePayload(u32);

extern u32 effDestroyPayload(EffPayload *);

struct MenuListNode;

typedef struct MenuList MenuList;

extern void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *, s32, s32, s32, s32, s32);

extern u8 *datSkillMessages;

extern s32 frFontDrawGlyphChain(struct _FRQ *, s8, u32);

extern _FRQ *itfDrawTextWithSelectedFontMode(s32, s32, s32, s8, u16, s32);

typedef struct MenuListNode MenuListNode;

extern void mnuCreateStaffImageSprite(s32);

extern void ptySkillMenuCopyPageState(s32);

extern void mnuDrawStaffCampScreen(s32, dds3ProcessID_t *);

extern void mnuDrawCampGridAndLabel(s32, s32, s32, s32, s32, s32, s32);

extern void mnuDrawCampGridAndLabelDefault(s32, s32, s32, s32, s32, s32);

extern void mnuPlayInputSound(s32, s32, u32 *);

extern MenuPopupEntry mnuCampFieldSkillPopupEntry[];

/* Selected child window and page of the party skill-menu runtime. */
typedef struct MenuPartyRuntime {
    u8 pad00[4];
    s32 selectionList; /* 0x04: list whose cursor selects the active window */
    u8 pad08[0x1C];
    s32 selectedWindow; /* 0x24 */
    u8 pad28[4];
    s32 selectedPage;   /* 0x2C */
} MenuPartyRuntime;

extern MenuListNode * mnuAdvanceListSelection(MenuWindowContainer *window, s32 direction);

extern MenuListNode * mnuReverseListSelection(MenuWindowContainer *window, s32 direction);

/* Shift the chosen child window to target's row; the cursor index selects its slot. */
void mnuSeekSelectedWindowRow(s32 menu, s32 target);

extern MenuPopupEntry mnuSkillMenuPagePopupEntry[];

extern MenuPopupEntry mnuSkillMenuEquippedSlotsPopupEntry[];

extern s32 ptySkillMenuHandlePageSwitch(dds3ProcessID_t *);

extern void mnuClearWindowListSelectionFlags(MenuWindowContainer *);

extern void mnuClearListFlagsOneAndTwo(u32 *flags);

/* The camp initializer allocates and clears this complete 0x38-byte child. */
typedef struct SkillBrowseRuntime {
    struct sdfMemHandle_t *allocation;
    u8 pad04[4];
    s32 active;
    struct MenuList *categoryList;
    MenuWindowContainer *windows[4];
    void *panel;
    MenuWindowContainer *selectedWindow;
    s32 activeMark;
    s32 motionSelection;
    u32 selectionFlags;
    u32 selectedIndex;
} SkillBrowseRuntime;

typedef char SkillBrowseRuntimeSizeCheck[(sizeof(SkillBrowseRuntime) == 0x38) ? 1 : -1];

/* Browse the candidate pages: pad direction moves the window selection and the confirm/cancel buttons open popups. */
s32 ptySkillMenuBrowseCandidatePages(dds3ProcessID_t *task);

s32 mnuOpenSkillDetailPanel(dds3ProcessID_t *callback);

/* Run the skill-detail panel's phase-two control and return its result. */
s32 mnuStepSkillDetailControl(dds3ProcessID_t *callback);

void mnuDrawSelectionLabel(s32 selection);

extern s32 ptyGetAffinityKind(s32 affinityId, s32 index);

extern s32 ptyGetAffinityFlagsWithoutOverride(s32 affinityId, s32 index);

extern s32 mnuLookupRangeEntry(u16);

extern const u8 *mnuComboGunRequirementTexts[16];

extern const u8 *D_003BAA8C;

/* Draw each affinity requirement, including range markers and optional text. */
void mnuDrawAffinityParticipantLabels(u16 affinityId, s32 placeholderResource, s32 rangeResource);

extern void mnuDrawRangeCostAndIcon(s32 x, s32 y, s32 depth, s32 xOffset, u32 fade,
                   dds_datUnitWork_t *actor, u16 rangeId, s32 style, s32 dim,
                   EffectSlotSet *specialResource, EffectSlotSet *costResource, u32 texture);

extern void mnuDispatchEntryWords(EffectSlotSet *, s32, struct MenuListNode *);

extern void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);

/* Draw a linkage skill's range and cost, or the empty-slot frame and marker. */
void ptyDrawLinkageSkillEntry(s32 x, s32 y, s32 depth, struct MenuList *list,
                              struct MenuListNode *node, s32 texture);

/* Collect up to max pointers to occupied, frontline party slots. */
void mnuCollectFrontlinePartySlots(const dds_datUnitWork_t **out, s32 max);

u8 mnuHasStandardPartnerRequirements(BtlCommandId commandId);

extern void ptyDrawLinkageSkillEntry(s32, s32, s32, struct MenuList *, struct MenuListNode *, s32);

extern s32 btlAccessRecordedLinkageSkillFlag(s32 id, s32 operation);

extern u16 mnuPickPairedTableValue(s32 boundWordIndex, s32 alternate);

extern const u8 D_003BC708[];

extern void mnuAttachWindowTextureState(MenuWindowContainer *, u32, u32, u32, u32);

extern void mnuSortItems(MenuList *, s32, s32);

extern void mnuSetWindowFadeScale(MenuWindowContainer *, u32);

/* Build the linkage-skill window: every recorded linkage skill with an
 * standard partner requirements, flagged when the frontline party cannot form it,
 * plus blank rows for unrecorded skills with standard partner requirements. */
void ptySkillMenuBuildLinkageSkills(dds3ProcessID_t *task);

extern void mnuDestroyWindowContainer(MenuWindowContainer *window);

u32 mnuDestroySkillSelectionWindow(dds3ProcessID_t *task);

extern s32 mnuCampMenuInit(dds3ProcessID_t *task);

s32 mnuResetSelection(dds3ProcessID_t *selection);

s32 mnuCloseSkillSelection(dds3ProcessID_t *selection);

s32 mnuUpdateSkillListInput(dds3ProcessID_t *callback);

/* Sprite slots of the active skill-menu context; intervening state is opaque. */
typedef struct MenuContextSprites {
    u8 pad00[0x64];
    u32 sprite64;
    s32 label68;
    u32 frame6C;
    u8 pad70[4];
    u32 sprite74;
    u32 sprite78;
    u8 pad7C[0x64];
    u32 spriteE0;
    u32 spriteE4;
} MenuContextSprites;

void mnuDrawSkillMenuFrameIcons(MenuContextSprites *menuContext);

extern void effDrawSpriteWithCornerFade(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuDrawListFrames(s32 menu);

s32 mnuCampMenuDrawStatus(dds3ProcessID_t *callback);

/* Run the camp-status panel's phase-two control and return its result. */
s32 mnuStepCampStatusControl(dds3ProcessID_t *callback);

void mnuBindAssetEffectPayloads(MenuAssets *assets);

extern char D_003B2330[];

extern char D_003B2348[];

extern u32 mnuCampBackdropSpriteResourceNames[];

extern u32 mnuCampBackdropAnimationResourceNames[];

/* Load the base sprite banks synchronously, bind the payloads, and reset the fifth bank's saved corner colors. */
void mnuLoadBackdropAssetsAndTintSlots(MenuAssets *assets);

extern char D_003B2330[];

extern char D_003B2348[];

extern u32 mnuCampBackdropSpriteResourceNames[];

extern u32 mnuCampBackdropAnimationResourceNames[];

void mnuRequestBaseAssets(MenuAssets *assets);

extern s32 mnuInitializeCampAssetSprites(MenuAssets *);

s32 mnuInitializeCampAssetSprites(MenuAssets *assets);

void mnuReleaseAssets(MenuAssets *assets);

extern void uiDrawSurfaceAtNearDepth(u32);

extern void itfGridLookupValueOrDefault(s32, s32);

void mnuDrawCampBackdropDecoration(MenuAssets *assets, u32 drawArg);

void mnuDrawCursorIcons(MenuAssets *assets, s32 surface);

extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);

void mnuDrawBackdrop(MenuAssets *assets, s32 option);

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);

u32 mnuDestroyListState(MenuList *list);

void mnuUpdateListScrollFlags(MenuList *list);

MenuListNode *mnuListAppendNode();

s32 mnuListContainsFinalNode(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *, s32, s32);

MenuListNode *mnuListRetreatCursor(MenuList *, s32, s32);

MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
                         const void *text, s32 mode, u32 options);

MenuListNode *mnuRemoveListCursorNode(MenuList *list);

void mnuSetGridSpriteSlot(MenuListNode *node, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect);

struct MenuListNode *mnuWalkNodeList(s32 targetIndex, MenuList *list);

s32 mnuSeekListNode(s32 targetIndex, MenuList *list);

void mnuSelectFirstListNode(MenuList *list);

void mnuSelectLastListNode(MenuList *list);

/* Advance the visible head when a prior window offset can be reduced. */
MenuListNode *mnuAdvanceListWindowStart(MenuList *list);

/* Step the visible head back one node when a full window follows it. */
MenuListNode *mnuRetreatListWindowStart(MenuList *list);

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade);

MenuListNode *mnuAdvanceListCursorDefault(MenuList *list);

MenuListNode *mnuRetreatListCursorDefault(MenuList *list);

void mnuClearListFlagsOneAndTwo(u32 *flags);

u32 mnuTestListFlagTwo(u32 *flags);
/* End of declarations from src/dds1/game/code_00279CC0.c */

MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing) {
    MenuList *item = (MenuList *)sdfChipAllocZeroDirect(sizeof(MenuList));
    item->id = id;
    item->visibleCount = visibleCount;
    item->rowStep = rowSpacing * 8;
    item->scale = 0x100;
    item->head = NULL;
    item->first = NULL;
    item->windowOffset = 0;
    item->cursor = NULL;
    return item;
}

u32 mnuDestroyListState(MenuList *list) {
    MenuListNode *pending;

    do {
        pending = mnuRemoveListCursorNode(list);
    } while (pending != 0);
    sdfChipFreeDirect(list);
    return 1;
}

void mnuUpdateListScrollFlags(MenuList *list) {
    MenuListNode *node = list->head;
    s32 i;

    if (node == NULL) {
        list->flags = 0;
        return;
    }
    if (node->prev != NULL) {
        list->flags |= 1;
    } else {
        list->flags &= ~1;
    }
    for (i = 0; i < list->visibleCount; i++) {
        node = node->next;
        if (node == NULL) {
            list->flags &= ~2;
            return;
        }
    }
    list->flags |= 2;
}

INCLUDE_ASM(const s32, "code_003BC198", mnuListAppendNode);

s32 mnuListContainsFinalNode(MenuList *list) {
    MenuListNode *node = list->head;
    s32 index = 0;
    if (node != NULL) {
        s32 count = list->visibleCount;
        do {
            if (index >= count) {
                return 0;
            }
            if (node == list->last) {
                return 1;
            }
            node = node->next;
            index++;
        } while (node != NULL);
    }
    return 0;
}

MenuListNode *mnuInsertListNodeRelativeToAnchor(MenuList *list, MenuListNode *anchor,
                         const void *text, s32 mode, u32 options) {
    MenuListNode *node;
    MenuListNode *walk;

    if (list->count == 0 || anchor == NULL || anchor == list->last) {
        node = mnuListAppendNode(list, text);
        if (list->windowOffset >= list->visibleCount - 1 && list->cursor != list->last) {
            list->head = list->head->next;
            list->cursor = list->cursor->next;
            if (mode == -1) {
                mnuListRetreatCursor(list, 0, 1);
            }
        } else if (mode == -2) {
            mnuListAdvanceCursor(list, 0, 1);
        }
        return node;
    }

    node = sdfChipAllocZeroDirect(sizeof(MenuListNode));
    node->text = text;
    if (options & MNU_LIST_INSERT_AFTER_ANCHOR) {
        node->prev = anchor;
        node->index = anchor->index;
        node->next = anchor->next;
        if (anchor->next != NULL) {
            anchor->next->prev = node;
        }
        anchor->next = node;
        walk = node;
        do {
            walk->index++;
            walk = walk->next;
        } while (walk != NULL);
    } else {
        node->prev = anchor->prev;
        node->next = anchor;
        node->index = anchor->index;
        if (anchor->prev != NULL) {
            anchor->prev->next = node;
        }
        anchor->prev = node;
        if (anchor == list->first) {
            list->first = node;
            if (list->cursor == list->head || list->count < list->visibleCount) {
                list->head = node;
            }
        }
        walk = anchor;
        while (walk != NULL) {
            walk->index++;
            walk = walk->next;
        }
    }
    list->count++;
    if (node->index >= list->head->index && node->index < list->cursor->index) {
        list->cursor = list->cursor->prev;
        if (list->first == list->head) {
            if (mode >= 0 || mode == -2) {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else if (mode == -2) {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            } else {
                if (list->count >= list->visibleCount + 1 &&
                    list->cursor->index - list->head->index == list->visibleCount - 1) {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                } else {
                    mnuListAdvanceCursor(list, 0, 1);
                }
            }
        } else {
            if (mnuListContainsFinalNode(list)) {
                if (mode < 0 && mode != -2) {
                    list->head = list->head->next;
                }
            } else {
                if (mode >= 0 || mode == -2) {
                    if (mode == -2) {
                        list->head = list->head->next;
                        list->cursor = list->cursor->next;
                    }
                } else {
                    list->head = list->head->next;
                    list->cursor = list->cursor->next;
                }
            }
        }
    }
    mnuUpdateListScrollFlags(list);
    return node;
}

MenuListNode *mnuRemoveListCursorNode(MenuList *list) {
    MenuListNode *node;
    MenuListNode *cursor;
    MenuListNode *previous;
    MenuListNode *next;

    if (list->count == 0) {
        return NULL;
    }
    node = list->cursor;
    if (node == NULL) {
        return NULL;
    }
    cursor = node;
    do {
        if (node->index > 0) {
            node->index--;
        }
        node = node->next;
    } while (node != NULL);
    node = cursor;
    previous = node->prev;
    next = node->next;
    if (list->last->index - list->head->index + 1 <= list->visibleCount) {
        if (list->head != list->first) {
            cursor = previous;
            list->head = list->head->prev;
            list->cursor = previous;
        } else if (node == list->head) {
            if (next != NULL) {
                list->head = next;
                cursor = next;
                list->cursor = next;
            } else {
                cursor = previous;
                list->head = previous;
                list->cursor = previous;
                list->windowOffset--;
            }
        } else if (next != NULL) {
            list->cursor = next;
            cursor = next;
        } else {
            cursor = previous;
            list->cursor = previous;
            list->windowOffset--;
        }
    } else if (next != NULL) {
        list->cursor = next;
        cursor = next;
    }
    if (cursor == NULL) {
        list->head = NULL;
        list->first = NULL;
        list->last = NULL;
        list->windowOffset = 0;
    }
    if (previous != NULL) {
        previous->next = next;
    }
    if (next != NULL) {
        next->prev = previous;
    }
    if (previous == NULL) {
        list->head = next;
        list->first = next;
    }
    if (next == NULL) {
        list->last = previous;
    }
    sdfChipFreeDirect(node);
    list->count--;
    mnuUpdateListScrollFlags(list);
    return list->cursor;
}

INCLUDE_ASM(const s32, "code_003BC198", mnuSetGridSpriteSlot);

INCLUDE_ASM(const s32, "code_003BC198", mnuWalkNodeList);

INCLUDE_ASM(const s32, "code_003BC198", mnuSeekListNode);

INCLUDE_ASM(const s32, "code_003BC198", func_003BC9A8);

INCLUDE_ASM(const s32, "code_003BC198", func_003BC9C8);

/* Advance the visible head when a prior window offset can be reduced. */
MenuListNode *mnuAdvanceListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *last = list->last;
    MenuListNode *head = list->head;

    if (cursor == last) {
        return cursor;
    }
    head = head->next;
    if (head == NULL) {
        return cursor;
    }
    list->head = head;
    list->windowOffset--;
    return cursor;
}

/* Step the visible head back one node when a full window follows it. */
MenuListNode *mnuRetreatListWindowStart(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *head = list->head;
    MenuListNode *node;
    s32 i;

    if (cursor == list->first) {
        return cursor;
    }
    node = head;
    for (i = 0; i < list->visibleCount; i++) {
        if (node == NULL) {
            return cursor;
        }
        node = node->next;
    }
    head = head->prev;
    list->head = head;
    list->windowOffset++;
    return cursor;
}

MenuListNode *mnuListAdvanceCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *last;
    MenuListNode *next;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    last = list->last;
    if (cursor == last) {
        mnuSelectFirstListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        next = cursor->next;
        if (next == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = next;
        list->cursor = cursor;
        list->windowOffset = offset + 1;
        if (list->windowOffset >= list->visibleCount - 1) {
            if (noScroll == 0) {
                cursor = mnuAdvanceListWindowStart(list);
                last = list->last;
            } else if (cursor != last) {
                cursor = cursor->prev;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == last) {
            list->stateFlags |= 3;
        }
        if (cursor->next == last && (cursor->next->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

MenuListNode *mnuListRetreatCursor(MenuList *list, s32 noScroll, s32 keepFade) {
    s32 count = list->count;
    MenuListNode *cursor = list->cursor;
    MenuListNode *first;
    MenuListNode *prev;
    s32 offset;

    if (count < 2) {
        list->stateFlags |= 2;
    }
    if (list->stateFlags & 2) {
        list->stateFlags &= ~1;
        return NULL;
    }
    if (count == 1) {
        return cursor;
    }
    first = list->first;
    if (cursor == first) {
        mnuSelectLastListNode(list);
        mnuUpdateListScrollFlags(list);
        cursor = list->cursor;
    } else {
        if (cursor == NULL) {
            return NULL;
        }
        prev = cursor->prev;
        if (prev == NULL) {
            return NULL;
        }
        if (keepFade == 0) {
            cursor->animationTimer = 0x100;
        }
        offset = list->windowOffset;
        cursor = prev;
        list->cursor = cursor;
        list->windowOffset = offset - 1;
        if (list->windowOffset <= 0) {
            if (noScroll == 0) {
                cursor = mnuRetreatListWindowStart(list);
                first = list->first;
            } else if (cursor != first) {
                cursor = cursor->next;
                list->windowOffset = offset;
                list->cursor = cursor;
            }
        }
        if (cursor == first) {
            list->stateFlags |= 3;
        }
        if (cursor->prev == first && (cursor->prev->flags48 & 2)) {
            list->stateFlags |= 3;
        }
        mnuUpdateListScrollFlags(list);
    }
    return cursor;
}

MenuListNode *mnuAdvanceListCursorDefault(MenuList *list) {
    return mnuListAdvanceCursor(list, 0, 0);
}

MenuListNode *mnuRetreatListCursorDefault(MenuList *list) {
    return mnuListRetreatCursor(list, 0, 0);
}
