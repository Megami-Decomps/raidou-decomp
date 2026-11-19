#include "common.h"
#include "sdk_libkernl.h"

#include "fileManager.h"
#include "libsdf.h"
#include "sdf_resource.h"
#include "sdk_libc.h"
#include "include_asm.h"
#include "newdata.h"
#include "basic.h"
#include "evt_world.h"
#include "code_r1_001CF808.h"
#include "sndAt3.h"
#include "sdfSystem.h"
#include "sdk_libat3dec.h"
#include "kernel_draw.h"
#include "mviManager.h"
#include "sdfMovie.h"
#include "sdk_libsdr.h"
#include "sdfMemory.h"

/* The three audio stream records (title stream, title sound buffer, and the
 * stream sndBgmLoadId resets); each is followed by its decoder state. */
typedef struct titleAudioStreamState_t {
    s32 frameCount;
    s32 frameIndex;
    s32 frameBytes;
    s32 repeatFrame;
    s32 control;
    u8 *compressedData;
    s16 *samples;
    u32 *decoder;
    struct sdfMemHandle_t *allocation;
    MnuTitleStreamLoadState loadState;
    s32 unk28;
    s32 tableIndex; /* Grown from 0x28: retail addresses +0x2C off all three records. */
} titleAudioStreamState_t;

typedef char titleAudioStreamState_size[(sizeof(titleAudioStreamState_t) == 0x30) ? 1 : -1];

/* One BGM table entry: decoder format, repeat frame and the .at3 base name. */
typedef struct sndBgmEntry_t {
    u8 format;
    s16 repeatFrame;
#ifdef VERSION_RAIDOU1
    char name[12];
#else
    char *name;
#endif
} sndBgmEntry_t;

#ifdef VERSION_RAIDOU1
typedef char sndBgmEntry_size[(sizeof(sndBgmEntry_t) == 0x10) ? 1 : -1];
#else
typedef char sndBgmEntry_size[(sizeof(sndBgmEntry_t) == 0x8) ? 1 : -1];
#endif

/* Shared title-audio stream operations, implemented by each title. */
void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex);

typedef struct titleAudioStreamStatusSnapshot_t {
    u32 frameCount;
    u32 frameIndex;
    u32 repeatFrame;
} titleAudioStreamStatusSnapshot_t;

typedef char titleAudioStreamStatusSnapshot_size[
    (sizeof(titleAudioStreamStatusSnapshot_t) == 0x0C) ? 1 : -1];

#define SND_MIX_SAMPLE_COUNT 0x800

#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_SOUND_PATH_BYTES 32

#define MNU_SOUND_FRAME_BYTES_LARGE 0x180

#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0

#define MNU_SOUND_FRAME_BYTES_SMALL 0x60

#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400

#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200

#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

extern titleAudioStreamState_t mnuTitleSoundBufferState;
extern u32 mnuTitleAudioSilenceBuffer[];
extern titleAudioStreamState_t mnuTitleStreamStatus;
extern s32 evtData25;
extern u32 sndWork3[];
extern u32 mnuTitleStreamSemaphore;

void sndMixSampleBuffers(s16 *destinationSamples, titleAudioStreamState_t *firstSource, titleAudioStreamState_t *secondSource);
extern u32 mnuTitleStreamDecoderState[];
extern u8 sndWork5[0x9000];
extern u8 sndCallMemsetWork[0x1000];
extern u8 sndCallMemsetWork2[0x1000];
extern u8 sndCallMemsetWork3[0x1000];
void mnuResetTitleStream(void);
void mnuReleaseSoundBuffer(void);
void mnuReadTitleStreamStatusLocked(titleAudioStreamStatusSnapshot_t *statusSnapshot);
void mnuWriteTitleStreamStatusLocked(titleAudioStreamStatusSnapshot_t *statusValues);
void File_BgmLoadStart(char *audioPath);
void sndBgmLoadFile(void);
void sndDeadSePlay_VolAngle(void);
void sndCallSeStop(void);
s32 GetVolume(titleAudioStreamState_t *stream);
s32 BgmPlaySub(void);
void sndBgmFadeIn(void);

extern sndBgmEntry_t sndBgmLoadId_XFData[];
extern sndBgmEntry_t mnuQueueTitleSoundStreamEntryData[];
extern sndBgmEntry_t mnuLoadTitleSoundBufferEntryData[];
/* Each BGM's resume frame, by table index. */
extern u16 sndBgmFadeOutWork[];
extern titleAudioStreamState_t sndCallSeStopWork6;
extern u32 sndCallSeStopWork7[];
extern sdfThCB_t sndCallWaitVblankWork;
extern u8 sndCallWaitVblankWork2[0x1000];
extern s32 sndData2;
extern s32 sndBgmLoadId_XFData2;
extern s32 sndCallSeStopData4;
extern s32 sndData5;
extern s32 sndData6;
extern s32 sndData7;

void sndUploadStreamToBothIopBuffers(u32 wordCount);
void sndUploadStreamToBothIopBuffers(u32 wordCount) {
    sndCopyWordsToIopSynchronously((const void *)sndInitializeStreamTransferBuffersData, sndData[0], wordCount);
    sndCopyWordsToIopSynchronously((const void *)sndInitializeStreamTransferBuffersData, sndData[1], wordCount);
    sceSdRemote(1, 0x8010, 0xF81, 0);
    sceSdRemote(1, 0x8010, 0x1081, 0);
    sceSdRemote(1, 0x80E0, 1, 2, 0, 0);
}

