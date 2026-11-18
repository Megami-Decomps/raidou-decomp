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
    u8 unk10[0x2];
    u8 stat; /* 1 and 2 count as started (sbtlIsStart), 4 deleted */
    u8 unk13[0x39];
} sbtlCmndMenuNew_t;

extern const char sbtlStrCmndMenu[];
extern const char sbtlStrCmndMenuDraw[];
extern dds3ProcessID_t *sbtlData181;
s32 Proc_sbtlCmndMenuNew(dds3ProcessID_t *id);
void Shutdown_sbtlCmndMenuNew(dds3ProcessID_t *id);
sbtlCmndMenuNew_t *GetProcessData(void);
void Delete_sbtlCmndMenuNew(void);
s32 sbtlGetStat(void);
s32 DrawProc(dds3ProcessID_t *id);
extern u32 sbtlCheckData;
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
extern s32 sbtlData141; /* frames counted by the time limit battle */
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
void sbtlRefSbtl25(sbtlSummonText_t *text);
void sbtlRefSbtl27(void (*func)(sbtlSummonText_t *text));
void sbtlRefSbtl28(void);
void sbtlRefSbtl29(sbtlSummonText_t *text, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
sbtlSummonText_t *sbtlRefSbtl30(void);
void sbtlCallSetOt(sbtlSummonText_t *text);
s32 sbtlBtlFirstSummonTextSetSub(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

/* Battle talk files: names, then the two loaded files' handles, data and sizes. */
extern const char *sbtlData136[];
extern sdfMemHandle_t *sbtlWork12[4];
extern u32 slWork2[4];
extern u32 sbtlWork14[4];
extern u32 sbtlData179;
extern u32 btlLoadKaiwaDataData;
s32 sbtlCallMemFreeDirect3(void);

/* One entry of sbtl_kaiwa_data.dat (0xD8A bytes). */
typedef struct {
    u8 pad000[0xD2C];
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
    u8 pad10[0xA];
    u16 field1A;
    u16 field1C;
    u8 pad1E[0x6];
    s16 kind;
    s16 field26;
    u8 pad28[0x20];
    sbtlKaiwaMsg_t msg[3]; /* copies of the talk files */
    u8 pad6C[0xF4 - 0x6C];
} sbtlKaiwa_t;

extern u8 evtKAIWANoEnemyData2;
s32 evtKAIWANoEnemy(dds3ProcessID_t *id);
void evtMoonShutdown(dds3ProcessID_t *id);
s32 evtCallGetProcessData4(dds3ProcessID_t *id);
void cmpSbtlkaiwaMain(void);
s32 cmpCallGetProcessData2(s32 index);
void cmpPartyListMake_cmpMisc(dds3ProcessID_t *menuPID);
s32 slCallGetProcessData6(void *msg, s32 arg1, struct SBTLUNIT_WORK *unit, s32 arg3, s32 arg4, s32 arg5);
s32 slCallGetProcessData7(s32 arg0);
s32 slCallGetProcessData8(s32 arg0);
s32 nbCallGetProcessData13(void);
s32 nbCallGetProcessData16(void);
s32 trmSetProcessTerm(void);
#endif

#endif
