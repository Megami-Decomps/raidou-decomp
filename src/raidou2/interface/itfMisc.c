#include "common.h"

/* Declarations from smt3-decomp src/smt3/interface/itfMisc.c */
#include "common.h"

#include "interface.h"

#include "libsdf.h"

#include "kernel.h"

/* Font chains and their children have different pool strides. Only the
 * coordinates, color word and links used by this unit are exposed. */
typedef struct frFontChild_t
{
    u8 unk00[0x10];
    union { u32 renderWord; } parentDimensionsOrRenderWord;
    u8 unk14[0x14];
    struct frFontChild_t *next;
} frFontChild_t;

typedef struct frFontRequest_t
{
    u32 unk00;
    s32 x;
    s32 y;
    u8 unk0C[0x10];
    frFontChild_t *firstChild;
    void *lastChild;
    struct frFontRequest_t *prev;
    u8 unk28[0x1C];
} frFontRequest_t;

typedef struct itfLink_t
{
    struct itfLink_t *previous;
    struct itfLink_t *next;
    s32 index;
    void *state;
    sdfMemHandle_t *resourceHandle;
} itfLink_t;

typedef struct
{
    itfLink_t *puse_top, *puse_tail, *pdisuse_top, *pdisuse_tail;
} itfmisclinkstack_t;

typedef struct { s32 x, y, z, w; } itfColorVector_t;

typedef struct { u8 unk00[0x20]; u64 value, registerCode; } itfGsState_t;

extern frFontRequest_t *frTestFontHandle;

extern u64 itfAlphaValueTable[];

extern u8 itfGaugeQuadVertexIndex[8], itfGaugeQuadColorIndex[8], itfGaugeHighlightVertexIndex[], itfGaugeHighlightColorIndex[];

extern itfColorVector_t itfGaugeHighlightVectors[];

extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);

extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern s32 sdf2DGetPrimPacketSize(s32 aNumOfRegs, s32 aNumOfPrim);

extern s32 sdfGrAllocPacketBuff(u32 aSize);

extern void sdf2DMakePrimPacketHeader(void *aPacketPtr, s32 aPrimReg, s32 aNumOfRegs, u64 aGifRegs, s32 aNumOfPrim);

extern saddr_t sdf2DGetPrimPtr(saddr_t aPacketPtr);

extern u32 sdf2DMakeStdPacket(u32 aPacket, s32 aLeng);

extern s32 sdf2DLinkTexPacket(sdfDrawTag_t *, sdfTexHandle_t *, s32);

extern void sdfDrawTagLink(sdfDrawTag_t *, addr_t aPacket);

extern void dds3AdminiExitSequence(void);

#include "sdk_libc.h"

#define ITF_BYTE_MASK 0xFF

#define ITF_VECTOR_WORD_COUNT 4

#define ITF_FIXED_FRACTION_BITS 16

#define ITF_FIXED_COMPONENT_MAX 0xff0000

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

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride);

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack);

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *pelem, itfmisclinkstack_t *pstack);

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const itfColorVector_t *input, s32 count);

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(frFontRequest_t *pfont, u32 color);

/* Clear each child's low byte, then OR unmasked bits into the full color word. */
void itfMiscChangeFontAlpha(frFontRequest_t *pfont, u32 alpha);

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *pfont, u32 ofsx, u32 ofsy);

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *filename);

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(sdfIVector2_t *pvertex, itfColor_t *pcolor, s32 x, s32 y, u32 z, sdfDrawTag_t *pdrawtag);

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command);

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(sdfIVector2_t *pvertex, f32 *pstq, itfColor_t *pcolor, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command);

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag);

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *pdrawtag, u64 para, s32 context);

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context);

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *pdrawtag, s32 context, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v);

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 portion, s32 entire, s32 shadow,
                   sdfIVector2_t *pos, u32 z, itfColor_t *pcolor_portion,
                   itfColor_t *pcolor_entire, sdfDrawTag_t *pdrawtag);

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id);

u32 itfCamp(void);

/* Exit the active sequence unless already in camp, then report the halt. */
void itfCampStartProcess(void);
/* End of declarations from src/smt3/interface/itfMisc.c */

