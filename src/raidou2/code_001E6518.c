#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00269C10.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "dds/sdk_ps2.h"

#include "dds/dds3_world_counter.h"

#include "dds/common.h"

#include "dds/sdf_semaphore_api.h"

#include "dds/mnu_result.h"

#include "dds/sdf_chip.h"

#include "dds/dds3obj.h"

#include "dds/dds3_world_selection.h"

#include "dds/dds3_world_value.h"

#include "dds/sdf_heap_api.h"

#include "dds/kwln.h"

#include "dds/mnu_title_effect.h"

#include "dds/file.h"

#include "dds/file_request_api.h"

#include "dds/file_request_end_check_api.h"

#include "dds/sdf_thread.h"

#include "dds/sdf_thread_sleep.h"

#include "dds/mnu_title_stream.h"

#include "dds/mnu_title_stream_status_snapshot.h"

#include "dds/mnu_title_stream_entry.h"

#include "dds/mnu_title_sound.h"

#include "dds/mnu_title_sound_buffer_status.h"

#define SND_MIX_SAMPLE_COUNT 0x800

#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_SOUND_PATH_BYTES 32

#define MNU_SOUND_FRAME_BYTES_LARGE 0x180

#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0

#define MNU_SOUND_FRAME_BYTES_SMALL 0x60

#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400

#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200

#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 counter, u128 *position, u128 *rotation);

extern void dds3GetRadiusPoseSet(dds3Basic_t *object, f32 value);

extern u8 mnuTitleCameraPosition[];

extern u8 mnuTitleCameraRotation[];

extern dds3Basic_t *mnuTitleCameraObject;

extern char D_003AFD48[];

extern TitleAudioStreamState mnuTitleSoundBufferState;

extern u32 mnuTitleAudioSilenceBuffer[];

extern TitleAudioStreamState mnuTitleStreamStatus;

extern u32 mnuTitleStreamSemaphore;

extern struct FileRequest *mnuTitleStreamFileRequest;

extern s32 D_003BC5C8;

extern u8 D_003D9178[];

#include "dds/sdf_debug_api.h"

extern u32 D_003BC5B0[2];

extern u32 D_003BC5B8;

extern void mnuInitializeTitleAudioStreams(void);

s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount);

void mnuInitTitleSoundRemoteRequest(u32 wordCount);

s32 mnuCopySoundStreamOnStatusChange(const void *source, u32 words);

/* The argument is a word count, not a source address; both use the EE buffer. */
void sndUploadStreamToBothIopBuffers(u32 wordCount);

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource);

extern s32 D_003BC5C0;

extern s32 D_003BC5C4;

extern s32 D_003BC5C8;

extern s32 D_003BC5CC;

extern u32 mnuTitleStreamDecoderState[];

extern u32 mnuTitleSoundBufferDecoderState[];

extern u32 mnuTitleAudioSilenceBuffer[];

extern u8 mnuTitleAudioMixBuffer[];

extern TitleAudioStreamState mnuTitleStreamStatus;

extern TitleAudioStreamState mnuTitleSoundBufferState;

void mnuResetTitleStream(void);

void mnuReleaseSoundBuffer(void);

extern void mnuConfigureTitleAudioStream(s32 transferSize);

void mnuUpdateTitleAudioStreams(void);

extern void sdfWaitVblank(s32 requestedSleepCount);

void mnuRunTitleStreamThread(void);

extern u32 mnuTitleStreamSemaphore;

extern sdfThCB_t mnuTitleStreamThread __attribute__((section(".sbss")));

extern u8 mnuTitleStreamThreadStack[];

void mnuCreateTitleStreamThread(void);

extern u32 mnuTitleStreamDecoderState[];

extern u32 mnuTitleSoundBufferDecoderState[];

extern u8 mnuTitleStreamDecodedSamples[];

extern u8 mnuTitleSoundBufferDecodedSamples[];

void mnuInitializeTitleAudioStreams(void);

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusSnapshot);

extern u32 mnuTitleStreamSemaphore;

/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusValues);

void mnuLoadTitleStreamFrameData(char *filePath, TitleAudioStreamState *streamState);

void mnuStoreTaskResult(char *audioPath);

extern u32 mnuTitleStreamDecoderState[];

extern void at3decReset(u32 *);

/* Allocate in global status, but use the supplied state's copy destination
 * and frame counts. Keep those distinct accesses and the cleanup-before-copy
 * ordering; return 1 after a ready file is copied, otherwise its ready result. */
s32 mnuCompleteTitleStreamFileLoad(TitleAudioStreamState *destinationState);

/* Install the fixed title track and queue its compressed frames under the lock. */
void mnuStartFixedSoundStream(void);

extern MnuTitleStreamEntry D_00377650[];

extern MnuTitleStreamEntry mnuTitleStreamEntries[];

extern char D_003AFCF0[];

/* Load the named sound stream for the requested entry format under the lock. */
void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex);

/* Poll a pending file without acquiring the semaphore in this entry point. */
MnuTitleStreamLoadState mnuUpdateTitleTransition(void);

/* Poll under the semaphore, but retain the native load-state read after unlock. */
MnuTitleStreamLoadState mnuPollTitleStreamStateLocked(void);

extern void fileSyncAll(void);

void mnuTitleStreamUpdateAndLogBgm(void);

void mnuMarkTitleStreamResetPending(void);

