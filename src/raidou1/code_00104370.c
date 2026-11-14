#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00102ED8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "common.h"

#include "libsdf.h"

#include "sdfDevCons.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade_color.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/dds3_task_update_count.h"

#include "dds/eff_random.h"

#include "dds/eff_random_mod.h"

#include "dds/evt_frame_state_packet.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_gs_scene_state.h"

#include "dds/sdf_linked_packet.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_pad_motor_api.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_scene_packet.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_texture_clamp.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_lists.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libm.h"

f32 acosf(f32 x);

/* Test membership in the starting, running, and destruction-pending queues. */
dds3ProcessRegistrationResult_t dds3ExistProcess(const struct dds3ProcessID_t *task);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

/* The API returns this count; it resets on creation/activation and increments
 * after callback handling, except when an active callback finishes immediately. */
dds3TaskUpdateCount_t dds3GetProcessRunTime(const dds3ProcessID_t *task);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

dds3TaskKillRequestResult_t dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);

extern SdfProjectionRecord dds3ScreenParam;

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* The three frame states' packets, one per draw buffer. */
extern EvtFrameStatePacket dds3ZClearDrawTag[3][2];

extern dds3FadeColor_t fadergba;

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context);

extern void sdf3DMakeScreenParam(SdfCamera *);

extern void sdf3DMakeViewportPacket(struct ConsMatrixPacket *, SdfProjectionRecord *, const Matrix4 *);

void *sdfChipAllocDirect(s32 size);

void sdfDrawTagInit(struct sdfDrawTag_t *list);

void sdfDrawTagLink(struct sdfDrawTag_t *list, u32 packetAddress);

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfGrAllocPacketBuff(s32 size);

void sdfGrLinkStdDrawEnv(struct sdfDrawTag_t *dmaList,
                                  sdfPreDrawTag_t *linkedList,
                                  sdfPreDrawHook_t *linkedNode);

void sdfGrSetClearPacket(SdfGsCenteredBoundsRegisters *packet,
    s32 width, s32 height, s32 unused0, s32 unused1);

void sdfGrSetDrawEnv(SdfSceneDrawPacket *packet,
    struct sdfGrFrameSet_t *view, s32 bufferIndex);

void sdfGrSetStdDrawEnv(SdfSceneNode *node, sdfGrFrameSet_t *view);

/* Return borrowed endpoints; callers may inspect list nodes but do not own them. */
sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

void sdfPadSetAct(SdfPadIndex padIndex, s32 smallMotor, s32 largeMotor);

/* These APIs index the two-entry logical-pad table without bounds checks. */
void sdfPadSetAct0(SdfPadIndex padIndex, u16 strength);

void sdfPadSetAct1(SdfPadIndex padIndex, u8 strength);

void sdfPreDrawTagInit(sdfPreDrawTag_t *list);

void sdfPrimLinkLine(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkRect(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

void sdfPrimLinkSprite(
    sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
    s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth,
    s32 (*allocatePacket)(s32));

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);

/* The active resource registry contains sdfTexHandle_t nodes; lookup compares the
 * texture's resource key and follows its previous-node link. A match is borrowed:
 * lookup does not retain it, and callers must not release it as an acquired ref. */
extern struct sdfTexHandle_t *sdfTexHandleLink;

/* Store the context-one resource packet on the texture's primary buffer. */
void sdfTexMakeGIFPacket(struct sdfTexHandle_t *texture);

void sdfTexSetWrapMode(struct sdfTexHandle_t *texture, SdfTextureClampMode mode);

#include "dds/sdk_ps2.h"

#include "dds/sdf_scene_packet.h"

#include "dds/sdf_gs_scene_state.h"

#include "dds/common.h"

#include "dds/file_config_option_api.h"

#include "dds/sdf_vu0.h"

#include "dds/dds3_background_fade_api.h"

#include "dds/sdf_pad_motor_api.h"

#include "dds/dds3Admin.h"

#include "dds/kernel.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/eff_random_mod.h"

#include "dds/eff_random.h"

#include "dds/sdf_cons_matrix_packet.h"

#include "dds/sdf_dma_packet_header.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_tex_packet.h"

#include "dds/kwln.h"

#include "dds/dds3_task_update_count.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_named_slot.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_runner.h"

#include "dds/dds3_fade.h"

#include "dds/dds3_fade_timing.h"

#include "dds/sdf.h"

#include "dds/sdf_texture_lists.h"

#include "dds/sdf_texture_registry.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_linked_packet.h"

#include "dds/sdf_draw.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_sif_command.h"

#include "dds/pcp_vu0.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/sdf_dma_tag.h"

#include "dds/sdf_gs_packet.h"

#include "dds/btl.h"

#include "dds/dds3_fade_color.h"

#include "dds/evt_frame_state_packet.h"

/* Declared before first use (were implicit int). */
extern void eglSetOt(u32 surfaceIndex);

extern void evtSubmitDefaultDepthGradientRect(s32 x, s32 y, s32 width, s32 height, s32 topLeftColor, s32 topRightColor, s32 bottomRightColor, s32 bottomLeftColor);

extern void evtSubmitPrimaryAlphaBlendMode(s32 blendMode);

extern void evtSubmitPrimaryGsTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 unusedZte, s32 ztst);

