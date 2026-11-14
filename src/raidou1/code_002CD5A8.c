#include "common.h"

/* Declarations from smt3-decomp src/smt3/facility/fclMisc.c */
#include "common.h"

#include "scr.h"

#include "facility.h"

#include "fpu.h"

#include "libsdf.h"

#include "newdata.h"

#include "ps2asm.h"

#include "event.h"

void fclFreeStage(void);

extern s32 dds3GetProcessStat(void *id);

extern u32 dds3GetRandA(void *name);

extern s32 fclSetWorkValue(s8 index, s32 value);

s32 fclGetFlag(s8 index);

void fclClearFlag(s8 index);

extern s8 gSelMsgPos;

extern s8 gMsgInvFlag;

extern s8 fclFlags[8];

extern s32 gMsgID;

extern s32 gSelMsgNo;

extern s8 fclEndMessageEx(s8 Mode);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void fclFilterOFF(void);

extern void sdfFileCmdReadFile(const char *aFileName, s32 aBuffer, s32 aLength, void *aCallback, void *aUserData);

extern void itfMesMngSetVarImmediate(s32 id, u32 index, s32 pstr);

extern s16 itfMesMngGetSelectNo(s32 msgid);

extern s16 itfMesMngGetSelPhase(s32 msgid);

extern s32 fclWorkValue[16];

extern s32 scrGetIntPara(s32 n);

extern void scrSetIntReturnValue(s32 i);

typedef struct
{
    u8 unk_00000[0x1928];
    u8 item[0xA0];
} fclGlobalWorkItemInventory_t;


extern s32 datAddItem(s32 id, s32 num);

extern s32 dds3KillProcess(s32 id, s32 shutdowntimer);

extern s32 fclChkMessage(void);

extern void fclScriptForcedEnd(u8 *PID);

extern u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s8 gMsgFlag;

extern void itfMesMngRelease(s32 id);

extern void *dds3GetControlScene(void);

extern void *dds3SceneBasicSearchNameAndType(void *scene, s32 type, char *name);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

extern void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern s32 cmpItemOrder(u8 *a, u8 *b);

extern s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

extern s32 datGetItemSlotIndex(s32 id);

extern void datAddHearts(s32 id);

s32 fclAddItem(u16 ItemID, s32 Nums);

typedef struct
{
    /* 0x00 */ u8 unk_00[0x18];
    /* 0x18 */ u8 *unk_18;
} fclSceneObj_t;

s32 fcl_213998(char *name, fclSceneObj_t *dst);

extern s32 dds3GetMapNo(void *scene);

extern void evtLoadArea(s32 fld, s32 area);

void fclLoadStage(s32 MajorNo, s32 MinorNo);

extern void dds3SetBlur(s32 frame, s32 alpha, s32 scale);

extern void dds3MultiBlurOff(s32 frame);

extern void dds3DistBlurOff(s32 frame);

