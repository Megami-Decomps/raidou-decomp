#include "common.h"
#include "sdfImageSet.h"
#include "font.h"
#include "kernel.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfDraw.h"
#include "sdfPacketBuff.h"
#include "sdfGsMemory.h"
#include "sdk_libgraph.h"
#include "sdk_libkernl.h"
#include "frFont.h"
#include "itfMisc.h"
#include "cmbFont.h"
#include "code_r1_001CF808.h"
#include "evtCommand.h"
#include "itfMesManager.h"
#include "sdfSound.h"
#include "sBtlSM.h"

typedef struct frFontHeader_t frFontHeader_t;


typedef struct frFontData_t frFontData_t;


typedef struct frFontAtlas_t frFontAtlas_t;

typedef struct frFontSystem_t frFontSystem_t;

typedef union frFontDmaTag_t frFontDmaTag_t;

typedef struct frFontGsWrite_t frFontGsWrite_t;

typedef struct frFontGsPacket_t frFontGsPacket_t;

typedef struct frTextIndex_t frTextIndex_t;




/* Serialized resource header. */
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

void frCopyFontClutToVram(s32 GS_LocalAddr, u32 *pClut);
extern volatile s32 dds3GsSema;
void frLoadStyleAll(void);
void frSetFontClut(void);

#define FR_FONT_INDEXED_TEXT_SLOT_COUNT 32

/* Address words substituted by the indexed encoded-text control codes. */
extern u32 gVarStr[32];
extern s8 gFontBaseWidth[];
extern u8 frSetFontString_style;
extern s8 frData2;
extern s32 frData3;
extern s32 frData[];
extern s8 frSetFontSubData[];
extern s32 gRegistMax;
extern u8 frFontAppendIndexedTextToGlyphChainData[];
void frClearTexBuf(void);
void frDestroyFontTable(dds3ProcessID_t *id);
void frDelStyleAll(void);
extern frFontHandle_t *frDestroyFontRequestNow(frFontHandle_t *pFRQ);
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
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ);
s32 frEraseEffect(frFontHandle_t *pFRQ);
s32 frDestroyFontRequestAfter(frFontHandle_t *pFRQ);
extern u32 dds3GetFrameID(void);
extern frFontHandle_t *frCombineFontRequestEx(frFontHandle_t *, frFontHandle_t *, s32);
s32 frGetGsFontNum(void);
s32 frGetTextHandleNum(void);
u32 frGetFontHandleNum(void);
frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ);
frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2);
FontMem_t *fmRentFontMem(void);

#define FR_FONT_ATLAS_POSITION_SHIFT 4

#define FR_FONT_PIXEL_FORMAT 0x14

#define FR_FONT_ATLAS_DEST_SHIFT 6


#define FR_FONT_RESOURCE_CLONE_BYTES 0x120


GsFont_t *frTranslateFont2GS(frTextHandle_t *pText, s32 FontNo);
u16 frGetFontWidth(s32 slotIndex);
u16 frGetFontHeight(s32 slotIndex);
extern u8 gModeFlag;
void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed);

#define FR_FONT_CONTEXT_BYTE_THRESHOLD 0x81

#define FR_FONT_CONTEXT_BYTE_CAP (-0x80)

void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space);
u32 frGetStrWidth(const frFontHandle_t *pFRQ);
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

#define FR_FONT_DEFAULT_CURSOR_SPACING 0x19

void frSetMessageStepY(u32 StepY);

#define FR_FONT_CONTEXT_CURSOR_SCALE 8

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ);
frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);
frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ);
frFontHandle_t *frAnalyzeMessage(frFontCtx_t *pMsgInfo);
u32 frGetMessageResult(u32 Flag);
extern u32 gMessageRequestResult;
void frSetVariableRequest(s32 VarNo, s32 pStr);
extern s8 gMessageSpaceX;
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
extern void frFontCheckPendingGlyphState(frFontCtx_t *);
#else
extern u32 itfTextLipsUnitId;
#endif
extern u32 itfTextLipsMotionId;
#ifdef VERSION_RAIDOU1
extern u32 itfTextLipsUnitId;
u32 itfReadEncodedTextLead(frFontCtx_t *args);
u32 itfReadEncodedCode(frFontCtx_t *args);
#else

u32 itfReadEncodedTextLead(frFontCtx_t *stream);
u32 itfReadEncodedCode(frFontCtx_t *stream);
#endif
u32 itfGetTextLipsUnitId(void);
u32 itfGetTextLipsMotionId(void);

struct effSpriteUV_t;

struct sdfTexHandle_t;

union EffSpriteColor;

#ifdef VERSION_RAIDOU1


#define FR_FONT_SWITCH_OK_SPEED 128

#define FR_FONT_FADE_COMPLETE 0x80

#define FR_FONT_FADE_ADVANCE_SCALE 22

#define FR_FONT_JITTER_OPTION 1

#define FR_FONT_JITTER_PERIOD 5

#define FR_FONT_JITTER_CENTER 2

#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C

#define FR_FONT_POSITION_SHIFT 4
#else


#define FR_FONT_SWITCH_OK_SPEED 14

#define FR_FONT_FADE_COMPLETE 0x80

#define FR_FONT_FADE_ADVANCE_SCALE 22

#define FR_FONT_JITTER_OPTION 1

#define FR_FONT_JITTER_PERIOD 5

#define FR_FONT_JITTER_CENTER 2

#define FR_FONT_FADE_PREDECESSOR_THRESHOLD 0x1C

#define FR_FONT_POSITION_SHIFT 4
#endif

#define FR_FONT_POSITION_BAND_HEIGHT 0x64

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
void frCopyFontClutToVram(s32 GS_LocalAddr, u32 *pClut) {
    sceGsLoadImage loadImage;

    sceGsSetDefLoadImage(&loadImage, (s16)GS_LocalAddr, 1, 0, 0, 0, 8, 2);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(&loadImage, pClut);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
}

void frClearTexBuf(void) {
    sceGsLoadImage loadImage;
    sdfMemHandle_t *allocation;
    u8 *pixels;
    s32 byteCount = gFT.atlas.height * gFT.atlas.width;

    byteCount = (u32)byteCount >> 1;
    allocation = sdfMemAllocDirect(byteCount);
    pixels = EE_ADDR_AS(u8 *, sdfMemLock(allocation));
    memset(pixels, 0x11, byteCount);
    sceGsSetDefLoadImage(&loadImage, gFT.atlas.bufferBase, gFT.atlas.bufferWidth,
                         FR_FONT_PIXEL_FORMAT, 0, 0, gFT.atlas.width, gFT.atlas.height);
    WaitSema(dds3GsSema);
    FlushCache(0);
    sceGsExecLoadImage(&loadImage, pixels);
    sceGsSyncPath(0, 0);
    SignalSema(dds3GsSema);
    sdfMemFreeDirect(allocation);
}

void frCallDevLoadFileEx(u8 StyleNo, const char *FileName) {
    frCallMemLock(StyleNo, NULL, sdfDevLoadFileEx(FileName, NULL, NULL));
}

