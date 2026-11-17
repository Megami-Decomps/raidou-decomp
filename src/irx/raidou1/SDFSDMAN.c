#include "sdfsnd.h"

/* SDFSDMAN.IRX, the sound driver: it owns the SPU2, plays the .SMG banks and
 * the streamed ADPCM, and takes its orders from the EE over SIF.
 * See docs/sdf-irx-modules.md.
 *
 * The strings stay in asm/<version>/irx/SDFSDMAN/rodata.s so .rodata keeps its layout;
 * /tmp aside, their text is listed in docs/sdf-irx-modules.md. */
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

s32 sdfSdManInit(void)
{
    iop_thread_t rpc;
    iop_thread_t smg;
    iop_thread_t adpcm;
    s32 tid;
    s32 ret;

    if (sifman_SifSetIOPrcvaddr() == 0) {
        sifman_SifSIF2Init();
    }
    sifcmd_SifInitRpc(0);

    rpc.thread = sdfSndDrvRpcInit;
    rpc.attr = 0;
    rpc.priority = 40;
    rpc.stacksize = 2048;
    rpc.option = 0;
    tid = thbase_CreateThread(&rpc);
    if (tid <= 0) {
        goto fail;
    }
    thbase_StartThread(tid, NULL);

    smg.thread = sdfSndSmgLoadThread;
    smg.attr = 0;
    smg.priority = 41;
    smg.stacksize = 2048;
    smg.option = 0;
    tid = thbase_CreateThread(&smg);
    if (tid <= 0) {
        goto fail;
    }
    thbase_StartThread(tid, NULL);

    adpcm.thread = sdfAdpcmStreamInit;
    adpcm.attr = 0;
    adpcm.priority = 42;
    adpcm.stacksize = 2048;
    adpcm.option = 0;
    tid = thbase_CreateThread(&adpcm);
    if (tid <= 0) {
fail:
        stdio_printf(aSdfsdmanIFThre);
        ret = 1;
    } else {
        thbase_StartThread(tid, NULL);
        ret = 0;
    }
    return ret;
}

/* Newton's method; the caller wants the distance between two points. */
s32 sdfSndIsqrt(s32 v)
{
    s32 g, prev;

    if (v <= 0) {
        return -1;
    }
    g = 1;
    do {
        prev = g;
        g = (v / g + g) / 2;
    } while (g != prev && g != prev + 1 && g != prev - 1);
    return g;
}

#ifdef NON_MATCHING
/* The angle of (x, y) in degrees, from a 32 entry table of atan(n/31).
 * Two instructions short: gcc copies x into the negation where the retail
 * code negates its own copy in place. */
s32 sdfSndAtan2Deg(s32 x, s32 y)
{
    s32 ax;
    s32 ay;
    s32 deg;
    s32 idx;

    ax = x;
    if (x < 0) {
        ax = -ax;
    }
    ay = y;
    if (y < 0) {
        ay = -ay;
    }
    if (ax == 0) {
        deg = 0;
    } else if (ay == 0) {
        deg = 90;
    } else if (ax >= ay) {
        idx = 31 * ay / ax;
        deg = 90 - sdfSndAtanTbl[idx];
    } else {
        idx = 31 * ax / ay;
        deg = sdfSndAtanTbl[idx];
    }
    if (y < 0) {
        deg = 180 - deg;
    }
    if (x < 0) {
        deg = -deg;
    }
    return deg;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndAtan2Deg);
#endif

s32 sdfSndPostError(s32 code)
{
    sdfSndWorkArea->errCode = code;
    sdfSndWorkArea->errCount++;
    return p2sdIrxSpuIopMap();
}

/* Copy the synthesiser voice map into the work area the EE reads: 24 voices
 * per core, -1 where the voice is not in use. */
#ifdef NON_MATCHING
/* One instruction short: gcc fills the branch delay slot with the second
 * arm instead of the address of the slot being written. */
s32 sdfSndUpdateVoiceMap(void)
{
    s32 core;
    s32 base;
    s32 i;
    u8 m;
    u8 v;

    core = 0;
    base = 0;
    do {
        i = 0;
        do {
            m = sdfSndHsynVoiceMap[i + base];
            if ((m & 0x80) != 0) {
                v = m & 0x70;
            } else {
                v = -1;
            }
            sdfSndWorkArea->voiceMap[i + 32 * core] = v;
            i++;
        } while (i < 24);
        core++;
        base += 24;
    } while (core < 2);
    return p2sdIrxSpuIopMap();
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndUpdateVoiceMap);
#endif

