#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/frFont.c */
#include "font.h"

#include "kernel.h"

typedef struct frFontHeader_t frFontHeader_t;

typedef struct frFontResource_t frFontResource_t;

typedef struct frFontData_t frFontData_t;

typedef struct frFontNodeHeader_t frFontNodeHeader_t;

typedef struct frFontAtlas_t frFontAtlas_t;

typedef struct frFontSystem_t frFontSystem_t;

typedef union frFontDmaTag_t frFontDmaTag_t;

typedef struct frFontGsWrite_t frFontGsWrite_t;

typedef struct frFontGsPacket_t frFontGsPacket_t;

typedef struct frTextIndex_t frTextIndex_t;

typedef struct frTextBank_t frTextBank_t;

typedef struct frMsgInfo_t frMsgInfo_t;

typedef struct frFontImage_t frFontImage_t;

/* Serialized resource header; DDS2 moved the lookup offset by eight bytes. */
struct frFontHeader_t {
    u32 tableOffset;
    u8 pad04[6];
    u8 tableCount;
    u8 pad0B[3];
    u16 widthCount;
    u16 cellWidth;
    u16 cellHeight;
    u8 pad14[2];
    u8 hasExtra;
    u8 pad17;
    u32 lookupOffset;
};

/* Relative offsets select three packed-font resource regions. */
struct frFontResource_t {
    s32 firstOffset; /* 0x00 */
    s32 secondDelta; /* 0x04 */
    u8 opaque08[0x10];
    s32 thirdDelta; /* 0x18 */
};

struct frFontData_t {
    sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *prop;
    void *ysht;
    GsFont_t **trans;
    struct frFontResource_t *resource;
    addr_t data;
};

/* Free-list header stored immediately before each interface-pool payload. */
struct frFontNodeHeader_t {
    u32 slotIndex; /* Zero identifies the ring sentinel. */
    struct frFontNodeHeader_t *next;
};

/* frFontWork + 0x160; the small texture label aliases this owned subobject. */
struct frFontAtlas_t {
    s32 width;
    u32 widthExponent;
    s32 height;
    u32 heightExponent;
    u32 bufferBase;
    u32 bufferWidth;
};

struct frFontSystem_t {
    frFontData_t entries[9];
    s32 cachedItemCount;
    s32 itemCount;
    s32 glyphCount;
    struct frFontNodeHeader_t *itemPool;
    struct frFontNodeHeader_t *glyphPool;
    sdfGsMemSlice_t *textureHead0;
    sdfGsMemSlice_t *textureHead1;
    frFontAtlas_t atlas;
    u32 imageBuffers[6];
    u8 pad190[4];
    struct frFontHandle_t *glyphSlots[2];
};

/* The SDK DMA tag is accessed as a quadword and as its four command words. */
union frFontDmaTag_t {
    u128 value;
    u32 words[4];
};

struct frFontGsWrite_t {
    u64 value;
    u64 reg;
};

struct frFontGsPacket_t {
    frFontDmaTag_t dma;
    u64 gifTag;
    u64 gifRegisters;
    frFontGsWrite_t writes[6];
};

/* Serialized font-bank entries use byte offsets relative to their bank. */
struct frTextIndex_t {
    u32 unk00;
    u32 offset;
};

struct frTextBank_t {
    u8 pad00[0x18];
    s32 count;
    u8 pad1C[4];
    frTextIndex_t entries[1];
};

/* Encoded text cursor and pending glyph updates; distinct from a glyph (0x20). */
struct frMsgInfo_t {
    s32 X;
    s32 Y;
    s32 Z;
    s8 Style;
    s8 ColNo;
    s8 Tp;
    u8 Spd;
    u8 *pMsg;
    frFontHandle_t *pFRQ;
    s32 Ofs;
    s8 FrqFlag;
    s8 RetFlag;
    u8 pad1E[2];
};

/* Output pointers address the tree, entry bit ranges, and packed bit words. */
struct frFontImage_t {
    void *treeNodes; /* 0x00 */
    void *entryBitOffsets; /* 0x04 */
    void *packedBitstream; /* 0x08 */
};

/* Pixel upload inputs shared by texture and font-atlas producers. */
typedef struct sdfImageRequest_t {
    void *pixels;
    sdfMemHandle_t *allocation;
    u8 allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} sdfImageRequest_t;

/* sdfImageSetPacket_tag.freeMode: which heap owns the uploaded pixels. */
enum SdfImageSetMem
{
    SDF_IMAGESET_MEM_IGNORE = 0,
    SDF_IMAGESET_MEM_HANDLE = 1,
    SDF_IMAGESET_MEM_CHIP = 2
};

void frSetWhiteSpaceW(s32 StyleNo, s32 Width);

extern frFontSystem_t gFT;

#define FR_FONT_BYTE_MASK 0xFF

#define FR_FONT_ENTRY_ENABLED 1

u32 frSwapColor(u32 Col);

#define FR_FONT_BYTE_SHIFT 8

#define FR_FONT_MIDDLE_BYTE_MASK 0x0000FF00

#define FR_FONT_OUTER_BYTE_SHIFT 24

s32 frConvertVal2N(u32 Val);

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt);

void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot);

#define FR_FONT_GLYPH_HEIGHT_SHIFT 1

void frCopyFontClutToVram(s32 GS_LocalAddr, saddr_t pClut);

extern void sceGsSetDefLoadImage(void *, s16, s16, s32, s32, s32, s16, s16);

extern s32 WaitSema(s32);

extern volatile s32 dds3GsSema;

extern void FlushCache(s32);

extern void sceGsExecLoadImage(void *, saddr_t);

extern s32 sceGsSyncPath(s32, s32);

extern s32 SignalSema(s32);

void frInitFontTable(s32 GsFontNums, s32 TextNums);

extern sdfGsMemSlice_t * sdfGsMemAllocFromTail(s32 aSize, s32 aType);

s32 fmInitFontMemControl(s32 unused, s32 columns, s32 rows, s32 cellWidth, s32 cellHeight,
                               s32 recordBytes);

extern frFontNodeHeader_t *frAllocFontTable(s32 payloadBytes, s32 count);

void frLoadStyleAll(void);

void frSetFontClut(void);

#define FR_FONT_INDEXED_TEXT_SLOT_COUNT 32

/* Address words substituted by the indexed encoded-text control codes. */
extern u32 gVarStr[32];

void frClearTexBuf(void);

extern s32 gRegistMax;

void frDestroyFontTable(dds3ProcessID_t *id);

void frDelStyleAll(void);

extern frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);

extern u32 frFreeFontTable(frFontNodeHeader_t *pool);

s32 fmDestroyFontMemControl(void);

extern void sdfGsMemFree(sdfGsMemSlice_t *aHandle);

void frAddFontStyle(
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
);

extern u32 sdfMemLock(sdfMemHandle_t *aHandle);

#define FR_FONT_TABLE_ENTRY_SHIFT 6

#define FR_FONT_WORD_BYTES 4

void frDelFontStyle(s32 StyleNo);

#define FR_FONT_FIRST_RELEASABLE_SLOT 2

extern u32 frFontLoaded[];

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern void frLoadFontData(s32 StyleNo, const char *pFileName);

#define FR_FONT_ENTRY_CAPACITY 9

#define FR_FONT_IMAGE_WORD_COUNT 16

extern u8 frFontClutTable[];

#define FR_FONT_IMAGE_WORD_BYTES 4

#define FR_FONT_IMAGE_LAST_WORD 15

#define FR_FONT_IMAGE_BUFFER_COUNT 6

void frFlushGsFontTable(frTextHandle_t *pText);

s32 fmRetFontMem(FontMem_t *pFontArea);

frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ);

s32 frEraseEffect(frFontHandle_t *pFRQ);

s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

extern u32 dds3GetFrameID(void);

extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);

s32 frGetGsFontNum(void);

s32 frGetTextHandleNum(void);

u32 frGetFontHandleNum(void);

s32 frGetYShiftLev(s8 Style, s32 FontNo);

frFontHandle_t *frSetUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

extern void *frAllocFontNode(frFontNodeHeader_t *pool);

extern void frInitFontHandle(frFontHandle_t *pFRQ);

void frInitTextHandle(frTextHandle_t *pText, s32 Code, s32 Style, s32 ColNo, s32 Color, s32 Tp);

extern s8 gFontBaseWidth[];

u32 frGetDefaultFontWidth(u8 Style);

u32 frGetDefaultFontHeight(u8 Style);

frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);

GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);

FontMem_t *fmRentFontMem(void);

#define FR_FONT_ATLAS_POSITION_SHIFT 4

#define FR_FONT_PIXEL_FORMAT 0x14

