#ifndef SDFSOUND_H
#define SDFSOUND_H

#include "common.h"

/* One loaded MIDI entry of the sound driver; sdfLeaf35 returns the table. */
typedef struct {
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} p2sdLoadEntry_t;

u32 p2sdGetPcmState(void);
s32 p2sdLoadCheckMidi(s32 MidiNo);
s32 p2sdLoadPcmFile(const char *name, u8 channel);
void p2sdLoadSMGFile(s32 MidiNo);
void p2sdPlayBGM(s32 MidiNo);
void p2sdSetPcmBuffer(saddr_t data, s32 size);
void p2sdPlayBGMFade(s32 trackId);
s32 p2sdPlayPcm(u8 channel);
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);
void p2sdSetBgmTrack(s32 aTrack);
u32 p2sdSetPcmCallback(addr_t aCallback);
s32 p2sdStopAllSe(void);
void p2sdStopSe(u32 soundId);
s32 p2sdStopBGM(s32 aId);
s32 p2sdSyncMovie(void);
s32 p2sdSyncSound(void);
void sdfSndInit(char *aVoiceInfo, s32 aVoiceCount, char *aSequenceInfo, s32 aSequenceCount);
void sdfDumpHexLine(const void *aData, s32 size);
s32 sdfSndSendCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize);
s32 sdfBtlReLoadMapSBtlSceneSub(void);
p2sdLoadEntry_t *sdfLeaf35(void);
void sdfCallSndSendCmd(s32 arg);

#endif
