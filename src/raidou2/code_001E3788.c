#include "common.h"

/* Declarations from dds-decomp src/dds2/game/code_00306F80.c */
#ifndef VERSION_DDS2
#define VERSION_DDS2 1
#endif

#include "kernel_draw.h"

#include "libsdf.h"

#include "sdk_libc.h"

#include "sdk_libgcc.h"

#include "dds/btl.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_append.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdf_projection.h"

#include "dds/sdk_libc.h"

/* Build a look-at basis from four-float target, eye, and up vectors. */
void LookAtMatrix(const f32 *targetPoint, const f32 *eyePosition, const f32 *upVector);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

extern SdfProjectionRecord dds3ScreenParam;

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

extern void sdf3DMakeScreenParam(SdfCamera *);

extern void sdf3DMakeViewportPacket(struct ConsMatrixPacket *, SdfProjectionRecord *, const Matrix4 *);

void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32));

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                              s32 (*allocatePacket)(s32));

/* Build the serialized VIF attribute stream and return its word cursor. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

void sdfDrawTagLinkRefGif(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfLinkLoadImage(
    sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

void *sdfMemLock(struct sdfMemHandle_t *allocation);

void sdfMemUnlock(struct sdfMemHandle_t *allocation);

char *strcpy(char *dst, const char *src);

u32 strlen(const char *text);

#include "dds/sdk_ps2.h"

#include "dds/sdf_gs_header.h"

#include "dds/sdf_gs_alpha_register_api.h"

#include "dds/sdf_gs_register_submit_api.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_frame_draw_buffer.h"

#include "dds/ui_color.h"

#include "dds/sdf_cons_matrix_packet.h"

#include "dds/sdf_dma_packet_header.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_header.h"

#include "dds/sdf_packet_builders.h"

#include "dds/eff_resource_records.h"

#include "dds/eff_slot_phase.h"

#include "dds/itf.h"

#include "dds/fpu.h"

#include "dds/itf_grid_text.h"

#include "dds/itf_grid_text_color_api.h"

#include "dds/sdf_grid_seek_api.h"

#include "dds/sdf.h"

#include "dds/mnu.h"

#include "dds/itf_draw_grid.h"

#include "dds/eff_resource_slots.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_projection.h"

#include "dds/pcp_vu0.h"

#include "dds/sdf_primitive_packet_api.h"

#include "dds/sdf_packet_append.h"

#include "dds/grid_packed_vertex.h"

#include "dds/itf_grid_status_records.h"

#include "dds/sdf_packet_alloc.h"

extern GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *);

extern GridTextListItem *itfFindGridNodeByKey(u32, GridTextWidget *);

/* Four words filled together; their corner/channel interpretation is unknown. */
typedef struct UiQuadWords {
    u32 unk00[4];
} UiQuadWords; // 0x10

/* Result bit 1: at least widget->rows successors remain from firstVisible. */
enum { GRID_LIST_HAS_COUNT_FOLLOWERS = 2 };

typedef struct GridDrawWork {
    u8 pad00[0xC];
    s16 width;               /* 0x0C */
    s16 height;              /* 0x0E */
    u32 packetHandle;        /* 0x10 */
    u32 overlayHandle;       /* 0x14 */
    u8 overlayEnabled;       /* 0x18 */
    u8 pad19;
    u8 overlayKind;          /* 0x1A */
    u8 pad1B[0x19];
    s32 overlayDataSize;     /* 0x34 */
} GridDrawWork;

extern void *effGetSlotWorkOrOverride(EffectSlotSet *, s32);

extern EffectSlotSet *effUpdateTimedStates(EffectSlotSet *, u32, void *);

extern void effDrawSpriteWithWorkCornerColors(s32, s32, s32, s32, EffectSlotSet *, s32, BdWork *, s32);

extern dds3FrameDrawBuffer_t dds3FrameDrawPacketRecords[];

extern void sdfGrSetFrameMaskRGB(const u8 *, SdfDmaReferenceChainPacket *, s32);

