#include "common.h"

/* Declarations from smt3-decomp src/smt3/newbattle/nbSound.c */
#include "common.h"

#include "newbattle.h"

#include "libsdf.h"

#include "basic.h"

#include "scr.h"

#include "ps2asm.h"

#include "newdata.h"

typedef struct evtUnitHandle_t
{
    u8 unk_00[0x8C];
    dds3Basic_t *basic;
    u8 unk_90[0x14];
    u32 flags;
} evtUnitHandle_t;

typedef struct effBTLSysHandle_t effBTLSysHandle_t;

typedef struct NB_FILEHANDLE_T NB_FILEHANDLE_T;

typedef struct fileRequest fileRequest_t;

/* 0xE0 bytes; only the field this unit reads is named. */
typedef struct
{
    s16 id;
    u16 unk_02;
    s32 refcnt;
    fileRequest_t *file[26];
    sdfMemHandle_t *buf[26];
    fileRequest_t *motfile;
    sdfMemHandle_t *motbuf;
} nbStatWork_t;

/* Only the fields this unit touches are named; the rest of the layout is unconfirmed. */
typedef struct nbMainProcessData_t
{
    nbFormation_t *form;
    u8 unk_04[0xC];
    nbParty_t *party;
    u8 unk_14[0x4];
    s16 playerpcnt;
    s16 enemypcnt;
    u8 unk_1C[0x4];
    nbStatWork_t *statwork;
    void *statbuf;
    s32 unk_28;
    u8 unk_2C[0x8];
    s32 encno;
    u8 unk_38[0x8];
    s32 actionstackp;
    u32 endtype;
    u8 unk_48[0x18];
    u32 stat;
    u8 unk_64[0x4];
    u8 turn;
    u8 unk_69[0xF];
    s16 press4_p;
    s16 pressturn;
    u16 stagemajor;
    u8 unk_7E[0x6];
    s32 unk_84;
    s32 eventno;
    s32 eventrid;
    s16 negopformindex;
    s16 negoeformindex;
    s32 negoresult;
    u8 item[4];
    u8 itemcnt[4];
    u32 maka;
    u32 exp;
    u8 unk_A8[0xC];
    nbPacHandle_t *formpack;
    u8 unk_B8[0x4];
    dds3ProcessID_t *MainProcessID;
    u8 unk_C0[0x1C];
    void *packdata;
    u8 unk_E0[0x14];
    dds3ProcessID_t *koukaproc;
    dds3ProcessID_t *negoproc;
    u8 unk_FC[0x4];
    dds3ProcessID_t *debugproc;
    dds3ProcessID_t *aiproc;
    dds3ProcessID_t *pnlproc;
    dds3ProcessID_t *helpproc;
    dds3ProcessID_t *autocommproc;
    dds3ProcessID_t *encproc;
    dds3ProcessID_t *eventproc;
    u8 unk_11C[0x4];
    void *pnltex[4];
    s32 unk_130;
    s32 unk_134;
    nbPacHandle_t *modelpack;
    void *modelpackbuf;
} nbMainProcessData_t;

/* 0x1A4 bytes; only the fields this unit touches are named. */
typedef struct nbActionProcessData_t
{
    nbMainProcessData_t *data;
    u32 stat;
    u8 unk_08[0x1C];
    nbParty_t *party;
    nbFormation_t *form;
    datUnitWork_t *work;
    s32 partyindex;
    s32 type;
    s32 target;
    s32 select;
    u8 unk_40[0x28];
    s32 massuruid;
    u8 unk_6C[0x124];
    u32 aiselect;
    s32 aijokentbl;
    u8 aichktbl[0x8];
    u32 aisummonid;
} nbActionProcessData_t;

typedef struct nbNegoProcessData_t
{
    nbMainProcessData_t *data;
    nbActionProcessData_t *actdata;
    u32 stat;
    s32 negotype;
    s32 waitframe;
    u32 nownegoid;
    s32 nownegophase;
    u32 nextnegoid;
    s32 nextkouhocnt;
    u32 nextkouho[8];
    s32 nowmsgid;
    s32 nowmsgno;
    s32 nowmes;
    s32 msgwndid;
    u32 msgwndcnt;
    s32 nowselect;
    u32 workid[8];
    s32 requesttype;
    s32 requestdata;
    s32 requesthpmp;
    s32 prevreqtype;
    s32 cnt6_36;
    s32 cnt6_37;
    s32 cnt6_58;
    s32 cnt6_66;
    s32 mes6_2;
    s32 mes6_25;
    s32 mes6_67;
    s32 flag1_6;
    s32 flag1_39;
    s32 historyindex;
    u32 history[14];
    s32 historymes[14];
    s32 endid;
    s32 endtype;
    s32 negoreqtype;
    u32 negoskillid;
    s32 aisyo;
    s32 aisyomes;
    s32 hojopartyindex;
    s32 hojoskillid;
    s32 hojoseikou;
    s32 hojoaisyo;
    s32 hojoaisyomes;
    s32 qtype;
    s32 qindex;
    datUnitWork_t *pwork;
    datUnitWork_t *ework;
    s32 pformindex;
    s32 eformindex;
    nbFormation_t *pform;
    nbFormation_t *eform;
    s32 ppartyindex;
    s32 epartyindex;
    nbParty_t *pparty;
    nbParty_t *eparty;
    s32 pkucyo;
    s32 ekucyo;
    s32 innen_id;
    s32 temp_param[4];
    s32 boupartyindex;
    s32 getmakka;
    s32 getitem;
    s32 getinfotype;
    s32 getinfomsg;
    s32 flowmsg[31];
    s32 questionmsg;
    s32 aisyomsg[31];
    s32 infomsg[31];
    s32 helpmsg;
    s32 helpmsg2;
    s32 innenmsg;
    s32 bossmsg;
    s32 selectcur;
} nbNegoProcessData_t;

