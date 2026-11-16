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

#endif