/* Copy the second source, add the first, and clamp to [-32767, 32767].
 * The native lower limit intentionally excludes the s16 value -32768. */
void sndMixSampleBuffers(s16 *destinationSamples, titleAudioStreamState_t *firstSource, titleAudioStreamState_t *secondSource) {
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

INCLUDE_ASM(const s32, "sndAt3", sndLeaf2);

/* NON_MATCHING: 181 of 611 words match. R1 only (R2 body is 0xa44 bytes, differs); retail expands memcpy inline, mine calls memcpy; third voice stream state at sndCallSeStopWork6. */
#ifdef VERSION_RAIDOU1
#ifdef NON_MATCHING
extern s32 sndCallSeStopData2;
extern s32 sndCallSeStopData3;
extern s32 sndData3;
extern s32 sndCallSeStopData4;
extern s32 sndData4;
extern s32 sndCallSeStopData5;
extern s32 sndCallSeStopData6;
extern s32 sndCallSeStopData7;
extern s16 sndCallSeStopData[];
extern titleAudioStreamState_t sndCallSeStopWork6;
/* Copies base's samples to destinationSamples, then adds first's and second's, clamped. */
extern void sndLeaf2(s16 *destinationSamples, titleAudioStreamState_t *first, titleAudioStreamState_t *base,
                     titleAudioStreamState_t *second);

#define MNU_STREAM_COMMIT_READY 3
#define MNU_STREAM_COMMIT_COMPLETE 4
#define MNU_STREAM_STATE_5 5
#define MNU_STREAM_STATE_6 6
#define MNU_STREAM_STATE_7 7

/* Per-frame update of the title audio: the stream, the sound buffer and a third voice stream. */
void sndCallSeStop(void) {
    titleAudioStreamState_t *voice = &sndCallSeStopWork6;
    u32 *voiceDecoder = sndCallSeStopWork7;
    s16 *mix = sndCallSeStopData;

    sndCallSeStopData7++;
    if (mnuTitleStreamStatus.control == 1) {
        if (mnuTitleStreamStatus.loadState == MNU_STREAM_STATE_5) {
            if (mnuTitleStreamDecoderState[4] != 0) {
                if (sndCallSeStopData6 + 1 >= sndData3) {
                    sndCallSeStopData6 = 0;
                    mnuTitleStreamDecoderState[4]--;
                } else {
                    sndCallSeStopData6++;
                }
            } else {
                sndCallSeStopData6++;
                mnuTitleStreamStatus.loadState = MNU_STREAM_COMMIT_READY;
                mnuTitleStreamDecoderState[4] = 0;
            }
        } else {
            sndCallSeStopData6++;
        }
        if (mnuTitleStreamStatus.control == 1) {
            if (mnuTitleStreamStatus.loadState == MNU_STREAM_COMMIT_COMPLETE) {
                if (mnuTitleStreamDecoderState[4] >= 0x1F) {
                    sndCallSeStopData5++;
                    mnuTitleStreamDecoderState[4] = 0;
                    mnuTitleStreamStatus.control = 2;
                    mnuTitleStreamStatus.frameIndex = 0;
                    mnuResetTitleStream();
                } else if (sndCallSeStopData5 + 1 >= sndCallSeStopData4) {
                    sndCallSeStopData5 = 0;
                    mnuTitleStreamDecoderState[4]++;
                } else {
                    sndCallSeStopData5++;
                }
            } else {
                sndCallSeStopData5++;
            }
        } else {
            sndCallSeStopData5++;
        }
    } else {
        sndCallSeStopData5++;
        sndCallSeStopData6++;
    }

    if (voice->control == 1 && voice->loadState == MNU_STREAM_STATE_6) {
        if (voiceDecoder[4] >= 0x1F) {
            voiceDecoder[4] = 0;
            voice->control = 2;
            voice->frameIndex = 0;
        } else if (sndCallSeStopData5 >= sndCallSeStopData4) {
            sndCallSeStopData5 = 0;
            voiceDecoder[4]++;
        }
    }
    if (mnuTitleSoundBufferState.control == 1 && mnuTitleSoundBufferState.loadState == MNU_STREAM_COMMIT_COMPLETE) {
        if (sndWork3[4] >= 0x1F) {
            sndWork3[4] = 0;
            mnuTitleSoundBufferState.control = 2;
            mnuTitleSoundBufferState.frameIndex = 0;
            mnuReleaseSoundBuffer();
        } else if (sndCallSeStopData7 >= sndData4) {
            sndCallSeStopData7 = 0;
            sndWork3[4]++;
        }
    }

    if (mnuTitleStreamStatus.control == 0 || (sndCallSeStopData2 != 0 && mnuTitleStreamStatus.control == 1)) {
        mnuTitleStreamDecoderState[0] =
            (u32)(mnuTitleStreamStatus.compressedData + mnuTitleStreamStatus.frameBytes * mnuTitleStreamStatus.frameIndex);
        at3DecDecode(mnuTitleStreamDecoderState);
    }
    if (mnuTitleStreamStatus.control == 1) {
        mnuTitleStreamStatus.samples = (s16 *)mnuTitleStreamDecoderState[1];
    } else {
        mnuTitleStreamStatus.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    }
    if (mnuTitleSoundBufferState.control == 0 || (sndCallSeStopData2 != 0 && mnuTitleSoundBufferState.control == 1)) {
        sndWork3[0] = (u32)(mnuTitleSoundBufferState.compressedData +
                            mnuTitleSoundBufferState.frameBytes * mnuTitleSoundBufferState.frameIndex);
        at3DecDecode(sndWork3);
    }
    if (mnuTitleSoundBufferState.control == 1) {
        mnuTitleSoundBufferState.samples = (s16 *)sndWork3[1];
    } else {
        mnuTitleSoundBufferState.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    }
    if (voice->control == 0 || (sndCallSeStopData2 != 0 && voice->control == 1)) {
        voiceDecoder[0] = (u32)(voice->compressedData + voice->frameBytes * voice->frameIndex);
        at3DecDecode(voiceDecoder);
    }
    if (voice->control == 1) {
        voice->samples = (s16 *)voiceDecoder[1];
    } else {
        voice->samples = (s16 *)mnuTitleAudioSilenceBuffer;
    }

    if (mnuTitleStreamStatus.control == 2 && voice->control == 2 && mnuTitleSoundBufferState.control == 2) {
        sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mnuTitleAudioSilenceBuffer, 0x400);
        memcpy(mnuTitleAudioSilenceBuffer, mnuTitleStreamStatus.samples, 0x1000);
        if (sndCallSeStopData3 != 0) {
            sndUploadStreamToBothIopBuffers(0x400);
        }
        sndCallSeStopData3 = 0;
        return;
    }

    if (sndCallSeStopData3 == 0) {
        evtFunc8(1024);
    }
    sndCallSeStopData3 = 1;
    if (mnuTitleStreamStatus.control != 2 && (voice->control == 2 || voice->control == 0) &&
        mnuTitleSoundBufferState.control == 2) {
        sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mnuTitleStreamStatus.samples, 0x400);
        memcpy(mix, mnuTitleStreamStatus.samples, 0x1000);
    } else if (mnuTitleStreamStatus.control == 0 && voice->control == 1) {
        if (mnuTitleSoundBufferState.control != 2) {
            sndMixSampleBuffers(mix, &mnuTitleSoundBufferState, voice);
            sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mix, 0x400);
        } else {
            sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(voice->samples, 0x400);
            memcpy(mix, voice->samples, 0x1000);
        }
    } else if (mnuTitleStreamStatus.control == 2 && voice->control == 1) {
        if (mnuTitleSoundBufferState.control != 2) {
            sndMixSampleBuffers(mix, &mnuTitleSoundBufferState, voice);
            sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mix, 0x400);
        } else {
            sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(voice->samples, 0x400);
            memcpy(mix, voice->samples, 0x1000);
        }
    } else if (mnuTitleStreamStatus.control != 2 && voice->control == 1) {
        if (mnuTitleSoundBufferState.control != 2) {
            sndLeaf2(mix, &mnuTitleSoundBufferState, voice, &mnuTitleStreamStatus);
        } else {
            sndMixSampleBuffers(mix, voice, &mnuTitleStreamStatus);
        }
        sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mix, 0x400);
    } else {
        sndMixSampleBuffers(mix, &mnuTitleSoundBufferState, &mnuTitleStreamStatus);
        sndCallSeStopData2 = mnuCopySoundStreamOnStatusChange(mix, 0x400);
    }

    if (mnuTitleStreamStatus.control == 0) {
        mnuTitleStreamStatus.control = 1;
        sndCallSeStopData2 = 1;
    }
    if (sndCallSeStopData2 != 0 && mnuTitleStreamStatus.control == 1) {
        if (mnuTitleStreamStatus.loadState == MNU_STREAM_STATE_7 ||
            (++mnuTitleStreamStatus.frameIndex >= mnuTitleStreamStatus.frameCount &&
             mnuTitleStreamStatus.repeatFrame == -1)) {
            mnuTitleStreamStatus.frameIndex = 0;
            mnuTitleStreamStatus.control = 2;
            mnuResetTitleStream();
        } else if (mnuTitleStreamStatus.frameIndex >= mnuTitleStreamStatus.frameCount) {
            mnuTitleStreamStatus.frameIndex = mnuTitleStreamStatus.repeatFrame;
        }
    }

    if (mnuTitleSoundBufferState.control == 0) {
        mnuTitleSoundBufferState.control = 1;
        sndCallSeStopData2 = 1;
    }
    if (sndCallSeStopData2 != 0) {
        if (mnuTitleSoundBufferState.control == 1 &&
            ++mnuTitleSoundBufferState.frameIndex >= mnuTitleSoundBufferState.frameCount) {
            mnuTitleSoundBufferState.frameIndex = 0;
            mnuTitleSoundBufferState.control = 2;
            mnuReleaseSoundBuffer();
        }
    }

    if (voice->control == 0) {
        voice->control = 1;
        sndCallSeStopData2 = 1;
    }
    if (sndCallSeStopData2 != 0 && voice->control == 1 && ++voice->frameIndex >= voice->frameCount) {
        voice->control = 2;
        voice->samples = (s16 *)mnuTitleAudioSilenceBuffer;
        voice->loadState = 0;
        voice->frameIndex = 0;
    }
}
#else
INCLUDE_ASM(const s32, "sndAt3", sndCallSeStop);
#endif
#else
INCLUDE_ASM(const s32, "sndAt3", sndCallSeStop);
#endif