void frInitFontTable(s32 GsFontNums, s32 TextNums) {
    s32 i;
    s32 rowCount;
    s32 glyphsPerRow;
    s32 rowTotal;
    s32 glyphRingCount;
    sdfGsMemSlice_t *atlasHead;

    gFT.cachedItemCount = 0;
    gFT.itemCount = 0;
    gFT.glyphCount = 0;
    for (i = 0; i < 9; i++) {
        gFT.entries[i].resource = NULL;
    }
    glyphsPerRow = 21;
    rowCount = GsFontNums / glyphsPerRow;
    rowTotal = rowCount;
    glyphRingCount = TextNums;
    TextNums = glyphRingCount * 4;
    gFT.atlas.width = 0x200;
    gFT.atlas.widthExponent = frConvertVal2N(0x200);
    gFT.atlas.height = 1 << (frConvertVal2N(rowTotal * 0x18) + 1);
    gFT.atlas.heightExponent = frConvertVal2N(gFT.atlas.height);
    gFT.textureHead1 = sdfGsMemAllocFromTail(0x180, 3);
    for (i = 0; i < 6; i++) {
        gFT.imageBuffers[i] = (gFT.textureHead1->addr >> 6) + i;
    }
    atlasHead = sdfGsMemAllocFromTail((u32)(gFT.atlas.width * gFT.atlas.height) >> 3, 2);
    gFT.textureHead0 = atlasHead;
    gFT.atlas.bufferBase = atlasHead->addr >> 6;
    gFT.atlas.bufferWidth = (u32)gFT.atlas.width >> 6;
    for (i = 0; i < 2; i++) {
        gFT.glyphSlots[i] = NULL;
    }
    fmInitFontMemControl(gFT.atlas.bufferBase, glyphsPerRow, rowCount, 0x18, 0x18, 0xC);
    gFT.itemPool = frAllocFontTable(0x2C, TextNums);
    gFT.glyphPool = frAllocFontTable(sizeof(frFontHandle_t), glyphRingCount);
    frLoadExchangeTable();
    frLoadStyleAll();
    frSetFontClut();
    for (i = 0; i < FR_FONT_INDEXED_TEXT_SLOT_COUNT; i++) {
        gVarStr[i] = 0;
    }
    frClearTexBuf();
    gRegistMax = glyphsPerRow * rowTotal;
}

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

/* body: matched in smt3 @ 9401f79e: src/smt3/interface/frFont.c:762, rework, same function 0.90 (words 57/66, calls 1.00, shape 0.83) */
void frCallMemLock(
    u8 StyleNo, u8 *pFontData, sdfMemHandle_t *DataMh
) {
    frFontData_t *entry;
    s32 sectionOffset;

    if (pFontData == NULL) {
        if (DataMh != NULL) {
            pFontData = EE_ADDR_AS(u8 *, sdfMemLock(DataMh));
        }
    }
#ifdef FIX_BUGS
    /* BUG: no data and no handle (or a failed lock) dereferences NULL below */
    if (pFontData == NULL) {
        return;
    }
#endif
    entry = &gFT.entries[StyleNo];
    entry->allocation = DataMh;
    entry->resourceHeader = (frFontHeader_t *)pFontData;
    sectionOffset = entry->resourceHeader->tableOffset + (entry->resourceHeader->tableCount << FR_FONT_TABLE_ENTRY_SHIFT);
    if (entry->resourceHeader->hasExtra != 0) {
        s32 *flagSection = (s32 *)(pFontData + sectionOffset);
        s32 flagDataBytes = *flagSection;
        s32 *valueSection;
        s32 valueDataBytes;
        s32 flagSectionBytes;
        s32 valueSectionBytes;

        entry->prop = (s8 *)(flagSection + 1);
        entry->metricByteCount = flagDataBytes;
        flagSectionBytes = flagDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += flagSectionBytes;
        valueSection = (s32 *)(pFontData + sectionOffset);
        valueDataBytes = *valueSection;
        entry->ysht = valueSection + 1;
        entry->valueByteCount = valueDataBytes;
        valueSectionBytes = valueDataBytes + FR_FONT_WORD_BYTES;
        sectionOffset += valueSectionBytes;
    } else {
        entry->metricByteCount = 0;
        entry->prop = NULL;
        entry->valueByteCount = 0;
        entry->ysht = NULL;
    }
    entry->trans = (GsFont_t **)(pFontData + sectionOffset);
    sectionOffset += entry->resourceHeader->widthCount * FR_FONT_WORD_BYTES;
    entry->resource = (struct frFontResource_t *)(pFontData + sectionOffset);
}

void frCallMemFreeDirect(s32 StyleNo) {
    frFontData_t *entry = &gFT.entries[StyleNo & FR_FONT_BYTE_MASK];

    if (entry->allocation != NULL) {
        sdfMemFreeDirect(entry->allocation);
        entry->resource = NULL;
    }
    entry->trans = NULL;
}

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

        frCopyFontClutToVram(*imageBuffer++, imageWords);
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

s32 frCallFlushGsFontTable(frFontHandle_t *pFRQ) {
    if (pFRQ == NULL) {
        return 0;
    }
    do {
        frTextHandle_t *childGlyph = pFRQ->firstChild;
        frFontHandle_t *previousGlyph;

        while (childGlyph != NULL) {
            frTextHandle_t *nextGlyph = childGlyph->next;

            if (childGlyph->sourceItem == NULL) {
                childGlyph->cachedItem->used_count--;
                frFlushGsFontTable(childGlyph);
            } else if (childGlyph->sourceItem->font_no < 18) {
                cmbCallRetFontMem(childGlyph->sourceItem->font_no);
            } else {
                nbCallRetFontMem();
            }
            frFreeFontNode(childGlyph, gFT.itemPool);
            gFT.itemCount--;
            childGlyph = nextGlyph;
        }
        previousGlyph = pFRQ->previous;
        frFreeFontNode(pFRQ, gFT.glyphPool);
        gFT.glyphCount--;
        pFRQ = previousGlyph;
    } while (pFRQ != NULL);
    return 0;
}

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

s32 frSetFontSub(s8 Style, s32 FontNo) {
    s32 shift = 0;

    switch (FontNo) {
        case '1':
            shift = 1;
            break;
        case 'G':
        case 'P':
        case 'Q':
            shift = 3;
            break;
        case 'J':
        case 'Y':
            shift = 2;
            break;
    }
    if (Style == 1 && FontNo == 'Y') {
        shift = 3;
    }
    if (Style == 0 && (u32)(FontNo - 340) < 114 && frSetFontSubData[FontNo - 340] != 0) {
        shift = 3;
    }
    return shift * 8;
}

INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth);

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 72 of 152 words match.  */
#ifdef NON_MATCHING
void frCallGetFontWidth2(frTextHandle_t *pText, s32 FontNo, s32 Width, s32 Height) {
    frFontData_t *entry = &gFT.entries[pText->renderValueOrSetupOrShade.setupBytes.fontIndex];
    s32 fontWidth;
    s32 fontHeight;
    s32 advance;
    s32 offset;
    s8 *metric;
    u8 flags;

    fontWidth = frGetFontWidth(pText->renderValueOrSetupOrShade.setupBytes.fontIndex);
    pText->cellDimensions.cellWidth = fontWidth;
    fontHeight = frGetFontHeight(pText->renderValueOrSetupOrShade.setupBytes.fontIndex);
    pText->cellDimensions.cellHeight = fontHeight;
    pText->advance = gFontBaseWidth[pText->renderValueOrSetupOrShade.setupBytes.fontIndex];
    if (Width != 0 || Height != 0) {
        pText->advance = pText->advance * Width / fontWidth;
        pText->cellDimensions.cellHeight = pText->cellDimensions.cellHeight * Height / fontHeight;
        pText->cellDimensions.cellWidth = pText->cellDimensions.cellWidth * Width / fontWidth;
    }
    flags = pText->renderValueOrSetupOrShade.setupBytes.sharedFlags;
    if ((flags & 0x40) && pText->glyphCodeOrContext.glyphCode < 0x80) {
        pText->advance = pText->advance * 2 / 3;
        pText->cellDimensions.cellWidth = pText->cellDimensions.cellWidth * 2 / 3;
    }
    if (!(flags & 1) || !entry->resourceHeader->hasExtra || FontNo >= 454) {
        return;
    }
    if (FontNo == 0) {
        advance = (fontWidth >> 1) - 4;
        offset = 0;
    } else {
        FontNo *= 2;
        if (FontNo >= entry->metricByteCount) {
            return;
        }
        metric = &entry->prop[FontNo];
        if (metric[1] == 0) {
            return;
        }
        advance = metric[1] - metric[0];
        offset = metric[0] * 16;
        if (Width != 0 || Height != 0) {
            advance = advance * Width / fontWidth;
            offset = offset * Width / fontWidth;
        }
        if ((flags & 0x40) && pText->glyphCodeOrContext.glyphCode < 0x80) {
            offset = offset * 2 / 3;
            advance = advance * 2 / 3;
        }
        switch ((FontNo >> 1) + ' ') {
            case '!':
            case '1':
                offset -= 32;
                advance += 4;
                break;
        }
    }
    pText->advance = advance;
    pText->x = -offset;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth2);
#endif
#else
INCLUDE_ASM(const s32, "interface/frFont", frCallGetFontWidth2);
#endif

void frSetFontSub2(frTextHandle_t *pText, s32 FontNo) {
    frCallGetFontWidth2(pText, FontNo, 0, 0);
}

frFontHandle_t *frSetUserImage(GsFont_t *pFont, u8 Style, frFontHandle_t *pFRQ) {
    frTextHandle_t *glyph;
    frTextHandle_t *previous;

    if (pFRQ == NULL) {
        pFRQ = frAllocFontNode(gFT.glyphPool);
        gFT.glyphCount++;
        frInitFontHandle(pFRQ);
    }
    glyph = frAllocFontNode(gFT.itemPool);
    previous = pFRQ->lastChild;
    gFT.itemCount++;
    frInitTextHandle(glyph, 0, 0, Style, 0xA09DC300, 0);
    if (previous == NULL) {
        pFRQ->firstChild = glyph;
    } else {
        previous->next = glyph;
    }
    glyph->sourceItem = pFont;
    glyph->advance = gFontBaseWidth[0];
    glyph->cellDimensions.cellWidth = frGetFontWidth(0);
    glyph->cellDimensions.cellHeight = frGetFontHeight(0);
    glyph->previous = previous;
    pFRQ->lastChild = glyph;
    pFRQ->childCount++;
    pFRQ->advance += glyph->advance;
    pFRQ->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
    pFRQ->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    return pFRQ;
}

frFontHandle_t *frLinkUserImage(GsFont_t *pFont, frFontHandle_t *pFRQ) {
    frFontHandle_t *referenceChain = frSetUserImage(pFont, 0, NULL);

    if (referenceChain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, referenceChain);
}

GsFont_t *frTranslateUserData(s32 Width, s32 Height, void *pData, s8 DataFreeMode) {
    s32 imagePosition[2];
    sdfImageSetReqParam_t uploadRequest;
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
        uploadRequest.freeMode = SDF_IMAGESET_MEM_IGNORE;
    } else {
        uploadRequest.freeMode = SDF_IMAGESET_MEM_CHIP;
    }
    uploadRequest.data = pData;
    uploadRequest.pixelFormat = FR_FONT_PIXEL_FORMAT;
    uploadRequest.w = Width;
    uploadRequest.h = Height;
    uploadRequest.bufferWidth = gFT.atlas.width;
    uploadRequest.addr = gFT.atlas.bufferBase << FR_FONT_ATLAS_DEST_SHIFT;
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

void frInitTextHandle(frTextHandle_t *pText, u16 Code, u8 Style, u8 ColNo, u32 Color, u8 Tp) {
    memset(pText, 0, sizeof(frTextHandle_t));
    pText->glyphCodeOrContext.glyphCode = Code;
    pText->renderValueOrSetupOrShade.setupBytes.fontIndex = Style;
    pText->renderValueOrSetupOrShade.setupBytes.firstOption = ColNo;
    pText->renderValueOrSetupOrShade.setupBytes.secondOption = Tp;
    pText->parentDimensionsOrRenderWord.renderWord = Color & ~FR_FONT_BYTE_MASK;
    pText->renderValueOrSetupOrShade.setupBytes.sharedFlags = gModeFlag;
}

void frInitFontHandle(frFontHandle_t *pFRQ) {
    memset(pFRQ, 0, sizeof(frFontHandle_t));
    pFRQ->chainHead = pFRQ;
    pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    pFRQ->glyphCodeOrContext.byteRoles.baseContextByte = FR_FONT_CONTEXT_BYTE_CAP;
}

/* NON_MATCHING: 78 of 82 words match.  */
#ifdef NON_MATCHING
frTextHandle_t *frSetFont(u16 glyphCode, u8 fontIndex, u8 firstOption, u8 secondOption) {
    frTextHandle_t *glyph = frAllocFontNode(gFT.itemPool);
    s32 shiftIndex;

    gFT.itemCount++;
    frInitTextHandle(glyph, glyphCode, fontIndex, firstOption, 0xA09DC300, secondOption);
    if (glyph->glyphCodeOrContext.glyphCode < 0x80) {
        shiftIndex = glyph->glyphCodeOrContext.glyphCode - 0x20;
    } else {
        s32 relative = glyph->glyphCodeOrContext.glyphCode - 0x8080;

        shiftIndex = ((relative & FR_FONT_MIDDLE_BYTE_MASK) >> 1) + (relative & 0x7F);
    }
    if (shiftIndex >= gFT.entries[fontIndex].resourceHeader->widthCount) {
        shiftIndex = 0x147;
    }
    glyph->cachedItem = frTranslateFont2GS(glyph, shiftIndex);
    frSetFontSub2(glyph, shiftIndex);
    if ((glyph->renderValueOrSetupOrShade.setupBytes.sharedFlags & 0x10) != 0) {
        glyph->y = frSetFontSub((s8)fontIndex, shiftIndex);
    } else {
        glyph->y = 0;
    }
    return glyph;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetFont);
#endif

frFontHandle_t *frCallSetFontString(u8 *pStr, s8 Style, s8 ColNo, s8 Tp, frFontHandle_t *pFRQ) {
    frFontHandle_t *chain = frSetFontString(pStr, Style, ColNo, Tp, NULL);

    if (chain == NULL) {
        return pFRQ;
    }
    return frCombineFontRequest(pFRQ, chain);
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "interface/frFont", frSetFontStringSub);

