#ifndef FRFONT_H
#define FRFONT_H

#include "common.h"
#include "font.h"

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
s32 frDrawFontEx2(frFontHandle_t *pFRQ, s32 mode, s32 ot);
frFontHandle_t *frwGetSmallFontFRQ2(s32 x, s32 y, s32 z, u32 color, void *font, s32 arg);
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

typedef struct frTextHandle_t _TEXT;

typedef char FrFontChildGlyph_size_must_be_0x2C[
    sizeof(_TEXT) == 0x2C ? 1 : -1];

typedef struct frFontHandle_t _FRQ;
typedef struct frTextBank_t frTextBank_t;

u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);
frFontHandle_t *frCallMessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg,
    frFontHandle_t *pFRQ, s32 Line);
s32 frCallGetStrWidth3(s32 requestedLine, const _FRQ *glyph);
void frFontMoveChainTo(s32 targetX, s32 targetY, _FRQ *glyphChain);
frFontHandle_t *frSetUserImage(GsFont_t *pFont, u8 Style, frFontHandle_t *pFRQ);
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);
u8 *frDecodeFontImage(u8 Style, s32 FontNo);
s32 frSetFontSub(u8 Style, s32 FontNo);
s32 frSetFontSub2(frTextHandle_t *pText, s32 FontNo);

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

struct dds3BillResource_t;
struct dds3Billboard_Basic_t;

s32 frDrawFont(frFontHandle_t *pfont);
s32 frDrawMesFlowSub2(frFontHandle_t *pfont, s32 tagID);
void frDumpFontTexture(struct dds3Billboard_Basic_t *hBillboard, struct dds3BillResource_t *hResource, struct dds3BillResource_t *hResourceB);
s32 frPreCalcStrWidth(const char *str, u8 font, u8 unk2);

#endif
