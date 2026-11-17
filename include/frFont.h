#ifndef FRFONT_H
#define FRFONT_H

#include "common.h"
#include "font.h"

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
s32 frDrawFontOt(frFontHandle_t *pFRQ, s8 mode, u32 ot);
s32 frCallGetFontWidth3(const char *str, u8 font, u8 unk2, s32 spacing);
frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 z, u32 color, void *font, frFontHandle_t *pFRQ);
u16 frGetSmallFontFRQ2Sub(u16 code);
frFontHandle_t *frCallSetFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ);
frFontHandle_t *frDestroyFontRequestAfterSub(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2, s32 Mode);
frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);
frFontHandle_t *frSetFontString(u8 *text, s8 style, s8 firstOption, s8 secondOption, frFontHandle_t *chain);
u32 frGetStrWidth2(frFontHandle_t *pFRQ);
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);
u8 frDisableMode(u8 Flag);
u8 frEnableMode(s32 Flag);
u32 frGetFontHandleNum(void);
void frCallMemFreeDirect(s32 slot);
s32 frGetTextHandleNum(void);
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);
s32 frCallFlushGsFontTable(frFontHandle_t *pFRQ);
void frSetExtDic(u8 *pExtFontData);
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);
void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height);
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);
void frSetVariableRequest(s32 VarNo, s32 pStr);

typedef struct frTextHandle_t _TEXT;

typedef char FrFontChildGlyph_size_must_be_0x2C[
    sizeof(_TEXT) == 0x2C ? 1 : -1];

typedef struct frFontHandle_t _FRQ;

/* Output point written by glyph-chain position lookup. */
typedef struct frFontPosition_t {
    s32 x;
    s32 y;
} frFontPosition_t;

typedef char frFontPosition_size_must_be_0x08[
    (sizeof(frFontPosition_t) == 0x08) ? 1 : -1];

void frFontGetGlyphPositionAtIndex(frFontPosition_t *position, s32 requestedIndex, const _FRQ *glyph);
typedef struct frTextBank_t frTextBank_t;

u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);
frFontHandle_t *frCallMessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg,
    frFontHandle_t *pFRQ, s32 Line);
s32 frCallGetStrWidth3(s32 requestedLine, const _FRQ *glyph);
void frFontMoveChainTo(s32 targetX, s32 targetY, _FRQ *glyphChain);
void frLeaf(_FRQ *glyph);
frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);
u32 frGetMessageResult(u32 Flag);
u32 itfGetTextLipsUnitId(void);
u32 itfGetTextLipsMotionId(void);
s32 frCallEventVoiceSub(const _FRQ *glyph);
void frFontCenterGlyphChainAt(s32 x, s32 y, _FRQ *glyphChain);
frFontHandle_t *frSetUserImage(GsFont_t *pFont, u8 Style, frFontHandle_t *pFRQ);
frFontHandle_t *frCallGetFontEx(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colors, const char *text,
                                frFontHandle_t *previousGlyph);
frFontHandle_t *frLinkKanjiSub(s32 x, s32 y, s32 depth, u32 colors, const char *text, frFontHandle_t *previousGlyph);
void frInitTextHandle(frTextHandle_t *pText, u16 Code, u8 Style, u8 ColNo, u32 Color, u8 Tp);
void frInitFontHandle(frFontHandle_t *pFRQ);
frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption);
void SwitchOK(frFontHandle_t *glyph);
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);
u8 *frDecodeFontImage(u8 Style, s32 FontNo);
s32 frSetFontSub(s8 Style, s32 FontNo);
void frCallGetFontWidth2(frTextHandle_t *pText, s32 FontNo, s32 Width, s32 Height);
void frSetFontSub2(frTextHandle_t *pText, s32 FontNo);

#ifdef VERSION_RAIDOU1
typedef char FrFontGlyph_size_must_be_0x58[sizeof(_FRQ) == 0x58 ? 1 : -1];
#endif

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
typedef struct frFontCtx_t {
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
} frFontCtx_t;

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
frFontNodeHeader_t *frAllocFontTable(s32 payloadBytes, s32 count);
void *frAllocFontNode(frFontNodeHeader_t *queue);
s32 frFreeFontNode(void *payload, frFontNodeHeader_t *queue);
s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight, s32 recordBytes);
void frCallMemLock(u8 StyleNo, u8 *pFontData, struct sdfMemHandle_t *DataMh);
void frGetFontImagePtrs(frFontResource_t *block, frFontImage_t *out);
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, void *resourceBase, frFontImage_t *segments);

struct dds3BillResource_t;
struct dds3Billboard_Basic_t;

void frFontAppendIndexedTextToGlyphChain(u8 VarNo, frFontCtx_t *ctx);
void frFontCreateContext(frFontCtx_t *ctx);
void frMessageReturnCode(frFontCtx_t *pMsgInfo);
s32 execTag_0(s32 code, frFontCtx_t *ctx);
s32 frDrawFont(frFontHandle_t *pfont);
s32 frDrawFontEx(frFontHandle_t *pFRQ, s8 Mode);
void frDumpFontTexture(struct dds3Billboard_Basic_t *hBillboard, struct dds3BillResource_t *hResource, struct dds3BillResource_t *hResourceB);
s32 frPreCalcStrWidth(const char *str, u8 font, u8 unk2);

#endif
