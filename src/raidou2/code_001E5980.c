#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_0014F4C8.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "field.h"

#include "dds/dds3_task_create.h"

#include "dds/eff_manager_task.h"

#include "dds/eff_random.h"

#include "dds/eff_random_mod.h"

#include "dds/mnu_title_sound.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_string_parameter_api.h"

dds3TaskUpdateResult_t dds3EffectManagerRun(dds3ProcessID_t *task);

/* Return the next caller-owned or shared state value reduced modulo `modulus`. */
u32 dds3GetRandIntA(EffRandState *state, u32 modulus);

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, dds3ProcessFunc_t update,
                         dds3ProcessShutdownFunc_t destroy, u32 userValue);

/* The voice-file RPC sets this busy state on open and clears it on close.
 * It is separate from the PCM callback-command busy latch. */
MnuTitleSoundBusyState p2sdGetPcmState(void);

/* Read a command parameter as a signed VM integer, converting floats and
 * dereferencing global integer or float references. */
ScrVmInteger scrGetIntPara(ScrCommandParameterIndex parameterIndex);

/* Returns a borrowed read-only string for a STRING-tagged parameter, or NULL. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex);

/* Declared void for the callers that ignore its result: ee-gcc 2.96
 * allocates registers around the call as retail did only then. The
 * definition (scrTraceCode.c) returns the context it set the value in, for
 * the script commands that pass it on as their result; their units declare
 * it so. */
void scrSetIntReturnValue(ScrVmInteger retVal);

/* Seed caller-owned state from the clock; NULL selects the shared state. */
void sdfRandomizeByTime(EffRandState *state);

#include "dds/fld_area_work.h"

#include "dds/common.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_string_parameter_api.h"

#include "dds/evt_solar.h"

#include "dds/eff_random_mod.h"

#include "dds/dds3_task_create.h"

#include "dds/eff_manager_task.h"

#include "dds/eff_random.h"

#include "dds/mnu_title_sound.h"

#include "dds/fld_scene_selection_resource.h"

extern s32 D_0032E4E4[];

u32 fldCmdGetCurrentSceneSelectionResource(void);

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommand_FLD_GET_KIRETA(void);

extern s32 fldMirroredSolarThresholds[];

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void);

extern s32 p2sdLoadPcmFile(const char *name, u8 channel);

extern void p2sdSyncSound(void);

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void);

u32 fldCommandSendSoundControl(void);

/* Return sound-command busy state to the field script interpreter. */
u32 fldCommandIsSoundBusy(void);

extern s32 D_0032E3C0[];

extern void fldMarkStageExplorationBitmapRegion(s32, s32, s32, s32, s32, s32);

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void);

s32 fldCmdSetStateFlag(void);

extern EffRandState effDefaultRandomState[];

extern void fileManagerResetSubsystems(void);

extern void effInitializeBillResourceOwners(void);

extern void dds3PolygonManagerInit(void);

extern void dds3ParticleManagerInit(void);

extern void effFieldTaskStartupHook(void);

extern void Eve_EffectInit(void);

extern void effBTLFieldColorResetFlags(void);

extern EffRandState effDefaultRandomState[];

extern char D_003BB008[];

void fldCreateFieldEffectTask(void);
/* End of declarations from src/dds1/game/code_0014F4C8.c */

/* Declarations from dds-decomp src/dds1/game/code_002665E0.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "kernel_draw.h"

#include "sdk_libc.h"

#include "sdk_libkernl.h"

#include "dds/fr_font.h"

#include "dds/fr_font_measure.h"

#include "dds/kwln.h"

#include "dds/sdf_chip.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_trig.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

#include "dds/snd_command_packet.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/snd_track_playback_api.h"

#include "dds/ui_color.h"

void Exit(s32 status);

void FlushCache(s32 mode);

u32 dds3GetProcessData(const dds3ProcessID_t *task);

/* Return a borrowed scheduler-owned handle from the starting, active, or
 * destroy-pending queue. The handle remains valid through its destroy callback
 * and is freed when finalization completes; return NULL when no name matches. */