#define FR_FONT_ATLAS_DEST_SHIFT 6

extern void sdfImageSetReq(sdfImageRequest_t *parm);

u8 *frDecodeFontImage(u8 Style, s32 FontNo);

extern void *sdfChipAllocDirect(s32 aSize);

#define FR_FONT_RESOURCE_CLONE_BYTES 0x120

extern void frGetFontImagePtrs(frFontResource_t *resource,
                              frFontImage_t *segments);

extern s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex,
                                void *resourceBase,
                                frFontImage_t *segments);

GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo);

u16 frGetFontWidth(s32 slotIndex);

u16 frGetFontHeight(s32 slotIndex);

extern u8 gModeFlag;

void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed);

#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81

#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)

void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);

u32 frGetStrWidth(frFontHandle_t *pFRQ);

void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height);

void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);

void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);

#define FR_FONT_CONTEXT_VALUE_SHIFT 4

void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);

void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);

u8 frEnableMode(s32 Flag);

u8 frDisableMode(u8 Flag);

void frSetFontDrawOT(u32 OtType);

extern u32 gFontDrawOT;

#define FR_FONT_FADE_OPTION 2

#define FR_FONT_FADE_VALUE_STEP 8

#define FR_FONT_FADE_Y_STEP 0x10

s32 frDrawFont(frFontHandle_t *pFRQ);

extern s32 frDrawFontEx(frFontHandle_t *pFRQ, s8 Mode);

s32 func_001C7E38(frFontHandle_t *pFRQ, s8 Mode, u32 Ot);

s32 frFlipPacketBuf(dds3ProcessID_t *id);

void frSetExtDic(u8 *pExtFontData);

#define FR_FONT_TEMPORARY_SLOT 8

void frDestroyExtDic(void);

s32 frStrLen(s8 *pStr);

u32 frGetStrWidth2(frFontHandle_t *pFRQ);

#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2

extern s32 frDefaultFontHandle;

void frSetMessageDefStepY(void);

extern u32 gMessageStepY;

#define FR_FONT_DEFAULT_CURSOR_SPACING 0x14

void frSetMessageStepY(u32 StepY);

u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);

extern void *memcpy(void *a, const void *b, size_t c);

void frMessageReturnCode(frMsgInfo_t *pMsgInfo);

#define FR_FONT_CONTEXT_CURSOR_SCALE 8

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *frAnalyzeMessage(frMsgInfo_t *pMsgInfo);

u32 frGetMessageResult(u32 Flag);

extern u32 gMessageRequestResult;

void frSetVariableRequest(s32 VarNo, s32 pStr);

extern s8 gMessageSpaceX;

extern u16 frMessageCode;

extern s32 frFindTagCode(u16 code);

extern s32 frFindTagCode2(u16 code);

extern frFontHandle_t *frGetNameFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetName1NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetName2NFRQ(s8 target, frFontHandle_t *chain, s32 firstOption);

extern frFontHandle_t *frGetCNameFRQ(s8 nameIndex, s8 fontIndex, frFontHandle_t *chain, s32 firstOption);

extern s32 frAnalyzeTag(s32 code, frMsgInfo_t *mi);

extern u8 frSetFontString_style;

extern void frGetMessageCode(u16 code);

frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption, s32 shiftFlag);

void frCreateVariableRequest(u8 VarNo, frMsgInfo_t *pMsgInfo);

extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);

extern void *memset(void *a, s32 b, size_t c);

extern u32 strlen(const char *text);

extern void frRestoreNameImage(s8 TargetNo);

extern s32 frFreeFontNode(void *payload, frFontNodeHeader_t *pool);

extern void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);

u32 frPreCalcStrWidth(s8 *pStr, s32 Style, s32 Mode);

s32 frCalcStringHash(s8 *text);

extern u8 frEmptyString[];

extern frFontHandle_t *frSetFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 flag);

frFontHandle_t *frLinkFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ);

void frCreateNewHandle(frMsgInfo_t *pMsgInfo);

void frPushTagChar(frMsgInfo_t *ctx, u16 code);

s32 frGetTagArgCount(s32 tagCode);

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious);

/* Select by the low byte of slotId; enable the entry and store value + 1 as a byte. */
void frSetWhiteSpaceW(s32 StyleNo, s32 Width);

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 Col);

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frConvertVal2N(u32 Val);

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt);

typedef struct {
    u64 uv;
    u64 rgbaq;
    u64 xyz;
} frFontSpriteCorner_t;

typedef struct {
    frFontDmaTag_t dma;
    u64 setupTag;
    u64 setupRegisters;
    frFontGsWrite_t tex0;
    frFontGsWrite_t tex1;
    frFontGsWrite_t clamp;
    u64 spriteTag;
    u64 spriteRegisters;
    frFontSpriteCorner_t corners[2];
} frFontSpritePacket_t;

/* Two direct views of the allocated DMA/GIF packet: header words and GS data. */
typedef union {
    frFontSpritePacket_t sprite;
    u32 words[36];
} frFontSpritePacketView_t;

extern sdfGrOt_t sdfGrOtTable[];

extern void *sdfGrAllocPacketBuff(u32 size);

extern void sdfDrawTagInit(sdfDrawTag_t *tag);

extern void sdfDrawTagLink(sdfDrawTag_t *tag, addr_t packet);

void frCreateDrawFontPacket_20550848(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const frFontUvRect_t *uv,
    const frFontAtlas_t *atlas, s32 surface);

/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot);

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 GS_LocalAddr, saddr_t pClut);

void frClearTexBuf(void);

void frLoadFontData(s32 StyleNo, const char *pFileName);

void frInitFontTable(s32 GsFontNums, s32 TextNums);

/* Release slot allocations, both glyph chains/rings, and the shared GS resource buffers. */
void frDestroyFontTable(dds3ProcessID_t *id);

/* Bind sections from resource bytes or a retained allocation.
 * Optional flag/value sections each start with a byte-length word; their data
 * precedes the word table and remaining resource data. Offsets are unchecked.
 */
void frAddFontStyle(
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
);

/* Slots zero/one are not freed here; clear the data pointer only when an allocation exists. */
void frDelFontStyle(s32 StyleNo);

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void);

/* Free each installed entry, including the temporary slot, without reloading. */
void frDelStyleAll(void);

/* Assemble six 16-word image blocks from little-endian source bytes and upload
 * each to its matching GS buffer. The inner countdown includes zero. */
void frSetFontClut(void);

void frFlushGsFontTable(frTextHandle_t *pText);

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ);

frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);

/* Return the live count of retained font-cache records. */
s32 frGetGsFontNum(void);

/* Return the live pooled child-glyph count. */
s32 frGetTextHandleNum(void);

u32 frGetFontHandleNum(void);

s32 frGetYShiftLev(s8 Style, s32 FontNo);

extern u32 frSpaceAdvanceOffset;

void frGetXShiftLev(frTextHandle_t *glyph, s32 shiftIndex, s32 isFirst);

/* Create a glyph wrapper for source and append it to an optional destination chain. */
frFontHandle_t *frSetUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

/* Append a source-glyph reference wrapper, not a deep copy of source storage.
 * If wrapper creation returns NULL, preserve the original destination. */
frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);

/* Retain one atlas item and submit its pixel upload with the requested ownership
 * mode. Coordinates are converted from sixteenths; pool exhaustion is unchecked. */
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);

/* Decode one packed font-resource entry into a fixed-size clone.
 * Neither allocation nor entry index is checked. */
u8 *frDecodeFontImage(u8 Style, s32 FontNo);

/* Retain a cached table item, or create/upload a new one and cache it.
 * New records start with one reference; itemIndex is not bounds-checked. */
GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo);

/* Store the font/options, narrowed glyph code and high flag bits, then clear
 * positioning/links. Other glyph fields are deliberately not fully reset here. */
void frInitTextHandle(frTextHandle_t *pText, s32 Code, s32 Style, s32 ColNo, s32 Color, s32 Tp);

void frInitFontHandle(frFontHandle_t *pFRQ);

frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption, s32 shiftFlag);

frFontHandle_t *frLinkFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ);

frFontHandle_t *frSetFontString(u8 *text, s8 style, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 relative);

/* Double the masked input byte and saturate the stored byte at 0x80.
 * Keep the original signed constant used for the saturated byte assignment. */
void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed);

/* Store the signed spacing byte and refresh the cached child-chain advance. */
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);

/* Set the initial glyph's halfword dimensions and every visited child's advance/
 * byte dimensions, then remeasure only the initial glyph. Input must be non-NULL. */
void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height);

/* Store the glyph position without scaling. */
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y);

/* Store the glyph render value in sixteenths. */
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri);

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo);

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color);

