#include "common.h"
#include "sdfMemory.h"
#include "sdfChip.h"
#include "sdfSound.h"
#include "scrTraceCode.h"
#include "dds3KernelCore.h"

#include "field.h"
#include "include_asm.h"
#include "kernel.h"
#include "draw3d.h"
#include "basic.h"
#include "evt_world.h"
#include "newdata.h"
#include "code_r1_001CF808.h"

/* The PCM voice owner stores idle as 0 and active playback as 1. */
typedef s32 MnuTitleSoundBusyState;

/* The voice-file RPC sets this busy state on open and clears it on close.
 * It is separate from the PCM callback-command busy latch. */

/* Returns a borrowed read-only string for a STRING-tagged parameter, or NULL. */

u32 fldCmdGetCurrentSceneSelectionResource(void);

/* Clear the selected flag only if it was set, and report whether it changed. */
s32 fldCommand_FLD_GET_KIRETA(void);

/* Roll against the threshold associated with the mirrored solar phase. */
s32 fldCmdRollMirroredSolarThreshold(void);
s32 fldCommandSendNamedSound(void);
u32 fldCommandSendSoundControl(void);

/* Return sound-command busy state to the field script interpreter. */
u32 fldCommandIsSoundBusy(void);

/* Pass five field-script arguments to the underlying handler. */
s32 fldCmdMarkBitmapRegion(void);
s32 fldCmdSetStateFlag(void);

#include "kernel_draw.h"
#include "sdk_libc.h"
#include "sdk_libkernl.h"

void Exit(s32 status);

/* Return the descriptor's payload base as a byte address, not the descriptor. */

s32 sceSdRemoteInit(void);

/* Shared per-row lifecycle for EXP and profile-point result animations. */
typedef s8 BrsProgressAnimationPhase;

/* Cross-title completion view, 0x14 bytes before the corresponding
 * BrsProgressAnimation row (see mnu_result.h). Only byte 0 of the two
 * icon-state regions is polled; other bytes stay opaque. */
typedef struct BrsResultCounterView {
    u8 pad00[0x14];
    BrsProgressAnimationPhase phase;
    u8 pad15[0x1F];
    s32 remaining;
    u8 pad38[0x14];
    s8 progressIconState[1];
    u8 pad4D[0x13];
    s8 completionIconState[8];
} BrsResultCounterView;

typedef char BrsResultCounterView_size_must_be_0x68[(sizeof(BrsResultCounterView) == 0x68) ? 1 : -1];

extern void mnuTitleStreamUpdateAndLogBgm(void);

/* The four sprite corner colours start from the shared grey table. */
typedef struct BrsCornerColors {
    u32 rgba[4];
} BrsCornerColors;

typedef struct BrsLevelCornerColors {
    u32 values[4];
} BrsLevelCornerColors;

typedef struct TitleEffectState {
    s32 soundNameIndex;
    s32 frameCounter;
} TitleEffectState;

#define MNU_STREAM_LOAD_IDLE 0

MnuTitleStreamLoadState mnuPollTitleStreamStateLocked(void);

#define BRS_RESULT_COUNTER_PAIR_COUNT 5

#define BRS_RESULT_SETTLED_POLL_LIMIT 6

#define BRS_RESULT_SETTLED_POLL_CLAMP 7

#define SND_TRACK_DEFAULT_VOLUME 0x7f

#define SND_TRACK_DEFAULT_PAN 0x3f

#define SND_TRANSFER_WORD_BYTES 4

#define SND_TRANSFER_WORD_HALFWORDS 2

#define SND_TRANSFER_PAIR_WORD_BYTES 8

#define SND_MIX_SAMPLE_COUNT 0x800

#define SND_MIX_SAMPLE_LIMIT 0x7FFF

#define MNU_TITLE_EFFECT_BYTES 8

#define MNU_TITLE_VOICE_PREFIX_BYTES 5

#define MNU_TITLE_VOICE_PATH_BYTES 16

#define MNU_TITLE_SOUND_PATH_BYTES 32

#define MNU_SOUND_FRAME_BYTES_LARGE 0x180

#define MNU_SOUND_FRAME_BYTES_MEDIUM 0xC0

#define MNU_SOUND_FRAME_BYTES_SMALL 0x60

#define MNU_SOUND_BUFFER_BYTES_LARGE 0x38400

#define MNU_SOUND_BUFFER_BYTES_MEDIUM 0x1C200

#define MNU_SOUND_BUFFER_BYTES_SMALL 0xE100

struct dds3Basic_t;

extern u32 D_003BC5BC;

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC598[];
extern dds3ProcessID_t *mnuTitleSoundTask;
extern char evtPicturePriProcName[];

extern u32 D_003BC5B0[2];
extern u32 D_003BC5B8;
extern s32 sndStreamTransferIopHeapAddress;