/* Declarations from dds-decomp src/dds1/game/code_00196478.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#define SMT3_DEFINES_frFontChild_t 1

#define SMT3_DEFINES_frFontRequest_t 1

#define SMT3_DEFINES_itfColorVector_t 1

#define SMT3_DEFINES_itfGsState_t 1

#define SMT3_DEFINES_itfLink_t 1

#define SMT3_DEFINES_itfmisclinkstack_t 1

#include "scr.h"

#include "dds/btl.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_state_query.h"

#include "dds/fr_font.h"

#include "dds/fr_font_context.h"

#include "dds/itf.h"

#include "dds/sdf_gs_packet.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdk_libc.h"

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

void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);

/* Replace each child render word along the borrowed glyph chain. */
void frSetFontColor(struct _FRQ *parentNode, u32 colorWord);

/* Update the first-option byte of every child on this glyph chain. */
void frSetFontColorNo(struct _FRQ *glyph, u8 firstOption);

void frSetFontSize(_FRQ *glyph, s32 cellAdvance, s32 cellHeight);

void frSetFontSpace(struct _FRQ *glyph, s32 spacing);

void frSetPriority(struct _FRQ *glyph, u32 unshiftedValue);

extern void *memcpy(void *, const void *, unsigned int);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void *sdfMemLock(struct sdfMemHandle_t *allocation);

u32 strlen(const char *text);

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

extern void frDelFontStyle(s32 slotId);

#define ITF_NODE_HEADER_BYTES 8

#define ITF_ALLOCATION_HANDLE_BYTES 4

#define ITF_RGBA_COMPONENT_COUNT 4

#define ITF_NEUTRAL_COLOR 0x80808080

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

extern _FRQ *frCombineFontRequest(_FRQ *, _FRQ *);

extern _FRQ *frFontLinkGlyph(_FRQ *, _FRQ *, s32);

extern void frSetWhiteSpaceW(s32 kind, u64 flag);

extern void frEnableMode(s32 value);

extern u8 frDisableMode(u8 value);

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

_FRQ *frwGetStaffFontFRQ(u32 x, u32 y, s32 depth, s32 alt, s32 measureFlag, u64 entryFlag, u32 colors, const char *source);

void frwLoadStaffFont(void);

void frwFreeStaffFont(void);

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
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
_FRQ *itfCreateTightFont3TextGlyph(s32 x, s32 y, s32 depth, s32 colors, char *text, _FRQ *previousGlyph);

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u32 x, u32 y, s32 depth, u32 colors,
                                    void *glyphSource, _FRQ *parent);

extern const u8 *MessageNumSearch(s32 textId, const FrFontTextBank *bank, s32 mode);

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

u32 frGetFontHeader(u32 resourceBase);

/* Resolve three signed, cumulative relative offsets without validating bounds. */
void frGetFontImagePtrs(ItfRelativeSegmentResourceView *block, ItfPackedResourceSegments *out);

/* Read the opaque header word at 0x10; its role is not established. */
u32 itfGetPackedResourceHeaderWord(u32 resourceBase);

/* Decode entryIndex's bit span through the resource's six-byte prefix-tree records.
 * Write terminal bytes to decodedBytes and return/cache the count. The font atlas
 * uses these bytes as pixels; neither entry nor output bounds are checked. */
s32 frDecodeFontRow(u8 *decodedBytes, s32 entryIndex, ItfRelativeSegmentResourceView *resourceBase,
                    ItfPackedResourceSegments *segments);

/* Build count usable nodes plus index-zero sentinel, retaining each payload gap.
 * The allocation handle is stored four bytes before the returned ring base. */
MemNode *frAllocFontTable(s32 payloadBytes, s32 count);

/* Remove the next free node, or return NULL at the index-zero sentinel. */
void *frAllocFontNode(MemNode *queue);

/* Prepend a detached payload's header; NULL or an already linked node fails.
 * Header pointer formation before the NULL test is retained unchanged. */
s32 frFreeFontNode(void *payload, MemNode *queue);

/* Release the handle preceding the original ring base, not an acquired payload. */
u32 frFreeFontTable(MemNode *ringBase);

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

#include "dds/sdf_packet_alloc.h"

extern u64 D_00357998[];

/* Draw three adjacent quads using zero-alpha/source-alpha edge color records.
 * Border widths and resulting signed middle width are not clamped. */
void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

extern u8 D_003BB198[8];

extern u8 D_003BB1A0[8];

