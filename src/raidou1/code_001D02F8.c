#include "common.h"

/* Declarations from dds-decomp src/dds1/game/code_00269C10.c */
#ifndef VERSION_DDS1
#define VERSION_DDS1 1
#endif

#include "fileManager.h"

#include "libsdf.h"

#include "sdf_resource.h"

#include "sdk_libc.h"

#include "dds/btl.h"

#include "dds/dds3_world_counter.h"

#include "dds/dds3_world_secondary.h"

#include "dds/dds3_world_value.h"

#include "dds/file_request_api.h"

#include "dds/sdf_heap_api.h"

#include "dds/sdf_semaphore_api.h"

#include "dds/sdf_thread_sleep.h"

#include "dds/sdk_libc.h"

#include "dds/sdk_libkernl.h"

s32 SignalSema(s32 semaphoreId);

s32 WaitSema(s32 semaphoreId);

/* Advance the wrapping 16-bit world object sequence carried in a u32. */
u32 dds3AllocObjectId(void);

/* Return the borrowed secondary world node, or NULL when no world is active. */
struct dds3Basic_t *dds3GetControlScene(void);

/* Store objectName directly as the node's lookup key; no copy is made.
 * A NULL node is ignored and a NULL name clears the key. Keep non-NULL names
 * alive while the node can be searched. */
void dds3SetUnitObjectId(struct dds3Basic_t *node, const char *objectName);

/* Return the address of the loaded resource payload as a byte pointer. */
u8 *fileGetLoadPtr(const struct FileRequest *request);

/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request);

/* Return the payload allocation descriptor. For normal file loads, freeing the
 * request wrapper does not free this allocation; its resource owner must keep
 * or release it separately. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request);

/* Free a completed request wrapper; return one while it remains pending. */
FileRequestCleanupResult fileHandleFree(struct FileRequest *request);

extern void *memcpy(void *, const void *, unsigned int);

void *memset(void *dst, s32 value, u32 size);

s32 pacFileCheckEnd(const struct FileRequest *request);

s32 sdfCreateSema(u32 initialCount, u32 option, u32 maximumCount);

struct sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

struct sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 requestedBytes);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation);

void sdfMemFreeDirect(struct sdfMemHandle_t *allocation);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct sdfMemHandle_t *block);

s32 sdfMemGetSize(const struct sdfMemHandle_t *block);

/* Record the current thread, lower its priority for sleep, and return the
 * result from the SDK SleepThread call. */
s32 sdfSetMainThreadPriority(void);

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

INCLUDE_ASM(const s32, "code_001D02F8", sndUploadStreamToBothIopBuffers);

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

INCLUDE_ASM(const s32, "code_001D02F8", func_001D0400);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D04C0);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D0E50);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D0ED0);

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

INCLUDE_ASM(const s32, "code_001D02F8", mnuLoadTitleStreamFrameData);

INCLUDE_ASM(const s32, "code_001D02F8", mnuStoreTaskResult);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D11D0);

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

INCLUDE_ASM(const s32, "code_001D02F8", func_001D12F8);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1458);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D14D8);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1570);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1630);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1708);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D17F0);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1848);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1878);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1928);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D19A0);

void mnuResetTitleStreamLocked(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1A30);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1A60);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454CE0);

INCLUDE_ASM(const s32, "code_001D02F8", mnuInitializeTitleSoundBuffer);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1BC8);

INCLUDE_ASM(const s32, "code_001D02F8", mnuLoadTitleSoundBufferEntry);

INCLUDE_ASM(const s32, "code_001D02F8", mnuInstallTitleSoundBufferFromData);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D1FC8);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D2028);

INCLUDE_ASM(const s32, "code_001D02F8", func_001D20C8);

void mnuResetSoundBuffer(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
}

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454D10);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454D30);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454D50);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454D70);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454D90);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454DB0);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454DD0);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454DF0);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454E08);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454E28);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454E48);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454E68);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454E88);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454EA8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454EC8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454EE8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454F08);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454F28);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454F48);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454F68);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454F88);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454FA8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454FC8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00454FE8);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00455008);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00455028);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00455048);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00455068);

INCLUDE_RODATA(const s32, "code_001D02F8", D_00455088);

INCLUDE_RODATA(const s32, "code_001D02F8", D_004550A8);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD50);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD54);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD58);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD5C);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD60);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD64);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD68);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD6C);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD70);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD74);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD78);

INCLUDE_SDATA(const s32, "code_001D02F8", D_0046CD7C);