extern u32 dds3DrawFlag;

extern u16 D_003BA918;

extern s16 D_003BA91A;

extern u16 D_003BA91C;

extern u16 D_003BA91E;

extern u32 dds3RenderTex;

extern sdfGsMemSlice_t *D_003BA8F4;

extern sdfTexHandle_t *dds3RenderTexture;

extern u32 dds3TextureViewerPageIndex;

extern u8 D_003BA890;

extern s8 D_003BA7FC;

extern s8 dds3BackgroundFadeMode;

extern s16 dds3BackgroundFadeFirstRamp;

extern s16 dds3BackgroundFadeSecondRamp;

extern u8 dds3BackgroundFadeColor[4];

extern s8 D_r1_003AD3BB[];

extern f32 D_003245EC[];

extern sdfGrOt_t sdfDebugGrOt;

extern u8 dds3PadVibrationState[2];

extern s32 D_003BD6A0[2];

extern u8 kwlnLargeMotorTarget;

extern u16 fadenow;

extern u16 fademax;

extern u16 dds3BackgroundFadeCounter;

extern u16 dds3BackgroundFadeDuration;

extern sdfTexHandle_t *dds3CurrentIncompleteResource;

extern EffRandState effSharedRandomState[];

extern void sdfRandomize(void *data, u32 tag);

extern s32 dds3TextureViewerHandlePad(void);

extern void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);

extern void dds3TextureAttachTask(sdfGrOt_t *surface);

extern void dds3InitializeFrameDrawPackets(s32 bufferIndex);

extern void dds3InitZClearDrawTag(void);

extern void eglInit(void);

extern void sdfGrSetScreen(s32 displayModeIndex);

#include "dds/sdf_packet_alloc.h"

extern void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);

extern void dds3PadVibrationSet(u32, u8, s32);

extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

extern dds3ProcessID_t *dds3DebugGraphTask;

extern s32 dds3TextureCountIncompleteResources(void);

extern s32 D_003BA724;

extern s32 distblurovercnt;

extern s32 ripblurovercnt;

extern s8 dds3PadBitTable[32];

extern s32 dds3SelectIncompleteResourceAtPageIndex(s32);

extern sdfGrOt_t D_00325708;

extern char dds3AdminTaskName[];

extern dds3TaskUpdateResult_t dds3AdminiNeoProcess(dds3ProcessID_t *task);

extern void dds3AdminiNeoShutdown(dds3ProcessID_t *task);

void dds3InitAdmini(void);

extern fclNamedSlot_t D_003C1C90[];

extern char D_0039E018[];

extern s32 D_003BA850;

extern s32 D_003BA854;

extern s32 D_003BA858;

extern s32 D_003BA85C;

s32 fclEncycFreeEntry(const char *name);

s32 fclEncycFindFreeSlot(const char *name);

/* One shared initializer for the three configure modes and unregister. */
static inline void dds3SetNamedSlotState(s32 index, s32 kind, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy,
                                         void (*setState)(s32), void *userValue) {
    D_003C1C90[index].kind = kind;
    D_003C1C90[index].task = NULL;
    D_003C1C90[index].update = update;
    D_003C1C90[index].destroy = destroy;
    D_003C1C90[index].setState = setState;
    D_003C1C90[index].enabled = 0;
    D_003C1C90[index].userValue = userValue;
}

#define DDS3_PAD_INPUTS_PER_SET 16

#define DDS3_PAD_REPEAT_BIT 2

#define DDS3_ALT_CONFIRM_INPUT 0x11

#define DDS3_ALT_CANCEL_INPUT 0x13

#define DDS3_PAD_SMALL_MOTOR 0

#define DDS3_PAD_LARGE_MOTOR 1

#define DDS3_PAD_MOTOR_COUNT 2

