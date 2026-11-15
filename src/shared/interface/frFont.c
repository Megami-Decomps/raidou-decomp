#include "common.h"
#include "font.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfGsMemory.h"
#include "sdk_libkernl.h"
#include "frFont.h"
#include "itfMisc.h"

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

/* SMT3's has a ninth word (data); Raidou's frFontSystem_t entries are 0x20 apart. */
struct frFontData_t {
    sdfMemHandle_t *allocation;
    frFontHeader_t *resourceHeader;
    s32 metricByteCount;
    s32 valueByteCount;
    s8 *prop;
    void *ysht;
    GsFont_t **trans;
    struct frFontResource_t *resource;
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
extern volatile s32 dds3GsSema;
extern void sceGsExecLoadImage(void *, saddr_t);
extern s32 sceGsSyncPath(s32, s32);
void frLoadStyleAll(void);
void frSetFontClut(void);

#define FR_FONT_INDEXED_TEXT_SLOT_COUNT 32

/* Address words substituted by the indexed encoded-text control codes. */
extern u32 gVarStr[32];
void frClearTexBuf(void);
void frDestroyFontTable(dds3ProcessID_t *id);
void frDelStyleAll(void);
extern frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);
extern u32 frFreeFontTable(frFontNodeHeader_t *pool);
s32 fmDestroyFontMemControl(void);

void frAddFontStyle(
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
);

#define FR_FONT_TABLE_ENTRY_SHIFT 6

#define FR_FONT_WORD_BYTES 4

void frDelFontStyle(s32 StyleNo);

#define FR_FONT_FIRST_RELEASABLE_SLOT 2

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
frFontHandle_t *frSetUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);
frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);
frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);
GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode);
FontMem_t *fmRentFontMem(void);

#define FR_FONT_ATLAS_POSITION_SHIFT 4

#define FR_FONT_PIXEL_FORMAT 0x14

#define FR_FONT_ATLAS_DEST_SHIFT 6

extern void sdfImageSetReq(sdfImageRequest_t *parm);
u8 *frDecodeFontImage(u8 Style, s32 FontNo);

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

s32 frFlipPacketBuf(dds3ProcessID_t *id);
void frSetExtDic(u8 *pExtFontData);

#define FR_FONT_TEMPORARY_SLOT 8

void frDestroyExtDic(void);
s32 frStrLen(s8 *pStr);
u32 frGetStrWidth2(frFontHandle_t *pFRQ);

#define FR_FONT_DEFAULT_CELL_SLOT_COUNT 2

void frSetMessageDefStepY(void);
extern u32 gMessageStepY;

#define FR_FONT_DEFAULT_CURSOR_SPACING 0x14

void frSetMessageStepY(u32 StepY);
u8 *MessageNumSearch(s32 Index, frTextBank_t *pData, s32 Size);
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
extern frFontHandle_t *frSetFontString(u8 *text, s8 fontIndex, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 flag);
frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious);

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

void frCreateDrawFontPacket_20550848(s32 x, s32 y, s32 width, s32 height, u8 style,
    u32 color, u32 depth, s32 filtered, const frFontUvRect_t *uv,
    const frFontAtlas_t *atlas, s32 surface);

void frLoadFontData(s32 StyleNo, const char *pFileName);
frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);
frFontHandle_t *frSetFontString(u8 *text, s8 style, s8 firstOption, s8 secondOption, frFontHandle_t *chain, s32 relative);
frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2, s32 Mode);

#ifdef VERSION_RAIDOU1
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
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "newdata.h"

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
#else
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
#include "libsdf.h"
#include "sdf_movie_types.h"
#include "effect.h"
#include "newdata.h"

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
#endif

#ifdef VERSION_RAIDOU1
extern void frFontCheckPendingGlyphState(FrFontCtx *);
#else
extern u32 itfTextLipsUnitId;
#endif
extern u32 itfTextLipsMotionId;
#ifdef VERSION_RAIDOU1
extern u32 itfTextLipsUnitId;
u32 itfReadEncodedTextLead(FrFontCtx *args);
u32 itfReadEncodedCode(FrFontCtx *args);
#else

u32 itfReadEncodedTextLead(FrFontCtx *stream);
u32 itfReadEncodedCode(FrFontCtx *stream);
#endif
u32 itfGetTextLipsUnitId(void);
u32 itfGetTextLipsMotionId(void);
extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);

struct EffSpriteUV;

struct sdfTexHandle_t;

union EffSpriteColor;

#ifdef VERSION_RAIDOU1

/* Output point written by glyph-chain position lookup. */
typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

typedef char FrFontPosition_size_must_be_0x08[
    (sizeof(FrFontPosition) == 0x08) ? 1 : -1];

