#ifndef SBTLWORK_H
#define SBTLWORK_H

#include "common.h"
#include "fileManager.h"
#include "kernel.h"
#include "sdf_types.h"
#include "sdatUnit.h"
#include "sdfSound.h"
#include "sbtlDMG.h"
#include "sBtlEff.h"

struct sprHandle_t;

/* Battle reward work, cleared by sbtlInitDropWork. The R1 layout differs from HD's and is not read yet. */
typedef struct SBTLDROP_WORK {
    u8 pad00[0xC4];
} SBTLDROP_WORK;

/* Pointed to by SBTL_WORK.panel at 0x628. */
typedef struct SBTL_PANEL {
    u8 pad00[8];
    struct sprHandle_t *resources;
    u8 pad0C[4];
    struct sprHandle_t *hPanelSpr3;
    u8 pad14[0xC];
    u32 flg;
} SBTL_PANEL;

struct SBTLDMG_HITLISTWORK;
struct SBTLDMG_LISTWORK;
struct SBTLDVLTK_LISTWORK;
struct SBTLEFF_DELLISTWORK;
struct SBTLEFF_LISTWORK;
struct SBTLEXP_LISTWORK;
struct SBTLITEM_LISTWORK;
struct SBTLSM_LISTWORK;
struct SBTLUNIT_LISTWORK;
struct SBTL_MODELBIND_LIST;
struct SBTL_STP;
struct SBTLEFF_WORK;
struct scrProcessWork_s;