#include "sdk_libc.h"

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern void *memset(void *a, s32 b, size_t c);

extern char nbTestHeartsMessage[];

extern void nbPlaySe(s32 no);

extern char nbTestProcessName[];

extern void p2sdPlaySE(s32 seno, s32 vol, s32 pan);

extern s32 nbLoadMotionByIndex(s32 unitid, s32 index);

extern s32 nbResultCount;

extern s32 bgmno;

extern void p2sdPlayBGM(s32 MidiNo);

extern s32 nbBgmSaved;

extern void p2sdSetBgmTrack(s32 track);

extern s32 nbBgmFadeState;

extern void p2sdStopBGM(s32 no);

extern void sdfSndSendChannelGroupCommand150(s32 no);

extern const char nbBgmStopFormat[];

extern u8 datModelParamTable[];

extern u8 nbDefaultUnitVisual[];

extern void nbFreeFormResources(nbMainProcessData_t *data, s32 index);

extern void *sdfMemAllocDirect(u32 aSize);

extern void *sdfMemLock(void *aHandle);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern void p2sdLoadSMGFile(s32 MidiNo);

extern void nbSetMaeFlag(s32 flag);

extern s32 (*D_00527518[])(nbEventProcessData_t *e);

extern s32 nbFindFormIndex(s32 id);

extern s32 nbMaeCheckDone(void);

extern void nbMaeWaitStep(void);

extern void sdfMemFreeDirect(void *aHandle);

extern void p2sdSetPcmBuffer(void *buf, u32 size);

extern void datDebugPrintf(const char *format, ...);

extern const char nbNegoSeOkMsg[];

extern const char nbMidiDebugFmt[];

extern const char nbMotSeOkMsg[];

extern s32 (*D_00527420[])(nbEventProcessData_t *e);

extern s32 nbMaeMidiNo;

extern s32 nbMaeSubMidiNo;

extern s32 nbMaeFlag;

extern s32 nbMaeLoadDone;

extern void nbBgmStop(void);

extern s32 nbFindFreeFormSlot(void);

extern s32 (*D_004F0788[])(nbActionProcessData_t *a, s32 joken);

extern s32 (*D_00527688[])(nbEventProcessData_t *e);

extern s32 (*D_00527698[])(nbEventProcessData_t *e);

extern s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);

extern sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle);

extern s32 fileHandleFree(fileRequest_t *fileHandle);

extern s32 (*nbE902Table[])(nbEventProcessData_t *e);

extern s32 nbLoadMidiFile(s32 no);

extern u32 sdfMemGetSize(void *aHandle);