/* The title stream thread: one frame of stream work per vblank, under the lock. */
void sndCallWaitVblank(void) {
    for (;;) {
        sdfWaitVblank(1);
        WaitSema(mnuTitleStreamSemaphore);
        sndCallSeStop();
        SignalSema(mnuTitleStreamSemaphore);
    }
}

void sndCallWaitVblankLabel(void) {
    mnuTitleStreamSemaphore = sdfCreateSema(1, 0xFF, 0);
    sdfStartThreadAndWait(&sndCallWaitVblankWork, sndCallWaitVblank, sndCallWaitVblankWork2, 0x1000,
                          0x45, 0);
    sdfSetMainThreadPriority();
}

void sndCallMemset(void) {
    memset(mnuTitleAudioSilenceBuffer, 0, 0x1000);

    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.loadState = 0;
    mnuTitleStreamStatus.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleStreamStatus.allocation = NULL;
    mnuTitleStreamStatus.compressedData = NULL;
    mnuTitleStreamStatus.decoder = mnuTitleStreamDecoderState;
    mnuTitleStreamStatus.tableIndex = 0;

    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.repeatFrame = 0;
    mnuTitleSoundBufferState.loadState = 0;
    mnuTitleSoundBufferState.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleSoundBufferState.allocation = NULL;
    mnuTitleSoundBufferState.compressedData = NULL;
    mnuTitleSoundBufferState.decoder = sndWork3;
    mnuTitleSoundBufferState.tableIndex = 0;

    sndCallSeStopWork6.control = 2;
    sndCallSeStopWork6.repeatFrame = 0;
    sndCallSeStopWork6.loadState = 0;
    sndCallSeStopWork6.allocation = NULL;
    sndCallSeStopWork6.decoder = sndCallSeStopWork7;
    sndCallSeStopWork6.compressedData = sndWork5;
    sndCallSeStopWork6.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    sndCallSeStopWork7[2] = 2;
    sndCallSeStopWork6.frameBytes = 192;
    sndCallSeStopWork6.frameCount = sizeof(sndWork5) / sndCallSeStopWork6.frameBytes;
    sndCallSeStopWork6.frameIndex = 0;
    sndCallSeStopWork6.tableIndex = 0;

    mnuInitTitleSoundRemoteRequest(0x400);

    mnuTitleStreamDecoderState[1] = (u32)sndCallMemsetWork;
    mnuTitleStreamDecoderState[3] = (u32)sdfMemGetPtr(sdfMemAllocDirect(26826));
    mnuTitleStreamDecoderState[4] = 0;
    sndWork3[1] = (u32)sndCallMemsetWork2;
    sndWork3[3] = (u32)sdfMemGetPtr(sdfMemAllocDirect(26826));
    sndWork3[4] = 2;
    sndCallSeStopWork7[1] = (u32)sndCallMemsetWork3;
    sndCallSeStopWork7[3] = (u32)sdfMemGetPtr(sdfMemAllocDirect(26826));
    sndCallSeStopWork7[4] = 0;
    at3DecInit(sndCallSeStopWork7);

    memset(sndBgmFadeOutWork, 0, 0xC0);
    sndCallWaitVblankLabel();
}