/* Battle main work (0x87C bytes), pointed to by wk_sbtl. Only fields read so far are named. */
typedef struct SBTL_WORK {
    u8 pad000[0x150];
    s32 mapNo[2];
    u8 pad158[4];
    u32 startFlg; /* 8: map fade running, 0x10: second map fade running */
    u8 pad160[0x198 - 0x160];
    fileRequest_t *panelPack;
    s32 panelPackPos;
    s32 panelPackLen;
    u8 pad1A4[4];
    sdfMemHandle_t *handle;
    dds3ProcessID_t *process;
    s32 syscnt;
    s32 endcnt;
    u8 pad1B8[4];
    s16 equipSword;
    s16 equipGun;
    s32 equipBullet;
    u32 flg;
    u32 flg2;
    u8 pad1CC[4];
    s32 targetUnitID;
    u16 hitMdlNo; /* next mdl resource number for hit models (type 7) */
    u8 pad1D6[2];
    s32 stageNo;
    struct SBTLUNIT_LISTWORK *unitList;
    s32 loadListID;
    s16 battleMode;
    u8 pad1E6[0x1F0 - 0x1E6];
    s32 nego_gstat;
    u8 pad1F4[0x210 - 0x1F4];
    s16 eventno;
    u8 pad212[2];
    s16 eventgstat;
    u8 pad216[2];
    s32 eventListID;
    u8 pad21C[4];
    s16 summonIndex[2];
    u8 pad224[0x22C - 0x224];
    s32 useitemID;
    u8 pad230[4];
    s16 useitem_stock; /* stock slot the item is used on, -1 for none */
    u8 pad236[0x238 - 0x236];
    s32 equipBulletBk;
    u8 pad23C[0x2CC - 0x23C];
    struct SBTLUNIT_LISTWORK *list2CC;
    u8 pad2D0[4];
    struct SBTLUNIT_LISTWORK *list2D4;
    u8 pad2D8[4];
    s32 stockMinor[SDAT_STOCK_NUM];
    s32 stockDevilSkill[SDAT_STOCK_NUM];
    u32 summonBit;
    s32 stockNum;
    s32 field344;
    u8 pad348[0x358 - 0x348];
    u8 field358[SDAT_STOCK_NUM];
    s32 stockUnitID[SDAT_STOCK_NUM];
    u8 pad394[0x398 - 0x394];
    s32 stackID[2]; /* stock slots of the summoned demons (HD stack_id) */
    u8 pad3A0[0x3A4 - 0x3A0];
    s32 unitPadID;
    u8 pad3A8[0x3E8 - 0x3A8];
    struct SBTLUNIT_LISTWORK *list3E8;
    u8 pad3EC[8];
    struct SBTLDMG_LISTWORK *animList;
    struct SBTLDMG_HITLISTWORK *hitList;
    u8 pad3FC[4];
    struct SBTLEFF_LISTWORK *effList;
    s32 field404;
    s32 field408;
    struct SBTLUNIT_LISTWORK *field40C;
    struct SBTLITEM_LISTWORK *itemList;
    struct SBTLITEM_LISTWORK *bulletList;
    struct SBTLSM_LISTWORK *sysmsgList;
    struct SBTLEFF_DELLISTWORK *effdelList;
    struct SBTLDMG_HITWORK *huka2hwk; /* hit whose attacker data later hits copy (HD name) */
    s32 field424;
    u8 pad428[4];
    s32 hitTargetID;
    s16 mode;
    u8 pad432[0x436 - 0x432];
    s16 field436;
    s16 gstat[2];
    s16 walk_counter;
    s16 wait_counter;
    s16 field440;
    s16 field442;
    s16 field444;
    u8 pad446[0x448 - 0x446];
    s32 mag;
    u8 pad44C[0x458 - 0x44C];
    s32 hgunNum;
    u8 pad45C[0x478 - 0x45C];
    s16 field478;
    u8 pad47A[0x480 - 0x47A];
    f32 field480;
    s32 field484;
    s32 field488;
    s32 field48C;
    s32 field490;
    s32 field494;
    s32 field498;
    u8 pad49C[0x4A0 - 0x49C];
    u8 field4A0;
    u8 pad4A1[3];
    s32 field4A4;
    s32 field4A8;
    s32 field4AC;
    u8 pad4B0[0x4B4 - 0x4B0];
    u8 field4B4;
    u8 pad4B5;
    s16 field4B6;
    s16 field4B8;
    s16 field4BA;
    s16 field4BC;
    s16 field4BE;
    u8 pad4C0[0x4CE - 0x4C0];
    s16 field4CE;
    u8 pad4D0[0x4D4 - 0x4D0];
    s32 huumaTargetID;
    s32 huumaDevilID[12];
    s16 field508;
    s16 field50A;
    s16 field50C;
    u8 pad50E[0x2];
    f32 field510;
    f32 field514;
    f32 field518;
    SBTLDROP_WORK drop;
    s16 mf_mode;
    s16 mf_counter;
    s16 mf_cmpcounter;
    u8 pad5E6[2];
    u32 mf_abgr;
    u32 mf_abgr_s;
    s16 mf_r;
    s16 mf_g;
    s16 mf_b;
    s16 mfh_mode;
    s16 mfh_counter;
    s16 mfh_cmpcounter;
    u32 mfh_abgr;
    u32 mfh_abgr_s;
    s16 mfh_r;
    s16 mfh_g;
    s16 mfh_b;
    u8 pad60A[0x614 - 0x60A];
    s32 field614;
    u8 field618;
    u8 pad619[0x620 - 0x619];
    s32 field620;
    u8 field624;
    u8 pad625[3];
    SBTL_PANEL *panel;
    u8 pad62C[4];
    struct SBTL_FIELD630_WORK *field630;
    struct SBTLDVLTK_LISTWORK *dvltkList;
    struct SBTLEXP_LISTWORK *expList;
    struct SBTL_MODELBIND_LIST *modelBindings;
    u8 pad640[4];
    s32 field644;
    struct SBTL_STP *stp;
    s8 field64C;
    u8 pad64D[0x650 - 0x64D];
    struct scrProcessWork_s *hAIScript;
    u8 pad654[0x4];
    struct SBTLUNIT_WORK *scrUnit;
    u8 pad65C[0x674 - 0x65C];
    s32 field674;
    s32 field678;
    s32 field67C;
    struct SBTLUNIT_WORK *scrUnitList[16];
    s32 scrCmpCnt;
    s32 scrCnt[4];
    s32 scrEncountNo;
    u32 scrFlag;
    u8 pad6DC[4];
    s32 enshutuCnt[8];
    u8 pad700[0x720 - 0x700];
    s32 scrData[8];
    u8 pad740[0x74C - 0x740];
    s32 field74C;
    s32 field750;
    s32 field754;
    s32 field758;
    s32 field75C;
    s16 field760;
    s16 field762;
    s16 field764;
    s16 field766;
    s16 field768;
    s16 field76A[9];
    s32 field77C;
    p2sdLoadEntry_t midiLoad[30]; /* copy of the sound driver's MIDI table; 11 to 28 are skill MIDIs */
    s32 midiSkillCount;
    s32 midiCount8;
    u8 field878;
    u8 field879;
    u8 field87A;
    u8 pad87B[0x87C - 0x87B];
} SBTL_WORK;

