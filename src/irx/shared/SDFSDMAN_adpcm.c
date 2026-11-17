#include "sdfsnd.h"

/* SDFSDMAN.IRX's last translation units, the same in both games: the EE RPC
 * call, the ADPCM stream and the import tables. The declarations are
 * src/irx/<version>/SDFSDMAN.c's. */
extern const char fmt[];
extern const char aSdfsdmanIFThre[];
extern const char aSdfsnddrvSOpen[];
extern const char aSdfsndfileread[];
extern const char aSdfsnddrvScems[];
extern const char aHardTimerAlloc[];
extern const char aTimerHandlerRe[];
extern const char aHardTimerSetup[];
extern const char aHardTimerRelea[];
extern const char aHardTimerStart[];
extern const char aHardTimerStopF[];
extern const char aA1[];
extern const char aA2[];
extern const char aB0[];
extern const char aB1[];
extern const char aC[];
extern const char aD[];
extern const char aAd[];
extern const char aSdfsnddrvScemi[];
extern const char aSdfsnddrvScemi_0[];
extern const char aSdfsnddrvScemi_1[];
extern const char aSdfsnddrvScehs[];
extern const char aSdfsnddrvScemi_2[];
extern const char aSdfsnddrvScemi_3[];
extern const char aSdfsnddrvPlayS[];
extern const char aSdfsnddrvScemi_4[];
extern const char aSdfsnddrvScehs_0[];
extern const char aSdfsnddrvSeSta[];
extern const char aSdfsnddrvFileR[];
extern const char aSdfsnddrvMidi0[];
extern const char aSdfsnddrvSData[];
extern const char aSdfsnddrvDataS[];
extern const char aSdfsnddrvSRead[];
extern const char aSdfsnddrvScehs_1[];
extern const char aSdfsnddrvCreat[];
extern const char aSdfsnddrvWaits[];
extern const char aP2sdirxspuiopm[];
extern const char aP2sdirxspuiopm_0[];
extern const char aSdfsnddrvScesi[];
extern const char aSdfsnddrvIfcou[];
extern const char aSdfsnddrvUnkno[];
extern const char aSdfadpcmStartF[];
extern const char aSdfadpcmAdpcmS[];
extern const char aA[];
extern const char aB[];
extern const char aSdfadpcmAdpcmN[];
extern const char aSdfadpcmAdpcmS_0[];
extern const char aSdfadpcmCreate[];
extern const char aSdfadpcmStartA[];
extern const char aSdfadpcmAdpcmF[];
extern const char a02xh[];
extern const char aSdfadpcmOnMemo[];
extern const char aSdfadpcmMono[];
extern const char aSdfadpcmStreo[];
extern const char aSdfadpcmFreqSL[];
extern const char aSdfadpcmAdpcmM[];
extern const char aSdfadpcmAdpcmE[];

