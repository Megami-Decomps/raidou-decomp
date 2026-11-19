#ifndef SBTLCMNDMENUNEW_H
#define SBTLCMNDMENUNEW_H

#include "common.h"

void DrawFuncSkl(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_fontColor, u8 a_alphaRate);
void btlLoadBossEventTable(void);
s32 btlLoadKaiwaData(void);

#ifdef VERSION_RAIDOU2
#include "kernel.h"
#include "sdfMemory.h"
#include "dat.h"
#include "sbtlWork.h"

typedef struct {
    sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    dds3ProcessID_t *menuPID;
    dds3ProcessID_t *menuPID2;
    u8 unk10;
    u8 field11; /* party slot: DrawProc draws sbtlGetNakama(field11) */
    u8 stat; /* 1 and 2 count as started (sbtlIsStart), 4 deleted */
    u8 unk13;
    s8 field14; /* DrawProc draws the tab panel when set */
    u8 unk15[0x3];
    s32 field18;
    u8 unk1C[0x4];
    f32 field20;
    u8 unk24[0x4];
    s32 field28;
    u8 unk2C[0x20];
} sbtlCmndMenuNew_t;
SIZE_CHECK(sbtlCmndMenuNew_tSizeCheck, sbtlCmndMenuNew_t, 0x4C);

extern const char sbtlStrCmndMenu[];
extern const char sbtlStrCmndMenuDraw[];
extern dds3ProcessID_t *sbtlData181;
s32 Proc_sbtlCmndMenuNew(dds3ProcessID_t *id);
void Shutdown_sbtlCmndMenuNew(dds3ProcessID_t *id);
sbtlCmndMenuNew_t *GetProcessData(void);
void Delete_sbtlCmndMenuNew(void);
s32 sbtlGetStat(void);
s32 DrawProc(dds3ProcessID_t *id);
extern u32 DrawProcData;
s32 GetTabLimit(s32 *a_pmin, s32 *a_pmax);
void DrawRepeatIconEx(s32 a_x, s32 a_y, s32 a_z, f32 a_angle, f32 a_alphaRate, DDS3_OT a_otNo, struct SBTLUNIT_WORK *a_pBtlUnit, SKLID a_sklId);
extern u32 sbtlCheckData;

/* Boss event table row (0x94 bytes), loaded from /battle/sb_bossevtbl.dat; ends at devil_id -1. */
typedef struct TYPE_BOSS_NML_EVENT {
    s32 enc_id;
    s32 devil_id;
    s32 cam_size; /* HD BOSSEVENT_CAMERA; 4 aims between two helper points */
    s32 skill_id;
    s8 start_on;
    s8 mag_on1;
    s8 mag_on2;
    s8 mag_on3;
    s8 dead_on; /* bits 0-1 enable, bit 2 only for the last enemy */
    s8 lose_on;
    s8 dying_on;
    s8 time_on;
    u8 pad18[0x94 - 0x18];
} TYPE_BOSS_NML_EVENT;

extern TYPE_BOSS_NML_EVENT *arrBossEveTable;

/* Boss event process work (sbtlCallGetProcessStatData); only the fields read so far. */
typedef struct sbtlBossEvent_t {
    u8 pad00[0x1E];
    s16 eventtype; /* HD SBTLBOSSEVENT: 0 start, 1-3 mag, 4 dead, 5 lose, 6 dying, 7 time */
    u8 pad20[0x2C - 0x20];
    TYPE_BOSS_NML_EVENT *pTypeBossNmlEvent;
} sbtlBossEvent_t;

/* HD S_CameraMoveDt; R2 entries hold only the two position moves. */
typedef struct S_CameraMoveDt {
    struct S_PosMove *m_pPosMoveCam;
    struct S_PosMove *m_pPosMoveTgt;
} S_CameraMoveDt;

extern u8 sbtlData138[][7]; /* camera move pattern per event kind and cam_size */
S_CameraMoveDt *sbtlBtlRunSklProcSub(s32 idx); /* HD sBtlGetCamMovePtn */
void sbtlRefSbtl65(S_CameraMoveDt *dt); /* HD sBtlSetMove */
extern u32 btlLoadBossEventTableData;

/* Time limit battle table entry, matched by (field0, field4) in Start_sbtlTime. */
typedef struct {
    s32 field0;
    s32 field4;
    s32 timeLimit;
    s8 mode;
    u8 fieldD;
    u8 padE[0x2];
} sbtlTimeTbl_t;

extern sbtlTimeTbl_t Start_sbtlTimeData[29];
extern s32 gb_btltime; /* frames counted by the time limit battle */
extern s32 Result_sbtlTimeData;
extern s32 Result_sbtlTimeData2;
s32 Start_sbtlTime(s32 arg0, s32 arg1);
s32 Result_sbtlTime(void);
s32 End_sbtlTime(void);
/* Message ids per sword kind. */
typedef struct {
    s32 field0;
    s32 field4;
    s32 field8;
} sbtlSwordMsg_t;