extern u8 D_003579B0[];

extern u8 D_003579B8[];

extern TextVector D_003579C0[4];

extern char D_003A13F0[];

extern s32 D_00357AB8[];

dds_itfpanel_t *itfCreateUiSpriteByKind(s32 kind, const sdfTexHandle_t *borrowedTexture);
/* End of declarations from src/dds1/game/code_00196478.c */

/* Initialize a FIFO free list and empty active list.
 * Native do/while writes at least two nodes; the s8 index can wrap. */
void itfMiscLinkInitialize(itfmisclinkstack_t *pool, itfLink_t *nodes, s32 count, s32 stride) {
    s8 index = 0;
    itfLink_t *previous = NULL;
    itfLink_t *node = nodes;
    itfLink_t *next;

    do {
        count--;
        next = (itfLink_t *)((u8 *)node + stride);
        node->previous = previous;
        node->index = index;
        index++;
        node->next = next;
        previous = node;
        node = next;
    } while (count >= 2);
    node->previous = previous;
    node->index = index;
    node->next = NULL;
    pool->pdisuse_tail = node;
    pool->pdisuse_top = nodes;
    pool->puse_tail = NULL;
    pool->puse_top = NULL;
}

/* Move the free-list head to the active-list tail; exhausted pools return NULL. */
itfLink_t *itfMiscLinkTake(itfmisclinkstack_t *pstack) {
    itfLink_t *node = pstack->pdisuse_top;
    itfLink_t *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pstack->puse_top != NULL) {
        node->previous = pstack->puse_tail;
        pstack->puse_tail->next = node;
    } else {
        node->previous = NULL;
        pstack->puse_top = node;
    }
    node->next = NULL;
    pstack->puse_tail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pstack->pdisuse_tail = NULL;
    }
    pstack->pdisuse_top = next;
    return node;
}

/* Unlink an active node and append it to the free-list tail; membership is trusted. */
void itfMiscLinkGive(itfLink_t *pelem, itfmisclinkstack_t *pstack) {
    itfLink_t *previous = pelem->previous;
    itfLink_t *next = pelem->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pstack->puse_top = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pstack->puse_tail = previous;
    }
    pelem->next = NULL;
    {
        itfLink_t *lastFree = pstack->pdisuse_tail;
        pelem->previous = lastFree;
        if (lastFree != NULL) {
            lastFree->next = pelem;
        }
    }
    pstack->pdisuse_tail = pelem;
    if (pstack->pdisuse_top == NULL) {
        pstack->pdisuse_top = pelem;
    }
}

/* Multiply XYZ, cap only the positive product at 255 << 16, then shift by 16.
 * Negative products are not clamped; W comes from the argument. */
void itfMiscSetRatioRGB(itfColorVector_t *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const itfColorVector_t *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->w = w;
        if (x > ITF_FIXED_COMPONENT_MAX) x = ITF_FIXED_COMPONENT_MAX;
        if (y > ITF_FIXED_COMPONENT_MAX) y = ITF_FIXED_COMPONENT_MAX;
        if (z > ITF_FIXED_COMPONENT_MAX) z = ITF_FIXED_COMPONENT_MAX;
        output->x = x >> ITF_FIXED_FRACTION_BITS;
        output->y = y >> ITF_FIXED_FRACTION_BITS;
        output->z = z >> ITF_FIXED_FRACTION_BITS;
        output++;
        input++;
        count--;
    }
}