/* Enable mask bits and return the previous shared flag byte. */
u8 frEnableMode(s32 Flag);

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 Flag);

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 OtType);

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frEraseEffect(frFontHandle_t *pFRQ);

/* Shared DDS fade/jitter algorithm with Nocturne's two-parent fallback and
 * default 24-pixel advance. No DDS timed-control or lips-stop state exists here. */
s32 frDrawEffect(frFontHandle_t *parent, frTextHandle_t *glyph,
                 u8 threshold, u8 step);

/* Draw through the shared render flags with mode zero. */
s32 frDrawFont(frFontHandle_t *pFRQ);

s32 frDrawFontEx(frFontHandle_t *pFRQ, s8 Mode);

/* Nocturne's ASCII bearing adjustments are separate 26-entry X/Y tables. */
extern s32 frFontUpperYOffset[26];

extern s32 frFontUpperXOffset[26];

extern s32 frFontLowerXOffset[26];

extern s32 frFontLowerYOffset[26];

extern u32 frDrawFontWork;

/* DDS1 frFontDrawGlyphChain supplies the shared child/parent traversal;
 * Nocturne adds its ASCII bearings and per-parent draw count, not DDS waits. */
s32 func_001C7E38(frFontHandle_t *glyph, s8 mode, u32 renderFlags);

/* Release queue slot 1 when the draw-buffer index's low byte is zero, otherwise
 * slot 0; return 0. This is buffer selection, not a current-font selection. */
s32 frFlipPacketBuf(dds3ProcessID_t *id);

/* Link chains with the renderer's relative-positioning option enabled. */
frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);

frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2, s32 Mode);

/* Bind the caller's font data into the reserved temporary entry. */
void frSetExtDic(u8 *pExtFontData);

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void);

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *pStr);

/* Sum one parent's child advances and signed spacing after every child, even
 * the last. Require a non-NULL parent; preserve the existing unsigned result. */
u32 frGetStrWidth(frFontHandle_t *pFRQ);

/* Sum all child advances/spacing from the chain origin. This is neither a line
 * count nor a maximum line width; require a non-NULL glyphChain. */
u32 frGetStrWidth2(frFontHandle_t *pFRQ);

/* Measure encoded glyph advances, retaining native modulo-2^32 accumulation. */
u32 frPreCalcStrWidth(s8 *pStr, s32 Style, s32 Mode);

/* Slots 0/1 use the shared default size; later slots read their resource width.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frGetDefaultFontWidth(u8 Style);

u16 frGetFontWidth(s32 slotIndex);

/* Slots 0/1 use the shared default size; later slots read their resource height.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frGetDefaultFontHeight(u8 Style);

u16 frGetFontHeight(s32 slotIndex);

/* Restore Nocturne's default vertical message spacing of 20. */
void frSetMessageDefStepY(void);

/* Set the vertical advance used by encoded-message line breaks. */
void frSetMessageStepY(u32 StepY);

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);

void frCreateVariableRequest(u8 VarNo, frMsgInfo_t *pMsgInfo);

void frCreateNewHandle(frMsgInfo_t *pMsgInfo);

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo);

void frPushTagChar(frMsgInfo_t *ctx, u16 code);

s32 frGetTagArgCount(s32 tagCode);

void frCopyTagArgs(s8 *text, s8 *dest, s32 maxArgs);

s32 frCalcStringHash(s8 *text);

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious);

u32 frGetMessageResult(u32 Flag);

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 VarNo, s32 pStr);

extern const char D_00535968[16];

extern const char D_00535978[16];

extern const char D_00535988[16];

extern u32 gMessageRequestResult;

extern s8 gMessageSpaceX;

extern u32 gMessageStepY;

extern s32 frDefaultFontHandle;

extern u32 frSpaceAdvanceOffset;

extern u32 D_0054F418;

extern u8 gModeFlag;

extern u32 gFontDrawOT;

extern u8 frSetFontString_style;

extern u32 frDrawFontWork;
/* End of declarations from src/smt3/interface/frFont.c */

/* Declarations from dds-decomp src/dds2/game/code_0019E138.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_SDF_IMAGESET_MEM_CHIP 1

#define SMT3_DEFINES_SDF_IMAGESET_MEM_HANDLE 1

#define SMT3_DEFINES_SDF_IMAGESET_MEM_IGNORE 1

#define SMT3_DEFINES_SdfImageSetMem 1

#define SMT3_DEFINES_frFontAtlas_t 1

#define SMT3_DEFINES_frFontData_t 1

#define SMT3_DEFINES_frFontDmaTag_t 1

#define SMT3_DEFINES_frFontGsPacket_t 1

#define SMT3_DEFINES_frFontGsWrite_t 1

#define SMT3_DEFINES_frFontHeader_t 1

#define SMT3_DEFINES_frFontImage_t 1

#define SMT3_DEFINES_frFontNodeHeader_t 1

#define SMT3_DEFINES_frFontResource_t 1

#define SMT3_DEFINES_frFontSpriteCorner_t 1

#define SMT3_DEFINES_frFontSpritePacketView_t 1

#define SMT3_DEFINES_frFontSpritePacket_t 1

#define SMT3_DEFINES_frFontSystem_t 1

#define SMT3_DEFINES_frMsgInfo_t 1

#define SMT3_DEFINES_frTextBank_t 1

#define SMT3_DEFINES_frTextIndex_t 1

#define SMT3_DEFINES_sdfImageRequest_t 1

#include "scr.h"

#include "sdk_libc.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_state_query.h"

#include "dds/sdf_gs_packet.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_file.h"

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

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

/* Return the scheduler state, mapping unregistered or invalid states to DETACHED. */
dds3TaskState_t dds3GetProcessStat(const struct dds3ProcessID_t *task);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

/* Both entry points return one caller-owned texture reference. A nonzero
 * resourceKey reuses a live matching texture and increments its count; a new
 * texture starts at one. Release the returned reference once with
 * sdfTexReleaseReference or sdfTexFreeDelayed. */
/* For a new texture, copy payload bytes into queue-owned storage before return. */
struct sdfTexHandle_t *sdfTexSet(const SdfTextureFileHeader *header);

#include "dds/sdk_ps2.h"

#include "dds/scr.h"

#include "dds/common.h"

#include "dds/itf_bit_range.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/itf_text_vector.h"

#include "dds/itf_text_slots_api.h"

#include "dds/fr_font_text_slots.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_header.h"

#include "dds/itf_gs_state_packet.h"

#include "dds/fr_font.h"

#include "dds/fr_font_measure.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_state.h"

#include "dds/dds3_task_state_query.h"

#include "dds/sdf_heap_api.h"

#include "dds/itf.h"

#include "dds/itf_panel_api.h"

#include "dds/itf_mem_node.h"

#include "dds/itf_resource_segments.h"

#include "dds/sdf.h"

#include "dds/itf_panel_draw.h"

#include "dds/sdf_texture_file.h"

#include "dds/btl.h"

#define ITF_BYTE_MASK 0xFF

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

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

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

#include "dds/sdf_packet_alloc.h"

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern u8 *datSkillMessages;

extern u8 *datItemMessages;

extern void itfSetTextDrawLimit(s32);

extern _FRQ *itfDrawBankTextWithSegmentCounter(s32, s32, s32, u16, FrFontTextBank *, s32);

extern _FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *);

extern s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);

extern void itfReleaseFontTestTaskResources(dds3ProcessID_t *task);

extern u32 D_0043658C;

extern sdfTexHandle_t *itfBackgroundSpriteTexture;

extern u8 D_00436580[];

extern u8 D_003B4378[];

extern u8 D_003B4380[];

extern _FRQ *frFontLinkGlyph(_FRQ *, _FRQ *, s32);

extern u8 D_003B4378[];

extern u8 D_003B4380[];

extern u32 D_0043654C;

extern u32 itfTextLipsUnitId;

extern u32 itfTextLipsMotionId;

extern dds3ProcessID_t *itfFontTestScriptTask;

extern s32 itfScanEncodedTextControls(FrFontCtx *args);

void frFontEnsureSlotLoaded(s32 id, const char *path);

#include "dds/sdf_debug_api.h"

extern char D_00414C50[];

extern u16 D_003B2F58[];

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *stream);

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *stream);

extern void frFontCheckPendingGlyphState(FrFontCtx *);

extern void frFontAppendIndexedTextToGlyphChain(u8, FrFontCtx *);

extern void mnuSetTitleVoicePrefixIndex(s32);

extern void mnuPlayTitleVoiceFile(char *);

extern s32 mnuGetTitleEffectFrameCounter(void);

/* Non-GP data import; the native parser consumes its first sequence word. */
extern s32 D_003B2F28[];

