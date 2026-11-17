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

#include "sdk_libc.h"
#include "sdk_libgcc.h"
#include "include_asm.h"
#include "code_r1_002C8188.h"
#include "code_r1_002CD5A8.h"

/* Append a TEST-register write to the active GS packet stream. */
void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

extern itfGridTextListItem_t *itfFindGridNodeByKey(u32, itfGridTextWidget_t *);
extern itfGridTextListItem_t *itfRemoveSelectedGridTextItem(itfGridTextWidget_t *);
void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);
void uiDrawSurfaceAtNearDepth(u32 surface);
void itfInitializeAdminInterfaceNoop(void);
u32 itfReturnZeroDuplicate(void);
u32 itfDestroyGridTextWidget(itfGridTextWidget_t *widget);
u32 itfDestroyGridTextWidgetTree(itfGridTextWidget_t *widget);

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, itfGridTextWidget_t *widget);
u32 itfGetGridListLinkFlags(itfGridTextWidget_t *owner);
itfGridTextListItem_t *itfAppendGridTextItem(itfGridTextWidget_t *owner, const char *text, u32 value);
itfGridTextListItem_t *itfRemoveSelectedGridTextItem(itfGridTextWidget_t *widget);
void itfReplaceGridTextAndExpandColumn(itfGridTextWidget_t *widget, itfGridTextListItem_t *item, const char *text);
void itfAdvanceGridScrollPosition(itfGridTextWidget_t *widget, u32 key, s32 steps);
void itfAdvanceSelectedGridScroll(itfGridTextWidget_t *widget, u32 steps);
void itfReverseGridScrollPosition(itfGridTextWidget_t *widget, u32 key, s32 steps);
void itfReverseSelectedGridScroll(itfGridTextWidget_t *widget, u32 steps);
itfGridTextListItem_t *itfFindGridNodeByKey(u32 key, itfGridTextWidget_t *widget);

