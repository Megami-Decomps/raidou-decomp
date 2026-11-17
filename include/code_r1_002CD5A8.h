#ifndef CODE_R1_002_CD5_A8_H
#define CODE_R1_002_CD5_A8_H

#include "common.h"

/* Complete 0x14-byte descriptor copied into a numeric item's parameter. */
typedef struct itfGridNumericDescriptor_t {
    s32 mode;       /* 0x00 */
    f32 minimum;    /* 0x04 */
    f32 maximum;    /* 0x08 */
    f32 step;       /* 0x0C */
    f32 value;      /* 0x10 */
} itfGridNumericDescriptor_t;

typedef char itfGridNumericDescriptorSizeCheck[sizeof(itfGridNumericDescriptor_t) == 0x14 ? 1 : -1];

/* Numeric item formatting modes stored in itfGridNumericDescriptor_t.mode. */
enum {
    GRID_NUMERIC_FORMAT_DECIMAL = 0,
    GRID_NUMERIC_FORMAT_HEXADECIMAL = 1,
    GRID_NUMERIC_FORMAT_FLOAT = 2 /* One fractional digit. */
};

/* Display behavior in itfGridTextWidget_t.flags: DDS's bits from 0x2 up, shifted one place. */
enum {
    GRID_TEXT_HIGHLIGHT_SELECTION = 0x8,
    GRID_TEXT_HIGHLIGHT_VALUES = 0x10,
    GRID_TEXT_HIDE_ROWS = 0x80,
    GRID_TEXT_PREFIX_ROW_INDEX = 0x200,
    GRID_TEXT_HEX_ROW_INDEX = 0x400
};

typedef s32 SdfGridSeekResult;

enum {
    SDF_GRID_SEEK_RESULT_ITEMS_EXHAUSTED = 0,
    SDF_GRID_SEEK_RESULT_SEEK_APPLIED = 1
};

typedef struct itfGridTextWidget_t itfGridTextWidget_t;

typedef struct itfGridTextListItem_t itfGridTextListItem_t;

/* Shared grid and local-map list owner. */
struct itfGridTextWidget_t {
    char *text;                     /* 0x00 */
    u16 textLength;                 /* 0x04 */
    u16 rows;                       /* 0x06 */
    s16 cursorRow;                  /* 0x08 */
    s16 itemCount;                  /* 0x0A */
    u32 flags;                      /* 0x0C */
    itfGridTextListItem_t *firstVisible; /* 0x10 */
    itfGridTextListItem_t *head;         /* 0x14 */
    itfGridTextListItem_t *selected;     /* 0x18 */
    itfGridTextListItem_t *tail;         /* 0x1C */
    s32 x;                          /* 0x20 */
    s32 y;                          /* 0x24 */
    s32 width;                      /* 0x28 */
    s32 height;                     /* 0x2C */
    u32 reference;                  /* 0x30 */
    void (*onSelect)(itfGridTextWidget_t *); /* 0x34 */
    void (*onDraw)(s32, s32, s32, itfGridTextWidget_t *, s32); /* 0x38 */
    s32 rowOffset;                  /* 0x3C */
    u32 unk40;                      /* 0x40: cleared by itfCreateGridTextWidget, no reader found */
};

/* Text rows share this 0x2C-byte node layout in both renderers. */
struct itfGridTextListItem_t {
    char *text;                     /* 0x00 */
    u16 textLength;                 /* 0x04 */
    u16 index;                      /* 0x06 */
    itfGridNumericDescriptor_t *numericDescriptor; /* 0x08; NULL for nonnumeric rows. */
    f32 number;                     /* 0x0C */
    s32 formatWidth;                /* 0x10 */
    u32 value;                      /* 0x14 */
    itfGridTextListItem_t *previous;     /* 0x18 */
    itfGridTextListItem_t *next;         /* 0x1C */
    itfGridTextWidget_t *child;          /* 0x20 */
    void (*select)(itfGridTextWidget_t *); /* 0x24 */
    void (*format)(void *, void *, char *, s32); /* 0x28 */
};

typedef char itfGridTextWidgetSizeCheck[sizeof(itfGridTextWidget_t) == 0x44 ? 1 : -1];

typedef char itfGridTextListItemSizeCheck[sizeof(itfGridTextListItem_t) == 0x2C ? 1 : -1];

void fclCallDrawBoxF(s32 offsetX, s32 offsetY, s32 z, itfGridTextWidget_t *widget, s32 surfaceIndex);
u32 itfGetGridListLinkFlags(itfGridTextWidget_t *owner);
void fldLocalMapDrawScrollIndicators(s32 offsetX, s32 offsetY, s32 z, u32 color, itfGridTextWidget_t *list,
                                     s32 channel);
/* fldLmapDrawListTree's recursion depth and the last list it drew. */
extern s32 fldLmapDrawListTreeData;
extern itfGridTextWidget_t *fldLmapDrawListTreeWork;
u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);
s32 sdfGridSeekSelectedNodeByIndex(s32 index, itfGridTextWidget_t *widget);

/* Index consumed by sdfDevSetPrintCB and resolved through sdfDevConsColorTable. */
typedef s32 ItfGridTextColorTableIndex;

ItfGridTextColorTableIndex itfGetGridTextColorTableIndex(const itfGridTextWidget_t *widget,
                                                         const itfGridTextListItem_t *target);
void itfFormatGridValueEntryText(itfGridTextWidget_t *widget, itfGridTextListItem_t *entry, char *out);
void fldLmapSubmitPositionedCommandPacket(s32 x, s32 y, s32 z, ItfGridTextColorTableIndex colorTableIndex,
                                          const char *command, s32 surfaceIndex);

#endif
