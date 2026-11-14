#include "common.h"

/* Declarations from smt3-decomp src/smt3/kernel/dds3PadManager.c */
#include "common.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "sdfFrame.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

extern u8 dds3PadVibrationState[2];

extern u8 dds3PadVibrationCurrent;

extern u8 dds3PadVibrationMode[2];

extern s32 dds3PadVibrationTable[2];

extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 dds3GetGlobalConfigFlag(u32 flag);

extern void dds3GetVibrationLevel(void);

extern void sdfPadSetAct0(s32 pad, u16 value);

extern void sdfPadSetAct1(s32 pad, u8 value);

extern void sdfPadSetAct(s32 pad, s32 value0, s32 value1);

extern u8 dds3PadButtonState[64];

extern u8 dds3PadButtonPrev[64];

extern u32 dds3PadButtonHoldCounters[64];

extern u32 dds3PadStickHoldCounters[4];

extern u8 dds3PadStickPrev[8];

extern s8 dds3PadStickStatusPrev[4];

extern u8 dds3PadStickState[8];

extern u8 dds3PadStickStatusState[4];

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

s32 dds3PadVibrationTest(dds3ProcessID_t *task);

void dds3PadInitialize(void);

extern s8 dds3PadBitTable[32];

extern u8 sdfPadAnalogSticks[8];

extern void dds3UpdateVibrationSlots(void);

extern void effPadVibeFadeUpdate(void);

extern f32 acosf(f32 x);

void dds3RunProcessList(void);

#include "sdk_libc.h"

void dds3GetVibrationLevel(void);

void dds3PadVibrationSet(u32 motor, u8 level, s32 duration);

void dds3PadVibrationReset(void);

void dds3UpdateVibrationSlots(void);

extern const char dds3StartMenuModeSelect[16];

extern const char dds3StartMenuEndSaveTest[16];

extern const char dds3StartMenuStaffTest[16];

extern const char dds3StartMenuMovieViewer[16];

extern const char dds3StartMenuPuzzleBoyGame[16];

extern const char dds3StartMenuMagatuhiEditor[16];

extern const char dds3StartMenuSpriteViewer[16];

extern const char dds3StartMenuParticleEditor[16];

extern const char dds3StartMenuFontTest[16];

extern const char dds3StartMenuSoundTest[16];

extern const char dds3StartMenuModelViewer[16];

extern const char dds3StartMenuNameEntryTest[16];

extern const char dds3StartMenuResult2Test[16];

extern const char dds3StartMenuTerminalTest[16];

extern const char dds3StartMenuRecoverTest[16];

extern const char dds3StartMenuRagTest[16];

extern const char dds3StartMenuShopTest[16];

extern const char dds3StartMenuCombineTest[16];

extern const char dds3StartMenuCampTest[16];

extern const char dds3StartMenuMessageScriptTest[24];

extern const char dds3StartMenuEventViewer[16];

extern const char dds3StartMenuEventTest[16];

extern const char dds3StartMenuKernelTest[16];

extern const char dds3StartMenuModelCollisionEditor[24];

extern const char dds3StartMenuBattleFieldTest[24];

extern const char dds3StartMenuNewBattleTest[16];

extern const char dds3StartMenuGameStart[16];
/* End of declarations from src/smt3/kernel/dds3PadManager.c */

/* Declarations from dds-decomp src/dds1/game/code_00102ED8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "common.h"

#include "sdfDevCons.h"

#include "dds/btl.h"

#include "dds/dds3_draw_buffer.h"

#include "dds/dds3_fade_color.h"

#include "dds/dds3_process_registration.h"

#include "dds/dds3_task_create.h"

#include "dds/dds3_task_lifecycle.h"

#include "dds/eff_random.h"

#include "dds/evt_frame_state_packet.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_gs_blend.h"

#include "dds/sdf_gs_scene_state.h"

#include "dds/sdf_linked_packet.h"

#include "dds/sdf_packet_alloc.h"

#include "dds/sdf_packet_builders.h"

#include "dds/sdf_packet_list.h"

#include "dds/sdf_projection.h"

#include "dds/sdf_scene_packet.h"

#include "dds/sdf_tex_packet.h"

#include "dds/sdf_texture_clamp.h"

#include "dds/sdf_texture_draw_packet.h"

#include "dds/sdf_texture_lists.h"

#include "dds/sdf_texture_registry.h"

/* Test membership in the starting, running, and destruction-pending queues. */
dds3ProcessRegistrationResult_t dds3ExistProcess(const struct dds3ProcessID_t *task);

struct sdfDrawTag_t *dds3FontReqAllocDrawTag(void);

/* Return the current frame draw-buffer slot (0 or 1). */
u32 dds3GetFrameID(void);

/* Returns 24 random bits scaled to [0, 1); NULL selects the shared state. */
f32 dds3GetRandFloatA(EffRandState *state);

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

extern u8 D_r1_0046B1E0;

extern s8 D_003BA7FC;

extern s8 dds3BackgroundFadeMode;

extern s16 dds3BackgroundFadeFirstRamp;

extern s16 dds3BackgroundFadeSecondRamp;

extern u8 dds3BackgroundFadeColor[4];

extern s8 D_r1_003AD3BB[];

extern f32 D_003245EC[];

