#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "ps2asm.h"
#include "sif_rpc.h"

struct sdfGsMemSlice_t;

typedef struct {
    u32 mode;
    u32 attributes;
    u32 size;
    u8 creationTime[8];
    u8 accessTime[8];
    u8 modificationTime[8];
    u32 highSize;
    u32 privateData[6];
} sdfSceIoStat_t;

s32 strlen(const char *aStr);
s32 sdfSndCmdTrans(void);
s32 SleepThread(void);
extern s8 sdfSndBurstReqMode;

typedef struct
{
    u32 unk0;
    u8 unk4;
    u8 unk5;
    u16 unk6;
} p2sdLoadEntry_t;

s32 sdfSndSendCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize);
extern s32 SignalSema(s32 aSemaId);

/* The PCM fill hook, held as an address and called through a cast. */
typedef u8 *(*p2sdPcmFunc_t)(s32);

extern u32 p2sdPcmCallback;
extern u32 sdfSndMap[];

#define LastLoad ((p2sdLoadEntry_t *)((u8 *)sdfSndMap + 0x190))

extern u32 p2sdPcmState;
extern u32 p2sdPcmBusy;

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

typedef struct sndRingPacket_t {
    u32 transportHeader[3];
    u32 command;
    u32 sequence;
    u8 payload[108];
} sndRingPacket_t;

typedef struct sdfSndTrackPacket_t {
    u32 trackId;
    u32 unk4;
    u16 setting;
    u16 unkA;
    u32 unkC;
} sdfSndTrackPacket_t;

typedef struct sndSequenceVolumePanPacket_t {
    u32 trackId;
    u32 opaque04;
    u16 volume;
    u16 pan;
    u32 opaqueNotSent0C;
} sndSequenceVolumePanPacket_t;

typedef struct sndListenerState_t {
    s32 opaque00[2];
    s32 scaledValues[6];
} sndListenerState_t;

typedef struct sndFileRequestState_t {
    u8 path[0x100];
    s32 size;
    s32 pending;
    s32 data;
} sndFileRequestState_t;

typedef struct sndChannel_t {
    u8 index;
    u8 unk1;
    s16 unk2;
    u32 unk4;
    u32 iopWorkAddress;
    u32 unkC;
} sndChannel_t;

typedef struct sndIopBuffer_t {
    s32 size;
    u32 iopAddress;
} sndIopBuffer_t;

typedef struct {
    u8 values[24];
    u8 pad18[8];
} sndMidiStateGroup_t;

typedef struct sndMergedWork_t {
    s32 header;
    s32 bufferCount;
    u32 iopBufferAddress;
    u32 iopBufferBytes;
    sndChannel_t channels[16];
    sndIopBuffer_t buffers[16];
    p2sdLoadEntry_t slots[7];
    void *eeWorkAddress;
    u32 eeWorkByteCount;
    u32 opaque1D0;
    u32 opaque1D4;
    u32 opaque1D8;
    u32 opaque1DC;
    u8 controlData[7][0x80];
    sndMidiStateGroup_t stateGroups[2];
} sndMergedWork_t;

extern sndMergedWork_t sdfSndWork FAKEMATCH_ASM_NAME("sdfSndMap");

typedef struct sdfSoundRpcRequest_t {
    u8 opaque00[8];
    s32 destination;
    s32 byteCount;
} sdfSoundRpcRequest_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

typedef struct {
    u32 control;
    u32 address;
    u32 vifCode0;
    u32 vifCode1;
} sdfDmaDumpTag_t;

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern void sdfSndTransToIOP(s32 aIOPAddr, void *aEEAddr, s32 aSize);
extern void *memcpy(void *, const void *, size_t);
extern char * strcpy(char *, const char *);
extern sndRingPacket_t sdfSndCmdPacketRing[32];
extern u32 sdfSndSendCmd_cmd_id;
extern vu16 sdfSndCmdIndex;
extern vu16 sdfSndCmdTailIndex;
extern s32 sdfSndCmdBuf;
extern s32 WakeupThread(s32 thread);
extern void FlushCache(s32);
extern sndListenerState_t sdfSndListenerPrevState;
extern s32 p2sdLoadCheckMidi(s32 MidiNo);
extern s32 p2sdSmgHandle;

#include "sdk_libc.h"

