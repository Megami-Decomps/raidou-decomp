#include "common.h"

/* Declarations from smt3-decomp src/smt3/sdf/sdfSound.c */
#include "fpu.h"

#include "common.h"

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

extern s32 sceSifLoadModule(const char *path, s32 argSize, const char *args);

extern s32 sceGetstat(const char *path, sdfSceIoStat_t *info);

extern s32 SdfIopFileRpc(const char *name, s32 dataSize, const void *data, s32 *outSize);

extern u8 sdfHddValid;

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

extern void sdfDelayThread(u32 delay);

extern char * strcpy(char *, const char *);

extern sndRingPacket_t sdfSndCmdPacketRing[32];

extern u32 sdfSndSendCmd_cmd_id;

extern vu16 sdfSndCmdIndex;

extern vu16 sdfSndCmdTailIndex;

extern s32 sdfSndCmdBuf;

extern s32 WakeupThread(s32 thread);

extern void FlushCache(s32);

extern sndListenerState_t sdfSndListenerPrevState;

char *sdfSndMakeFilename(char *aDstName, const char *aSrcName);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern s32 p2sdSmgHandle;

#include "sdk_libc.h"

extern sndFileRequestState_t sdfSndPcmRequest;

extern s32 sdfSndPcmBufferAddr;

extern char sdfSndPcmFileName[];

extern s32 sifRpcServerSema;

extern char sdfDumpAddrFmt[];

extern char sdfDumpPadStr[];

extern char sdfDumpMidGapStr[];

extern char sdfDumpByteSepStr[];

extern char sdfDumpByteFmt[];

extern char sdfDumpLineBuf[];

extern char sdfDumpEndStr[];

extern const char sdfDmaDumpStartFmt[];

extern const char sdfDmaNotQwordFmt[];

extern const char sdfDmaTagLineFmt[];

extern const char sdfDmaNestOverflowMsg[];

extern char *sdfDmaTagNameTable[];

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void);

extern const char sdfGsMemInfoTitle[];

extern const char sdfGsMemNodeFmt[];

extern char sdfGsMemTypeFmt[];

extern char *sdfGsMemTypeNames[];

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void);

extern char *sdfGsMemTypeNamesTail[];

void sdfMemGetInfo(sdfMemInfo_t *aInfo);

void sdfChipGetInfo(sdfChipInfo_t *aInfo);

extern s32 (*p2sdSmgBuf)(void);

void p2sdLoadSMGFile(s32 MidiNo);

void *sdfFileRpcHandleCmd(s32 function, void *requestData, s32 requestSize);

typedef struct sdfFileCmdHandle_t sdfFileCmdHandle_t;

extern sdfFileCmdHandle_t *sdfFileRpcArg;

extern sdfSoundRpcRequest_t *sdfFileRpcResult;

extern s32 sdfFileRpcCmdState;

/* This handler writes the first word of the RPC reply block. */
extern u32 sdfFileRpcQueue[];

/* The first cell stores the PCM base pointer. This unsized view keeps the
 * non-small-data address form; see docs/levers/addressing-and-types.md. */
extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);

extern void *sdfMemLock(sdfMemHandle_t *handle);

extern void sdfMemFreeDirect(sdfMemHandle_t *handle);

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(
    const char *path,
    void (*callback)(sdfFileCmdHandle_t *, s32, saddr_t, s32, s32),
    s32 callbackContext);

extern s32 sdfFileCmdClearError(sdfFileCmd_t *handle);

extern s32 sdfFileCmdFree(sdfFileCmd_t *handle);

extern s32 sdfFileCmdRead(void *handle, void *buffer, s32 byteCount);

extern s32 sdfFileCmdClose(void *handle);

void sdfGrFlushOtChain(u32 *source);

s32 sdfPrintf(const char *str, ...);

extern s32 sceSifAllocIopHeap(s32 size);

extern s32 sceSifQueryMaxFreeMemSize(void);

extern s32 sceSifQueryMemSize(void);

extern s32 sceSifQueryTotalFreeMemSize(void);

s32 sdfSndIopAlloc(s32 size);

extern s32 sceSifSendCmd(s32 command, void *packet, s32 packetBytes,
                         void *source, void *destination, s32 dataBytes);

s32 sdfSndCmdTrans(void);

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

extern sdfSndInitThreadParam_t sdfSndCmdThreadParam;

extern u8 sdfSndCmdThreadStack[];

extern u8 sdfSifRpcThreadStack[];

extern s32 sdfSifRpcThreadId;

extern s32 _gp;

extern s32 sceSifGetSreg(s32 index);

extern s32 CreateThread(sdfSndInitThreadParam_t *parameter);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern void SifRpcServerThreadProc(void);

void sdfSndInitCmdIF(void);

s32 sdfSndSendCmd(s32 aCmd, s32 aParam, void *aExDataPtr, s32 aExDataSize);

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

void sdfSndLoadIRXModules(void);

extern char sdfSndIopSizeMsg[];