extern sdfGrOt_t sdfDebugGrOt;

extern s32 D_003BD6A0[2];

extern u8 D_r1_0046F619;

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

extern s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

extern dds3ProcessID_t *dds3DebugGraphTask;

extern s32 dds3TextureCountIncompleteResources(void);

extern s32 D_003BA724;

extern s32 distblurovercnt;

extern s32 ripblurovercnt;

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

s32 func_dds1_00103400(const char *name, void (*setState)(s32));

s32 fclEncycAllocEntry(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

s32 func_dds1_00103540(const char *name, dds3ProcessFunc_t update, dds3ProcessShutdownFunc_t destroy, void *userValue);

void dds3InitDebugSystem(void);

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

extern u8 D_003BA860[2][2][2];

extern s8 D_003BA868[2][2];

extern u8 D_003BA870[2][2][2];

extern u8 D_003BA878[2][2];

extern u8 D_003BD698[2];

void func_dds1_00103B10(void);

void Act2Sub(void);

/* Clear motor levels and countdowns, then send both zero strengths to pad 0. */
void func_r1_00104210(void);

s32 dds3TaskNoopCallback(dds3ProcessID_t *task);

/* Seed the shared effect random state with the fixed initialization seed. */
void dds3InitRandCB(void);

extern s16 D_r1_0046B128;

extern u16 sdfDrawFrameCounter;

extern u16 D_r1_0046B1D0;

extern u16 D_003BA882;

extern u16 D_r1_0046B1D4;

extern u16 D_003BA886;

extern char D_r1_0046B1D8[];

s32 dds3DebugGraphUpdate(dds3ProcessID_t *task);

void dds3DebugGraphTaskDestroy(dds3ProcessID_t *task);

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

extern s32 D_r1_0046B230;

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

extern f32 D_dds1_00324780[4];

extern Matrix4 sdfViewMatrix;

extern Matrix4 D_00329790;

extern sdf3DLight_t *D_dds1_00324B10[3];

extern f32 D_dds1_00324B20[4];

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
void func_dds1_00105630(void);

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

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadVibrationTest);

void dds3PadInitialize(void) {
    s32 pad;
    s32 half;
    s32 key;

    for (pad = 0; pad < 2; pad++) {
        for (half = 0; half < 2; half++) {
            for (key = 0; key < 16; key++) {
                dds3PadButtonHoldCounters[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonPrev[pad * 16 + half * 32 + key] = 0;
                dds3PadButtonState[pad * 16 + half * 32 + key] = 0;
            }
        }
        for (key = 0; key < 2; key++) {
            dds3PadStickHoldCounters[pad * 2 + key] = 0;
            dds3PadStickPrev[pad * 4 + key * 2] = 0;
            dds3PadStickPrev[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusPrev[pad * 2 + key] = 0;
            dds3PadStickState[pad * 4 + key * 2] = 0;
            dds3PadStickState[pad * 4 + key * 2 + 1] = 0;
            dds3PadStickStatusState[pad * 2 + key] = 0;
        }
    }
    dds3PadVibrationState[0] = 0;
    dds3PadVibrationMode[0] = 0;
    dds3PadVibrationTable[0] = 0;
    dds3PadVibrationState[1] = 0;
    dds3PadVibrationMode[1] = 0;
    dds3PadVibrationTable[1] = 0;
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadUpdate);

void Act2Sub(void) {
    u8 target = D_r1_0046F619;

    if (dds3PadVibrationState[1] < target) {
        if (target - dds3PadVibrationState[1] > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] + 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else if (target < dds3PadVibrationState[1]) {
        if (dds3PadVibrationState[1] - target > 0x20) {
            dds3PadVibrationState[1] = dds3PadVibrationState[1] - 0x20;
        } else {
            dds3PadVibrationState[1] = target;
        }
    } else {
        dds3PadVibrationState[1] = target;
    }
}

INCLUDE_ASM(const s32, "kernel/dds3PadManager", dds3PadAct);

INCLUDE_ASM(const s32, "kernel/dds3PadManager", func_r1_00104210);

void dds3UpdateVibrationSlots(void) {
    s32 largeMotor = 1;
    u8 *levels = dds3PadVibrationState;
    u8 *targets = dds3PadVibrationMode;
    u8 *largeLevel;
    s32 motor;

    for (motor = 0; motor < 2; motor++) {
        if (dds3PadVibrationTable[motor] != 0) {
            /* The native countdown wraps as a 32-bit word, including negative durations. */
            dds3PadVibrationTable[motor] = (s32)((u32)dds3PadVibrationTable[motor] - 1);
            if (dds3PadVibrationTable[motor] == 0) {
                levels[motor] = 0;
                if (motor == 0) {
                    sdfPadSetAct0(0, 0);
                } else {
                    sdfPadSetAct1(0, 0);
                }
            } else if (motor == largeMotor) {
                largeLevel = levels + 1;
                if (*largeLevel != targets[1]) {
                    dds3GetVibrationLevel();
                    sdfPadSetAct1(0, *largeLevel);
                }
            }
        }
    }
}
INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1B0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1B1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1BF);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1C0);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1C1);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1C2);

INCLUDE_SDATA(const s32, "kernel/dds3PadManager", D_r1_0046B1C3);

