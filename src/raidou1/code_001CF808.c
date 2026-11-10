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

INCLUDE_ASM(const s32, "code_001CF808", func_001CF838);

INCLUDE_ASM(const s32, "code_001CF808", func_001CF8F8);

INCLUDE_ASM(const s32, "code_001CF808", func_001CF900);

INCLUDE_ASM(const s32, "code_001CF808", func_001CF930);

INCLUDE_ASM(const s32, "code_001CF808", func_001CF958);

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

INCLUDE_ASM(const s32, "code_001CF808", func_001CFA58);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFAF8);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFB30);

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

INCLUDE_ASM(const s32, "code_001CF808", func_001CFBD8);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFC50);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFC98);

INCLUDE_ASM(const s32, "code_001CF808", sndOpStopNamedPlayback);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFD08);

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

INCLUDE_ASM(const s32, "code_001CF808", func_001CFDA0);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFDC8);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFDD0);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFE88);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFEB0);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFEE0);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFF08);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFF30);

u8 mnuIsTitleStreamIdle(void) {
    MnuTitleStreamLoadState loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

INCLUDE_ASM(const s32, "code_001CF808", func_001CFF78);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFFD0);

INCLUDE_ASM(const s32, "code_001CF808", func_001CFFF0);

INCLUDE_ASM(const s32, "code_001CF808", func_001D0010);

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

INCLUDE_ASM(const s32, "code_001CF808", func_001D0150);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "code_001CF808", func_001D0268);
INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD08);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD0C);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD10);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD18);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD20);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD28);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD2C);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD30);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD38);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD40);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD44);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD48);

INCLUDE_SDATA(const s32, "code_001CF808", D_0046CD4C);

