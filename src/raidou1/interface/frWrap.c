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

extern const char frTestFontText[];

extern void frSetDrawSpeed(frFontHandle_t *request, s32 speed);

void fr_1CAC50(void);

extern const char frTestFontText[24];
/* End of declarations from src/smt3/interface/frWrap.c */

/* Declarations from dds-decomp src/dds1/game/code_00196478.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_frBitRange_t 1

#define SMT3_DEFINES_frFontNode_t 1

#define SMT3_DEFINES_frFontRing_t 1

#define SMT3_DEFINES_frImagePtrs_t 1

#define SMT3_DEFINES_frPackedHeader_t 1

#define SMT3_DEFINES_frRelativeImage_t 1

#define SMT3_DEFINES_frTextBank_t 1

#include "scr.h"

#include "sdk_libc.h"

#include "dds/dds3_admin_control_api.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_state_query.h"

#include "dds/sdf_gs_packet.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_list.h"

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

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

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

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_state.h"

#include "dds/dds3_task_state_query.h"

#include "dds/sdf_heap_api.h"

#include "dds/itf.h"

#include "dds/itf_panel_api.h"

#include "dds/fr_font_glyph_chain_count.h"

#include "dds/fr_font_measure.h"

#include "dds/itf_mem_node.h"

#include "dds/itf_resource_segments.h"

#include "dds/sdf.h"

#include "dds/itf_panel_draw.h"

#include "dds/sdf_texture_file.h"

#include "dds/btl.h"

#include "dds/fr_font.h"

/* Declared before first use (were implicit int). */
extern _FRQ * frDestroyFontRequest(_FRQ *glyph);

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

#include "dds/sdf_debug_api.h"

extern dds3ProcessID_t *itfFontTestScriptTask;

extern u32 D_003BB18C;

extern sdfTexHandle_t *itfBackgroundSpriteTexture;

extern u8 D_003BB188[];

extern void frFontCheckPendingGlyphState(FrFontCtx *);

extern void frMessageReturnCode(FrFontCtx *);

extern void frFontAppendIndexedTextToGlyphChain(u8, FrFontCtx *);

extern u8 gModeFlag;

extern void mnuSetTitleVoicePrefixIndex(s32);

extern void mnuPlayTitleVoiceFile(char *);

extern s32 mnuGetTitleEffectFrameCounter(void);

extern _FRQ *frFontLinkGlyph(_FRQ *, _FRQ *, s32);

extern u8 D_00357980[];

extern u8 D_00357988[];

extern s32 frDefaultFontHandle;

extern u32 itfTextLipsMotionId;

extern u32 itfTextLipsUnitId;

extern s32 D_003BD818;

void frFontEnsureSlotLoaded(s32 id, const char *path);

extern u16 itfGlyphDecodeTable[];

extern s32 itfScanEncodedTextControls(FrFontCtx *args);

/* Subtract one modulo 256 from the first byte, then advance over the whole pair. */
u32 itfReadEncodedTextLead(FrFontCtx *args);

/* Decode a little-endian pair; encoded high byte 0xFF is the zero escape.
 * Neither reader checks the input length. */
u32 itfReadEncodedCode(FrFontCtx *args);

extern u32 D_003BB15C;

extern FrFontSystem frFontWork;

extern u8 *datSkillMessages;

extern u8 *datItemMessages;

s32 itfProcessEncodedTextControl(s32 code, FrFontCtx *stream);

extern s8 frFontEncodedTextGlyphSpacing;

extern _FRQ * frFontBuildGlyphChain(const char *text, s8 fontIndex, s8 firstOption, s8 secondOption, _FRQ *existingGlyph);

_FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *stream);

_FRQ *MessageRequest(s32 x, s32 y, const u8 *encodedText, _FRQ *sub);

_FRQ *MessageRequestEx(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *sub);

extern _FRQ *itfBuildGlyphChainFromEncodedStream(FrFontCtx *);

_FRQ *MessageRequestEx2(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *sub);

_FRQ *itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
                   s32 channel2, s32 channel3, const u8 *encodedText, _FRQ *unusedSub);

u32 frGetMessageResult(u32 mask);

/* Store a replacement-text byte pointer in the font's indexed table. */
void frSetVariableRequest(s32 slotIndex, const u8 *encodedText);

/* Return the unit identifier saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsUnitId(void);

/* Return the motion number saved by text lip-sync control 0xF416. */
u32 itfGetTextLipsMotionId(void);

s32 itfSkipEncodedTextOpcode(s32 code, FrFontCtx *stream);

s32 itfScanEncodedTextControls(FrFontCtx *stream);

s32 itfInitTextDrawArgs(const u8 *encodedText, _FRQ *sub);

/* Return the selected resource header's cell width; index is unchecked. */
u16 frFontGetSlotCellWidth(s32 slotIndex);

/* Return the selected resource header's cell height; index is unchecked. */
u16 frFontGetSlotCellHeight(s32 slotIndex);

void itfSetTextDrawLimit(s32 limit);

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 glyphCode);

/* Preserve single-byte text and substitute 0x8080 for unmapped glyph pairs. */
void itfConvertText(u8 *output, const char *input);

/* Build a glyph with a fixed 16x18 cell, then attach it to its parent. */
void itfAttachGlyph16x18(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent);

