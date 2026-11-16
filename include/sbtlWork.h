#ifndef SBTLWORK_H
#define SBTLWORK_H

#include "common.h"
#include "fileManager.h"
#include "kernel.h"
#include "sdf_types.h"
#include "sdatUnit.h"

struct EffectSlotSet;

/* Pointed to by SBTL_WORK.panel at 0x628. */
typedef struct SBTL_PANEL {
    u8 pad00[8];
    struct EffectSlotSet *resources;
    u8 pad0C[0x14];
    u32 flg;
} SBTL_PANEL;

struct SBTLDMG_HITLISTWORK;
struct SBTLDVLTK_LISTWORK;
struct SBTLEFF_DELLISTWORK;
struct SBTLEFF_LISTWORK;
struct SBTLEXP_LISTWORK;
struct SBTLITEM_LISTWORK;
struct SBTLSM_LISTWORK;
struct SBTLUNIT_LISTWORK;
struct SBTL_MODELBIND_LIST;
struct SBTL_STP;

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
    u8 pad1B8[0xC];
    u32 flg;
    u32 flg2;
    u8 pad1CC[4];
    s32 targetUnitID;
    u8 pad1D4[4];
    s32 stageNo;
    struct SBTLUNIT_LISTWORK *unitList;
    u8 pad1E0[4];
    s16 battleMode;
    u8 pad1E6[0x220 - 0x1E6];
    s16 summonIndex[2];
    u8 pad224[0x2CC - 0x224];
    s32 field2CC;
    u8 pad2D0[4];
    struct SBTLUNIT_LISTWORK *list2D4;
    u8 pad2D8[0x30C - 0x2D8];
    s32 stockDevilSkill[SDAT_STOCK_NUM];
    u32 summonBit;
    s32 stockNum;
    s32 field344;
    u8 pad348[0x364 - 0x348];
    s32 stockUnitID[SDAT_STOCK_NUM];
    u8 pad394[0x3E8 - 0x394];
    struct SBTLUNIT_LISTWORK *list3E8;
    u8 pad3EC[8];
    s32 animList;
    struct SBTLDMG_HITLISTWORK *hitList;
    u8 pad3FC[4];
    struct SBTLEFF_LISTWORK *effList;
    s32 field404;
    s32 field408;
    s32 field40C;
    struct SBTLITEM_LISTWORK *itemList;
    struct SBTLITEM_LISTWORK *bulletList;
    struct SBTLSM_LISTWORK *sysmsgList;
    struct SBTLEFF_DELLISTWORK *effdelList;
    u8 pad420[4];
    s32 field424;
    u8 pad428[8];
    s16 mode;
    u8 pad432[0x5E0 - 0x432];
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
    u8 pad60A[0x628 - 0x60A];
    SBTL_PANEL *panel;
    u8 pad62C[4];
    s32 *field630;
    struct SBTLDVLTK_LISTWORK *dvltkList;
    struct SBTLEXP_LISTWORK *expList;
    struct SBTL_MODELBIND_LIST *modelBindings;
    u8 pad640[8];
    struct SBTL_STP *stp;
    u8 pad64C[0x6C4 - 0x64C];
    s32 scrCnt[4];
    u8 pad6D4[4];
    u32 scrFlag;
    u8 pad6DC[4];
    s32 enshutuCnt[8];
    u8 pad700[0x87C - 0x700];
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
    u8 pad00[0x28];
    s32 val28;
    s32 mode;
    s16 val30;
    u8 pad32[2];
} SBTL_START_DATA;

/* One loaded SB_PACK resource, indexed by the pack entry id. */
typedef struct SBTL_PACK_SLOT {
    void *data;
    struct sdfMemHandle_t *handle;
    void *unk8;
    void *unkC;
} SBTL_PACK_SLOT;

SBTL_PLAYER_DATA *sbtlGetPlayerData(void);

/* One battle stage's row (0x100 bytes) in the table sbtlData100 points at, indexed by SBTL_WORK.stageNo. */
typedef struct SBTL_STAGE_DATA {
    s8 mapNo[2];    /* 0x00: copied to SBTL_WORK.mapNo */
    s8 form;        /* 0x02: mapMapSetForm, 0 for none */
    s8 encPoint;    /* 0x03: mapEncPointGet point */
    u8 pad04[2];
    s16 bgmId;      /* 0x06 */
    u8 pad08[0xF8];
} SBTL_STAGE_DATA;

extern SBTL_STAGE_DATA *sbtlData100;

/* Floating marker node, linked through next. The position is read as a VU0 quad. */
typedef struct SBTL_STP_NODE {
    sdfVector4_t pos;
    s32 state;
    u8 pad14[8];
    s16 slotIndex;
    u8 pad1E[10];
    u32 alpha;
    u8 pad2C[4];
    f32 yOffset;
    u8 pad34[0x10];
    struct SBTL_STP_NODE *next;
} SBTL_STP_NODE;

