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

/* The three audio stream records (title stream, title sound buffer, and the
 * stream sndBgmLoadId resets); each is followed by its decoder state. */
typedef struct TitleAudioStreamState {
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
} TitleAudioStreamState;

typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x30) ? 1 : -1];

/* One BGM table entry: decoder format, repeat frame and the .at3 base name. */
typedef struct SndBgmEntry {
    u8 format;
    s16 repeatFrame;
    char name[12];
} SndBgmEntry;

typedef char SndBgmEntry_size[(sizeof(SndBgmEntry) == 0x10) ? 1 : -1];

/* Shared title-audio stream operations, implemented by each title. */
void mnuQueueTitleSoundStreamEntry(s32 soundEntryIndex);

typedef struct TitleAudioStreamStatusSnapshot {
    u32 frameCount;
    u32 frameIndex;
    u32 repeatFrame;
} TitleAudioStreamStatusSnapshot;

typedef char TitleAudioStreamStatusSnapshot_size[
    (sizeof(TitleAudioStreamStatusSnapshot) == 0x0C) ? 1 : -1];

#define SND_MIX_SAMPLE_COUNT 0x800

#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_SOUND_PATH_BYTES 32

#define MNU_SOUND_FRAME_BYTES_LARGE 0x180

#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0

#define MNU_SOUND_FRAME_BYTES_SMALL 0x60

#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400

#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200

#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

extern TitleAudioStreamState mnuTitleSoundBufferState;
extern u32 mnuTitleAudioSilenceBuffer[];
extern TitleAudioStreamState mnuTitleStreamStatus;
extern u32 mnuTitleStreamSemaphore;

void sndMixSampleBuffers(s16 *destinationSamples, TitleAudioStreamState *firstSource, TitleAudioStreamState *secondSource);
extern u32 mnuTitleStreamDecoderState[];
void mnuResetTitleStream(void);
void mnuReleaseSoundBuffer(void);
void mnuReadTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusSnapshot);
void mnuWriteTitleStreamStatusLocked(TitleAudioStreamStatusSnapshot *statusValues);
void File_BgmLoadStart(char *audioPath);
void sndBgmLoadFile(void);
void sndDeadSePlay_VolAngle(void);
void sndCallSeStop(void);
void GetVolume(TitleAudioStreamState *stream);
s32 BgmPlaySub(void);
void sndBgmFadeIn(void);

extern SndBgmEntry sndBgmLoadId_XFData[];
/* Each BGM's resume frame, by table index. */
extern u16 sndBgmFadeOutWork[];
extern TitleAudioStreamState sndCallSeStopWork6;
extern sdfThCB_t sndCallWaitVblankWork;
extern u8 sndCallWaitVblankWork2[0x1000];
extern s32 sndData2;
extern s32 sndBgmLoadId_XFData2;
extern s32 sndCallSeStopData4;
extern s32 sndData5;
extern s32 sndData6;
extern s32 sndData7;

INCLUDE_ASM(const s32, "sndAt3", sndUploadStreamToBothIopBuffers);

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

INCLUDE_ASM(const s32, "sndAt3", sndLeaf2);

INCLUDE_ASM(const s32, "sndAt3", sndCallSeStop);

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

INCLUDE_ASM(const s32, "sndAt3", sndCallMemset);

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

INCLUDE_ASM(const s32, "sndAt3", mnuLoadTitleStreamFrameData);

INCLUDE_ASM(const s32, "sndAt3", File_BgmLoadStart);

INCLUDE_ASM(const s32, "sndAt3", GetVolume);

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
    TitleAudioStreamState *status;

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

INCLUDE_ASM(const s32, "sndAt3", BgmPlaySub);

s32 sndCallWaitSema2(void) {
    TitleAudioStreamState *stream = &mnuTitleStreamStatus;
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

INCLUDE_ASM(const s32, "sndAt3", sndCallMemFreeDirect);

void sndBgmFree(void) {
    WaitSema(mnuTitleStreamSemaphore);
    mnuResetTitleStream();
    SignalSema(mnuTitleStreamSemaphore);
}

INCLUDE_ASM(const s32, "sndAt3", sndCallLoad);

INCLUDE_ASM(const s32, "sndAt3", sndCallFileCheckEnd);

INCLUDE_RODATA(const s32, "sndAt3", sndStrSoundat3At3);

INCLUDE_ASM(const s32, "sndAt3", mnuInitializeTitleSoundBuffer);

INCLUDE_ASM(const s32, "sndAt3", mnuQueueTitleSoundStreamEntry);

INCLUDE_ASM(const s32, "sndAt3", mnuLoadTitleSoundBufferEntry);

INCLUDE_ASM(const s32, "sndAt3", mnuInstallTitleSoundBufferFromData);

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


INCLUDE_ASM(const s32, "sndAt3", sndSeStop);

INCLUDE_ASM(const s32, "sndAt3", nbEffectShutdown);

INCLUDE_ASM(const s32, "sndAt3", sndSeFree);

INCLUDE_ASM(const s32, "sndAt3", sndSeFadeOut);

INCLUDE_ASM(const s32, "sndAt3", sndBgmLoadIdXFSub);

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

INCLUDE_ASM(const s32, "sndAt3", sndBgmStatus_XF);

INCLUDE_ASM(const s32, "sndAt3", sndProcSequenceSub);

INCLUDE_ASM(const s32, "sndAt3", sndProcSequenceSub2);

INCLUDE_ASM(const s32, "sndAt3", sndProcSequenceSub3);

INCLUDE_ASM(const s32, "sndAt3", sndLeaf3);

INCLUDE_ASM(const s32, "sndAt3", sndProcSequenceSub4);

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "sndAt3", sndHardSelect1Sub);
#endif

INCLUDE_ASM(const s32, "sndAt3", sndLeaf4);

INCLUDE_ASM(const s32, "sndAt3", movieNoMemoryAlloc);

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