extern sndFileRequestState_t sdfSndPcmRequest;
extern s32 sdfSndPcmBufferAddr;
extern char sdfSndPcmFileName[];
extern s32 sifRpcServerSema;
extern s32 (*p2sdSmgBuf)(void);
void p2sdLoadSMGFile(s32 MidiNo);
void *sdfFileRpcHandleCmd(s32 function, void *requestData, s32 requestSize);

typedef struct sdfFileCmdHandle_t sdfFileCmdHandle_t;

/* This handler writes the first word of the RPC reply block. */
extern u32 sdfFileRpcQueue[];
void sdfSndCmdIFThread(void);

typedef struct sdfSndInitThreadParam_t {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 reserved[0x18];
} sdfSndInitThreadParam_t;

extern s32 sdfSifRpcThreadId;
extern void SifRpcServerThreadProc(void);
void sdfSndSyncCmd(s32 aCmdId);
s32 sdfSndSendSyncCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize);

/* The SIF DMA request block, reused for every transfer. */
extern struct
{
    u32 source;
    s32 destination;
    s32 size;
    s32 attributes;
} sdfSndTransBuf;

#include "sdk_libkernl.h"

extern s32 sceSifDmaStat(s32 aId);
void sdfSndTransToIOP(s32 aIOPAddr, void *aEEAddr, s32 aSize);
void p2sdPlayBGM(s32 MidiNo);
s32 p2sdSendStopCmd(s32 aId);
void p2sdResumeBgm(s32 trackId);
void p2sdStopSe(u32 soundId);
s32 p2sdStopAllBgm(void);
s32 p2sdStopAllSe(void);
u32 sdfSndLoadHDFile(s32 aVcChNum, char *aFilename);
u32 sdfSndLoadBDFile(s32 aVcChNum, char *aFilename);
u32 sdfSndLoadSQFile(s32 aSqChNum, char *aFilename);

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);
void p2sdPlaySeDeg(s32 trackId, f32 angle, f32 distance);
void p2sdPlaySe3DEx(s32 MidiNo, s32 posx, f32 posy, f32 posz, f32 limit);

void p2sdSetStandPos(f32 Cposx, f32 Cposy, f32 Cposz,
                                 f32 Pposx, f32 Pposy, f32 Pposz);

s32 p2sdLoadCheckMidi(s32 MidiNo);
void p2sdSetPcmBuffer(saddr_t data, s32 size);
void sdfSndReleaseMidiTrack(s32 id);
void p2sdPlayBGMFade(s32 trackId);
s32 p2sdStopBGM(s32 aId);
s32 p2sdBgmVolDown(void);
s32 p2sdBgmVolUp(void);
void p2sdSetBgmTrack(s32 aTrack);
s32 p2sdLoadPcmFile(const char *name, u8 channel);
u32 p2sdGetPcmState(void);
s32 p2sdSyncSound(void);
s32 sdfSndSetChannelCount(u32 aCh);
u32 p2sdSetPcmCallback(addr_t aCallback);
s32 p2sdPlayPcm(u8 channel);
s32 p2sdSyncMovie(void);
s32 sdfFileRpcCallback(s32 unused, u32 event);
extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
extern s32 GetThreadId(void);
void SifRpcServerThreadProc(void);
extern char sdfDmaTagEndName[8];
extern char sdfDmaTagRetName[8];
extern char sdfDmaTagCallName[8];
extern char sdfDmaTagRefsName[8];
extern char sdfDmaTagRefName[8];
extern char sdfDmaTagNextName[8];
extern char sdfDmaTagCntName[8];
extern char sdfDmaTagRefeName[8];

INCLUDE_ASM(const s32, "sdf/sdfSound", sndTryDispatchQueuedCommand);