s32 sdfSdManInit(void);
s32 sdfSndIsqrt(s32 v);
s32 sdfSndAtan2Deg(s32 x, s32 y);
s32 sdfSndPostError(s32 code);
s32 sdfSndUpdateVoiceMap(void);
s32 sdfSndFileOpen(const char *path, s32 mode);
s32 sdfSndFileRead(s32 fd, void *buf, s32 len, s32 mode);
s32 sdfSndFileGetSize(s32 fd);
s32 sdfSndFileClose(s32 fd);
u8 *sdfSndGetVolTable(s32 kind, s32 *count);
s32 sdfSndApplyVolume(s32 id, s32 slot, s32 vol);
s32 sdfSndPutMsg(s32 slot);
s32 sdfSndIsSlotMuted(s32 id);
s32 sdfSndMidiCbIgnore1(void);
s32 sdfSndMidiCbIgnore2(void);
s32 sdfSndMidiCbMessage(u32 msg, s32 slot);
s32 sdfSndTimerHandler(sdfSndTimer_t *tm);
s32 sdfSndTimerSetup(sdfSndTimer_t *tm);
s32 sdfSndTimerRelease(sdfSndTimer_t *tm);
s32 sdfSndTimerStart(sdfSndTimer_t *tm);
s32 sdfSndTimerStop(sdfSndTimer_t *tm);
s32 sdfSndInitWorkAreas(void);
void sdfSndSeqThread(void *arg);
s32 sdfSndInitSpu2(void);
s32 sdfSndMidiSelect(s32 slot, s32 seq, s32 bank, void *data, s32 loc);
s32 sdfSndStopAll(s32 which);
s32 sdfSndInitSynth(sdfSndWorkArea_t *work);
s32 sdfSndQuitSynth(void);
s32 sdfSndFindSlotById(s32 idPacked);
s32 sdfSndBgmPlay(sdfSndBgmReq_t *req);
s32 sdfSndSaveResumePoint(s32 slot);
sdfSndResume_t *sdfSndFindResumePoint(s32 id);
s32 sdfSndBgmResume(sdfSndBgmReq_t *req);
s32 sdfSndBgmStop(s32 *req);
s32 sdfSndHsynLoad(s32 bank, s32 slot);
s32 sdfSndSePlay(sdfSndSeReq_t *req);
s32 sdfSndDistanceAtten(s32 dist, s32 radius);
s32 sdfSndUpdate3dVoice(s32 slot);
s32 sdfSndSePlayPositional(s32 id, s32 angle, s32 dist, s32 radius);
s32 sdfSndSetListener(sdfSndListenerReq_t *req);
s32 sdfSndSePlay3d(sdfSndSe3dReq_t *req);
s32 sdfSndIsBankLoaded(s32 kind);
s32 sdfSndLoadSmg(const char *path, s32 mode);
void sdfSndSmgLoadThread(void *arg);
s32 sdfSndSetBankAddr(s32 kind, u32 waveSize);
s32 sdfSndStopByGroup(s32 group);
s32 sdfSndUnloadBank(s32 kind);
s32 p2sdIrxSpuIopMap(void);
s32 sdfSndFadeOutById(s32 *req);
s32 sdfSndRequestBgmSwitch(s32 midi);
s32 sdfSndAllocBgmSlot(void);
s32 sdfSndAllocSeSlot(void);
s32 sdfSndSetSeqVolume(s32 vol);
s32 sdfSndFadeOutAll(void);
s32 sdfSndFadeInAll(void);
s32 sdfSndSifCmdHandler(sdfSndCmd_t *pkt, void *harg);
s32 sdfSndCreateRpcSema(void);
s32 sdfSndLoadVolTables(void);
s32 sdfSndStoreVolTables(void);
void sdfSndKeepAliveThread(void *arg);
s32 sdfSndStartKeepAliveThread(void);
s32 sdfSndDrvRpcInit(void *arg);
s32 sdfSndCallEeRpc(s32 arg, void *buf, s32 len, u32 fno, u32 *reply);
s32 sdfAdpcmFadeTick(void);
s32 sdfAdpcmFadeOut(s32 step);
s32 sdfAdpcmStop(void);
s32 sdfAdpcmUploadBlock(void);
s32 sdfAdpcmSpu2IntrHandler(void *arg);
s32 sdfAdpcmFillBuffer(s32 blocking);
s32 sdfAdpcmSetVolume(s32 vol);
s32 sdfAdpcmStreamPlay(const char *path, s32 vol, s8 onMemory);
void sdfAdpcmStreamInit(void *arg);

ALIGN_TEXT(4);

#ifdef NON_MATCHING
/* 8 instructions short of the retail code. */
/* FUNC sdfSndCallEeRpc */
/* Ask the EE to do something for us: read a block of the stream, open a
 * file, and so on. */
s32 sdfSndCallEeRpc(s32 arg, void *buf, s32 len, u32 fno, u32 *reply)
{
    sdfSndEeReq_t send;
    u32 recv[4];
    u32 a;
    u32 b;
    u32 c;

    thsemap_WaitSema(sdfSndEeRpcSema);
    send.buf = buf;
    send.arg = arg;
    send.len = len;
    while (sifcmd_SifCheckStatRpc(&sdfSndEeRpcClient)) {
    }
    sifcmd_SifCallRpc(&sdfSndEeRpcClient, fno, 0, &send, 16, recv, 16, NULL, NULL);
    if (reply != NULL) {
        a = recv[1];
        b = recv[2];
        c = recv[3];
        reply[0] = recv[0];
        reply[1] = a;
        reply[2] = b;
        reply[3] = c;
    }
    return thsemap_SignalSema(sdfSndEeRpcSema);
}
/* ENDF sdfSndCallEeRpc */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndCallEeRpc);
#endif

