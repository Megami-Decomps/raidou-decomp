#ifndef FRFONT_H
#define FRFONT_H

#include "common.h"
#include "font.h"

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);
u8 frDisableMode(u8 Flag);
u8 frEnableMode(s32 Flag);
u32 frGetFontHandleNum(void);
s32 frGetTextHandleNum(void);
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);
void frSetExtDic(u8 *pExtFontData);
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);
void frSetVariableRequest(s32 VarNo, s32 pStr);

/* Font items use their own 0x2C-byte pool, separate from parent chains. */
typedef struct _TEXT {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            s8 spacing;
        } byteRoles;
    } glyphCodeOrContext;
    /* Child draw count: setup clears it; nonzero-fade draws increment it, and
     * the fade helper also uses it to select the jitter phase. */
    u16 drawCount;
    s32 x;
    s32 y;
    s32 advance;
    /* The same render-state word is also accessed by its low fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    struct {
        u8 cellWidth;
        u8 cellHeight;
        u8 opaque[2];
    } cellDimensions;
    GsFont_t *cachedItem;
    GsFont_t *sourceItem;
    struct _TEXT *previous;
    struct _TEXT *next;
} _TEXT;

typedef char FrFontChildGlyph_size_must_be_0x2C[
    sizeof(_TEXT) == 0x2C ? 1 : -1];

/* Parent chain heads: frInitFontTable's glyph pool, 0x58 bytes in Raidou 1 and
 * 0x5C in Raidou 2. */
typedef struct _FRQ {
    union {
        u16 glyphCode;
        struct {
            u8 encodedContextByte;
            u8 baseContextByte; /* copied back to +0 when the chain's context is reset */
        } byteRoles;
    } glyphCodeOrContext;
    s8 spacing; /* frSetFontSpace writes it; frGetStrWidth adds it per child */
    u8 pad03;
    s32 x;
    s32 y;
    s32 advance;
    /* Parent glyphs store dimensions; render words also carry a fade byte. */
    union {
        struct {
            u16 cellAdvance;
            u16 cellHeight;
        } parentDimensions;
        u32 renderWord;
        struct {
            u8 value;
            u8 opaque[3];
        } fadeByte;
    } parentDimensionsOrRenderWord;
    /* Rendering, glyph setup, and message shade paths use distinct byte views. */
    union {
        u32 renderValue;
        struct {
            u8 firstOption;
            u8 fontIndex;
            u8 secondOption;
            u8 sharedFlags;
        } setupBytes;
        struct {
            u8 green;
            u8 red;
            u8 blue;
            u8 opaque;
        } shadeColor;
    } renderValueOrSetupOrShade;
    u32 childCount;
    _TEXT *firstChild;
    _TEXT *lastChild;
    struct _FRQ *previous;
    struct _FRQ *next;
    struct _FRQ *chainHead;
    u32 timedControlCode; /* F214/F215 terminal timing control. */
    u32 pendingLipsStopCode; /* F117 until evtLipsStopFunction runs. */
    u32 skipLipsStopWait; /* Set when a timing control follows F117. */
    s32 remainingWaitFrames; /* F215 uses 0xFFFF for title-sound completion. */
    s32 contextModeEnabled;
    u8 pad44[8];
#ifdef VERSION_RAIDOU2
    u8 pad4C[4];
#endif
    s32 contextResetWord; /* cleared on every chain node with the context byte */
    u8 pad_end[8];
} _FRQ;

#ifdef VERSION_RAIDOU1
typedef char FrFontGlyph_size_must_be_0x58[sizeof(_FRQ) == 0x58 ? 1 : -1];
#endif

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
typedef struct FrFontCtx {
    s32 x;
    s32 y;
    s32 z;
    s8 fontIndex;
    s8 firstOption;
    s8 secondOption;
    u8 contextEncodedByte;
    const u8 *encodedText; /* Borrowed source bytes; parsing advances the offset, not this data. */
    _FRQ *glyphChain;
    s32 encodedTextOffset;
    s8 pendingCreate;
    s8 pendingPosition;
    u8 pad1E[2];
} FrFontCtx;

#ifdef VERSION_RAIDOU2
typedef char FrFontGlyph_size_must_be_0x5C[sizeof(_FRQ) == 0x5C ? 1 : -1];
#endif

/* Relative offsets select three packed-font resource regions. */
typedef struct frFontResource_t {
    s32 firstOffset; /* 0x00 */
    s32 secondDelta; /* 0x04 */
    u8 opaque08[0x10];
    s32 thirdDelta; /* 0x18 */
} frFontResource_t;

/* Free-list header stored immediately before each interface-pool payload. */
typedef struct frFontNodeHeader_t {
    u32 slotIndex; /* Zero identifies the ring sentinel. */
    struct frFontNodeHeader_t *next;
} frFontNodeHeader_t;

/* Output pointers address the tree, entry bit ranges, and packed bit words. */
typedef struct frFontImage_t {
    void *treeNodes;       /* 0x00 */
    void *entryBitOffsets; /* 0x04 */
    void *packedBitstream; /* 0x08 */
} frFontImage_t;

u32 frFreeFontTable(frFontNodeHeader_t *ringBase);
void frGetFontImagePtrs(frFontResource_t *block, frFontImage_t *out);
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frFontImage_t *segments);

#endif