void sdfSndCmdIFThread(void)
{
    for (;;) {
        SleepThread();
        if (sdfSndBurstReqMode == 0) {
            while (sdfSndCmdTrans() == 0) {
                ;
            }
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSndInitCmdIF);

s32 sdfSndSendCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize) {
    s32 nextWriteIndex;
    s32 wakeAttemptsRemaining;
    sndRingPacket_t *entry;
    u32 transferQuadwords;
    if (++sdfSndSendCmd_cmd_id == 0) {
        sdfSndSendCmd_cmd_id = 1;
    }
    nextWriteIndex = ((s16)sdfSndCmdTailIndex + 1) & (32 - 1);
    if (nextWriteIndex == (s16)sdfSndCmdIndex) {
        for (wakeAttemptsRemaining = 8; wakeAttemptsRemaining != 0; wakeAttemptsRemaining--) {
            do {
                WakeupThread(sdfSndCmdBuf);
            } while ((s16)sdfSndCmdIndex != nextWriteIndex);
        }
        return 0;
    }
    entry = &sdfSndCmdPacketRing[(s16)sdfSndCmdTailIndex];
    entry->sequence = sdfSndSendCmd_cmd_id;
    if (aExDataSize != 0) {
        memcpy(entry->payload, aExDataPtr, aExDataSize);
    }
    /* The SIF length includes the transport/command header and rounds up to quadwords. */
    transferQuadwords = (aExDataSize + 0x14 +
                        (16 - 1)) >> 4;
    entry->command = (aCmd << 16) |
                     (aParam & 0xFFFF) |
                     (transferQuadwords << 28);
    sdfSndCmdTailIndex = nextWriteIndex;
    if (sdfSndBurstReqMode != 0) {
        while (sdfSndCmdTrans() != 0) {
        }
    } else {
        WakeupThread(sdfSndCmdBuf);
    }
    return sdfSndSendCmd_cmd_id;
}

void sdfSndSyncCmd(s32 aCmdId) {
}

s32 sdfSndSendSyncCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize)
{
    s32 handle = sdfSndSendCmd(aCmd, aParam, aExDataPtr, aExDataSize);

    sdfSndSyncCmd(handle);
    return handle;
}

void sdfSndTransToIOP(s32 aIOPAddr, void *aEEAddr, s32 aSize)
{
    s32 dmaId;
    s32 delay;

    sdfSndTransBuf.source = EE_ADDR(aEEAddr);
    sdfSndTransBuf.destination = aIOPAddr;
    sdfSndTransBuf.size = aSize;
    sdfSndTransBuf.attributes = 0;
    FlushCache(0);
    while ((dmaId = sceSifSetDma(&sdfSndTransBuf, 1)) == 0) {
        /* Spin before retrying a full SIF queue. */
        delay = 0x186A0;
        while (delay != 0) {
            delay -= 0x271;
            EE_SPIN_NOP();
        }
    }
    while (sceSifDmaStat(dmaId) >= 0) {
    }
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSndLoadIRXModules);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSndVoiceMapInit);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSndSeqMapInit);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSndInit);

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void p2sdPlayBGM(s32 MidiNo) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(MidiNo);
    packet.trackId = MidiNo;
    packet.unk4 = 0;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

s32 p2sdSendStopCmd(s32 aId)
{
    s32 arg[4];

    arg[0] = aId;
    return sdfSndSendCmd(0x30, 0, arg, sizeof(arg));
}

/* Starts the alternate track command, leaving the other packet fields untouched. */
void p2sdResumeBgm(s32 trackId) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.setting = 0x7F;
    sdfSndSendCmd(0x130, 0, &packet, 0x10);
}

/* Command 0x30 copies a full 16-byte payload; only its identifier is set. */
void p2sdStopSe(u32 soundId) {
    u32 packet[4];

    packet[0] = soundId;
    sdfSndSendCmd(0x30, 0, packet, sizeof(packet));
}