ALIGN_TEXT(4);

/* FUNC sdfAdpcmFadeTick */
/* One step of the stream fade out, called once per sequence tick. */
s32 sdfAdpcmFadeTick(void)
{
    s32 frac;

    if (sdfAdpcm.playing == 0 || sdfAdpcm.fadeStep == 0) {
        return sdfAdpcm.playing;
    }
    frac = sdfAdpcm.fadeFrac + sdfAdpcm.fadeStep;
    sdfAdpcm.fadeFrac = frac;
    if (frac >= 16) {
        sdfAdpcm.fadeFrac = frac & 0xF;
        sdfAdpcm.volume -= (frac >> 4) & 0xF;
    }
    if (sdfAdpcm.volume > 0) {
        return sdfAdpcmSetVolume(sdfAdpcm.volume);
    }
    return sdfAdpcmStop();
}

s32 sdfAdpcmFadeOut(s32 step)
{
    if (sdfAdpcm.playing) {
        sdfAdpcm.fadeStep = step;
    }
}

s32 sdfAdpcmStop(void)
{
    u32 voices;

    sdfAdpcm.playing = 0;
    voices = (1 << sdfAdpcm.voiceL) | (1 << sdfAdpcm.voiceR);
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 0);
    libsd_sceSdSetParam(0xF80, 0);
    libsd_sceSdSetParam(0x1080, 0);
    libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1600, voices);
    return thsemap_SignalSema(sdfAdpcmIntrSema);
}

#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* FUNC sdfAdpcmUploadBlock */
/* Send the block the reader just filled to the SPU2 and point the voice at
 * it. Stereo splits the 512 bytes into two 256 byte halves. */
s32 sdfAdpcmUploadBlock(void)
{
    u32 spuAddr;
    u16 entry;
    s32 off;
    s32 next;

    if (sdfAdpcm.stereo) {
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9)),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 8), 0x100) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9) + 256),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 8) + 768,
                                     0x100) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        spuAddr = sdfAdpcm.spuAddr;
        entry = (s8)sdfAdpcm.coreId | 0x1F00;
        off = sdfAdpcm.xferBlk << 8;
    } else {
        while (libsd_sceSdVoiceTrans(sdfAdpcm.core, 0,
                                     (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.xferIdx << 9)),
                                     sdfAdpcm.spuAddr + (sdfAdpcm.xferBlk << 9), 0x200) < 0) {
        }
        libsd_sceSdVoiceTransStatus(sdfAdpcm.core, 1);
        spuAddr = sdfAdpcm.spuAddr;
        entry = (s8)sdfAdpcm.coreId | 0x1F00;
        off = sdfAdpcm.xferBlk << 9;
    }
    libsd_sceSdSetAddr(entry, spuAddr + off);
    next = sdfAdpcm.xferBlk + 1;
    sdfAdpcm.xferIdx = (sdfAdpcm.xferIdx + 1) & 1;
    sdfAdpcm.xferBlk = next % 3;
}
/* ENDF sdfAdpcmUploadBlock */
#else
INCLUDE_ASM("SDFSDMAN", sdfAdpcmUploadBlock);
#endif

/* FUNC sdfAdpcmSpu2IntrHandler */
s32 sdfAdpcmSpu2IntrHandler(void *arg)
{
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 0);
    sdfAdpcm.intrState = 2;
    thsemap_iSignalSema(sdfAdpcmIntrSema);
    libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 1);
    return 0;
}

#ifdef NON_MATCHING
/* 4 instructions longer than the retail code. */
/* Read the next 512 bytes of the stream from the EE and write the ADPCM
 * block flags: 6 on the first block, 2 while it runs, 3 on the last. */