/* mode 1 reads through the EE instead of the file library. */
s32 sdfSndFileOpen(const char *path, s32 mode)
{
    s32 fd;

    if (mode == 1) {
        sdfSndCallEeRpc(0, NULL, 0, 3, NULL);
        return 0;
    }
    fd = sdfiopfl_sdfIopFileReadOpen(path);
    if (fd == 0) {
        stdio_printf(aSdfsnddrvSOpen, path);
        sdfSndPostError(1);
        return -1;
    }
    return fd;
}

s32 sdfSndFileRead(s32 fd, void *buf, s32 len, s32 mode)
{
    s32 read;

    if (mode == 1) {
        sdfSndCallEeRpc(0, buf, len, 4, NULL);
        return len;
    }
    read = sdfiopfl_sdfIopFileRead(fd, buf, len);
    if (read != len) {
        stdio_printf(aSdfsndfileread, read);
    }
    return read;
}

s32 sdfSndFileGetSize(s32 fd)
{
    return sdfiopfl_sdfIopFileGetFileSize(fd);
}

s32 sdfSndFileClose(s32 fd)
{
    return sdfiopfl_sdfIopFileClose(fd);
}

INCLUDE_RODATA("SDFSDMAN", aSdfSoundDriver);
INCLUDE_RODATA("SDFSDMAN", aSdfsdmanIFThre);
INCLUDE_RODATA("SDFSDMAN", aB1);
INCLUDE_RODATA("SDFSDMAN", aC);
INCLUDE_RODATA("SDFSDMAN", aD);
INCLUDE_RODATA("SDFSDMAN", aAd);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvSOpen);
INCLUDE_RODATA("SDFSDMAN", aSdfsndfileread);
INCLUDE_RODATA("SDFSDMAN", jtbl_000062A8);
INCLUDE_ASM("SDFSDMAN", sdfSndGetVolTable);

#ifdef NON_MATCHING
/* Scale the slot volume by the per sound entry in its volume table, and by
 * the global sequence volume for the three sequence slots.
 * Matches but for the two registers id and vol land in. */
s32 sdfSndApplyVolume(s32 id, s32 slot, s32 vol)
{
    s32 kind;
    u8 *tbl;
    s32 v;
    s32 state;

    kind = sdfSndFindSlotById(id);
    if (kind < 0) {
        return kind;
    }
    tbl = sdfSndGetVolTable(kind, NULL);
    if (tbl != NULL) {
        v = tbl[id & 0x7F];
    } else {
        v = 127;
    }
    v = (vol * v) >> 7;
    if (slot < 3) {
        v = (v * sdfSndSeqVolume) >> 7;
    }
    if (v >= 128) {
        v = 127;
    }
    if (v < 0) {
        v = 0;
    }
    intrman_CpuSuspendIntr(&state);
    modmidi_sceMidi_MidiSetVolume(&sdfSndMidiWork, slot, 255, (u8)v);
    return intrman_CpuResumeIntr(state);
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndApplyVolume);
#endif

/* Reset all 16 channels of a slot: bank select, volume and all notes off. */
s32 sdfSndPutMsg(s32 slot)
{
    s32 i;
    s32 ret;

    for (i = 0; i != 16; i++) {
        if (modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x400AB0)) {
            stdio_printf(aSdfsnddrvScems);
        }
        if (modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x7F0BB0)) {
            stdio_printf(aSdfsnddrvScems);
        }
        ret = modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, i | 0x4000E0);
        if (ret) {
            ret = stdio_printf(aSdfsnddrvScems);
        }
    }
    return ret;
}

