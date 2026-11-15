#ifndef SNDAT3_H
#define SNDAT3_H

#include "common.h"

void mnuLoadTitleStreamFrameData(char **fname, void *);
void sndBgmContinue(void);
void sndBgmFadeOut(void);
void sndBgmFadeOut2(void);
void sndBgmLoadId(s32 id);
void sndBgmLoadIdXFSub(void);
void sndBgmLoadId_XF(s32 id);
s32 sndBgmStatus_XF(void);
void sndBgmStop(void);
void sndBgmTop(void);
struct TitleAudioStreamState;
/* Retail passes the stream record, not a file name pointer. */
void sndCallFileCheckEnd(struct TitleAudioStreamState *stream);
s32 sndCallLoad(void *);
void sndCallMemset(void);
s32 sndCallWaitSema3(void);
void sndProcSequenceSub(void);
void sndSeFadeOut(void);
void sndSeFree(void);

#endif