extern void Eve_BlurOFF(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_FilterOFF(void);

void fclFilterOFF(void);

void fclFreeStage(void);

void fclInitPanelPos(s32 X, s32 Y, s32 Z, s16 W, s16 H, s32 Work, fclPanel_t *pPanelPos);

void fclScriptForcedEnd(u8 *PID);

s32 fclScriptStart(s32 pScriptData, s32 StartNo, void *pPID);

typedef struct
{
    /* 0x00 */ u8 count;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u16 item[0xA0];
} fclItemList_t;

void func_00213BB8(fclItemList_t *list);

extern s32 cmpItemHasFlag(u16 item, s8 which);

void fclCreateItemListEx(fclItemList_t *list, s8 which);

s32 cmpItemOrder(u8 *a, u8 *b);

s32 func_00213CD8(u8 *list, s32 count, u8 value);

void shuffleIndices(s8 *order, s32 count, s32 rounds);

extern char *text2[];

char *func_00213DC0(datUnitWork_t *unit);

u32 func_00213DF0(u32 color, u32 scaleFactor);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

extern s32 itfMesMngInitialize(void *data);

extern void itfMesMngChangeWindowType(s32 id, s32 type, s32 alpha);

s8 fclInitMessage(void *pData);

extern void itfMesMngMarkBitStat(s32 id, s32 flag);

extern void itfMesMngRequestMessage(s32 msg, s32 id, s32 flag);

extern void itfMesMngSetSelPhase(s32 msgid, s32 phase);

s32 fclStartMessage(s32 MsgNo);

s32 fclStartSelMessage(s32 SelMsgNo);

void fclSetSelMessagePos(s8 SelPos);

s8 fclGetCurrentSelMessagePos(void);

s8 fclGetSelMessagePos(void);

extern void itfMesMngSetPhase(s32 msgid, s32 phase);

extern void itfMesMngMsgWndCls(s32 msgid);

extern void itfMesMngRequestMessageRelease(s32 id, s32 cls_speaker);

extern void fclSetMessageInvalid(s8 Mode);

s8 fclEndMessageEx(s8 Mode);

s8 fclEndMessage(void);

s32 fclDestroyMessage(void);

s32 fclChkMessage(void);

s32 fclChkSelMessage(void);

extern s8 itfMesMngGetPhase(s32 msgid);

extern s32 itfMesMngSetSelectDefKey(s32 id, s32 key, s32 index);

extern void itfMesMngSetSelect(s32 id, s32 selno);

extern void itfMesMngRequestSelect(s32 id, s32 select);

void fclUpdateMessage(void);

void fclSetMessageVar_20743088(s8 Index, saddr_t pStr);

s8 fcl_214270(void);

extern void itfMesMngClearBitStat(s32 id, s32 flag);

void fclSetMessageInvalid(s8 Mode);

extern void itfMesMngSetMesPosition(s32 id, s32 x, s32 y);

extern void itfMesMngSetOffsetY1(s32 msgid, s32 y1);

void fcl_2142F0(s32 x, s32 y);

s32 fclChkMessageDraw(void *PID);

void fclClearFlag(s8 index);

s32 fclGetFlag(s8 index);

s32 fclSetWorkValue(s8 index, s32 value);

s8 fclGetWorkValue(s8 index);

s32 fcl_214428(void);

s32 fcl_214458(void);

s32 fclLoadFileDone(s32 unused, s32 cmd, s32 value, s32 unused2, s32 *out);

void fclLoadFileAsync(const char *name, void *buf);

extern s32 gMsgID;

extern s8 gMsgFlag;

extern s8 gMsgInvFlag;

extern s32 gSelMsgNo;

extern s8 gSelMsgPos;

extern char fclStrFee[4];

extern u32 D_0054FC68[2];

extern char fclStrOwn[4];

extern u32 D_0054FC78[2];

extern u32 D_0054FC80[2];

extern char fclStrTotal[6];

extern char fclStrMoney[6];

extern char fclStrNo[3];

extern char fclStrYes[4];
/* End of declarations from src/smt3/facility/fclMisc.c */

/* Declarations from dds-decomp src/dds1/game/code_002C1588.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_fclGlobalWorkItemInventory_t 1

#define SMT3_DEFINES_fclItemList_t 1

#define SMT3_DEFINES_fclSceneObj_t 1

#include "sdk_libc.h"

#include "sdk_libgcc.h"

#include "dds/btl.h"

#include "dds/sdf_chip.h"

#include "dds/sdk_libc.h"

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

char *strcpy(char *dst, const char *src);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/sdf_gs_register_submit_api.h"

#include "dds/sdf_gs_alpha_register_api.h"

#include "dds/sdf_gs_header.h"

#include "dds/common.h"

#include "dds/ui_color.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/fpu.h"

#include "dds/eff.h"

#include "dds/eff_resource_slots.h"

#include "dds/eff_resource_records.h"

#include "dds/itf_grid_text.h"

#include "dds/itf_grid_text_color_api.h"

#include "dds/sdf_grid_seek_api.h"

#include "dds/itf_draw_grid.h"

#include "dds/sdf.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_projection.h"

#include "dds/pcp_vu0.h"

extern GridTextListItem *itfFindGridNodeByKey(u32, GridTextWidget *);

extern GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *);

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

void uiDrawSurfaceAtNearDepth(u32 surface);

/* Scale packed RGB by a Q8 factor, preserving the low alpha byte. */
u32 uiScaleColorRgb(u32 color, u32 scaleFactor);

void itfInitializeAdminInterfaceNoop(void);

void itfShutdownAdminInterfaceNoop(void);

/* Separate native copy of itfReturnZero; no more specific caller role is established. */
u32 itfReturnZeroDuplicate(void);

s32 itfActivateGridTextWidget(GridTextWidget *widget);

/* Apply parent flags and enable bit 1 on the linked child widget, if present. */
s32 itfSetWidgetFlagsAndActivateChild(GridTextWidget *widget, u32 flags);

/* Own a NUL-terminated text copy; store X scaled by 16 and Y scaled by 8. */
GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                               u32 reference);

/* A zero column or row count leaves that dimension unchanged. */
void itfSetGridDimensions(GridTextWidget *widget, s32 columns, s32 rows);

u32 itfDestroyGridTextWidget(GridTextWidget *widget);

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget);

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *widget);

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(GridTextWidget *owner);

GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value);

GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *widget);

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text);

s32 itfSetGridNumericItemDescriptor(GridTextWidget *widget, GridTextListItem *item,
                  GridNumericDescriptor *descriptor);

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps);

void itfAdvanceSelectedGridScroll(GridTextWidget *widget, u32 steps);

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(GridTextWidget *widget, u32 key, s32 steps);

void itfReverseSelectedGridScroll(GridTextWidget *widget, u32 steps);

ItfGridTextColorTableIndex itfGetGridTextColorTableIndex(const GridTextWidget *widget,
                                                          const GridTextListItem *target);

/* Format a native value row, then optionally prefix its decimal/hex row number. */
void itfFormatGridValueEntryText(GridTextWidget *widget, GridTextListItem *entry, char *out);

/* Draw visible local-map rows and invoke the selected row callback once. */
void itfDrawGridTextRows(s32 offsetX, s32 offsetY, s32 z, GridTextWidget *widget,
                   s32 surfaceIndex);

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekSelectedNodeByIndex(s32 itemsToAdvance, GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekFirstNode(GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekLastNode(GridTextWidget *widget);
/* End of declarations from src/dds1/game/code_002C1588.c */

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

void func_r1_002CD700(void) {
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

INCLUDE_ASM(const s32, "code_002CD5A8", func_r1_002CDB50);

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

INCLUDE_ASM(const s32, "code_002CD5A8", func_r1_002CE320);

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget) {
    GridTextListItem *item = widget->head;

    while (item != NULL && item->index != key) {
        item = item->next;
    }
    return item;
}

INCLUDE_ASM(const s32, "code_002CD5A8", sdfGridSeekSelectedNodeByIndex);

INCLUDE_ASM(const s32, "code_002CD5A8", func_r1_002CE640);
INCLUDE_SDATA(const s32, "code_002CD5A8", D_r1_0046E7B8);

INCLUDE_SDATA(const s32, "code_002CD5A8", D_r1_0046E7C0);

INCLUDE_SDATA(const s32, "code_002CD5A8", D_r1_0046E7C8);

INCLUDE_SDATA(const s32, "code_002CD5A8", D_r1_0046E7D0);