#endif
#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 99 of 102 words match.  */
#ifdef NON_MATCHING
frFontHandle_t *frSetFontString(u8 *text, s8 style, s8 firstOption, s8 secondOption, frFontHandle_t *chain) {
    u32 i;
    u32 length = 0;
    frTextHandle_t *glyph;
    frTextHandle_t *previous;

    if (style >= 0) {
        frSetFontString_style = style;
    }
    if (gFT.entries[frSetFontString_style].resource == NULL) {
        return NULL;
    }
    if (text != NULL) {
        length = strlen((const char *)text);
    }
    if (chain == NULL) {
        chain = frAllocFontNode(gFT.glyphPool);
        gFT.glyphCount++;
        frInitFontHandle(chain);
    }
    previous = chain->lastChild;
    glyph = NULL;
    for (i = 0; i < length; i++) {
        u16 code = text[i];

        if (code >= 0x80) {
            i++;
            code = text[i] | (code << FR_FONT_BYTE_SHIFT);
        }
        glyph = frSetFont(code, frSetFontString_style, firstOption, secondOption);
        if (previous == NULL) {
            chain->firstChild = glyph;
        } else {
            previous->next = glyph;
        }
        chain->advance = frData2 + (glyph->advance + chain->advance);
        chain->childCount++;
        glyph->previous = previous;
        previous = glyph;
    }
    if (glyph != NULL) {
        chain->lastChild = glyph;
        chain->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = glyph->cellDimensions.cellWidth;
        chain->parentDimensionsOrRenderWord.parentDimensions.cellHeight = glyph->cellDimensions.cellHeight;
    }
    return chain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetFontString);
#endif
#else
INCLUDE_ASM(const s32, "interface/frFont", frSetFontString);
#endif

void frSetDrawSpeed(frFontHandle_t *pFRQ, s32 Speed) {
    s32 encodedValue = (Speed & FR_FONT_BYTE_MASK) * 2;

    if (encodedValue >= FR_FONT_CONTEXT_BYTE_THRESHOLD) {
        pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = FR_FONT_CONTEXT_BYTE_CAP;
    } else {
        pFRQ->glyphCodeOrContext.byteRoles.encodedContextByte = encodedValue;
        if (encodedValue < 0x80) {
            pFRQ->glyphCodeOrContext.byteRoles.baseContextByte = encodedValue;
        }
    }
}

void SwitchOK(_FRQ *glyph) {
    glyph->switchState = 1;
    frSetDrawSpeed(glyph, FR_FONT_SWITCH_OK_SPEED);
}

void frLeaf(_FRQ *glyph) {
    if (glyph != NULL) {
        do {
            glyph->switchState = 0;
            glyph->glyphCodeOrContext.byteRoles.encodedContextByte = glyph->glyphCodeOrContext.byteRoles.baseContextByte;
            glyph = glyph->next;
        } while (glyph != NULL);
    }
}

void frSetFontSpace(frFontHandle_t *pFRQ, s32 Space) {
    pFRQ->spacing = Space;
    pFRQ->advance = frGetStrWidth(pFRQ);
}

INCLUDE_ASM(const s32, "interface/frFont", frCallSetFontSpace);

INCLUDE_ASM(const s32, "interface/frFont", frCallSetFontSpace2);

void frSetFontSize(frFontHandle_t *pFRQ, s32 Width, s32 Height) {
    frFontHandle_t *head = pFRQ;
    frTextHandle_t *childGlyph;
    s32 index;

    head->parentDimensionsOrRenderWord.parentDimensions.cellAdvance = Width;
    head->parentDimensionsOrRenderWord.parentDimensions.cellHeight = Height;
    for (; pFRQ != NULL; pFRQ = pFRQ->previous) {
        for (childGlyph = pFRQ->firstChild; childGlyph != NULL; childGlyph = childGlyph->next) {
            if (childGlyph->glyphCodeOrContext.glyphCode < 0x80) {
                index = childGlyph->glyphCodeOrContext.glyphCode - 0x20;
            } else {
                index = (((childGlyph->glyphCodeOrContext.glyphCode - 0x8080) & 0xFF00) >> 1)
                    + ((childGlyph->glyphCodeOrContext.glyphCode - 0x8080) & 0x7F);
            }
            frCallGetFontWidth2(childGlyph, index, Width, Height);
        }
    }
    head->advance = frGetStrWidth(head);
}

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
INCLUDE_ASM(const s32, "interface/frFont", frSetFontDrawOT);
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

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 146 of 210 words match.  */
#ifdef NON_MATCHING
s32 frDrawEffect(frFontHandle_t *parent, frTextHandle_t *glyph, u8 threshold, u8 step) {
    frFontHandle_t *previousParent;
    frTextHandle_t *previousGlyph;
    u32 *fadeWord;
    u8 previousOption;
    u8 previousFade = 0x80;
    s32 canAdvanceFade = 0;
    s32 canStopLips = 1;
    s32 glyphJitter = 0;

    previousGlyph = glyph->previous;
    previousOption = 0;
    if (previousGlyph == NULL) {
        previousParent = parent->previous;
        if (previousParent != NULL && previousParent->lastChild != NULL) {
            previousFade = previousParent->lastChild->parentDimensionsOrRenderWord.fadeByte.value;
            previousOption = previousParent->lastChild->renderValueOrSetupOrShade.setupBytes.secondOption;
        }
    } else {
        previousFade = previousGlyph->parentDimensionsOrRenderWord.fadeByte.value;
        previousParent = parent->previous;
    }

    fadeWord = &glyph->parentDimensionsOrRenderWord.renderWord;
    if (previousOption == 0 || previousOption == glyph->renderValueOrSetupOrShade.setupBytes.secondOption) {
        if (previousFade >= threshold) {
            canAdvanceFade = 1;
        }
    } else {
        canAdvanceFade = previousFade == 0x80;
    }

    if (previousParent != NULL && parent->firstChild == glyph) {
        switch (previousParent->tag_flag[0]) {
        case CCN_MW:
            if (previousParent->wait > 0) {
                canAdvanceFade = 0;
                if (previousFade == 0x80) {
                    previousParent->wait--;
                }
                frLeaf(parent);
            }
            break;
        case CCN_VMW:
            if (previousParent->wait != CCN_VMW_VOICE) {
                if (previousParent->wait > 0) {
                    canAdvanceFade = 0;
                    previousParent->wait--;
                }
            } else if (scrCheckEnableMovieCancel() != 0) {
                canAdvanceFade = 0;
            }
            break;
        }
    }

    if (parent->tag_flag[2] == CCN_LIPS_STOP) {
        u32 lastFade = parent->lastChild->parentDimensionsOrRenderWord.fadeByte.value;
        s32 fading = lastFade < 0x80;

        canStopLips = fading == 0;
        if (canStopLips) {
            evtLipsStopFunction_evtCommand(0);
            parent->tag_flag[2] = 0;
        }
    }
    if (parent->tag_flag[5] == CCN_LIPS_STOP2) {
        u32 lastFade = parent->lastChild->parentDimensionsOrRenderWord.fadeByte.value;

        if (lastFade < 0x80) {
            canStopLips = 0;
        }
        if (canStopLips) {
            evtLipsStopFunction_evtCommand(parent->work[0]);
            parent->tag_flag[5] = 0;
        }
    }
    if (parent->tag_flag[1] == CCN_CHIME && parent->firstChild->parentDimensionsOrRenderWord.fadeByte.value > 0x50
        && parent->work[1] != 0) {
        p2sdPlaySE(frData[parent->work[0]], 127, 63);
        parent->work[1] = 0;
    }
    if (parent->tag_flag[3] == CCN_QUAKE_ON && parent->firstChild->parentDimensionsOrRenderWord.fadeByte.value > 0x50
        && parent->work[1] != 0) {
        itfLeaf13(parent->work[0], parent->work[1]);
        parent->work[1] = 0;
    }
    if (parent->tag_flag[4] == CCN_QUAKE_OFF) {
        u32 lastFade = parent->lastChild->parentDimensionsOrRenderWord.fadeByte.value;

        if (lastFade != 0x80) {
            FadeOutWindow();
            parent->tag_flag[4] = 0;
        }
    }

    if (canAdvanceFade) {
        u32 currentFadeWord;

        if ((s8)step >= 0) {
            step = (step * 22) / (glyph->advance + parent->spacing);
        }
        currentFadeWord = *fadeWord;
        if ((u32)(0x80 - (currentFadeWord & 0xFF)) >= step) {
            *fadeWord = currentFadeWord + step;
        } else {
            *fadeWord = (currentFadeWord & ~0xFF) | 0x80;
        }
    }

    {
        s8 currentFade = fadeWord[0];
        u16 glyphValue = glyph->drawCount;

        if (currentFade >= 0) {
            if (glyph->renderValueOrSetupOrShade.setupBytes.secondOption == 1) {
                glyphJitter = -((((glyphValue * 2) % 5) - 2) << 4);
            }
        }
    }
    return glyphJitter;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frDrawEffect);
