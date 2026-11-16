#ifndef FLDNME_H
#define FLDNME_H

#include "common.h"
#include "kernel.h"
#include "fileManager.h"
#include "newdata.h"
#include "frFont.h"

struct sprHandle_t;

/* Name entry drawing state at fldNmeWork_t+0xD0: glyph handles released with
 * frCallFlushGsFontTable, then the "name spr" sprite set. */
typedef struct fldNmeDraw_t {
    frFontHandle_t *glyph[90];  /* 0x000 */
    frFontHandle_t *glyph2[44]; /* 0x168 */
    u8 unk218[0x1C];
    struct sprHandle_t *hSpr;   /* 0x234 */
} fldNmeDraw_t;

typedef char fldNmeDrawSizeCheck[sizeof(fldNmeDraw_t) == 0x238 ? 1 : -1];

/* A slot list: count slots of 8 bytes, each free while id is -1. */
typedef struct fldNmeSlot_t {
    u16 id;
    u16 unk2;
    frFontHandle_t *font;
} fldNmeSlot_t;

typedef struct fldNmeSlotList_t {
    u16 count;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    fldNmeSlot_t *slot;
} fldNmeSlotList_t;

/* Work of the name entry process, 0x308 bytes (sdfMemAllocDirect). */
typedef struct fldNmeWork_t {
    s32 unk0;
    dds3ProcessID_t *drawProcess;
    u32 unk8;
    u32 unkC;
    s32 unk10;
    s32 unk14;
    fldNmeSlotList_t list[2];   /* 0x18 */
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    fldNmeSlot_t slot1[8];      /* 0x4C: slots of list[1] */
    fldNmeSlot_t slot0[8];      /* 0x8C: slots of list[0] */
    fileRequest_t *font;
    fldNmeDraw_t draw;          /* 0xD0 */
} fldNmeWork_t;

typedef char fldNmeWorkSizeCheck[sizeof(fldNmeWork_t) == 0x308 ? 1 : -1];

/* Setup of one slot list: its slots and their count, then the initial codes. */
typedef struct fldNmeListInit_t {
    fldNmeSlot_t *slot;
    s32 count;
    u16 *text;
    s32 length;
} fldNmeListInit_t;

void fldFunc20(fldNmeDraw_t *draw);
void fldLeaf29(fldNmeDraw_t *draw, s32 x, s32 y, s32 z, u32 alpha, s16 *list);
void SetSelect(fldNmeDraw_t *draw);
void fldLeaf32(fldNmeDraw_t *draw, s32 x, s32 y);
void fclLeaf15(fldNmeDraw_t *draw);

/* Sprite lists drawn with fldLeaf29. */
extern s16 fclLeaf15Data[];
extern s16 fclCallSetSelectData[];
extern s16 fclCallSetSelectData2[];
extern s16 fldLeaf30Data[];
extern s16 fldFunc24Data[];
extern s16 fldFunc24Data2[];
extern s16 fldFunc24Data3[];
extern s16 fldFunc24Data4[];
extern s16 fldFunc24Data5[];
extern s16 fldFunc24Data6[];
extern u16 nmeLeaf2Data[][15];
extern u16 nmeLeaf2Data2[][15];
extern u16 nmeLeaf2Data3[][15];
u16 nmeLeaf2(s32 page, s32 x, s32 y);
extern s16 fclFunc8Data[];
extern u8 *fclFunc8Data2[];
void fclFunc8(fldNmeDraw_t *draw, s32 row);
void fldCallEnableMode(s32 x, s32 y, s32 color, u8 *text);
void fclCallSetFontColorNo2(fldNmeSlotList_t *list, s32 color, f32 x, f32 y, f32 dx);
void fldLeaf33(fldNmeDraw_t *draw, s32 row, s32 column);

/* Character code table of the name entry grid, 0xC86 entries. */
extern u16 fclData[0xC86];
extern s32 effData45;
extern const char fldStrNameNameSpr[];

/* Code count of each of the 44 grid pages, and the first code index of each
 * page (fclWork[44] is the total). */
extern u16 fclFirstInitSubData[44];
extern u16 fclWork[45];


void fclFirstInitSub(void);
void fldFunc24(fldNmeDraw_t *draw, s32 arg1, s32 arg2);
void fldFunc25(fldNmeDraw_t *draw, s32 arg1);
void fldCallMemGetPtr9(dds3ProcessID_t *id);
void fldFunc23(s32 flag);
extern u8 *fldFunc23Data[];
void fclFunc11(fldNmeSlotList_t *list, dds3NameChar_t *name, u32 *length);
void fldCallMemGetPtr8(dds3ProcessID_t *id);
dds3ProcessID_t *fldNmeProcess(void);
void fclFunc10(fldNmeSlotList_t *list);
void fclFunc4(fldNmeDraw_t *draw);
void fldCallMemGetPtr5(dds3ProcessID_t *id);
void fclLeaf22(frFontHandle_t *font, u32 index, f32 x, f32 y);

/* Per-code draw offset (x, y) of the first 241 codes. */
extern s8 fclLeaf22Data[241][2];

extern s16 fldFunc25Data[];
extern s16 fldFunc25Data2[];
void fclLeaf25(fldNmeSlotList_t *list, fldNmeSlot_t *slot, s32 count, s32 unk4);

void fldNameNameSpr(fldNmeDraw_t *draw);

void fclLeaf20(fldNmeDraw_t *draw);
void fclFunc5(fldNmeDraw_t *draw);
void fldLeaf30(fldNmeDraw_t *draw, s32 index);
void fclCallSetSpriteNormal(fldNmeDraw_t *draw, u32 page, s32 index);
void fclLeaf17(fldNmeDraw_t *draw);
void fclLeaf18(fldNmeDraw_t *draw, s32 code);
s32 fclLeaf23(s32 page);
s32 fclFunc7(fldNmeDraw_t *draw, s32 code);
u16 fclSetSpriteNormal(s32 page, s32 index);
s32 titleStartSub(s32 page);
s32 titleStartSub2(s32 page);
s32 titleFunc(s32 page, s32 x, s32 y);
u32 titleFunc6(u32 code);
void titleFunc4(u32 code, s32 *page, s32 *index);
s32 titleFunc5(u32 code);
u32 fclLeaf24(s32 page, s32 index);
void fldLeaf31(fldNmeDraw_t *draw, s32 x, s32 y);
void fldFunc21(fldNmeDraw_t *draw, s32 x, s32 y);
void fldFunc22(s32 page, s32 x, s32 y, s32 color);
extern s16 fclCallSetFontColorNoData[];
void effFirstInitSub(void);
u16 fclLeaf21(u16 code);
u16 fclSetSpriteNormalSub(s32 index);
void fclCallSetFontString(fldNmeSlotList_t *list, u16 *text, u32 n);

#endif /* FLDNME_H */
void fclCallSetFontColorNo(fldNmeDraw_t *draw, u32 page, s32 index, s32 mode, u32 col, s32 row);
void fclFunc6(fldNmeDraw_t *draw, s32 mode, s32 index);
void fclCallSetSelect(fldNmeDraw_t *draw, s32 flag);
void fldCallMemGetPtr10(dds3ProcessID_t *id);
void fldCallMemGetPtr11(dds3ProcessID_t *id);
