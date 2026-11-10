#include "shared/sdf/sdfDevCons/decls.h"

void *sdfDevVprintf(u32 *aCB, const char *aStr, void *LegacyParamArray) {
    sdfDevPrintCB_t *input = (sdfDevPrintCB_t *)aCB;
    char text[0x200];
    s32 formattedCount;
    u8 *start;
    u8 *cursor;
    sdfDevConsPacketHeader_t *header;
    u8 *textCursor;
    s32 character;
    s32 glyphCount;
    u32 color;
    u64 position;
    s32 quadwordCount;

    sdfDevConsInit();
    formattedCount = vsprintf(text, aStr, LegacyParamArray);
    start = (u8 *)sdfGrOpenPacketBuff();
    if (formattedCount == 0) {
        QUAD_ZERO(*(u128 *)start);
        cursor = start + 0x10;
    } else {
        header = (sdfDevConsPacketHeader_t *)start;
        cursor = start + sizeof(*header);
        glyphCount = 0;
        /* This dual-use command carries x, y, depth and RGBA in its four words. */
        position = (u32)((u16)input->x | ((u32)(u16)input->y << 16)) |
                   ((u64)(u32)input->z << 32);
        color = input->attr;
        textCursor = (u8 *)text;
        while ((character = *textCursor++) != 0) {
            if (character >= 0x20) {
                sdfDevConsGlyph_t *glyph = (sdfDevConsGlyph_t *)cursor;
                u32 uv = sdfDevConsFontUvTable[character - 0x20];

                glyph->color = color;
                glyph->firstUv = uv;
                glyph->firstPosition = position;
                glyph->secondUv = (u32)(uv + 0x00C000C0);
                glyph->secondPosition = position + 0x006000C0;
                cursor += sizeof(*glyph);
                glyphCount++;
            }
            /* Control bytes consume the same horizontal advance as glyphs. */
            position += 0xC0;
        }
        if (glyphCount & 1) {
            *(u64 *)cursor = 0;
            cursor += sizeof(u64);
        }
        quadwordCount = ((s32)(cursor - start) >> 4) - 1;
        input->x = (u16)position;
        header->quadwordCount = quadwordCount;
        header->vifNop = 0;
        header->vifDirect = 0x50000000 | quadwordCount;
        header->setupTag = 0x10AB400000000003ULL;
        header->setupRegisters = 0xEEE;
        header->textureClamp = 0;
        header->textureClampRegister = 8;
        header->textureFilter = 1;
        header->textureFilterRegister = 0x14;
        header->textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
        header->textureStateRegister = 6;
        header->glyphTag = 0x5400000000008000ULL | glyphCount;
        header->glyphRegisters = 0x53531;
    }
    sdfGrClosePacketBuff(EE_ADDR_S(cursor));
    return start;
}