/* Resolve the indexed render entry before applying position, depth, and draw flags. */
void itfDrawGridWithResolvedSlot(u32 offsetX, u32 offsetY, u32 z, u32 drawFlags, EffectSlotSet *resource, u32 resourceIndex, u32 surfaceIndex);

/* Update the indexed slot's description countdown and carry its active state forward. */
void itfUpdateGridSlotDescription(EffectSlotSet *resource, s32 resourceIndex);

s32 itfGridLookupValueOrDefault(EffectSlotSet *resource, s32 resourceIndex);

/* Store grid bounds in the renderer's fixed-point coordinate units. */
void itfSetGridEntryQuantizedAndRefresh(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

/* Copy the quantized bounds into the corresponding render entry as four words. */
void itfGridSetQuantizedBounds(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y,
                   s32 width, s32 height);

/* Set the unquantized bounds of the selected grid widget. */
void itfGridSetBounds(EffectSlotSet *resource, s32 resourceIndex, s32 x, s32 y, s32 width, s32 height);

/* Copy four words from a render entry's +0x84 data to its +0x14 data. */
void itfGridCopyEntryQuad(s32 resource, s32 entryIndex);

/* Bind one real resource-set/index pair. */
void itfGridStorePosition(MenuGridSlot *slot, EffectSlotSet *resource, s32 resourceIndex);

void itfCreateGridPacketWithDefaultFlags(u32 packetHandle, s32 width, s32 height, u8 *data,
                  sdfDrawTag_t *context);

/* The overlay packet is present only when this work flag is set. */
u8 itfGridGetOverlayFlag(GridDrawWork *work);

/* Two overlay kinds use a 16x16 region; other kinds use 8x2. */
void itfDrawGridOverlayPacket(GridDrawWork *work, u8 *data, sdfDrawTag_t *context);

extern s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount);

extern u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet);

/* Build the optional overlay and main packet, then dispatch their draw callback. */
GridDrawWork *itfSubmitGridPacketsAndDraw(GridDrawWork *object, u8 *data, s32 kind);

typedef struct ItfGridResource {
    u32 handle;
    u8 pad04[0x18];
    u32 entryCount;
} ItfGridResource;

/* Return the address of `entryIndex` in the resource's 8-byte-stride offset table, or NULL when out of range. */
u8 *itfGetGridResourceEntryData(ItfGridResource *resource, u32 entryIndex);

/* Set the selected descriptor word's high control bits. */
void itfSetGridDescriptorControlBit(s32 object, s32 index);

