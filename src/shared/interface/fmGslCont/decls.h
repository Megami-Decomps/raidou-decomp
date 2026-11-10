/* Declarations of smt3-decomp src/smt3/interface/fmGslCont.c, for its bodies in this directory. */
#ifndef SHARED_INTERFACE_FMGSLCONT_H
#define SHARED_INTERFACE_FMGSLCONT_H

#include "font.h"

typedef struct FontGsControl_t FontGsControl_t;

struct FontGsControl_t {
    sdfMemHandle_t *mh;
    sdfMemHandle_t *ud_mh;
    s32 unit_width; /* 0x08 */
    s32 unit_height; /* 0x0C */
    s32 unit_size; /* 0x10 */
    s32 unit_num_max; /* 0x14 */
    u32 unit_nums; /* 0x18 */
    FontMem_t *root; /* 0x1C */
};

extern FontGsControl_t gFGC;

extern s8 fmFontMemInitialized;

extern u32 frSpaceAdvanceOffset;

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern u16 frTagCodeTable[];

extern u16 frTagCodeTable2[];

extern s8 D_00326061[];

extern u16 frMessageCode;

extern s32 frDefaultFontHandle;

extern s32 strlen(const char *s);

extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);

s32 fmDestroyFontMemControl(void);

FontGsControl_t *fmGetInternalFGC(void);

s32 fmGetRestBufNum(void);

/* Build the glyph atlas once: COLUMNS x ROWS cells in a circular node list, each with its
 * UV rectangle in 1/16-texel units and an optional RECORDBYTES-sized per-cell record. */
s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes);

/* Release the group's handles once and clear its active-node flag. */
s32 fmDestroyFontMemControl(void);

/* Unlink the first live node after the sentinel, or NULL when its slot is empty. */
FontMem_t *fmRentFontMem(void);

/* Insert after the list's fixed anchor and update the entry count. */
void fmRetFontMem(FontMem_t *pFontArea);

s32 frFindTagCode(u16 code);

s32 frFindTagCode2(u16 code);

void frGetMessageCode(u16 code);

void cmpSetSkillListRows(s32 rows);

s32 cmp_1C6180(const char *text, s32 width);

s32 func_001C61B8(frFontHandle_t *chain, s32 limit);

void fr_1C6270(frFontHandle_t *chain);

void func_001C62F8(u32 messageCode);

#endif