typedef struct BrsResultWork {
    u8 pad00[0xD40];
    s32 settledFrames;
    u8 padD44[0x19C];
    BrsResultCounterView levelCounters[8];
    BrsResultCounterView profileCounters[8];
} BrsResultWork;

typedef struct BrsProfilePalette { u32 values[4]; } BrsProfilePalette;

void mnuResetTitleEffectState(s32 command);
void mnuSetTitleVoicePrefixIndex(s32 prefixIndex);
void scrGetAfterSkipBgmFlag(void);
void mnuNoOpTitleAudio(void);
s32 mnuGetTitleEffectFrameCounter(void);
u32 sndOpStartTrackFromScript(void);
u32 sndOpSetTrackDefaultVolumePan(void);
u32 sndOpSendTitleAudioCommand(void);
s32 movCheckStartupSoundState(void);
u32 sndOpUpdateTitleBgmState(void);
u32 mnuResetTitlePlaybackCommands(void);
u32 mnuAdvanceTitleStreamFromScript(void);
u8 mnuIsTitleStreamIdle(void);
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount);
void sndInitializeStreamTransferBuffers(s32 wordCount);
void mnuInitTitleSoundRemoteRequest(u32 wordCount);

/* Use the voice prefix with the same packed, unbounded append convention. */
char *mnuBuildVoiceResourcePath(char *pathBuffer, const char *filename) {
    *(u64p *)pathBuffer = *(u64p *)D_003BC598;
    return strcat(pathBuffer, filename);
}

INCLUDE_ASM(const s32, "code_r1_001CF808", sndInit);

INCLUDE_ASM(const s32, "code_r1_001CF808", sndLeaf);

INCLUDE_ASM(const s32, "code_r1_001CF808", mnuIncrementTitleEffectFrameCounter);

INCLUDE_ASM(const s32, "code_r1_001CF808", mnuDestroyTitleEffectTask);

void ProcessInit(void)
{
    TitleEffectState *state = sdfChipAllocDirect(sizeof(TitleEffectState));

    mnuTitleSoundTask = dds3StartProcess(evtPicturePriProcName, 0x5212, 1, 1,
                                         mnuIncrementTitleEffectFrameCounter,
                                         mnuDestroyTitleEffectTask, NULL);
    dds3SetProcessData(mnuTitleSoundTask, state);
    state->soundNameIndex = 0;
    state->frameCounter = 0;
}

void mnuResetTitleEffectState(s32 command) {
    TitleEffectState *effectState = (TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(command, 0x7f);
    effectState->frameCounter = 0;
}

void mnuSetTitleVoicePrefixIndex(s32 prefixIndex) {
    ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->soundNameIndex = prefixIndex;
}

INCLUDE_ASM(const s32, "code_r1_001CF808", fldNowPlayengStart);

INCLUDE_ASM(const s32, "code_r1_001CF808", fldNKIY);

INCLUDE_ASM(const s32, "code_r1_001CF808", fldCheckNextFile);

void scrGetAfterSkipBgmFlag(void) {
    p2sdSyncSound();
}

MnuTitleSoundBusyState scrCheckEnableMovieCancel(void) {
    return p2sdGetPcmState();
}

void mnuNoOpTitleAudio(void) {
}

s32 mnuGetTitleEffectFrameCounter(void) {
    return ((TitleEffectState *)dds3GetProcessData(mnuTitleSoundTask))->frameCounter;
}

#ifdef VERSION_RAIDOU2
INCLUDE_ASM(const s32, "code_r1_001CF808", scrLeaf);

#endif
INCLUDE_ASM(const s32, "code_r1_001CF808", scrCallLoadCheckMidi);

INCLUDE_ASM(const s32, "code_r1_001CF808", scrCallGetIntPara7);

INCLUDE_ASM(const s32, "code_r1_001CF808", scrCallGetIntPara8);

INCLUDE_ASM(const s32, "code_r1_001CF808", sndOpStopNamedPlayback);

INCLUDE_ASM(const s32, "code_r1_001CF808", slCallGetIntPara);

/* Handle an occupied sound-command channel before dispatching a named sound. */
s32 fldCommandSendNamedSound(void) {
    const char *commandName;
    commandName = scrGetStrPara(0);
    if (p2sdGetPcmState() != 0) {
        p2sdSyncSound();
    }
    p2sdLoadPcmFile(commandName, 0x7f);
    return 1;
}

u32 sndOpUpdateTitleBgmState(void) {
    mnuTitleStreamUpdateAndLogBgm();
    return 1;
}

INCLUDE_ASM(const s32, "code_r1_001CF808", slCallGetPcmState);

INCLUDE_ASM(const s32, "code_r1_001CF808", slLeaf3);

INCLUDE_ASM(const s32, "code_r1_001CF808", slHardSelect1);

INCLUDE_ASM(const s32, "code_r1_001CF808", slCallBgmPlay);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCloseSkyEventTask);