dds3ProcessID_t *dds3SearchProcessName(const char *name);

void dds3SetProcessData(dds3ProcessID_t *task, u32 userValue);

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 fclBlendColor(u32 color0, u32 color1, u32 weight);

/* Link a glyph chain into the current draw-buffer slot. */
s32 frDestroyFontRequestAfter(struct _FRQ *glyph);

s32 frGetStrWidth2(const struct _FRQ *glyphChain);

void p2sdPlayBGM(s32 trackId);

/* Apply volume and pan to a sequence track through the command queue. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* Queue sound command 0x40 without a payload. */
void p2sdStopAllSe(void);

s32 sceSifAllocIopHeap(s32 size);

s32 sceSifDmaStat(s32 transferId);

s32 sceSifInitIopHeap(void);

void *sdfChipAllocDirect(s32 size);

void sdfChipFreeDirect(void *memory);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

f32 sdfSin(f32 angle);

char *strcat(char *dst, const char *src);

#include "dds/sdk_ps2.h"

#include "dds/snd_command_packet.h"

#include "dds/common.h"

#include "dds/pty_experience_threshold_type.h"

#include "dds/scr_integer_parameter_api.h"

#include "dds/scr_integer_return_value_api.h"

#include "dds/scr_string_parameter_api.h"

#include "dds/dds3_world_selection.h"

#include "dds/snd_sequence_volume_pan.h"

#include "dds/sdf_trig.h"

#include "dds/dds3_world_value.h"

#include "dds/mnu_sound_resource_path_api.h"

#include "dds/mnu_title_sound.h"

#include "dds/mnu_voice_resource_path_api.h"

#include "dds/ui_color.h"

#include "dds/mnu_result.h"

#include "dds/brs_progress_animation_kind.h"

#include "dds/brs_result_counter.h"

#include "dds/brs_result_counter_completion_api.h"

#include "dds/dds3_world_counter.h"

#include "dds/snd_track_playback_api.h"

#include "dds/snd_work.h"

#include "dds/prf_current_record_view.h"

extern void mnuAdvanceTitleStateUnderSemaphore(void);

extern void mnuTitleStreamUpdateAndLogBgm(void);

void brsUpdateResultBackgroundFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                  BrsProgressRow *row, s32 context, s32 rowIndex);

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

/* Initialized triplet tables have their actual inferred row counts.  Separate
 * aggregate alias sets let EE GCC schedule these independent template copies
 * in the retail order; see docs/idioms.md, inferred array bounds.
 * Each selected row supplies x/y/slot values before the draw call.  The shared
 * table cursor borrows whole rows, rather than flattening inner arrays. */
void brsDrawProgressRowEntries(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                              BrsProgressRow *row, s32 context, s32 rowIndex);

void brsUpdateResultPortraitFade(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                 BrsProgressRow *row, s32 context, s32 rowIndex);

#include "dds/sdf.h"

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

extern void eglSetOt(u32 surfaceIndex);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void btlDrawButtonIconFixed64(sdfGrOt_t *, s32, s32, s32, s32, s32, s32, s32);

/* The four sprite corner colours start from the shared grey table. */
typedef struct BrsCornerColors {
    u32 rgba[4];
} BrsCornerColors;

extern const BrsCornerColors D_003AFC40;

void brsDrawProgressRowOverlays(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 surfaceIndex, s32 rowIndex);

extern void nbPlaySe(u32 sequenceId);

extern PtyExperienceThreshold ptyComputeTotalExp(const dds_datUnitWork_t *unit, s32 levelDelta);

void brsUpdateLevelProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *progressRow, s32 context, s32 rowIndex);

#include "dds/eff.h"

#include "dds/fr_font.h"

#include "dds/fr_font_draw_glyph.h"