EffSlotPhaseStep itfGridApplySqrtBoundsAndColorScale(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Apply the ZOOM_01 easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyQuadraticZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* The FLUSH_01 status payload stores the signed IN, WAIT and OUT durations. */
/* Shrink the grid bounds and apply the three-phase flush envelope to corner alpha. */
EffSlotPhaseStep itfGridApplyTimedBoundsAndAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

EffSlotPhaseStep itfGridShrinkBoundsAndFadeThresholdAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Convert the owner's fixed-point angle to degrees and return its angular step. */
EffSlotPhaseStep itfUpdateAngleAndGetCycleStep(BdWork *unused, BdWork *out, EffTimedState *timedState);

/* Blend each saved corner color toward the table's target color. */
EffSlotPhaseStep itfGridInterpolateCornerColorsToTarget(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Apply linear ZOOM easing to the adjustment bounds and fade their alpha. */
EffSlotPhaseStep itfGridApplyLinearZoomBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

EffSlotPhaseStep itfGridShrinkBoundsAndFadeAlpha(BdWork *rectangle, BdWork *out, EffTimedState *timedState);

/* Unpack engine RGBA order into GS packed R/G and B/A word pairs. */
void cmpExpandColor(u64 *channels, u32 color);

extern sdfDrawTag_t D_0045C380[2];

extern DmaPacketHeader D_0045C3C0[2];

extern sdfDrawTag_t D_0045C400[2];

extern sdf3DLightParallelPacket_t D_0045C440[2];

extern ConsMatrixPacket D_0045C600[2];

extern sdf3DLight_t *D_0037F770[3];

extern f32 D_dds1_00324780[4];

extern Matrix4 sdfViewMatrix;

extern f32 aViewTarget[4];

extern f32 aViewEye[4];

extern f32 aViewDown[4];

extern void sdf3DLinkInitPacket(sdfDrawTag_t *, DmaPacketHeader *);

/* Rebuild the view matrix and both frame banks' matrix and lighting packet lists. */
void itfInitDoubleBufferedScenePackets(void);

void cmpDrawSpriteFlag(u8 enabled, s32 alternate, s32 kind);

void itfSetPrimaryFramebufferAlphaFlag(u8 value, u32 kind);

void cmpDrawSprite(u64 data, s32 alternate, s32 kind);

void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);

void cmpDrawSprite2(s32 data, s32 alternate, s32 kind);

void sdfSubmitGsAlphaOneRegisterPacket(u32 data, u32 kind);

void cmpDrawSpriteIndexed(s32 data, s32 kind);

void cmpDrawSpriteIndexed2(s32 data, s32 kind);

/* Fill all four words with value without assigning a corner or channel order. */
void uiFillQuadColorWords(UiQuadWords *quad, u32 colorWord);

void cmpDrawTriG();

/* Draw a triangle with the same packed color at all three vertices. */
void uiDrawUniformRgbRange(u32 xCoordinates, u32 yCoordinates, u32 z, u32 color, u32 surfaceIndex, u32 extraA, u32 extraB, u32 extraC);

extern void itfDrawVertexColoredRectangle(u32, u32, u32, u32, u32, const u32 *, u32, u32);

/* Draw a rectangle as a four-vertex triangle strip with one packed color. */
void cmpDrawBoxSolid(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 gsContext, u32 surfaceIndex);

void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);

/* Queue a four-vertex GS strip with individual colors and the requested context. */
void itfDrawVertexColoredRectangle(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *colors, u32 gsContext, u32 surfaceIndex);

void uiDrawGradientColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, const u32 *vertexColors, u32 surfaceIndex);

void cmpDrawPanelEffCell(u32 startX, u32 startY, u32 startZ, u32 endX, u32 endY, u32 endZ, u32 color, u32 surfaceIndex);

/* Draw four frame edges; the bottom edge extends 16 units beyond the right side. */
void uiDrawFrameEdges(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 context);

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                   const u32 *colors, u32 surfaceIndex);

void cmpDrawPanelEffCell(u32 startX, u32 startY, u32 startZ, u32 endX, u32 endY, u32 endZ, u32 color, u32 surfaceIndex);

void cmpDrawBoxLine(u32 x0, u32 y0, u32 z0, u32 x1, u32 y1, u32 z1,
                  const u32 *colors, u32 surfaceIndex);

/* vu0 routine: the SDK quadword copies are present at 00308D20..00308D3C. */
void cmpDraw3DPoly(const f32 *first, const f32 *second, const f32 *third,
                   const u32 *colors, s32 unused, sdfMat_t *asset, s32 surfaceIndex);

void uiDrawActiveSurfaceRegion(s32 surfaceIndex);

extern void sdfGrSetFrameMaskReset(const u8 *, SdfDmaReferenceChainPacket *, s32);

void sdfDispatchSurfaceWithPreparedTexturePacket();

void uiDrawTexturedSurfaceAtFarDepth(u32 context);

/* Prepare the surface and choose GS ALPHA 0x54 for mode zero, 0x58 otherwise. */
void itfConfigureSurfaceAlphaBlendMode(s32 mode, s32 surface);

void uiDrawActiveSurfaceWithTestMode(u32 context);

void uiConfigureSurfaceAlphaState(s32 context);

void uiDrawSurfaceAtNearDepth(u32 context);