INCLUDE_ASM(const s32, "code_r1_001CF808", At3Play);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCallBgmFadeOut);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCallBgmContinue);

u8 mnuIsTitleStreamIdle(void) {
    MnuTitleStreamLoadState loadState;

    loadState = mnuPollTitleStreamStateLocked();
    return loadState == MNU_STREAM_LOAD_IDLE;
}

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCallSePlay);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtFunc7);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCallEffectShutdown);

INCLUDE_ASM(const s32, "code_r1_001CF808", evtCallSeFadeOut);

/* Flush EE cache, submit one word-sized transfer to IOP, and busy-wait for DMA.
 * Return the native request ID; there is no failure retry or timeout here. */
s32 sndCopyWordsToIopSynchronously(const void *source, u32 iopAddress, u32 wordCount) {
    sifDmaTransfer_t transfer;
    s32 dmaRequest;
    transfer.source = (u32)source;
    transfer.destination = iopAddress;
    transfer.size = wordCount * SND_TRANSFER_WORD_BYTES;
    transfer.attributes = 0;
    FlushCache(0);
    dmaRequest = sceSifSetDma(&transfer, 1);
    while (sceSifDmaStat(dmaRequest) >= 0) {
    }
    return dmaRequest;
}

/* Zero one EE staging buffer, then seed both halves of the IOP allocation. */
void sndInitializeStreamTransferBuffers(s32 wordCount) {
    s32 bufferBytes = wordCount * SND_TRANSFER_WORD_BYTES;
    s32 halfwordIndex;

    D_003BC5B8 = sdfMemGetPtr(sdfMemAllocDirect(bufferBytes));
    for (halfwordIndex = 0; halfwordIndex < wordCount * SND_TRANSFER_WORD_HALFWORDS; halfwordIndex++) {
        ((u16 *)D_003BC5B8)[halfwordIndex] = 0;
    }
    sceSifInitIopHeap();
    sndStreamTransferIopHeapAddress = sceSifAllocIopHeap(wordCount * SND_TRANSFER_PAIR_WORD_BYTES);
    if (sndStreamTransferIopHeapAddress <= 0) {
        Exit(0);
    }
    D_003BC5B0[0] = sndStreamTransferIopHeapAddress;
    D_003BC5B0[1] = sndStreamTransferIopHeapAddress + bufferBytes;
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[0], wordCount);
    sndCopyWordsToIopSynchronously((const void *)D_003BC5B8, D_003BC5B0[1], wordCount);
}

INCLUDE_ASM(const s32, "code_r1_001CF808", evtFunc8);

void mnuInitTitleSoundRemoteRequest(u32 wordCount) {
    sceSdRemoteInit();
    sndInitializeStreamTransferBuffers(wordCount);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "code_r1_001CF808", mnuCopySoundStreamOnStatusChange);
#ifdef VERSION_RAIDOU2

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrOpTitle);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr600001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr681001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr680002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr690004);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr696001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr695001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr690005);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr6900042);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr690003);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr690002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr690001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr687001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr685001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr680001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr621001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr620001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr619001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr617002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr617001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr616001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr615001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr614002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr614001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr613002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr613001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr612001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr611004);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr611003);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr611002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr611001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr610001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr609005);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr609004);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr609003);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr609002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr609001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr606002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr606001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr605003);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr605002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr605001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr604003);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr604002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr604001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr603002);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr603001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr602001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStr601001);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle06);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle05);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle04);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle03);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle02);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrBattle01);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrEpilogue);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrMilkHall);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrAyashii2);

INCLUDE_RODATA(const s32, "code_r1_001CF808", evtStrKanashii);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon0);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon02);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon03);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon09);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon08);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon07);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon06);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon05);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon04);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon032);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon022);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMahjon01);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse50);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse4);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse42);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse43);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse44);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse45);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse46);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse49);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse48);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse47);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse462);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse452);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse442);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse432);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse422);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse41);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse40);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse3);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse32);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse33);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse34);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse35);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse36);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse39);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse38);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse37);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse362);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse352);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse342);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse332);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse322);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse31);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse30);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse2);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse22);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse23);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse24);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse25);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse26);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse29);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse28);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse27);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse262);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse252);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse242);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse232);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse222);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse21);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse20);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse1);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse12);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse13);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse14);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse15);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse16);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse19);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse18);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse17);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse162);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse152);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse142);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse132);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse122);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse11);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse10);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse0);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse02);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse03);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse04);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse05);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse06);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse09);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse08);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse07);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse062);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse052);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse042);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse032);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse022);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse01);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrMapse00);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe11);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe10);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe0);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe02);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe03);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe04);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe05);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe06);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe09);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe08);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe07);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe062);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe052);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe042);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe032);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe022);