s32 sdfSndIsSlotMuted(s32 id)
{
    s32 kind;
    u8 *tbl;

    kind = sdfSndFindSlotById(id);
    if (kind >= 0) {
        tbl = sdfSndGetVolTable(kind, NULL);
        if (tbl != NULL) {
            if (tbl[id & 0x7F] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 sdfSndMidiCbIgnore1(void)
{
    return 1;
}

s32 sdfSndMidiCbIgnore2(void)
{
    return 1;
}

INCLUDE_ASM("SDFSDMAN", sdfSndMidiCbMessage);

s32 sdfSndTimerHandler(sdfSndTimer_t *tm)
{
    s32 thid;

    thid = tm->thid;
    thbase_iCancelWakeupThread(tm->thid);
    thbase_iWakeupThread(thid);
    return tm->interval;
}

/* One hard timer ticking at 240Hz wakes the sequence thread. */
s32 sdfSndTimerSetup(sdfSndTimer_t *tm)
{
    u32 sysclock[2];
    u32 clk;
    s32 id;

    thbase_USec2SysClock(0x1047, sysclock);
    clk = sysclock[0];
    tm->interval = clk;
    id = timrman_AllocHardTimer(1, 32, 1);
    tm->timerId = id;
    if (id <= 0) {
        stdio_printf(aHardTimerAlloc);
        return 1;
    }
    if (timrman_SetTimerHandler(id, clk, (s32 (*)(void *))sdfSndTimerHandler, tm) != 0) {
        stdio_printf(aTimerHandlerRe);
        return 1;
    }
    if (timrman_SetupHardTimer(id, 1, 0, 1) != 0) {
        stdio_printf(aHardTimerSetup);
        return 1;
    }
    return 0;
}

s32 sdfSndTimerRelease(sdfSndTimer_t *tm)
{
    s32 id;

    id = tm->timerId;
    if (id > 0) {
        if (timrman_FreeHardTimer(id) != 0) {
            stdio_printf(aHardTimerRelea);
            return 1;
        }
        tm->timerId = 0;
    }
    return 0;
}

s32 sdfSndTimerStart(sdfSndTimer_t *tm)
{
    if (timrman_StartHardTimer(tm->timerId) != 0) {
        stdio_printf(aHardTimerStart, tm->timerId);
        return 1;
    }
    return 0;
}

s32 sdfSndTimerStop(sdfSndTimer_t *tm)
{
    s32 ret;

    ret = timrman_StopHardTimer(tm->timerId);
    if (ret != 0 && ret != -156) {
        stdio_printf(aHardTimerStopF);
        return 1;
    }
    return 0;
}

#ifdef NON_MATCHING
/* Point the synthesiser and MIDI tables at the 32 sequence and slot work
 * areas, then take the per bank voice counts from the work area. */
s32 sdfSndInitWorkAreas(void)
{
    s32 i;
    u32 bank;
    sceMSInBuf_t *hseq;
    sceHSynSlot_t *hslot;
    sceMidiSlot_t *mslot;
    sceMSInBuf_t *voice;

    sdfSndHsynWork.unk00 = 1;
    sdfSndHsynWork.unk10 = 0;
    sdfSndHsynWork.unk0C = 0;
    sdfSndHsynWork.tbl = &sdfSndHsynTbl;
    sdfSndHsynTbl.nEntry = SDF_SND_SEQS * 2;
    sdfSndHsynTbl.entry = sdfSndHsynEntry;
    hseq = sdfSndMsInBuf;
    hslot = sdfSndHsynSlotWork;
    i = 0;
    do {
        sdfSndHsynEntry[i].unk00 = 0;
        sdfSndHsynEntry[i].buf = hseq;
        sdfSndHsynEntry[i].unk08 = 0;
        sdfSndHsynEntry[i].slot = hslot;
        hseq->size = 1032;
        hseq->len = 0;
        hseq++;
        hslot++;
        i++;
    } while (i < SDF_SND_SEQS);

    bank = 0;
    if (sdfSndWorkArea->bankCount != 0) {
        i = 0;
        do {
            sdfSndHsynSlotWork[i].unk00 = sdfSndWorkArea->banks[bank].unk00;
            sdfSndHsynSlotWork[i].unk01 = sdfSndWorkArea->banks[bank].unk01;
            bank++;
            i++;
        } while (bank < sdfSndWorkArea->bankCount);
    }

    sdfSndMidiWork.unk00 = 2;
    sdfSndMidiWork.unk10 = 0;
    sdfSndMidiWork.unk0C = 0;
    sdfSndMidiWork.tbl = &sdfSndMidiTbl;
    sdfSndMidiTbl.nSeq = SDF_SND_SEQS * 2;
    sdfSndMidiTbl.seq = sdfSndMidiSeq;
    mslot = sdfSndMidiSlotWork;
    i = 0;
    do {
        sdfSndMidiSeq[i].slot = mslot;
        sdfSndMidiSeq[i].unk00 = 0;
        sdfSndMidiSeq[i].data = NULL;
        sdfSndMidiSeq[i].unk08 = 0;
        mslot++;
        i++;
    } while (i < SDF_SND_SEQS);

    sdfSndMidiTbl.nVoice = SDF_SND_SEQS;
    sdfSndMidiTbl.voice = sdfSndMidiVoice;
    voice = sdfSndMsInBuf;
    i = 0;
    do {
        sdfSndMidiVoice[i].buf = voice;
        sdfSndMidiVoice[i].unk00 = 0;
        voice++;
        i++;
    } while (i < SDF_SND_SEQS);
    return sdfSndSetBankAddr(-1, 0);
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndInitWorkAreas);
#endif

INCLUDE_ASM("SDFSDMAN", sdfSndSeqThread);

INCLUDE_ASM("SDFSDMAN", sdfSndInitSpu2);

INCLUDE_ASM("SDFSDMAN", sdfSndMidiSelect);

INCLUDE_ASM("SDFSDMAN", sdfSndStopAll);

INCLUDE_ASM("SDFSDMAN", sdfSndInitSynth);

s32 sdfSndQuitSynth(void)
{
    s32 ret;

    ret = (s32)sdfSndWorkArea;
    if (sdfSndWorkArea != NULL) {
        sdfSndTimerStop(&sdfSndTimer);
        sdfSndTimerRelease(&sdfSndTimer);
        sdfSndStopAll(0);
        sysmem_FreeSysMemory(sdfSndWorkArea->xferBuf);
        sysmem_FreeSysMemory(sdfSndWorkArea->banks[0].data);
        sysmem_FreeSysMemory(sdfSndWorkArea->progData[0].ptr);
        ret = sysmem_FreeSysMemory(sdfSndWorkArea);
        sdfSndWorkArea = NULL;
    }
    return ret;
}

/* The bank slot a packed sound id belongs to, or -1. */
s32 sdfSndFindSlotById(s32 idPacked)
{
    s32 i;

    if (idPacked < 0) {
        return -1;
    }
    idPacked >>= 16;
    for (i = 0; i < SDF_SND_KINDS; i++) {
        if (sdfSndWorkArea->stat[i].id == idPacked) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("SDFSDMAN", func_00001A24);

INCLUDE_ASM("SDFSDMAN", func_00001A74);

INCLUDE_ASM("SDFSDMAN", func_00001ACC);

INCLUDE_ASM("SDFSDMAN", sdfSndBgmPlay);

INCLUDE_ASM("SDFSDMAN", sdfSndBgmStop);

/* Hand the synthesiser the wave data for a bank. */
s32 sdfSndHsynLoad(s32 bank, s32 slot)
{
    sceHSynSlot_t *hs;
    s32 state;

    hs = &sdfSndHsynSlotWork[slot];
    hs->unk00 = 0;
    hs->unk04 = 0;
    hs->unk08 = 0;
    hs->unk0C = 0;
    hs->unk10 = 0;
    hs->unk02 = 0;
    hs->unk03 = 0;
    intrman_CpuSuspendIntr(&state);
    if (modhsyn_sceHSyn_Load(&sdfSndHsynWork, slot, sdfSndWorkArea->banks[bank].spuAddr,
                             sdfSndWorkArea->banks[bank].data, 0) != 0 ||
        modhsyn_sceHSyn_Load(&sdfSndHsynWork, slot, sdfSndWorkArea->banks[bank].spuAddr,
                             sdfSndWorkArea->banks[bank].data, 1) != 0) {
        intrman_CpuResumeIntr(state);
        return stdio_printf(aSdfsnddrvScehs_0);
    }
    modhsyn_e15(&sdfSndHsynLoadParam);
    return intrman_CpuResumeIntr(state);
}

INCLUDE_ASM("SDFSDMAN", sdfSndSePlay);

#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndDistanceAtten */
/* Volume percentage for a sound `dist` away with the given audible radius. */
s32 sdfSndDistanceAtten(s32 dist, s32 radius)
{
    s32 outer;
    s32 atten;

    outer = 4 * radius;
    if (radius < 3) {
        radius = 3;
        outer = 12;
    }
    atten = 10 * (2 * (outer + radius) - (dist - 10)) / radius;
    if (atten < 0) {
        return 0;
    }
    if (atten >= 101) {
        return 100;
    }
    return atten;
}
/* ENDF sdfSndDistanceAtten */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndDistanceAtten);
#endif

#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* Re-pan and re-attenuate a positional sound after the listener moved. */
s32 sdfSndUpdate3dVoice(s32 slot)
{
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 sign;
    s32 level;
    s32 pan;
    s32 vol;
    s32 state;

    if (sdfSndSlots[slot].is3d == 0) {
        return sdfSndSlots[slot].is3d;
    }
    dx = sdfSndSlots[slot].posX - sdfSndListenerX;
    dz = sdfSndSlots[slot].posZ - sdfSndListenerZ;
    dist = sdfSndIsqrt(dx * dx +
                       (sdfSndSlots[slot].posY - sdfSndListenerY) *
                       (sdfSndSlots[slot].posY - sdfSndListenerY) + dz * dz);
    angle = (sdfSndAtan2Deg(-dx, dz) - sdfSndListenerAngle) % 360;
    if (angle >= 181) {
        angle -= 360;
    }
    if (angle < -180) {
        angle += 360;
    }
    sign = 1;
    if (angle < 0) {
        sign = -1;
        angle = -angle;
    }
    level = 100;
    if (angle >= 45) {
        if (angle >= 91) {
            pan = 63 - (180 - angle) * 70 * sign / 100;
            level = 75 - ((angle - 90) >> 2);
        } else {
            level = 100 - (angle - 45) / 2;
            pan = 63 - angle * 70 * sign / 100;
        }
    } else {
        pan = 63 - angle * 70 * sign / 100;
    }
    if (dist < 11) {
        pan = ((pan - 63) >> 1) + 63;
    } else {
        level = level * sdfSndDistanceAtten(dist, sdfSndSlots[slot].radius) / 100;
    }
    vol = 127 * level / 100;
    if (vol >= 127) {
        vol = 127;
    }
    sdfSndSlots[slot].vol = vol << 8;
    sdfSndSlots[slot].volStep = 0;
    sdfSndApplyVolume(sdfSndSlots[slot].id, slot, vol);
    intrman_CpuSuspendIntr(&state);
    modmsin_sceMSIn_PutMsg(&sdfSndMidiWork, slot, ((pan & 0x7F) << 16) | 0xAB0);
    return intrman_CpuResumeIntr(state);
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndUpdate3dVoice);
#endif

#ifdef NON_MATCHING
/* 4 instructions short of the retail code. */
/* FUNC sdfSndSePlayPositional */
/* Turn an angle and a distance into a pan and a volume, then play. */
s32 sdfSndSePlayPositional(s32 id, s32 angle, s32 dist, s32 radius)
{
    sdfSndSeReq_t req;
    s32 a;
    s32 sign;
    s32 level;

    a = angle % 360;
    req.id = id;
    if (a >= 181) {
        a -= 360;
    }
    if (a < -180) {
        a += 360;
    }
    sign = 1;
    if (a < 0) {
        sign = -1;
        a = -a;
    }
    level = 100;
    if (a >= 45) {
        if (a >= 91) {
            req.pan = 63 - (180 - a) * 70 * sign / 100;
            level = 75 - ((a - 90) >> 2);
        } else {
            level = 100 - (a - 45) / 2;
            req.pan = 63 - a * 70 * sign / 100;
        }
    } else {
        req.pan = 63 - a * 70 * sign / 100;
    }
    if (dist < 11) {
        req.pan = ((req.pan - 63) >> 1) + 63;
    } else {
        level = level * sdfSndDistanceAtten(dist, radius) / 100;
    }
    req.vol = 127 * level / 100;
    return sdfSndSePlay(&req);
}
/* ENDF sdfSndSePlayPositional */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSePlayPositional);
#endif

#ifdef NON_MATCHING
/* 1 instruction longer than the retail code. */
/* FUNC sdfSndSetListener */
/* Remember where the listener is and which way it faces; the sequence thread
 * re-pans the positional sounds on the next tick. */
s32 sdfSndSetListener(sdfSndListenerReq_t *req)
{
    s32 angle;
    s32 posZ;
    s32 atZ;
    s32 posY;
    s32 dx;

    if (sdfSndListenerX == req->posX && sdfSndListenerY == req->posY &&
        sdfSndListenerZ == req->posZ) {
        angle = sdfSndAtan2Deg(req->atX - sdfSndListenerX, sdfSndListenerZ - req->atZ);
        if (sdfSndListenerAngle == angle) {
            return angle;
        }
    }
    posZ = req->posZ;
    atZ = req->atZ;
    posY = req->posY;
    dx = req->atX - req->posX;
    sdfSndListenerX = req->posX;
    sdfSndListenerY = posY;
    sdfSndListenerZ = posZ;
    sdfSndListenerAngle = sdfSndAtan2Deg(dx, posZ - atZ);
    sdfSndListenerDirty = 1;
    return 1;
}
/* ENDF sdfSndSetListener */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetListener);
#endif

#ifdef NON_MATCHING
/* 1 instruction longer than the retail code. */
/* Play a sound effect at a point in the world. */
s32 sdfSndSePlay3d(sdfSndSe3dReq_t *req)
{
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 slot;

    dx = req->x - sdfSndListenerX;
    dz = req->z - sdfSndListenerZ;
    dist = sdfSndIsqrt(dx * dx + (req->y - sdfSndListenerY) * (req->y - sdfSndListenerY) +
                       dz * dz);
    angle = sdfSndAtan2Deg(-dx, dz);
    slot = sdfSndSePlayPositional(req->id, angle - sdfSndListenerAngle, dist, req->radius);
    if (slot >= 0) {
        sdfSndSlots[slot].is3d = 1;
        sdfSndSlots[slot].posX = req->x;
        sdfSndSlots[slot].posY = req->y;
        sdfSndSlots[slot].posZ = req->z;
        sdfSndSlots[slot].radius = req->radius;
    }
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSePlay3d);
#endif

s32 sdfSndIsBankLoaded(s32 kind)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sdfSndWorkArea->stat[kind + i].id >= 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScems);
INCLUDE_RODATA("SDFSDMAN", aHardTimerAlloc);
INCLUDE_RODATA("SDFSDMAN", aTimerHandlerRe);
INCLUDE_RODATA("SDFSDMAN", aHardTimerSetup);
INCLUDE_RODATA("SDFSDMAN", aHardTimerRelea);
INCLUDE_RODATA("SDFSDMAN", aHardTimerStart);
INCLUDE_RODATA("SDFSDMAN", aHardTimerStopF);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi_0);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi_1);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScehs);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi_2);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi_3);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvPlayS);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScemi_4);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScehs_0);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvSeSta);
INCLUDE_RODATA("SDFSDMAN", jtbl_00006570);
INCLUDE_ASM("SDFSDMAN", func_00002AD0);

