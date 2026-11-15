#include "common.h"

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

/* The voice-file RPC sets this busy state on open and clears it on close.
 * It is separate from the PCM callback-command busy latch. */
MnuTitleSoundBusyState p2sdGetPcmState(void);

/* Returns a borrowed read-only string for a STRING-tagged parameter, or NULL. */
const char *scrGetStrPara(ScrCommandParameterIndex parameterIndex);

#include "dds/fld_area_work.h"
#include "dds/common.h"
#include "dds/evt_solar.h"
#include "dds/fld_scene_selection_resource.h"

u32 fldCmdGetCurrentSceneSelectionResource(void);

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommand_FLD_GET_KIRETA(void);

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void);
extern s32 p2sdLoadPcmFile(const char *name, u8 channel);
extern void p2sdSyncSound(void);
s32 fldCommandSendNamedSound(void);
u32 fldCommandSendSoundControl(void);

/* Return sound-command busy state to the field script interpreter. */
u32 fldCommandIsSoundBusy(void);

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void);
s32 fldCmdSetStateFlag(void);

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
s32 sceSifAllocIopHeap(s32 size);
s32 sceSifDmaStat(s32 transferId);
s32 sceSifInitIopHeap(void);
struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);
char *strcat(char *dst, const char *src);

#include "dds/sdk_ps2.h"
#include "dds/pty_experience_threshold_type.h"
#include "dds/dds3_world_selection.h"
#include "dds/dds3_world_value.h"
#include "dds/mnu_sound_resource_path_api.h"
#include "dds/mnu_voice_resource_path_api.h"
#include "dds/mnu_result.h"
#include "dds/brs_progress_animation_kind.h"
#include "dds/brs_result_counter.h"
#include "dds/brs_result_counter_completion_api.h"
#include "dds/dds3_world_counter.h"
#include "dds/snd_work.h"
#include "dds/prf_current_record_view.h"

extern void mnuTitleStreamUpdateAndLogBgm(void);

#include "dds/sdf.h"

/* The four sprite corner colours start from the shared grey table. */
typedef struct BrsCornerColors {
    u32 rgba[4];
} BrsCornerColors;

#include "dds/eff.h"
#include "dds/fr_font_draw_glyph.h"

typedef struct BrsLevelCornerColors {
    u32 values[4];
} BrsLevelCornerColors;

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

struct dds3Basic_t;

extern u32 D_003BC5BC;

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC598[];
extern dds3ProcessID_t *mnuTitleSoundTask;

#include "dds/sdf_debug_api.h"

extern u32 D_003BC5B0[2];
extern u32 D_003BC5B8;
extern s32 sndStreamTransferIopHeapAddress;

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounterView levelCounters[8];
    BrsResultCounterView profileCounters[8];
} BrsResultWork;

#include "dds/prf_profile_cap.h"

typedef struct BrsProfilePalette { u32 values[4]; } BrsProfilePalette;

char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename);
void mnuResetTitleEffectState(s32 command);
void mnuSetTitleVoicePrefixIndex(s32 prefixIndex);
void scrGetAfterSkipBgmFlag(void);
MnuTitleSoundBusyState scrCheckEnableMovieCancel(void);
void mnuNoOpTitleAudio(void);
s32 mnuGetTitleEffectFrameCounter(void);
u32 sndOpStartTrackFromScript(void);
u32 sndOpSetTrackDefaultVolumePan(void);
u32 sndOpSendTitleAudioCommand(void);
s32 movCheckStartupSoundState(void);
u32 sndOpUpdateTitleBgmState(void);
u32 mnuResetTitlePlaybackCommands(void);
u32 mnuAdvanceTitleStreamFromScript(void);
u8 mnuIsTitleStreamIdle(void);
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount);
void sndInitializeStreamTransferBuffers(s32 wordCount);
void mnuInitTitleSoundRemoteRequest(u32 wordCount);

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
}

INCLUDE_ASM(const s32, "code_001CF808", sndInit);

INCLUDE_ASM(const s32, "code_001CF808", sndLeaf);

INCLUDE_ASM(const s32, "code_001CF808", mnuIncrementTitleEffectFrameCounter);

INCLUDE_ASM(const s32, "code_001CF808", mnuDestroyTitleEffectTask);

INCLUDE_ASM(const s32, "code_001CF808", ProcessInit);

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

INCLUDE_ASM(const s32, "code_001CF808", fldNowPlayengStart);

INCLUDE_ASM(const s32, "code_001CF808", fldNKIY);

INCLUDE_ASM(const s32, "code_001CF808", fldCheckNextFile);

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

INCLUDE_ASM(const s32, "code_001CF808", scrCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_001CF808", scrCallGetIntPara7);

INCLUDE_ASM(const s32, "code_001CF808", scrCallGetIntPara8);

INCLUDE_ASM(const s32, "code_001CF808", sndOpStopNamedPlayback);

INCLUDE_ASM(const s32, "code_001CF808", slCallGetIntPara);

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

INCLUDE_ASM(const s32, "code_001CF808", slCallGetPcmState);

INCLUDE_ASM(const s32, "code_001CF808", slLeaf3);

INCLUDE_ASM(const s32, "code_001CF808", slHardSelect1);

INCLUDE_ASM(const s32, "code_001CF808", slCallBgmPlay);

INCLUDE_ASM(const s32, "code_001CF808", evtCloseSkyEventTask);

INCLUDE_ASM(const s32, "code_001CF808", At3Play);

INCLUDE_ASM(const s32, "code_001CF808", evtCallBgmFadeOut);

INCLUDE_ASM(const s32, "code_001CF808", evtCallBgmContinue);

u8 mnuIsTitleStreamIdle(void) {
    MnuTitleStreamLoadState loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

INCLUDE_ASM(const s32, "code_001CF808", evtCallSePlay);

INCLUDE_ASM(const s32, "code_001CF808", evtFunc7);

INCLUDE_ASM(const s32, "code_001CF808", evtCallEffectShutdown);

INCLUDE_ASM(const s32, "code_001CF808", evtCallSeFadeOut);

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

INCLUDE_ASM(const s32, "code_001CF808", evtFunc8);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "code_001CF808", mnuCopySoundStreamOnStatusChange);
INCLUDE_SDATA(const s32, "code_001CF808", evtPicturePriList);

INCLUDE_SDATA(const s32, "code_001CF808", sndInitData2);

INCLUDE_SDATA(const s32, "code_001CF808", sndPathPrefix);

INCLUDE_SDATA(const s32, "code_001CF808", sndPathPrefix2);

INCLUDE_SDATA(const s32, "code_001CF808", evtPicturePriProcName);

INCLUDE_SDATA(const s32, "code_001CF808", fldNKIYData);

INCLUDE_SDATA(const s32, "code_001CF808", sndInitData3);

INCLUDE_SDATA(const s32, "code_001CF808", sndInitData4);

INCLUDE_SDATA(const s32, "code_001CF808", sndData);

INCLUDE_SDATA(const s32, "code_001CF808", sndInitializeStreamTransferBuffersData);

INCLUDE_SDATA(const s32, "code_001CF808", mnuCopySoundStreamOnStatusChangeData);

INCLUDE_SDATA(const s32, "code_001CF808", sndBgmLoadId_XFData2);

INCLUDE_SDATA(const s32, "code_001CF808", sndData2);

