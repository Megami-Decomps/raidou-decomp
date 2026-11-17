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

/* HD SBTLCAMERA_WORK, the head of SBTL_WORK; the PS2 layout starts at targetpos. */
typedef struct SBTLCAMERA_WORK {
    sdfVector4_t targetpos;
    sdfVector4_t direction;
    sdfVector4_t camerapos;
    struct dds3Basic_t *cm;
    f32 distance;
    f32 fovy;
    u8 pad3C[4];
} SBTLCAMERA_WORK;

/* HD SBTLVIBE_WORK: one pad's rumble envelope, SBTL_WORK.vibe at 0x3AC. */
typedef struct SBTLVIBE_WORK {
    s32 in;
    s32 out;
    s32 life;
    s32 count;
    s8 st;
    u8 power;
    u8 pad12[2];
} SBTLVIBE_WORK;

/* HD SBTLPNL_WORK; 0x2C on PS2, fields past y not read yet. */
typedef struct SBTLPNL_WORK {
    s16 mode;
    s16 mode1;
    s32 alpha;
    s32 acounter;
    s32 counter;
    s32 x;
    s32 y;
    u8 pad18[0x14];
} SBTLPNL_WORK;

/* HD SBTLPANEL_WORK, 0x530 bytes from sbtlCallMemAllocDirect5; pointed to by SBTL_WORK.panel at 0x628. */
typedef struct SBTL_PANEL {
    struct sdfMemHandle_t *handle;
    struct effList_t *list;
    struct sprHandle_t *resources;
    struct sprHandle_t *hPanelSpr2;
    struct sprHandle_t *hPanelSpr3;
    struct sprHandle_t *hPanelSpr4;
    struct sprHandle_t *hPanelSpr5;
    s16 mode;
    u8 pad1E[2];
    u32 flg;
    s32 alpha;
    s32 acounter;
    s32 ofsx;
    s32 ofsy;
    s32 gstat0;
    s32 gstat1;
    s32 astat0;
    s32 astat1;
    s32 acnt0;
    s32 acnt1;
    SBTLPNL_WORK mds_waku0;
    SBTLPNL_WORK mds_waku1;
    SBTLPNL_WORK mds_light;
    u8 padD0[0x522 - 0xD0];
    s16 meirei_no; /* order given to the summoned demon (HD SBTLPNL_SETDATA meirei_no) */
    u8 pad524[0x530 - 0x524];
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
    SBTLCAMERA_WORK camera;
    u8 pad040[0x70 - 0x40];
    s32 field070; /* cleared by sbtlSbtlCameraproc */
    u8 pad074[0x80 - 0x74];
    sdf3DLight_t light[3]; /* copy of the parallel lights, made by sbtlRefSbtl31 */
    sdfVector4_t ambient;
    s32 mapNo[2];
    u8 pad158[4];
    u32 startFlg; /* 8: map fade running, 0x10: second map fade running */
    s32 zoomlockID;
    s8 zoomlockflg;
    f32 zoom_x;
    f32 zoom_y;
    f32 zoom_z;
    s32 zoom_time;
    f32 zoom_bairitu;
    s32 zoomlocktime;
    u8 pad180[0x198 - 0x180];
    fileRequest_t *panelPack;
    u32 panelPackPos;
    s32 panelPackLen;
    u8 pad1A4[4];
    sdfMemHandle_t *handle;
    dds3ProcessID_t *process;
    s32 syscnt;
    s32 endcnt;
    s32 scrollID;
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
    s32 nego_chrid;
    s32 nego_alpha;
    s16 evtloadst;
    u8 pad1FE[2];
    sdfMemHandle_t *evtloadHandle;
    fileRequest_t *evtloadFile; /* /battle/event/sb_btlevent.bf */
    u8 *evtloadpData;
    s32 evtload_evtno;
    s16 eventno;
    s16 eventcounter;
    s16 eventgstat;
    u8 pad216[2];
    s32 eventListID;
    u8 pad21C[4];
    s16 summonIndex[2];
    s32 field224;
    u8 pad228[0x22C - 0x228];
    s32 useitemID;
    SKLID field230; /* effect skill id checked before the item list is refilled */
    s16 useitem_stock; /* stock slot the item is used on, -1 for none */
    u8 pad236[0x238 - 0x236];
    s32 equipBulletBk;
    u8 pad23C[0x298 - 0x23C];
    s32 tgt_csr; /* target cursor: unit ID under the cursor */
    u8 pad29C[0x2CC - 0x29C];
    struct SBTLUNIT_LISTWORK *list2CC;
    s32 field2D0; /* list2D4 index found by sbtlCallBtlCheckDamageTarget2, -1 for none */
    struct SBTLUNIT_LISTWORK *list2D4;
    u8 pad2D8[4];
    s32 stockMinor[SDAT_STOCK_NUM];
    s32 stockDevilSkill[SDAT_STOCK_NUM];
    u32 summonBit;
    s32 stockNum;
    s32 field344;
    s32 field348;
    u8 field34C[SDAT_STOCK_NUM];
    u8 field358[SDAT_STOCK_NUM];
    s32 stockUnitID[SDAT_STOCK_NUM];
    u8 pad394[0x398 - 0x394];
    s32 stackID[2]; /* stock slots of the summoned demons (HD stack_id) */
    u8 pad3A0[0x3A4 - 0x3A0];
    s32 unitPadID;
    s32 field3A8;
    SBTLVIBE_WORK vibe[2];
    s32 vibe_startf[2];
    s32 vibe_frame[2];
    u8 vibe_power[2];
    u8 vibe_type[2];
    struct SBTLUNIT_LISTWORK *list3E8;
    s32 field3EC;
    u8 pad3F0[4];
    struct SBTLDMG_LISTWORK *animList;
    struct SBTLDMG_HITLISTWORK *hitList;
    u8 pad3FC[4];
    struct SBTLEFF_LISTWORK *effList;
    struct SBTLUNIT_LISTWORK *field404; /* HD friend_List */
    struct SBTLUNIT_LISTWORK *field408; /* HD other_List */
    struct SBTLUNIT_LISTWORK *field40C;
    struct SBTLITEM_LISTWORK *itemList;
    struct SBTLITEM_LISTWORK *bulletList;
    struct SBTLSM_LISTWORK *sysmsgList;
    struct SBTLEFF_DELLISTWORK *effdelList;
    struct SBTLDMG_HITWORK *huka2hwk; /* hit whose attacker data later hits copy (HD name) */
    s32 field424;
    s32 field428;
    s32 hitTargetID;
    s16 mode;
    s16 modebk;
    s16 mode2;
    s16 field436;
    s16 gstat[2];
    s16 walk_counter;
    s16 wait_counter;
    s16 field440;
    s16 field442;
    s16 field444;
    u8 pad446[0x448 - 0x446];
    s32 mag;
    u8 pad44C[4];
    s32 gunskillid;
    u8 pad454[4];
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
    s32 field4B0;
    s8 field4B4;
    u8 pad4B5;
    s16 field4B6;
    s16 field4B8;
    s16 field4BA;
    s16 field4BC;
    s16 field4BE;
    u8 pad4C0[0x4CE - 0x4C0];
    s16 field4CE;
    s16 huumamagnone;
    u8 pad4D2[2];
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
    struct scrProcessWork_s *hDEADScript;
    struct SBTLUNIT_WORK *scrUnit;
    u8 pad65C[0x664 - 0x65C];
    s32 scrZoomFrame;
    s32 scrZoomOutFrame;
    s32 scrCutinID;
    s32 scrMsgID;
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
    s32 scrWarp[3];
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

typedef char SBTL_WORKSizeCheck[sizeof(SBTL_WORK) == 0x87C ? 1 : -1];

extern SBTL_WORK *wk_sbtl;

/* A devil's item drop slot. */
typedef struct SBTLITEM_WORK {
    s16 itemid;
    u8 para;
    s8 per;
} SBTLITEM_WORK;

/* Devil table entry returned by sbtlGetDevilData_sbtlSub. */
typedef struct SBTL_DEVIL_DATA {
    u32 flag;
    u16 level;
    s16 motionId;
    u8 pad08[2];
    s16 field0A;
    s16 aisyoId;
    s8 field0E; /* race, copied to sdatUnitWork_t.raceId */
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
    u8 pad44[0x50 - 0x44];
    SBTLITEM_WORK item[4];
    f32 field60;
    f32 field64;
    s16 field68;                /* read by sbtlBtlLoadHuumaMesDataSub */
    s16 huumaMesNo;
    u8 pad6C[4];
} SBTL_DEVIL_DATA;

typedef char SBTL_DEVIL_DATASizeCheck[sizeof(SBTL_DEVIL_DATA) == 0x70 ? 1 : -1];


/* Battle start parameters, copied to main_data by sbtlSbattle. */
typedef struct SBTL_START_DATA {
    sdfVector4_t pos;
    sdfVector4_t rot;
    f32 val20; /* x offset added to the battle camera positions */
    f32 val24; /* z offset */
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
void sbtlCallBtlGetPlayerData2(void);
void sbtlCallBtlGetPlayerData6(s32 num);
s32 sbtlCallBtlGetMotionVisualData(struct SBTLUNIT_WORK *wk, s32 motionID, s32 bframe, s32 aframe, s8 flg);
f32 sbtlCallBtlGetMotionVisualData4(struct SBTLUNIT_WORK *wk, s32 type);
s32 sdfRemoveTaskItem(struct SBTLUNIT_WORK *wk, s32 flg);
s32 sbtlCallBtlGetSkilltbl8(struct SBTLUNIT_WORK *wk, u32 flg);

/* One generator entry of a stage (HD SB_ENCOUNT1, PS2 layout). */
typedef struct SB_ENCOUNT1 {
    s32 type;       /* 0x00: 2-5 start a generator */
    s16 pos;
    s16 devil;
    s16 groupID;
    s16 disp_max;
    s16 zan_num;    /* 0x0C: copied to SB_GENERATOR_WORK.zan_cnt */
    s16 groupID2;
} SB_ENCOUNT1;

/* One battle stage's row (0x100 bytes) in the table sbtlData100 points at, indexed by SBTL_WORK.stageNo. */
typedef struct SBTL_STAGE_DATA {
    s8 mapNo[2];    /* 0x00: copied to SBTL_WORK.mapNo */
    s8 form;        /* 0x02: mapMapSetForm, 0 for none */
    s8 encPoint;    /* 0x03: mapEncPointGet point */
    u16 flags;      /* 0x04: 0x20 stops units at battle start */
    s16 bgmId;      /* 0x06 */
    u8 pad08[0x28];
    SB_ENCOUNT1 enc1[13]; /* 0x30 */
} SBTL_STAGE_DATA;

extern SBTL_STAGE_DATA *sbtlData100;

/* Status marker over a unit (HD SBTLSTP_WORK), linked through next. R1 layout differs from HD. */
typedef struct SBTL_STP_NODE {
    sdfVector4_t pos;
    sdfMemHandle_t *handle;
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
    struct SBTL_STP_NODE *prev;
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

/* Devil talk window (HD SBTLDVLTK_WORK, with the work's own handle first in R1). Partial layout. */
typedef struct SBTLDVLTK_WORK {
    sdfMemHandle_t *handle;
    s32 listID;
    s32 devilid;
    s32 sklid;
    s32 seikaku;
    s32 counter;
    s32 cmpcounter;
    SBTLDVLTK_MODE mode;
    SBTLDVLTK_ST status;
    s32 resetcounter;
    s32 cmpreset;
    s32 samecnt;
    s32 sincount;
    s32 ofs_x;
    s32 ofs_y;
    s32 alpha;
    u8 pad40[0xE0 - 0x40];
    struct SBTLDVLTK_WORK *prev;
    struct SBTLDVLTK_WORK *next;
} SBTLDVLTK_WORK;

typedef struct SBTLDVLTK_LISTWORK {
    s32 num;
    s32 samecnt;
    s32 resetcounter;
    SBTLDVLTK_WORK *dwk;
} SBTLDVLTK_LISTWORK;

/* Model binding made for an effect, linked in SBTL_WORK.modelBindings. Partial layout. */
typedef struct SBTL_MODELBIND {
    sdfMemHandle_t *handle;
    s32 unitID; /* read by sbtlCallBtlSearchUnitID26 */
    s32 status;
    u32 flg; /* 1: add para to SBTL_WORK.field644, 2: subtract it */
    u8 pad10[4];
    s32 para;
    struct SBTL_MODELBIND *prev;
    struct SBTL_MODELBIND *next;
} SBTL_MODELBIND;

typedef struct SBTL_MODELBIND_LIST {
    u8 pad00[4];
    s32 num;
    SBTL_MODELBIND *top;
} SBTL_MODELBIND_LIST;

/* One motion's timing record (HD MOTIONDATA_WORK, 0x24), returned by sbtlGetMotionVisualData. */
typedef struct SBTL_UNIT_SUB {
    u16 flags;
    u8 pad02[2];
    f32 field04;
    f32 m_stoprate;
    f32 e_startrate;
    f32 field10;
    f32 field14;
    s16 field18;
    u8 pad1A[2];
    f32 cancelrate;
    f32 se_rate; /* s32 in HD */
} SBTL_UNIT_SUB;

SIZE_CHECK(SBTL_UNIT_SUBSizeCheck, SBTL_UNIT_SUB, 0x24);

typedef struct SBTL_MOTION_VISUAL {
    u8 kind;
    u8 pad01[3];
    f32 field04;
    f32 field08;
    f32 field0C;
    f32 field10;
    f32 scale;
    f32 field18;
    f32 field1C;
    f32 field20;
    f32 field24;
    f32 field28;
    u8 field2C;
    u8 pad2D;
    s16 field2E;
    s16 field30;
    s16 field32;
    SBTL_UNIT_SUB mwk[43]; /* HD VISUALDATA_WORK.mwk, inline on the PS2 */
} SBTL_MOTION_VISUAL;

SIZE_CHECK(SBTL_MOTION_VISUALSizeCheck, SBTL_MOTION_VISUAL, 0x640);

/* Per-model visual tables for devils (major nonzero) and for the player (major zero). */
extern SBTL_MOTION_VISUAL *sbtlData98;
extern SBTL_MOTION_VISUAL *sbtlData99;

struct SBTLUNIT_WORK;

extern SBTL_START_DATA main_data;
extern char sbtlData85[];
extern char sbtlStrSoundat3Boss00At3[]; /* "/soundat3/boss00.at3" */
extern char sbtlStrSoundat3Boss01At3[]; /* "/soundat3/boss01.at3" */
extern char sbtlStrSoundat3Boss02At3[]; /* "/soundat3/boss02.at3" */
extern char sbtlStr0204[];              /* "%02d %04x %d" */
extern char sbtlStrEnmy[];              /* "ENMY: %d" */
extern char sbtlENMYData[];             /* "SKL: %d" */
extern s32 sbtlSbattleData;
extern s32 sbtlSceneLoadData;
extern s32 sbtlSceneLoadData2;
extern s32 sbtlSceneLoadData3;
extern s32 sbtlData86;
extern f32 sbtlData87;
extern f32 sbtlData88;
extern f32 sbtlData89;
extern s32 sbtlData90;
extern s32 sbtlData91;
extern s32 loadstep;
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
extern s8 pbData;
extern sdfVector4_t aViewTarget;
extern sdfVector4_t aViewEye;
extern sdfVector4_t aViewDown;
extern sdfVector4_t pbWork;
extern sdfVector4_t pbWork2;
extern sdfVector4_t pbWork3;

s32 DataLoadProc(dds3ProcessID_t *id);
s32 sbtlCallMemset4(s32 fld, s32 area);
s32 sbtlSetZoomLock(struct SBTLUNIT_WORK *unit, f32 zoom);
s32 sbtlCheckZoom(void);
s32 sbtlSetLoadPanelData(void);
s32 sbtlCallCheckEnd(void);
s32 sbtlCallBitCheck(s32 stageNo);
s32 DrawProc(dds3ProcessID_t *id);
s32 RestartBattle(void);
s32 sbtlSbattle4(void);
s32 sbtlCallBtlGetScrollID(void);
void pbSetGameCamera0(void);
s32 sbtlCallBtlDeleteOnData(void);
s32 sbtlCallBtlGetPlayerData(void);
s32 sbtlCallBtlSearchEffectData2(s32 id, u16 flg, void *pData);
s32 sbtlBtlLoadPlayerOnMemDataSub(s32 index, void *pData);
struct SBTLEFF_WORK *sbtlCallMemset17(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid, s32 starttime,
                                      const f32 *defpos, u32 rgba, s8 flg);
void sbtlCallMemset3(s32 battleMode, s32 stageNo);
s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo);
s32 sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk);
s32 sbtlCallCameraObjectBasicGetFovy(SBTLCAMERA_WORK *wk, struct dds3Basic_t *cm);
s32 sbtlCheckScriptData(s32 devilId);
s32 sbtlCheckTutorialBattle(void);
s64 sbtlCallBtlCheckTutorialBattle3(void);
void sbtlGetDevilData(struct SBTLUNIT_WORK *wk, s32 a, s32 b);
SBTL_DEVIL_DATA *sbtlGetDevilData_sbtlSub(s32 id);
s32 sbtlGetExeSkill(SKLID sklid);
s32 sbtlSobedSAB(SKLID id, char *buf);
SBTL_FIELD630_WORK *sbtlCallMemAllocDirect4(void);
s32 sbtlRefSbtl42(SBTL_FIELD630_WORK *wk);
SBTL_MOTION_VISUAL *sbtlGetVisualData(s32 a, s32 id);
u32 *sbtlLeaf10(s32 id, s32 mode);
s32 sbtlLeaf62(struct SBTLUNIT_WORK *wk, s32 mode);
s32 sbtlGetScrollID(void);
struct SBTLUNIT_WORK *sbtlRefSbtl(void);
void sbtlInitDropWork(void);
s32 sbtlLeaf(dds3ProcessID_t *id);
s32 OnDataLoadProc(dds3ProcessID_t *id);
void sbtlLeaf75(dds3ProcessID_t *id);
s32 sbtlLeaf54(dds3ProcessID_t *id);
void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode);
s32 sbtlRefSbtl31(void);
void sbtlResetRndEncCount(void);
void sbtlRndEncTopPriorityMap(s32 *mapOut, s32 *floorOut);
s32 sbtlSbattle(SBTL_START_DATA *start, s8 flag);
s32 sbtlSbattle3(void);
void sbtlCallEffectShutdown3(s32 mode);
sdatUnitWork_t *sbtlGetStockUnitData(s32 id);
s32 sbtlLeaf53(sdatUnitWork_t *unit);
s32 sbtlGetDamage_GB(sdatUnitWork_t *acter, sdatUnitWork_t *unit, SKLID sklid);
s32 sbtlCallBtlGetItemtbl3(sdatUnitWork_t *unit, s32 itemID);
s32 sbtlCallBtlSearchUnitID35(s32 stockID);
s32 sbtlCheckItemMember_Dead(s32 stockID);
s32 sbtlSbattle5(dds3ProcessID_t *id);
s32 sbtlSbattle6(dds3ProcessID_t *id);
void sbtlSbtlCameraproc(void);
s32 sbtlLeaf103(dds3ProcessID_t *id);
s32 sbtlLeaf87(dds3ProcessID_t *id);
s32 sbtlSetScrollID(s32 id);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 side, s32 devilId, s32 arg);
#ifdef VERSION_RAIDOU2
/* The raidou2 symbol of this name takes sbtlGetVisualData's arguments. */
SBTL_MOTION_VISUAL *sbtlGetMotionVisualData(s32 a, s32 id);
SBTL_DEVIL_DATA *sbtlIfDrawActionSub(s32 id);
#else
SBTL_UNIT_SUB *sbtlGetMotionVisualData(s32 a, s32 id, s32 b);
#endif
void sbtlStopVibe(void);
s32 Draw_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk);
s32 Draw_sbtlScene(void);
void Init_sbtlDraw(void);
s32 Proc_sbtl(void);
s32 Proc_sbtlEXP(struct SBTL_MODELBIND_LIST *lwk);
s32 sbtlCallBtlSearchUnitID26(struct SBTL_MODELBIND *wk);
s32 sbtlProcSBtlSTPSub(struct SBTLUNIT_WORK *wk);
s32 Proc_sbtlSTP(SBTL_STP *stp);
s32 Draw_sbtlSTP(SBTL_STP *stp);
s32 btlLoadSbPack(void);
void campFreeStatusModel(void);
struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void);
void effStopEffectStop(void);
SBTL_STP *evtAllocateContext(void);
s32 pbCallBtlCheckLoadFlg(void);
void sbtlAllInit(void);
s32 sbtlBattleSBPACK(void);
s32 sbtlCallStartRain(void);
s32 sbtlBattleEventSb(void);
void sbtlBattleStart(SBTL_STAGE_DATA *stage);
void sbtlSupport_BattleStop(s32 motionFlag, s32 effectFlag, s32 SEFlag);
s32 sbtlBtlEntryStockDataSub(sdatUnitWork_t *unit);
s32 sbtlCheckAutoSkill(sdatUnitWork_t *unit, s32 flg);
s32 sbtlSetAutoSkill_GB(sdatUnitWork_t *unit);
s32 sbtlGetStockPoint(void);
s32 sbtlBtlNakamaSetUpSub(sdatUnitWork_t *unit);
void sbtlCallBtlCheckEnshutuSummonUnit(void);
s32 sbtlCallBtlCheckGeneratorGroupID(void);
void sbtlCallBtlCheckTutorialBattle2(void);
s32 sbtlCallBtlEndAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg);
s32 sbtlSetMotionHit(struct SBTLUNIT_WORK *wk, s32 motionno, SKLID skillid);
void sbtlCallBtlGetItemtbl(void);
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
s32 Proc_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk);
s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlRefSbtl38(void);
f32 sbtlFree(f32 x, f32 z); /* ground height at x, z (mapGetY); HD sBtlGetGroundPos */
s32 sbtlCheckLoadFlg(void);
s32 sbtlCallSetColor(void);
s32 sbtlCallSetColor2(u32 color);
s32 sbtlCallSetColor3(u32 color);
s32 sbtlCheckPad(void);
s32 sbtlCheckUnit_Exec(void);
s32 sbtlChkBossEventStart_EnemySummonWait(void);
s32 sbtlClearGeneratorWork(void);
struct SBTLEFF_LISTWORK *sbtlCreateEffList(void);
struct SBTLITEM_LISTWORK *sbtlCreateItemList(void);
s32 sbtlEntryItemList(struct SBTLITEM_LISTWORK *lwk);
#ifdef VERSION_RAIDOU2
s32 sbtlFunc2(struct SBTLUNIT_WORK *wk, s32 type);
#else
void sbtlFunc2(void);
#endif
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
s32 sbtlRefSbtl36(void);
u32 sbtlRefSbtl37(void);
s32 sbtlCutMotion(struct SBTLUNIT_WORK *wk, f32 frame);
s32 sbtlCallBtlSearchUnitID9(struct SBTLUNIT_WORK *wk);
s32 sbtlCallMemset9(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *target, f32 frame);
extern s32 (*sbtlCallBtlSearchUnitID8Data[])(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlCallMemset7(s32 a, s32 motionId, s32 stock, s8 flg);
s32 sbtlCallBtlGetDevilDataSBtlSub5(sdatUnitWork_t *summoner, sdatUnitWork_t *unit, s32 devilId);
void FakeRndHit(struct SBTLUNIT_WORK *wk, s32 pow);
s32 sbtlCallBtlSearchUnitID12(void);
void sbtlBeginSetsumaSub(struct SBTLUNIT_WORK *wk);
s32 sbtlCallHirukoYukaGoIdx2B(u32 color);
void *sbtlCallGetProcessData43(void);
void sbtlSummonMenuFuncOpen00Sub(u32 no);
s32 sbtlRefSbtl27(void);
s32 sbtlRefSbtl29(void);
void sbtlRefSbtl4(void);
void sbtlRefSbtl45(void);
void sbtlInitTutorial(void);
s32 sbtlRefSbtl47(void);
s32 sbtlRefSbtl62(struct SBTLUNIT_WORK *wk);
void sbtlBtlSetDamageSub(struct SBTLUNIT_WORK *wk);
void sbtlBtlGetDamageGBSub(sdatUnitWork_t *unit, sdatParaWork_t *para);
s32 sbtlReleaseDelList(struct SBTLEFF_DELLISTWORK *lwk, u8 flg);
s32 sbtlReleaseDvltk_List(struct SBTLDVLTK_LISTWORK *lwk, u8 flg);
s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg);
s32 sbtlSbattle2(s32 flg);
void sbtlSbtlmenuMain(void);
s32 sbtlRefSbtl13(void);
s32 sbtlSearchPacData(s32 major, s32 minor, s32 devilid);
s32 sbtlSearchPacData2(s32 major, s32 minor, s32 devilid);
void sbtlSetBit(u32 *bits, s32 no);
s32 sbtlSetDevilSkillID(s32 id, s32 flg);
s32 sbtlSetMotion_sbtlUnit(struct SBTLUNIT_WORK *wk, s32 motion, s32 bframe, s32 aframe, s8 flg);
s32 sbtlSetPacData_MHT(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlSetSummon_Unit(struct SBTLUNIT_WORK *wk, s32 stock, s32 flg, s8 usePos, const sdfVector4_t *pos, s8 mode);
void sbtlUpdateVibe(void);

s32 sbtlInitPlayerData(void);

extern SBTL_DEVIL_DATA *gohData7;
s32 sbtlCheckDrink(void);
u16 GetPercentage(void);

#endif