extern s32 D_0043656C;

extern void p2sdPlaySE(s32, s32, s32);

s32 itfProcessEncodedTextControl(s32 code, FrFontCtx *stream);

extern s8 frFontEncodedTextGlyphSpacing;

_FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *stream);

extern s8 frFontEncodedTextGlyphSpacing;

_FRQ *itfDrawPlainEncodedTextWithByteColors(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *unusedSub);

/* Return the unit identifier saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsUnitId(void);

/* Return the motion number saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsMotionId(void);

/* Skip one encoded-text control without executing it; returns 1 for the stop codes and 2 for the voice trigger. */
s32 itfSkipEncodedTextOpcode(s32 code, FrFontCtx *stream);

s32 itfScanEncodedTextControls(FrFontCtx *stream);

s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub);

_FRQ *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, s32 entryFlag, u32 colors, const char *source);

void frwLoadStaffFont(void);

void frwFreeStaffFont(void);

/* DDS2 uses the larger 0xA10-entry glyph table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 glyphCode);

/* Preserve single-byte text and substitute 0x8080 for unmapped glyph pairs. */
void itfConvertText(u8 *output, const char *input);

/* Build a glyph with a fixed 16x18 cell, then attach it to its parent. */
void itfAttachGlyph16x18(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent);

_FRQ *frwGetFontEx(s32 x, s32 y, s32 depth, u32 colors, const char *text,
                         s8 glyphMode, s8 sharedFlag, _FRQ *previousGlyph);

_FRQ *frwGetSmallFontFRQ();

/* Create positioned, colored text with measured spacing and link it to parent. */
_FRQ *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
                           const char *text, _FRQ *parent);

/* Decode two-byte glyph codes before building and linking the text glyph. */
_FRQ *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth, u32 colors, const u8 *text, _FRQ *parent);

_FRQ *itfCreateFont2TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph);

/* Build text with font index 3; retain the existing glyph chain's spacing. */
_FRQ *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, const char *text, _FRQ *previousGlyph);

/* Build font-index-3 text with signed spacing -2 and return its glyph chain. */
_FRQ *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph);

/* The sequel uses the negative flag variant on its alternate 12x16 glyph. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent);

/* Count 0xFx0F separators in the packed text stream. */
s32 itfCountTextSeparators(const char *text);

/* Copy one plain-text segment; other two-byte control codes pass through. */
void itfCopyTextSegment(const char *src, char *dst, s32 segmentIndex);

/* Cycling text cache follows the shared empty-string bytes. */
extern u16 D_00436582;

extern s32 D_00436584;

extern s32 D_00436588;

extern s32 frFontCountGlyphBands(_FRQ *text);

extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);

/* Cycle a segment counter every 30 calls, but draw the full bank text stream.
 * Return NULL for absent text; optionally center the resulting glyph chain. */
_FRQ *itfDrawBankTextWithSegmentCounter(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags);

_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags);

_FRQ *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags);

_FRQ *itfDrawUnderscoreTextSegment();

u32 itfReturnZero(void);

void func_001A0040(void);

void func_001A0048(void);

s32 itfReturnThirdCallbackValue(s32 unused0, s32 unused1, s32 value);

extern s32 D_00438F20;

/* Return the byte count saved by the most recent packed-resource decode. */
s32 itfGetLastDecodedByteCount(void);

u32 frGetFontHeader(u32 resourceBase);

/* Read the opaque header word at 0x10; its role is not established. */
u32 itfGetPackedResourceHeaderWord(u32 resourceBase);

/* Acquire the background texture, then release the temporary file allocation. */
void nbLoadDeadMoonTex(void);

/* Drop the held texture reference without clearing the global. */
void itfReleaseBackgroundSpriteTexture(void);

struct EffSpriteUV;

struct sdfTexHandle_t;

union EffSpriteColor;

extern void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height,
                                      const struct EffSpriteUV *uvRect,
                                      const union EffSpriteColor *color, u32 flipFlags,
                                      u32 blendKind, s32 mode, struct sdfTexHandle_t *texture,
                                      s32 surfaceId);

/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void);

/* Start the background draw task and host-file font-test process. */
void itfStartFontTestScene(void);

/* Advance/retain the test glyph and release its background texture reference. */
void itfReleaseFontTestTaskResources(dds3ProcessID_t *task);

/* Draw first, then report all bits set only for a task awaiting destruction. */
s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(ItfMesPool *pool, mesflow_tag *nodes, s32 count, s32 stride);

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
mesflow_tag *itfMiscLinkTake(ItfMesPool *pool);

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(mesflow_tag *node, ItfMesPool *pool);

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(DrawColorRec *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const TextVector *input, s32 count);

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(_FRQ *entry, u32 color);

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(_FRQ *entry, u32 colorBits);

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(_FRQ *entry, u32 xOffset, u32 yOffset);

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *path);

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(DrawVertex *vertices, DrawColorRec *colors, s32 xOffset, s32 yOffset, u32 tail, sdfDrawTag_t *command);

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

/* Draw three adjacent quads using zero-alpha/source-alpha edge color records.
 * Border widths and resulting signed middle width are not clamped. */
void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(DrawVertex *vertices, f32 *uvs, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command);

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(DrawVertex *vertices, DrawColorRec *uv,
                            DrawColorRec *colors, s32 tail,
                            const sdfTexHandle_t *borrowedTexture, s32 flag,
                            sdfDrawTag_t *command);

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(DrawVertex *vertices, DrawColorRec *uv, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command);

/* Despite the current name, PRIM selects a two-corner sprite, not a line. */
void itfMiscDrawSpritePlanePrim(DrawVertex *vertices, DrawColorRec *colors, u32 tail, s32 flag, sdfDrawTag_t *command);

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

extern void *sdf2DMakeStdPacket(void *packet, s32 packetBytes);

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *list, s64 value, s32 flag);

extern s64 D_003B4390[];

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *list, s32 index, s32 flag);

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *list, s32 flag, s32 wms, s32 wmt, s32 minu, s32 maxu, s32 minv, s32 maxv);

extern u8 D_00436598[8];

extern u8 D_004365A0[8];

extern u8 D_003B43A8[];

extern u8 D_003B43B0[];

extern TextVector D_003B43B8[4];

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 value, s32 limit, s32 highlight,
                   DrawVertex *bounds, u32 tail, DrawColorRec *filledColor,
                   DrawColorRec *remainingColor, sdfDrawTag_t *command);

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *task);

u32 itfCamp(void);

void itfCampStartProcess(void);

extern s32 D_003B44B0[];

dds_itfpanel_t *itfCreateUiSpriteByKind(s32 kind, const sdfTexHandle_t *borrowedTexture);
/* End of declarations from src/dds2/game/code_0019E138.c */

/* Declarations from dds-decomp src/dds2/interface/frFont.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#define SMT3_DEFINES_SDF_GIF_REGISTER_AD 1

#define SMT3_DEFINES_SDF_GS_ALPHA_1 1

#define SMT3_DEFINES_SDF_GS_ALPHA_2 1

#define SMT3_DEFINES_SDF_GS_BITBLTBUF 1

#define SMT3_DEFINES_SDF_GS_CLAMP_1 1

#define SMT3_DEFINES_SDF_GS_CLAMP_2 1

#define SMT3_DEFINES_SDF_GS_COLCLAMP 1

#define SMT3_DEFINES_SDF_GS_DTHE 1

#define SMT3_DEFINES_SDF_GS_FRAME_1 1

#define SMT3_DEFINES_SDF_GS_FRAME_2 1

#define SMT3_DEFINES_SDF_GS_PRIM 1

#define SMT3_DEFINES_SDF_GS_PRMODECONT 1

#define SMT3_DEFINES_SDF_GS_RGBAQ 1

#define SMT3_DEFINES_SDF_GS_SCISSOR_1 1

#define SMT3_DEFINES_SDF_GS_SCISSOR_2 1

#define SMT3_DEFINES_SDF_GS_TEST_1 1

#define SMT3_DEFINES_SDF_GS_TEST_2 1

#define SMT3_DEFINES_SDF_GS_TEX0_1 1

#define SMT3_DEFINES_SDF_GS_TEX0_2 1

#define SMT3_DEFINES_SDF_GS_TEX1_1 1

#define SMT3_DEFINES_SDF_GS_TEX1_2 1

#define SMT3_DEFINES_SDF_GS_TEXA 1

#define SMT3_DEFINES_SDF_GS_TEXFLUSH 1

#define SMT3_DEFINES_SDF_GS_TRXDIR 1

#define SMT3_DEFINES_SDF_GS_TRXPOS 1

#define SMT3_DEFINES_SDF_GS_TRXREG 1

#define SMT3_DEFINES_SDF_GS_XYOFFSET_1 1

#define SMT3_DEFINES_SDF_GS_XYOFFSET_2 1

#define SMT3_DEFINES_SDF_GS_XYZ2 1

#define SMT3_DEFINES_SDF_GS_ZBUF_1 1

#define SMT3_DEFINES_SDF_GS_ZBUF_2 1

#include "dds/sdk_ps2.h"

#include "dds/common.h"

#include "dds/fr_font_draw_glyph.h"

#include "dds/fr_font_position.h"

#include "dds/fr_font_text_slots.h"

#include "dds/sdf_image_upload.h"

#include "dds/sdf_chip.h"

#include "dds/fr_font_measure.h"

#include "dds/fr_font_glyph_measure.h"

#include "dds/fr_font_text_measure_mode.h"

#include "dds/fr_font_clone_entry_resource.h"

#include "dds/itf.h"

#include "dds/fr_font_temporary_entry_api.h"

#include "dds/itf_mem_node.h"

#include "dds/itf_resource_segments.h"

#include "dds/kwln.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_task_runner.h"

#include "dds/btl.h"

#include "dds/mnu_title_sound.h"

/* Declared before first use (were implicit int). */
extern void evtLipsStopFunction(void);

