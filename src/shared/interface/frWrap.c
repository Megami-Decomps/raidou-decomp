#include "common.h"
#include "sdfFrame.h"
#include "libsdf.h"
#include "kernel.h"
#include "font.h"
#include "sdfMemory.h"
#include "frFont.h"
#include "itfMisc.h"

typedef struct frRelativeImage_t
{
    s32 firstOffset;
    s32 secondDelta;
    u8 unk08[0x10];
    s32 thirdDelta;
} frRelativeImage_t;

typedef struct frImagePtrs_t
{
    void *treeNodes;
    void *entryBitOffsets;
    void *packedBitstream;
} frImagePtrs_t;

typedef struct { u32 start, end; } frBitRange_t;

typedef struct frFontNode_t
{
    u32 slotIndex;
    struct frFontNode_t *next;
} frFontNode_t;

typedef struct
{
    sdfMemHandle_t *allocation;
    frFontNode_t first;
} frFontRing_t;

extern u32 frFontRowBuf;
extern char frWrapEmptyString[];
extern void frSetWhiteSpaceW(s32 StyleNo, s32 Width);
extern frFontHandle_t *frLinkFontString(const char *, s8, s8, s8, frFontHandle_t *);
extern void frLoadFontData(s32 StyleNo, const char *pFileName);
extern void frDelFontStyle(s32 StyleNo);

typedef struct { u8 unk00[0x10]; u32 headerWord; } frPackedHeader_t;

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

frFontHandle_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, void *source, void *unusedRequest);
void frwLoadStaffFont(void);
void frwFreeStaffFont(void);

typedef struct frTextBank_t frTextBank_t;

extern frTextBank_t itfMesEmbedded_3E8CA8, itfMesEmbedded_3F39E0, itfMesEmbedded_3F7350;
extern u8 *MessageNumSearch(s32 index, frTextBank_t *bank, s32 size);
extern s32 frHelpLastIndex, D_0054F458, frHelpPosX, frHelpPosY;
u32 frGetFontHeader(addr_t resourceBase);
void frGetFontImagePtrs(frRelativeImage_t *block, frImagePtrs_t *out);
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frImagePtrs_t *segments);
frFontNode_t *frAllocFontTable(s32 payloadBytes, s32 count);
void *frAllocFontNode(frFontNode_t *queue);
s32 frFreeFontNode(void *payload, frFontNode_t *queue);
u32 frFreeFontTable(frFontNode_t *ringBase);
extern s32 frTestFontX, frTestFontY;

#include "scr.h"
#include "sdk_libc.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */

#include "include_asm.h"
#include "sdf_resource.h"
#include "billboard.h"
#include "sdf_types.h"
#include "draw.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "newdata.h"

/* Serialized font-bank entries use byte offsets relative to their bank. */
typedef struct FrFontTextIndex {
    u32 unk00;
    u32 offset;
} FrFontTextIndex;

typedef struct FrFontTextBank {
    u8 pad00[0x18];
    s32 count;
    u8 pad1C[4];
    FrFontTextIndex entries[1];
} FrFontTextBank;

_FRQ *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
    s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *unusedSub);

s32 frFontCountGlyphChainNodes(const struct _FRQ *glyph);

s32 frFontMeasureGlyphStripWidth(s32 line, const struct _FRQ *glyph);

#define ITF_BYTE_MASK 0xFF

#define ITF_VECTOR_WORD_COUNT 4

#define ITF_RGBA_COMPONENT_COUNT 4

#define ITF_FIXED_FRACTION_BITS 16

#define ITF_FIXED_COMPONENT_MAX 0xff0000

#define ITF_NEUTRAL_COLOR 0x80808080

#define ITF_GS_X_BIAS 0x7000

#define ITF_GS_Y_BIAS 0x7900

#define ITF_TRIANGLE_VERTEX_COUNT 3

#define ITF_QUAD_VERTEX_COUNT 4

#define ITF_STATE_PACKET_BYTES 0x30

#define ITF_TEST_FIRST_CONTEXT 0x47

#define ITF_TEST_SECOND_CONTEXT 0x48

#define ITF_ALPHA_FIRST_CONTEXT 0x42

#define ITF_ALPHA_SECOND_CONTEXT 0x43

#define ITF_TRIANGLE_PRIMITIVE_BITS 0x4B

#define ITF_QUAD_FAN_PRIMITIVE_BITS 0x4D

#define ITF_TEXTURED_FAN_PRIMITIVE_BITS 0x5D