#define DDS3_DEBUG_RUMBLE_PERIOD 0x4650

#define DDS3_DEBUG_SMALL_PULSE_FRAMES 0xF

#define DDS3_DEBUG_LARGE_PULSE_FRAMES 30

#define DDS3_DEBUG_RUMBLE_VARIATION 150

#define DDS3_DEBUG_RUMBLE_BASE 100

#define DDS3_SHARED_RANDOM_SEED 0x12345678

#define DDS3_CELL_COLUMN_SPAN 0xC0

#define DDS3_CELL_ROW_SPAN 0x60

#define DDS3_PREVIEW_MAX_TEXELS 0x100

#define DDS3_PREVIEW_WRAP_TEXELS 0x400

#define DDS3_DIAG_DEPTH 0x0FFFFF80

/* Find an exact NUL-terminated name along the linked slot indices, or return -1. */
s32 fclEncycFreeEntry(const char *name);

/* Unregister a named slot: stop its task, clear it and unlink it from the slot chain. */
s32 fclEncycFreeEntries(const char *name);

s32 kwlnConfigureNamedSlotMode3(const char *name, void (*setState)(s32));

s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

s32 kwlnConfigureNamedSlotMode2(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

void dds3InitDebugSystem(void);

extern s8 dds3PadButtonState[64];

/* Fresh presses wrap; repeat-bit input stops at the implemented end bounds.
 * Without scrolling, forward motion is bounded by visibleRows, not itemCount. */
void SelectSub(s32 *scrollOffset, s32 *cursorRow, s32 itemCount, s32 visibleRows, s32 padSet, s32 forwardInput, s32 backwardInput);

/* Step the primary then secondary cursor pair; return 1/-1 for confirm/cancel.
 * Any nonzero padSet tests set 1's confirm/cancel, while movement uses padSet. */
s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible, s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll, s32 *secondaryCursor, s32 *primaryCursor);

/* Append a cell rectangle; rowSpan also supplies the horizontal padding. */
void dds3ConsPanelTagLink(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount);

/* Append the same cell rectangle with caller-selected depth. */
void dds3ConsPanelTagLinkZ(sdfDrawTag_t *packetList, s32 column, s32 row, s32 columnCount, s32 rowCount, s32 depth);

/* Periodically pulses both pad motors, varying the large-motor level. */
s32 dds3PadVibrationTest(dds3ProcessID_t *task);

extern u32 dds3PadButtonHoldCounters[4][16];

extern u32 dds3PadStickHoldCounters[2][2];

extern s8 dds3PadButtonPrev[2][2][16];

extern u8 D_003BA860[2][2][2];

extern s8 D_003BA868[2][2];

extern u8 D_003BA870[2][2][2];

extern u8 D_003BA878[2][2];

extern u8 D_003BD698[2];

void dds3PadInitialize(void);

extern u8 sdfPadAnalogSticks[8];

extern void dds3UpdateVibrationSlots(void);

void kwlnUpdatePadInputState(void);

void kwlnPadStepLargeMotorLevel(void);

/* Honor the saved rumble option, normalize small-motor strength and clamp
 * motor indices to the large motor. Preserve target update before output. */
void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void kwlnPadResetMotorLevelsAndOutput(void);

/* Tick both rumble countdowns; stop expired motors and refresh the active large motor. */
void dds3UpdateVibrationSlots(void);

s32 dds3TaskNoopCallback(dds3ProcessID_t *task);

/* Seed the shared effect random state with the fixed initialization seed. */
void dds3InitRandCB(void);

extern s16 D_r1_0046B128;

extern u16 sdfDrawFrameCounter;

extern u16 D_003BA880;

extern u16 D_003BA882;

extern u16 D_003BA884;

extern u16 D_003BA886;

extern char D_003BA888[];

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled);

/* Scale the longer preview side to at most 256 texels, preserving division
 * before coordinate scaling; UV endpoints at 1024 texels are reduced by one. */
void dds3DrawImageOutline(sdfDrawTag_t *packetList, sdfTexHandle_t *image);

extern u32 dds3TextureGetPageIndex(void);

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList);

extern const char *D_r1_003AD3E0[];

extern char D_0039E170[];

extern char D_0039E188[];

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture);

#define DDS3_DIAG_PACKET_LIST_BYTES 0x20

#define DDS3_VIEWER_FAST_PAGE_STEP 10

#define DDS3_MAP_ROW_UNITS 0x1000

#define DDS3_MAP_ROW_SHIFT 12

