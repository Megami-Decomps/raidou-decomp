#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/frWrap.c */
#include "common.h"

#include "sdfFrame.h"

#include "libsdf.h"

#include "kernel.h"

#include "font.h"

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

extern frFontHandle_t *frSetFontString(const char *, s8, s8, s8, frFontHandle_t *, s32);

extern void frSetFontSize(frFontHandle_t *, s32, s32);

extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);

extern u32 strlen(const char *text);

extern void *memcpy(void *a, const void *b, size_t c);

extern sdfMemHandle_t *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(sdfMemHandle_t *aHandle);

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void frSetWhiteSpaceW(s32 StyleNo, s32 Width);

extern u8 frEnableMode(s32 Flag);

extern u8 frDisableMode(u8 Flag);

extern frFontHandle_t *frLinkFontString(const char *, s8, s8, s8, frFontHandle_t *);

extern void frSetFontSpace(frFontHandle_t *, s32);

extern void frSetDrawPosition(frFontHandle_t *, u32, u32);

extern void frSetPriority(frFontHandle_t *, s32);

extern void frSetFontColor(frFontHandle_t *, u32);

extern void frSetFontColorNo(frFontHandle_t *, u8);

extern frFontHandle_t *frCombineFontRequest(frFontHandle_t *, frFontHandle_t *);

extern void frLoadFontData(s32 StyleNo, const char *pFileName);

extern void frDelFontStyle(s32 StyleNo);

typedef struct { u8 unk00[0x10]; u32 headerWord; } frPackedHeader_t;

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

/* Staff callers supply a trailing request; this wrapper creates a fresh chain. */
frFontHandle_t *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, void *source, void *unusedRequest);

void frwLoadStaffFont(void);

void frwFreeStaffFont(void);

frFontHandle_t *frwGetSmallFont1FRQ(s32 x, s32 y, s32 priority, u32 color,
                                      const char *text, frFontHandle_t *previous);

frFontHandle_t *frwGetFontEx(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, frFontHandle_t *previousGlyph);

frFontHandle_t *frwGetSmallFontFRQ(s32 X, s32 Y, s32 Pri, s32 Color, const char *pStr, frFontHandle_t *pFRQ);

frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 priority, u32 color,
                                      const char *text, frFontHandle_t *previous);

frFontHandle_t *frwGetSmallFontNoPropFRQ(s32 x, s32 y, s32 depth, u32 colors,
                                       const char *text, frFontHandle_t *previous);

frFontHandle_t *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                        const char *text, frFontHandle_t *previous);

frFontHandle_t *frwGetNumFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                        const char *text, frFontHandle_t *previous);

frFontHandle_t *frwGetSmallCaptionFontFRQ(s32 x, s32 y, s32 priority, u32 color,
                                           const char *text, frFontHandle_t *previous);

typedef struct frTextBank_t frTextBank_t;

extern frTextBank_t itfMesEmbedded_3E8CA8, itfMesEmbedded_3F39E0, itfMesEmbedded_3F7350;

extern u8 *MessageNumSearch(s32 index, frTextBank_t *bank, s32 size);

extern frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 style,
    s32 color, s32 type, s32 speed, u8 *text, frFontHandle_t *unusedPrevious);

frFontHandle_t *frwGetHelpFRQ(s32 x, s32 y, s32 priority, s8 bank, u16 index, s8 mode);

extern s32 frHelpLastIndex, D_0054F458, frHelpPosX, frHelpPosY;

extern s8 frHelpText[];

extern s32 frCalcStringHash(s8 *text);

extern void frCopyTagArgs(s8 *text, s8 *destination, s32 line);

frFontHandle_t *frGetHelpPos(s32 x, s32 y, s32 priority, s32 bank, s32 index);

s32 fr_1CA990(void);

void func_001CA998(void);

void func_001CA9A0(void);

void func_001CA9A8(void);

u32 fr_1CA9B0(u32 unused0, u32 unused1, u32 value);

/* Return the byte count saved by the most recent packed-resource decode. */
u32 fr_1CA9B8(void);

u32 frGetFontHeader(addr_t resourceBase);

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(frRelativeImage_t *block, frImagePtrs_t *out);

/* Read the opaque header word at 0x10; its role is not established. */
u32 fr_1CA9F8(u32 resourceBase);

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frImagePtrs_t *segments);

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
frFontNode_t *frAllocFontTable(s32 payloadBytes, s32 count);

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *frAllocFontNode(frFontNode_t *queue);

/* Prepend a detached payload's header; NULL or an already linked node fails.
 * Header pointer formation before the NULL test is retained unchanged. */
s32 frFreeFontNode(void *payload, frFontNode_t *queue);

/* Release the handle preceding the original ring base, not an acquired payload. */
u32 frFreeFontTable(frFontNode_t *ringBase);

extern s32 frTestFontX, frTestFontY;

extern s8 frTestFontAlpha;

extern s8 dds3PadBitTable[];

extern frFontHandle_t *frTestFontHandle;

extern char frTestFontText[];

extern void frSetDrawSpeed(frFontHandle_t *request, s32 speed);

void fr_1CAC50(void);
/* End of declarations from src/smt3/interface/frWrap.c */

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

INCLUDE_ASM(const s32, "interface/frWrap", frwLoadStaffFont);

void frwFreeStaffFont(void) {
    frDelFontStyle(4);
    frDelFontStyle(5);
}

INCLUDE_ASM(const s32, "interface/frWrap", func_00197CD0);

INCLUDE_ASM(const s32, "interface/frWrap", func_00197D18);

INCLUDE_ASM(const s32, "interface/frWrap", func_00197DE8);

INCLUDE_ASM(const s32, "interface/frWrap", func_00197F78);

INCLUDE_ASM(const s32, "interface/frWrap", func_00197FA0);

INCLUDE_ASM(const s32, "interface/frWrap", func_00197FD0);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198160);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198240);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198310);

INCLUDE_ASM(const s32, "interface/frWrap", func_001983F0);

INCLUDE_ASM(const s32, "interface/frWrap", func_001984B8);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198680);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198710);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198850);

void func_00198858(void) {
}

void func_00198860(void) {
}

INCLUDE_ASM(const s32, "interface/frWrap", func_00198868);

INCLUDE_ASM(const s32, "interface/frWrap", func_00198870);

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

INCLUDE_ASM(const s32, "interface/frWrap", func_001988B0);

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

INCLUDE_SDATA(const s32, "interface/frWrap", D_004DF4A0);

