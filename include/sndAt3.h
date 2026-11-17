#ifndef SNDAT3_H
#define SNDAT3_H

#include "common.h"

struct titleAudioStreamState_t;
#ifdef VERSION_RAIDOU2
extern s32 sndData9;
#endif
void mnuLoadTitleStreamFrameData(char *filePath, struct titleAudioStreamState_t *streamState);
void mnuInstallTitleSoundBufferFromData(const void *compressedData, s32 dataBytes, s32 format);
void nbEffectShutdown(void);
void sndBgmContinue(void);
void sndBgmFadeIn(void);
void sndBgmFadeOut(void);
void sndBgmFadeOut2(void);
void sndBgmFree(void);
void sndBgmLoadId(s32 id);
void sndBgmLoadIdXFSub(void);
void sndBgmLoadId_XF(s32 id);
s32 sndBgmStatus_XF(void);
void sndBgmStop(void);
void sndBgmTop(void);
struct fileRequest;
extern struct fileRequest *GetVolumeWork;

struct titleAudioStreamState_t;
/* Retail passes the stream record, not a file name pointer. */
s32 sndCallFileCheckEnd(struct titleAudioStreamState_t *stream);
extern struct fileRequest *sndCallFileCheckEndWork;
s32 sndCallLoad(void *);
void sndCallMemset(void);
s32 sndCallWaitSema3(void);
void sndProcSequenceSub(void);
s32 sndProcSequenceSub2(void);
s32 sndProcSequenceSub4(void);
void sndCallWaitSema(void);
void sndSeFadeOut(void);
s32 sndSePlay(void);
void sndSeFree(void);

void sndBgmPlay(void);

extern const char sndStrSoundat3At3[];

#endif