#define ITF_TEXTURED_SPRITE_PRIMITIVE_BITS 0x156

#define ITF_COLORED_SPRITE_PRIMITIVE_BITS 0x15E

#define ITF_SPRITE_PRIMITIVE_BITS 0x46

#define ITF_TRIANGLE_STRIP_PRIMITIVE_BITS 0x4C

#define ITF_LINE_STRIP_PRIMITIVE_BITS 0x14A

#define ITF_LINES_PRIMITIVE_BITS 0x49

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 glyphCode);
void itfConvertText(u8 *output, const char *input);
extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);
_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags);

struct EffSpriteUV;

struct sdfTexHandle_t;

union EffSpriteColor;

/* Staff callers supply a trailing request; this wrapper creates a fresh chain. */
frFontHandle_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, void *source, void *unusedRequest) {
    frFontHandle_t *glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frSetWhiteSpaceW(kind, entryFlag);
    frEnableMode(1);
    frDisableMode(2);
    frDisableMode(0x10);
    glyph = frLinkFontString(source, kind, 0, 0, 0);
    frEnableMode(0x10);
    frEnableMode(2);
    frDisableMode(1);
    frSetFontSpace(glyph, measureFlag);
    frSetDrawPosition(glyph, x, y);
    frSetPriority(glyph, depth << 4);
    frSetFontColor(glyph, colors);
    frSetFontColorNo(glyph, 5);
    return glyph;
}

void frwLoadStaffFont(void) {
    frLoadFontData(4, "/font/staff1.fnt");
    frLoadFontData(5, "/font/staff2.fnt");
}

void frwFreeStaffFont(void) {
    frDelFontStyle(4);
    frDelFontStyle(5);
}

INCLUDE_ASM(const s32, "interface/frWrap", frGetSmallFontFRQ2Sub);

/* Preserve single-byte text and substitute 0x8080 for unmapped glyph pairs. */
void itfConvertText(u8 *output, const char *input) {
    s32 i;
    s32 length = strlen(input);

    for (i = 0; i < length; i++, output++) {
        if (input[i] >= 0) {
            output[0] = input[i];
        } else {
            u32 value = itfDecodeGlyph((u8)input[i + 1] | ((u8)input[i] << 8));
            if (value != 0xffff) {
                output[0] = value >> 8;
                output[1] = value;
            } else {
                output[0] = 0x80;
                output[1] = 0x80;
            }
            output++;
            i++;
        }
    }
}

INCLUDE_ASM(const s32, "interface/frWrap", frwGetFontEx);

INCLUDE_ASM(const s32, "interface/frWrap", frCallGetFontEx);

INCLUDE_ASM(const s32, "interface/frWrap", frLinkKanjiSub);

INCLUDE_ASM(const s32, "interface/frWrap", frwGetSmallFontFRQ2);

INCLUDE_ASM(const s32, "interface/frWrap", frwGetSmallNumFontFRQ);

INCLUDE_ASM(const s32, "interface/frWrap", frwGetNumFontFRQ);

INCLUDE_ASM(const s32, "interface/frWrap", frwGetSmallCaptionFontFRQ);

INCLUDE_ASM(const s32, "interface/frWrap", frwGetSmallFont1FRQ);

_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags) {
    const u8 *text = MessageNumSearch(textId, bank, 0);
    u32 mode;
    _FRQ *handle;

    if (text == 0) {
        return 0;
    }
    mode = (u16)flags;
    switch (mode) {
    case 1:
        frDisableMode(4);
        break;
    case 2:
        frEnableMode(8);
        break;
    case 4:
        frEnableMode(0x20);
        break;
    }
    handle = itfDrawColor(x, y, depth, 1, 0, 0, 0x80, text, 0);
    if (flags & 0x10000) {
        s32 maxWidth = 0;
        s32 i;
        s32 width;
        for (i = 0; i < frFontCountGlyphChainNodes(handle); i++) {
            width = frFontMeasureGlyphStripWidth(i, handle);
            if (maxWidth < width) {
                maxWidth = width;
            }
        }
        frFontMoveChainTo(x - maxWidth / 2, y, handle);
    }
    switch (mode) {
    case 1:
        frEnableMode(4);
        break;
    case 2:
        frDisableMode(8);
        break;
    case 4:
        frDisableMode(0x20);
        break;
    }
    return handle;
}

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "interface/frWrap", frDrawMiscHelpSub);
#else
INCLUDE_ASM(const s32, "interface/frWrap", ButtonLineHelpTextSet);
#endif