void frFontCreateContext();

#define FR_FONT_CONTEXT_ENABLE_VALUE 0x80

#define FR_FONT_FADE_COMPLETE 0x80

#define FR_FONT_FADE_ADVANCE_SCALE 22

#define FR_FONT_JITTER_OPTION 1

#define FR_FONT_JITTER_PERIOD 5

#define FR_FONT_JITTER_CENTER 2

#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C

#define FR_FONT_POSITION_SHIFT 4
#else

/* Output point written by glyph-chain position lookup. */
typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

typedef char FrFontPosition_size_must_be_0x08[
    (sizeof(FrFontPosition) == 0x08) ? 1 : -1];

void frFontCreateContext();

#define FR_FONT_CONTEXT_ENABLE_VALUE 0x80

#define FR_FONT_FADE_COMPLETE 0x80

#define FR_FONT_FADE_ADVANCE_SCALE 22

#define FR_FONT_JITTER_OPTION 1

#define FR_FONT_JITTER_PERIOD 5

#define FR_FONT_JITTER_CENTER 2

#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C

#define FR_FONT_POSITION_SHIFT 4
#endif

#ifdef VERSION_RAIDOU1
#define FR_FONT_POSITION_BAND_HEIGHT 0x54
#else
#define FR_FONT_POSITION_BAND_HEIGHT 0x64

#endif

/* Append the shared-text child context, apply its cached encoded byte and clear
 * the pending-create flag. Preserve the old-style implicit-argument interface. */

void frRefFT(s32 StyleNo, s32 Width) {
    frFontData_t *entry = &gFT.entries[StyleNo & FR_FONT_BYTE_MASK];

    entry->prop[0] = FR_FONT_ENTRY_ENABLED;
    entry->prop[1] = Width + 1;
}

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

INCLUDE_ASM(const s32, "interface/frFont", frCallDevLoadFileEx);

INCLUDE_ASM(const s32, "interface/frFont", frInitFontTable);

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

INCLUDE_ASM(const s32, "interface/frFont", frCallMemLock);

INCLUDE_ASM(const s32, "interface/frFont", frCallMemFreeDirect);

/* Ensure the four named default font files are installed, in slot order. */
void frLoadStyleAll(void) {
    frLoadFontData(0, "/font/font0.fnt");
    frLoadFontData(1, "/font/font1.fnt");
    frLoadFontData(2, "/font/font2.fnt");
    frLoadFontData(3, "/font/font3.fnt");
}

void frDelStyleAll(void) {
    s32 fontIndex;

    for (fontIndex = 0; fontIndex < FR_FONT_ENTRY_CAPACITY; fontIndex++) {
        if (gFT.entries[fontIndex].resource != NULL) {
            frDelFontStyle((u8)fontIndex);
        }
    }
}

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

void frFlushGsFontTable(frTextHandle_t *pText) {
    GsFont_t *cachedItem = pText->cachedItem;

    if (cachedItem != NULL) {
        if (cachedItem->used_count == 0) {
            gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex].trans[cachedItem->font_no] = NULL;
            fmRetFontMem(cachedItem->font_mem);
            gFT.cachedItemCount--;
        }
    }
}

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ) {
    if (frEraseEffect(pFRQ) != 0) {
        return pFRQ;
    }
    return frDestroyFontRequestNow(pFRQ);
}

INCLUDE_ASM(const s32, "interface/frFont", frCallFlushGsFontTable);

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

INCLUDE_ASM(const s32, "interface/frFont", frSetFontSub);

INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth);

INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth2);

INCLUDE_ASM(const s32, "interface/frFont", frSetFontSub2);

INCLUDE_ASM(const s32, "interface/frFont", frSetUserImage);

#ifdef NON_MATCHING
/* 15 of 20 words differ (first at +0x4) */
frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ) {
    frFontHandle_t *referenceChain = frSetUserImage(pFont, 0);

    if (referenceChain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, referenceChain);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frLinkUserImage);
#endif

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

u8 *frDecodeFontImage(u8 Style, s32 FontNo) {
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

GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo) {
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

INCLUDE_ASM(const s32, "interface/frFont", frCallMemset);

INCLUDE_ASM(const s32, "interface/frFont", frCallMemset2);

INCLUDE_ASM(const s32, "interface/frFont", frSetFont);

#ifdef NON_MATCHING
/* 13 of 26 words differ (first at +0x24) */
frFontHandle_t *frCallSetFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ) {
    frFontHandle_t *chain = frSetFontString(pStr, Style, ColNo, Tp, NULL, 0);

    if (chain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, chain);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frCallSetFontString);