void frFontCreateContext();

extern u8 D_00436578[];

extern _FRQ *frFontReleaseGlyphChain(_FRQ *glyph);

void frFontInitGlyph(_FRQ *);

u32 frFontGetGlyphCellWidth(u8);

u32 frFontGetGlyphCellHeight(u8);

extern u8 D_00436570;

_FRQ *frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *head);

_TEXT *frFontCreateGlyphFromCode(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption);

#define FR_FONT_CONTEXT_ENABLE_VALUE 0x80

#define FR_FONT_FADE_COMPLETE 0x80

#define FR_FONT_FADE_ADVANCE_SCALE 22

#define FR_FONT_JITTER_OPTION 1

#define FR_FONT_JITTER_PERIOD 5

#define FR_FONT_JITTER_CENTER 2

#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C

#define FR_FONT_POSITION_SHIFT 4

#define FR_FONT_POSITION_BAND_HEIGHT 0x64

/* Release parents backward and children forward, returning both to their pools.
 * Children with a borrowed source record bypass cached-item reference release. Return NULL. */
_FRQ *frFontReleaseGlyphChain(_FRQ *glyph);

/* Glyph setup queries its initial Y here for flag 0x10; retail always returns zero. */
u32 frFontGetInitialGlyphY(s8 fontIndex, s32 glyphIndex);

extern u16 frFontGetSlotCellWidth(s32 index);

/* Fill cell dimensions, advance and horizontal bearing for one glyph. */
void frFontFillGlyphMetrics(FrFontGlyphMeasureWork *work, s32 glyphIndex);

extern u16 frFontGetSlotCellHeight(s32 index);

/* Reset standalone chain-head state and context controls; leave drawCount intact. */
void frFontInitGlyph(_FRQ *glyph);

extern void frFontFillGlyphMetrics(FrFontGlyphMeasureWork *work, s32 glyphIndex);

_TEXT *frFontCreateGlyphFromCode(u16 glyphId, s32 fontIndexArg, u8 firstOption, u8 secondOption);

/* Build text and position its chain after the previous glyph. A NULL build
 * preserves the previous chain. */
_FRQ *frFontAppendTextToGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *previousGlyph);

/* Build a glyph chain for a byte string, appending to a supplied head. */
_FRQ *frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *head);

/* Enable the glyph mode and initialize its encoded byte to the cap. */
void frFontEnableContextMode(_FRQ *glyph);

/* Advance one rendered glyph's fade state and return its transient X jitter. */
s32 frFontAdvanceChildFadeAndGetJitter(_FRQ *parent, _TEXT *glyph, u8 threshold, u8 step);

/* Draw through the shared render flags with mode zero. */
s32 frFontDrawGlyphInDefaultMode(_FRQ *glyph);

/* Pass the selected mode and current shared render-flag word to the renderer. */
s32 frFontDrawGlyphWithSharedFlags(_FRQ *glyph, s8 mode);

/* Draw every child glyph, advance each parent chain, and report its total
 * child-glyph count only while all control/fade conditions remain ready. DDS2 additionally
 * suppresses one result per positive draw-delay count. */
s32 frFontDrawGlyphChain(_FRQ *glyph, s8 mode, u32 renderFlags);

extern void frFontFillGlyphMetrics(FrFontGlyphMeasureWork *work, s32 glyphIndex);

s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, FrFontTextMeasureMode mode);

/* Slots 0/1 use the shared default size; later slots read their resource width.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frFontGetGlyphCellWidth(u8 fontIndex);

/* Slots 0/1 use the shared default size; later slots read their resource height.
 * Retain the nested signed check and the absence of upper-bound validation. */
u32 frFontGetGlyphCellHeight(u8 fontIndex);

/* Restore this game's cursor spacing (DDS1: 21, DDS2: 25). */
void frFontResetContextCursorSpacing(void);

/* Store unscaled cursor spacing; advancement applies its factor of eight later. */
void frFontSetContextCursorSpacing(u32 spacing);

s32 frFontMeasureLineWidth(s32 requestedLine, const _FRQ *glyph);

/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. DDS2 advances
 * to the first node outside its y-band, unlike DDS1's single-node advance. */
void frFontGetGlyphPositionAtIndex(FrFontPosition *position, s32 requestedIndex, const _FRQ *glyph);

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. Preserve the old-style implicit-argument interface. */
/* Old-style definition: callers invoke it without arguments and rely on $a0. */
void frFontCreateContext();
/* End of declarations from src/dds2/interface/frFont.c */

#ifdef NON_MATCHING
/* runs past the next function at 0x001923D8 */
void func_r1_0017D778(s32 StyleNo, s32 Width) {
    frFontData_t *entry = &gFT.entries[StyleNo & FR_FONT_BYTE_MASK];

    entry->prop[0] = FR_FONT_ENTRY_ENABLED;
    entry->prop[1] = Width + 1;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017D778);
#endif

/* Reverse the four byte positions of a 32-bit word. */
u32 frSwapColor(u32 Col) {
    u32 shiftedWord = Col >> FR_FONT_BYTE_SHIFT;
    u32 lowerMiddleByte = Col & FR_FONT_MIDDLE_BYTE_MASK;
    u32 outerBytes = Col << FR_FONT_OUTER_BYTE_SHIFT;

    Col >>= FR_FONT_OUTER_BYTE_SHIFT;
    shiftedWord &= FR_FONT_MIDDLE_BYTE_MASK;
    lowerMiddleByte <<= FR_FONT_BYTE_SHIFT;
    outerBytes |= Col;
    shiftedWord |= lowerMiddleByte;
    return outerBytes | shiftedWord;
}

/* Return the zero-based highest set bit; both zero and one return zero. */
s32 frConvertVal2N(u32 Val) {
    s32 shiftCount = 0;

    if (Val == 0) {
        return 0;
    }
    do {
        Val = Val >> 1;
        shiftCount += 1;
    } while (Val != 0);
    return (shiftCount + FR_FONT_BYTE_MASK) & FR_FONT_BYTE_MASK;
}

void frCreateDrawEnvPacket(frFontGsPacket_t *pDrawEnvPkt) {
    QUAD_ZERO(pDrawEnvPkt->dma.value);
    pDrawEnvPkt->dma.words[0] = 0x70000007;
    pDrawEnvPkt->dma.words[2] = 0;
    pDrawEnvPkt->dma.words[3] = 0x50000007;
    pDrawEnvPkt->gifTag = 0x1000000000008006ULL;
    pDrawEnvPkt->gifRegisters = 0xE;
    pDrawEnvPkt->writes[0].value = 0;
    pDrawEnvPkt->writes[0].reg = 0x3F;
    pDrawEnvPkt->writes[1].value = 0x44;
    pDrawEnvPkt->writes[1].reg = 0x42;
    pDrawEnvPkt->writes[2].value = 0;
    pDrawEnvPkt->writes[2].reg = 0x49;
    pDrawEnvPkt->writes[3].value = 0x8080;
    pDrawEnvPkt->writes[3].reg = 0x3B;
    pDrawEnvPkt->writes[4].value = 0;
    pDrawEnvPkt->writes[4].reg = 0x4A;
    pDrawEnvPkt->writes[5].value = 0x517ED;
    pDrawEnvPkt->writes[5].reg = 0x47;
}