INCLUDE_ASM("SDFSDMAN", func_00002C24);

INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvFileR);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvMidi0);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvSData);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvDataS);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvSRead);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScehs_1);
INCLUDE_RODATA("SDFSDMAN", jtbl_000066C0);
INCLUDE_ASM("SDFSDMAN", sdfSndLoadSmg);

/* Waits on its semaphore and loads whatever path the EE last set. */
void sdfSndSmgLoadThread(void *arg)
{
    s32 ret;

    sdfSndSmgSemaParam.initial = 0;
    sdfSndSmgSemaParam.max = 1;
    sdfSndSmgSemaParam.option = 0;
    sdfSndSmgSema = thsemap_CreateSema(&sdfSndSmgSemaParam);
    if (sdfSndSmgSema <= 0) {
        stdio_printf(aSdfsnddrvCreat);
        thbase_ExitThread();
    }
    while (1) {
        ret = thsemap_WaitSema(sdfSndSmgSema);
        if (ret != 0) {
            stdio_printf(aSdfsnddrvWaits, ret);
        }
        sdfSndLoadSmg(sdfSndSmgPath, 0);
    }
}

#ifdef NON_MATCHING
/* One nop longer: gas does not fill the load delay before the store to
 * sdfSndBankBase with the store's lui, which the retail code has. */