INCLUDE_RODATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3StrEveSe01);

#endif
INCLUDE_SDATA(const s32, "code_r1_001CF808", evtPicturePriList);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndInitData2);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndPathPrefix);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndPathPrefix2);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtPicturePriProcName);

INCLUDE_SDATA(const s32, "code_r1_001CF808", fldNKIYData);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndInitData3);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndInitData4);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndData);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndInitializeStreamTransferBuffersData);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuCopySoundStreamOnStatusChangeData);

#ifdef VERSION_RAIDOU2
INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData30);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData31);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData32);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData33);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData34);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData35);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData36);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData37);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData38);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData39);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData40);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData41);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData42);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData43);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData44);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData45);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData46);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData47);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData48);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData49);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData50);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData51);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData52);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData53);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData54);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData55);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData56);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData57);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData58);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData59);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData60);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData61);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData62);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData63);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData64);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData65);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData66);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData67);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData68);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData69);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData70);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData71);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData72);

INCLUDE_SDATA(const s32, "code_r1_001CF808", evtData73);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry2);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry3);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry4);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry5);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry6);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry7);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry8);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry9);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry10);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry11);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry12);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry13);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry14);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry15);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry16);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry17);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry18);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry19);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry20);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry21);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry22);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry23);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry24);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry25);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry26);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry27);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry28);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry29);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry30);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry31);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry32);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry33);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry34);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry35);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry36);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry37);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry38);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry39);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry40);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry41);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry42);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry43);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry44);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry45);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry46);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry47);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry48);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry49);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry50);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry51);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry52);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry53);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry54);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry55);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry56);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry57);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry58);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry59);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry60);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry61);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry62);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry63);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry64);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry65);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry66);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry67);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry68);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry69);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry70);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry71);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuQueueTitleSoundStreamEntryData3Entry72);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry2);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry3);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry4);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry5);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry6);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry7);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry8);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry9);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry10);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry11);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry12);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry13);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry14);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry15);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry16);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry17);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry18);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry19);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry20);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry21);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry22);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry23);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry24);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry25);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry26);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry27);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry28);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry29);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry30);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry31);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry32);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry33);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry34);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry35);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry36);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry37);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry38);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry39);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry40);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry41);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry42);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry43);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry44);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry45);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry46);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry47);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry48);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry49);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry50);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry51);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry52);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry53);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry54);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry55);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry56);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry57);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry58);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry59);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry60);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry61);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry62);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry63);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry64);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry65);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry66);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry67);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry68);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry69);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry70);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry71);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry72);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry73);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry74);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry75);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry76);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry77);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry78);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry79);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry80);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry81);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry82);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry83);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry84);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry85);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry86);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry87);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry88);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry89);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry90);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry91);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry92);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry93);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry94);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry95);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry96);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry97);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry98);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry99);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry100);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry101);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry102);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry103);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry104);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry105);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry106);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry107);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry108);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry109);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry110);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry111);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry112);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry113);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry114);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry115);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry116);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry117);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry118);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry119);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry120);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry121);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry122);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry123);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry124);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry125);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry126);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry127);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry128);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry129);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry130);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry131);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry132);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry133);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry134);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry135);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry136);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry137);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry138);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry139);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry140);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry141);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry142);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry143);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry144);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry145);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry146);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry147);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry148);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry149);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry150);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry151);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry152);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry153);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry154);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry155);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry156);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry157);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry158);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry159);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry160);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry161);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry162);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry163);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry164);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry165);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry166);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry167);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry168);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry169);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry170);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry171);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry172);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry173);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry174);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry175);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry176);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry177);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry178);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry179);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry180);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry181);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry182);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry183);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry184);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry185);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry186);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry187);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry188);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry189);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry190);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry191);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry192);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry193);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry194);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry195);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry196);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry197);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry198);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry199);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry200);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry201);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry202);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry203);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry204);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry205);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry206);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry207);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry208);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry209);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry210);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry211);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry212);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry213);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry214);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry215);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry216);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry217);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry218);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry219);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry220);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry221);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry222);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry223);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry224);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry225);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry226);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry227);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry228);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry229);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry230);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry231);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry232);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry233);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry234);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry235);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry236);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry237);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry238);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry239);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry240);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry241);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry242);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry243);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry244);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry245);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry246);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry247);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry248);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry249);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry250);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry251);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry252);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry253);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry254);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry255);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry256);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry257);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry258);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry259);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry260);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry261);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry262);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry263);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry264);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry265);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry266);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry267);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry268);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry269);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry270);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry271);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry272);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry273);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry274);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry275);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry276);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry277);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry278);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry279);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry280);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry281);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry282);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry283);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry284);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry285);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry286);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry287);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry288);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry289);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry290);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry291);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry292);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry293);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry294);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry295);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry296);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry297);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry298);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry299);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry300);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry301);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry302);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry303);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry304);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry305);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry306);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry307);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry308);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry309);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry310);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry311);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry312);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry313);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry314);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry315);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry316);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry317);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry318);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry319);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry320);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry321);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry322);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry323);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry324);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry325);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry326);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry327);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry328);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry329);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry330);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry331);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry332);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry333);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry334);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry335);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry336);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry337);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry338);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry339);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry340);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry341);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry342);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry343);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry344);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry345);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry346);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry347);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry348);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry349);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry350);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry351);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry352);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry353);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry354);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry355);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry356);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry357);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry358);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry359);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry360);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry361);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry362);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry363);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry364);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry365);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry366);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry367);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry368);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry369);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry370);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry371);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry372);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry373);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry374);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry375);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry376);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry377);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry378);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry379);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry380);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry381);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry382);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry383);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry384);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry385);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry386);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry387);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry388);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry389);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry390);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry391);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry392);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry393);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry394);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry395);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry396);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry397);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry398);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry399);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry400);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry401);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry402);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry403);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry404);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry405);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry406);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry407);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry408);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry409);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry410);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry411);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry412);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry413);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry414);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry415);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry416);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry417);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry418);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry419);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry420);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry421);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry422);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry423);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry424);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry425);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry426);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry427);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry428);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry429);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry430);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry431);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry432);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry433);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry434);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry435);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry436);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry437);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry438);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry439);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry440);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry441);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry442);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry443);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry444);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry445);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry446);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry447);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry448);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry449);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry450);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry451);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry452);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry453);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry454);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry455);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry456);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry457);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry458);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry459);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry460);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry461);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry462);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry463);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry464);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry465);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry466);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry467);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry468);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry469);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry470);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry471);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry472);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry473);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry474);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry475);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry476);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry477);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry478);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry479);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry480);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry481);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry482);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry483);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry484);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry485);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry486);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry487);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry488);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry489);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry490);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry491);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry492);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry493);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry494);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry495);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry496);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry497);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry498);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry499);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry500);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry501);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry502);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry503);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry504);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry505);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry506);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry507);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry508);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry509);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry510);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry511);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry512);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry513);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry514);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry515);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry516);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry517);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry518);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry519);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry520);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry521);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry522);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry523);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry524);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry525);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry526);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry527);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry528);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry529);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry530);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry531);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry532);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry533);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry534);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry535);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry536);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry537);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry538);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry539);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry540);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry541);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry542);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry543);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry544);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry545);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry546);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry547);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry548);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry549);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry550);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry551);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry552);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry553);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry554);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry555);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry556);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry557);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry558);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry559);

INCLUDE_SDATA(const s32, "code_r1_001CF808", mnuLoadTitleSoundBufferEntryData3Entry560);

#endif
INCLUDE_SDATA(const s32, "code_r1_001CF808", sndBgmLoadId_XFData2);

INCLUDE_SDATA(const s32, "code_r1_001CF808", sndData2);

