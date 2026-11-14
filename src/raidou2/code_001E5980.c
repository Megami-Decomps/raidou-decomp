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

#include "dds/dds3_process.h"

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

#include "dds/dds3_process.h"

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

extern void ProcessInit(void);

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

extern void ProcessInit(void);

void mnuInitializeTitleAudioAndEffects(void);

u32 func_002694F8(void);

s32 mnuIncrementTitleEffectFrameCounter(dds3ProcessID_t *task);

void mnuDestroyTitleEffectTask(dds3ProcessID_t *task);

/* Allocate the two-word effect state and attach it to the frame-counter task. */
void ProcessInit(void);

void mnuResetTitleEffectState(s32 command);

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex);

void mnuPlayTitleVoiceFile(char *voiceArgument);

void scrGetAfterSkipBgmFlag(void);

MnuTitleSoundBusyState scrCheckEnableMovieCancel(void);

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

INCLUDE_ASM(const s32, "code_001E5980", sndInit);

INCLUDE_ASM(const s32, "code_001E5980", sndLeaf);

INCLUDE_ASM(const s32, "code_001E5980", mnuIncrementTitleEffectFrameCounter);

INCLUDE_ASM(const s32, "code_001E5980", mnuDestroyTitleEffectTask);

INCLUDE_ASM(const s32, "code_001E5980", ProcessInit);

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

INCLUDE_ASM(const s32, "code_001E5980", fldNowPlayengStart);

INCLUDE_ASM(const s32, "code_001E5980", fldNKIY);

INCLUDE_ASM(const s32, "code_001E5980", fldCheckNextFile);

void scrGetAfterSkipBgmFlag(void) {
    p2sdSyncSound();
}

MnuTitleSoundBusyState scrCheckEnableMovieCancel(void) {
    return p2sdGetPcmState();
}

void mnuNoOpTitleAudio(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->frameCounter;
}

INCLUDE_ASM(const s32, "code_001E5980", scrLeaf);

INCLUDE_ASM(const s32, "code_001E5980", scrCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_001E5980", scrCallGetIntPara7);

INCLUDE_ASM(const s32, "code_001E5980", scrCallGetIntPara8);

INCLUDE_ASM(const s32, "code_001E5980", sndOpStopNamedPlayback);

INCLUDE_ASM(const s32, "code_001E5980", slCallGetIntPara);

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

INCLUDE_ASM(const s32, "code_001E5980", slCallGetPcmState);

INCLUDE_ASM(const s32, "code_001E5980", slLeaf3);

INCLUDE_ASM(const s32, "code_001E5980", slHardSelect1);

INCLUDE_ASM(const s32, "code_001E5980", slCallBgmPlay);

INCLUDE_ASM(const s32, "code_001E5980", evtCloseSkyEventTask);

INCLUDE_ASM(const s32, "code_001E5980", At3Play);

INCLUDE_ASM(const s32, "code_001E5980", evtCallBgmFadeOut);

INCLUDE_ASM(const s32, "code_001E5980", evtCallBgmContinue);

u8 mnuIsTitleStreamIdle(void) {
    MnuTitleStreamLoadState loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

INCLUDE_ASM(const s32, "code_001E5980", evtCallSePlay);

INCLUDE_ASM(const s32, "code_001E5980", evtFunc7);

INCLUDE_ASM(const s32, "code_001E5980", evtCallEffectShutdown);

INCLUDE_ASM(const s32, "code_001E5980", evtCallSeFadeOut);

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

INCLUDE_ASM(const s32, "code_001E5980", evtFunc8);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "code_001E5980", mnuCopySoundStreamOnStatusChange);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrOpTitle);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr600001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr681001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr680002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr690004);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr696001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr695001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr690005);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr6900042);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr690003);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr690002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr690001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr687001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr685001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr680001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr621001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr620001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr619001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr617002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr617001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr616001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr615001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr614002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr614001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr613002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr613001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr612001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr611004);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr611003);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr611002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr611001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr610001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr609005);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr609004);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr609003);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr609002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr609001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr606002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr606001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr605003);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr605002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr605001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr604003);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr604002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr604001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr603002);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr603001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr602001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStr601001);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle06);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle05);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle04);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle03);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle02);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrBattle01);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrEpilogue);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrMilkHall);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrAyashii2);