s32 p2sdStopAllBgm(void)
{
    return sdfSndSendCmd(0x1A0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfCommandPADDISABLESub);

s32 p2sdStopAllSe(void)
{
    return sdfSndSendCmd(0x40, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf31);

/* Send a NUL-terminated command string; the transport excludes the terminator. */
u32 sdfSndLoadHDFile(s32 aVcChNum, char *aFilename) {
    u32 length = strlen(aFilename);
    return sdfSndSendSyncCmd(aVcChNum | 0x70, 0, aFilename, length);
}

u32 sdfSndLoadBDFile(s32 aVcChNum, char *aFilename) {
    u32 length = strlen(aFilename);
    return sdfSndSendSyncCmd(aVcChNum | 0x60, 0, aFilename, length);
}

/* Send text with command bit 0x80; payload length excludes its NUL terminator. */
u32 sdfSndLoadSQFile(s32 aSqChNum, char *aFilename) {
    u32 length = strlen(aFilename);
    return sdfSndSendSyncCmd(aSqChNum | 0x80, 0, aFilename, length);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfSkil);

INCLUDE_ASM(const s32, "sdf/sdfSound", p2sdPlaySE);

/* The angle normalization and attenuation share one frame through the SE tail call. */
void p2sdPlaySeDeg(s32 trackId, f32 angle, f32 distance) {
    s32 degrees = (s32)angle % 360;
    s32 side = 1;
    s32 pan;
    f32 volume;

    if (degrees > 180) {
        degrees -= 360;
    }
    if (degrees < -180) {
        degrees += 360;
    }
    if (degrees < 0) {
        degrees = -degrees;
        side = -1;
    }
    volume = 1.0f;
    if (degrees < 45) {
        pan = 63 - (s32)(degrees * 0.71111f) * side;
    } else if (degrees <= 90) {
        pan = 63 - (s32)(degrees * 0.7f) * side;
        volume = 1.0f - (degrees - 45) * 0.004444f;
    } else {
        volume = 0.75f - (degrees - 90) * 0.005556f;
        pan = 63 - (s32)((180 - degrees) * 0.7f) * side;
    }
    if (distance > 1.0f) {
        volume /= distance * 0.333f + 1.0f;
    }
    p2sdPlaySE(trackId, (s32)(volume * 127.0f), pan);
}

/* Converts world coordinates to the sound engine's one-tenth scale. */
void p2sdPlaySe3DEx(s32 MidiNo, s32 posx, f32 posy, f32 posz, f32 limit) {
    u32 packet[8];
    packet[0] = MidiNo;
    packet[1] = posx;
    packet[2] = (s32)(posy * 0.1f);
    packet[3] = (s32)(posz * 0.1f);
    packet[4] = (s32)(limit * 0.1f);
    sdfSndSendSyncCmd(0x170, 0, packet, 0x20);
}

/* Scale six world-space values and send them to the sound engine when they changed. */
void p2sdSetStandPos(f32 Cposx, f32 Cposy, f32 Cposz,
                                 f32 Pposx, f32 Pposy, f32 Pposz) {
    sndListenerState_t state;
    state.scaledValues[0] = Cposx * 0.1f;
    state.scaledValues[1] = Cposy * 0.1f;
    state.scaledValues[2] = Cposz * 0.1f;
    state.scaledValues[3] = Pposx * 0.1f;
    state.scaledValues[4] = Pposy * 0.1f;
    state.scaledValues[5] = Pposz * 0.1f;
    if (sdfSndListenerPrevState.scaledValues[0] != state.scaledValues[0] || sdfSndListenerPrevState.scaledValues[1] != state.scaledValues[1] ||
        sdfSndListenerPrevState.scaledValues[2] != state.scaledValues[2] || sdfSndListenerPrevState.scaledValues[3] != state.scaledValues[3] ||
        sdfSndListenerPrevState.scaledValues[4] != state.scaledValues[4] || sdfSndListenerPrevState.scaledValues[5] != state.scaledValues[5]) {
        sdfSndSendSyncCmd(0x160, 0, &state, 0x20);
        sdfSndListenerPrevState = state;
    }
}

INCLUDE_ASM(const s32, "sdf/sdfSound", p2sdLoadCheckMidi);

INCLUDE_ASM(const s32, "sdf/sdfSound", p2sdLoadSMGFile);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfCallStrlen);

void p2sdSetPcmBuffer(saddr_t data, s32 size) {
    s32 *packet;
    sdfSndPcmRequest.size = size;
    sdfSndPcmRequest.pending = 1;
    sdfSndPcmRequest.data = data;
    if (data != 0 && size > 0) {
        packet = &sdfSndPcmBufferAddr;
        *packet = data;
        sdfSndSendSyncCmd(0x1B0, 0, packet, 4);
    }
    p2sdSmgHandle = -1;
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sndSendChannelControlCommand);

INCLUDE_ASM(const s32, "sdf/sdfSound", sndSendChannelGroupCommand150);

INCLUDE_ASM(const s32, "sdf/sdfSound", sndSendChannelGroupCommand140);

void sdfSndReleaseMidiTrack(s32 id) {
    u32 packet[4];
    if (p2sdLoadCheckMidi(id) != 0) {
        packet[0] = id;
        sdfSndSendSyncCmd(0xB0, 0, packet, 0x10);
        id >>= 16;
        if (p2sdSmgHandle == id) {
            p2sdSmgHandle = -1;
        }
    }
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf32);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf33);

INCLUDE_ASM(const s32, "sdf/sdfSound", sndGetNonnegativeEntryBalance);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf34);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf35);

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf36);