void frCreateDrawFontPacket_20550848(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const frFontUvRect_t *uv,
    const frFontAtlas_t *atlas, s32 surface) {
    frFontSpritePacketView_t *packet;
    sdfDrawTag_t *list;
    sdfGrOt_t *drawSurface;
    u32 reversedColor;
    s32 right, bottom;
    u64 rgbaq;
    u64 z;
    u64 texBase, texWidth, texBuffer, texHeight;

    reversedColor = frSwapColor(color);
    packet = (frFontSpritePacketView_t *)sdfGrAllocPacketBuff(sizeof(frFontSpritePacketView_t));
    packet->words[0] = 0x70000008;
    packet->words[3] = 0x50000008;
    packet->sprite.tex1.value = ((u64)filtered << 5) | ((u64)filtered << 6);
    packet->sprite.setupTag = 0x10AF400000000003ULL;
    packet->sprite.setupRegisters = 0xE;
    packet->sprite.tex1.reg = 0x14;
    packet->words[1] = 0;
    packet->words[2] = 0;
    x += 0x7000;
    width <<= 4;
    y += 0x7900;
    height <<= 4;
    right = x + width;
    bottom = y + height;
    texBase = atlas->bufferBase | 0x8000000000000000ULL;
    texWidth = ((u64)atlas->widthExponent << 26) | 0x400000000ULL;
    texBuffer = ((u64)atlas->bufferWidth << 14) | 0x1400000;
    texHeight = ((u64)atlas->heightExponent << 30) | ((u64)gFT.imageBuffers[style] << 37);
    texBase |= texWidth;
    texBuffer |= texHeight;
    packet->sprite.tex0.reg = 6;
    packet->sprite.clamp.reg = 8;
    packet->sprite.spriteTag = 0x3400000000008002ULL;
    packet->sprite.spriteRegisters = 0x413;
    packet->sprite.tex0.value = texBase | texBuffer;
    packet->sprite.clamp.value = 0;
    rgbaq = reversedColor | 0x3F80000000000000ULL;
    z = (u64)depth << 32;
    packet->sprite.corners[0].uv = (uv->v1 << 16) | uv->u1;
    packet->sprite.corners[0].rgbaq = rgbaq;
    packet->sprite.corners[0].xyz = (u32)((right & 0xFFFF) | (bottom << 16)) | z;
    packet->sprite.corners[1].uv = (uv->v0 << 16) | uv->u0;
    packet->sprite.corners[1].rgbaq = rgbaq;
    packet->sprite.corners[1].xyz = (u32)((x & 0xFFFF) | (y << 16)) | z;
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(sdfDrawTag_t));
    sdfDrawTagInit(list);
    sdfDrawTagLink(list, EE_ADDR(packet));
    drawSurface = &sdfGrOtTable[surface];
    drawSurface->regFunc(drawSurface, list);
}

/* Submit glyph-relative coordinates and the cache node's owned UV rectangle. */
void frCreateUserFontPacket(s32 X, s32 Y, s32 Z, frTextHandle_t *pText,
                  s32 Ot) {
    frCreateDrawFontPacket_20550848(X + pText->x, Y + pText->y,
                 pText->cellDimensions.cellWidth,
                 pText->cellDimensions.cellHeight >> FR_FONT_GLYPH_HEIGHT_SHIFT,
                 pText->renderValueOrSetupOrShade.setupBytes.firstOption, pText->parentDimensionsOrRenderWord.renderWord, Z, 1,
                 &pText->cachedItem->font_mem->uv, &gFT.atlas, Ot);
}

/* Upload an image to GS memory at the given buffer, serialized by the GS semaphore. */
void frCopyFontClutToVram(s32 GS_LocalAddr, saddr_t pClut) {
    u8 loadImage[0x60];

    sceGsSetDefLoadImage(loadImage, (s16)GS_LocalAddr, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, pClut);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
}

#ifdef NON_MATCHING
/* 6 of 54 words differ (first at +0x20) */
void frClearTexBuf(void) {
    u8 loadImage[0x60];
    sdfMemHandle_t *allocation;
    u8 *pixels;
    s32 byteCount = gFT.atlas.height * gFT.atlas.width;

    byteCount = (u32)byteCount >> 1;
    allocation = sdfMemAllocDirect(byteCount);
    pixels = EE_ADDR_AS(u8 *, sdfMemLock(allocation));
    memset(pixels, 0x11, byteCount);
    sceGsSetDefLoadImage(loadImage, gFT.atlas.bufferBase, gFT.atlas.bufferWidth,
                         FR_FONT_PIXEL_FORMAT, 0, 0, gFT.atlas.width, gFT.atlas.height);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(loadImage, EE_ADDR_S(pixels));
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
    sdfMemFreeDirect(allocation);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frClearTexBuf);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017DCC8);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017DD08);

#ifdef NON_MATCHING
/* 6 of 23 words differ (first at +0x1C) */
void frDestroyFontTable(dds3ProcessID_t *id) {
    frDelStyleAll();
    frDestroyFontRequestNow(gFT.glyphSlots[0]);
    frDestroyFontRequestNow(gFT.glyphSlots[1]);
    frFreeFontTable(gFT.itemPool);
    frFreeFontTable(gFT.glyphPool);
    fmDestroyFontMemControl();
    sdfGsMemFree(gFT.textureHead0);
    sdfGsMemFree(gFT.textureHead1);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontTable);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017DF88);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E070);

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void) {
    frLoadFontData(0, "/font/font0.fnt");
    frLoadFontData(1, "/font/font1.fnt");
    frLoadFontData(2, "/font/font2.fnt");
    frLoadFontData(3, "/font/font3.fnt");
}

#ifdef NON_MATCHING
/* 1 of 23 words differ (first at +0x2C) */
void frDelStyleAll(void) {
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (gFT.entries[fontIndex].resource != NULL) {
            frDelFontStyle((u8)fontIndex);
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frDelStyleAll);
#endif

#ifdef NON_MATCHING
/* 1 of 48 words differ (first at +0x28) */
void frSetFontClut(void) {
    u32 imageWords[FR_FONT_IMAGE_WORD_COUNT];
    s32 bufferIndex = 0;
    s32 sourceWordOffset = 0;
    frFontSystem_t *work = &gFT;
    u32 *imageBuffer = work->imageBuffers;
    const u8 *sourceTable = frFontClutTable;

    do {
        u32 *outputWord = imageWords;
        const u8 *sourceBytes = EE_ADDR_AS(const u8 *, (sourceWordOffset * FR_FONT_IMAGE_WORD_BYTES + EE_ADDR(sourceTable)));
        s32 wordCountdown = FR_FONT_IMAGE_LAST_WORD;

        do {
            *outputWord = (((sourceBytes[3] << 8) | sourceBytes[2]) << 8 |
                       sourceBytes[1]) << 8 | sourceBytes[0];
            sourceBytes += FR_FONT_IMAGE_WORD_BYTES;
            outputWord++;
            wordCountdown--;
        } while (wordCountdown >= 0);

        frCopyFontClutToVram(*imageBuffer++, EE_ADDR_S(imageWords));
        bufferIndex++;
        sourceWordOffset += FR_FONT_IMAGE_WORD_COUNT;
    } while (bufferIndex < FR_FONT_IMAGE_BUFFER_COUNT);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetFontClut);
#endif

#ifdef NON_MATCHING
/* runs past the next function at 0x00192ED0 */
void func_r1_0017E230(frTextHandle_t *pText) {
    GsFont_t *cachedItem = pText->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->used_count == 0) {
            gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex].trans[cachedItem->font_no] = NULL;
            fmRetFontMem(cachedItem->font_mem);
            gFT.cachedItemCount--;
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E230);
#endif

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ) {
    if (frEraseEffect(pFRQ) != 0) {
        return pFRQ;
    }
    return frDestroyFontRequestNow(pFRQ);
}

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E2E0);

/* Link without repositioning into the draw-buffer-indexed queue; return 0.
 * The low-byte buffer index is used directly, without a two-slot bounds check. */
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ) {
    frFontHandle_t **queueSlot = &gFT.glyphSlots[dds3GetFrameID() & 0xFF];

    *queueSlot = frCombineFontRequestEx(*queueSlot, pFRQ, 0);
    return 0;
}

/* Return the live count of retained font-cache records. */
s32 frGetGsFontNum(void) {
    return gFT.cachedItemCount;
}

/* Return the live pooled child-glyph count. */
s32 frGetTextHandleNum(void) {
    return gFT.itemCount;
}

u32 frGetFontHandleNum(void) {
    return gFT.glyphCount;
}

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E478);

INCLUDE_ASM(const s32, "interface/frFont", func_00193130);

INCLUDE_ASM(const s32, "interface/frFont", func_001931E0);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E868);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E888);