/* Scale packed RGB by a Q8 factor, preserving the low alpha byte. */
u32 uiScaleColorRgb(u32 color, u32 scaleFactor);

u32 fclBlendColor(u32 firstColor, u32 secondColor, u32 weight);

void itfInitializeAdminInterfaceNoop(void);

void itfShutdownAdminInterfaceNoop(void);

/* Separate native copy of itfReturnZero; no more specific caller role is established. */
u32 itfReturnZeroDuplicate(void);

s32 itfActivateGridTextWidget(GridTextWidget *widget);

/* Apply parent flags and enable bit 1 on the linked child widget, if present. */
s32 itfSetWidgetFlagsAndActivateChild(GridTextWidget *widget, u32 flags);

/* Own a NUL-terminated text copy; store X scaled by 16 and Y scaled by 8. */
GridTextWidget *itfCreateGridTextWidget(const char *text, s32 x, s32 y, s32 columns, s32 rows,
                                        u32 reference);

/* A zero column or row count leaves that dimension unchanged. */
void itfSetGridDimensions(GridTextWidget *widget, s32 columns, s32 rows);

u32 itfDestroyGridTextWidget(GridTextWidget *widget);

/* Destroy linked child widgets recursively before releasing the parent widget. */
u32 itfDestroyGridTextWidgetTree(GridTextWidget *widget);

/* Grow only; numbered prefixes reserve four columns, plus two for hexadecimal. */
void itfExpandWidgetColumnWidth(s32 columns, GridTextWidget *widget);

/* Bit 0: the visible node has a predecessor. Bit 1: at least rows successors remain. */
u32 itfGetGridListLinkFlags(GridTextWidget *owner);

GridTextListItem *itfAppendGridTextItem(GridTextWidget *owner, const char *text, u32 value);

GridTextListItem *itfRemoveSelectedGridTextItem(GridTextWidget *widget);

/* Replace the owned text and grow the optional parent column to fit its byte length. */
void itfReplaceGridTextAndExpandColumn(GridTextWidget *widget, GridTextListItem *item, const char *text);

s32 itfSetGridNumericItemDescriptor(GridTextWidget *widget, GridTextListItem *item,
                  GridNumericDescriptor *descriptor);

/* Advance by at least one configured step; crossing the maximum wraps to minimum. */
void itfAdvanceGridScrollPosition(GridTextWidget *owner, u32 key, s32 steps);

void itfAdvanceSelectedGridScroll(GridTextWidget *owner, u32 steps);

/* Reverse by at least one configured step; crossing the minimum wraps to maximum. */
void itfReverseGridScrollPosition(GridTextWidget *owner, u32 key, s32 steps);

void itfReverseSelectedGridScroll(GridTextWidget *owner, u32 steps);

ItfGridTextColorTableIndex itfGetGridTextColorTableIndex(const GridTextWidget *widget,
                                                          const GridTextListItem *target);

/* Format a native value row, then optionally prefix its decimal/hex row number. */
void itfFormatGridValueEntryText(GridTextWidget *widget, GridTextListItem *entry, char *out);

/* Draw visible local-map rows and invoke the selected row callback once. */
void itfDrawGridTextRows(s32 offsetX, s32 offsetY, s32 z, GridTextWidget *widget,
                   s32 surfaceIndex);

GridTextListItem *itfFindGridNodeByKey(u32 key, GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekSelectedNodeByIndex(s32 itemsToAdvance, GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekFirstNode(GridTextWidget *widget);

SdfGridSeekResult sdfGridSeekLastNode(GridTextWidget *widget);
/* End of declarations from src/dds2/game/code_00306F80.c */

void itfCreateGridPacketWithDefaultFlags(u32 packetHandle, s32 width, s32 height, u8 *data,
                  sdfDrawTag_t *context) {
    sdfLinkLoadImage(context, (sdfGsMemSlice_t *)packetHandle,
                              0, 0, width, height, (s32)data, 0);
}