INCLUDE_ASM(const s32, "interface/frWrap", itfDrawUnderscoreTextSegment);

INCLUDE_ASM(const s32, "interface/frWrap", frLeaf4);

void frLeaf5(void) {
}

void frLeaf6(void) {
}

INCLUDE_ASM(const s32, "interface/frWrap", frLeaf7);

INCLUDE_ASM(const s32, "interface/frWrap", frLeaf8);

u32 frGetFontHeader(addr_t resourceBase) {
    return resourceBase;
}

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(frRelativeImage_t *block, frImagePtrs_t *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->treeNodes = (u8 *)block + firstOffset;
    out->entryBitOffsets = (u8 *)block + secondOffset;
    out->packedBitstream = (u8 *)block + thirdOffset;
}

INCLUDE_ASM(const s32, "interface/frWrap", frCallGetFontHeader);

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frImagePtrs_t *segments) {
    frBitRange_t range;
    s32 produced;
    u8 *table = segments->treeNodes;
    u16 *bitWords;
    u32 wordIndex;
    u32 start;
    s32 remaining;
    u32 state;
    u16 *root;
    u16 *node;

    memcpy(&range.start, (u8 *)segments->entryBitOffsets + entryIndex * 4, 4);
    memcpy(&range.end, (u8 *)segments->entryBitOffsets + entryIndex * 4 + 4, 4);
    produced = 0;
    frGetFontHeader(EE_ADDR(resourceBase));
    start = range.start;
    bitWords = segments->packedBitstream;
    wordIndex = start >> 4;
    remaining = range.end - start;
    state = (bitWords[wordIndex] | 0x10000) >> (start & 0xF);
    root = (u16 *)(table + 2);
    node = root;
    do {
        u16 index = node[state & 1];

        node = (u16 *)(table + index * 6 + 2);
        if (*node == 0) {
            decodedBytes[produced++] = ((u8 *)node)[2];
            node = root;
        }
        state >>= 1;
        if (state == 1) {
            wordIndex++;
            state = bitWords[wordIndex] | 0x10000;
        }
        remaining--;
    } while (remaining != 0);
    frFontRowBuf = produced;
    return produced;
}

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
frFontNode_t *frAllocFontTable(s32 payloadBytes, s32 count) {
    sdfMemHandle_t *buffer;
    u8 *list;
    frFontNode_t *cursor;
    frFontNode_t *next;
    s32 i;

    buffer = sdfMemAllocDirect((payloadBytes + ITF_NODE_HEADER_BYTES) * (count + 1) + ITF_ALLOCATION_HANDLE_BYTES);
    list = (u8 *)sdfMemLock(buffer);
    i = 0;
    memcpy(list, &buffer, ITF_ALLOCATION_HANDLE_BYTES);
    list += ITF_ALLOCATION_HANDLE_BYTES;
    cursor = (frFontNode_t *)list;
    for (; i < count; i++) {
        next = (frFontNode_t *)((u8 *)cursor + payloadBytes + ITF_NODE_HEADER_BYTES);
        cursor->slotIndex = i;
        cursor->next = next;
        cursor = next;
    }
    cursor->slotIndex = count;
    cursor->next = (frFontNode_t *)list;
    return (frFontNode_t *)list;
}

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *frAllocFontNode(frFontNode_t *queue) {
    frFontNode_t *head = queue->next;

    if (head->slotIndex == 0) {
        return NULL;
    }
    queue->next = head->next;
    head->next = NULL;
    return head + 1;
}

/* Prepend a detached payload's header; NULL or an already linked node fails.
 * Header pointer formation before the NULL test is retained unchanged. */
s32 frFreeFontNode(void *payload, frFontNode_t *queue) {
    frFontNode_t *node = (frFontNode_t *)payload - 1;
    if (payload == NULL) {
        return 0;
    }
    if (node->next != NULL) {
        return 0;
    }
    node->next = queue->next;
    queue->next = node;
    return 1;
}

/* Release the handle preceding the original ring base, not an acquired payload. */
u32 frFreeFontTable(frFontNode_t *ringBase) {
    sdfMemFreeDirect(((frFontRing_t *)((u8 *)ringBase - ITF_ALLOCATION_HANDLE_BYTES))->allocation);
    return 1;
}

INCLUDE_SDATA(const s32, "interface/frWrap", frWrapEmptyString);