INCLUDE_RODATA(const s32, "code_001E5980", evtStrKanashii);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon0);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon02);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon03);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon09);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon08);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon07);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon06);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon05);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon04);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon032);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon022);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMahjon01);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse50);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse4);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse42);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse43);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse44);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse45);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse46);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse49);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse48);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse47);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse462);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse452);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse442);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse432);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse422);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse41);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse40);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse3);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse32);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse33);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse34);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse35);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse36);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse39);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse38);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse37);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse362);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse352);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse342);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse332);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse322);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse31);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse30);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse2);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse22);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse23);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse24);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse25);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse26);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse29);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse28);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse27);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse262);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse252);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse242);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse232);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse222);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse21);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse20);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse1);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse12);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse13);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse14);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse15);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse16);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse19);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse18);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse17);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse162);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse152);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse142);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse132);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse122);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse11);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse10);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse0);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse02);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse03);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse04);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse05);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse06);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse09);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse08);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse07);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse062);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse052);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse042);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse032);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse022);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse01);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrMapse00);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe11);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe10);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe0);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe02);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe03);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe04);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe05);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe06);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe09);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe08);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe07);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe062);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe052);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe042);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe032);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe022);

INCLUDE_RODATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3StrEveSe01);

INCLUDE_SDATA(const s32, "code_001E5980", evtPicturePriList);

INCLUDE_SDATA(const s32, "code_001E5980", sndInitData2);

INCLUDE_SDATA(const s32, "code_001E5980", sndPathPrefix);

INCLUDE_SDATA(const s32, "code_001E5980", sndPathPrefix2);

INCLUDE_SDATA(const s32, "code_001E5980", evtPicturePriProcName);

INCLUDE_SDATA(const s32, "code_001E5980", fldNKIYData);

INCLUDE_SDATA(const s32, "code_001E5980", sndInitData3);

INCLUDE_SDATA(const s32, "code_001E5980", sndInitData4);

INCLUDE_SDATA(const s32, "code_001E5980", sndData);

INCLUDE_SDATA(const s32, "code_001E5980", sndInitializeStreamTransferBuffersData);

INCLUDE_SDATA(const s32, "code_001E5980", mnuCopySoundStreamOnStatusChangeData);

INCLUDE_SDATA(const s32, "code_001E5980", evtData30);

INCLUDE_SDATA(const s32, "code_001E5980", evtData31);

INCLUDE_SDATA(const s32, "code_001E5980", evtData32);

INCLUDE_SDATA(const s32, "code_001E5980", evtData33);

INCLUDE_SDATA(const s32, "code_001E5980", evtData34);

INCLUDE_SDATA(const s32, "code_001E5980", evtData35);

INCLUDE_SDATA(const s32, "code_001E5980", evtData36);

INCLUDE_SDATA(const s32, "code_001E5980", evtData37);

INCLUDE_SDATA(const s32, "code_001E5980", evtData38);

INCLUDE_SDATA(const s32, "code_001E5980", evtData39);

INCLUDE_SDATA(const s32, "code_001E5980", evtData40);

INCLUDE_SDATA(const s32, "code_001E5980", evtData41);

INCLUDE_SDATA(const s32, "code_001E5980", evtData42);

INCLUDE_SDATA(const s32, "code_001E5980", evtData43);

INCLUDE_SDATA(const s32, "code_001E5980", evtData44);

INCLUDE_SDATA(const s32, "code_001E5980", evtData45);

INCLUDE_SDATA(const s32, "code_001E5980", evtData46);

INCLUDE_SDATA(const s32, "code_001E5980", evtData47);

INCLUDE_SDATA(const s32, "code_001E5980", evtData48);

INCLUDE_SDATA(const s32, "code_001E5980", evtData49);

INCLUDE_SDATA(const s32, "code_001E5980", evtData50);

INCLUDE_SDATA(const s32, "code_001E5980", evtData51);

INCLUDE_SDATA(const s32, "code_001E5980", evtData52);

INCLUDE_SDATA(const s32, "code_001E5980", evtData53);

INCLUDE_SDATA(const s32, "code_001E5980", evtData54);

INCLUDE_SDATA(const s32, "code_001E5980", evtData55);

INCLUDE_SDATA(const s32, "code_001E5980", evtData56);

INCLUDE_SDATA(const s32, "code_001E5980", evtData57);

INCLUDE_SDATA(const s32, "code_001E5980", evtData58);

INCLUDE_SDATA(const s32, "code_001E5980", evtData59);

INCLUDE_SDATA(const s32, "code_001E5980", evtData60);

INCLUDE_SDATA(const s32, "code_001E5980", evtData61);

INCLUDE_SDATA(const s32, "code_001E5980", evtData62);

INCLUDE_SDATA(const s32, "code_001E5980", evtData63);