extern struct _FRQ *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct _FRQ *);

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

extern const char D_003BC578[];

typedef struct BrsLevelCornerColors {
    u32 values[4];
} BrsLevelCornerColors;

extern const BrsLevelCornerColors D_003AFC50;

void brsDrawLevelProgressDetails(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                BrsProgressRow *row, s32 context, s32 index);

void brsUpdateLevelProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 progressRowIndex);

extern const BrsLevelCornerColors D_003AFC60;

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

void brsDrawLevelProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                                 BrsProgressRow *row, s32 context, s32 rowIndex);

void brsUpdateProfileProgressIndicator(s32 screenX, s32 screenY, s32 depth, BrsSkillPackageWork *work, BrsProgressRow *row, s32 context, s32 rowIndex);

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);

void brsDrawProfileProgressIndicator(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
    BrsProgressRow *row, s32 context, s32 rowIndex);

extern PtyExperienceThreshold ptyComputeTotalExp(const dds_datUnitWork_t *unit, s32 levelDelta);

extern s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *);

extern u32 prfGetCapValue(u16);

s32 brsCalcProgressAnimationStep(BrsSkillPackageWork *work, BrsProgressRow *progressRow,
                                 s32 rowIndex, BrsProgressAnimationKind progressKind);

#include "dds/common.h"

#include "dds/mnu_result.h"

#include "dds/sdf_chip.h"

#include "dds/dds3_world_value.h"

#include "dds/sdf_heap_api.h"

#include "dds/kwln.h"

#include "dds/dds3_task_create.h"

#include "dds/mnu_title_effect.h"

#include "dds/file.h"

#include "dds/file_request_api.h"

#include "dds/sdf_thread.h"

#include "dds/mnu_title_stream.h"

#define BRS_RESULT_COUNTER_PAIR_COUNT 5

#define BRS_RESULT_SETTLED_POLL_LIMIT 6

#define BRS_RESULT_SETTLED_POLL_CLAMP 7

#define SND_TRACK_DEFAULT_VOLUME 0x7f

#define SND_TRACK_DEFAULT_PAN 0x3f

#define SND_TRANSFER_WORD_BYTES 4

#define SND_TRANSFER_WORD_HALFWORDS 2

#define SND_TRANSFER_PAIR_WORD_BYTES 8

#define SND_MIX_SAMPLE_COUNT 0x800

#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_EFFECT_BYTES 8

#define MNU_TITLE_VOICE_PREFIX_BYTES 5

#define MNU_TITLE_VOICE_PATH_BYTES 16

#define MNU_TITLE_SOUND_PATH_BYTES 32

#define MNU_SOUND_FRAME_BYTES_LARGE 0x180

#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0

#define MNU_SOUND_FRAME_BYTES_SMALL 0x60

#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400

#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200

#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

extern struct dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 counter, u128 *position, u128 *rotation);

struct dds3Basic_t;

extern void dds3GetRadiusPoseSet(struct dds3Basic_t *object, f32 value);

extern u8 mnuTitleCameraPosition[];

extern u8 mnuTitleCameraRotation[];

extern struct dds3Basic_t *mnuTitleCameraObject;

extern char D_003AFD48[];

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern u32 mnuTitleAudioSilenceBuffer[];

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamSemaphore;

extern u32 mnuTitleStreamFileRequest;

extern u32 D_003BC5BC;

extern s32 D_003BC5C8;

extern u8 D_003D9178[];

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern dds3ProcessID_t *mnuTitleSoundTask;

extern char D_003771D8[];

#include "dds/sdf_debug_api.h"

extern void mnuCreateTitleEffectTask(void);

extern u8 D_003BC5A0[];

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern s32 sndStreamTransferIopHeapAddress;

extern char D_003AFBA0[];

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounterView levelCounters[8];
    BrsResultCounterView profileCounters[8];
} BrsResultWork;