/* kind < 0 lays the whole bank table out again from the size and offset
 * tables; otherwise it just records one bank's wave size. */
s32 sdfSndSetBankAddr(s32 kind, u32 waveSize)
{
    s32 i;
    u8 *base;

    if (kind < 0) {
        for (i = 0; i < SDF_SND_KINDS; i++) {
            sdfSndWorkArea->banks[i].spuAddr = sdfSndBankSizeTbl[i];
            sdfSndWorkArea->banks[i].waveSize = sdfSndBankAddrTbl[i];
        }
        if (sdfSndBankBase == NULL) {
            sdfSndBankBase = sdfSndWorkArea->banks[0].data;
        }
        base = sdfSndBankBase;
        for (i = 0; i < SDF_SND_KINDS + 1; i++) {
            sdfSndWorkArea->banks[i].data = &base[sdfSndBankOffTbl[i]];
            sdfSndWorkArea->banks[i].size = sdfSndBankFlagTbl[i];
        }
        return -1;
    }
    sdfSndWorkArea->banks[kind].waveSize = waveSize;
    return kind;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetBankAddr);
#endif

INCLUDE_ASM("SDFSDMAN", sdfSndStopByGroup);

s32 sdfSndUnloadBank(s32 kind)
{
    if (kind >= 0) {
        sdfSndStopByGroup(kind);
        sdfSndWorkArea->banks[kind].waveSize = 0;
        sdfSndWorkArea->banks[kind].size = 0;
        sdfSndWorkArea->stat[kind].id = -1;
        sdfSndWorkArea->stat[kind].unk06 = 0;
        if (kind == 1) {
            sdfSndBank1Loaded = 0;
        }
    }
}