INCLUDE_SDATA(const s32, "code_001E5980", evtData64);

INCLUDE_SDATA(const s32, "code_001E5980", evtData65);

INCLUDE_SDATA(const s32, "code_001E5980", evtData66);

INCLUDE_SDATA(const s32, "code_001E5980", evtData67);

INCLUDE_SDATA(const s32, "code_001E5980", evtData68);

INCLUDE_SDATA(const s32, "code_001E5980", evtData69);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry);

INCLUDE_SDATA(const s32, "code_001E5980", evtData70);

INCLUDE_SDATA(const s32, "code_001E5980", evtData71);

INCLUDE_SDATA(const s32, "code_001E5980", evtData72);

INCLUDE_SDATA(const s32, "code_001E5980", evtData73);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry2);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry3);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry4);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry5);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry6);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry7);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry8);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry9);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry10);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry11);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry12);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry13);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry14);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry15);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry16);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry17);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry18);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry19);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry20);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry21);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry22);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry23);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry24);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry25);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry26);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry27);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry28);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry29);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry30);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry31);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry32);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry33);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry34);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry35);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry36);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry37);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry38);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry39);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry40);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry41);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry42);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry43);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry44);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry45);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry46);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry47);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry48);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry49);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry50);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry51);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry52);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry53);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry54);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry55);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry56);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry57);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry58);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry59);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry60);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry61);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry62);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry63);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry64);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry65);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry66);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry67);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry68);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry69);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry70);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry71);