extern s32 (*D_004EF4B0[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF550[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF698[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF6F8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF780[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7C8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF7D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF810[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF868[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF8D8[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF928[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF950[])(nbNegoProcessData_t *n);

extern s32 (*D_004EF960[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA28[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFA60[])(nbNegoProcessData_t *n);

extern s32 (*D_004EFAC0[])(nbNegoProcessData_t *n);

s32 nbGetUnitWorkFromAction(nbFormation_t *form);

void nbPlaySe(s32 no);

s32 nbGetMotionTable(s32 unitid, s32 index);

extern u16 nbUnitMotionIdTable[];

extern s32 evtBitCheck(s32 no);

s32 nbLoadMotionByIndex(s32 unitid, s32 index);

s32 nbGetUnitFromAction(nbFormation_t *form, s32 index);

s32 nbMakeSeId(s32 seno);

s32 nbPlayMotionSe(nbFormation_t *form);

s32 nbMemoryLoadNegoSe(nbFormation_t *form);

s32 nbMemoryLoadMotionSe(nbFormation_t *form, s32 mot);

void nbGetFormEntry(s32 id);

nbStatWork_t *nbSetFormEntry(s32 id);

extern char nbMidiPath[];

extern fileRequest_t *fileLoad(const char *path);

extern const char nbNegoSeOkMsg[16];

extern const char nbMidiDebugFmt[16];

extern const char nbMotSeOkMsg[16];

s32 nbLoadMidiFile(s32 no);

void nbCheckPackLoad(nbMainProcessData_t *data);

s32 nbFindFreeFormSlot(void);

s32 nbFindFormIndex(s32 id);

void nbFreeFormResources(nbMainProcessData_t *data, s32 index);

void nbFreeAllFormResources(nbMainProcessData_t *data);

void nbAllocMainProcessData(nbMainProcessData_t *data);

s32 nbMaeCheckDone();

extern const u16 nbMaeMidiTable1[16];

extern const u16 D_00545C58[16];

extern const u16 nbMaeMidiTable2[20];

void nbIncResultCounter(void);

void nbMaeWaitStep(void);

void nbRestoreBgm(void);

void nbBgmFadeOut(void);

void nbBgmStop(void);

void nb_28DE10(void);

void nb_28DE28(void);

extern const char nbBgmStopFormat[24];

extern const char nbUnitLoadTimeFormat[72];

extern s32 bgmno;

extern s32 nbBgmSaved;

extern s32 nbBgmFadeState;

extern s32 nbResultCount;

extern s32 nbMaeMidiNo;

extern s32 nbMaeSubMidiNo;

extern s32 nbMaeFlag;

extern s32 nbMaeLoadDone;
/* End of declarations from src/smt3/newbattle/nbSound.c */

INCLUDE_ASM(const s32, "kernel/ModeSelect", InitEx);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002CF148);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002CF1A0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002CF240);

void func_r1_002CF280(void) {
}

INCLUDE_ASM(const s32, "kernel/ModeSelect", FreeEffect);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_r1_004601B8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_r1_004601D0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ManageModelPhase);

INCLUDE_ASM(const s32, "kernel/ModeSelect", PadDealer);

INCLUDE_ASM(const s32, "kernel/ModeSelect", InitHanchan);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0190);

INCLUDE_ASM(const s32, "kernel/ModeSelect", TsumikomiDecide);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0368);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D03C8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0598);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D06E8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0860);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D08C8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0AB0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0DA0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0EB8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D0FC0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D10D0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D1210);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D14B8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D14F8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D1688);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D16D8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D1778);

INCLUDE_ASM(const s32, "kernel/ModeSelect", SetHaipai);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D1A30);

INCLUDE_ASM(const s32, "kernel/ModeSelect", NextKyoku);

INCLUDE_ASM(const s32, "kernel/ModeSelect", InitKyoku);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D1D38);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D1D68);

INCLUDE_ASM(const s32, "kernel/ModeSelect", Ryukyoku);

INCLUDE_ASM(const s32, "kernel/ModeSelect", TochuRyukyoku);

INCLUDE_ASM(const s32, "kernel/ModeSelect", SetTehai14);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D2250);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D2310);

INCLUDE_ASM(const s32, "kernel/ModeSelect", Suteru);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D24B8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", AITurn);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D2A68);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D2A90);

INCLUDE_ASM(const s32, "kernel/ModeSelect", Ignore);

INCLUDE_ASM(const s32, "kernel/ModeSelect", mjnAfterConfirmationKan);

INCLUDE_ASM(const s32, "kernel/ModeSelect", NakiWait);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D30F8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D3CF0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ApplyRonRyukyoku);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D45A0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D45F8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4690);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4810);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D48C0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4AF0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4B68);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4BE8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4C68);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4D40);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4DC0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", AskRon);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4E90);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D4F08);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5050);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5058);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5060);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5138);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5230);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D52F0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5328);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D53D8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5480);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5628);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5640);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5760);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5930);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5A78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5C28);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D5DE0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ApplyRon);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D6100);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D6150);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D61D8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D6228);

INCLUDE_ASM(const s32, "kernel/ModeSelect", InitDevil);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D6390);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D6408);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D65F8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D69D0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ManageSummonPhase);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_002D6B70);

INCLUDE_ASM(const s32, "kernel/ModeSelect", InitInterface);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D7080);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D70B0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D7540);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ManageEndHanchanPhase);

INCLUDE_ASM(const s32, "kernel/ModeSelect", EndMahjong);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ModelLoad_mjn);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D78B8);

void nbPlaySe(s32 no)
{
    p2sdPlaySE(no, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_r1_002D78F0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", ShutDown);
INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E7E0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E7E8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", Work_mjn);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E7F8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E800);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E808);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E810);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E818);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E820);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E828);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E830);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E838);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E840);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E848);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E850);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E858);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E860);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E868);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E870);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E878);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E880);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E888);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E890);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E898);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8A0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8A8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8B0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8B8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8C0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8C8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8D0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8D8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8E0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8E8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8F0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E8F8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E900);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E908);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E910);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E918);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E920);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_r1_0046E928);