#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* Push the work area up to the EE by DMA and wait for it to land. */
s32 p2sdIrxSpuIopMap(void)
{
    SifDmaTransfer_t sdd;
    s32 state;
    s32 tries;
    u32 id;
    u32 start;
    u32 now;
    s32 stat;
    s32 ret;

    sdd.src = sdfSndWorkArea;
    sdd.dest = sdfSndWorkArea->eeAddr;
    tries = 5;
    sdd.attr = 0;
    sdd.size = sdfSndWorkArea->size;
    loadcore_FlushDcache();
    while (1) {
        intrman_CpuSuspendIntr(&state);
        id = sifman_sceSifSetDma(&sdd, 1);
        intrman_CpuResumeIntr(state);
        if (id != 0) {
            break;
        }
        thbase_DelayThread(1000);
        if (--tries == 0) {
            tries = 10000;
            stdio_printf(aP2sdirxspuiopm);
        }
    }
    start = thbase_e43();
    while (1) {
        intrman_CpuSuspendIntr(&state);
        stat = sifman_sceSifDmaStat(id);
        ret = intrman_CpuResumeIntr(state);
        if (stat < 0) {
            break;
        }
        thbase_DelayThread(1000);
        now = thbase_e43();
        if (now - start > 0x1F78A40) {
            stdio_printf(aP2sdirxspuiopm_0, stat);
            start = now;
        }
    }
    return ret;
}
#else
INCLUDE_ASM("SDFSDMAN", p2sdIrxSpuIopMap);
#endif