#endif
#else
INCLUDE_ASM(const s32, "interface/frFont", frDrawEffect);
#endif

/* body: matched in smt3 @ 9401f79e: src/smt3/interface/frFont.c:1523, match, same function 1.00 (words 6/6, calls 1.00, shape 1.00) */
s32 frDrawFont(frFontHandle_t *pFRQ) {
    return frDrawFontEx(pFRQ, 0);
}

s32 frDrawFontEx(frFontHandle_t *pFRQ, s8 Mode) {
    return frDrawFontOt(pFRQ, Mode, gFontDrawOT);
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 189 of 211 words match.  */
#ifdef NON_MATCHING
s32 frDrawFontOt(frFontHandle_t *pFRQ, s8 mode, u32 ot) {
    frFontHandle_t *glyph;
    frTextHandle_t *child;
    s32 ready = 1;
    s32 enabled = 1;
    s32 canStopLips = 1;
    s32 result = 0;
    s32 totalGlyphCount = 0;
    s32 canStopQuake = 1;

    if (pFRQ != NULL) {
        glyph = pFRQ->chainHead;
        if (glyph != NULL) {
            do {
                s32 x = glyph->x;
                s32 y = glyph->y;
                s8 spacing = glyph->spacing;

                child = glyph->firstChild;
                while (child != NULL) {
                    s32 xOffset = mode != 0 ? 0 :
                        frDrawEffect(glyph, child, 0x1C, glyph->glyphCodeOrContext.byteRoles.encodedContextByte);
                    u32 glyphState;

                    if (child->sourceItem == NULL) {
                        if ((u16)child->glyphCodeOrContext.glyphCode < 0x80) {
                            enabled = 1;
                        }
                        frCreateDrawFontPacket_20550848(x + child->x + xOffset, y + child->y,
                            child->cellDimensions.cellWidth, child->cellDimensions.cellHeight >> 1,
                            child->renderValueOrSetupOrShade.setupBytes.firstOption,
                            child->parentDimensionsOrRenderWord.renderWord,
                            glyph->renderValueOrSetupOrShade.renderValue, enabled,
                            &child->cachedItem->font_mem->uv, &gFT.atlas, ot);
                    } else {
                        frCreateDrawFontPacket_20550848(x + child->x + xOffset, y + child->y,
                            child->cellDimensions.cellWidth, child->cellDimensions.cellHeight >> 1,
                            child->renderValueOrSetupOrShade.setupBytes.firstOption,
                            child->parentDimensionsOrRenderWord.renderWord,
                            glyph->renderValueOrSetupOrShade.renderValue, enabled,
                            &child->sourceItem->font_mem->uv, &gFT.atlas, ot);
                    }
                    glyphState = child->parentDimensionsOrRenderWord.fadeByte.value;
                    if (glyphState != 0) {
                        child->drawCount++;
                    }
                    if (glyphState < 0x80) {
                        ready = 0;
                    }
                    x += (child->advance + spacing) << 4;
                    child = child->next;
                }

                if (glyph->next == NULL) {
                    switch (glyph->tag_flag[0]) {
                    case CCN_MW:
                        if (ready != 0 && glyph->wait > 0) {
                            glyph->wait--;
                            ready = 0;
                        }
                        break;
                    case CCN_VMW:
                        if (glyph->wait != CCN_VMW_VOICE) {
                            if (glyph->wait > 0) {
                                glyph->wait--;
                                ready = 0;
                            }
                        } else if (scrCheckEnableMovieCancel() != 0) {
                            ready = 0;
                        }
                        break;
                    }
                    if (glyph->tag_flag[2] == CCN_LIPS_STOP) {
                        if (ready == 0) {
                            canStopLips = 0;
                        }
                        if (canStopLips) {
                            evtLipsStopFunction_evtCommand(0);
                            glyph->tag_flag[2] = 0;
                        }
                    }
                    if (glyph->tag_flag[5] == CCN_LIPS_STOP2) {
                        if (ready == 0) {
                            canStopLips = 0;
                        }
                        if (canStopLips) {
                            evtLipsStopFunction_evtCommand(glyph->work[0]);
                            glyph->tag_flag[5] = 0;
                        }
                    }
                    if (glyph->tag_flag[1] == CCN_CHIME
                        && glyph->previous->firstChild->parentDimensionsOrRenderWord.fadeByte.value > 0x50
                        && glyph->work[1] != 0) {
                        p2sdPlaySE(frData[glyph->work[0]], 127, 63);
                        glyph->work[1] = 0;
                    }
                    if (glyph->tag_flag[4] == CCN_QUAKE_OFF) {
                        if (ready == 0) {
                            canStopQuake = 0;
                        }
                        if (canStopQuake) {
                            FadeOutWindow();
                            glyph->tag_flag[4] = 0;
                        }
                    }
                }
                totalGlyphCount += glyph->childCount;
                glyph = glyph->next;
            } while (glyph != NULL);
        }
        result = totalGlyphCount;
        if (ready == 0) {
            result = 0;
        }
    }
    return result;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frDrawFontOt);
#endif
#else
INCLUDE_ASM(const s32, "interface/frFont", frDrawFontOt);
#endif

s32 frFlipPacketBuf(dds3ProcessID_t *id) {
    s32 queueIndex = (dds3GetFrameID() & FR_FONT_BYTE_MASK) == 0;

    gFT.glyphSlots[queueIndex] = frDestroyFontRequestNow(gFT.glyphSlots[queueIndex]);
    return 0;
}

frFontHandle_t *frCombineFontRequest(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2) {
    return frCombineFontRequestEx(pFRQ1, pFRQ2, 1);
}

frFontHandle_t *frDestroyFontRequestAfterSub(frFontHandle_t *pFRQ1, frFontHandle_t *pFRQ2, s32 Mode) {
    if (pFRQ1 == NULL) {
        return pFRQ2;
    }
    if (pFRQ2 == NULL) {
        return pFRQ1;
    }
    pFRQ1->next = pFRQ2->chainHead;
    pFRQ2->chainHead->previous = pFRQ1;
    pFRQ2->chainHead = pFRQ1->chainHead;
    if (Mode == 1) {
        pFRQ2->x = pFRQ1->x + pFRQ1->advance * 16;
        pFRQ2->y = pFRQ1->y;
    }
    return pFRQ2;
}

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
u32 frGetStrWidth(const frFontHandle_t *pFRQ) {
    frTextHandle_t *childGlyph = pFRQ->firstChild;
    s32 totalAdvance = 0;

    if (childGlyph != NULL) {
        do {
            if (frData3 == 0) {
                totalAdvance += childGlyph->advance;
            } else {
                totalAdvance += childGlyph->advance * frData3
                    / frGetFontWidth(childGlyph->renderValueOrSetupOrShade.setupBytes.fontIndex);
            }
            childGlyph = childGlyph->next;
            totalAdvance += pFRQ->spacing;
        } while (childGlyph != NULL);
    }
    return totalAdvance;
}
#else
u32 frGetStrWidth(const frFontHandle_t *pFRQ) {
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

/* body: matched in store @ 2026-11-16: /home/wsl/.local/state/raidou-decomp-work/clusters/store/raidou1/0017FEB8.c:1, match */
s32 frCallGetFontWidth3(const char *str, u8 font, u8 unk2, s32 spacing)
{
    frTextHandle_t glyph;
    s32 totalWidth = 0;
    u32 i = 0;
    u32 length;

    glyph.renderValueOrSetupOrShade.setupBytes.fontIndex = font;
    glyph.renderValueOrSetupOrShade.setupBytes.sharedFlags = unk2;
    glyph.advance = 0;
    glyph.cellDimensions.cellWidth = frGetFontWidth(glyph.renderValueOrSetupOrShade.setupBytes.fontIndex);
    glyph.cellDimensions.cellHeight = frGetFontHeight(glyph.renderValueOrSetupOrShade.setupBytes.fontIndex);
    length = strlen(str);

    while (i < length) {
        u32 code = (u8)str[i];
        s32 glyphIndex;

        if (code >= 0x80) {
            i++;
            code = (code << 8) | (u8)str[i];
        }
        glyph.glyphCodeOrContext.glyphCode = code;
        if (code < 0x80) {
            glyphIndex = code - 0x20;
        } else {
            u32 adjusted = code - 0x8080;

            glyphIndex = ((adjusted & 0xFF00) >> 1) + (adjusted & 0x7F);
        }
        frSetFontSub2(&glyph, glyphIndex);
        i++;
        totalWidth += glyph.advance + spacing;
    }
    return totalWidth;
}

u16 frGetFontWidth(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellWidth;
}

u16 frGetFontHeight(s32 slotIndex) {
    return gFT.entries[slotIndex & FR_FONT_BYTE_MASK].resourceHeader->cellHeight;
}

void frSetMessageDefStepY(void) {
    gMessageStepY = FR_FONT_DEFAULT_CURSOR_SPACING;
}

void frSetMessageStepY(u32 StepY) {
    gMessageStepY = StepY;
}

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

#ifdef VERSION_RAIDOU1
void frFontGetGlyphPositionAtIndex(frFontPosition_t *position, s32 requestedIndex, const _FRQ *glyph) {
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
/* Write each visited position and stop at requestedIndex. Missing indices leave
 * the last visited position; empty input leaves output unchanged. */
void frFontGetGlyphPositionAtIndex(frFontPosition_t *position, s32 requestedIndex, const _FRQ *glyph) {
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

/* NON_MATCHING: 43 of 90 words match.  */
#ifdef NON_MATCHING
void frFontAppendIndexedTextToGlyphChain(u8 VarNo, frFontCtx_t *ctx) {
    u8 glyphText[3];
    const char *textData;
    s32 textByteOffset;
    s32 textLength;
    s8 previousMode;
    s8 mode;
    frFontHandle_t *context;
    frFontHandle_t *existingGlyph;

    textData = (const char *)gVarStr[VarNo];
    memset(glyphText, 0, sizeof(glyphText));
    if (textData == NULL) {
        return;
    }

    textLength = strlen(textData);
    previousMode = ctx->fontIndex;
    for (textByteOffset = 0; textByteOffset < textLength; ) {
        glyphText[0] = textData[textByteOffset++];
        glyphText[1] = textData[textByteOffset++];
        mode = ctx->fontIndex;

        if (mode != previousMode) {
            context = ctx->glyphChain;
            if (context != NULL) {
                if (context->firstChild != NULL) {
                    ctx->glyphChain = frCallSetFontString(frFontAppendIndexedTextToGlyphChainData, 0, ctx->firstOption, ctx->secondOption, context);
                    context = ctx->glyphChain;
                }
            } else {
                ctx->glyphChain = frCallSetFontString(frFontAppendIndexedTextToGlyphChainData, 0, ctx->firstOption, ctx->secondOption, NULL);
                context = ctx->glyphChain;
            }
            frSetDrawSpeed(context, ctx->contextEncodedByte);
            previousMode = mode;
        }

        existingGlyph = frSetFontString(glyphText, mode, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
        ctx->glyphChain = existingGlyph;
        frSetFontSpace(existingGlyph, gMessageSpaceX);
    }
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frFontAppendIndexedTextToGlyphChain);
#endif

void frFontCreateContext(frFontCtx_t *ctx) {
    s16 cellHeight = 0;
    _FRQ *childGlyph;

    if (ctx->glyphChain != NULL) {
        cellHeight = ctx->glyphChain->parentDimensionsOrRenderWord.parentDimensions.cellHeight;
    }
    childGlyph = frCallSetFontString(frFontAppendIndexedTextToGlyphChainData, 0, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
    childGlyph->parentDimensionsOrRenderWord.parentDimensions.cellHeight = cellHeight;
    ctx->glyphChain = childGlyph;
    frSetDrawSpeed(childGlyph, ctx->contextEncodedByte);
    ctx->pendingCreate = 0;
}

/* Cancel creation for an empty child, process pending creation, then process
 * the independent pair refresh. The refresh phase has no NULL-child guard. */
void frFontCheckPendingGlyphState(frFontCtx_t *ctx) {
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
        /* BUG: retail calls without setting $a0, which still holds ctx. */
#ifdef FIX_BUGS
        frFontCreateContext(ctx);
#else
        ((void (*)())frFontCreateContext)();
#endif
        pending = ctx->pendingPosition;
    }
    if (pending != 0) {
        frSetDrawPosition(ctx->glyphChain, ctx->x, ctx->y);
        ctx->pendingPosition = 0;
    }
}

/* Advance by eight times cursor spacing and request both context updates. */
void frMessageReturnCode(frFontCtx_t *pMsgInfo) {
    pMsgInfo->y += gMessageStepY * FR_FONT_CONTEXT_CURSOR_SCALE;
    pMsgInfo->pendingCreate = 1;
    pMsgInfo->pendingPosition = 1;
}

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
#ifdef VERSION_RAIDOU1
u32 itfReadEncodedTextLead(frFontCtx_t *args) {
    s32 *position = &args->encodedTextOffset;
    const u8 *byte = args->encodedText + *position;
    u32 value = *byte;

    *position += 2;
    return (value + ITF_BYTE_MASK) & ITF_BYTE_MASK;
}

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
#else
u32 itfReadEncodedTextLead(frFontCtx_t *stream) {
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
u32 itfReadEncodedCode(frFontCtx_t *args) {
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

/* Run one 0xFx control tag; returns 1 when the message stops here. */
s32 execTag_0(s32 code, frFontCtx_t *ctx) {
    s32 *position = &ctx->encodedTextOffset;
    const u8 *bytes = ctx->encodedText;
    s32 payloadWords = code & 0xF;
    s32 payloadPosition = *position;
    s32 arg;

    code = (code << 8) | bytes[payloadPosition++];
    *position = payloadPosition;
    switch (code) {
    case 0xF206:
        /* A style 1 tag right after a style 2 tag (directly, after one
         * character, after a line feed, or after variable 1 and a return)
         * is skipped. */
        if (bytes[payloadPosition] == 1) {
            if (((bytes[payloadPosition - 6] << 8) | bytes[payloadPosition - 5]) == 0xF206
                && bytes[payloadPosition - 4] == 2) {
                *position = payloadPosition + 2;
                break;
            }
            if ((bytes[ctx->encodedTextOffset] == 1
                 && ((bytes[ctx->encodedTextOffset - 8] << 8) | bytes[ctx->encodedTextOffset - 7]) == 0xF206
                 && bytes[ctx->encodedTextOffset - 6] == 2)) {
                ctx->encodedTextOffset += 2;
                break;
            }
            if ((bytes[ctx->encodedTextOffset] == 1
                    && ((bytes[ctx->encodedTextOffset - 7] << 8) | bytes[ctx->encodedTextOffset - 6]) == 0xF206
                    && bytes[ctx->encodedTextOffset - 5] == 2
                    && bytes[ctx->encodedTextOffset - 3] == '\n')) {
                ctx->encodedTextOffset += 2;
                break;
            }
            if ((bytes[ctx->encodedTextOffset] == 1
                    && ((bytes[ctx->encodedTextOffset - 12] << 8) | bytes[ctx->encodedTextOffset - 11]) == 0xF206
                    && bytes[ctx->encodedTextOffset - 10] == 2
                    && ((bytes[ctx->encodedTextOffset - 8] << 8) | bytes[ctx->encodedTextOffset - 7]) == 0xF203
                    && bytes[ctx->encodedTextOffset - 6] == 1
                    && ((bytes[ctx->encodedTextOffset - 4] << 8) | bytes[ctx->encodedTextOffset - 3]) == 0xF10F)) {
                ctx->encodedTextOffset += 2;
                break;
            }
        }
        {
            s32 offset = *position;

            ctx->fontIndex = bytes[offset] - 1;
            *position = offset + 2;
        }
        break;
    case 0xF202:
        ctx->firstOption = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF209:
        ctx->secondOption = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF207:
        ctx->contextEncodedByte = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        break;
    case 0xF203:
        if (gVarStr[bytes[payloadPosition] - 1] != 0) {
            frFontCheckPendingGlyphState(ctx);
            frFontAppendIndexedTextToGlyphChain(ctx->encodedText[*position] - 1, ctx);
        }
        *position += 2;
        break;
    case 0xF20A: {
        s32 arg = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        frFontCheckPendingGlyphState(ctx);
        if (arg == 1) {
            frTag0Sub(ctx);
        } else {
            nbCallSetFormPosRot(arg, ctx);
        }
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    }
    case 0xF20B:
        arg = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        frFontCheckPendingGlyphState(ctx);
        nbSetFormPosRot(nbTag0Sub2(arg), ctx);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    case 0xF20C:
        arg = bytes[payloadPosition] - 1;
        *position = payloadPosition + 2;
        frFontCheckPendingGlyphState(ctx);
        nbSetFormPosRot(nbTag0Sub(arg), ctx);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    case 0xF20D:
        break;
    case 0xF20E: {
        s32 arg = bytes[payloadPosition];
        *position = payloadPosition + 2;
        frFontCheckPendingGlyphState(ctx);
        if (arg == 0) {
            arg = DDS3_GBWK->mapSummonID;
        }
        ctx->glyphChain = frSetFontString((u8 *)g_itemNo[arg], ctx->fontIndex, ctx->firstOption,
                                          ctx->secondOption, ctx->glyphChain);
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        break;
    }
    case 0xF10F:
        if (!(gModeFlag & 4)) {
            goto advanceLine;
        }
        break;
    case 0xF110:
        gMessageRequestResult |= 2;
        return 1;
    case 0xF111:
        if (!(gModeFlag & 8)) {
            goto checkAutomaticLine;
        }
    case 0xF104:
        gMessageRequestResult |= 1;
        return 1;

    checkAutomaticLine:
        if (!(gModeFlag & 0x20)) {
            break;
        }
    advanceLine:
        frMessageReturnCode(ctx);
        break;
    case 0xF112:
        gMessageRequestResult |= 4;
        break;
    case 0xF413:
        mnuSetTitleVoicePrefixIndex(itfReadEncodedTextLead(ctx));
        fldNowPlayengStart(itfReadEncodedCode(ctx));
        break;
    case CCN_MW:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[0] = code;
        ctx->glyphChain->wait = itfReadEncodedCode(ctx);
        break;
    case CCN_VMW:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[0] = code;
        ctx->glyphChain->wait = itfReadEncodedCode(ctx);
        if (ctx->glyphChain->wait != CCN_VMW_VOICE) {
            ctx->glyphChain->wait -= mnuGetTitleEffectFrameCounter();
        }
        if (ctx->glyphChain->wait < 0) {
            ctx->glyphChain->wait = 0;
        }
        break;
    case 0xF416:
        itfTextLipsUnitId = itfReadEncodedTextLead(ctx);
        itfTextLipsMotionId = itfReadEncodedTextLead(ctx);
        gMessageRequestResult |= 8;
        break;
    case CCN_LIPS_STOP:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[2] = code;
        break;
    case CCN_CHIME:
        if (itfReadEncodedTextLead(ctx) != 0) {
            break;
        }
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->work[1] = 1;
        ctx->glyphChain->tag_flag[1] = code;
        ctx->glyphChain->work[0] = 0;
        break;
    case CCN_QUAKE_ON:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[3] = code;
        ctx->glyphChain->work[0] = itfReadEncodedTextLead(ctx);
        ctx->glyphChain->work[1] = itfReadEncodedCode(ctx);
        break;
    case CCN_QUAKE_OFF:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[4] = code;
        break;
    case CCN_LIPS_STOP2:
        frFontCheckPendingGlyphState(ctx);
        ctx->glyphChain->tag_flag[5] = code;
        ctx->glyphChain->work[0] = itfReadEncodedTextLead(ctx);
        break;
    default:
        ctx->encodedTextOffset += (payloadWords - 1) << 1;
        break;
    }
    if (ctx->pendingCreate == 0) {
        ctx->pendingCreate = 1;
    }
    return 0;
}

#ifdef VERSION_RAIDOU1
frFontHandle_t *frAnalyzeMessage(frFontCtx_t *ctx) {
    u8 encodedText[3];
    s32 code;

    if ((ctx->encodedText[0] & 0xF0) != 0xF0 && ctx->encodedText[1] == 8) {
        ctx->encodedTextOffset += 4;
    }
    gMessageRequestResult &= ~1;
    gMessageRequestResult &= ~2;
    gMessageRequestResult &= ~4;
    gMessageRequestResult &= ~8;
    frEnableMode(1);
    frDisableMode(2);
    while (1) {
        code = ctx->encodedText[ctx->encodedTextOffset++];
        if (code == 0) {
            break;
        }
        if ((code & 0xF0) == 0xF0) {
            if (execTag_0(code, ctx) != 0) {
                break;
            }
            continue;
        }
        if (code == 0xA) {
            if ((gModeFlag & 4) != 0) {
                frMessageReturnCode(ctx);
            }
            continue;
        }
        frFontCheckPendingGlyphState(ctx);
        if (code < 0x80) {
            encodedText[0] = code;
            encodedText[1] = 0;
            ctx->glyphChain = frSetFontString(encodedText, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
        } else {
            code = (code << FR_FONT_BYTE_SHIFT) | ctx->encodedText[ctx->encodedTextOffset++];
            if (code <= 0xC07F) {
                encodedText[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encodedText[1] = code;
                encodedText[2] = 0;
                ctx->glyphChain = frSetFontString(encodedText, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
            } else {
                code -= 0x4000;
                encodedText[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encodedText[1] = code;
                encodedText[2] = 0;
                ctx->glyphChain = frSetFontString(encodedText, FR_FONT_TEMPORARY_SLOT, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
            }
        }
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        frSetPriority(ctx->glyphChain, ctx->z);
    }
    frDisableMode(1);
    frEnableMode(2);
    return ctx->glyphChain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeMessage);
#endif

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, 0, 0, 0, 0x80, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    frFontCtx_t args;

    args.x = X;
    args.y = Y;
    args.z = Z << 4;
    args.fontIndex = Style;
    args.firstOption = ColNo;
    args.secondOption = Tp;
    args.contextEncodedByte = Spd;
    args.encodedText = pMsg;
    args.glyphChain = pFRQ;
    args.encodedTextOffset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return frAnalyzeMessage(&args);
}

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain;

    gMessageSpaceX = 0;
    chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}

frFontHandle_t *frCallMessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg,
    frFontHandle_t *pFRQ, s32 Line) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain;

    gMessageSpaceX = Line;
    frData2 = Line;
    chain = MessageRequestEx2(X, Y, Z, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, NULL);
    gMessageSpaceX = savedSpacing;
    frData2 = savedSpacing;
    return chain;
}

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
u32 itfReadEncodedCode(frFontCtx_t *stream) {
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

/* NON_MATCHING: 56 of 142 words match. branch layout around the code < 0x80 test. */
#ifdef NON_MATCHING
frFontHandle_t *frAnalyzeMessage(frFontCtx_t *ctx) {
    u8 encodedText[3];
    s32 code;

    if ((ctx->encodedText[0] & 0xF0) != 0xF0 && ctx->encodedText[1] == 8) {
        ctx->encodedTextOffset += 4;
    }
    gMessageRequestResult &= ~1;
    gMessageRequestResult &= ~2;
    gMessageRequestResult &= ~4;
    gMessageRequestResult &= ~8;
    gMessageRequestResult &= ~0x10;
    frEnableMode(1);
    frDisableMode(2);
    while (1) {
        code = ctx->encodedText[ctx->encodedTextOffset++];
        if (code == 0) {
            break;
        }
        if ((code & 0xF0) == 0xF0) {
            if (execTag_0(code, ctx) != 0) {
                break;
            }
            continue;
        }
        if (code == 0xA) {
            if ((gModeFlag & 4) != 0) {
                frMessageReturnCode(ctx);
            }
            continue;
        }
        frFontCheckPendingGlyphState(ctx);
        if (code < 0x80) {
            if (code == 0x24 && ctx->fontIndex == 0) {
                encodedText[0] = 0x82;
                encodedText[1] = 0xD4;
                encodedText[2] = 0;
            } else {
                encodedText[0] = code;
                encodedText[1] = 0;
            }
            ctx->glyphChain = frSetFontString(encodedText, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
        } else {
            code = (code << FR_FONT_BYTE_SHIFT) | ctx->encodedText[ctx->encodedTextOffset++];
            if (code <= 0xC07F) {
                encodedText[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encodedText[1] = code;
                encodedText[2] = 0;
                ctx->glyphChain = frSetFontString(encodedText, ctx->fontIndex, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
            } else {
                code -= 0x4000;
                encodedText[0] = (u32)code >> FR_FONT_BYTE_SHIFT;
                encodedText[1] = code;
                encodedText[2] = 0;
                ctx->glyphChain = frSetFontString(encodedText, FR_FONT_TEMPORARY_SLOT, ctx->firstOption, ctx->secondOption, ctx->glyphChain);
            }
        }
        frSetFontSpace(ctx->glyphChain, gMessageSpaceX);
        frSetPriority(ctx->glyphChain, ctx->z);
    }
    frDisableMode(1);
    frEnableMode(2);
    return ctx->glyphChain;
}
#else
INCLUDE_ASM(const s32, "interface/frFont", frAnalyzeMessage);
#endif

frFontHandle_t *MessageRequest(s32 X, s32 Y, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, 0, 0, 0, 0x80, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx(s32 X, s32 Y, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    return MessageRequestEx2(X, Y, 0, Style & 0xFF, ColNo & 0xFF, Tp & 0xFF, Spd & 0xFF, pMsg, pFRQ);
}

frFontHandle_t *MessageRequestEx2(s32 X, s32 Y, s32 Z, s32 Style, s32 ColNo, s32 Tp, s32 Spd, u8 *pMsg, frFontHandle_t *pFRQ) {
    frFontCtx_t args;

    args.x = X;
    args.y = Y;
    args.z = Z << 4;
    args.fontIndex = Style;
    args.firstOption = ColNo;
    args.secondOption = Tp;
    args.contextEncodedByte = Spd;
    args.encodedText = pMsg;
    args.glyphChain = pFRQ;
    args.encodedTextOffset = 0;
    args.pendingCreate = 1;
    args.pendingPosition = 1;
    return frAnalyzeMessage(&args);
}

frFontHandle_t *MessageRequestNS(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, u8 *encodedText, frFontHandle_t *unusedPrevious) {
    s8 savedSpacing = gMessageSpaceX;
    frFontHandle_t *chain;

    gMessageSpaceX = 0;
    chain = MessageRequestEx2(x, y, depth, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, NULL);

    gMessageSpaceX = savedSpacing;
    return chain;
}

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
INCLUDE_SDATA(const s32, "interface/frFont", frSetFontString_style);
#else
INCLUDE_SDATA(const s32, "interface/frFont", ButtonLineHelpAddTextSetData);

INCLUDE_SDATA(const s32, "interface/frFont", frSetFontString_style);
#endif

INCLUDE_SDATA(const s32, "interface/frFont", frFontAppendIndexedTextToGlyphChainData);