extern s32 ptyGetCurrentProfileId(const dds_datUnitWork_t *);

extern u32 prfGetCapValue(u16);

extern void nbPlaySe(u32 sequenceId);

void brsUpdateProfileProgressAnimation(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *row, s32 context, s32 rowIndex);

#include "dds/eff.h"

#include "dds/fr_font.h"

#include "dds/fr_font_draw_glyph.h"

#include "dds/fr_font_measure.h"

#include "dds/prf_profile_cap.h"

extern s32 prfResolveMantraNameOrFallback(u16, u8 **);

extern s32 mnuIsTitleEntryAvailable(dds_datUnitWork_t *);

typedef struct BrsProfilePalette { u32 values[4]; } BrsProfilePalette;

extern const BrsProfilePalette D_003AFC80;

void brsDrawProfileProgressDetails(s32 x, s32 y, s32 depth, BrsSkillPackageWork *work,
                                  BrsProgressRow *row, s32 context, s32 index);

/* Complete after six successful polls of five counter pairs.
 * Early returns deliberately leave the accumulated settled count unchanged. */
BrsResultCounterCompletionResult brsPollResultCounterCompletion(void);

void nbPlaySe(u32 sequenceId);

void func_00269400(void);

void func_00269408(void);

void func_00269410(void);

void func_00269418(void);

/* Copy the packed eight-byte prefix, then append the caller's filename. */
char *sdfSndMakeFilename(char *pathBuffer, const char *filename);

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename);

extern s8 D_003BC58C;

extern char D_00377318[];

extern s32 D_00377338[];

extern s32 D_00370D10[];

extern s32 D_003BC5A8;

extern s16 D_003BD8B0;

extern s32 D_003BC5AC;

extern void sdfSndInit(char *, s32, const s32 *, s32);

extern void p2sdLoadSMGFile(s32);

extern void mnuInitializeTitleAudioStreams(void);

extern void mnuCreateTitleEffectTask(void);

void mnuInitializeTitleAudioAndEffects(void);

u32 func_002694F8(void);

s32 mnuIncrementTitleEffectFrameCounter(dds3ProcessID_t *task);

void mnuDestroyTitleEffectTask(dds3ProcessID_t *task);

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void mnuCreateTitleEffectTask(void);

void mnuResetTitleEffectState(s32 command);

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex);

void mnuPlayTitleVoiceFile(char *voiceArgument);

void mnuStopTitleVoicePlayback(void);

MnuTitleSoundBusyState mnuQueryTitleSoundBusy(void);

void mnuNoOpTitleAudio(void);

s32 mnuGetTitleEffectFrameCounter(void);

u32 sndOpStartTrackFromScript(void);

u32 sndOpSetTrackDefaultVolumePan(void);

u32 sndOpSendTitleAudioCommand(void);

s32 mnuInitializeTitleEffects(void);

u32 sndOpStopNamedPlayback(void);

u32 sndOpPushCommandBusyState(void);

u32 sndOpAlwaysSucceeds(void);

s32 movCheckStartupSoundState(void);

u32 sndOpUpdateTitleBgmState(void);

u32 mnuResetTitlePlaybackCommands(void);

u32 mnuAdvanceTitleStreamFromScript(void);

u8 mnuIsTitleStreamIdle(void);

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount);

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount);

/* Send the per-core parameter commands for both sound cores, then the IOP
 * stream buffer with the transfer size. */
void mnuConfigureTitleAudioStream(s32 transferSize);

void mnuInitTitleSoundRemoteRequest(u32 wordCount);

extern u64 D_003BD8B8;

s32 mnuCopySoundStreamOnStatusChange(const void *source, u32 words);
/* End of declarations from src/dds1/game/code_002665E0.c */

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
}

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CF838);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CF8F8);

INCLUDE_ASM(const s32, "code_001E5980", mnuIncrementTitleEffectFrameCounter);