/* Create positioned, colored text with measured spacing and link it to parent. */
_FRQ *itfCreateMeasuredTextGlyph(s32 x, s32 y, s32 depth, s32 colors,
                           const char *text, _FRQ *parent);

/* Decode two-byte glyph codes before building and linking the text glyph. */
_FRQ *itfCreateConvertedTextGlyph(s32 x, s32 y, s32 depth, u32 colors, const u8 *text, _FRQ *parent);

_FRQ *itfCreateFont2TextGlyph(s32 x, s32 y, s32 depth, u32 colors, char *text, _FRQ *previousGlyph);

/* Build text with font index 3; retain the existing glyph chain's spacing. */
_FRQ *itfCreateFont3TextGlyph(s32 x, s32 y, s32 depth, u32 colors, const char *text, _FRQ *previousGlyph);

/* Build font-index-3 text with signed spacing -2 and return its glyph chain. */
_FRQ *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, s32 colors, char *text, _FRQ *previousGlyph);

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent);

extern void frFontMoveChainTo(s32 x, s32 y, _FRQ *text);

_FRQ *itfDrawBankTextWithLayoutFlags(s32 x, s32 y, s32 depth, u16 textId, FrFontTextBank *bank, s32 flags);

_FRQ *itfDrawTextWithSelectedFontMode(s32 x, s32 y, s32 depth, s8 fontMode, u16 textId, s32 flags);

_FRQ *itfDrawUnderscoreTextSegment();

u32 itfReturnZero(void);

void func_00198010(void);

void func_00198018(void);

u32 itfReturnThirdCallbackValue(u32 unused0, u32 unused1, u32 value);

/* Return the byte count saved by the most recent packed-resource decode. */
s32 itfGetLastDecodedByteCount(void);

/* Read the opaque header word at 0x10; its role is not established. */
u32 itfGetPackedResourceHeaderWord(u32 resourceBase);

/* Acquire the background texture, then release the temporary file allocation. */
void nbLoadDeadMoonTex(void);

/* Drop the held texture reference without clearing the global. */
void itfReleaseBackgroundSpriteTexture(void);

struct EffSpriteUV;

struct sdfTexHandle_t;

union EffSpriteColor;

extern void itfDrawTexturedSpriteRect(s32 x, s32 y, u32 depth, s32 width, s32 height, const struct EffSpriteUV *uvRect, const union EffSpriteColor *color, u32 flipFlags, u32 blendKind, s32 mode, struct sdfTexHandle_t *texture, s32 surfaceId);

/* Draw only when a texture is present; native X/Y extent scaling differs. */
void itfDrawBackgroundSprite(void);

extern s32 itfDrawBackgroundAndGetTaskReadyMask(dds3ProcessID_t *task);

extern void itfReleaseFontTestTaskResources(dds3ProcessID_t *task);

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

#include "dds/sdf_packet_alloc.h"

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

extern void *sdf2DMakeStdPacket(void *, s32);

extern u64 D_00357998[];

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

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *command, u64 value, s32 flag);

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *command, s32 index, s32 flag);

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *command, s32 flag, s32 wms, s32 wmt, s32 minu, s32 maxu, s32 minv, s32 maxv);

extern u8 D_003BB198[8];

extern u8 D_003BB1A0[8];

extern u8 D_003579B0[];

extern u8 D_003579B8[];

extern TextVector D_003579C0[4];

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 value, s32 limit, s32 highlight,
                   DrawVertex *bounds, u32 tail, DrawColorRec *filledColor,
                   DrawColorRec *remainingColor, sdfDrawTag_t *command);

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *task);

u32 itfCamp(void);

extern char D_003A13F0[];

void itfCampStartProcess(void);

extern s32 D_00357AB8[];

dds_itfpanel_t *itfCreateUiSpriteByKind(s32 kind, const sdfTexHandle_t *borrowedTexture);
/* End of declarations from src/dds1/game/code_00196478.c */

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

INCLUDE_ASM(const s32, "interface/frWrap", func_00182CA8);

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

INCLUDE_ASM(const s32, "interface/frWrap", func_00182DC0);

INCLUDE_ASM(const s32, "interface/frWrap", func_00182F50);

INCLUDE_ASM(const s32, "interface/frWrap", func_00182F78);

INCLUDE_ASM(const s32, "interface/frWrap", func_00182FA8);

INCLUDE_ASM(const s32, "interface/frWrap", func_00183138);

INCLUDE_ASM(const s32, "interface/frWrap", func_00183218);

INCLUDE_ASM(const s32, "interface/frWrap", func_001832E8);

INCLUDE_ASM(const s32, "interface/frWrap", func_001833C8);

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

INCLUDE_ASM(const s32, "interface/frWrap", func_00183658);

INCLUDE_ASM(const s32, "interface/frWrap", itfDrawUnderscoreTextSegment);

INCLUDE_ASM(const s32, "interface/frWrap", func_00183800);

void func_00183808(void) {
}

void func_00183810(void) {
}

INCLUDE_ASM(const s32, "interface/frWrap", func_00183818);

INCLUDE_ASM(const s32, "interface/frWrap", func_00183820);

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

INCLUDE_ASM(const s32, "interface/frWrap", func_00183860);

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

INCLUDE_SDATA(const s32, "interface/frWrap", D_0046C1E8);

