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

INCLUDE_ASM(const s32, "interface/frFont", func_001923A8);

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

INCLUDE_ASM(const s32, "interface/frFont", func_001924D8);

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

INCLUDE_ASM(const s32, "interface/frFont", frClearTexBuf);

INCLUDE_ASM(const s32, "interface/frFont", func_001928F8);

INCLUDE_ASM(const s32, "interface/frFont", func_00192938);

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontTable);

INCLUDE_ASM(const s32, "interface/frFont", func_00192BB8);

INCLUDE_ASM(const s32, "interface/frFont", func_00192CA0);

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void) {
    frLoadFontData(0, "/font/font0.fnt");
    frLoadFontData(1, "/font/font1.fnt");
    frLoadFontData(2, "/font/font2.fnt");
    frLoadFontData(3, "/font/font3.fnt");
}

INCLUDE_ASM(const s32, "interface/frFont", frDelStyleAll);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontClut);

INCLUDE_ASM(const s32, "interface/frFont", func_00192E60);

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ) {
    if (frEraseEffect(pFRQ) != 0) {
        return pFRQ;
    }
    return frDestroyFontRequestNow(pFRQ);
}

INCLUDE_ASM(const s32, "interface/frFont", func_00192F10);

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

INCLUDE_ASM(const s32, "interface/frFont", func_001930A8);

INCLUDE_ASM(const s32, "interface/frFont", func_00193130);

INCLUDE_ASM(const s32, "interface/frFont", func_001931E0);

INCLUDE_ASM(const s32, "interface/frFont", func_001933E0);

INCLUDE_ASM(const s32, "interface/frFont", func_00193400);

INCLUDE_ASM(const s32, "interface/frFont", func_00193540);

INCLUDE_ASM(const s32, "interface/frFont", frTranslateUserData);

INCLUDE_ASM(const s32, "interface/frFont", func_00193670);

INCLUDE_ASM(const s32, "interface/frFont", func_001936E8);

INCLUDE_ASM(const s32, "interface/frFont", func_001937D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00193868);

INCLUDE_ASM(const s32, "interface/frFont", func_001938A8);

INCLUDE_ASM(const s32, "interface/frFont", func_001939F0);

INCLUDE_ASM(const s32, "interface/frFont", func_00193A58);

INCLUDE_ASM(const s32, "interface/frFont", func_00193B60);

INCLUDE_ASM(const s32, "interface/frFont", func_00193D38);

INCLUDE_ASM(const s32, "interface/frFont", func_00193D70);

INCLUDE_ASM(const s32, "interface/frFont", func_00193D90);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontSpace);

INCLUDE_ASM(const s32, "interface/frFont", func_00193DF8);

INCLUDE_ASM(const s32, "interface/frFont", func_00193E40);

INCLUDE_ASM(const s32, "interface/frFont", func_00193E88);

INCLUDE_ASM(const s32, "interface/frFont", func_00193F68);

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

INCLUDE_ASM(const s32, "interface/frFont", func_001941E8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194580);

INCLUDE_ASM(const s32, "interface/frFont", func_00194598);

INCLUDE_ASM(const s32, "interface/frFont", func_001945B8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194A00);

INCLUDE_ASM(const s32, "interface/frFont", func_00194A50);

INCLUDE_ASM(const s32, "interface/frFont", func_00194A68);

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

INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth);

INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth2);

INCLUDE_ASM(const s32, "interface/frFont", func_00194BC8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194CB8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194DB8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194DD8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194DF8);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E08);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E10);

INCLUDE_ASM(const s32, "interface/frFont", func_00194E88);

INCLUDE_ASM(const s32, "interface/frFont", func_00194F58);

INCLUDE_ASM(const s32, "interface/frFont", func_00194FE8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195038);

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

INCLUDE_ASM(const s32, "interface/frFont", func_001951D0);

INCLUDE_ASM(const s32, "interface/frFont", func_00195340);

INCLUDE_ASM(const s32, "interface/frFont", func_001953B0);

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo) {
    pMsgInfo->Y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->FrqFlag = 1;
    pMsgInfo->RetFlag = 1;
}

INCLUDE_ASM(const s32, "interface/frFont", func_00195448);

INCLUDE_ASM(const s32, "interface/frFont", func_00195470);

INCLUDE_ASM(const s32, "interface/frFont", func_001954C8);

INCLUDE_ASM(const s32, "interface/frFont", func_00195DC8);

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

INCLUDE_ASM(const s32, "interface/frFont", func_001960D8);

INCLUDE_ASM(const s32, "interface/frFont", func_00196118);

INCLUDE_ASM(const s32, "interface/frFont", func_00196168);

u32 frGetMessageResult(u32 Flag) {
    return gMessageRequestResult & Flag;
}

/* Store an unchecked replacement-text address word in the font's indexed table. */
void frSetVariableRequest(s32 VarNo, s32 pStr) {
    gVarStr[VarNo] = pStr;
}

INCLUDE_ASM(const s32, "interface/frFont", func_001961E8);

INCLUDE_ASM(const s32, "interface/frFont", func_001961F0);

INCLUDE_ASM(const s32, "interface/frFont", frDumpFontTexture);
INCLUDE_SDATA(const s32, "interface/frFont", D_004DF45C);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF460);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF464);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF468);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF46C);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF470);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF474);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF478);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF47C);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF480);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF484);

INCLUDE_SDATA(const s32, "interface/frFont", D_004DF488);

