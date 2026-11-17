#include "common.h"
#include "sdfChip.h"
#include "cmbFont.h"

INCLUDE_ASM(const s32, "code_00196848", cmbRefDDS3GBWK);

void cmbCallChipAllocDirect(s32 idx, s32 count, dds3NameImage_t *image) {
    GsFont_t **cell = cmbData[idx];
    dds3NameImage_t *copy;
    GsFont_t *font;
    s32 i;

    for (i = 0; i < count; i++) {
        copy = sdfChipAllocDirect(sizeof(dds3NameImage_t));
        *copy = image[i];
        font = frTranslateUserData(24, 24, copy, 1);
        cell[i] = font;
        font->font_no = idx;
    }
}

void cmbFunc(s32 idx, s32 count, dds3NameImage_t *image) {
    s32 *ref = &cmbWork[idx];

    if (*ref == 0) {
        cmbCallChipAllocDirect(idx, count, image);
    }
    *ref += count;
}

void cmbRefDDS3GBWK2(s8 idx) {
    cmbFunc(idx, DDS3_GBWK->nameLength[idx], cmbRefDDS3GBWKWork[idx]);
}

void cmbCallRetFontMem(s32 idx) {
    GsFont_t **cell = cmbData[idx];
    s32 i;

    if (--cmbWork[idx] <= 0) {
        for (i = 0; i < cmbData2[idx] && cell[i] != NULL; i++) {
            fmRetFontMem(cell[i]->font_mem);
            cell[i] = NULL;
        }
    }
}

INCLUDE_ASM(const s32, "code_00196848", cmbCallSetUserImage);

void cmbRefDDS3GBWK3(s32 idx, frFontCtx_t *ctx) {
    cmbCallSetUserImage(idx, DDS3_GBWK->nameLength[idx], cmbRefDDS3GBWKWork2[idx], ctx);
}

INCLUDE_ASM(const s32, "code_00196848", cmbCallMemset);

INCLUDE_ASM(const s32, "code_00196848", cmbInitActionData);

INCLUDE_ASM(const s32, "code_00196848", cmbCallEnableMode);

INCLUDE_ASM(const s32, "code_00196848", cmbCallEnableMode2);

INCLUDE_ASM(const s32, "code_00196848", cmbCallEnableMode3);

INCLUDE_ASM(const s32, "code_00196848", cmbCallEnableMode4);

void nbCallDecodeFontImage(dds3NameImage_t *image, s32 style, u16 code) {
    u32 sjis = (((code & 0xFF) << 8) | (code >> 8)) - 0x8080;
    dds3NameImage_t *glyph;

    glyph = (dds3NameImage_t *)frDecodeFontImage(style, ((sjis & 0xFF00) >> 1) + (sjis & 0x7F));
    *image = *glyph;
    sdfChipFreeDirect(glyph);
}

void nbRefDDS3GBWK12(s32 idx, dds3NameChar_t *name, u32 length, s32 style) {
    dds3NameChar_t *code;
    dds3NameImage_t *image;

    u32 i;

    image = NULL;
    code = NULL;

    if (idx != 0) {
        if (idx == 1) {
            image = DDS3_GBWK->nameImage[1];
            code = DDS3_GBWK->nameCode[1];
        }
    } else {
        image = DDS3_GBWK->nameImage[0];
        code = DDS3_GBWK->nameCode[0];
    }
    for (i = 0; i < length; i++) {
        *code++ = *name;
        nbCallDecodeFontImage(image, style, name->code);
        image++;
        name++;
    }
    DDS3_GBWK->nameLength[idx] = length;
}

INCLUDE_ASM(const s32, "code_00196848", nbLeaf56);

INCLUDE_ASM(const s32, "code_00196848", nbRefDDS3GBWK13);

void nbSetFormPosRot(s32 idx, frFontCtx_t *ctx) {
    cmbRefDDS3GBWK2(idx);
    cmbRefDDS3GBWK3(idx, ctx);
}

INCLUDE_ASM(const s32, "code_00196848", nbCallSetFormPosRot);

s32 nbTag0Sub(s32 tag) {
    return 0;
}

s32 nbTag0Sub2(s32 tag) {
    return 1;
}

void nbRefDDS3GBWK14(void) {
    u32 i;
    nbData6 = DDS3_GBWK->nbNameImage;
    nbData7 = DDS3_GBWK->nbNameCode;
    for (i = 0; i < 16; i++) {
        nbWork3[i] = NULL;
    }
    nbData5 = 0;
}

INCLUDE_ASM(const s32, "code_00196848", nbCallChipAllocDirect);

INCLUDE_ASM(const s32, "code_00196848", nbFunc27);

INCLUDE_ASM(const s32, "code_00196848", nbRefDDS3GBWK15);

INCLUDE_ASM(const s32, "code_00196848", nbCallRetFontMem);

INCLUDE_ASM(const s32, "code_00196848", nbCallSetUserImage);

INCLUDE_ASM(const s32, "code_00196848", frRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00196848", frCallMemset3);

INCLUDE_ASM(const s32, "code_00196848", frCallMemset4);

INCLUDE_ASM(const s32, "code_00196848", frCallEnableMode2);

INCLUDE_ASM(const s32, "code_00196848", frCallEnableMode3);

INCLUDE_ASM(const s32, "code_00196848", frCallDecodeFontImage);

INCLUDE_ASM(const s32, "code_00196848", frRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_00196848", frTag0Sub);

s32 frLoadExchangeTable(void) {
    sdfDevLoadFileEx(frStrFontExchg01Bin, &frWork, &frLoadExchangeTableData);
    frLoadExchangeTableData >>= 1;
    return 1;
}

INCLUDE_RODATA(const s32, "code_00196848", frStrFontExchg01Bin);