/* Replace each child color; parent color words are not modified. */
void itfMiscChangeFontVertexColor(frFontRequest_t *pfont, u32 color) {
    for (; pfont != NULL; pfont = pfont->prev) {
        frFontChild_t *child;
        for (child = pfont->firstChild; child != NULL; child = child->next) {
            child->parentDimensionsOrRenderWord.renderWord = color;
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMisc", func_00198FD8);

/* Translate parent entries only; child coordinates are left unchanged. */
void itfMiscMoveFontPosition(frFontRequest_t *pfont, u32 ofsx, u32 ofsy) {
    for (; pfont != NULL; pfont = pfont->prev) {
        pfont->x += ofsx;
        pfont->y += ofsy;
    }
}

/* Keep the sprite texture handle while releasing the temporary file allocation. */
sdfTexHandle_t *itfMiscLoadTexture(const char *filename) {
    sdfMemHandle_t *fileAllocation;
    sdfTexHandle_t *textureHandle;
    u32 assetInfo[ITF_VECTOR_WORD_COUNT];

    fileAllocation = sdfDevLoadFileEx(filename, assetInfo, 0);
    textureHandle = sdfTexSet(EE_ADDR_AS(void *, (assetInfo[0])));
    sdfMemFreeDirect(fileAllocation);
    return textureHandle;
}

/* Emit three per-vertex RGBA/XYZ2 records; PRIM's low bits select a triangle.
 * Coordinates receive the native GS screen biases; no clipping is performed. */
void itfMiscDrawTrianglePrim(sdfIVector2_t *pvertex, itfColor_t *pcolor, s32 x, s32 y, u32 z, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_TRIANGLE_VERTEX_COUNT)));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_PRIMITIVE_BITS, 2, 0x51, ITF_TRIANGLE_VERTEX_COUNT);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < ITF_TRIANGLE_VERTEX_COUNT; i++) {
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}

/* Emit an indexed four-vertex triangle fan with a color record per vertex. */
void itfMiscDrawRectanglePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, ITF_QUAD_VERTEX_COUNT)));
    sdf2DMakePrimPacketHeader(packet, ITF_QUAD_FAN_PRIMITIVE_BITS, 2, 0x51, ITF_QUAD_VERTEX_COUNT);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
}

/* Draw three adjacent quads using zero-alpha/source-alpha edge color records.
 * Border widths and resulting signed middle width are not clamped. */
void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command) {
    DrawVertex vertices[ITF_QUAD_VERTEX_COUNT];
    DrawColorRec colors[2];
    s32 middleWidth;

    colors[0].components[0] = color->components[0];
    colors[0].components[1] = color->components[1];
    colors[0].components[2] = color->components[2];
    colors[0].components[3] = 0;
    colors[1].components[0] = color->components[0];
    colors[1].components[1] = color->components[1];
    colors[1].components[2] = color->components[2];
    colors[1].components[3] = color->components[3];

    middleWidth = bounds[1].x - bounds[0].x - borderWidth * 2;
    vertices[0].x = bounds[0].x;
    vertices[0].y = bounds[0].y;
    vertices[1].x = bounds[0].x + borderWidth;
    vertices[1].y = bounds[0].y;
    vertices[2].x = bounds[0].x + borderWidth;
    vertices[2].y = bounds[1].y;
    vertices[3].x = bounds[0].x;
    vertices[3].y = bounds[1].y;

    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988, tail, command);
    vertices[0].x += borderWidth;
    vertices[1].x += middleWidth;
    vertices[2].x += middleWidth;
    vertices[3].x += borderWidth;
    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988 + 4, tail, command);
    vertices[0].x += middleWidth;
    vertices[1].x += borderWidth;
    vertices[2].x += borderWidth;
    vertices[3].x += middleWidth;
    itfMiscDrawRectanglePrim(vertices, colors, D_00357980, D_00357988 + 8, tail, command);
}

/* Emit a textured triangle fan: each four-float input supplies S/T/Q only.
 * The fourth float is skipped, and the context word is shifted without masking. */
void itfMiscDrawTexPoly(sdfIVector2_t *pvertex, f32 *pstq, itfColor_t *pcolor, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, ITF_QUAD_VERTEX_COUNT)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_TEXTURED_FAN_PRIMITIVE_BITS, 3, 0x512, ITF_QUAD_VERTEX_COUNT);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < ITF_QUAD_VERTEX_COUNT; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = pstq[0];
        uvDst[1] = pstq[1];
        uvDst[2] = pstq[2];
        pstq += 4;
        dst += 2;
        dst[0] = (u64)pcolor->components[0] | ((u64)pcolor->components[1] << 32);
        dst[1] = (u64)pcolor->components[2] | ((u64)pcolor->components[3] << 32);
        pcolor++;
        dst += 2;
        dst[1] = (u64)z;
        dst[0] = (u64)(u32)(pvertex->x + ITF_GS_X_BIAS) | ((u64)(pvertex->y + ITF_GS_Y_BIAS) << 32);
        pvertex++;
        dst += 2;
    }
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}

/* Bind the texture and emit one two-corner sprite using two packed UV pairs.
 * Unwritten padding words in the native packet remain untouched. */
