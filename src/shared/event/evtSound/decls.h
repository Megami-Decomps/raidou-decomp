/* Declarations of smt3-decomp src/smt3/event/evtSound.c, for its bodies in this directory. */
#ifndef SHARED_EVENT_EVTSOUND_H
#define SHARED_EVENT_EVTSOUND_H

#include "common.h"

#include "kernel.h"

extern char evtSkyProcName[];

extern char evtFmtStr[];

extern char evtPackNameFormat[];

extern u16 skyanime;

extern s32 fldbokasi;

#include "sdk_libc.h"

extern s32 p2sdLoadSMGFile(s32 MidiNo);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern void p2sdPlayBGM(s32 MidiNo);

extern void p2sdSetBgmTrack(s32 index);

extern void p2sdPlayBGMFade(s32 index);

extern void p2sdStopBGM(s32 index);

extern void p2sdPlaySE(s32 index, s32 volume, s32 pan);

extern void p2sdStopSe(u32 sound);

s32 evtGetSoundIndex(s32 eventId, s32 variation);

s32 evtGetSoundIndex(s32 eventId, s32 variation);

s32 evtLoadEventBgm(s32 eventId);

s32 evtCheckEventBgmLoaded(s32 eventId);

s32 evtPlayEventBgm(s32 eventId, s32 variation);

void evtTransEventBgm(s32 eventId, s32 variation);

s32 evtFadeInEventBgm(s32 eventId, s32 variation);

s32 evtStopEventBgm(s32 eventId, s32 variation);

s32 evtPlaySe(s32 eventno, s32 seno);

s32 func_001FA158(s32 eventId, s32 variation);

void evtMakeEventPackName(s32 index, char *buffer);

#endif