void uiDrawSurfaceAtNearDepth(u32 surface) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, surface);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, surface);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xe00, 0, surface);
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:35, match, same function 1.00 (words 19/19, calls 1.00, shape 1.00) */
u32 uiScaleColorRgb(u32 color, u32 scaleFactor) {
    u32 red = color >> 24;
    u32 green = (color >> 16) & 0xFF;
    u32 blue = (color & 0xFF00) >> 8;
    u32 alpha = color & 0xFF;
    red = (red * scaleFactor) >> 8;
    green = (green * scaleFactor) >> 8;
    blue = (blue * scaleFactor) >> 8;
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

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

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:81, rework, same function 1.00 (words 11/11, calls 1.00, shape 1.00) */
s32 itfActivateGridTextWidget(itfGridTextWidget_t *widget) {
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & -2) | 4;
    return 1;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:90, rework, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
s32 itfSetWidgetFlagsAndActivateChild(itfGridTextWidget_t *widget, u32 flags) {
    itfGridTextListItem_t *childLink;
    if (widget == 0) {
        return 0;
    }
    widget->flags = (widget->flags & ~4) | flags;
    childLink = widget->selected;
    if (childLink != 0) {
        itfGridTextWidget_t *childWidget = childLink->child;
        if (childWidget != 0) {
            childWidget->flags |= 6;
        }
    }
    return 1;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:108, rework, same function 1.00 (words 69/69, calls 1.00, shape 1.00) */
itfGridTextWidget_t *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                               u32 reference) {
    itfGridTextWidget_t *widget = (itfGridTextWidget_t *)sdfChipAllocDirect(sizeof(itfGridTextWidget_t));
    u32 textBytes;
    char *textCopy;

    memset(widget, 0, sizeof(itfGridTextWidget_t));
    textBytes = strlen(text) + 1;
    textCopy = (char *)sdfChipAllocDirect(textBytes);
    widget->textLength = textBytes;
    widget->text = textCopy;
    memcpy(textCopy, text, textBytes);
    widget->firstVisible = NULL;
    widget->x = x << 4;
    widget->y = y << 3;
    widget->reference = reference;
    widget->rows = rows;
    widget->width = columns * 12 + 6;
    widget->height = rows * 14 + 6;
    widget->head = NULL;
    widget->selected = NULL;
    widget->tail = NULL;
    widget->flags = 0;
    widget->cursorRow = 0;
    widget->itemCount = 0;
    return widget;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:137, rework, same function 1.00 (words 16/16, calls 1.00, shape 1.00) */
void itfSetGridDimensions(itfGridTextWidget_t *widget, s32 columns, s32 rows) {
    s32 columnWidth = columns * 12 + 6;
    s32 rowHeight = rows * 14 + 6;

    if (columns != 0) {
        widget->width = columnWidth;
    }
    if (rows != 0) {
        widget->height = rowHeight;
        widget->rows = rows;
    }
}

u32 itfDestroyGridTextWidget(itfGridTextWidget_t *widget) {
    itfGridTextListItem_t *next;

    sdfChipFreeDirect(widget->text);
    do {
        next = itfRemoveSelectedGridTextItem(widget);
    } while (next != NULL);
    sdfChipFreeDirect(widget);
    return 1;
}

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(itfGridTextWidget_t *widget) {
    itfGridTextListItem_t *childLink;

    sdfChipFreeDirect(widget->text);
    childLink = widget->selected;
    if (childLink != NULL) {
        do {
            itfGridTextWidget_t *childWidget = childLink->child;
            if (childWidget != NULL) {
                itfDestroyGridTextWidgetTree(childWidget);
            }
            childLink = itfRemoveSelectedGridTextItem(widget);
        } while (childLink != NULL);
    }
    sdfChipFreeDirect(widget);
    return 1;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:181, rework, same function 1.00 (words 18/18, calls 1.00, shape 1.00) */
void itfExpandWidgetColumnWidth(s32 columns, itfGridTextWidget_t *widget) {
    s32 flags = widget->flags;
    s32 requiredWidth;
    if (flags & 0x200) {
        columns += 4;
        if (flags & 0x400) {
            columns += 2;
        }
    }
    requiredWidth = columns * 12 + 6;
    if (widget->width < requiredWidth) {
        widget->width = requiredWidth;
    }
}

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(itfGridTextWidget_t *owner) {
    itfGridTextListItem_t *node = owner->firstVisible;
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

itfGridTextListItem_t *itfAppendGridTextItem(itfGridTextWidget_t *owner, const char *text, u32 value) {
    itfGridTextListItem_t *item = (itfGridTextListItem_t *)sdfChipAllocDirect(sizeof(itfGridTextListItem_t));
    itfGridTextListItem_t *tail;
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

INCLUDE_ASM(const s32, "code_r1_002CD5A8", fclGohInitSkillChangeSub);

itfGridTextListItem_t *itfRemoveSelectedGridTextItem(itfGridTextWidget_t *widget) {
    itfGridTextListItem_t *node;
    itfGridTextListItem_t *cursor;
    itfGridTextListItem_t *previous;
    itfGridTextListItem_t *next;

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
void itfReplaceGridTextAndExpandColumn(itfGridTextWidget_t *widget, itfGridTextListItem_t *item, const char *text) {
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

#ifdef VERSION_RAIDOU1
/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:351, rework, same function 1.00 (words 103/103, calls 1.00, shape 1.00) */
s32 itfSetGridNumericItemDescriptor(itfGridTextWidget_t *widget, itfGridTextListItem_t *item,
                  itfGridNumericDescriptor_t *descriptor) {
    itfGridNumericDescriptor_t *copy;
    f32 maximum;
    s32 width = 1;
    s32 length;

    copy = (itfGridNumericDescriptor_t *)sdfChipAllocDirect(sizeof(itfGridNumericDescriptor_t));
    item->numericDescriptor = copy;
    memcpy(copy, descriptor, sizeof(itfGridNumericDescriptor_t));
    item->number = descriptor->value;
    length = strlen(item->text);
    maximum = item->numericDescriptor->maximum;

    switch (descriptor->mode) {
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            while (maximum >= 16.0f) {
                maximum *= 0.0625f;
                width++;
            }
            width += 2;
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            width += 2;
            break;
        default:
            while (maximum >= 10.0f) {
                maximum /= 10.0f;
                width++;
            }
            break;
    }

    item->formatWidth = width;
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        length += width;
    } else {
        length += width + 1;
    }
    itfExpandWidgetColumnWidth(length, widget);
    return 1;
}
#else
INCLUDE_ASM(const s32, "code_r1_002CD5A8", itfSetGridNumericItemDescriptor);
#endif

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(itfGridTextWidget_t *widget, u32 key, s32 steps) {
    itfGridNumericDescriptor_t *range;
    itfGridTextListItem_t *entry;
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

void itfAdvanceSelectedGridScroll(itfGridTextWidget_t *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfAdvanceGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(itfGridTextWidget_t *widget, u32 key, s32 steps) {
    itfGridNumericDescriptor_t *range;
    itfGridTextListItem_t *entry;
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

void itfReverseSelectedGridScroll(itfGridTextWidget_t *widget, u32 steps) {
    if ((widget->flags & 1) != 0) {
        itfReverseGridScrollPosition(widget, (u32)widget->selected->index + widget->rowOffset, steps);
        return;
    }
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:456, rework, same function 1.00 (words 28/28, calls 1.00, shape 1.00) */
ItfGridTextColorTableIndex itfGetGridTextColorTableIndex(const itfGridTextWidget_t *widget,
                                                         const itfGridTextListItem_t *target) {
    u32 flags = widget->flags;

    if (flags & 4) {
        if (target == widget->selected) {
            return (flags & 1) ? 6 : 4;
        }
        return 0;
    }
    if (flags & 0x100) {
        if (target == widget->selected) {
            return 12;
        }
    } else if (target == widget->selected && (flags & 1)) {
        return 6;
    }
    return 0;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:482, rework, same function 1.00 (words 125/125, calls 1.00, shape 1.00) */
void itfFormatGridValueEntryText(itfGridTextWidget_t *widget, itfGridTextListItem_t *entry, char *out) {
    char text[0x100];
    char prefix[0x100];
    char format[0x100];

    if (entry->format != NULL) {
        entry->format(widget, entry, text, 0x100);
    } else if (entry->numericDescriptor == NULL) {
        sprintf(text, "%s", entry->text);
    } else {
        if (strlen(entry->text) == 0) {
            strcpy(prefix, "");
        } else {
            sprintf(prefix, "%s ", entry->text);
        }
        switch (entry->numericDescriptor->mode) {
        case GRID_NUMERIC_FORMAT_DECIMAL:
            sprintf(format, "%%s%%0%dd", entry->formatWidth);
            sprintf(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_HEXADECIMAL:
            sprintf(format, "%%s0x%%0%dX", entry->formatWidth - 2);
            sprintf(text, format, prefix, (s32)entry->number);
            break;
        case GRID_NUMERIC_FORMAT_FLOAT:
            sprintf(format, "%%s%%0%d.1f", entry->formatWidth);
            sprintf(text, format, prefix, (double)entry->number);
            break;
        }
    }
    if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
        s32 row = entry->index + widget->rowOffset;

        if (!(widget->flags & GRID_TEXT_HEX_ROW_INDEX)) {
            sprintf(out, "%03d:%s", row, text);
        } else {
            sprintf(out, "0x%03X:%s", row, text);
        }
    } else {
        strcpy(out, text);
    }
}

void fclCallDrawBoxF(s32 offsetX, s32 offsetY, s32 z, itfGridTextWidget_t *widget,
                     s32 surfaceIndex) {
    char text[0x100];
    itfGridTextListItem_t *item;
    itfGridTextListItem_t *selected;
    itfGridTextWidget_t *child;
    ItfGridTextColorTableIndex colorTableIndex;
    s32 rowEnd;
    u32 column;
    u32 flags;
    s32 itemIndex = 0;
    s32 invokeSelected = 1;

    offsetY = widget->y + offsetY + 0x10;
    rowEnd = offsetY + ((widget->height << 3) + widget->y) - 0x6C;
    offsetX = widget->x + offsetX + 0x20;
    item = widget->firstVisible;
    if (item != NULL) {
        if (offsetY < rowEnd) {
            if (widget->rows != 0) {
                do {
                    colorTableIndex = itfGetGridTextColorTableIndex(widget, item);
                    if (!(widget->flags & GRID_TEXT_HIDE_ROWS)) {
                        if (item == widget->selected && (widget->flags & GRID_TEXT_HIGHLIGHT_SELECTION)) {
                            column = (s32)item->number;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                column += 4;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
                                    column += 2;
                                }
                            }
                            column = (column * 3) << 6;
                            uiDrawUniformColorRect(offsetX + column, offsetY, z, 0xD0, 0x78, 0x40408080,
                                                   surfaceIndex);
                        }
                        if ((widget->flags & GRID_TEXT_HIGHLIGHT_VALUES) && item->value != 0) {
                            s32 width = strlen(item->text) * 0xC0 + 0x10;
                            s32 indent;

                            if (widget->flags & GRID_TEXT_PREFIX_ROW_INDEX) {
                                indent = 0x300;
                                if (widget->flags & GRID_TEXT_HEX_ROW_INDEX) {
                                    indent = 0x480;
                                }
                            } else {
                                indent = 0;
                            }
                            uiDrawUniformColorRect(offsetX + indent, offsetY, z, width, 0x78, 0x40408080,
                                                   surfaceIndex);
                        }
                        itfFormatGridValueEntryText(widget, item, text);
                        fldLmapSubmitPositionedCommandPacket(offsetX, offsetY, z, colorTableIndex, text,
                                                             surfaceIndex);
                    }
                    flags = widget->flags;
                    selected = widget->selected;
                    child = selected->child;
                    if (child != NULL) {
                        if (child->flags & 4) {
                            invokeSelected = 0;
                        }
                        if (child->flags & 1) {
                            invokeSelected = 0;
                        }
                    }
                    widget->flags = flags & ~2;
                    if (flags & 2) {
                        invokeSelected = 0;
                    }
                    if (item->select != NULL && item == selected && invokeSelected != 0) {
                        invokeSelected = 0;
                        item->select(widget);
                    }
                    offsetY += 0x70;
                    item = item->next;
                    itemIndex++;
                    if (item == NULL) {
                        break;
                    }
                    if (offsetY >= rowEnd) {
                        break;
                    }
                } while (itemIndex < widget->rows);
            }
        }
    }
}

itfGridTextListItem_t *itfFindGridNodeByKey(u32 key, itfGridTextWidget_t *widget) {
    itfGridTextListItem_t *item = widget->head;

    while (item != NULL && item->index != key) {
        item = item->next;
    }
    return item;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_002C1588.c:615, rework, same function 1.00 (words 29/29, calls 1.00, shape 1.00) */
s32 sdfGridSeekSelectedNodeByIndex(s32 itemsToAdvance, itfGridTextWidget_t *widget) {
    s16 count = widget->itemCount;
    u16 width;
    itfGridTextListItem_t *first;

    if (itemsToAdvance >= count) {
        return SDF_GRID_SEEK_RESULT_ITEMS_EXHAUSTED;
    }
    first = widget->head;
    widget->cursorRow = 0;
    widget->firstVisible = first;
    widget->selected = first;
    if (itemsToAdvance > 0) {
        width = (u16)widget->rows;
        do {
            itfGridTextListItem_t *current = widget->firstVisible;
            if (width >= count - current->index) {
                widget->cursorRow++;
            } else {
                widget->firstVisible = current->next;
            }
            current = widget->selected;
            widget->selected = current->next;
        } while (--itemsToAdvance != 0);
    }
    return SDF_GRID_SEEK_RESULT_SEEK_APPLIED;
}

s32 fclLeaf13(itfGridTextWidget_t *widget) {
    return sdfGridSeekSelectedNodeByIndex(0, widget);
}