#define DDS3_MAP_ROW_MASK 0xFFF

#define DDS3_MAP_COLUMN_SHIFT 5

#define DDS3_MAP_COLUMN_UNITS 32

#define DDS3_MAP_COLUMN_COUNT 128

#define DDS3_MAP_ROW_SPAN 0x800

#define DDS3_MAP_BORDER_WIDTH 0x840

#define DDS3_MAP_CELL_X_STEP 16

#define DDS3_MAP_ROW_Y_STEP 8

#define DDS3_MAP_CELL_X_SHIFT 4

#define DDS3_MAP_GREEN 0x80008000

#define DDS3_MAP_YELLOW 0x80008080

#define DDS3_MAP_RED 0x80000080

#define DDS3_MAP_BLUE 0x80800000

#define DDS3_MAP_CYAN 0x80808000

#define DDS3_HELD_TEXTURE_CLAMP_MODE 5

/* Count texture entries without a reference clone source. */
s32 dds3TextureCountIncompleteResources(void);

u32 dds3TextureGetPageIndex(void);

/* Clamp a negative request and return the reached index. DDS1 index zero
 * selects the head without scanning clone-source links; tail exits retain prior writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex);

/* Exit input returns 0 immediately; otherwise apply one prioritized page step,
 * select its reached index, then optionally toggle the viewer mode and return 1. */
s32 dds3TextureViewerHandlePad(void);

extern s32 D_003BA8E0;

void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image);

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface);

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void);

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode);

/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y);

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface);

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void);

#include "dds/dds3_frame_draw_bank.h"

extern dds3FrameDrawBank_t dds3DrawEnv[2];

extern u32 D_003BA8EC;

extern sdfGrFrameSet_t sdfGrFrameSet;

extern sdf3DLight_t *D_00324770[3];

extern f32 kwlnDefaultColorVector[4];

extern Matrix4 sdfViewMatrix;

extern Matrix4 D_00329790;

extern sdf3DLight_t *kwlnOverlayLightSources[3];

extern f32 kwlnOverlayAmbientColor[4];

extern void sdf3DLinkInitPacket(sdfDrawTag_t *, DmaPacketHeader *);

/* Initialize the selected frame bank's scene and overlay packet chains. */
void dds3InitializeFrameDrawPackets(s32 bufferIndex);

/* Select display mode 1, rebuild both projection blocks and reset draw-vector state. */
void dds3InitDrawEnv(void);

#include "dds/dds3_texture_bank.h"

extern sdfGrFrameSet_t dds3RenderTexId;

extern u16 D_003BA8FC;

extern u16 D_003BA8FE;

extern u32 dds3RenderDrawEnv;

extern sdf3DLight_t *dds3RenderLightParallel[3];

extern f32 dds3RenderAmbientColor[4];

extern Matrix4 dds3RenderViewportMatrix;

/* Rebuild the held-texture render target's view and both frame banks. */
void dds3InitRenderDrawEnv(void);

extern u16 D_003BA8FC;

extern u16 D_003BA8FE;

extern void dds3FreeRenderTexture(void);

extern void dds3InitRenderDrawEnv(void);

extern sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format);

extern sdfTexHandle_t *sdfTexNew(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary);

/* Replace the held texture with a width x height image buffer; return 1 on success.
 * A texture-creation failure does not roll back the already allocated image buffer. */
s32 dds3AllocRenderTexture(u16 width, u16 height, f32 aspect);

/* Release the held texture if present and clear tracking; no separate buffer free. */
void dds3FreeRenderTexture(void);

#include "dds/dds3_texture_reference_flag_api.h"

s32 dds3RenderTextureON(void);

void dds3RenderTextureOFF(void);

u32 dds3CheckRenderTexture(void);

sdfTexHandle_t *dds3GetRenderTexture(void);

extern f32 sdfViewShakeVector[];

extern f32 D_003BA940[];

extern f32 fabsf(f32 value);

/* Advance the two bounded shake offsets and taper finite-duration amplitudes. */
void kwlnAdvanceShakeOffsets(void);

#define DDS3_FADE_TICKS_PER_FRAME 10

#define DDS3_FADE_MARK_NUMERATOR 4

#define DDS3_FADE_COUNT_DOWN_BIT 1

#define DDS3_FADE_COUNT_UP_BIT 2

#define DDS3_FADE_DIRECTION_BITS 3

#define DDS3_FADE_MAX_ALPHA 0x80

#define DDS3_FADE_ALPHA_SHIFT 7