#endif

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "interface/frFont", frSetFontStringSub);

#endif
INCLUDE_ASM(const s32, "interface/frFont", frSetFontString);

INCLUDE_ASM(const s32, "interface/frFont", frSetDrawSpeed);

#ifdef NON_MATCHING
#ifdef VERSION_RAIDOU1
/* 1 of 8 words differ (first at +0x10) */
#else
/* 2 of 8 words differ (first at +0xC) */
#endif
void SwitchOK(_FRQ *glyph) {
    glyph->contextModeEnabled = 1;
    frSetDrawSpeed(glyph, FR_FONT_CONTEXT_ENABLE_VALUE);
}
#else
INCLUDE_ASM(const s32, "interface/frFont", SwitchOK);
#endif

INCLUDE_ASM(const s32, "interface/frFont", frLeaf);

void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space) {
    pFRQ->spacing = Space;
    pFRQ->advance = frGetStrWidth(pFRQ);
}

INCLUDE_ASM(const s32, "interface/frFont", frCallSetFontSpace);

INCLUDE_ASM(const s32, "interface/frFont", frCallSetFontSpace2);

INCLUDE_ASM(const s32, "interface/frFont", frCallGetStrWidth);

INCLUDE_ASM(const s32, "interface/frFont", frCallGetStrWidth2);

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

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "interface/frFont", frDrawMesFlowSub);

#endif
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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "interface/frFont", frLeaf2);
#else
/* Replace the complete shared render-flag word used by subsequent drawing. */
void frSetFontDrawOT(u32 OtType) {
    gFontDrawOT = OtType;
}

INCLUDE_ASM(const s32, "interface/frFont", frBtlEnemyDataSub);
#endif

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

INCLUDE_ASM(const s32, "interface/frFont", frCallCheckEnableMovieCancel);

INCLUDE_ASM(const s32, "interface/frFont", frDrawFont);

INCLUDE_ASM(const s32, "interface/frFont", frDrawMesFlowSub2);

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "interface/frFont", frDrawFontEx2);
#else
INCLUDE_ASM(const s32, "interface/frFont", ButtonLineHelpAddTextSet);
#endif

s32 frFlipPacketBuf(dds3ProcessID_t *id) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    gFT.glyphSlots[queueIndex] = frDestroyFontRequestNow(gFT.glyphSlots[queueIndex]);
    return 0;
}

frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2) {
    return frCombineFontRequestEx(pFRQ1, pFRQ2, 1);
}

INCLUDE_ASM(const s32, "interface/frFont", frDestroyFontRequestAfterSub);

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

#ifdef VERSION_RAIDOU1
INCLUDE_ASM(const s32, "interface/frFont", frGetStrWidth);
#else
u32 frGetStrWidth(frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph = pFRQ->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        s8 childSpacing = pFRQ->spacing;

        do {
            totalAdvance += childGlyph->advance;
            childGlyph = childGlyph->next;
            totalAdvance += childSpacing;
        } while (childGlyph != NULL);
    }
    return totalAdvance;
}
#endif

u32 frGetStrWidth2(frFontHandle_t *pFRQ) {
    frFontHandle_t *parentGlyph;
    frTextHandle_t *childGlyph;
    s32 totalAdvance = 0;

    for (parentGlyph = pFRQ->chainHead; parentGlyph != NULL; parentGlyph = parentGlyph->next) {
        childGlyph = parentGlyph->firstChild;
        if (childGlyph != NULL) {
            s8 childSpacing = parentGlyph->spacing;

            do {
                totalAdvance += childGlyph->advance;
                childGlyph = childGlyph->next;
                totalAdvance += childSpacing;
            } while (childGlyph != NULL);
        }
    }
    return totalAdvance;
}

INCLUDE_ASM(const s32, "interface/frFont", frPreCalcStrWidth);

INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth3);

u16 frGetFontWidth(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellWidth;
}

u16 frGetFontHeight(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellHeight;
}

#ifdef NON_MATCHING
/* 1 of 3 words differ (first at +0x0) */
void frSetMessageDefStepY(void) {
    gMessageStepY = FR_FONT_DEFAULT_CURSOR_SPACING;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetMessageDefStepY);
#endif

void frSetMessageStepY(u32 StepY) {
    gMessageStepY = StepY;
}