s32 sdfSndFadeOutById(s32 *req)
{
    s32 i;

    for (i = 0; i < 16; i++) {
        if (sdfSndSlots[i].id == *req &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            sdfSndSlots[i].volStep = -32;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}

/* ENDF sdfSndFadeOutById */
/* FUNC sdfSndRequestBgmSwitch */
/* The next controller 0 on the switch channel starts this sequence. */
s32 sdfSndRequestBgmSwitch(s32 midi)
{
    if (midi >= 0) {
        sdfSndBgmSwitchMidi = midi;
    }
    sdfSndBgmSwitchReq = 1;
    return 1;
}

/* An idle one of the three sequence slots, else the one playing the oldest
 * sound. The slot used last time is skipped so a restart is audible. */
s32 sdfSndAllocBgmSlot(void)
{
    s32 i;
    s32 slot;
    u32 serial;
    s32 state;

    slot = -1;
    serial = -1;
    for (i = 0; i < 3; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) == 0 && i != sdfSndLastBgmSlot &&
            serial >= sdfSndSlots[i].serial) {
            serial = sdfSndSlots[i].serial;
            slot = i;
        }
    }
    if (slot >= 0) {
        sdfSndLastBgmSlot = slot;
        return slot;
    }
    serial = -1;
    slot = -1;
    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].serial < serial && i != sdfSndLastBgmSlot &&
            sdfSndSlots[i].is3d == 0) {
            serial = sdfSndSlots[i].serial;
            slot = i;
        }
    }
    if (slot < 0) {
        return -1;
    }
    intrman_CpuSuspendIntr(&state);
    modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 0);
    intrman_CpuResumeIntr(state);
    sdfSndLastBgmSlot = slot;
    return slot;
}

#ifdef NON_MATCHING
/* 2 instructions short of the retail code. */
s32 sdfSndAllocSeSlot(void)
{
    s32 i;
    s32 slot;
    u32 serial;
    s32 state;

    for (i = 3; i < 16; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) == 0 && i != sdfSndLastSeSlot) {
            sdfSndLastSeSlot = i;
            return i;
        }
    }
    serial = -1;
    slot = -1;
    for (i = 3; i < 16; i++) {
        if (sdfSndSlots[i].serial < serial && i != sdfSndLastSeSlot &&
            sdfSndSlots[i].is3d == 0) {
            serial = sdfSndSlots[i].serial;
            slot = i;
        }
    }
    if (slot < 0) {
        return -1;
    }
    intrman_CpuSuspendIntr(&state);
    modmidi_sceMidi_MidiPlaySwitch(&sdfSndMidiWork, slot, 0);
    intrman_CpuResumeIntr(state);
    sdfSndLastSeSlot = slot;
    return slot;
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndAllocSeSlot);
#endif

#ifdef NON_MATCHING
/* 1 instruction short of the retail code. */
/* The EE sends 0..15; the driver keeps it in eighths with the odd steps
 * rounded up to the next full one. */