s32 sdfAdpcmFillBuffer(s32 blocking)
{
    u32 reply[4];
    u8 *buf;
    u8 *left;
    u8 *right;
    s32 i;
    s32 count;
    s32 next;

    sdfAdpcm.intrState = 0;
    if (sdfAdpcm.playing == 0) {
        return sdfAdpcm.playing;
    }
    if (sdfAdpcm.bytesLeft == 0) {
        return sdfAdpcmStop();
    }
    buf = (u8 *)(sdfAdpcm.iopBuf + ((s8)sdfAdpcm.fillIdx << 9));
    do {
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, buf, 512, 7, reply);
        } else {
            sdfSndCallEeRpc(0, buf, 512, 1, reply);
        }
    } while (blocking && (reply[0] & 0x80000000) != 0);

    if (sdfAdpcm.fadeState == 1) {
        if ((reply[0] & 0x80000000) != 0) {
            sdfAdpcm.fadeState = 2;
            sdfAdpcmSetVolume(0);
        }
    } else if (sdfAdpcm.fadeState == 2) {
        if ((reply[0] & 0x80000000) == 0) {
            sdfAdpcm.fadeState = 1;
            sdfAdpcmSetVolume(sdfAdpcm.volume);
        }
    }

    left = buf + 1;
    right = buf + 257;
    if (sdfAdpcm.fillBlk != 0) {
        i = 0;
    } else {
        left = buf + 17;
        right = buf + 273;
        i = 1;
        buf[1] = 6;
        buf[257] = 6;
    }
    count = 31;
    if (sdfAdpcm.stereo) {
        count = 15;
    }
    for (; i < count; i++) {
        *right = 2;
        *left = 2;
        left += 16;
        right += 16;
    }
    if (sdfAdpcm.fillBlk == 2) {
        *right = 3;
        *left = 3;
    } else {
        *right = 2;
        *left = 2;
    }
    if ((reply[0] & 0x80000000) == 0 && sdfAdpcm.bytesLeft != 0) {
        if (sdfAdpcm.bytesLeft >= 0x201) {
            sdfAdpcm.bytesLeft -= 512;
        } else {
            sdfAdpcm.bytesLeft = 0;
        }
    }
    next = sdfAdpcm.fillBlk + 1;
    sdfAdpcm.fillIdx = (sdfAdpcm.fillIdx + 1) & 1;
    sdfAdpcm.fillBlk = next % 3;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfAdpcmFillBuffer);
#endif

s32 sdfAdpcmSetVolume(s32 vol)
{
    s32 entry;
    s32 entryR;

    vol = vol << 7;
    if ((vol & 0x80) != 0) {
        vol |= 0xFF;
    }
    entry = 2 * sdfAdpcm.voiceL + (s8)sdfAdpcm.coreId;
    entryR = 2 * sdfAdpcm.voiceR + (s8)sdfAdpcm.coreId;
    libsd_sceSdSetParam(entry, vol);
    libsd_sceSdSetParam(entry | 0x100, 0);
    libsd_sceSdSetParam(entryR, 0);
    libsd_sceSdSetParam(entryR | 0x100, vol);
}

s32 sdfAdpcmStreamPlay(const char *path, s32 vol, s8 onMemory)
{
    if (sdfAdpcm.playing) {
        stdio_printf(aSdfadpcmAdpcmN);
    } else {
        sdfAdpcm.volume = vol;
        sdfAdpcm.fadeStep = 0;
        sdfAdpcm.onMemory = onMemory;
        thsemap_SignalSema(sdfAdpcmPlaySema);
    }
}

#ifdef NON_MATCHING
/* 8 instructions longer than the retail code. */
/* The stream player thread: wait for a play request, read the APCM header,
 * set the two voices up and keep the double buffer fed until the end. A mono
 * stream plays the same buffer on both voices. */