INCLUDE_ASM(const s32, "sdf/sdfSound", sndCopyGroupedMidiStateBytes);

/* Sends a prepared track identifier with the 0x17f setting. */
void p2sdPlayBGMFade(s32 trackId) {
    sdfSndTrackPacket_t packet;
    p2sdLoadSMGFile(trackId);
    packet.trackId = trackId;
    packet.unk4 = 0;
    packet.setting = 0x17F;
    sdfSndSendCmd(0x20, 0, &packet, 0x10);
}

s32 p2sdStopBGM(s32 aId)
{
    s32 arg[4];

    arg[0] = aId;
    return sdfSndSendCmd(0xD0, 0, arg, sizeof(arg));
}

s32 p2sdBgmVolDown(void)
{
    return sdfSndSendCmd(0x180, 0, 0, 0);
}

s32 p2sdBgmVolUp(void)
{
    return sdfSndSendCmd(0x190, 0, 0, 0);
}

void p2sdSetBgmTrack(s32 aTrack)
{
    sdfSndSendCmd(0xE0 | ((aTrack + 1) & 0xF), 0, NULL, 0);
}

s32 p2sdLoadPcmFile(const char *name, u8 channel) {
    if (p2sdPcmState != 0) {
        return 1;
    }
    strcpy(sdfSndPcmFileName, name);
    sdfSndSendSyncCmd((channel >> 3) | 0xF0, 0, 0, 0);
    return 0;
}

u32 p2sdGetPcmState(void)
{
    return p2sdPcmState;
}

s32 p2sdSyncSound(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sndSendChannelGroupCommand110Duplicate);

s32 sdfSndSetChannelCount(u32 aCh)
{
    if (aCh > 16) {
        aCh = 16;
    }
    if (aCh == 0) {
        aCh = 1;
    }
    return sdfSndSendSyncCmd((aCh - 1) | 0x1D0, 0, NULL, 0);
}

u32 p2sdSetPcmCallback(addr_t aCallback)
{
    if (p2sdPcmBusy != 0) {
        return 0;
    }
    p2sdPcmCallback = aCallback;
    return aCallback;
}

/* Send opcode 0x120 with the channel's group (bits 3-6) unless a command is
 * pending; returns nonzero when busy. */
s32 p2sdPlayPcm(u8 channel) {
    if (p2sdPcmBusy) {
        return 1;
    }
    channel = (channel >> 3) & 0xF;
    sdfSndSendSyncCmd(0x120 | channel, 0, 0, 0);
    return 0;
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfLeaf37);

s32 p2sdSyncMovie(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sndSendChannelGroupCommand110);

s32 sdfFileRpcCallback(s32 unused, u32 event) {
    switch (event) {
    case 1:
        break;
    case 3:
        break;
    case 6:
        break;
    case 5:
        FlushCache(0);
        /* Fall through: flush and wake the waiting thread. */
    case 4:
        SignalSema(sifRpcServerSema);
        break;
    case 0:
    case 2:
    case 7:
        SignalSema(sifRpcServerSema);
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "sdf/sdfSound", sdfFileRpcHandleCmd);

void SifRpcServerThreadProc(void)
{
    sifRpcDataQueue_t queue;
    sifRpcServerData_t server;
    s32 sema = sdfCreateSema(0, 1, 0);

    sifRpcServerSema = sema;
    if (sema <= 0) {
        for (;;) {
        }
    }
    sceSifSetRpcQueue(&queue, GetThreadId());
    sceSifRegisterRpc((struct sifRpcServerData_t *)&server, 0x54524E53, (sceSifRpcFunc)sdfFileRpcHandleCmd, sdfFileRpcQueue, 0, NULL, (struct sifRpcDataQueue_t *)&queue);
    sceSifRpcLoop(&queue);
}
s32 p2sdSmgHandle = -1;

u32 p2sdPcmCallback = 0;

u32 p2sdPcmState = 0;

u32 p2sdPcmBusy = 0;

u32 sdfSndSendCmd_cmd_id = 0;

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfSifRpcThreadId);

s32 (*p2sdSmgBuf)(void) = NULL;

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagEndName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagRetName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagCallName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagRefsName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagRefName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagNextName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagCntName);

INCLUDE_SDATA(const s32, "sdf/sdfSound", sdfDmaTagRefeName);

