#ifndef CODE_R1_001_CF808_H
#define CODE_R1_001_CF808_H

#include "common.h"

/* The value moves from idle through file loading and then IOP commit. */
typedef s32 MnuTitleStreamLoadState;

#include "kernel.h"

void ProcessInit(void);
s32 mnuDestroyTitleEffectTask(dds3ProcessID_t *id);
s32 mnuIncrementTitleEffectFrameCounter(dds3ProcessID_t *id);
void sndInit(void);

extern char sndVcInfo[];
extern char sndSqInfo[];
extern s16 sndInitWork;
extern s32 sndInitData[];
extern s8 sndInitData2;
extern s32 sndInitData3;
extern s32 sndInitData4;
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename);

/* The EE staging buffer and the two IOP stream buffers it is copied into. */
extern u32 sndData[2];
extern u32 sndInitializeStreamTransferBuffersData;
extern u64 mnuCopySoundStreamOnStatusChangeWork;
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount);
void mnuSetTitleVoicePrefixIndex(s32 prefixIndex);
void fldNowPlayengStart(s32 fileNo);
s32 mnuGetTitleEffectFrameCounter(void);

/* The PCM voice owner stores idle as 0 and active playback as 1. */
typedef s32 MnuTitleSoundBusyState;

MnuTitleSoundBusyState scrCheckEnableMovieCancel(void);
void scrGetAfterSkipBgmFlag(void);
s32 scrCallLoadCheckMidi(s32 id);
void mnuInitTitleSoundRemoteRequest(u32 wordCount);

void evtFunc8(s32 transferSize);
s32 mnuCopySoundStreamOnStatusChange(void *samples, u32 wordCount);

#endif