extern const char sdfSndSeqMapInitMsg[];

void sdfSndVoiceMapInit(char *aVoiceInfo, s32 aVoiceCount);

void sdfSndVoiceMapInit(char *aVoiceInfo, s32 aVoiceCount);

void sdfSndSeqMapInit(u32 *aSequenceInfo, s32 aSequenceCount);

extern s32 sceSifInitIopHeap(void);

extern s32 sdfSndIopWorkAddr;

extern const char sdfSndSeqMapInitMsg[18];

void sdfSndInit(char *aVoiceInfo, s32 aVoiceCount, char *aSequenceInfo, s32 aSequenceCount);

/* Starts a track with the default 0x7f setting after preparing its identifier. */
void p2sdPlayBGM(s32 MidiNo);

s32 p2sdSendStopCmd(s32 aId);

/* Starts the alternate track command, leaving the other packet fields untouched. */
void p2sdResumeBgm(s32 trackId);

/* Command 0x30 copies a full 16-byte payload; only its identifier is set. */
void p2sdStopSe(u32 soundId);

s32 p2sdStopAllBgm(void);

s32 sdfSndQueueNoPayloadCommand210(void);

s32 p2sdStopAllSe(void);

s32 sdfSndDispatchCommandWithoutPayload(s32 aSeId);

/* Send a NUL-terminated command string; the transport excludes the terminator. */
u32 sdfSndLoadHDFile(s32 aVcChNum, char *aFilename);

u32 sdfSndLoadBDFile(s32 aVcChNum, char *aFilename);

/* Send text with command bit 0x80; payload length excludes its NUL terminator. */
u32 sdfSndLoadSQFile(s32 aSqChNum, char *aFilename);

/* Send a 12-byte sequence command; the trailing struct word is not transmitted. */
void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

/* The angle normalization and attenuation share one frame through the SE tail call. */
void p2sdPlaySeDeg(s32 trackId, f32 angle, f32 distance);

/* Converts world coordinates to the sound engine's one-tenth scale. */
void p2sdPlaySe3DEx(s32 MidiNo, s32 posx, f32 posy, f32 posz, f32 limit);

/* Scale six world-space values and send them to the sound engine when they changed. */
void p2sdSetStandPos(f32 Cposx, f32 Cposy, f32 Cposz,
                                 f32 Pposx, f32 Pposy, f32 Pposz);

extern void InvalidDCache(void *start, void *end);

s32 p2sdLoadCheckMidi(s32 MidiNo);

/* Make sure the MIDI bank named by the packed track id is resident, loading it if not. */
void p2sdLoadSMGFile(s32 MidiNo);

u64 sdfSndSendFilenameCommand(const char *aName);

void p2sdSetPcmBuffer(saddr_t data, s32 size);

u64 sdfSndSendChannelControlCommand(s32 aCh);

u64 sdfSndSendChannelGroupCommand140(u8 aCh);

u64 sdfSndSendChannelGroupCommand150(u8 aCh);

void sdfSndReleaseMidiTrack(s32 id);

u8 sdfSndGetTrackBalanceFirstComponent(s32 aIndex);

u8 sdfSndGetTrackBalanceSecondComponent(s32 aIndex);

s32 sdfSndGetNonnegativeEntryBalance(s32 aIndex);

u8 *sdfSndGetMidiChannelTable(void);

p2sdLoadEntry_t *sdfSndGetTrackSlotTable(void);

u32 sdfSndGetMidiStateWordPair(u32 *aOutValue);

/* Pack two 24-byte MIDI state groups into out; NULL leaves the state untouched. */
void sdfSndCopyGroupedMidiStateBytes(u8 *out);

/* Sends a prepared track identifier with the 0x17f setting. */
void p2sdPlayBGMFade(s32 trackId);

s32 p2sdFadeOutBgm(s32 aId);

s32 p2sdStopBGM(s32 aId);

s32 p2sdBgmVolDown(void);

s32 p2sdBgmVolUp(void);

void p2sdSetBgmTrack(s32 aTrack);

s32 p2sdLoadPcmFile(const char *name, u8 channel);

u32 p2sdGetPcmState(void);

s32 p2sdSyncSound(void);

void sdfSndSendChannelGroupCommand110(u8 channel);

s32 sdfSndSetChannelCount(u32 aCh);

u32 p2sdSetPcmCallback(addr_t aCallback);

/* Send opcode 0x120 with the channel's group (bits 3-6) unless a command is
 * pending; returns nonzero when busy. */
s32 p2sdPlayPcm(u8 channel);

u32 sdfSndGetCommandStatus(void);

s32 p2sdSyncMovie(void);

/* Separate native copy of the command-0x110 channel-group sender. */
void sdfSndSendChannelGroupCommand110Duplicate(u8 channel);

s32 sdfFileRpcCallback(s32 unused, u32 event);

/* Handle sound-file and PCM-buffer RPC commands. */
extern s32 WaitSema(s32 sema);

void *sdfFileRpcHandleCmd(s32 function, void *requestData, s32 requestSize);