void itfMiscDrawSpritePrim(void *vertexData, void *uvData, void *colorData, s32 tail, sdfTexHandle_t *texture, s32 flag, sdfDrawTag_t *command) {
    sdfIVector2_t *vertices = vertexData;
    itfColor_t *uv = uvData;
    itfColor_t *colors = colorData;
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1)));
    sdf2DMakePrimPacketHeader(packet, (flag << 9) | ITF_TEXTURED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = vertices[0].x + ITF_GS_X_BIAS;
    y0 = vertices[0].y + ITF_GS_Y_BIAS;
    x1 = vertices[1].x + ITF_GS_X_BIAS;
    y1 = vertices[1].y + ITF_GS_Y_BIAS;
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    dst[0] = (u64)colors->components[0] | ((u64)colors->components[1] << 32);
    dst[1] = (u64)colors->components[2] | ((u64)colors->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)(u32)tail;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)(u32)tail;
    sdf2DLinkTexPacket(command, texture, flag);
    sdfDrawTagLink(command, EE_ADDR(packet));
}

/* Emit a colored textured sprite without binding a texture in this function. */
void itfMiscDrawSpritePrim2(sdfIVector2_t *pos, itfColor_t *uv, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, 1)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_COLORED_SPRITE_PRIMITIVE_BITS, 5, 0x53531, 1);
    x0 = pos[0].x + ITF_GS_X_BIAS;
    y0 = pos[0].y + ITF_GS_Y_BIAS;
    x1 = pos[1].x + ITF_GS_X_BIAS;
    y1 = pos[1].y + ITF_GS_Y_BIAS;
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst[2] = (u64)uv->components[0] | ((u64)uv->components[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)z;
    dst[6] = (u64)uv->components[2] | ((u64)uv->components[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)z;
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}

/* Emit an untextured two-corner GS sprite. */
void itfMiscDrawSpritePlanePrim(sdfIVector2_t *pos, itfColor_t *color, u32 z, s32 context, sdfDrawTag_t *pdrawtag) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(3, 1)));
    sdf2DMakePrimPacketHeader(packet, (context << 9) | ITF_SPRITE_PRIMITIVE_BITS, 3, 0x551, 1);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
    dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[0].x + ITF_GS_X_BIAS) | ((u64)(pos[0].y + ITF_GS_Y_BIAS) << 32);
    dst += 2;
    dst[1] = (u64)z;
    dst[0] = (u64)(u32)(pos[1].x + ITF_GS_X_BIAS) | ((u64)(pos[1].y + ITF_GS_Y_BIAS) << 32);
    sdfDrawTagLink(pdrawtag, EE_ADDR(packet));
}

/* Emit an indexed triangle strip, packing two vertices per GIF loop.
 * Header loop count truncates odd vertex counts; the native writer does not. */
void itfMiscDrawTriangleStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 pairCount = count >> 1;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(4, pairCount)));
    sdf2DMakePrimPacketHeader(packet, ITF_TRIANGLE_STRIP_PRIMITIVE_BITS, 4, 0x5151, pairCount);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
}

/* Emit indexed RGBA/XYZ2 records with the line-strip primitive. */
void itfMiscDrawLineStripPrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count)));
    sdf2DMakePrimPacketHeader(packet, ITF_LINE_STRIP_PRIMITIVE_BITS, 2, 0x51, count);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
}

/* Emit indexed RGBA/XYZ2 records with independent line primitives. */
void itfMiscDrawLinePrim(sdfIVector2_t *vertices, itfColor_t *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command) {
    sdfGrPacketHeader_t *packet;
    u64 *dst;
    s32 i;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(2, count)));
    sdf2DMakePrimPacketHeader(packet, ITF_LINES_PRIMITIVE_BITS, 2, 0x51, count);
    dst = EE_ADDR_AS(u64 *, sdf2DGetPrimPtr(EE_ADDR_S(packet)));
    for (i = 0; i < count; i++) {
        sdfIVector2_t *vertex = &vertices[*vertexIndex++];
        itfColor_t *color = &colors[*colorIndex++];

        dst[0] = (u64)color->components[0] | ((u64)color->components[1] << 32);
        dst[1] = (u64)color->components[2] | ((u64)color->components[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + ITF_GS_X_BIAS) | ((u64)(vertex->y + ITF_GS_Y_BIAS) << 32);
        dst += 2;
    }
    sdfDrawTagLink(command, EE_ADDR(packet));
}

