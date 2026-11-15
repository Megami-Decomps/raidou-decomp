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

/* The two title-audio states own the complete ten-word frame/decoder record. */
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
} TitleAudioStreamState;

typedef char TitleAudioStreamState_size[(sizeof(TitleAudioStreamState) == 0x28) ? 1 : -1];

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

INCLUDE_ASM(const s32, "sndAt3", sndCallWaitVblank);

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

INCLUDE_ASM(const s32, "sndAt3", sndBgmLoadId);

INCLUDE_ASM(const s32, "sndAt3", BgmPlaySub);

INCLUDE_ASM(const s32, "sndAt3", sndCallWaitSema2);

INCLUDE_ASM(const s32, "sndAt3", sndBgmPlay);

INCLUDE_ASM(const s32, "sndAt3", sndBgmFadeIn);

INCLUDE_ASM(const s32, "sndAt3", sndBgmContinue);

INCLUDE_ASM(const s32, "sndAt3", sndBgmStop);

INCLUDE_ASM(const s32, "sndAt3", sndBgmTop);

INCLUDE_ASM(const s32, "sndAt3", sndBgmFadeOut);

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

INCLUDE_ASM(const s32, "sndAt3", sndSePlay);

INCLUDE_ASM(const s32, "sndAt3", sndCallWaitSema);

void sndDeadSePlay_VolAngle(void) {
    mnuTitleSoundBufferState.frameIndex = 0;
    mnuTitleSoundBufferState.control = 2;
    mnuReleaseSoundBuffer();
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