s32 sdfSndSetSeqVolume(s32 vol)
{
    s32 v;
    s32 i;

    v = 8 * vol;
    if ((vol & 1) != 0) {
        v |= 7;
    }
    sdfSndSeqVolume = v;
    for (i = 0; i < 3; i++) {
        if ((sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0) {
            sdfSndApplyVolume(sdfSndSlots[i].id, i, sdfSndSeqVolume);
        }
    }
}
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSetSeqVolume);
#endif

s32 sdfSndFadeOutAll(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].id >= 0 &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0 &&
            sdfSndSlots[i].volStep >= 0) {
            sdfSndSlots[i].volStep = -64;
            sdfSndSlots[i].fadeIn = 1;
            sdfSndSlots[i].unk34[0] = 1;
        }
    }
}

s32 sdfSndFadeInAll(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (sdfSndSlots[i].id >= 0 &&
            (sdfSndMidiWork.tbl->seq[i].slot->flags & 2) != 0 &&
            sdfSndSlots[i].unk34[0] == 1) {
            sdfSndSlots[i].volStep = 64;
            sdfSndSlots[i].fadeIn = 0;
            sdfSndSlots[i].unk34[0] = 0;
        }
    }
}

INCLUDE_ASM("SDFSDMAN", func_00003EB8);

/* ENDF sdfSndFadeInAll */
#ifdef NON_MATCHING
/* 2 instructions longer than the retail code. */
/* FUNC sdfSndSifCmdHandler */
/* Copy an EE command packet into the ring and wake the RPC thread. A full
 * ring drops the oldest entry and counts the overflow. */
s32 sdfSndSifCmdHandler(sdfSndCmd_t *pkt, void *harg)
{
    s32 ret;

    sysclib_memcpy(&sdfSndCmdRing[(u8)(sdfSndCmdHead + sdfSndCmdCount)], pkt,
                   16 * ((pkt->opt >> 28) + 1));
    ret = thsemap_iSignalSema(sdfSndCmdSema);
    if (ret == 0) {
        return ++sdfSndCmdCount;
    }
    if (ret == -420) {
        sdfSndCmdHead = (u8)(sdfSndCmdHead + 1);
        sdfSndCmdOverflow++;
        return sdfSndCmdHead;
    }
    return -420;
}
/* ENDF sdfSndSifCmdHandler */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndSifCmdHandler);
#endif

#ifdef NON_MATCHING
/* Matches instruction for instruction, but gcc chooses different
 * registers. */
/* FUNC sdfSndCreateRpcSema */
s32 sdfSndCreateRpcSema(void)
{
    s32 sema;

    sdfSndRpcSemaParam.initial = 1;
    sdfSndRpcSemaParam.max = 2;
    sdfSndRpcSemaParam.option = 0;
    sema = thsemap_CreateSema(&sdfSndRpcSemaParam);
    sdfSndRpcSema = sema;
    if (sema <= 0) {
        stdio_printf(aSdfsnddrvCreat);
        return thbase_ExitThread();
    }
    return sema;
}
/* ENDF sdfSndCreateRpcSema */
#else
INCLUDE_ASM("SDFSDMAN", sdfSndCreateRpcSema);
#endif

INCLUDE_ASM("SDFSDMAN", sdfSndLoadVolTables);

INCLUDE_ASM("SDFSDMAN", sdfSndStoreVolTables);

/* The IOP idles its clock without a running thread, which upsets the timing
 * of everything else; this one never sleeps. */
void sdfSndKeepAliveThread(void *arg)
{
    while (1) {
        thbase_DelayThread(10000);
    }
}

s32 sdfSndStartKeepAliveThread(void)
{
    iop_thread_t thp;
    s32 tid;

    thp.attr = 0x2000000;
    thp.thread = sdfSndKeepAliveThread;
    thp.priority = 1;
    thp.stacksize = 2048;
    tid = thbase_CreateThread(&thp);
    return thbase_StartThread(tid, NULL);
}

INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvCreat);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvWaits);
INCLUDE_RODATA("SDFSDMAN", aP2sdirxspuiopm);
INCLUDE_RODATA("SDFSDMAN", aP2sdirxspuiopm_0);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvScesi);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvIfcou);
INCLUDE_RODATA("SDFSDMAN", aSdfsnddrvUnkno);
INCLUDE_RODATA("SDFSDMAN", jtbl_00006800);
INCLUDE_ASM("SDFSDMAN", sdfSndDrvRpcInit);