/* Read state words 0, 1 and 3; the frame-byte word 2 is not in this snapshot. */
void mnuReadTitleStreamStatusLocked(titleAudioStreamStatusSnapshot_t *statusSnapshot) {
    WaitSema(mnuTitleStreamSemaphore);
    statusSnapshot->frameCount = mnuTitleStreamStatus.frameCount;
    statusSnapshot->frameIndex = mnuTitleStreamStatus.frameIndex;
    statusSnapshot->repeatFrame = mnuTitleStreamStatus.repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

/* Restore the same condensed three-word status mapping under the lock. */
void mnuWriteTitleStreamStatusLocked(titleAudioStreamStatusSnapshot_t *statusValues) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.frameCount = statusValues->frameCount;
    mnuTitleStreamStatus.frameIndex = statusValues->frameIndex;
    mnuTitleStreamStatus.repeatFrame = statusValues->repeatFrame;
    SignalSema(mnuTitleStreamSemaphore);
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00269C10.c:321, rework, same function 0.92 (words 32/32, calls 0.75, shape 1.00) */
void mnuLoadTitleStreamFrameData(char *filePath, titleAudioStreamState_t *streamState) {
    u32 resolvedAddress;
    s32 frameCount;
    struct sdfMemHandle_t *resourceHandle = sdfDevLoadFileEx(filePath, &resolvedAddress, 0);
    s32 fileBytes = sdfMemGetSize(resourceHandle);
    memcpy(streamState->compressedData, (void *)resolvedAddress, fileBytes);
    frameCount = fileBytes / streamState->frameBytes;
    streamState->frameIndex = 0;
    streamState->frameCount = frameCount;
    sdfMemFreeDirect(resourceHandle);
}

void File_BgmLoadStart(char *audioPath) {
    GetVolumeWork = fileLoadUseTail(audioPath);
    mnuTitleStreamStatus.loadState = 1;
}

#ifdef VERSION_RAIDOU1
s32 GetVolume(titleAudioStreamState_t *stream) {
    sdfMemHandle_t *handle;
    u8 *data;
    s32 size;

    if (pacFileCheckEnd(GetVolumeWork) == 0) {
        return 0;
    }
    handle = fileGetMemHandle(GetVolumeWork);
    data = fileGetLoadPtr(GetVolumeWork);
    size = fileGetLoadSize(GetVolumeWork);
    fileHandleFree(GetVolumeWork);
    mnuTitleStreamStatus.compressedData = data;
    mnuTitleStreamStatus.allocation = handle;
    stream->frameCount = size / stream->frameBytes;
    stream->frameIndex = 0;
    at3DecInit(mnuTitleStreamDecoderState);
    mnuTitleStreamStatus.loadState = 2;
    return 1;
}
#else
INCLUDE_ASM(const s32, "sndAt3", GetVolume);
#endif

/* Install the fixed title track and queue its compressed frames under the lock. */
void sndBgmLoadFile(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.decoder = mnuTitleStreamDecoderState;
    mnuTitleStreamStatus.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleStreamStatus.control = 2;
    mnuTitleStreamStatus.repeatFrame = 0;
    mnuTitleStreamStatus.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuTitleStreamDecoderState[2] = 2;
    File_BgmLoadStart("/soundat3/b_bgm2-2.at3");
    SignalSema(mnuTitleStreamSemaphore);
}

/* Start loading BGM id; reuses the loaded track when id is already current. */
/* Residual: 57 of 86 words differ: register allocation and store scheduling around the table lookup. */
#ifdef NON_MATCHING
void sndBgmLoadId(s32 id) {
    char path[MNU_TITLE_SOUND_PATH_BYTES];
    s32 frameBytes;
    s32 format;
    titleAudioStreamState_t *status;

    sndData5 = id;
    if (sndData6 == id) {
        sndData7 = 1;
        return;
    }
    sndData7 = 0;
    WaitSema(mnuTitleStreamSemaphore);
    status = &mnuTitleStreamStatus;
    if (status->loadState != 0) {
        status->frameIndex = 0;
        status->control = 2;
        mnuResetTitleStream();
        sndCallSeStopWork6.control = 2;
        sndCallSeStopWork6.samples = (s16 *)mnuTitleAudioSilenceBuffer;
        sndCallSeStopWork6.loadState = 0;
        sndCallSeStopWork6.frameIndex = 0;
    }
    format = sndBgmLoadId_XFData[id].format;
    status->samples = (s16 *)mnuTitleAudioSilenceBuffer;
    status->repeatFrame = sndBgmLoadId_XFData[id].repeatFrame;
    sndData6 = id;
    status->decoder = mnuTitleStreamDecoderState;
    status->control = 2;
    status->tableIndex = id;
    switch (format) {
    case 1:
        frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        break;
    case 2:
    case 3:
        frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        break;
    case 4:
        frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        break;
    default:
        goto skip;
    }
    mnuTitleStreamDecoderState[2] = format;
    status->frameBytes = frameBytes;
skip:
    sprintf(path, "/soundat3/%s.at3", sndBgmLoadId_XFData[id].name);
    File_BgmLoadStart(path);
    SignalSema(mnuTitleStreamSemaphore);
}
#else
INCLUDE_ASM(const s32, "sndAt3", sndBgmLoadId);
#endif

s32 BgmPlaySub(void) {
    titleAudioStreamState_t *stream = &mnuTitleStreamStatus;

    if (stream->loadState == 1) {
        GetVolume(stream);
    }
    if (sndData7 == 1) {
        return 2;
    }
    switch (stream->loadState) {
    case 4:
    case 5:
    case 6:
    case 7:
        return 3;
    default:
        return mnuTitleStreamStatus.loadState;
    }
}

s32 sndCallWaitSema2(void) {
    titleAudioStreamState_t *stream = &mnuTitleStreamStatus;
    s32 status;

    WaitSema(mnuTitleStreamSemaphore);
    if (stream->loadState == 1) {
        GetVolume(stream);
    }
    status = 2;
    if (sndData7 != 1) {
        switch (stream->loadState) {
        case 4:
        case 5:
        case 6:
        case 7:
            status = 3;
            break;
        default:
            status = mnuTitleStreamStatus.loadState;
            break;
        }
    }
    SignalSema(mnuTitleStreamSemaphore);
    return status;
}

void sndBgmPlay(void) {
    s32 result;

    if (sndData7 == 1) {
        sndData7 = 0;
        return;
    }
    WaitSema(mnuTitleStreamSemaphore);
    result = BgmPlaySub();
    if (result == 1) {
        fileSyncAll();
        GetVolume(&mnuTitleStreamStatus);
    }
    if (result == 0) {
        SignalSema(mnuTitleStreamSemaphore);
        return;
    }
    if (mnuTitleStreamStatus.control != 1) {
        mnuTitleStreamStatus.control = 0;
        mnuTitleStreamStatus.loadState = 3;
        mnuTitleStreamDecoderState[4] = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndBgmFadeIn);

INCLUDE_ASM(const s32, "sndAt3", sndBgmContinue);

void sndBgmStop(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleStreamStatus.control = 2;
    sndBgmFadeOutWork[mnuTitleStreamStatus.tableIndex] = mnuTitleStreamStatus.frameIndex;
    mnuTitleStreamStatus.frameIndex = 0;
    sndData7 = 0;
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndBgmTop);

void sndBgmFadeOut(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleStreamStatus.control == 1) {
        if (mnuTitleStreamStatus.loadState == 3) {
            mnuTitleStreamStatus.loadState = 4;
            sndBgmFadeOutWork[mnuTitleStreamStatus.tableIndex] = mnuTitleStreamStatus.frameIndex;
            mnuTitleStreamDecoderState[4] = 0;
            sndCallSeStopData4 = 6;
        } else if (mnuTitleStreamStatus.loadState == 5) {
            mnuTitleStreamStatus.loadState = 4;
            sndBgmFadeOutWork[mnuTitleStreamStatus.tableIndex] = mnuTitleStreamStatus.frameIndex;
            sndCallSeStopData4 = 6;
        }
        sndData7 = 0;
    }
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndBgmFadeOut2);

/* NON_MATCHING: 22 of 24 words match.  */
#ifdef NON_MATCHING
void sndCallMemFreeDirect(void) {
    if (mnuTitleStreamStatus.allocation != NULL) {
        sdfMemFreeDirect(mnuTitleStreamStatus.allocation);
        mnuTitleStreamStatus.loadState = 0;
        mnuTitleStreamStatus.allocation = NULL;
        mnuTitleStreamStatus.compressedData = NULL;
        mnuTitleStreamStatus.samples = (s16 *)mnuTitleAudioSilenceBuffer;
        sndData5 = -1;
        sndData6 = -1;
        sndData7 = 0;
    }
}
#else
INCLUDE_ASM(const s32, "sndAt3", sndCallMemFreeDirect);
#endif

void sndBgmFree(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

void sndCallLoad(char *audioPath) {
    sndCallFileCheckEndWork = fileLoad(audioPath);
    mnuTitleSoundBufferState.loadState = 1;
}

s32 sndCallFileCheckEnd(titleAudioStreamState_t *stream) {
    sdfMemHandle_t *handle;
    u8 *data;
    s32 size;

    if (pacFileCheckEnd(sndCallFileCheckEndWork) == 0) {
        return 0;
    }
    handle = fileGetMemHandle(sndCallFileCheckEndWork);
    data = fileGetLoadPtr(sndCallFileCheckEndWork);
    size = fileGetLoadSize(sndCallFileCheckEndWork);
    fileHandleFree(sndCallFileCheckEndWork);
    mnuTitleSoundBufferState.compressedData = data;
    mnuTitleSoundBufferState.allocation = handle;
    stream->frameCount = size / stream->frameBytes;
    stream->frameIndex = 0;
    at3DecInit(sndWork3);
    mnuTitleSoundBufferState.loadState = 2;
    return 1;
}

/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00269C10.c:517, rework, same function 0.93 (words 42/42, calls 0.80, shape 1.00) */
INCLUDE_RODATA(const s32, "sndAt3", sndStrSoundat3At3);

void mnuInitializeTitleSoundBuffer(void) {
    titleAudioStreamState_t *streamState = &mnuTitleSoundBufferState;
    u32 *decoder = sndWork3;
    struct sdfMemHandle_t *allocation;
    u8 *bufferAddress;

    WaitSema(mnuTitleStreamSemaphore);
    streamState->decoder = decoder;
    allocation = sdfMemAllocDirect(96000);
    bufferAddress = sdfMemGetPtr(allocation);
    streamState->allocation = allocation;
    streamState->control = 2;
    streamState->decoder[2] = 2;
    streamState->compressedData = bufferAddress;
    streamState->samples = (s16 *)mnuTitleAudioSilenceBuffer;
    streamState->frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    mnuLoadTitleStreamFrameData("/soundat3/se01-2.at3", streamState);
    at3DecInit(decoder);
    SignalSema(mnuTitleStreamSemaphore);
}

#ifdef VERSION_RAIDOU1
/* NON_MATCHING: 21 of 62 words match.  */
#ifdef NON_MATCHING
void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex) {
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];
    s32 format;

    WaitSema(mnuTitleStreamSemaphore);
    sndDeadSePlay_VolAngle();
    format = mnuQueueTitleSoundStreamEntryData[soundEntryIndex].format;
    mnuTitleSoundBufferState.repeatFrame = mnuQueueTitleSoundStreamEntryData[soundEntryIndex].repeatFrame;
    mnuTitleSoundBufferState.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleSoundBufferState.decoder = sndWork3;
    mnuTitleSoundBufferState.control = 2;
    if (format == 2) {
        sndWork3[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    } else if (format < 3) {
        if (format == 1) {
            sndWork3[2] = format;
            mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        }
    } else if (format == 3) {
        sndWork3[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
    } else if (format == 4) {
        sndWork3[2] = format;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
    }
    sprintf(soundPath, sndStrSoundat3At3, mnuQueueTitleSoundStreamEntryData[soundEntryIndex].name);
    sndCallLoad(soundPath);
    SignalSema(mnuTitleStreamSemaphore);
}
#else
INCLUDE_ASM(const s32, "sndAt3", mnuQueueTitleSoundStreamEntry);
#endif

void mnuLoadTitleSoundBufferEntry(s32 soundEntryIndex) {
    struct sdfMemHandle_t *allocation = NULL;
    u8 *bufferAddress;
    char soundPath[MNU_TITLE_SOUND_PATH_BYTES];

    WaitSema(mnuTitleStreamSemaphore);
    mnuTitleSoundBufferState.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleSoundBufferState.repeatFrame = mnuLoadTitleSoundBufferEntryData[soundEntryIndex].repeatFrame;
    mnuTitleSoundBufferState.decoder = sndWork3;
    mnuTitleSoundBufferState.control = 2;
    switch (mnuLoadTitleSoundBufferEntryData[soundEntryIndex].format) {
    case 1:
        sndWork3[2] = 1;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfMemAllocDirect(192000);
        break;
    case 2:
        sndWork3[2] = 2;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfMemAllocDirect(96000);
        break;
    case 3:
        sndWork3[2] = 3;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfMemAllocDirect(96000);
        break;
    case 4:
        sndWork3[2] = 4;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfMemAllocDirect(48000);
        break;
    }
    bufferAddress = sdfMemGetPtr(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = bufferAddress;
    sprintf(soundPath, sndStrSoundat3At3, mnuLoadTitleSoundBufferEntryData[soundEntryIndex].name);
    mnuLoadTitleStreamFrameData(soundPath, &mnuTitleSoundBufferState);
    at3DecInit(sndWork3);
    SignalSema(mnuTitleStreamSemaphore);
}
#else
INCLUDE_ASM(const s32, "sndAt3", mnuQueueTitleSoundStreamEntry);

INCLUDE_ASM(const s32, "sndAt3", mnuLoadTitleSoundBufferEntry);
#endif

void mnuInstallTitleSoundBufferFromData(const void *compressedData, s32 dataBytes, s32 format) {
    struct sdfMemHandle_t *allocation = NULL;
    u8 *bufferAddress;

    WaitSema(mnuTitleStreamSemaphore);
    sndDeadSePlay_VolAngle();
#ifdef VERSION_RAIDOU2
    sndData9 = 0;
#endif
    mnuTitleSoundBufferState.samples = (s16 *)mnuTitleAudioSilenceBuffer;
    mnuTitleSoundBufferState.decoder = sndWork3;
    mnuTitleSoundBufferState.control = 2;
    mnuTitleSoundBufferState.repeatFrame = -1;
    switch (format) {
    case 1:
        sndWork3[2] = 1;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_LARGE;
        allocation = sdfMemAllocDirect(192000);
        break;
    case 2:
        sndWork3[2] = 2;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfMemAllocDirect(96000);
        break;
    case 3:
        sndWork3[2] = 3;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_MEDIUM;
        allocation = sdfMemAllocDirect(96000);
        break;
    case 4:
        sndWork3[2] = 4;
        mnuTitleSoundBufferState.frameBytes = MNU_SOUND_FRAME_BYTES_SMALL;
        allocation = sdfMemAllocDirect(48000);
        break;
    }
    bufferAddress = sdfMemGetPtr(allocation);
    mnuTitleSoundBufferState.allocation = allocation;
    mnuTitleSoundBufferState.compressedData = bufferAddress;
    memcpy(bufferAddress, compressedData, dataBytes);
    mnuTitleSoundBufferState.frameCount = dataBytes / mnuTitleSoundBufferState.frameBytes - 1;
    mnuTitleSoundBufferState.frameIndex = 0;
    at3DecInit(sndWork3);
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndFunc2);

/* The title sound buffer's status: 1 loading, 0 empty, 2 stopped, 3 playing. */
s32 sndSePlay(void) {
    WaitSema(mnuTitleStreamSemaphore);
    if (mnuTitleSoundBufferState.loadState == 1) {
        sndCallFileCheckEnd(&mnuTitleSoundBufferState);
        SignalSema(mnuTitleStreamSemaphore);
        return 1;
    }
    if (mnuTitleSoundBufferState.allocation == NULL) {
        SignalSema(mnuTitleStreamSemaphore);
        return 0;
    }
    if (mnuTitleSoundBufferState.control == 2) {
        SignalSema(mnuTitleStreamSemaphore);
        return 2;
    }
    SignalSema(mnuTitleStreamSemaphore);
    return 3;
}

INCLUDE_ASM(const s32, "sndAt3", sndCallWaitSema);

void sndDeadSePlay_VolAngle(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
}


/* body: matched in dds @ 18d1fa88f: src/dds1/game/code_00269C10.c:659, rework, same function 1.00 (words 15/15, calls 1.00, shape 1.00) */
void sndSeStop(void) {
    titleAudioStreamState_t *streamState = &mnuTitleSoundBufferState;
    struct sdfMemHandle_t *allocationHandle = streamState->allocation;

    if (allocationHandle == 0) {
        return;
    }
    sdfMemFreeDirect(allocationHandle);
    streamState->allocation = 0;
}

void nbEffectShutdown(void) {
    WaitSema(mnuTitleStreamSemaphore);
    sndDeadSePlay_VolAngle();
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndSeFree);

INCLUDE_ASM(const s32, "sndAt3", sndSeFadeOut);

void sndBgmLoadIdXFSub(void) {
    u8 *dst = sndWork5;
    s32 n = 0;
    s32 i = 0;
    u8 *src = mnuTitleStreamStatus.compressedData + mnuTitleStreamStatus.frameBytes * mnuTitleStreamStatus.frameIndex;
    s32 frame = mnuTitleStreamStatus.frameIndex;
    s32 j;

    do {
        if (frame + n >= mnuTitleStreamStatus.frameCount) {
            src = mnuTitleStreamStatus.compressedData;
            frame = 0;
            n = 0;
        }
        for (j = 0; j < 192; j++) {
            *dst++ = *src++;
        }
        i++;
        n++;
    } while (i < 192);

    if (mnuTitleStreamStatus.control == 1 && (mnuTitleStreamStatus.loadState >= 3 && mnuTitleStreamStatus.loadState <= 5)) {
        sndCallSeStopData4 = 12;
        mnuTitleStreamStatus.loadState = 7;
        sndBgmFadeOutWork[mnuTitleStreamStatus.tableIndex] = mnuTitleStreamStatus.frameIndex;
        sndCallSeStopWork6.loadState = 6;
        sndCallSeStopWork6.control = 0;
        sndCallSeStopWork6.frameIndex = 0;
        sndCallSeStopWork7[4] = mnuTitleStreamDecoderState[4];
        if (mnuTitleStreamDecoderState[4] >= 31) {
            sndCallSeStopWork7[4] = 29;
        }
    }
}

s32 sndCallWaitSema3(void) {
    s32 status = 1;

    WaitSema(mnuTitleStreamSemaphore);
    if (sndCallSeStopWork6.loadState == 0) {
        status = 0;
    } else {
        switch (sndCallSeStopWork6.control) {
        case 0:
            break;
        case 1:
            if (mnuTitleStreamStatus.control == 2 && mnuTitleStreamStatus.loadState == 0) {
                status = mnuTitleStreamStatus.control;
            }
            break;
        }
    }
    SignalSema(mnuTitleStreamSemaphore);
    return status;
}

/* Request a crossfade to BGM id; sndData2 records the outcome for sndBgmStatus_XF. */
/* Residual: R1 2 of 81 words (delay slot order before the last SignalSema), R2 4 (also a register in the index shift). */
#ifdef NON_MATCHING
void sndBgmLoadId_XF(s32 id) {
    WaitSema(mnuTitleStreamSemaphore);
    sndData7 = 0;
    sndData5 = id;
    if (sndBgmLoadId_XFData[id].format == 1) {
        sndBgmLoadId_XFData2 = id;
        sndData2 = 0x66;
        SignalSema(mnuTitleStreamSemaphore);
        return;
    }
    if (mnuTitleStreamStatus.loadState == 4 && sndData6 == id) {
        sndData2 = 3;
        SignalSema(mnuTitleStreamSemaphore);
        sndBgmFadeIn();
        return;
    }
    if (BgmPlaySub() == 3) {
        if (sndData6 == id) {
            sndBgmLoadId_XFData2 = id;
            sndData7 = 1;
            sndData2 = 4;
            SignalSema(mnuTitleStreamSemaphore);
            return;
        }
        sndBgmLoadId_XFData2 = id;
        if (sndBgmLoadId_XFData[sndData6].format == 1) {
            sndData2 = 0x66;
            SignalSema(mnuTitleStreamSemaphore);
            return;
        }
        sndData2 = 1;
        sndBgmLoadIdXFSub();
    } else {
        sndBgmLoadId_XFData2 = id;
        sndData2 = 0x65;
    }
    SignalSema(mnuTitleStreamSemaphore);
}
#else
INCLUDE_ASM(const s32, "sndAt3", sndBgmLoadId_XF);
#endif

s32 sndBgmStatus_XF(void) {
    s32 stat;
    s32 ret;

    ret = 0;
    stat = sndData2;

    if (stat == 0) {
        ret = sndCallWaitSema2();
    } else if (stat == 1) {
        ret = sndCallWaitSema3();
        switch (ret) {
            case 0:
            case 2:
                sndBgmLoadId(sndBgmLoadId_XFData2);
                sndData2 = 2;
            case 1:
                ret = 1;
                break;
            default:
                ret = 1;
                break;
        }
    } else if (stat == 101) {
        sndBgmLoadId(sndBgmLoadId_XFData2);
        sndData2 = 2;
        ret = 1;
    } else if (stat == 102) {
        sndBgmLoadId(sndBgmLoadId_XFData2);
        sndData2 = 2;
        ret = 1;
    } else if (stat == 2) {
        if (sndCallWaitSema2() == stat) {
            sndData2 = 0;
            ret = 2;
        } else {
            ret = 1;
        }
    } else if (stat == 3) {
        sndData2 = 0;
        ret = 2;
    } else if (stat == 4) {
        sndData2 = 0;
        ret = 2;
    }
    return ret;
}

void sndProcSequenceSub(void) {
    evtData25 = 0;
}

s32 sndProcSequenceSub2(void) {
    return evtData25;
}

/* body: matched in raidou @ 88f0d54: src/shared/sndAt3.c:887, near, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 sndProcSequenceSub3(void) {
    return sndData6;
}

INCLUDE_ASM(const s32, "sndAt3", sndLeaf3);

s32 sndProcSequenceSub4(void) {
    return sndWork3[4];
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "sndAt3", sndHardSelect1Sub);
#endif

INCLUDE_ASM(const s32, "sndAt3", sndLeaf4);

s32 movieNoMemoryAlloc(void)
{
    release_movieobj2(&movieCb, &sdfGrOtTable[movieData]);
    return 0;
}

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2Demo03Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie16EIpu);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2Demo02Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie16DIpu);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2Demo01Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie16CIpu);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2Ending2Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie16BIpu);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2TitleBgIpu);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieLoop01Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2EndingIps);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2TgsIps);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie16AIpu);
#endif

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie616001Ipu);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2661002Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2661001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2660003Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2660002Ips);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2660001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie2654001Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie15Ips);
#endif

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie696001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie657002Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie657001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie659001Ips);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie658002Ips);

#endif
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie658001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie695001Ips);
#ifdef VERSION_RAIDOU1

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie656001Ips);
#endif

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie653001Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie652003Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie652002Ips);

INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie652001Ips);

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie651002Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie02Ips);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie651001Ips);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie01Ips);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0Smovie606001Ipu);
#else
INCLUDE_RODATA(const s32, "sndAt3", movieStrCdrom0SmovieMovie00Ipu);
#endif

INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData2);

INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData3);

INCLUDE_SDATA(const s32, "sndAt3", sndData3);

INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData4);

INCLUDE_SDATA(const s32, "sndAt3", sndData4);

INCLUDE_SDATA(const s32, "sndAt3", sndData5);

INCLUDE_SDATA(const s32, "sndAt3", sndData6);

INCLUDE_SDATA(const s32, "sndAt3", sndData7);

INCLUDE_SDATA(const s32, "sndAt3", evtData25);

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData5);
#else
INCLUDE_SDATA(const s32, "sndAt3", sndData8);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData6);
#else
INCLUDE_SDATA(const s32, "sndAt3", sndData9);
#endif

#ifdef VERSION_RAIDOU1
INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData7);
#else
INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData9);
#endif

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData10);

INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData11);

INCLUDE_SDATA(const s32, "sndAt3", sndCallSeStopData12);

#endif