extern SBTL_WORK *wk_sbtl;

/* Devil table entry returned by sbtlGetDevilData_sbtlSub. */
typedef struct SBTL_DEVIL_DATA {
    u32 flag;
    u8 field04;
    u8 pad05;
    s16 motionId;
    u8 pad08[2];
    s16 field0A;
    s16 aisyoId;
    u8 field0E;
    u8 field0F;
    u8 pad10[4];
    s16 field14;
    u8 pad16[2];
    u32 stat18;
    u32 stat1C;
    u32 stat20;
    u32 stat24;
    u16 resist[4];
    s16 param[9];
    s16 field42;
    u8 pad44[0x60 - 0x44];
    f32 field60;
    f32 field64;
    u8 pad68[2];
    s16 huumaMesNo;
} SBTL_DEVIL_DATA;


/* Battle start parameters, copied to main_data by sbtlSbattle. */
typedef struct SBTL_START_DATA {
    sdfVector4_t pos;
    sdfVector4_t rot;
    u8 pad20[8];
    s32 val28;
    s32 mode;
    s16 val30;
    s16 stockId; /* stock slot summoned first in battle mode 3, used when below stockNum */
} SBTL_START_DATA;

/* One loaded SB_PACK resource, indexed by the pack entry id. */
typedef struct SBTL_PACK_SLOT {
    void *data;
    struct sdfMemHandle_t *handle;
    void *unk8;
    void *unkC;
} SBTL_PACK_SLOT;

SBTL_PLAYER_DATA *sbtlGetPlayerData(void);
s32 sbtlCallBtlGetPlayerData3(void);
s32 sbtlCallBtlGetMotionVisualData(struct SBTLUNIT_WORK *wk, s32 motionID, s16 bframe, s16 aframe, s8 flg);
f32 sbtlCallBtlGetMotionVisualData4(struct SBTLUNIT_WORK *wk, s32 type);
s32 sdfRemoveTaskItem(struct SBTLUNIT_WORK *wk, s32 flg);
s32 sbtlCallBtlGetSkilltbl8(struct SBTLUNIT_WORK *wk, u32 flg);

/* One battle stage's row (0x100 bytes) in the table sbtlData100 points at, indexed by SBTL_WORK.stageNo. */
typedef struct SBTL_STAGE_DATA {
    s8 mapNo[2];    /* 0x00: copied to SBTL_WORK.mapNo */
    s8 form;        /* 0x02: mapMapSetForm, 0 for none */
    s8 encPoint;    /* 0x03: mapEncPointGet point */
    u16 flags;      /* 0x04: 0x20 stops units at battle start */
    s16 bgmId;      /* 0x06 */
    u8 pad08[0xF8];
} SBTL_STAGE_DATA;

extern SBTL_STAGE_DATA *sbtlData100;

/* Status marker over a unit (HD SBTLSTP_WORK), linked through next. R1 layout differs from HD. */
typedef struct SBTL_STP_NODE {
    sdfVector4_t pos;
    u32 status;
    s32 mode; /* SBTLSTP_MODE */
    u8 pad18[4];
    s16 id; /* SBTLSTP_ID */
    u8 pad1E[2];
    s32 counter;
    s32 counterMax;
    s32 alpha;
    u8 pad2C[4];
    f32 y;
    f32 spd;
    s32 ListID;
    u8 pad3C[4];
    struct SBTL_STP_NODE *next;
} SBTL_STP_NODE;

/* 0x198 bytes, made by sbtlCallMemAllocDirect4, held in SBTL_WORK.field630. Partial layout. */
typedef struct SBTL_FIELD630_WORK {
    sdfMemHandle_t *handle;
    s32 num;
    u8 pad008[0x198 - 0x8];
} SBTL_FIELD630_WORK;

typedef struct SBTL_STP {
    s32 enabled;
    SBTL_STP_NODE *head;
} SBTL_STP;