INCLUDE_ASM(const s32, "code_001E5980", mnuDestroyTitleEffectTask);

INCLUDE_ASM(const s32, "code_001E5980", mnuCreateTitleEffectTask);

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFA58);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFAF8);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFB30);

void mnuStopTitleVoicePlayback(void) {
    p2sdSyncSound();
}

MnuTitleSoundBusyState mnuQueryTitleSoundBusy(void) {
    return p2sdGetPcmState();
}

void mnuNoOpTitleAudio(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->frameCounter;
}

INCLUDE_ASM(const s32, "code_001E5980", func_001E5D50);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFBD8);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFC50);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFC98);

INCLUDE_ASM(const s32, "code_001E5980", sndOpStopNamedPlayback);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFD08);

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    const char *commandName;
    commandName = scrGetStrPara(0);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(commandName, 0x7f);
    return 1;
}

u32 sndOpUpdateTitleBgmState(void) {
    mnuTitleStreamUpdateAndLogBgm();
    return 1;
}

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFDA0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFDC8);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFDD0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFE88);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFEB0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFEE0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFF08);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFF30);

u8 mnuIsTitleStreamIdle(void) {
    MnuTitleStreamLoadState loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

INCLUDE_ASM(const s32, "code_001E5980", func_001E6178);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFFD0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001CFFF0);

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001D0010);

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount) {
    SifDmaTransfer transfer;
    s32 dmaRequest;
    transfer.source = (u32)source;
    transfer.destination = iopAddress;
    transfer.size = wordCount * SND_TRANSFER_WORD_BYTES;
    transfer.attributes = 0;
    FlushCache(0);
    dmaRequest = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(dmaRequest) >= 0) {
    }
    return dmaRequest;
}

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount) {
    s32 bufferBytes = wordCount * SND_TRANSFER_WORD_BYTES;
    s32 halfwordIndex;

    D_003BC5B8 = sdfMemGetPtr(sdfMemAllocDirect(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_003BC5B8)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    sndStreamTransferIopHeapAddress = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (sndStreamTransferIopHeapAddress <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = sndStreamTransferIopHeapAddress;
    D_003BC5B0[1] = sndStreamTransferIopHeapAddress + bufferBytes;
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[1], wordCount);
}

INCLUDE_ASM(const s32, "code_001E5980", func_r1_001D0150);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "code_001E5980", mnuCopySoundStreamOnStatusChange);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB050);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB060);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB070);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB080);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB090);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB0F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB100);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB110);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB120);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB130);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB140);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB150);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB160);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB170);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB180);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB190);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB1F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB200);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB210);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB220);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB230);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB240);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB250);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB260);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB270);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB280);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB290);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB2F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB300);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB310);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB320);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB330);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB340);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB350);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB360);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB370);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB380);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB390);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB3F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB400);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB410);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB420);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB430);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB440);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB450);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB460);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB470);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB480);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB490);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB4F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB500);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB510);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB520);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB530);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB540);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB550);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB560);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB570);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB580);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB590);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB5F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB600);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB610);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB620);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB630);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB640);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB650);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB660);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB670);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB680);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB690);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB6F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB700);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB710);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB720);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB730);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB740);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB750);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB760);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB770);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB780);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB790);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB7F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB800);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB810);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB820);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB830);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB840);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB850);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB860);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB870);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB880);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB890);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB8F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB900);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB910);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB920);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB930);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB940);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB950);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB960);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB970);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB980);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB990);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9A0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9B0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9C0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9D0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9E0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BB9F0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA00);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA10);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA20);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA30);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA40);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA50);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA60);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA70);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA80);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBA90);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBAA0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBAB0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBAC0);

