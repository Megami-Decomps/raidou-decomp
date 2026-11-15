#include "common.h"
#include "scr.h"
#include "facility.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "sdfChip.h"

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "sdk_libc.h"
#include "sdk_libgcc.h"
#include "dds/btl.h"


#include "dds/sdf_gs_register_submit_api.h"
#include "dds/sdf_gs_alpha_register_api.h"
#include "dds/itf_grid_text.h"

extern GridTextListItem *itfFindGridNodeByKey(u32, GridTextWidget *);
extern GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *);
void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);
void uiDrawSurfaceAtNearDepth(u32 surface);
void itfInitializeAdminInterfaceNoop(void);
u32 itfReturnZeroDuplicate(void);
u32 itfDestroyGridTextWidget(GridTextWidget *widget);
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget);

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *widget);
u32 itfGetGridListLinkFlags(GridTextWidget *owner);
GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value);
GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *widget);
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text);
void itfAdvanceGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps);
void itfAdvanceSelectedGridScroll(GridTextWidget *widget, u32 steps);
void itfReverseGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps);
void itfReverseSelectedGridScroll(GridTextWidget *widget, u32 steps);
GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget);

void uiDrawSurfaceAtNearDepth(u32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xe00, 0, surface);
}

INCLUDE_ASM(const s32, "code_002CD5A8", uiScaleColorRgb);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight)
{
    u32 a0 = firstColor >> 24;
    u32 r0 = (firstColor & 0xFF0000) >> 16;
    u32 g0 = (firstColor & 0xFF00) >> 8;
    u32 b0 = firstColor & 0xFF;
    u32 a1 = secondColor >> 24;
    u32 r1 = (secondColor & 0xFF0000) >> 16;
    u32 g1 = (secondColor & 0xFF00) >> 8;
    u32 b1 = secondColor & 0xFF;
    u32 inv;

    if (weight > 0x100) {
        weight = 0x200 - weight;
    }
    inv = 0x100 - weight;
    a0 = (a0 * weight + a1 * inv) >> 8;
    r0 = (r0 * weight + r1 * inv) >> 8;
    g0 = (g0 * weight + g1 * inv) >> 8;
    b0 = (b0 * weight + b1 * inv) >> 8;
    return (a0 << 24) | (r0 << 16) | (g0 << 8) | b0;
}

void fclLeaf12(void) {
}

void itfInitializeAdminInterfaceNoop(void) {
}

/* Separate native copy of itfReturnZero; no more specific caller role is established. */
u32 itfReturnZeroDuplicate(void) {
    return 0;
}

INCLUDE_ASM(const s32, "code_002CD5A8", itfActivateGridTextWidget);

INCLUDE_ASM(const s32, "code_002CD5A8", itfSetWidgetFlagsAndActivateChild);

INCLUDE_ASM(const s32, "code_002CD5A8", itfCreateGridTextWidget);

INCLUDE_ASM(const s32, "code_002CD5A8", itfSetGridDimensions);

u32 itfDestroyGridTextWidget(GridTextWidget *widget) {
    GridTextListItem *next;

    sdfChipFreeDirect(widget->text);
    do {
        next = itfRemoveSelectedGridTextItem(widget);
    } while (next != NULL);
    sdfChipFreeDirect(widget);
    return 1;
}

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget) {
    GridTextListItem *childLink;

    sdfChipFreeDirect(widget->text);
    childLink = widget->selected;
    if (childLink != NULL) {
        do {
            GridTextWidget *childWidget = childLink->child;
            if (childWidget != NULL) {
                itfDestroyGridTextWidgetTree(childWidget);
            }
            childLink = itfRemoveSelectedGridTextItem(widget);
        } while (childLink != NULL);
    }
    sdfChipFreeDirect(widget);
    return 1;
}

INCLUDE_ASM(const s32, "code_002CD5A8", itfExpandWidgetColumnWidth);

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(GridTextWidget *owner) {
    GridTextListItem *node = owner->firstVisible;
    u32 flags;
    s32 i;

    if (node == 0) {
        return 0;
    }
    flags = node->previous != 0;
    for (i = 0; i < (u16)owner->rows; i++) {
        node = node->next;
        if (node == 0) {
            flags &= ~2;
            return flags;
        }
    }
    flags |= 2;
    return flags;
}

GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value) {
    GridTextListItem *item = (GridTextListItem *)sdfChipAllocDirect(sizeof(GridTextListItem));
    GridTextListItem *tail;
    s32 length;
    s32 allocation;
    char *copy;

    memset(item, 0, 0x2C);
    if (owner->itemCount == 0) {
        owner->firstVisible = item;
        owner->selected = item;
        owner->head = item;
    }
    length = strlen(text);
    allocation = length + 1;
    copy = (char *)sdfChipAllocDirect(allocation);
    item->textLength = allocation;
    item->text = copy;
    memcpy(copy, text, allocation);
    itfExpandWidgetColumnWidth(length, owner);

    /* Initialize the node links, then append it after the current tail. */
    item->previous = owner->tail;
    item->next = 0;
    item->value = value;
    tail = owner->tail;
    item->numericDescriptor = NULL;
    item->previous = tail;
    if (tail != 0) {
        tail->next = item;
    }
    owner->tail = item;
    item->index = owner->itemCount;
    owner->itemCount++;
    return item;
}