typedef struct SBTL_MOTION_VISUAL {
    u8 kind;
    u8 pad01[3];
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 scale;
    u8 pad18[0x30 - 0x18];
    s16 field30;
    s16 field32;
} SBTL_MOTION_VISUAL;

typedef struct SBTL_UNIT_SUB {
    u16 flags;
    u8 pad02[2];
    f32 field04;
    u8 pad08[8];
    f32 field10;
    f32 field14;
    s16 field18;
} SBTL_UNIT_SUB;

struct SBTLUNIT_WORK;

extern SBTL_START_DATA main_data;
extern char sbtlData85[];
extern s32 sbtlSbattleData;
extern s32 sbtlSceneLoadData;
extern s32 sbtlSceneLoadData2;
extern s32 sbtlSceneLoadData3;
extern s32 sbtlData86;
extern s32 sbtlData90;
extern s32 sbtlData91;
extern f32 sbtlData92;
extern sdfVector4_t sbtlCallMemset4Data2;
extern sdfVector4_t sbtlCallMemset4Data3;
extern s32 sbtlCallMemset4Data4;
extern f32 sbtlCallMemset4Data5;
extern f32 sbtlCallMemset4Data6;
extern f32 sbtlCallMemset4Data7;
extern char *sbtlSobedSABData[];
extern SBTL_PLAYER_DATA sbtlCallBtlCheckTutorialBattleData;
extern sdfMemHandle_t *sbtlLoadOnMemMsgWork[20];
extern u32 sbtlWork5[20];
extern u32 sbtlWork6[20];
extern char *sbtlData17[20];
#ifdef VERSION_RAIDOU1
#define SBTL_PACK_SLOT_COUNT 44
#else
#define SBTL_PACK_SLOT_COUNT 46
#endif
extern SBTL_PACK_SLOT btlLoadSbPackData[SBTL_PACK_SLOT_COUNT];
extern s32 btlLoadSbPackData2;
extern s8 sbtl_effonfilecheck[];

/* One file held for a model in SBTL_PACDATA. */
typedef struct SBTL_PACDATA_FILE {
    void *data;
    sdfMemHandle_t *handle;
    s32 size;
    u8 pad0C[4];
} SBTL_PACDATA_FILE;

typedef struct SBTL_PACDATA_KEY {
    s32 major;
    s32 minor;
    s32 devilid;
} SBTL_PACDATA_KEY;

/* Files loaded for one model, found by major, minor and devil id. */
typedef struct SBTL_PACDATA {
    SBTL_PACDATA_FILE mht;
    SBTL_PACDATA_FILE motionSE;
    SBTL_PACDATA_FILE file20;
    SBTL_PACDATA_HITLIST hit;
    SBTL_PACDATA_KEY key;
} SBTL_PACDATA;

#define SBTL_PACDATA_NUM 16

/* Pointed to by sbtlData84, 0x908 bytes. */
typedef struct SBTL_PACDATA_WORK {
    SBTL_PACDATA pac[SBTL_PACDATA_NUM];
    u8 pad900[4];
    sdfMemHandle_t *handle;
} SBTL_PACDATA_WORK;

/* Pointed to by sbtlData83, 0x115C bytes. */
typedef struct SBTL_WORK2 {
    SBTLEFF_PACLIST effpac[SBTLEFF_PACLIST_MAX];
    void *effOnRsc_P;
    u8 pad1144[4];
    struct fileRequest *pacHandle; /* SB_PACK3.LB, player on-memory data */
    s32 setflg;
    s32 dataCnt;
    sdfMemHandle_t *handle;
    s16 effpaccnt; /* packs loaded, clamped to 0..SBTLEFF_PACLIST_MAX - 1 */
    u8 pad115A[2];
} SBTL_WORK2;

extern SBTL_WORK2 *sbtlData83;
extern SBTL_PACDATA_WORK *sbtlData84;
extern s8 VRF_DeltaTime;
extern u8 pbData;
extern sdfVector4_t aViewTarget;
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewDown;
extern sdfVector4_t pbWork;
extern sdfVector4_t pbWork2;
extern sdfVector4_t pbWork3;