void sdfAdpcmStreamInit(void *arg)
{
    u32 reply[4];
    u16 vol;
    u16 entry;
    u32 voices;
    u32 sw;
    u32 i;

    sdfAdpcmPlaySemaParam.initial = 0;
    sdfAdpcmPlaySemaParam.max = 1;
    sdfAdpcmPlaySemaParam.option = 0;
    sdfAdpcmPlaySema = thsemap_CreateSema(&sdfAdpcmPlaySemaParam);
    if (sdfAdpcmPlaySema <= 0) {
        stdio_printf(aSdfadpcmCreate);
        thbase_ExitThread();
    }
    sdfAdpcmIntrSemaParam.initial = 0;
    sdfAdpcmIntrSemaParam.max = 1;
    sdfAdpcmIntrSemaParam.option = 0;
    sdfAdpcmIntrSema = thsemap_CreateSema(&sdfAdpcmIntrSemaParam);
    if (sdfAdpcmIntrSema <= 0) {
        stdio_printf(aSdfadpcmCreate);
        thbase_ExitThread();
    }

    while (1) {
        thsemap_WaitSema(sdfAdpcmPlaySema);
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, NULL, 0, 6, NULL);
        } else {
            sdfSndCallEeRpc(0, NULL, 0, 0, reply);
            if (reply[0] == 0) {
                stdio_printf(aSdfadpcmAdpcmF);
                sdfSndPostError(1);
                continue;
            }
        }
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, &sdfAdpcmHeader, 16, 7, reply);
        } else {
            sdfSndCallEeRpc(0, &sdfAdpcmHeader, 16, 1, reply);
        }
        if (sdfAdpcmHeader.type != 11 || sdfAdpcmHeader.magic != 0x4D435041) {
            for (i = 0; i < 0x10; i++) {
                stdio_printf(a02xh, ((u8 *)&sdfAdpcmHeader)[i]);
            }
            stdio_printf(aSdfadpcmOnMemo, reply[0]);
            sdfSndPostError(2);
            goto streamEnd;
        }
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, &sdfAdpcmHeader.sampleLen, 16, 7, reply);
        } else {
            sdfSndCallEeRpc(0, &sdfAdpcmHeader.sampleLen, 16, 1, reply);
        }
        sdfAdpcm.stereo = sdfAdpcmHeader.stereo;
        sdfAdpcm.totalBytes = 2 * sdfAdpcmHeader.sampleLen;
        sdfAdpcm.bytesLeft = 2 * sdfAdpcmHeader.sampleLen;
        vol = sdfAdpcm.volume << 7;
        if ((vol & 0x80) != 0) {
            vol |= 0xFF;
        }
        sdfAdpcm.playing = 1;
        sdfAdpcm.coreId = 0;
        libsd_sceSdSetCoreAttr(4, 0);
        sdfAdpcm.core = 0;
        sdfSndWorkArea->banks[SDF_SND_KINDS].size = 512;
        sdfAdpcm.iopBuf = (u32)sdfSndWorkArea->banks[SDF_SND_KINDS].data;
        sdfAdpcm.spuAddr = 0x1E0000;
        sdfAdpcm.xferIdx = 0;
        sdfAdpcm.xferBlk = 0;
        sdfAdpcm.fillBlk = 0;
        sdfAdpcm.fillIdx = 0;
        sdfAdpcm.voiceL = 0;
        sdfAdpcm.voiceR = 1;
        sdfAdpcm.pitch = sdfAdpcmPitchTbl[sdfAdpcmHeader.freq];
        libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1600,
                             libsd_sceSdGetSwitch((s8)sdfAdpcm.coreId | 0x1500) & 3);
        sdfAdpcm.fadeState = 0;
        sdfAdpcmFillBuffer(1);
        sdfAdpcmUploadBlock();
        sdfAdpcmFillBuffer(1);
        sdfAdpcmUploadBlock();
        sdfAdpcmFillBuffer(1);
        sdfAdpcmUploadBlock();
        sdfAdpcmFillBuffer(1);
        if (sdfAdpcm.stereo) {
            entry = (2 * sdfAdpcm.voiceL) | (s8)sdfAdpcm.coreId;
            libsd_sceSdSetParam(entry, vol);
            libsd_sceSdSetParam(entry | 0x100, 0);
            libsd_sceSdSetParam(entry | 0x200, sdfAdpcm.pitch);
            libsd_sceSdSetParam(entry | 0x300, 0xF);
            libsd_sceSdSetParam(entry | 0x400, 3);
            libsd_sceSdSetAddr(entry | 0x2040, 0x1E0000);
            entry = (2 * sdfAdpcm.voiceR) | (s8)sdfAdpcm.coreId;
            libsd_sceSdSetParam(entry, 0);
            libsd_sceSdSetParam(entry | 0x100, vol);
            libsd_sceSdSetParam(entry | 0x200, sdfAdpcm.pitch);
            libsd_sceSdSetParam(entry | 0x300, 0xF);
            libsd_sceSdSetParam(entry | 0x400, 3);
            libsd_sceSdSetAddr(entry | 0x2040, 0x1E0300);
        } else {
            entry = (2 * sdfAdpcm.voiceL) | (s8)sdfAdpcm.coreId;
            libsd_sceSdSetParam(entry, vol);
            libsd_sceSdSetParam(entry | 0x100, 0);
            libsd_sceSdSetParam(entry | 0x200, sdfAdpcm.pitch);
            libsd_sceSdSetParam(entry | 0x300, 0xF);
            libsd_sceSdSetParam(entry | 0x400, 3);
            libsd_sceSdSetAddr(entry | 0x2040, 0x1E0000);
            entry = (2 * sdfAdpcm.voiceR) | (s8)sdfAdpcm.coreId;
            libsd_sceSdSetParam(entry, 0);
            libsd_sceSdSetParam(entry | 0x100, vol);
            libsd_sceSdSetParam(entry | 0x200, sdfAdpcm.pitch);
            libsd_sceSdSetParam(entry | 0x300, 0xF);
            libsd_sceSdSetParam(entry | 0x400, 3);
            libsd_sceSdSetAddr(entry | 0x2040, 0x1E0000);
        }
        sdfAdpcm.fadeState = 1;
        libsd_sceSdSetSpu2IntrHandler(sdfAdpcmSpu2IntrHandler, NULL);
        libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 1);
        voices = (1 << sdfAdpcm.voiceL) | (1 << sdfAdpcm.voiceR);
        libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1500, voices);
        if (sdfAdpcmHeader.reverb) {
            sw = libsd_sceSdGetSwitch((s8)sdfAdpcm.coreId | 0x1900) | voices;
        } else {
            sw = (voices ^ 0xFFFFFF) & libsd_sceSdGetSwitch((s8)sdfAdpcm.coreId | 0x1900);
        }
        libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1900, sw);
        libsd_sceSdSetSwitch((s8)sdfAdpcm.coreId | 0x1B00, sw);
        do {
            thsemap_WaitSema(sdfAdpcmIntrSema);
            libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 1);
            if (sdfAdpcm.playing == 0) {
                break;
            }
            libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 0);
            sdfAdpcmUploadBlock();
            libsd_sceSdSetCoreAttr((s8)(sdfAdpcm.coreId | 4), 1);
            sdfAdpcmFillBuffer(0);
        } while (sdfAdpcm.playing);