INCLUDE_SDATA(const s32, "code_001E5980", mnuQueueTitleSoundStreamEntryData3Entry72);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry2);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry3);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry4);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry5);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry6);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry7);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry8);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry9);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry10);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry11);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry12);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry13);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry14);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry15);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry16);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry17);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry18);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry19);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry20);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry21);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry22);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry23);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry24);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry25);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry26);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry27);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry28);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry29);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry30);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry31);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry32);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry33);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry34);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry35);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry36);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry37);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry38);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry39);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry40);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry41);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry42);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry43);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry44);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry45);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry46);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry47);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry48);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry49);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry50);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry51);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry52);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry53);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry54);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry55);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry56);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry57);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry58);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry59);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry60);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry61);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry62);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry63);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry64);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry65);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry66);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry67);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry68);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry69);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry70);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry71);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry72);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry73);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry74);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry75);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry76);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry77);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry78);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry79);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry80);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry81);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry82);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry83);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry84);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry85);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry86);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry87);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry88);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry89);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry90);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry91);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry92);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry93);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry94);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry95);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry96);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry97);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry98);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry99);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry100);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry101);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry102);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry103);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry104);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry105);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry106);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry107);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry108);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry109);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry110);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry111);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry112);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry113);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry114);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry115);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry116);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry117);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry118);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry119);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry120);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry121);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry122);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry123);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry124);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry125);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry126);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry127);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry128);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry129);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry130);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry131);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry132);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry133);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry134);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry135);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry136);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry137);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry138);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry139);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry140);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry141);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry142);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry143);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry144);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry145);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry146);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry147);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry148);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry149);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry150);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry151);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry152);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry153);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry154);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry155);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry156);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry157);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry158);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry159);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry160);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry161);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry162);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry163);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry164);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry165);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry166);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry167);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry168);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry169);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry170);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry171);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry172);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry173);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry174);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry175);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry176);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry177);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry178);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry179);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry180);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry181);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry182);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry183);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry184);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry185);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry186);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry187);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry188);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry189);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry190);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry191);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry192);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry193);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry194);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry195);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry196);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry197);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry198);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry199);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry200);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry201);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry202);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry203);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry204);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry205);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry206);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry207);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry208);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry209);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry210);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry211);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry212);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry213);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry214);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry215);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry216);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry217);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry218);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry219);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry220);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry221);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry222);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry223);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry224);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry225);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry226);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry227);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry228);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry229);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry230);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry231);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry232);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry233);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry234);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry235);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry236);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry237);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry238);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry239);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry240);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry241);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry242);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry243);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry244);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry245);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry246);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry247);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry248);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry249);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry250);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry251);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry252);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry253);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry254);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry255);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry256);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry257);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry258);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry259);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry260);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry261);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry262);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry263);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry264);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry265);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry266);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry267);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry268);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry269);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry270);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry271);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry272);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry273);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry274);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry275);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry276);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry277);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry278);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry279);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry280);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry281);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry282);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry283);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry284);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry285);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry286);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry287);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry288);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry289);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry290);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry291);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry292);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry293);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry294);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry295);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry296);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry297);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry298);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry299);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry300);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry301);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry302);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry303);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry304);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry305);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry306);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry307);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry308);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry309);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry310);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry311);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry312);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry313);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry314);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry315);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry316);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry317);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry318);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry319);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry320);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry321);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry322);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry323);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry324);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry325);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry326);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry327);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry328);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry329);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry330);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry331);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry332);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry333);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry334);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry335);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry336);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry337);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry338);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry339);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry340);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry341);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry342);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry343);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry344);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry345);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry346);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry347);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry348);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry349);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry350);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry351);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry352);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry353);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry354);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry355);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry356);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry357);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry358);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry359);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry360);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry361);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry362);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry363);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry364);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry365);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry366);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry367);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry368);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry369);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry370);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry371);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry372);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry373);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry374);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry375);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry376);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry377);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry378);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry379);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry380);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry381);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry382);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry383);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry384);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry385);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry386);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry387);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry388);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry389);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry390);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry391);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry392);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry393);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry394);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry395);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry396);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry397);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry398);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry399);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry400);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry401);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry402);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry403);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry404);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry405);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry406);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry407);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry408);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry409);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry410);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry411);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry412);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry413);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry414);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry415);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry416);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry417);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry418);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry419);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry420);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry421);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry422);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry423);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry424);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry425);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry426);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry427);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry428);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry429);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry430);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry431);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry432);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry433);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry434);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry435);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry436);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry437);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry438);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry439);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry440);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry441);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry442);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry443);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry444);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry445);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry446);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry447);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry448);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry449);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry450);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry451);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry452);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry453);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry454);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry455);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry456);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry457);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry458);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry459);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry460);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry461);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry462);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry463);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry464);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry465);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry466);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry467);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry468);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry469);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry470);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry471);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry472);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry473);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry474);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry475);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry476);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry477);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry478);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry479);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry480);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry481);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry482);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry483);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry484);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry485);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry486);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry487);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry488);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry489);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry490);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry491);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry492);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry493);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry494);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry495);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry496);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry497);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry498);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry499);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry500);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry501);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry502);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry503);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry504);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry505);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry506);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry507);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry508);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry509);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry510);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry511);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry512);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry513);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry514);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry515);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry516);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry517);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry518);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry519);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry520);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry521);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry522);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry523);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry524);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry525);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry526);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry527);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry528);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry529);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry530);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry531);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry532);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry533);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry534);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry535);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry536);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry537);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry538);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry539);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry540);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry541);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry542);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry543);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry544);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry545);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry546);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry547);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry548);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry549);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry550);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry551);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry552);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry553);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry554);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry555);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry556);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry557);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry558);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry559);

INCLUDE_SDATA(const s32, "code_001E5980", mnuLoadTitleSoundBufferEntryData3Entry560);

INCLUDE_SDATA(const s32, "code_001E5980", sndBgmLoadId_XFData2);

INCLUDE_SDATA(const s32, "code_001E5980", sndData2);