#define DDS3_COLOR_ALPHA_CHANNEL 3

#define DDS3_BGFADE_COUNT_DOWN_BIT 0x04000000

#define DDS3_BGFADE_COUNT_UP_BIT 0x08000000

#define DDS3_BGFADE_DIRECTION_BITS 0x0C000000

#define DDS3_BGFADE_KEEP_OTHER_BITS 0xF3FFFFFF

#define DDS3_BGFADE_CLEAR_UP_MASK 0xF7FFFFFF

#define DDS3_BGFADE_CLEAR_DOWN_MASK 0xFBFFFFFF

#define DDS3_BGFADE_FIRST_RAMP_MAX 0x31

#define DDS3_BGFADE_SECOND_RAMP_MAX 0x4F

#define DDS3_BGFADE_FIRST_RAMP_SCALE 49.0f

#define DDS3_BGFADE_SECOND_RAMP_SCALE 79.0f

/* Store tenths in the 16-bit duration before deriving its forty-percent mark.
 * Zero frames only disables the configuration; mode zero stores the -1 sentinel. */
void dds3SetQuake(s32 fadeMode, s32 frames);

/* Disable the configured fade without clearing its stored timing parameters. */
void dds3StopQuake(void);

/* Fill each frame state's z-clear packet for both draw buffers. */
void dds3InitZClearDrawTag(void);

/* Append the selected state's packet for the current draw buffer to a new list. */
sdfDrawTag_t *dds3LinkZClearDrawTag(s32 stateIndex);

/* Cancel the foreground direction and zero RGBA; timing words are unchanged. */
void dds3FadeClear(void);

/* Store RGBA and cancel foreground direction flags without changing timing. */
void dds3FadeSet(s32 red, s32 green, s32 blue, s32 alpha);

/* Return the live color address through outColor, rather than copying channels. */
void dds3FadeColorGet(dds3FadeColor_t **outColor);

/* Replace RGB only; alpha, direction flags and timing are preserved. */
void dds3FadeColorSet(s8 red, s8 green, s8 blue);

/* Start the countdown with caller RGB. A zero duration also clears RGB via FadeClear.
 * The signed alpha byte -0x80 carries the raw maximum-alpha value 0x80. */
void dds3FadeIn(s32 red, s32 green, s32 blue, s32 duration);

/* Start the same countdown using existing RGB; zero duration preserves RGB. */
void dds3FadeIn2(s32 duration);

/* Start the count-up with caller RGB; zero duration leaves maximum alpha. */
void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

/* Start the same count-up using existing RGB; zero duration preserves RGB. */
void dds3FadeOut2(s32 duration);

/* Return whether either foreground counter direction is enabled. */
s32 dds3FadeCheck(void);

/* Step the u16 counter and sample alpha before testing exact completion.
 * Completion clears timing/flags but leaves the sampled endpoint alpha intact. */
void dds3FadeUpdate(void);

/* Submit diagnostics when enabled and either counter is nonzero; only positive
 * counters produce text, so negative-only errors still submit an empty list. */
void dds3DrawDistRipOverError(void);

/* Cancel background direction, zero RGBA and restore the inactive ramp maxima.
 * This reset does not rewrite the background counter or duration. */
void dds3FadeResetBackground(void);

/* Start the background countdown; zero duration performs the full background reset. */
void dds3FadeBackgroundStartOut(s32 duration);

/* Start the background count-up; zero duration leaves max alpha and zero ramps. */
void dds3FadeBackgroundStartIn(s32 duration);

/* Direction flags take precedence; idle visibility uses alpha in mode zero,
 * otherwise it tests whether the first ramp differs from its inactive maximum. */
dds3BackgroundOverlayActivity_t dds3FadeIsBackgroundOverlayActive(void);

/* Store the visibility mode; its stored zero value clears ramps, otherwise
 * maximum alpha is selected. Counter direction and timing are unchanged. */
void dds3FadeSetMode(s32 visibilityMode);

/* Step the u16 counter, then derive integer alpha and f32 ramp samples.
 * Completion clears timing/flags after sampling, retaining endpoint values. */
void dds3StepBackgroundFade(void);

void dds3DrawBackgroundFadeBands(void);