#ifdef NON_MATCHING
/* 15 of 20 words differ (first at +0x4) */
frFontHandle_t *func_r1_0017E9C8(GsFont_t *pFont, frFontHandle_t *pFRQ) {
    frFontHandle_t *referenceChain = frSetUserImage(pFont, 0);

    if (referenceChain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, referenceChain);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017E9C8);
#endif

#ifdef NON_MATCHING
/* 2 of 56 words differ (first at +0x80) */
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode) {
    s32 imagePosition[2];
    sdfImageRequest_t uploadRequest;
    FontMem_t *atlasNode;
    GsFont_t *cachedItem;

    atlasNode = fmRentFontMem();
    cachedItem = atlasNode->user_data;
    /* Convert fixed-point atlas placement to integer image coordinates. */
    imagePosition[0] = atlasNode->uv.u0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    imagePosition[1] = atlasNode->uv.v0 >> FR_FONT_ATLAS_POSITION_SHIFT;
    cachedItem->used_count = 1;
    cachedItem->font_mem = atlasNode;
    if (DataFreeMode == 0) {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_IGNORE;
    } else {
        uploadRequest.allocationMode = SDF_IMAGESET_MEM_CHIP;
    }
    uploadRequest.pixels = pData;
    uploadRequest.format = FR_FONT_PIXEL_FORMAT;
    uploadRequest.width = Width;
    uploadRequest.height = Height;
    uploadRequest.bufferWidth = gFT.atlas.width;
    uploadRequest.destination = gFT.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
    uploadRequest.x = imagePosition[0];
    uploadRequest.y = imagePosition[1];
    sdfImageSetReq(&uploadRequest);
    return cachedItem;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frTranslateUserData);
#endif

#ifdef NON_MATCHING
/* 30 of 31 words differ (first at +0x4) */
u8 *func_r1_0017EAF8(u8 Style, s32 FontNo) {
    frFontData_t *entry = &gFT.entries[Style];
    u8 *clonedResource = sdfChipAllocDirect(FR_FONT_RESOURCE_CLONE_BYTES);
    frFontImage_t segments;

#ifdef FIX_BUGS
    /* BUG: a glyph the font lacks reads its bit range from past the offset table. */
    if ((u32)FontNo >= entry->resourceHeader->widthCount) {
        FontNo = 0;
    }
#endif
    frGetFontImagePtrs(entry->resource, &segments);
    frDecodeFontRow(clonedResource, FontNo, entry->resource, &segments);
    return clonedResource;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017EAF8);
#endif

#ifdef NON_MATCHING
/* 45 of 61 words differ (first at +0x3C) */
GsFont_t *func_r1_0017EB70(frTextHandle_t *pText, s32 FontNo) {
    frFontData_t *entry = &gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex];
    GsFont_t *cachedItem;
    u8 *clonedResource;

#ifdef FIX_BUGS
    /* BUG: a glyph the font lacks (the untranslated debug text's kanji) reads
     * past trans[] and increments a word of glyph data as a GsFont_t. Draw a space. */
    if ((u32)FontNo >= entry->resourceHeader->widthCount) {
        FontNo = 0;
    }
#endif
    cachedItem = entry->trans[FontNo];
    if (cachedItem != NULL) {
        cachedItem->used_count++;
        return cachedItem;
    }
    clonedResource = frDecodeFontImage(pText->renderValueOrSetupOrShade.setupBytes.fontIndex, FontNo);
    cachedItem = frTranslateUserData(frGetFontWidth(pText->renderValueOrSetupOrShade.setupBytes.fontIndex), frGetFontHeight(pText->renderValueOrSetupOrShade.setupBytes.fontIndex), clonedResource, 1);
    cachedItem->font_no = FontNo;
    entry->trans[FontNo] = cachedItem;
    gFT.cachedItemCount++;
    return cachedItem;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017EB70);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017EC60);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017ECF0);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017ED30);

#ifdef NON_MATCHING
/* 13 of 26 words differ (first at +0x24) */
frFontHandle_t *func_r1_0017EE78(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ) {
    frFontHandle_t *chain = frSetFontString(pStr, Style, ColNo, Tp, NULL, 0);

    if (chain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, chain);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017EE78);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_00193A58);

INCLUDE_ASM(const s32, "interface/frFont", func_00193B60);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F078);

#ifdef NON_MATCHING
/* 2 of 8 words differ (first at +0xC) */
void func_r1_0017F0B0(_FRQ *glyph) {
    glyph->contextModeEnabled = 1;
    frSetDrawSpeed(glyph, FR_FONT_CONTEXT_ENABLE_VALUE);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F0B0);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F0D0);

#ifdef NON_MATCHING
/* 1 of 11 words differ (first at +0x14) */
void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space) {
    pFRQ->glyphCodeOrContext.byteRoles.spacing = Space;
    pFRQ->advance = frGetStrWidth(pFRQ);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetFontSpace);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F138);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F180);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F1C8);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F2A8);

/* Store the glyph position without scaling. */
void frSetDrawPosition(frFontHandle_t *pFRQ, u32 X, u32 Y) {
    pFRQ->x = X;
    pFRQ->y = Y;
}

/* Store the glyph render value in sixteenths. */
void frSetPriority(frFontHandle_t *pFRQ, u32 Pri) {
    pFRQ->renderValueOrSetupOrShade.renderValue = Pri >> FR_FONT_CONTEXT_VALUE_SHIFT;
}

/* Assign the first option byte across all visited children; NULL is a no-op. */
void frSetFontColorNo(frFontHandle_t *pFRQ, u8 ColorNo) {
    frTextHandle_t *childGlyph;

    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            childGlyph->renderValueOrSetupOrShade.setupBytes.firstOption = ColorNo;
        }
    }
}

/* Replace each child's full color word across the parent glyph chain. */
void frSetFontColor(frFontHandle_t *pFRQ, u32 Color) {
    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        frTextHandle_t *childNode;
        for (childNode = pFRQ->firstChild; childNode != NULL; childNode = childNode->next) {
            childNode->parentDimensionsOrRenderWord.renderWord = Color;
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_001940D8);

/* Enable mask bits and return the previous shared flag byte. */
u8 frEnableMode(s32 Flag) {
    u8 previousFlags = gModeFlag;

    Flag |= previousFlags;
    gModeFlag = Flag;
    return previousFlags;
}

/* Clear flag bits from the shared font flag byte; returns the previous value. */
u8 frDisableMode(u8 Flag) {
    u8 previousFlags = gModeFlag;

    gModeFlag = previousFlags & ~Flag;
    return previousFlags;
}

/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 OtType) {
    gFontDrawOT = OtType;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00194158);

/* Step only second-option-2 children: decrease the low byte by eight, clamp at
 * zero and move y by sixteen while changing it. Return whether any step occurred,
 * not whether the resulting chain still has a nonzero fade value. */
s32 frEraseEffect(frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph;
    s32 didChange = 0;

    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            if (childGlyph->renderValueOrSetupOrShade.setupBytes.secondOption == FR_FONT_FADE_OPTION) {
                u32 glyphWord = childGlyph->parentDimensionsOrRenderWord.renderWord;
                s32 fadeValue = glyphWord & FR_FONT_BYTE_MASK;

                if (fadeValue != 0) {
                    fadeValue -= FR_FONT_FADE_VALUE_STEP;
                    if (fadeValue < 0) {
                        fadeValue = 0;
                    }
                    childGlyph->y += FR_FONT_FADE_Y_STEP;
                    childGlyph->parentDimensionsOrRenderWord.renderWord = (glyphWord & ~FR_FONT_BYTE_MASK) | fadeValue;
                    didChange = 1;
                }
            }
        }
    }
    return didChange;
}

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F4D0);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F818);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017F830);

INCLUDE_ASM(const s32, "interface/frFont", func_001945B8);

#ifdef NON_MATCHING
/* 4 of 20 words differ (first at +0x20) */
s32 func_r1_0017FBA0(dds3ProcessID_t *id) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    gFT.glyphSlots[queueIndex] = frDestroyFontRequestNow(gFT.glyphSlots[queueIndex]);
    return 0;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FBA0);
#endif

frFontHandle_t *func_r1_0017FBF0(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2) {
    return frCombineFontRequestEx(pFRQ1, pFRQ2, 1);
}

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FC08);

/* Bind the caller's font data into the reserved temporary entry. */
void frSetExtDic(u8 *pExtFontData) {
    frAddFontStyle(FR_FONT_TEMPORARY_SLOT, pExtFontData, 0);
}

/* Free the reserved temporary font entry. */
void frDestroyExtDic(void) {
    frDelFontStyle(FR_FONT_TEMPORARY_SLOT);
}

/* Count characters until a zero lead byte. Any negative signed lead consumes
 * two bytes without checking its trailer; input must be a valid packed stream. */