extern sbtlSwordMsg_t sbtlData140[];
datSword_t *sbtlDat(void);
s32 sbtlCallBtlDat(void);
s32 sbtlCallBtlDat2(void);
s32 sbtlCallBtlDat3(void);
s32 sbtlCallBtlDat4(void);
void sbtlRefSbtl25(sbtlSummonText_t *text);
void sbtlRefSbtl26(sbtlSummonText_t *text);
void sbtlRefSbtl27(void (*func)(sbtlSummonText_t *text));
void sbtlRefSbtl28(void);
void sbtlRefSbtl29(sbtlSummonText_t *text, sdfVector4_t *pos, s32 arg2, s32 arg3, s32 arg4);
sbtlSummonText_t *sbtlRefSbtl30(void);
void sbtlCallSetOt(sbtlSummonText_t *text);
extern void (*sbtlCallSetOtTable[])(sbtlSummonText_t *text);
s32 sbtlBtlFirstSummonTextSetSub(sdfVector4_t *pos, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void sbtlStart(void);
s32 sbtlCheckEnemyAllDead(void);

/* Battle talk files: names, then the two loaded files' handles, data and sizes. */
extern const char *sbtlData136[];
extern sdfMemHandle_t *sbtlWork12[4];
extern u32 slWork2[4];
extern u32 sbtlWork14[4];
extern u32 sbtlData179;
extern u32 btlLoadKaiwaDataData;
s32 sbtlCallMemFreeDirect3(void);

/* One entry of sbtl_kaiwa_data.dat (0xD8A bytes). */
/* One talk entry, 0x22 bytes. */
typedef struct {
    s16 field00;
    u16 field02;
    s16 field04;
    s16 field06;
    u8 pad08[0x2];
    u16 field0A;
    s16 field0C[4];
    u16 field14[4];
    u8 pad1C[0x22 - 0x1C];
} sbtlKaiwaEntry_t;

typedef struct {
    u8 pad000[0xC];
    sbtlKaiwaEntry_t entry[48];
    sbtlKaiwaEntry_t entry2[48];
    u8 padCCC[0xD1A - 0xCCC];
    u16 fieldD1A;
    u8 padD1C[0x2];
    u16 fieldD1E;
    u8 padD20[0x2];
    u16 fieldD22;
    u8 padD24[0xD2C - 0xD24];
    s16 fieldD2C[47];
} sbtlKaiwaData_t;

typedef struct {
    sdfMemHandle_t *hMem;
    fileRequest_t *file;
    void *pData;
} sbtlKaiwaMsg_t;

/* Work of the battle talk process (inputpanel_id2), 0xF4 bytes. */
typedef struct {
    sdfMemHandle_t *hMem;
    dds3ProcessID_t *drawPID;
    dds3ProcessID_t *menuPID;
    sbtlKaiwaData_t *data;
    u8 pad10[0x6];
    s16 field16; /* evtGetCapturedWindowPanelValue */
    u8 pad18[0x2];
    u16 field1A;
    s16 field1C;
    u8 pad1E[0x2];
    s16 field20;
    u8 pad22[0x2];
    s16 kind;
    s16 field26;
    s16 field28;
    s16 field2A;
    u8 pad2C[0x3C - 0x2C];
    s16 field3C; /* party slot of the devil cmpCallGetProcessData5 picked */
    u8 pad3E[0x2];
    s16 field40;
    u8 pad42[0x46 - 0x42];
    s16 field46;
    sbtlKaiwaMsg_t msg[3]; /* copies of the talk files */
    u8 pad6C[0x98 - 0x6C];
    s16 field98; /* talk result from the devil table entry, 0 for none */
    s16 field9A;
    s16 field9C;
    u8 pad9E[0xAA - 0x9E];
    s8 fieldAA;
    s8 fieldAB;
    s32 fieldAC; /* unit ID sbtlCallGetProcessData5 picked to join the talk */
    u8 padB0[0x2];
    s8 fieldB2;
    u8 padB3[0xB8 - 0xB3];
    s16 fieldB8;
    u8 padBA[0xBC - 0xBA];
    s32 fieldBC;
    s16 fieldC0; /* cmpData29 entry of the party talk */
    s16 fieldC2;
    u8 padC4[0xC8 - 0xC4];
    s16 fieldC8; /* load step of sbtlCallGetProcessData38 */
    s16 fieldCA;
    f32 fieldCC[8]; /* copied from cmpData29 field0C[4..11] */
    s16 fieldEC;
    s16 fieldEE;
    s8 fieldF0;
    u8 padF1[0xF4 - 0xF1];
} sbtlKaiwa_t;

extern s8 evtKAIWANoEnemyData2;
s32 evtKAIWANoEnemy(dds3ProcessID_t *id);
void evtMoonShutdown(dds3ProcessID_t *id);
s32 evtCallGetProcessData4(dds3ProcessID_t *id);
void cmpSbtlkaiwaMain(void);
void sbtlProcessStart(SBTLBOSSEVENT type, s16 eventno);
s32 cmpCallGetProcessData2(s32 index, s32 kind);
s32 slRefSbtl(s32 devilId);

/* Party talk table entry (0x3C bytes): the party devil and the enemy devil it can talk with. */
typedef struct {
    s16 devilId;
    s16 field02; /* compared with SBTL_DEVIL_DATA.field82 */
    u8 pad04[0xC - 0x4];
    f32 field0C[12];
} cmpData29_t;

extern cmpData29_t cmpData29[80];
s32 cmpCallBtlCreateTargetList(void);
s32 cmpCallGetProcessData5(void);
s32 cmpCallGetProcessData6(void);
s32 sbtlLeaf37(s32 devilId, s32 kind);
void sbtlCallGetProcessData2(struct SBTLUNIT_WORK *wk);
s32 sbtlCallGetProcessData29(void);
void cmpPartyListMake_cmpMisc(dds3ProcessID_t *menuPID);
s32 slCallGetProcessData6(void *msg, s32 arg1, struct SBTLUNIT_WORK *unit, s32 arg3, s32 arg4, s32 arg5);
s32 slCallGetProcessData7(s32 arg0);
s32 slCallGetProcessData8(s32 arg0);
s32 nbCallGetProcessData12(void);
s32 nbCallGetProcessData13(void);
s32 nbCallGetProcessData16(void);
s32 trmSetProcessTerm(void);
s32 slCallBitCheck2(s32 devilId, s32 arg1);
s32 slChoiceMsgDraw(s32 msgNum);
s32 trmCallBtlSearchUnitID(void);
void trmCallBtlSearchUnitID5(void);
s32 trmCallGetProcessData(void);
struct SBTLUNIT_LISTWORK *trmCallBtlCreateUnitList(u8 group);
extern s32 cmpCallGetProcessData10Work[];
s32 mapMenuStart(void);
s32 sbtlCheckLoopMotion(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
struct sdatUnitWork_t *sbtlRefDDS3GBWK2(u32 index);
struct SBTLUNIT_WORK *GetTargetUnit_sbtlCmndMenuNew(void);
void sbtlHelp(s32 type, s32 num, u8 alphaRate);
s32 cmpGetNowSummonerRank(void);
void sbtlResetAction_Flag(struct SBTLUNIT_WORK *wk);
s32 trmBtlAnalyzeMeisiSub(s16 id);
s32 sbtlRefDDS3GBWK4(s32 id);
void sbtlRefDDS3GBWK5(s32 id, s32 value);
s32 sbtlCallBtlCheckLoopMotion(struct SBTLUNIT_WORK *wk);
void sbtlLeaf43(sbtlSummonText_t *text);
void evtCallBtlSearchUnitID2(void);
void sbtlCallBtlSearchUnitID15(s32 add);
s32 nbCallGetProcessData14(void);
void sbtlSetCmbBfrPos(void);
s32 sbtlCallBtlGetSkilltbl15(SKLID skillid);
void Add_AW_MagWait(s32 type, s32 count);
s32 cmpCallBtlSearchUnitID2(void);
s32 slCallBtlSearchUnitID(void);
void evtCallMemset3(s32 index, s32 value);
void evtCallGetProcessData3(void);
s32 cmpCallGetNowSummonerRank(s32 flag);
void trmCallBtlClearHitC(struct SBTLUNIT_WORK *wk);
void trmCallBtlCheckReturnUnit(s32 motionFlag);
s32 sbtlIsStart(s8 idx);
s32 sbtlCallGetProcessData38(s32 idx);
void CameraPosSet(s32 hpos);
void CameraSet(void);
void sbtlDeleteDeBuffAll(BST keep);
void ResetBadStatusAll(BST IgnoreBstNo);
s32 sbtlCallGetProcessData14(void);
s32 cmpCallGetProcessData7(void);
extern char cmpPartyListMake_cmpMiscData[];
extern sdfVector4_t trmWork;
extern sdfVector4_t trmCallBtlSearchUnitID5Work;
extern sdfVector4_t trmWork2;
void cmpPartyAnimFunc(dds3ProcessID_t *PID);
void cmpCallGetProcessData9(s32 a_id, s32 a_x, s32 a_y, s32 a_z, u32 a_color, u8 a_alpha);
s32 cmpCallGetProcessData3(s32 devilId);
s32 cmpCallBtlSearchUnitID(s32 devilId);
s16 cmpCallGetProcessData4(s32 second);
void sbtlInit(void);
void slCallSin(u8 a_alphaRate);
#endif

#endif