void dds3SetScreenFovy(f32 fieldOfView);
/* End of declarations from src/dds1/game/code_00102ED8.c */

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfPrimTransPacket_t *blendPacket;
    s32 originX = 0x7100;
    /* Keep the four real conversion epochs in one local; see docs/idioms.md,
     * "Trace-justified pixel conversion reuse: dds3DebugGraphUpdate". */
    s32 pixels;

    sdfDrawTagInit(packetList);
    blendPacket = (sdfPrimTransPacket_t *)sdfGrAllocPacketBuff(0x40);
    sdfPrimSetOpaquePacket(blendPacket);
    sdfDrawTagLink(packetList, (u32)blendPacket);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x70D0, 0x85A8,
                    0xFFFF7F, 0x1260, 0xF0, 0x20000000, 0x40806040));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x85C0, 0xFFFF80, 0, D_003BA888, (u16)D_r1_0046B128));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(originX,
                    0x8620, 0xFFFF80, 0, D_003BA888, sdfDrawFrameCounter));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x85C0, 0xFFFF80, 6, D_003BA888, D_003BA880));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x8080,
                    0x8620, 0xFFFF80, 6, D_003BA888, D_003BA882));
    pixels = (s32)((f32)(u16)D_r1_0046B128 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000C000, 0, 0x73A0, 0x85F0,
                    originX + ((pixels + 0x2A) << 4),
                    0x8600, 0xFF0000, NULL);
    pixels = (s32)((f32)sdfDrawFrameCounter / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x80000080, 0, 0x73A0, 0x8650,
                    originX + ((pixels + 0x2A) << 4),
                    0x8660, 0xFF0000, NULL);
    pixels = (s32)((f32)D_003BA880 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x85D0,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8610, 0xFF0000, NULL);
    pixels = (s32)((f32)D_003BA882 / 224.0f * 200.0f);
    sdfPrimLinkRect(packetList, 0x8000FFFF, 0,
                    originX + ((pixels + 0x2B) << 4),
                    0x8630,
                    originX + 0x20 + ((pixels + 0x2B) << 4),
                    0x8670, 0xFF0000, NULL);
    sdfDebugGrOt.regFunc(&sdfDebugGrOt, packetList);
    if (D_003BA884 >= 0x47 || D_003BA880 < (u16)D_r1_0046B128) {
        D_003BA880 = D_r1_0046B128;
        D_003BA884 = 0;
    } else {
        D_003BA884++;
    }
    if (D_003BA886 >= 0x47 || D_003BA882 < sdfDrawFrameCounter) {
        D_003BA882 = sdfDrawFrameCounter;
        D_003BA886 = 0;
    } else {
        D_003BA886++;
    }
    return 0;
}

void dds3DebugGraphTaskDestroy(void) {
}

/* Only mode 1 creates and mode 0 destroys the timing graph; other modes do nothing. */
void dds3DebugGraphSetEnabled(s8 enabled) {
    if (enabled == 1) {
        dds3DebugGraphTask = dds3StartProcess("DebugTimeGrph", 0x2710, 1, 1, dds3DebugGraphUpdate, dds3DebugGraphTaskDestroy, NULL);
    } else if (enabled == 0) {
        dds3KillProcess(dds3DebugGraphTask, 0);
    }
}

INCLUDE_ASM(const s32, "code_00104370", func_r1_00104758);

/* Append the zero-based viewer page and final page index; an empty list prints -1. */
void dds3TextureDrawPageCounter(sdfDrawTag_t *packetList) {
    char pageText[0x70];
    s32 pageIndex = dds3TextureGetPageIndex();
    s32 resourceCount = dds3TextureCountIncompleteResources();
    sprintf(pageText, "TEX VIEWER [%d/%d]", pageIndex, resourceCount - 1);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x79C0, DDS3_DIAG_DEPTH, 0, pageText));
}

INCLUDE_RODATA(const s32, "code_00104370", D_r1_004448F8);

void dds3DrawTextureResourceDetails(sdfDrawTag_t *packetList, sdfTexHandle_t *texture) {
    char text[0x70];
    s32 formatIndex = 0;
    s32 formatMode = 0;

    sprintf(text, "W:%4d H:%4d", texture->width, texture->height);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x82A0, 0x0FFFFF80, 0, text));
    switch (texture->pixelFormat) {
    case 0: formatIndex = 0; formatMode = 6; break;
    case 1: formatIndex = 1; formatMode = 6; break;
    case 2: formatIndex = 2; formatMode = 6; break;
    case 10: formatIndex = 3; formatMode = 6; break;
    case 19: formatIndex = 4; formatMode = 4; break;
    case 20: formatIndex = 5; formatMode = 5; break;
    case 27: formatIndex = 6; formatMode = 4; break;
    case 36: formatIndex = 7; formatMode = 5; break;
    case 44: formatIndex = 8; formatMode = 5; break;
    default: break;
    }
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x8300, 0x0FFFFF80,
                                                                formatMode, D_r1_003AD3E0[formatIndex]));
    sprintf(text, "ID[%2d,%2d]", texture->texId, texture->clutId);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x77C0, 0x8300, 0x0FFFFF80, 0, text));
}