/* Commit the ready load only for control 1, and select the native value 6. */
void mnuAdvanceTitleStateUnderSemaphore(void);

/* Save the pre-transition load-state read before marking the commit complete. */
void mnuCommitTitleStreamReadyState(void);

/* Reset load/data slots only when an allocation is present; otherwise do nothing. */
void mnuResetTitleStream(void);

void mnuResetTitleStreamLocked(void);

extern u32 mnuTitleSoundBufferDecoderState[];

/* Install the default medium-frame buffer and stream under the shared lock. */
void mnuInitializeTitleSoundBuffer(void);

extern char D_003AFCF0[];

/* Each format reserves 600 compressed frames before loading its named stream. */
void mnuLoadTitleSoundBufferEntry(s32 soundEntryIndex);

/* Install an in-memory ATRAC stream and configure the decoder for its frame
 * format while holding the shared sound-buffer semaphore. The native code
 * has no default-format guard or buffer-capacity check. */
void mnuInstallTitleSoundBufferFromData(const void *compressedData, s32 dataBytes, MnuTitleSoundFormat format);

/* This status reports allocation and stream control, not load progress. */
MnuTitleSoundBufferStatus mnuGetSoundBufferStateLocked(void);

void mnuPrintTitleDebugBanner(void);

void mnuResetSoundBuffer(void);

/* Release the allocation handle, not the data address, then clear its slot. */
void mnuReleaseSoundBuffer(void);

void mnuResetSoundBufferLocked(void);

void mnuReleaseSoundBufferLocked(void);

extern void mnuLoadMovieMenuResourceSlots(void);

void mnuLoadTitleMenuAssets(void);

void mnuCreateTitleCameraWorldEntry(void);
/* End of declarations from src/dds1/game/code_00269C10.c */

INCLUDE_ASM(const s32, "code_001E6518", func_001E6518);

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource) {
    s16 *destinationCursor = destinationSamples;
    s16 *sourceCursor = secondSource->samples;
    s32 sampleIndex;

    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        *destinationCursor++ = *sourceCursor++;
    }
    sourceCursor = firstSource->samples;
    destinationCursor -= SND_MIX_SAMPLE_COUNT;
    for (sampleIndex = 0; sampleIndex < SND_MIX_SAMPLE_COUNT; sampleIndex++) {
        s32 mixedSample = *destinationCursor + *sourceCursor++;

        if (mixedSample > SND_MIX_SAMPLE_LIMIT) {
            mixedSample = SND_MIX_SAMPLE_LIMIT;
        }
        if (mixedSample < -SND_MIX_SAMPLE_LIMIT) {
            mixedSample = -SND_MIX_SAMPLE_LIMIT;
        }
        *destinationCursor++ = mixedSample;
    }
}

INCLUDE_ASM(const s32, "code_001E6518", func_001E6620);

INCLUDE_ASM(const s32, "code_001E6518", func_001E66E0);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7128);

INCLUDE_ASM(const s32, "code_001E6518", func_001E71A8);

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusSnapshot) {
    WaitSema(mnuTitleStreamSemaphore);
    statusSnapshot->frameCount = mnuTitleStreamStatus.frameCount;
    statusSnapshot->frameIndex = mnuTitleStreamStatus.frameIndex;
    statusSnapshot->repeatFrame = mnuTitleStreamStatus.repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusValues) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameCount = statusValues->frameCount;
    mnuTitleStreamStatus.frameIndex = statusValues->frameIndex;
    mnuTitleStreamStatus.repeatFrame = statusValues->repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "code_001E6518", mnuLoadTitleStreamFrameData);

INCLUDE_ASM(const s32, "code_001E6518", mnuStoreTaskResult);

INCLUDE_ASM(const s32, "code_001E6518", func_001E74A8);

/* Install the fixed title track and queue its compressed frames under the lock. */
void mnuStartFixedSoundStream(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = mnuTitleStreamDecoderState;
    mnuTitleStreamStatus.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuTitleStreamDecoderState[2] = 2;
    mnuStoreTaskResult("/soundat3/b_bgm2-2.at3");
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "code_001E6518", func_001E75F8);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7760);

INCLUDE_ASM(const s32, "code_001E6518", func_001E77E0);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7878);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7938);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7A10);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7AF8);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7B50);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7B80);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7C30);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7CA8);

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "code_001E6518", func_001E7D38);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7D68);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBAF8);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7E28);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7ED0);

INCLUDE_ASM(const s32, "code_001E6518", func_001E7FD8);

INCLUDE_ASM(const s32, "code_001E6518", func_001E8160);

INCLUDE_ASM(const s32, "code_001E6518", func_001E82E8);

INCLUDE_ASM(const s32, "code_001E6518", func_001E8348);

INCLUDE_ASM(const s32, "code_001E6518", func_001E83E8);

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
}

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBB28);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBB48);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBB68);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBB88);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBBA8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBBC8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBBE8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBC08);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBC28);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBC48);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBC68);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBC88);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBCA8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBCC8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBCE8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBD08);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBD28);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBD48);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBD68);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBD88);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBDA8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBDC8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBDE8);

INCLUDE_RODATA(const s32, "code_001E6518", D_004BBE08);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14D8);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14DC);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14E0);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14E4);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14E8);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14EC);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14F0);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14F4);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14F8);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E14FC);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E1500);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E1504);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E1508);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E150C);

INCLUDE_SDATA(const s32, "code_001E6518", D_004E1510);