INCLUDE_RODATA(const s32, "code_001E5980", D_004BBAD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD08);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD0C);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD10);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD18);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD20);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD28);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD2C);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD30);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD38);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD40);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD44);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004DFFF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0000);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0008);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0010);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0018);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0020);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0028);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0030);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0038);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E003C);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0040);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0048);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0050);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0058);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0060);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0068);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0070);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0078);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0080);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0088);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0090);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0098);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E00F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0100);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0108);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0110);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0118);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0120);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0128);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0130);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0138);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0140);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0148);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0150);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0158);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0160);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0168);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0170);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0178);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0180);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0188);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0190);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0198);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E01F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0200);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0208);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0210);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0218);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0220);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0228);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0230);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0238);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0240);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0248);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0250);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0258);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0260);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0268);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0270);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0278);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0280);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0288);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0290);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0298);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E02F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0300);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0308);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0310);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0318);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0320);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0328);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0330);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0338);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0340);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0348);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0350);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0358);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0360);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0368);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0370);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0378);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0380);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0388);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0390);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0398);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E03F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0400);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0408);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0410);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0418);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0420);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0428);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0430);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0438);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0440);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0448);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0450);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0458);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0460);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0468);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0470);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0478);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0480);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0488);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0490);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0498);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E04F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0500);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0508);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0510);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0518);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0520);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0528);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0530);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0538);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0540);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0548);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0550);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0558);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0560);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0568);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0570);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0578);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0580);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0588);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0590);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0598);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E05F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0600);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0608);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0610);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0618);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0620);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0628);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0630);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0638);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0640);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0648);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0650);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0658);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0660);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0668);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0670);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0678);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0680);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0688);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0690);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0698);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E06F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0700);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0708);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0710);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0718);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0720);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0728);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0730);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0738);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0740);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0748);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0750);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0758);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0760);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0768);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0770);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0778);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0780);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0788);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0790);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0798);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E07F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0800);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0808);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0810);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0818);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0820);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0828);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0830);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0838);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0840);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0848);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0850);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0858);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0860);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0868);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0870);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0878);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0880);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0888);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0890);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0898);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E08F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0900);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0908);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0910);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0918);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0920);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0928);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0930);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0938);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0940);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0948);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0950);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0958);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0960);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0968);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0970);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0978);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0980);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0988);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0990);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0998);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E09F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0A98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0AF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0B98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0BF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0C98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0CF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0D98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0DF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0E98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0ED0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0ED8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0EF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F00);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F08);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F10);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F18);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F20);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F28);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F30);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F38);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F40);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F48);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F50);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F58);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F60);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F68);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F70);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F78);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F80);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F88);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F90);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0F98);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FA0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FA8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FB0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FB8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FC0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FC8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FD0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FD8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FE0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FE8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FF0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E0FF8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1000);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1008);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1010);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1018);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1020);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1028);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1030);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1038);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1040);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1048);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1050);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1058);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1060);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1068);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1070);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1078);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1080);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1088);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1090);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1098);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E10F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1100);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1108);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1110);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1118);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1120);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1128);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1130);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1138);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1140);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1148);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1150);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1158);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1160);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1168);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1170);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1178);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1180);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1188);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1190);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1198);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E11F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1200);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1208);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1210);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1218);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1220);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1228);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1230);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1238);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1240);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1248);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1250);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1258);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1260);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1268);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1270);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1278);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1280);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1288);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1290);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1298);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E12F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1300);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1308);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1310);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1318);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1320);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1328);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1330);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1338);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1340);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1348);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1350);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1358);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1360);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1368);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1370);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1378);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1380);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1388);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1390);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1398);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13D0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13D8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13E0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13E8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13F0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E13F8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1400);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1408);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1410);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1418);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1420);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1428);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1430);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1438);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1440);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1448);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1450);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1458);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1460);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1468);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1470);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1478);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1480);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1488);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1490);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E1498);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14A0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14A8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14B0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14B8);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14C0);

INCLUDE_SDATA(const s32, "code_001E5980", D_004E14C8);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD48);

INCLUDE_SDATA(const s32, "code_001E5980", D_r1_0046CD4C);