/* Count texture entries that are not duplicates of another texture. */
s32 dds3TextureCountIncompleteResources(void) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *nextTexture;
    s32 incompleteCount = 0;
    while (texture != NULL) {
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            nextTexture = texture->next;
            if (nextTexture == NULL) {
                return incompleteCount;
            }
            texture = nextTexture;
        }
        incompleteCount++;
        texture = texture->next;
    }
    return incompleteCount;
}

u32 dds3TextureGetPageIndex(void) {
    return dds3TextureViewerPageIndex;
}

/* Clamp a negative request and return the reached index; index zero selects the head
 * without scanning duplicates, and tail exits keep the prior selection writes. */
s32 dds3SelectIncompleteResourceAtPageIndex(s32 requestedIndex) {
    sdfTexHandle_t *texture = sdfTexHandleLink;
    sdfTexHandle_t *nextTexture;
    s32 visitedIndex = 0;

    if (requestedIndex < 0) {
        requestedIndex = 0;
    }
    while (texture != NULL && visitedIndex != requestedIndex) {
        dds3CurrentIncompleteResource = texture;
        while (texture->ref != NULL && texture->ref->origin != NULL) {
            nextTexture = texture->next;
            if (nextTexture == NULL) {
                return visitedIndex;
            }
            texture = nextTexture;
        }
        nextTexture = texture->next;
        if (nextTexture == NULL) {
            return visitedIndex;
        }
        visitedIndex++;
        texture = nextTexture;
    }
    dds3CurrentIncompleteResource = texture;
    return visitedIndex;
}

INCLUDE_ASM(const s32, "code_00104370", func_r1_00104B80);

void dds3DrawTextureViewerFrame(sdfGrOt_t *surface, sdfTexHandle_t *image) {
    sdfDrawTag_t *packetList;
    s32 counter;
    s32 mode;

    if (image == NULL) {
        return;
    }
    packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x79B0, 0x0FFFFF7F, 0x1040, 0x80, 0x80000000, 0x80806020));
    dds3TextureDrawPageCounter(packetList);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x7A50, 0x0FFFFF7F, 0x1040, 0x820, 0x80000000, 0x80806020));
    dds3DrawImageOutline(packetList, image);
    sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(0x7160, 0x8290, 0x0FFFFF7F, 0x1040, 0xE0, 0x80000000, 0x80806020));
    dds3DrawTextureResourceDetails(packetList, image);

    if (D_003BA890 != 0) {
        dds3DrawTextureListDiagnostic(packetList, 0x8240, 0x7A60);
        counter = D_003BA8E0;
        mode = (counter >> 4) & 1;
        if (image->pixelGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->pixelGsHandle, mode);
        }
        counter = D_003BA8E0;
        /* Boolean highlight selection; see docs/idioms.md, "Trace-justified boolean extraction: texture viewer highlight". */
        mode = (counter & 0x10) ? 2 : 0;
        if (image->clutGsHandle != NULL) {
            dds3DrawTextureAllocationMapBlock(packetList, 0x8240, 0x7A60, image->clutGsHandle, mode);
        }
        D_003BA8E0++;
    }
    surface->regFunc(surface, packetList);
}

/* With a nonempty list, invoke its renderer even when input reports exit.
 * The surface owns the packet submission callback; return input status. */
s32 dds3SwapActiveResource(sdfGrOt_t *surface) {
    s32 inputStatus;

    if (sdfTexHandleLink == 0) {
        return 0;
    }
    inputStatus = dds3TextureViewerHandlePad();
    dds3DrawTextureViewerFrame(surface, dds3CurrentIncompleteResource);
    return inputStatus;
}

/* Use the default surface: empty list returns 0, exit input -1, drawing 0. */
s32 dds3LoadDefaultResource(void) {
    if (sdfTexHandleLink == 0) {
        return 0;
    }
    if (dds3TextureViewerHandlePad() == 0) {
        return -1;
    }
    dds3DrawTextureViewerFrame(&sdfDebugGrOt, dds3CurrentIncompleteResource);
    return 0;
}

INCLUDE_ASM(const s32, "code_00104370", kwlnTextureFindIncompleteResource);