s32 DataLoadProc(dds3ProcessID_t *id);
s32 DrawProc(dds3ProcessID_t *id);
s32 RestartBattle(void);
s32 sbtlCallBtlGetScrollID(void);
void pbSetGameCamera0(void);
void sbtlCallBtlDeleteOnData(void);
s32 sbtlCallBtlGetPlayerData(void);
s32 sbtlCallBtlSearchEffectData2(s32 id, u16 flg, void *pData);
s32 sbtlBtlLoadPlayerOnMemDataSub(s32 index, void *pData);
struct SBTLEFF_WORK *sbtlCallMemset17(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid, s32 starttime,
                                      const f32 *defpos, u32 rgba, s8 flg);
void sbtlCallMemset3(s32 battleMode, s32 stageNo);
s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo);
void sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckScriptData(s32 devilId);
s32 sbtlCheckTutorialBattle(void);
s64 sbtlCallBtlCheckTutorialBattle3(void);
void sbtlGetDevilData(struct SBTLUNIT_WORK *wk, s32 a, s32 b);
SBTL_DEVIL_DATA *sbtlGetDevilData_sbtlSub(s32 id);
s32 sbtlGetExeSkill(SKLID sklid);
s32 sbtlSobedSAB(SKLID id, char *buf);
SBTL_FIELD630_WORK *sbtlCallMemAllocDirect4(void);
s32 sbtlRefSbtl42(SBTL_FIELD630_WORK *wk);
SBTL_MOTION_VISUAL *sbtlGetMotionVisualData(s32 a, s32 id);
s32 sbtlGetScrollID(void);
void sbtlInitDropWork(void);
s32 sbtlLeaf(dds3ProcessID_t *id);
s32 OnDataLoadProc(dds3ProcessID_t *id);
void sbtlLeaf75(dds3ProcessID_t *id);
s32 sbtlLeaf54(dds3ProcessID_t *id);
void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode);
void sbtlRefSbtl31(void);
void sbtlResetRndEncCount(void);
void sbtlRndEncTopPriorityMap(s32 *mapOut, s32 *floorOut);
s32 sbtlSbattle3(void);
sdatUnitWork_t *sbtlGetStockUnitData(s32 id);
s32 sbtlLeaf53(sdatUnitWork_t *unit);
s32 sbtlGetDamage_GB(sdatUnitWork_t *acter, sdatUnitWork_t *unit, SKLID sklid);
s32 sbtlCallBtlGetItemtbl3(sdatUnitWork_t *unit, s32 itemID);
s32 sbtlCallBtlSearchUnitID35(s32 stockID);
s32 sbtlCheckItemMember_Dead(s32 stockID);
s32 sbtlSbattle5(dds3ProcessID_t *id);
s32 sbtlSbattle6(dds3ProcessID_t *id);
void sbtlSbtlCameraproc(void);
void sbtlSetScrollID(s32 id);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 side, s32 devilId, s32 arg);
SBTL_UNIT_SUB *sbtlSetUnitSub(s32 a, s32 id, s32 b);
void sbtlStopVibe(void);
s32 Draw_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk);
s32 Draw_sbtlScene(void);
void Init_sbtlDraw(void);
s32 Proc_sbtl(void);
s32 Proc_sbtlEXP(struct SBTL_MODELBIND_LIST *lwk);
s32 sbtlProcSBtlSTPSub(struct SBTLUNIT_WORK *wk);
s32 Proc_sbtlSTP(SBTL_STP *stp);
s32 Draw_sbtlSTP(SBTL_STP *stp);
s32 btlLoadSbPack(void);
void campFreeStatusModel(void);
struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void);
s32 effStopEffectStop(void);
SBTL_STP *evtAllocateContext(void);
s32 pbCallBtlCheckLoadFlg(void);
void sbtlAllInit(void);
s32 sbtlBattleSBPACK(void);
s32 sbtlCallStartRain(void);
s32 sbtlBattleEventSb(void);
void sbtlBattleStart(SBTL_STAGE_DATA *stage);
void sbtlSupport_BattleStop(s32 motionFlag, s32 effectFlag, s32 SEFlag);
s32 sbtlBtlEntryStockDataSub(sdatUnitWork_t *unit);
s32 sbtlBtlNakamaSetUpSub(sdatUnitWork_t *unit);
void sbtlCallBtlCheckEnshutuSummonUnit(void);
s32 sbtlCallBtlCheckGeneratorGroupID(void);
void sbtlCallBtlCheckTutorialBattle2(void);
s32 sbtlCallBtlEndAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg);
s32 sbtlSetMotionHit(struct SBTLUNIT_WORK *wk, s32 motionno, SKLID skillid);
s32 sbtlCallBtlGetItemtbl(void);
s32 sbtlCallBtlGetNumUnitGroupID(void);
s32 sbtlCallBtlMapInit(void);
s32 sbtlCallBtlSearchEffectSABActer(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchPacData(s32 major, s32 minor, s32 devilid, SBTL_HIT_PACFILE *file, s32 size);
s32 sbtlCallBtlSearchPacData2(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 DrawProc2(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlCallBtlSearchUnitID32(void);
s32 sbtlCallBtlSearchUnitID33(void);
s32 sbtlCallBtlSearchUnitID34(sdatUnitWork_t *unit, s32 devilId, s8 flg);
s32 sbtlCallBtlSearchUnitID7(s32 x, s32 y);
struct SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void);
s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *lwk, u8 flg);
s32 sbtlCallPadVibeFadeUpdate(struct SBTLEFF_LISTWORK *lwk);
s32 sbtlCallSetOt(void);
s32 sbtlCallSin2(void);
s32 sbtlCallSin3(struct SBTLDVLTK_LISTWORK *lwk);
s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
f32 sbtlFree(f32 x, f32 z); /* ground height at x, z (mapGetY); HD sBtlGetGroundPos */
s32 sbtlCheckLoadFlg(void);
void sbtlCallSetColor(void);
s32 sbtlCheckPad(void);
s32 sbtlCheckUnit_Exec(void);
s32 sbtlChkBossEventStart_EnemySummonWait(void);
s32 sbtlClearGeneratorWork(void);
struct SBTLEFF_LISTWORK *sbtlCreateEffList(void);
struct SBTLITEM_LISTWORK *sbtlCreateItemList(void);
s32 sbtlEntryItemList(struct SBTLITEM_LISTWORK *lwk);
void sbtlFunc2(void);
s32 sbtlLoadEffectData_Decode(struct SBTLUNIT_WORK *wk);
s32 sbtlMapFadeProc(void);
s32 sbtlMapFadeProc_H(void);
void sbtlMap_Init(void);
s32 sbtlNakamaSetUp(void);
void sbtlPacData_Init(void);
s32 sbtlRefSbtl15(s32 x, s32 y);
s32 sbtlCallBtlSprite(s32 a, s32 b);
s32 sbtlRefSbtl23(void);
s32 sbtlRefSbtl24(struct SBTLUNIT_WORK *wk, s8 flg);
s32 sbtlRefSbtl27(void);
s32 sbtlRefSbtl29(void);
void sbtlRefSbtl4(void);
s32 sbtlRefSbtl45(void);
void sbtlInitTutorial(void);
s32 sbtlRefSbtl47(void);
s32 sbtlRefSbtl62(struct SBTLUNIT_WORK *wk);
s32 sbtlReleaseDelList(struct SBTLEFF_DELLISTWORK *lwk, u8 flg);
s32 sbtlReleaseDvltk_List(struct SBTLDVLTK_LISTWORK *lwk, u8 flg);
s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg);
s32 sbtlSbattle2(s32 flg);
void sbtlSbtlmenuMain(void);
s32 sbtlRefSbtl13(void);
s32 sbtlSearchPacData(s32 major, s32 minor, s32 devilid);
s32 sbtlSearchPacData2(s32 major, s32 minor, s32 devilid);
s32 sbtlSetBit(u32 *bits, s32 no);
s32 sbtlSetDevilSkillID(s32 id, s32 flg);
s32 sbtlSetMotion_sbtlUnit(struct SBTLUNIT_WORK *wk, s32 motion, s32 bframe, s32 aframe, s8 flg);
s32 sbtlSetPacData_MHT(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlSetSummon_Unit(struct SBTLUNIT_WORK *wk, s32 stock, s32 a, s8 b, s32 c, s8 d);
s32 sbtlUpdateVibe(void);

#endif
