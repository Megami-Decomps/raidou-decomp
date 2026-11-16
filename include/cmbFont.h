#ifndef CMBFONT_H
#define CMBFONT_H

#include "common.h"
#include "font.h"
#include "frFont.h"
#include "newdata.h"

/* Name glyph cells: cmb holds the two player names, nb the 16 battle names. */
extern GsFont_t **cmbData[18];
extern u8 cmbData2[18];
extern s32 cmbWork[18];
extern dds3NameImage_t *cmbRefDDS3GBWKWork[2];
extern dds3NameChar_t *cmbRefDDS3GBWKWork2[2];
extern s32 nbWork3[16];
extern s32 nbData5;
extern dds3NameImage_t *nbData6;
extern dds3NameChar_t *nbData7;
extern u32 frWork;
extern u32 frLoadExchangeTableData;
extern const char frStrFontExchg01Bin[];

void cmbRefDDS3GBWK(void);
void cmbCallChipAllocDirect(s32 idx, s32 count, dds3NameImage_t *image);
void cmbFunc(s32 idx, s32 count, dds3NameImage_t *image);
void cmbRefDDS3GBWK2(s8 idx);
void cmbCallRetFontMem(s32 idx);
void cmbCallSetUserImage(s32 idx, u32 count, dds3NameChar_t *name, frFontCtx_t *ctx);
void cmbRefDDS3GBWK3(s32 idx, frFontCtx_t *ctx);
void nbCallDecodeFontImage(dds3NameImage_t *image, s32 style, u16 code);
void nbRefDDS3GBWK12(s32 idx, dds3NameChar_t *name, u32 length, s32 style);
void nbSetFormPosRot(s32 idx, frFontCtx_t *ctx);
void nbCallSetFormPosRot(s32 tag, frFontCtx_t *ctx);
s32 nbTag0Sub(s32 tag);
s32 nbTag0Sub2(s32 tag);
void nbRefDDS3GBWK14(void);
s32 frLoadExchangeTable(void);
void fldFontNameEnt(void);

#endif