streamEnd:
        if (sdfAdpcm.onMemory) {
            sdfSndCallEeRpc(0, NULL, 0, 8, NULL);
        } else {
            sdfSndCallEeRpc(0, NULL, 0, 2, NULL);
        }
    }
}
#else
INCLUDE_ASM("SDFSDMAN", sdfAdpcmStreamInit);
#endif

ALIGN_TEXT(4);
IRX_IMPORT_TABLE(libsd, 1, 5);
IRX_IMPORT(libsd, sceSdInit, 0x4);
IRX_IMPORT(libsd, sceSdSetParam, 0x5);
IRX_IMPORT(libsd, sceSdSetSwitch, 0x7);
IRX_IMPORT(libsd, sceSdGetSwitch, 0x8);
IRX_IMPORT(libsd, sceSdSetAddr, 0x9);
IRX_IMPORT(libsd, sceSdSetCoreAttr, 0xb);
IRX_IMPORT(libsd, sceSdVoiceTrans, 0x11);
IRX_IMPORT(libsd, sceSdVoiceTransStatus, 0x13);
IRX_IMPORT(libsd, sceSdSetEffectAttr, 0x17);
IRX_IMPORT(libsd, sceSdSetSpu2IntrHandler, 0x1b);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modhsyn, 1, 4);
IRX_IMPORT(modhsyn, sceHSyn_Init, 0x4);
IRX_IMPORT(modhsyn, e5, 0x5);
IRX_IMPORT(modhsyn, sceHSyn_Load, 0x6);
IRX_IMPORT(modhsyn, sceHSyn_VoiceTrans, 0x7);
IRX_IMPORT(modhsyn, e8, 0x8);
IRX_IMPORT(modhsyn, e13, 0xd);
IRX_IMPORT(modhsyn, e15, 0xf);
IRX_IMPORT(modhsyn, e18, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modmidi, 1, 5);
IRX_IMPORT(modmidi, sceMidi_Init, 0x4);
IRX_IMPORT(modmidi, e5, 0x5);
IRX_IMPORT(modmidi, sceMidi_Load, 0x6);
IRX_IMPORT(modmidi, sceMidi_SelectMidi, 0xe);
IRX_IMPORT(modmidi, sceMidi_MidiPlaySwitch, 0xf);
IRX_IMPORT(modmidi, sceMidi_MidiSetLocation, 0x10);
IRX_IMPORT(modmidi, sceMidi_MidiSetVolume, 0x11);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(modmsin, 1, 1);
IRX_IMPORT(modmsin, sceMSIn_PutMsg, 0x7);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sdfiopfl, 1, 1);
IRX_IMPORT(sdfiopfl, sdfIopFileReadOpen, 0x4);
IRX_IMPORT(sdfiopfl, sdfIopFileGetFileSize, 0x5);
IRX_IMPORT(sdfiopfl, sdfIopFileRead, 0x6);
IRX_IMPORT(sdfiopfl, sdfIopFileClose, 0x7);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysmem, 1, 1);
IRX_IMPORT(sysmem, FreeSysMemory, 0x5);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(intrman, 1, 2);
IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(loadcore, 1, 3);
IRX_IMPORT(loadcore, FlushDcache, 0x5);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifcmd, 1, 1);
IRX_IMPORT(sifcmd, SifInitCmd, 0x4);
IRX_IMPORT(sifcmd, SifSetCmdBuffer, 0x8);
IRX_IMPORT(sifcmd, SifAddCmdHandler, 0xa);
IRX_IMPORT(sifcmd, SifSendCmd, 0xc);
IRX_IMPORT(sifcmd, SifInitRpc, 0xe);
IRX_IMPORT(sifcmd, SifBindRpc, 0xf);
IRX_IMPORT(sifcmd, SifCallRpc, 0x10);
IRX_IMPORT(sifcmd, SifCheckStatRpc, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sifman, 1, 1);
IRX_IMPORT(sifman, SifSIF2Init, 0x5);
IRX_IMPORT(sifman, sceSifSetDma, 0x7);
IRX_IMPORT(sifman, sceSifDmaStat, 0x8);
IRX_IMPORT(sifman, SifSetIOPrcvaddr, 0x1d);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(stdio, 1, 3);
IRX_IMPORT(stdio, printf, 0x4);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(sysclib, 1, 3);
IRX_IMPORT(sysclib, memcpy, 0xc);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thbase, 1, 2);
IRX_IMPORT(thbase, CreateThread, 0x4);
IRX_IMPORT(thbase, StartThread, 0x6);
IRX_IMPORT(thbase, ExitThread, 0x8);
IRX_IMPORT(thbase, SleepThread, 0x18);
IRX_IMPORT(thbase, iWakeupThread, 0x1a);
IRX_IMPORT(thbase, iCancelWakeupThread, 0x1c);
IRX_IMPORT(thbase, DelayThread, 0x21);
IRX_IMPORT(thbase, USec2SysClock, 0x27);
IRX_IMPORT(thbase, e43, 0x2b);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(thsemap, 1, 1);
IRX_IMPORT(thsemap, CreateSema, 0x4);
IRX_IMPORT(thsemap, SignalSema, 0x6);
IRX_IMPORT(thsemap, iSignalSema, 0x7);
IRX_IMPORT(thsemap, WaitSema, 0x8);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(timrman, 1, 3);
IRX_IMPORT(timrman, AllocHardTimer, 0x4);
IRX_IMPORT(timrman, FreeHardTimer, 0x6);
IRX_IMPORT(timrman, SetTimerHandler, 0x14);
IRX_IMPORT(timrman, SetupHardTimer, 0x16);
IRX_IMPORT(timrman, StartHardTimer, 0x17);
IRX_IMPORT(timrman, StopHardTimer, 0x18);
IRX_IMPORT_TABLE_END;
ALIGN_TEXT(4);