/* Write TEST for the selected GS context, not the ALPHA blend register. */
void itfMiscDrawTestPrim(sdfDrawTag_t *pdrawtag, u64 para, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = para;
    dst->registerCode = context ? ITF_TEST_SECOND_CONTEXT : ITF_TEST_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Write an unchecked table entry to ALPHA for the selected GS context. */
void itfMiscDrawAlphaPrim(sdfDrawTag_t *pdrawtag, s32 tp, s32 context) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = itfAlphaValueTable[tp];
    dst->registerCode = context ? ITF_ALPHA_SECOND_CONTEXT : ITF_ALPHA_FIRST_CONTEXT;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Write CLAMP for the selected GS context. */
void itfMiscDrawClampPrim(sdfDrawTag_t *pdrawtag, s32 context, s32 wms, s32 wmt, s32 min_u, s32 max_u, s32 min_v, s32 max_v) {
    s32 packet = sdfGrAllocPacketBuff(ITF_STATE_PACKET_BYTES);
    itfGsState_t *dst = EE_ADDR_AS(itfGsState_t *, sdf2DMakeStdPacket(packet, ITF_STATE_PACKET_BYTES));

    dst->value = ((u64)max_v << 34) | ((u64)min_v << 24) | (max_u << 14) | (min_u << 4) | (wmt << 2) | wms;
    dst->registerCode = context ? 9 : 8;
    sdfDrawTagLink(pdrawtag, packet);
}

/* Draw filled and remaining quad regions. The optional highlight is produced
 * as signed fixed-point vectors and passed as their four-word RGBA image. */
void itfMiscDrawGauge(s32 portion, s32 entire, s32 shadow,
                   sdfIVector2_t *pos, u32 z, itfColor_t *pcolor_portion,
                   itfColor_t *pcolor_entire, sdfDrawTag_t *pdrawtag) {
    sdfIVector2_t vertices[6];
    itfColorVector_t highlightColors[4];
    s32 left = pos[0].x;
    s32 right = pos[1].x;
    s32 width = (right - left) >> 4;
    s32 filledWidth;

    if (entire > 0) {
        filledWidth = (((portion << 16) / entire) * width) >> 12;
    } else {
        filledWidth = 0;
    }
    vertices[0].x = left;
    vertices[0].y = pos[0].y;
    vertices[1].x = filledWidth + left;
    vertices[1].y = pos[0].y;
    vertices[2].x = right;
    vertices[2].y = pos[0].y;
    vertices[3].x = left;
    vertices[3].y = pos[1].y;
    vertices[4].x = filledWidth + left;
    vertices[4].y = pos[1].y;
    vertices[5].x = right;
    vertices[5].y = pos[1].y;
    itfMiscDrawRectanglePrim(vertices, pcolor_portion, itfGaugeQuadVertexIndex, itfGaugeQuadColorIndex, z, pdrawtag);
    itfMiscDrawRectanglePrim(vertices, pcolor_entire, itfGaugeQuadVertexIndex + 4, itfGaugeQuadColorIndex, z, pdrawtag);
    if (shadow != 0 && filledWidth >= 3) {
        itfMiscSetRatioRGB(highlightColors, pcolor_portion->components[0],
                        pcolor_portion->components[1], pcolor_portion->components[2],
                        pcolor_portion->components[3], itfGaugeHighlightVectors, 4);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        itfGaugeHighlightVertexIndex, itfGaugeHighlightColorIndex, 3, z, pdrawtag);
        itfMiscDrawLineStripPrim(vertices, (itfColor_t *)highlightColors,
                        itfGaugeHighlightVertexIndex + 3, itfGaugeHighlightColorIndex + 3, 3, z, pdrawtag);
    }
}

/* Retail "DbgDevDump" task callback: perform no dump work and return zero. */
s32 dds3DebugDevDumpProcess(dds3ProcessID_t *proc_id) {
    return 0;
}

u32 itfCamp(void) {
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMisc", func_0019A170);
u8 itfGaugeQuadVertexIndex[8] = { 0, 1, 4, 3, 1, 2, 5, 4 };

u8 itfGaugeQuadColorIndex[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

