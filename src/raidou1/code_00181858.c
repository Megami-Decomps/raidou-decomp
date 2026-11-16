#include "common.h"
#include "include_asm.h"
#include "font.h"
#include "frFont.h"
#include "newdata.h"
#include "sdfChip.h"
#include "sdf_resource.h"
#include "cmbFont.h"

/* NON_MATCHING: 45 of 52 words match. Store order of the four name pointers and the loop init scheduling differ. */
#ifdef NON_MATCHING
void cmbRefDDS3GBWK(void) {
    u32 i;
    u32 j;
    s32 k;

    cmbRefDDS3GBWKWork[1] = DDS3_GBWK->nameImage[1];
    cmbRefDDS3GBWKWork[0] = DDS3_GBWK->nameImage[0];
    cmbRefDDS3GBWKWork2[1] = DDS3_GBWK->nameCode[1];
    cmbRefDDS3GBWKWork2[0] = DDS3_GBWK->nameCode[0];
    for (i = 0; i < 18; i++) {
        for (j = 0; j < cmbData2[i]; j++) {
            cmbData[i][j] = NULL;
        }
    }
    for (k = 17; k >= 0; k--) {
        cmbWork[k] = 0;
    }
    fldFontNameEnt();
}
#else
INCLUDE_ASM(const s32, "code_00181858", cmbRefDDS3GBWK);
#endif

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

/* NON_MATCHING: 64 of 67 words match. Retail computes the font number before the 0x83C5 branch and stores glyphCode before it. */
#ifdef NON_MATCHING
void cmbCallSetUserImage(s32 idx, u32 count, dds3NameChar_t *name, FrFontCtx *ctx) {
    GsFont_t **cell = cmbData[idx];
    u32 i;
    u32 code;
    frTextHandle_t *text;

    for (i = 0; i < count; i++) {
        code = (name[i].bytes[0] << 8) | name[i].bytes[1];
        ctx->glyphChain = frSetUserImage(cell[i], ctx->firstOption, ctx->glyphChain);
        text = ctx->glyphChain->lastChild;
        text->glyphCodeOrContext.glyphCode = code;
        if (code < 0x83C6) {
            code = (((code - 0x8080) & 0xFF00) >> 1) + ((code - 0x8080) & 0x7F);
            frSetFontSub2(text, code);
            ctx->glyphChain->lastChild->y = frSetFontSub(0, code);
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_00181858", cmbCallSetUserImage);
#endif

void cmbRefDDS3GBWK3(s32 idx, FrFontCtx *ctx) {
    cmbCallSetUserImage(idx, DDS3_GBWK->nameLength[idx], cmbRefDDS3GBWKWork2[idx], ctx);
}

INCLUDE_ASM(const s32, "code_00181858", cmbCallMemset);

INCLUDE_ASM(const s32, "code_00181858", cmbInitActionData);

INCLUDE_ASM(const s32, "code_00181858", cmbCallEnableMode);

INCLUDE_ASM(const s32, "code_00181858", cmbCallEnableMode2);

INCLUDE_ASM(const s32, "code_00181858", cmbCallEnableMode3);

INCLUDE_ASM(const s32, "code_00181858", cmbCallEnableMode4);

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

INCLUDE_ASM(const s32, "code_00181858", nbLeaf56);

INCLUDE_ASM(const s32, "code_00181858", nbRefDDS3GBWK13);

void nbSetFormPosRot(s32 idx, FrFontCtx *ctx) {
    cmbRefDDS3GBWK2(idx);
    cmbRefDDS3GBWK3(idx, ctx);
}

void nbCallSetFormPosRot(s32 tag, FrFontCtx *ctx) {
    nbSetFormPosRot(nbTag0Sub(tag), ctx);
    ctx->glyphChain->advance += 8;
    ctx->glyphChain->lastChild->advance += 8;
    nbSetFormPosRot(nbTag0Sub2(tag), ctx);
}

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
        nbWork3[i] = 0;
    }
    nbData5 = 0;
}

INCLUDE_ASM(const s32, "code_00181858", nbCallChipAllocDirect);

INCLUDE_ASM(const s32, "code_00181858", nbFunc27);

INCLUDE_ASM(const s32, "code_00181858", nbRefDDS3GBWK15);

INCLUDE_ASM(const s32, "code_00181858", nbCallRetFontMem);

INCLUDE_ASM(const s32, "code_00181858", nbCallSetUserImage);

INCLUDE_ASM(const s32, "code_00181858", frRefDDS3GBWK);

INCLUDE_ASM(const s32, "code_00181858", frCallMemset3);

INCLUDE_ASM(const s32, "code_00181858", frCallMemset4);

INCLUDE_ASM(const s32, "code_00181858", frCallEnableMode2);

INCLUDE_ASM(const s32, "code_00181858", frCallEnableMode3);

INCLUDE_ASM(const s32, "code_00181858", frCallDecodeFontImage);

INCLUDE_ASM(const s32, "code_00181858", frRefDDS3GBWK2);

INCLUDE_ASM(const s32, "code_00181858", frTag0Sub);

s32 frLoadExchangeTable(void) {
    sdfDevLoadFileEx(frStrFontExchg01Bin, &frWork, &frLoadExchangeTableData);
    frLoadExchangeTableData >>= 1;
    return 1;
}