#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
/* 1 of 29 words differ (first at +0x24) */
s32 frCallEventVoiceSub(const _FRQ *glyph) {
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
#else
INCLUDE_ASM(const s32, "interface/frFont", frCallEventVoiceSub);
#endif
#else
s32 frCallEventVoiceSub(const _FRQ *glyph) {
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
#endif

#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
/* 1 of 51 words differ (first at +0x54) */
s32 frCallGetStrWidth3(s32 requestedLine, const _FRQ *glyph) {
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
#else
INCLUDE_ASM(const s32, "interface/frFont", frCallGetStrWidth3);
#endif
#else
s32 frCallGetStrWidth3(s32 requestedLine, const _FRQ *glyph) {
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
#endif

#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
/* 1 of 35 words differ (first at +0x30) */
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
                currentNode = currentNode->next;
            } while (currentNode != NULL);
        }
    }
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frFontGetGlyphPositionAtIndex);
#endif
#else
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
#endif

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

INCLUDE_ASM(const s32, "interface/frFont", frFontCreateContext);

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frFontCheckPendingGlyphState(FrFontCtx *ctx) {
    s8 pending;

    if (ctx->glyphChain == NULL) {
        pending = ctx->pendingCreate;
    } else {
        if (ctx->glyphChain->firstChild == NULL) {
            ctx->pendingCreate = 0;
        }
        pending = ctx->pendingCreate;
    }
    if (pending == 0) {
        pending = ctx->pendingPosition;
    } else {
        frFontCreateContext();
        pending = ctx->pendingPosition;
    }
    if (pending != 0) {
        frSetDrawPosition(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frMsgInfo_t *pMsgInfo) {
    pMsgInfo->Y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->FrqFlag = 1;
    pMsgInfo->RetFlag = 1;
}

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
#ifdef VERSION_RAIDOU1
u32 itfReadEncodedTextLead(FrFontCtx *args) {
    s32 *position = &args->encodedTextOffset;
    const u8 *byte = args->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
#else
u32 itfReadEncodedTextLead(FrFontCtx *stream) {
    s32 *position = &stream->encodedTextOffset;
    const u8 *byte = stream->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
#endif
#ifdef VERSION_RAIDOU1
u32 itfReadEncodedCode(FrFontCtx *args) {
    const u8 *bytes = args->encodedText;
    u32 first;
    u32 second;

    first = (bytes[args->encodedTextOffset++] + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    second = bytes[args->encodedTextOffset++];
    if (second == ITF_BYTE_MASK) {
        second = 0;
    } else {
        second = (second + ITF_BYTE_MASK) & ITF_BYTE_MASK;
    }
    return (second << 8) | first;
}

INCLUDE_ASM(const s32, "interface/frFont", execTag_0);

INCLUDE_ASM(const s32, "interface/frFont", frCallEnableMode);

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
frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", MessageRequestNS);
#endif

INCLUDE_ASM(const s32, "interface/frFont", frCallMessageRequestEx2);

INCLUDE_ASM(const s32, "interface/frFont", frCallMessageRequestEx22);

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

INCLUDE_SDATA(const s32, "interface/frFont", gMessageSpaceX);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageStepY);

INCLUDE_SDATA(const s32, "interface/frFont", frData2);

INCLUDE_SDATA(const s32, "interface/frFont", frData3);

INCLUDE_SDATA(const s32, "interface/frFont", execData);

INCLUDE_SDATA(const s32, "interface/frFont", execData2);

INCLUDE_SDATA(const s32, "interface/frFont", gModeFlag);

INCLUDE_SDATA(const s32, "interface/frFont", gFontDrawOT);
#else
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

INCLUDE_ASM(const s32, "interface/frFont", execTag_0);

INCLUDE_ASM(const s32, "interface/frFont", frCallEnableMode);

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
frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", MessageRequestNS);
#endif

INCLUDE_ASM(const s32, "interface/frFont", frCallMessageRequestEx2);

INCLUDE_ASM(const s32, "interface/frFont", frCallMessageRequestEx22);

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

INCLUDE_SDATA(const s32, "interface/frFont", gMessageSpaceX);

INCLUDE_SDATA(const s32, "interface/frFont", gMessageStepY);

INCLUDE_SDATA(const s32, "interface/frFont", frData2);

INCLUDE_SDATA(const s32, "interface/frFont", frData3);

INCLUDE_SDATA(const s32, "interface/frFont", execData);

INCLUDE_SDATA(const s32, "interface/frFont", execData2);

INCLUDE_SDATA(const s32, "interface/frFont", gModeFlag);

INCLUDE_SDATA(const s32, "interface/frFont", gFontDrawOT);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "interface/frFont", frSetFontStringData);
#else
INCLUDE_SDATA(const s32, "interface/frFont", ButtonLineHelpAddTextSetData);

INCLUDE_SDATA(const s32, "interface/frFont", frSetFontStringData2);
#endif

INCLUDE_SDATA(const s32, "interface/frFont", frFontAppendIndexedTextToGlyphChainData);