/* Draw one allocation-map block in address units, choosing color by mode.
 * Only the initial partial row forces a nonzero remainder to occupy one cell;
 * the final partial row retains its truncated cell count. */
void dds3DrawTextureAllocationMapBlock(void *packetList, s32 x, s32 y, sdfGsMemSlice_t *block, u8 mode) {
    s32 color;
    u32 remainingUnits;
    s32 columnIndex;
    s32 rowUnits;
    s32 leftX;
    s32 topY;
    s32 cellCount;

    if (mode == 0) {
        switch ((s32)block->type) {
        case 1:
            color = DDS3_MAP_GREEN;
            break;
        case 2:
            color = DDS3_MAP_YELLOW;
            break;
        case 3:
            color = DDS3_MAP_RED;
            break;
        default:
            color = 0;
            break;
        }
    } else {
        color = mode == 1 ? DDS3_MAP_BLUE : DDS3_MAP_CYAN;
    }
    remainingUnits = block->size;
    topY = y + (block->addr >> DDS3_MAP_ROW_SHIFT) * DDS3_MAP_ROW_Y_STEP;
    columnIndex = (block->addr & DDS3_MAP_ROW_MASK) >> DDS3_MAP_COLUMN_SHIFT;
    leftX = x + columnIndex * DDS3_MAP_CELL_X_STEP;
    if (columnIndex > 0) {
        rowUnits = (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_COLUMN_UNITS;
        if (remainingUnits >= rowUnits) {
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + (DDS3_MAP_COLUMN_COUNT - columnIndex) * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits -= rowUnits;
        } else {
            cellCount = remainingUnits >> DDS3_MAP_COLUMN_SHIFT;
            if (cellCount == 0) {
                cellCount = remainingUnits != 0;
            }
            sdfPrimLinkLine(packetList, color, 0, leftX, topY,
                                         leftX + cellCount * DDS3_MAP_CELL_X_STEP, topY + DDS3_MAP_ROW_Y_STEP,
                                         DDS3_DIAG_DEPTH, NULL);
            remainingUnits = 0;
        }
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    while (remainingUnits >= DDS3_MAP_ROW_UNITS) {
        sdfPrimLinkLine(packetList, color, 0, x, topY, x + DDS3_MAP_ROW_SPAN, topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
        remainingUnits -= DDS3_MAP_ROW_UNITS;
        topY += DDS3_MAP_ROW_Y_STEP;
    }
    if (remainingUnits != 0) {
        sdfPrimLinkLine(packetList, color, 0, x, topY,
                                     x + ((remainingUnits >> DDS3_MAP_COLUMN_SHIFT) << DDS3_MAP_CELL_X_SHIFT), topY + DDS3_MAP_ROW_Y_STEP,
                                     DDS3_DIAG_DEPTH, NULL);
    }
}

/* Draw the allocation-map border, then walk blocks through their prev links. */
void dds3DrawTextureListDiagnostic(sdfDrawTag_t *packetList, s32 x, s32 y) {
    sdfGsMemSlice_t *block = sdfGsMemGetLinkHead();

    if (block != NULL) {
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x - 0x20, y - 0x10,
                                          0x0FFFFF7F, DDS3_MAP_BORDER_WIDTH, 0x820,
                                          0x80000000, 0x80806020));
        do {
            dds3DrawTextureAllocationMapBlock(packetList, x, y, block, 0);
            block = block->next;
        } while (block != NULL);
    }
}

/* Build an allocation-map packet list and submit it through the surface callback. */
void dds3TextureAttachTask(sdfGrOt_t *surface) {
    sdfDrawTag_t *packetList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(DDS3_DIAG_PACKET_LIST_BYTES);
    sdfDrawTagInit(packetList);
    dds3DrawTextureListDiagnostic(packetList, 0x7180, 0x79C0);
    surface->regFunc(surface, packetList);
}

/* Submit the default surface's map only when the control array's first byte is zero. */
s32 dds3EnsureDefaultResource(void) {
    if (D_r1_003AD3BB[0] != 0) {
        return 0;
    }
    dds3TextureAttachTask(&sdfDebugGrOt);
    return 0;
}

INCLUDE_ASM(const s32, "code_00104370", func_r1_00105238);
INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1D0);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1D4);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1D8);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1E0);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1E8);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1F0);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B1F8);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B200);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B208);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B210);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B218);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B220);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B228);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B22C);

INCLUDE_SDATA(const s32, "code_00104370", D_r1_0046B230);