extern s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);

extern s32 GetThreadId(void);

void SifRpcServerThreadProc(void);

void sdfSndSetTableEntry(u32 kind, const u8 *data);

void sdfSndGetTableEntry(u32 kind, u8 *data);

/* Print size bytes in address-aligned, sixteen-column hexadecimal rows. */
void sdfDumpHexLine(const u8 *data, s32 size);

extern const char sdfDmaDumpStartFmt[29];

extern const char sdfDmaNotQwordFmt[21];

extern const char sdfDmaTagLineFmt[33];

extern const char sdfDmaNestOverflowMsg[27];

void sdfDumpDmaChain(sdfDmaDumpTag_t *source);

extern char *sdfGrVifUnpackNames[];

extern s32 sdfGrVifUnpackBits[];

extern char sdfVifMaskedName[];

extern char sdfVifXferOkName[];

extern char sdfDumpIndentStr[];

extern char sdfDumpWordFmt[];

void sdfGrFlushOtChain(u32 *source);

void sdfDumpGsMemory(void);

void sdfDumpGsMemory2(void);

extern const char sdfGsMemInfoTitle[116];

extern const char sdfGsMemNodeFmt[31];

void sdfDumpMemory(void);

/* Print the chip heap totals and how many cells are in use per size class (1..16, 17..32, ...). */
void sdfDumpChipMemory(void);

extern s32 p2sdSmgHandle;

extern u32 p2sdPcmCallback;

extern u32 p2sdPcmState;

extern u32 p2sdPcmBusy;

extern u32 sdfSndSendCmd_cmd_id;

extern s32 sdfSifRpcThreadId;

extern s32 (*p2sdSmgBuf)(void);

extern char sdfDmaTagEndName[8];

extern char sdfDmaTagRetName[8];

extern char sdfDmaTagCallName[8];

extern char sdfDmaTagRefsName[8];

extern char sdfDmaTagRefName[8];

extern char sdfDmaTagNextName[8];

extern char sdfDmaTagCntName[8];

extern char sdfDmaTagRefeName[8];

extern char sdfDumpAddrFmt[8];

extern char sdfDumpPadStr[8];

extern char sdfDumpMidGapStr[8];

extern char sdfDumpByteSepStr[8];

extern char sdfDumpByteFmt[8];

extern char sdfDumpLineBuf[2];

extern char sdfDumpEndStr[8];

extern char sdfVifMaskedName[8];

extern char sdfVifXferOkName[8];

extern char sdfVifUnpackV4_5Name[8];

extern char sdfVifUnpackV4_8Name[8];

extern char sdfVifUnpackV4_16Name[8];

extern char sdfVifUnpackV4_32Name[8];

extern char sdfVifUnpackV3_8Name[8];

extern char sdfVifUnpackV3_16Name[8];

extern char sdfVifUnpackV3_32Name[8];

extern char sdfVifUnpackV2_8Name[8];

extern char sdfVifUnpackV2_16Name[8];

extern char sdfVifUnpackV2_32Name[8];

extern char sdfVifUnpackS_8Name[8];

extern char sdfVifUnpackS_16Name[8];

extern char sdfVifUnpackS_32Name[8];

extern char sdfVifFlagsMruName[8];

extern char sdfVifFlagsRuName[8];

extern char sdfVifFlagsMrName[8];

extern char sdfVifFlagsRName[8];

extern char sdfVifFlagsMuName[8];

extern char sdfVifFlagsUName[8];

extern char sdfVifFlagsMName[8];

extern char sdfVifFlagsNoneName[8];

extern char sdfDumpIndentStr[8];

extern char sdfDumpWordFmt[8];

extern char sdfGsMemClutName[8];

extern char sdfGsMemTexName[8];

extern char sdfGsMemFrameName[8];

extern char sdfGsMemFreeName[8];

extern char sdfGsMemTypeFmt[8];
/* End of declarations from src/smt3/sdf/sdfSound.c */

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036F610);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036F6D0);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036F9A0);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FA80);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FB28);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FC00);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FDF8);

s32 p2sdStopAllSe(void)
{
    return sdfSndSendCmd(0x40, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FE48);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_0036FF48);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003703A8);

INCLUDE_ASM(const s32, "sdf/sdfSound", p2sdLoadSMGFile);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370500);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370598);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003705C8);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003705F8);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370680);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370698);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003706B0);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003706D8);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003706E8);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003706F8);

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370718);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003708F8);

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

INCLUDE_ASM(const s32, "sdf/sdfSound", func_003709D0);

s32 p2sdSyncMovie(void)
{
    return sdfSndSendSyncCmd(0x100, 0, 0, 0);
}

INCLUDE_ASM(const s32, "sdf/sdfSound", func_00370A00);

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

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F43C);

s32 (*p2sdSmgBuf)(void) = NULL;

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F448);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F450);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F458);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F460);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F468);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F470);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F478);

INCLUDE_SDATA(const s32, "sdf/sdfSound", D_0046F480);