s32 frStrLen(s8 *pStr) {
    s32 characterCount = 0;

    while (*pStr != 0) {
        if (*pStr >= 0) {
            pStr++;
        } else {
            pStr += 2;
        }
        characterCount++;
    }
    return characterCount;
}

#ifdef NON_MATCHING
/* 1 of 13 words differ: retail reads the spacing byte at +2, the shared frFontHandle_t has it at +1 */
u32 frGetStrWidth(frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph = pFRQ->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = pFRQ->glyphCodeOrContext.byteRoles.spacing;

        do {
            totalAdvance += childGlyph->advance;
            childGlyph = childGlyph->next;
            totalAdvance += childSpacing;
        } while (childGlyph != NULL);
    }
    return totalAdvance;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth);
#endif

#ifdef NON_MATCHING
/* 1 of 20 words differ (first at +0x18) */
u32 frGetStrWidth2(frFontHandle_t *pFRQ) {
    frFontHandle_t *parentGlyph;
    frTextHandle_t *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = pFRQ->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
        childGlyph = parentGlyph->firstChild;
        if (childGlyph != NULL) {
            s8 childSpacing = parentGlyph->glyphCodeOrContext.byteRoles.spacing;

            do {
                totalAdvance += childGlyph->advance;
                childGlyph = childGlyph->next;
                totalAdvance += childSpacing;
            } while (childGlyph != NULL);
        }
    }
    return totalAdvance;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth2);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FDC8);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FEB8);

#ifdef NON_MATCHING
/* 8 of 9 words differ (first at +0x4) */
u16 func_r1_0017FFB8(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellWidth;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FFB8);
#endif

#ifdef NON_MATCHING
/* 8 of 9 words differ (first at +0x4) */
u16 func_r1_0017FFD8(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellHeight;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FFD8);
#endif

#ifdef NON_MATCHING
/* 1 of 3 words differ (first at +0x0) */
void func_r1_0017FFF8(void) {
    gMessageStepY = FR_FONT_DEFAULT_CURSOR_SPACING;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_0017FFF8);
#endif

void func_r1_00180008(u32 StepY) {
    gMessageStepY = StepY;
}

s32 func_r1_00180010(const _FRQ *glyph) {
    s32 count = 0;
    const _FRQ *currentNode;
    const _FRQ *scanNode;
    s32 bandLimit;
    s32 currentY;

    if (glyph == NULL) {
        return 0;
    }
    currentNode = glyph->chainHead;
    if (currentNode != NULL) {
        do {
            scanNode = currentNode;
            currentY = currentNode->y;
            if (currentNode != NULL) {
                bandLimit = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                if (currentY < bandLimit) {
                    do {
                        scanNode = scanNode->next;
                    } while (scanNode != NULL && scanNode->y < bandLimit);
                }
            }
            currentNode = currentNode->next;
            count++;
        } while (currentNode != NULL);
    }
    return count;
}

s32 func_r1_00180088(s32 requestedLine, const _FRQ *glyph) {
    const _FRQ *node;
    s32 bottom;
    s32 currentY;
    u32 width = 0;
    s32 line = 0;

    if (glyph == NULL) {
        return 0;
    }
    glyph = glyph->chainHead;
    if (glyph != NULL) {
        do {
            width = 0;
            node = glyph;
            currentY = glyph->y;
            /* Same null-tolerant band walk as the position query below. */
            if (glyph != NULL) {
                bottom = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                if (currentY < bottom) {
                    do {
                        width += frGetStrWidth(node);
                        node = node->next;
                    } while (node != NULL && node->y < bottom);
                }
            }
            if (line++ == requestedLine) {
                break;
            }
            glyph = glyph->next;
        } while (glyph != NULL);
    }
    return (s32)(width << FR_FONT_POSITION_SHIFT);
}

/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. DDS2 advances
 * to the first node outside its y-band, unlike DDS1's single-node advance. */
void frFontGetGlyphPositionAtIndex(FrFontPosition *position, s32 requestedIndex, const _FRQ *glyph) {
    s32 positionIndex = 0;
    const _FRQ *currentNode;
    const _FRQ *scanNode;
    s32 bandLimit;
    s32 currentY;

    if (glyph != NULL) {
        currentNode = glyph->chainHead;
        if (currentNode != NULL) {
            do {
                scanNode = currentNode;
                position->x = currentNode->x;
                position->y = currentNode->y;
                currentY = currentNode->y;
                if (currentNode != NULL) {
                    bandLimit = currentY + FR_FONT_POSITION_BAND_HEIGHT;
                    if (currentY < bandLimit) {
                        do {
                            scanNode = scanNode->next;
                        } while (scanNode != NULL && scanNode->y < bandLimit);
                    }
                }
                if (positionIndex++ == requestedIndex) {
                    break;
                }
                currentNode = scanNode;
            } while (currentNode != NULL);
        }
    }
}

/* Translate the complete next-linked chain to place its origin at x/y.
 * NULL glyphChain is a no-op; a non-NULL chain requires a non-NULL origin. */
void frFontMoveChainTo(s32 targetX, s32 targetY, _FRQ *glyphChain) {
    _FRQ *currentGlyph;
    s32 dx;
    s32 dy;

    if (glyphChain != NULL) {
        currentGlyph = glyphChain->chainHead;
        dx = targetX - currentGlyph->x;
        dy = targetY - currentGlyph->y;
        for (; currentGlyph != NULL; currentGlyph = currentGlyph->next) {
            currentGlyph->x += dx;
            currentGlyph->y += dy;
        }
    }
}

INCLUDE_ASM(const s32, "interface/frFont", frFontCenterGlyphChainAt);

/* Read the packed table count, then skip the table and its trailing metadata
 * word to reach the encoded text. The native parser copies that metadata even
 * though this lookup does not use its value. */
u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size) {
    u32 offset;
    u32 tableCount;
    u32 textMetadata;

    if (Index >= pData->count) {
        return NULL;
    }
    offset = pData->entries[Index].offset + 0x38;
    tableCount = 0;
    memcpy(&tableCount, (u8 *)pData + offset, 2);
    offset += 4;
    offset += tableCount * 4;
    memcpy(&textMetadata, (u8 *)pData + offset, sizeof(textMetadata));
    offset += sizeof(textMetadata);
    return (u8 *)pData + offset;
}

INCLUDE_ASM(const s32, "interface/frFont", frFontAppendIndexedTextToGlyphChain);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_00180540);

INCLUDE_ASM(const s32, "interface/frFont", frFontCheckPendingGlyphState);

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo) {
    pMsgInfo->Y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->FrqFlag = 1;
    pMsgInfo->RetFlag = 1;
}

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    const u8 *byte = stream->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *stream) {
    u32 first;
    u32 second;

    first = (stream->encodedText[stream->encodedTextOffset++] + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    second = stream->encodedText[stream->encodedTextOffset++];
    if (second == ITF_BYTE_MASK) {
        second = 0;
    } else {
        second = (second + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    }
    return (second << 8) | first;
}

INCLUDE_ASM(const s32, "interface/frFont", func_r1_001806C8);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_00180E20);

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, 0, 0, 0, 0x80, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    frMsgInfo_t args;

    args.X = X;
    args.Y = Y;
    args.Z = Z << 4;
    args.Style = Style;
    args.ColNo = ColNo;
    args.Tp = Tp;
    args.Spd = Spd;
    args.pMsg = pMsg;
    args.pFRQ = pFRQ;
    args.Ofs = 0;
    args.FrqFlag = 1;
    args.RetFlag = 1;
    return frAnalyzeMessage(&args);
}

#ifdef NON_MATCHING
/* 7 of 15 words differ (first at +0x20) */
frFontHandle_t *func_r1_001810E8(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", func_r1_001810E8);
#endif

INCLUDE_ASM(const s32, "interface/frFont", func_r1_00181128);

INCLUDE_ASM(const s32, "interface/frFont", func_r1_00181178);

u32 frGetMessageResult(u32 Flag) {
    return gMessageRequestResult & Flag;
}

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 VarNo, s32 pStr) {
    gVarStr[VarNo] = pStr;
}

/* Return the unit identifier saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsUnitId(void) {
    return itfTextLipsUnitId;
}

/* Return the motion number saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsMotionId(void) {
    return itfTextLipsMotionId;
}

INCLUDE_ASM(const s32, "interface/frFont", frDumpFontTexture);
u32 gMessageRequestResult = 0xFFFFFFFF;

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1A8);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1AC);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1B0);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1B4);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1B8);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1BC);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1C0);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1C4);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF480);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF484);

INCLUDE_SDATA(const s32, "interface/frFont", D_r1_0046C1D0);