INCLUDE_ASM(const s32, "code_002CD5A8", fclGohInitSkillChangeSub);

GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *widget) {
    GridTextListItem *node;
    GridTextListItem *cursor;
    GridTextListItem *previous;
    GridTextListItem *next;

    if (widget->itemCount == 0) {
        return NULL;
    }
    node = widget->selected;
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
    previous = node->previous;
    next = node->next;
    if (widget->tail->index - widget->firstVisible->index + 1 <= widget->rows) {
        if (widget->firstVisible != widget->head) {
            cursor = previous;
            widget->firstVisible = widget->firstVisible->previous;
            widget->selected = previous;
        } else if (node == widget->firstVisible) {
            if (next != NULL) {
                widget->firstVisible = next;
                cursor = next;
                widget->selected = next;
            } else {
                cursor = previous;
                widget->firstVisible = previous;
                widget->selected = previous;
                widget->cursorRow--;
            }
        } else if (next != NULL) {
            widget->selected = next;
            cursor = next;
        } else {
            cursor = previous;
            widget->selected = previous;
            widget->cursorRow--;
        }
    } else if (next != NULL) {
        widget->selected = next;
        cursor = next;
    }
    if (cursor == NULL) {
        widget->firstVisible = NULL;
        widget->head = NULL;
        widget->tail = NULL;
        widget->cursorRow = 0;
    }
    if (previous != NULL) {
        previous->next = next;
    }
    if (next != NULL) {
        next->previous = previous;
    }
    if (previous == NULL) {
        widget->firstVisible = next;
        widget->head = next;
    }
    if (next == NULL) {
        widget->tail = previous;
    }
    sdfChipFreeDirect(node->text);
    if (node->numericDescriptor != NULL) {
        sdfChipFreeDirect(node->numericDescriptor);
    }
    sdfChipFreeDirect(node);
    widget->itemCount--;
    return widget->selected;
}

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text) {
    s32 textLength;
    s32 textBytes;
    char *textCopy;

    sdfChipFreeDirect(item->text);
    textLength = strlen(text);
    textBytes = textLength + 1;
    textCopy = (char *)sdfChipAllocDirect(textBytes);
    item->textLength = textBytes;
    item->text = textCopy;
    memcpy(textCopy, text, textBytes);
    if (widget != NULL) {
        itfExpandWidgetColumnWidth(textLength, widget);
    }
}

INCLUDE_ASM(const s32, "code_002CD5A8", itfSetGridNumericItemDescriptor);

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
    range = entry->numericDescriptor;
    position = &entry->number;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous + delta;
    if (range->maximum < previous + delta) {
        *position = range->minimum;
    }
}

void itfAdvanceSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps) {
    GridNumericDescriptor *range;
    GridTextListItem *entry;
    float *position;
    float delta;
    float previous;

    entry = itfFindGridNodeByKey(key, widget);
    range = entry->numericDescriptor;
    position = &entry->number;
    delta = range->step;
    if (1 < steps) {
        delta = delta * (float)(s32)steps;
    }
    previous = *position;
    *position = previous - delta;
    if (previous - delta < range->minimum) {
        *position = range->maximum;
    }
}

void itfReverseSelectedGridScroll(GridTextWidget *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfReverseGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

INCLUDE_ASM(const s32, "code_002CD5A8", itfGetGridTextColorTableIndex);

INCLUDE_ASM(const s32, "code_002CD5A8", itfFormatGridValueEntryText);

INCLUDE_ASM(const s32, "code_002CD5A8", fclCallDrawBoxF);

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget) {
    GridTextListItem *item = widget->head;

    while (item != NULL && item->index != key) {
        item = item->next;
    }
    return item;
}

INCLUDE_ASM(const s32, "code_002CD5A8", sdfGridSeekSelectedNodeByIndex);

INCLUDE_ASM(const s32, "code_002CD5A8", fclLeaf13);
INCLUDE_SDATA(const s32, "code_002CD5A8", itfFormatGridValueEntryTextData);

INCLUDE_SDATA(const s32, "code_002CD5A8", itfFormatGridValueEntryTextData2);

INCLUDE_SDATA(const s32, "code_002CD5A8", itfFormatGridValueEntryTextData3);

INCLUDE_SDATA(const s32, "code_002CD5A8", itfFormatGridValueEntryTextData4);