typedef struct SBTL_STP {
    s32 enabled;
    SBTL_STP_NODE *head;
} SBTL_STP;

typedef struct SBTL_MOTION_VISUAL {
    u8 kind;
    u8 pad01[3];
    f32 field04;
    f32 field08;
    u8 padC[4];
    f32 field10;
    f32 scale;
    u8 pad18[0x30 - 0x18];
    s16 field30;
    s16 field32;
} SBTL_MOTION_VISUAL;

typedef struct SBTL_UNIT_SUB {
    u8 pad00[4];
    f32 field04;
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

/* One attack hit record copied from a hit file. */
typedef struct SBTL_HIT_PACENTRY {
    s32 word[37];
} SBTL_HIT_PACENTRY;

typedef struct SBTL_HIT_PACFILE {
    s32 num;
    SBTL_HIT_PACENTRY entry[1];
} SBTL_HIT_PACFILE;

/* 200-byte allocation holding one hit record and the handles it owns. */
typedef struct SBTL_HIT_PACDATA {
    u8 pad00[4];
    sdfMemHandle_t *handle;
    sdfMemHandle_t *handle08;
    u8 pad0C[4];
    sdfMemHandle_t *handle10;
    u8 pad14[0xC];
    SBTL_HIT_PACENTRY entry;
    u8 padB4[0x14];
} SBTL_HIT_PACDATA;

typedef struct SBTL_PACDATA_HITLIST {
    u8 pad00[0xC];
    s32 num;
    s16 gstat;
    u16 status;
    SBTL_HIT_PACDATA *pwk[15];
} SBTL_PACDATA_HITLIST;

typedef struct SBTL_PACDATA_KEY {
    u8 pad00[4];
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
    u8 pad0000[0x1154];
    sdfMemHandle_t *handle;
    u8 pad1158[4];
} SBTL_WORK2;

extern SBTL_WORK2 *sbtlData83;
extern SBTL_PACDATA_WORK *sbtlData84;
extern u8 VRF_DeltaTime;
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
void pbSetGameCamera0(void);
void sbtlCallBtlDeleteOnData(void);
s32 sbtlCallBtlGetPlayerData(void);
s32 sbtlCallBtlSearchEffectData2(s32 id, s32 a2, s32 a3);
void sbtlCallMemset17(struct SBTLEFF_LISTWORK *effList, struct SBTLUNIT_WORK *wk, s32 type, s32 a, f32 *pos, u32 color, s32 b);
void sbtlCallMemset3(s32 battleMode, s32 stageNo);
s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo);
void sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckScriptData(s32 devilId);
s32 sbtlCheckTutorialBattle(void);
void sbtlGetDevilData(struct SBTLUNIT_WORK *wk, s32 a, s32 b);
SBTL_DEVIL_DATA *sbtlGetDevilData_sbtlSub(s32 id);
s32 sbtlGetExeSkill(void);
SBTL_MOTION_VISUAL *sbtlGetMotionVisualData(s32 a, s32 id);
s32 sbtlGetScrollID(void);
void sbtlInitDropWork(void);
s32 sbtlLeaf(dds3ProcessID_t *id);
s32 sbtlLeaf54(dds3ProcessID_t *id);
void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode);
void sbtlRefSbtl31(void);
void sbtlResetRndEncCount(void);
void sbtlRndEncTopPriorityMap(s32 *mapOut, s32 *floorOut);
s32 sbtlSbattle5(dds3ProcessID_t *id);
s32 sbtlSbattle6(dds3ProcessID_t *id);
void sbtlSbtlCameraproc(void);
void sbtlSetScrollID(s32 id);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 side, s32 devilId, s32 arg);
SBTL_UNIT_SUB *sbtlSetUnitSub(s32 a, s32 id, s32 b);
void sbtlStopVibe(void);
s32 Draw_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk);
s32 Draw_sbtlScene(void);
s32 Init_sbtlDraw(void);
struct SBTLEXP_LISTWORK *InitTarSelProcessData(void);
s32 Proc_sbtl(void);
s32 Proc_sbtlEXP(struct SBTL_MODELBIND_LIST *lwk);
s32 Proc_sbtlSTP(SBTL_STP *stp);
s32 Draw_sbtlSTP(SBTL_STP *stp);
s32 btlLoadSbPack(void);
void campFreeStatusModel(void);
struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void);
s32 effStopEffectStop(void);
SBTL_STP *evtAllocateContext(void);
s32 pbCallBtlCheckLoadFlg(void);
s32 sbtlAllInit(void);
s32 sbtlBattleSBPACK(void);
s32 sbtlBattleStart(SBTL_STAGE_DATA *stage);
s32 sbtlBtlEntryStockDataSub(sdatUnitWork_t *unit);
s32 sbtlBtlNakamaSetUpSub(sdatUnitWork_t *unit);
void sbtlCallBtlCheckEnshutuSummonUnit(void);
s32 sbtlCallBtlCheckGeneratorGroupID(void);
void sbtlCallBtlCheckTutorialBattle2(void);
s32 sbtlCallBtlEndAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg);
s32 sbtlCallBtlGetItemtbl(void);
s32 sbtlCallBtlGetNumUnitGroupID(void);
s32 sbtlCallBtlMapInit(void);
s32 sbtlCallBtlSearchEffectSABActer(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchPacData(s32 major, s32 minor, s32 devilid, SBTL_HIT_PACFILE *file);
s32 sbtlCallBtlSearchPacData2(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlCallBtlSearchUnitID32(void);
s32 sbtlCallBtlSearchUnitID33(void);
s32 sbtlCallBtlSearchUnitID34(sdatUnitWork_t *unit, s32 devilId, s8 flg);
s32 sbtlCallBtlSearchUnitID7(s32 x, s32 y);
struct SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void);
s32 sbtlCallDeleteMessage(struct SBTLSM_LISTWORK *lwk);
s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *lwk, u8 flg);
s32 sbtlCallPadVibeFadeUpdate(struct SBTLEFF_LISTWORK *lwk);
s32 sbtlCallSetOt(void);
s32 sbtlCallSetVarSBtlSM(struct SBTLSM_LISTWORK *lwk);
s32 sbtlCallSin2(void);
s32 sbtlCallSin3(struct SBTLDVLTK_LISTWORK *lwk);
s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckLoadFlg(void);
s32 sbtlCheckPad(void);
s32 sbtlCheckUnit_Exec(void);
s32 sbtlChkBossEventStart_EnemySummonWait(void);
s32 sbtlClearGeneratorWork(void);
struct SBTLEFF_LISTWORK *sbtlCreateEffList(void);
struct SBTLITEM_LISTWORK *sbtlCreateItemList(void);
s32 sbtlCreateSM(struct SBTLSM_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 no, s32 flg);
struct SBTLSM_LISTWORK *sbtlCreateSMList(void);
s32 sbtlEntryItemList(struct SBTLITEM_LISTWORK *lwk);
void sbtlFunc2(void);
s32 sbtlLoadEffectData_Decode(struct SBTLUNIT_WORK *wk);
s32 sbtlMapFadeProc(void);
s32 sbtlMapFadeProc_H(void);
void sbtlMap_Init(void);
s32 sbtlNakamaSetUp(void);
void sbtlPacData_Init(void);
s32 sbtlRefSbtl15(s32 x, s32 y);
s32 sbtlRefSbtl23(void);
s32 sbtlRefSbtl24(struct SBTLUNIT_WORK *wk, s8 flg);
s32 sbtlRefSbtl27(void);
s32 sbtlRefSbtl29(void);
s32 sbtlRefSbtl4(void);
s32 sbtlRefSbtl45(void);
void sbtlInitTutorial(void);
s32 sbtlRefSbtl47(void);
s32 sbtlRefSbtl60(struct SBTLEXP_LISTWORK *lwk);
s32 sbtlRefSbtl62(struct SBTLUNIT_WORK *wk);
s32 sbtlRefSbtl7(void);
s32 sbtlReleaseDelList(struct SBTLEFF_DELLISTWORK *lwk, u8 flg);
s32 sbtlReleaseDvltk_List(struct SBTLDVLTK_LISTWORK *lwk, u8 flg);
s32 sbtlReleaseEXP_List(struct SBTLEXP_LISTWORK *lwk, u8 flg);
s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg);
s32 sbtlSbattle2(s32 flg);
s32 sbtlSbtlmenuMain(void);
s32 sbtlSearchPacData(s32 major, s32 minor, s32 devilid);
s32 sbtlSearchPacData2(s32 major, s32 minor, s32 devilid);
s32 sbtlSetBit(u32 *bits, s32 no);
s32 sbtlSetDevilSkillID(s32 id, s32 flg);
s32 sbtlSetMotion_sbtlUnit(struct SBTLUNIT_WORK *wk, s32 motion, s16 a, s16 b, s8 c);
s32 sbtlSetPacData_MHT(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlSetSummon_Unit(struct SBTLUNIT_WORK *wk, s32 stock, s32 a, s8 b, s32 c, s8 d);
s32 sbtlUpdateVibe(void);

#endif
