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

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F3C0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F788);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F7E0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F880);

void func_0032F8C0(void) {
}

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F8C8);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_004CD378);

INCLUDE_RODATA(const s32, "kernel/ModeSelect", D_004CD390);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032F900);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_0032FF58);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330768);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330840);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003308E8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330A18);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330A78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330C48);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330D98);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330F10);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00330F78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331160);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331450);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331568);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331670);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331780);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003318C0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331B60);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331BA0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331D30);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331D80);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00331E68);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332070);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332140);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332178);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332210);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332448);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332478);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003324E0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003327A0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003328D0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332960);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332A30);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332AD0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332BD8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00332F10);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00333188);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003331B0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00333368);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00333538);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00333590);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00333820);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334420);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334AC0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334CD0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334D28);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334DC8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00334F50);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335008);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335238);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003352B8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335338);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003353B8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335490);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335510);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335540);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003355E0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335658);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003357A0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003357A8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003357B0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335888);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335980);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335A40);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335A78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335B28);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335BD0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335D78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335D90);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00335EB0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336080);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003361C8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336378);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336530);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336818);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336850);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003368A0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336928);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336978);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003369D0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336AE0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336B78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00336D78);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003371B0);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003372D8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00337350);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00337958);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00337998);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003379C8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00337EA8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00337F18);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003381A8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_003381C8);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00338220);

void nbPlaySe(s32 no)
{
    p2sdPlaySE(no, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00338258);

INCLUDE_ASM(const s32, "kernel/ModeSelect", func_00338280);
INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3770);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3778);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3780);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3788);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3790);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3798);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37A0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37A8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37B0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37B8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37C0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37C8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37D0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37D8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37E0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37E8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37F0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E37F8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3800);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3808);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3810);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3818);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3820);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3828);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3830);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3838);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3840);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3848);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3850);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3858);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3860);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3868);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3870);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3878);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3880);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3888);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3890);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E3898);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E38A0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E38A8);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E38B0);

INCLUDE_SDATA(const s32, "kernel/ModeSelect", D_004E38B8);

