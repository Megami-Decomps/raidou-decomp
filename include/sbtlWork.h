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

#define DROPITEM_NUM 12
#define DROPEITEM_NUM 4

/* Battle reward work, cleared by sbtlInitDropWork. An R1 drop slot is id, num, per; R2 adds a fourth word. */
typedef struct SBTLDROP_WORK {
    s32 setflg;
    s32 exp;
    s32 nakama_exp;
    s32 mag;
    s32 chuusei;
    s32 tlev;
    s32 get_chuusei;
    s32 num;
    s32 gold;
#ifdef VERSION_RAIDOU2
    s32 item[DROPITEM_NUM][4];
#else
    s32 item[DROPITEM_NUM][3];
#endif
    s32 eventitem[DROPEITEM_NUM];
} SBTLDROP_WORK;

/* HD SBTLCAMERA_WORK, the head of SBTL_WORK; the PS2 layout starts at targetpos. */
typedef struct SBTLCAMERA_WORK {
    sdfVector4_t targetpos;
    sdfVector4_t direction;
    sdfVector4_t camerapos;
#ifdef VERSION_RAIDOU2
    u8 pad030[0x70];
#endif
    struct dds3Basic_t *cm;
    f32 distance;
    f32 fovy;
#ifdef VERSION_RAIDOU2
    u8 padAC[4];
#else
    u8 pad3C[4];
#endif
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

/* HD SBTLPNL_SETDATA: the order and target chosen for a summoned demon. */
typedef struct SBTLPNL_SETDATA {
    s8 flg;
    s16 target_p;
    s16 select_no; /* SBTLPNL_MENU */
    s16 meirei_no; /* SBTLPNL_MENU */
    SKLID skillid;
    s32 itemid;
    s32 targetid;
} SBTLPNL_SETDATA;
SIZE_CHECK(SBTLPNL_SETDATASizeCheck, SBTLPNL_SETDATA, 0x14);

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
#ifdef VERSION_RAIDOU2
    u8 padD0[0x51C - 0xD0];
    SBTLPNL_SETDATA setdata[2]; /* one per summoned demon, indexed by GetMeireiNo */
#else
    u8 padD0[0x522 - 0xD0];
    s16 meirei_no; /* order given to the summoned demon (HD SBTLPNL_SETDATA meirei_no) */
    u8 pad524[0x530 - 0x524];
#endif
} SBTL_PANEL;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(SBTL_PANELSizeCheck, SBTL_PANEL, 0x544);
#else
SIZE_CHECK(SBTL_PANELSizeCheck, SBTL_PANEL, 0x530);
#endif

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

#ifdef VERSION_RAIDOU2
/* Hit effect slot (0x10 bytes) in SBTL_WORK's hitEffList. */
typedef struct SBTL_HITEFF {
    s8 bUse;
    s32 effID;
    struct SBTL_HITEFF *prev;
    struct SBTL_HITEFF *next;
} SBTL_HITEFF;

typedef struct HITEFFLIST {
    s32 lstIdx;
    s32 cnt;
    SBTL_HITEFF aHitEff[16];
    SBTL_HITEFF *pHitEffBgn;
    SBTL_HITEFF *pHitEffEnd;
} HITEFFLIST;

/* Battle text entry (0x80 bytes) in SBTL_WORK's textPool; type 0 is free. */
typedef struct sbtlSummonText_t {
    sdfVector4_t pos;
    sdfVector4_t field10;
    sdfVector4_t field20;
    s32 type;
    f32 field34;
    f32 field38;
    f32 field3C;
    f32 field40;
    s32 field44;
    s32 field48;
    s32 count;
    struct sprHandle_t *hSpr;
    s32 sprNo;
    s32 field58;
    s32 field5C;
    s32 field60;
    f32 field64;
    s32 unitID;
    struct sbtlSummonText_t *prev;
    struct sbtlSummonText_t *next;
    s32 field74;
    u8 pad78[0x8];
} sbtlSummonText_t;

/* HD SUMMON_CTRL: a pending summon (io set) or return of a demon, run by SmnCtrlRun. */
typedef struct SUMMON_CTRL {
    s32 bNoOk;
    s32 stkId;
    s32 aprId;
    s32 io;
} SUMMON_CTRL;

/* Partial raidou2 layout of the battle main work: only the fields read so far are named, the rest is padding. */
typedef struct SBTL_WORK {
    sdfVector4_t vPosBossEvent; /* HD name; boss event camera target */
    u8 pad010[0x150 - 0x10];
    sdfVector4_t field150; /* position of the first gun target, set by sbtlCallBtlGetPChara2 */
    u8 pad160[0x180 - 0x160];
    SBTLCAMERA_WORK camera;
    SBTLCAMERA_WORK camerabk;
    u8 pad2E0[0x400 - 0x2E0];
    fileRequest_t *panelPack;
    u32 panelPackPos;
    s32 panelPackLen;
    u8 pad40C[0x420 - 0x40C];
    s16 effcnt; /* sbtlGetStartTime_Damage cycles it through 0..7 */
    u8 pad422[0x424 - 0x422];
    sdfMemHandle_t *handle;
    dds3ProcessID_t *process;
    s32 syscnt;
    s32 field430; /* sbtlMainDraw fades the moon out when set */
    u8 pad434[0x438 - 0x434];
    s32 equipbullet; /* HD name */
    s32 flg;
    u32 flg2;
    u32 status3;
    u8 pad448[0x4];
    u32 status4; /* HD name; 0x10 hides the summon foot markers, 0x200 forces the ND summon effect */
    u8 pad450[0x4];
    s32 targetUnitID;
    u16 hitMdlNo; /* next mdl resource number for hit models (type 7) */
    s16 battleMode;
    s32 stageNo;
    struct SBTLUNIT_LISTWORK *unitList;
    u8 pad464[0x18];
    s16 evtloadst;
    u8 pad47E[0x2];
    sdfMemHandle_t *evtloadHandle;
    fileRequest_t *evtloadFile; /* /battle/event/sb_btlevent.bf */
    u8 *evtloadpData;
    s32 evtload_evtno;
    s16 eventno;
    s16 eventcounter;
    s16 eventgstat; /* battle talk message step */
    u8 pad496[2];
    s32 eventListID;
    dds3ProcessID_t *eventprocID; /* script process of the running battle event */
    s16 summonIndex[2];
    s16 returnindex[2]; /* HD name; summonIndex slot of the demon returning to its tube */
    s32 summonindex_menu[2]; /* HD name; summonIndex as the command menu last showed it */
    u8 pad4B0[0x518 - 0x4B0];
    SKLID stock_useskill[10];
    u16 stock_useskill_num;
    u8 pad542[0x544 - 0x542];
    s32 tgt_csr;
    u8 pad548[0x598 - 0x548];
    struct SBTLUNIT_LISTWORK *tgtList;
    u8 pad59C[0x5A0 - 0x59C];
    struct SBTLUNIT_LISTWORK *list2D4;
    u8 pad5A4[0x5A8 - 0x5A4];
    s32 stockMinor[20];
    s32 stockDevilSkill[20];
    u32 stock_mask; /* HD name */
    s32 field64C; /* stock devil count, the bound of sbtlRefDDS3GBWK2 */
    s32 member_num; /* HD name; partners in the battle */
    s32 summonnum;
    u8 summon[20];
    u8 summonflg[20]; /* HD name; 2 while the stock slot's demon is out */
    s32 stockUnitID[20];
    u8 pad6D0[0x6EC - 0x6D0];
    SBTLVIBE_WORK vibe[2];
    s32 vibe_startf[2];
    s32 vibe_frame[2];
    u8 vibe_power[2];
    u8 vibe_type[2];
    struct SBTLUNIT_LISTWORK *list3E8;
    u8 pad72C[0x734 - 0x72C];
    struct SBTLDMG_LISTWORK *animList;
    struct SBTLDMG_HITLISTWORK *hitList;
    u8 pad73C[0x4];
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
    u8 pad768[0x8];
    s16 mode;
    u8 pad772[0x4];
    s16 field436;
    s16 gstat[2];
    s16 walk_counter;
    u8 pad77E[0x2];
    s16 field440;
    u8 pad782[0x6];
    s32 mag;
    s32 magMax;
    u8 pad790[0x79C - 0x790];
    s32 hgunNum;
    u8 pad7A0[0x7C8 - 0x7A0];
    s32 field7C8;
    s32 field7CC;
    s32 field7D0;
    s32 field7D4;
    s32 field7D8;
    s32 field7DC;
    u8 pad7E0[0x7E8 - 0x7E0];
    s32 field7E8;
    s32 field7EC;
    s32 field7F0;
    s32 field7F4;
    u8 pad7F8[0x7FC - 0x7F8];
    s32 field7FC;
    s32 field800;
    u8 pad804[0x8];
    s32 field80C;
    s32 field810;
    u8 pad814[0x832 - 0x814];
    s16 field4CE;
    s16 huumamagnone;
    u8 pad836[2];
    s32 huumaTargetID; /* unit ID of the battle talk partner */
    u8 pad83C[0x86C - 0x83C];
    s16 field508;
    s16 field50A;
    s16 escapegauge_w; /* HD name */
    u8 pad872[0x2];
    f32 escapegauge_counter; /* HD name */
    u8 pad878[0x880 - 0x878];
    SBTLDROP_WORK drop;
    s16 mf_mode;
    s16 mf_counter;
    s16 mf_cmpcounter;
    u8 pad97A[2];
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
    u8 pad99E[0x9A8 - 0x99E];
    s32 field614;
    u8 field618;
    u8 pad9AD[0x9B4 - 0x9AD];
    s32 field620;
    u8 pad9B8[0x4];
    SBTL_PANEL *panel;
    u8 pad9C0[0x4];
    struct SBTL_FIELD630_WORK *field630;
    struct SBTLDVLTK_LISTWORK *dvltkList;
    struct SBTLEXP_LISTWORK *expList;
    struct SBTL_MODELBIND_LIST *modelBindings;
    u8 pad9D4[0x4];
    s32 field9D8; /* Proc_sbtlEXP adds a bind's para, clamped to 0..999999 */
    struct SBTL_STP *stp;
    s8 field9E0; /* sbtlCallBtlGetBadStatus2 counts it up to 40 */
    u8 pad9E1[0x3];
    struct scrProcessWork_s *hAIScript;
    u8 pad9E8[0x4];
    struct SBTLUNIT_WORK *scrUnit;
    u8 pad9F0[0xA08 - 0x9F0];
    s32 scrStep;
    s32 field678;
    s32 scrUnitNum;
    struct SBTLUNIT_WORK *scrUnitList[16];
    s32 scrCmpCnt;
    s32 scrCnt[4];
    s32 scrEncountNo;
    s32 scrFlag;
    s8 scr_flg;
    s32 enshutuCnt[8];
    s32 enshutuCmpCnt[8];
    s32 scrData[8];
    s32 scrWarp[4];
    s32 EscOdds;
    s32 support_ListID;
    s32 supportindex;
    s32 hitst_sttime;
    s32 hitst_stcnt;
    s32 hitst_cnt;
    s32 hitst_cmpcnt;
    u8 padB00[0xB5C - 0xB00];
    p2sdLoadEntry_t midiLoad[30]; /* copy of the sound driver's MIDI table; 11 to 28 are skill MIDIs */
    s32 midiSkillCount;
    s32 midiCount8;
    u8 field878;
    u8 field879;
    u8 field87A;
    u8 padC57[0xC74 - 0xC57];
    s32 fieldC74; /* field1A0 of the first gun target */
    u8 padC78[0xC7B - 0xC78];
    s8 bBfrBtlEnd; /* set when the battle menu ends with type 1 */
    u8 padC7C[0xC84 - 0xC7C];
    u8 fieldC84; /* hit stops sbtlProcHitStop still has to start */
    u8 fieldC85; /* frames until the next of them */
    u8 padC86[0xC8C - 0xC86];
    struct SBTLUNIT_WORK *pNakama0; /* HD name */
    struct SBTLUNIT_WORK *pNakama1;
    u8 pad_C94[0xCA2 - 0xC94];
    s8 fieldCA2; /* sbtlMainDraw leaves the moon alone when set */
    u8 padCA3[0xCAC - 0xCA3];
    s32 fieldCAC; /* skill SetFirstAtk_Core uses for the gun when set */
    u8 padCB0[0x1];
    s8 fieldCB1; /* sbtlGetNakama index for fieldCAC */
    u8 padCB2[0xD90 - 0xCB2];
    SUMMON_CTRL smnCtrl;
    u8 padDA0[0xDA4 - 0xDA0];
    struct dds3Basic_t *apMarkerUnit[8];
    struct dds3Basic_t *apMarkerUnitR[8];
    u8 padDE4[0xE2C - 0xDE4];
    s32 fieldE2C; /* sbtlCallSetOt sets 10 when a text adds magnetite */
    u8 padE30[0xE40 - 0xE30];
    s32 fieldE40; /* sbtlCallSetOt sets 100 when a kind 0 text ends */
    u8 padE44[0xE81 - 0xE44];
    s8 bEffNoDraw; /* HD name */
    u8 padE82[0xE84 - 0xE82];
    s32 fieldE84; /* sbtlDat's index into datData20 */
    u8 padE88[0x4];
    s8 fieldE8C; /* nonzero: Start_sbtlTime starts no time limit */
    u8 padE8D;
    s8 hitStop;
    u8 padE8F[0xE94 - 0xE8F];
    s8 effNoStop; /* nonzero: effects keep running while the scene is stopped */
    u8 padE95[0xE98 - 0xE95];
    struct mdlObj_t *hMapMdl;
    struct mdlObj_t *hMapMdl2;
    u32 mapclr;
    u8 padEA4[0xEB0 - 0xEA4];
    s32 bltNumDrwCnt; /* HD name; frames left to draw the bullet count over the player */
    u8 padEB4[0xEC0 - 0xEB4];
    s32 fieldEC0;
    u8 padEC4[0xED8 - 0xEC4];
    s8 fieldED8; /* slCallSetFirstAtkCore input flags */
    s8 fieldED9;
    s8 fieldEDA;
    s8 fieldEDB;
    s8 fieldEDC;
    s8 fieldEDD;
    u8 padEDE[0xEE0 - 0xEDE];
    struct sbtlSummonText_t textPool[100];
    struct sbtlSummonText_t *textList;
    struct sbtlSummonText_t *textCursor; /* sbtlRefSbtl30 searches for a free entry from here */
    HITEFFLIST hitEffList;
    u8 pad41F8[0x41FC - 0x41F8];
    s32 field41FC; /* bar alpha sbtlBarCtrl steps toward 126 or 40 */
    s32 field4200; /* bar length sbtlBarCtrl steps toward 350 or 200 */
    u8 pad4204[0x420C - 0x4204];
    s32 field420C; /* sbtlSummonKey refuses the summon menu when set */
    u8 pad4210[0x4264 - 0x4210];
    s8 field4264; /* set: sbtlCreateSTP_Unit creates nothing */
    u8 pad4265[0x4288 - 0x4265];
    s8 field4288; /* nonzero: sbtlReset_SupportAction_Shugou does nothing and sbtlCallBtlGetBadStatus2 returns 0 */
    u8 pad4289[0x42A8 - 0x4289];
    s16 field42A8;
    u8 pad42AA[0x2];
    s16 field42AC;
    s16 field42AE;
    u8 pad42B0[0x42BC - 0x42B0];
    s32 field42BC;
    s16 field42C0;
    u8 pad42C2[0x2];
    struct SBTLUNIT_WORK *field42C4; /* enemy cmpCallBtlCreateTargetList found for a party talk */
    u8 pad42C8[0x42CA - 0x42C8];
    s8 field42CA; /* Proc skips the pause checks when nonzero */
    s8 field42CB;
    s8 field42CC;
    s8 timeState; /* time limit battle: 1 counting, 2 result */
    s8 field42CE; /* nonzero: DrawTime shows the clock */
    u8 pad42CF;
    s32 timeLimit;
    s32 timeMode;
    u8 pad42D8[0x42DD - 0x42D8];
    s8 field42DD;
    u8 pad42DE[0x4308 - 0x42DE];
    u8 field4308; /* Draw_sbtlUnit counts the units whose name it draws */
    s8 geirinstep;
    s16 geirincnt;
    s16 field430C;
    u8 pad430E[0x4310 - 0x430E];
    s32 boss_eventID; /* HD name; unit ID the boss event camera follows */
    s32 boss_devilID; /* HD name; sbtlCheck skips start events for this devil */
    s16 boss_timeno; /* HD name; time event row Result_sbtlTime found */
    s8 bNoEndCam; /* HD name */
    s8 field431B; /* set: sbtlEnemyData draws no enemy name plates */
    s8 field431C; /* 1: sbtlSENSEIDMGCHK always starts the talk */
    u8 pad431D[0x4324 - 0x431D];
    s32 raido_magwaza_costnot; /* HD name; sbtlResetAction clears it */
    s32 field4328; /* Init_sbtl: set when the caller gave no stage, so the stage and sword come from Exit_sbtl */
    s32 field432C;
    s32 field4330; /* frames while 0x80000 of flg2 is clear and the scene runs */
    s32 field4334;
    u8 pad4338[0x4344 - 0x4338];
    s32 field4344; /* sbtlSummonKey counts it down; the summon menu waits for 0 */
    s32 field4348; /* counted down to 0 by sbtlSbattle3 */
    s32 field434C;
    u8 pad4350[0x4];
    s32 field4354;
    u8 pad4358[0x4360 - 0x4358]; /* Init_sbtl clears 0x4360 bytes */
} SBTL_WORK;
SIZE_CHECK(SBTL_WORKSizeCheck, SBTL_WORK, 0x4360);
#else
/* Battle main work (0x87C bytes), pointed to by wk_sbtl. Only fields read so far are named. */
typedef struct SBTL_WORK {
    SBTLCAMERA_WORK camera;
    SBTLCAMERA_WORK camerabk;
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
    dds3ProcessID_t *eventprocID; /* script process of the running battle event */
    s16 summonIndex[2];
    s32 field224;
    u8 pad228[0x22C - 0x228];
    s32 useitemID;
    SKLID field230; /* effect skill id checked before the item list is refilled */
    s16 useitem_stock; /* stock slot the item is used on, -1 for none */
    u8 pad236[0x238 - 0x236];
    s32 equipBulletBk;
    u8 pad23C[0x26C - 0x23C];
    SKLID stock_useskill[10]; /* skills the menu lists, from sbtlGetSkillMenu */
    u16 stock_useskill_num;
    u8 pad296[0x298 - 0x296];
    s32 tgt_csr; /* target cursor: unit ID under the cursor */
    u8 pad29C[0x2CC - 0x29C];
    struct SBTLUNIT_LISTWORK *list2CC;
    s32 field2D0; /* list2D4 index found by sbtlCallBtlCheckDamageTarget2, -1 for none */
    struct SBTLUNIT_LISTWORK *list2D4;
    s16 field2D8; /* negotiation character set: indexes sbtlEvent_SetCharacterProc's tables */
    u8 pad2DA[2];
    s32 stockMinor[SDAT_STOCK_NUM];
    s32 stockDevilSkill[SDAT_STOCK_NUM];
    u32 summonBit;
    s32 stockNum;
    s32 field344;
    s32 field348;
    u8 field34C[SDAT_STOCK_NUM];
    u8 field358[SDAT_STOCK_NUM];
    s32 stockUnitID[SDAT_STOCK_NUM];
    u32 kuishibBit; /* HD kuishibBit, one bit per stock slot (sbtlCheckBit) */
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
    /* Capture (huuma) contest, set up by sbtlCallMemset10: field4BA is the
     * gauge, captured at 0 and missed at field4B8 - 1. */
    s16 field4C0; /* frames since the demon's last push */
    s16 field4C2; /* frames between the demon's pushes */
    s16 field4C4; /* the demon's push, added to the gauge */
    s16 field4C6; /* presses since the player's last push */
    s16 field4C8; /* presses per push */
    s16 field4CA; /* the player's push, taken from the gauge */
    s16 field4CC; /* MAG each press costs */
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
#endif

extern SBTL_WORK *wk_sbtl;
extern sdfVector4_t sbtlCallSetPosPoseSetDds3BasicData;  /* camera up vector (0, 1, 0, 0) */
extern sdfVector4_t sbtlCallSetPosPoseSetDds3BasicData2; /* camera rotation (0, 0, 0, 1) */
extern sdfVector4_t sbtlCallSetPosPoseSetDds3Basic2Data;
extern sdfVector4_t sbtlCallSetPosPoseSetDds3Basic2Data2;

/* A devil's item drop slot. */
typedef struct SBTLITEM_WORK {
    s16 itemid;
    u8 para;
    s8 per;
} SBTLITEM_WORK;

#ifdef VERSION_RAIDOU2
/* Devil table entry returned by sbtlIfDrawActionSub (0x94 bytes). Partial raidou2 layout. */
typedef struct SBTL_DEVIL_DATA {
    u32 flag;
    u8 pad04[0x6 - 0x4];
    s16 motionId;
    u8 pad08[0xA - 0x8];
    s16 field0A;
    s16 aisyoId;
    s8 kuda_id; /* HD name */
    u8 pad0F[0x14 - 0xF];
    s16 field14;
    u8 pad16[0x30 - 0x16];
    s16 haveskill[16]; /* HD DEVILDATA_WORK haveskill */
    u8 pad50[0x68 - 0x50];
    SBTLITEM_WORK item[4];
    f32 field60;
    f32 field64;
    s16 field80;
    s16 field82; /* index into DDS3_GBWK->devilWork */
    u8 pad84[0x8C - 0x84];
    u8 mskskill; /* HD name */
    u8 pad8D[0x94 - 0x8D];
} SBTL_DEVIL_DATA;

SIZE_CHECK(SBTL_DEVIL_DATASizeCheck, SBTL_DEVIL_DATA, 0x94);
#else
/* Devil table entry returned by sbtlGetDevilData_sbtlSub. */
typedef struct SBTL_DEVIL_DATA {
    u32 flag;
    u16 level;
    s16 motionId;
    s16 minorno_d; /* copied to SBTLUNIT_WORK.minor_d, the model id mdlPrepare loads */
    s16 field0A;
    s16 aisyoId;
    s8 field0E; /* race, copied to sdatUnitWork_t.raceId */
    u8 field0F;
    s16 field10; /* base MAG cost of a summon, read by sbtlGetMagCost */
    u8 pad12[2];
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
#endif


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
void sbtlResetCombine(void);
#ifndef VERSION_RAIDOU2 /* Raidou 2's sbtlCallMemset is other code */
s32 sbtlCallMemset(struct SBTLUNIT_WORK *wk, s32 index);
#endif
s32 sbtlForceDelete_SkillFlag_Acter(struct SBTLUNIT_WORK *wk, u32 flg);

#ifdef VERSION_RAIDOU2
/* One generator entry of a stage. Partial raidou2 layout: HD's s16 devil and s8 groupID, the rest unread so far. */
typedef struct SB_ENCOUNT1 {
    u8 pad00[2];
    s16 devil;
    s8 groupID;
    u8 pad05[3];
} SB_ENCOUNT1;
#else
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
#endif

/* One battle stage's row (0x100 bytes) in the table sbtlData100 points at, indexed by SBTL_WORK.stageNo. */
typedef struct SBTL_STAGE_DATA {
    s8 mapNo[2];    /* 0x00: copied to SBTL_WORK.mapNo */
    s8 form;        /* 0x02: mapMapSetForm, 0 for none */
    s8 encPoint;    /* 0x03: mapEncPointGet point */
    u16 flags;      /* 0x04: 0x20 stops units at battle start */
    s16 bgmId;      /* 0x06 */
    u8 pad08[8];
    s16 endtype;    /* 0x10: 0 and 2 reload the map after the result screen */
    s16 end_eveno;  /* 0x12: sequence after the battle when endtype is 1 */
    s8 end_map;     /* 0x14 */
    s8 end_floor;   /* 0x15 */
    s16 end_playerpos; /* 0x16 */
    char end_scrip[0x18]; /* 0x18 */
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
    u8 pad008[0x14 - 0x8];
    SKLID sklid[4]; /* talk skills (HD SBTLTALK_WORK sklid) */
    u8 pad024[0x198 - 0x24];
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
#ifndef VERSION_RAIDOU2
    f32 cancelrate;
    f32 se_rate; /* s32 in HD */
#endif
} SBTL_UNIT_SUB;

#ifdef VERSION_RAIDOU2
SIZE_CHECK(SBTL_UNIT_SUBSizeCheck, SBTL_UNIT_SUB, 0x1C);
#else
SIZE_CHECK(SBTL_UNIT_SUBSizeCheck, SBTL_UNIT_SUB, 0x24);
#endif

typedef struct SBTL_MOTION_VISUAL {
#ifdef VERSION_RAIDOU2
    u8 field00;
    u8 kind; /* Raidou 2 reads the kind from byte 1 */
#else
    u8 kind;
    u8 pad01;
#endif
    u8 pad02[2];
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
#ifndef VERSION_RAIDOU2
    u8 field2C;
    u8 pad2D;
    s16 field2E;
    s16 field30;
    s16 field32;
    SBTL_UNIT_SUB mwk[43]; /* HD VISUALDATA_WORK.mwk, inline on the PS2 */
#endif
} SBTL_MOTION_VISUAL;

#ifdef VERSION_RAIDOU2
/* raidou2 keeps the motion records after the shared head, 43 for the player and 16 or 27 for devils. */
SIZE_CHECK(SBTL_MOTION_VISUALSizeCheck, SBTL_MOTION_VISUAL, 0x2C);

typedef struct SBTL_PLAYER_VISUAL {
    SBTL_MOTION_VISUAL head;
    SBTL_UNIT_SUB mwk[43];
} SBTL_PLAYER_VISUAL;

SIZE_CHECK(SBTL_PLAYER_VISUALSizeCheck, SBTL_PLAYER_VISUAL, 0x4E0);

/* Devils below model 416. */
typedef struct SBTL_DEVIL_VISUAL {
    SBTL_MOTION_VISUAL head;
    SBTL_UNIT_SUB mwk[16];
} SBTL_DEVIL_VISUAL;

SIZE_CHECK(SBTL_DEVIL_VISUALSizeCheck, SBTL_DEVIL_VISUAL, 0x1EC);

/* Devils from model 416 on, indexed from 416. */
typedef struct SBTL_DEVIL_VISUAL2 {
    SBTL_MOTION_VISUAL head;
    SBTL_UNIT_SUB mwk[27];
} SBTL_DEVIL_VISUAL2;

SIZE_CHECK(SBTL_DEVIL_VISUAL2SizeCheck, SBTL_DEVIL_VISUAL2, 0x320);

extern SBTL_PLAYER_VISUAL *sbtlData99;
extern SBTL_DEVIL_VISUAL *sbtlData167;
extern SBTL_DEVIL_VISUAL2 *sbtlData168;
#else
SIZE_CHECK(SBTL_MOTION_VISUALSizeCheck, SBTL_MOTION_VISUAL, 0x640);

/* Per-model visual tables for devils (major nonzero) and for the player (major zero). */
extern SBTL_MOTION_VISUAL *sbtlData98;
extern SBTL_MOTION_VISUAL *sbtlData99;
#endif

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
#ifdef VERSION_RAIDOU2
extern s32 sbtlBGMLOADSTARTData;
#endif
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

/* Files loaded for one model, found by major, minor and devil id (R2 has no motionSE file). */
typedef struct SBTL_PACDATA {
    SBTL_PACDATA_FILE mht;
#ifdef VERSION_RAIDOU1
    SBTL_PACDATA_FILE motionSE;
#endif
    SBTL_PACDATA_FILE file20;
    SBTL_PACDATA_HITLIST hit;
    SBTL_PACDATA_KEY key;
} SBTL_PACDATA;

#ifdef VERSION_RAIDOU1
SIZE_CHECK(SBTL_PACDATASizeCheck, SBTL_PACDATA, 0x90);
#else
SIZE_CHECK(SBTL_PACDATASizeCheck, SBTL_PACDATA, 0x80);
#endif

#define SBTL_PACDATA_NUM 16

/* Pointed to by sbtlData84, 0x908 bytes (0x808 in Raidou 2). */
typedef struct SBTL_PACDATA_WORK {
    SBTL_PACDATA pac[SBTL_PACDATA_NUM];
    u8 pad900[4];
    sdfMemHandle_t *handle;
} SBTL_PACDATA_WORK;

/* Pointed to by sbtlData83, 0x115C bytes (0x385C in Raidou 2). */
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
#ifdef VERSION_RAIDOU2
SIZE_CHECK(SBTL_WORK2SizeCheck, SBTL_WORK2, 0x385C);
#else
SIZE_CHECK(SBTL_WORK2SizeCheck, SBTL_WORK2, 0x115C);
#endif

extern SBTL_WORK2 *sbtlData83;
#ifdef VERSION_RAIDOU2
extern const char slStrCampEffTesEpl[]; /* "/camp/EFF_TES.EPL" */
extern u32 effScalyTextureHandle3;
extern void *fldTitleMiniWork6;
extern struct effHandle_t *fldTitleMiniTex6;
extern const sdfVector4_t slCallEffectLoadEPLTable;
extern s32 sbtlSetRndEncBgData;
extern s32 sbtlSceneLoadData4;
extern s32 sbtlSceneLoadData5;
extern s32 sbtlSceneLoadData6;
extern dds3ProcessID_t *sbtlCallGetProcessStatData;
/* HD sBtlBossEvent_t, the boss event process work (0x30 bytes in R2). */
typedef struct sBtlBossEvent_t {
    struct sdfMemHandle_t *hMem;    /* 0x00 */
    dds3ProcessID_t *listPID;       /* 0x04 */
    struct sdfMemHandle_t *hDatScr; /* 0x08 */
    u8 *pDatScr;                    /* 0x0C */
    s32 scrReq;                     /* 0x10 */
    dds3ProcessID_t *scrPID;        /* 0x14 */
    u8 step;                        /* 0x18 */
    u8 flag;                        /* 0x19 */
    s8 end_mode;                    /* 0x1A */
    s8 end_enemypos_no_reset;       /* 0x1B */
    s16 fade_count;                 /* 0x1C */
    s16 eventtype;                  /* 0x1E */
    u8 unk20[0xC];
    void *pTypeBossNmlEvent;        /* 0x2C */
} sBtlBossEvent_t;
SIZE_CHECK(sBtlBossEvent_t_size, sBtlBossEvent_t, 0x30);
extern dds3ProcessID_t *inputpanel_id2;
#endif
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
s32 CheckStopBattle(void);
s32 sbtlCallBtlGetScrollID(void);
void pbSetGameCamera0(void);
void pbLeaf19(void);
s32 sbtlCallBtlDeleteOnData(void);
s32 sbtlCallBtlGetPlayerData(void);
s32 sbtlCallBtlSearchEffectData2(s32 id, u16 flg, void *pData);
s32 sbtlBtlLoadPlayerOnMemDataSub(s32 index, void *pData);
struct SBTLEFF_WORK *sbtlCreateEff(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid, s32 starttime,
                                      const f32 *defpos, u32 rgba, s8 flg);
void sbtlCallMemset3(s32 battleMode, s32 stageNo);
s32 sbtlBtlLoadHuumaMesDataSub(s32 huumaMesNo);
s32 sbtlCheckAutoSkill_Enemy(struct SBTLUNIT_WORK *wk);
s32 sbtlCallCameraObjectBasicGetFovy(SBTLCAMERA_WORK *wk, struct dds3Basic_t *cm);
s32 sbtlCallSetPosPoseSetDds3Basic(SBTLCAMERA_WORK *wk);
s32 sbtlCallSetPosPoseSetDds3Basic2(SBTLCAMERA_WORK *wk);
s32 sbtlCheckScriptData(s32 devilId);
s32 sbtlCheckTutorialBattle(void);
#ifdef VERSION_RAIDOU2
s32 IsSceanStop(void);
void sbtlInitSBtlSub3(void);
void sbtlCallBtlCheckTutorialBattle(void);
s32 sbtlClearGeneratorWork(void);
void sbtlSetGunNum(s32 num);
extern s32 Exit_sbtlData;  /* stage Init_sbtl uses when the caller gives none */
extern s32 Exit_sbtlData2; /* sword Init_sbtl equips in that case */
E_CAMERA_MODE sbtlGetMode(void);
extern s32 sbtlSbattle3Data; /* frames counted by sbtlSbattle3 */
void cmpRefSbtl2(void);
s32 Exit_sbtl(s32 mode);
s32 sbtlCheckStartBattle(void);
s32 sbtlRunSklProc(void);
void sbtlEisyoWaitTask(void);
void sbtlAttackwaitWaitTask(void);
void sbtlLeaf42(void);
s32 sbtlSetCameraLockOnTgtUpdate(void);
void sbtlKettouProc(void);
s32 sbtlCallObjectBaseSetPadControl(void);
s32 sbtlProcHitStop(void);
s32 sbtlGeneratorProc(void);
s32 slCallPlaySE(void);
void slQueryYesNoDraw_slMain(void);
s32 Proc_sbtlSound(void);
s32 CheckEnd(void);
#endif
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
void OnDataLoadShutdown(dds3ProcessID_t *id);
s32 DataLoadShutdown(dds3ProcessID_t *id);
void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode);
s32 sbtlRefSbtl31(void);
void sbtlResetRndEncCount(void);
void sbtlRndEncTopPriorityMap(s32 *mapOut, s32 *floorOut);
s32 sbtlSbattle(SBTL_START_DATA *start, s8 flag);
#ifdef VERSION_RAIDOU2
s32 sbtlSbattle3(dds3ProcessID_t *id);
s32 sbtlSbattle4(dds3ProcessID_t *id);
s32 DrawProc2_sbtl(dds3ProcessID_t *id);
s32 ReloadTableData(void);
void Init_sbtl(s32 battleMode, s32 stageNo);
#else
s32 sbtlSbattle3(void);
#endif
void sbtlCallEffectShutdown3(s32 mode);
sdatUnitWork_t *sbtlGetStockUnitData(s32 id);
s32 sbtlLeaf53(sdatUnitWork_t *unit);
s32 sbtlGetDamage_GB(sdatUnitWork_t *acter, sdatUnitWork_t *unit, SKLID sklid);
s32 CalcItem(sdatUnitWork_t *unit, s32 itemID);
s32 sbtlCheckItemMember(s32 stockID);
s32 sbtlCheckItemMember_Dead(s32 stockID);
void sbtlSetWeakBit(s32 devilId);
s32 sbtlSbattle5(dds3ProcessID_t *id);
s32 sbtlSbattle6(dds3ProcessID_t *id);
void sbtlSbtlCameraproc(void);
s32 ProcCamera(dds3ProcessID_t *id);
s32 ShutdownCamera(dds3ProcessID_t *id);
s32 sbtlSetScrollID(s32 id);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index, s32 flg);
#ifdef VERSION_RAIDOU2
/* The raidou2 symbol of this name takes sbtlGetVisualData's arguments. */
SBTL_MOTION_VISUAL *sbtlGetMotionVisualData(s32 a, s32 id);
SBTL_UNIT_SUB *sbtlGetMotionVisualData_sbtlVisual(s32 a, s32 id, s32 b);
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
void sbtlCheckBattleLose(void);
struct SBTL_MODELBIND_LIST *effAllocateModelBindings(void);
void effStopEffectStop(void);
SBTL_STP *evtAllocateContext(void);
s32 pbCallBtlCheckLoadFlg(void);
void sbtlAllInit(void);
s32 sbtlBattleSBPACK(void);
s32 sbtlEvent_CheckRain(void);
s32 sbtlEvent_LoadEventScript(void);
void sbtlBattleStart(SBTL_STAGE_DATA *stage);
void sbtlSupport_BattleStop(s32 motionFlag, s32 effectFlag, s32 SEFlag);
s32 sbtlCheckStockPara(sdatUnitWork_t *unit);
s32 sbtlCheckAutoSkill(sdatUnitWork_t *unit, s32 flg);
s32 sbtlSetAutoSkill_GB(sdatUnitWork_t *unit);
s32 sbtlGetStockPoint(void);
#ifdef VERSION_RAIDOU2
s32 sbtlChkHaveMemSkl(sdatUnitWork_t *unit, SKLID id);
#endif
s32 sbtlBtlNakamaSetUpSub(sdatUnitWork_t *unit);
void sbtlCallBtlCheckEnshutuSummonUnit(void);
s32 sbtlCheckGenerator_ND(void);
void sbtlCheckBattleWin(void);
s32 sbtlSetAction(struct SBTLUNIT_WORK *wk, u32 action, s32 flg);
s32 sbtlSetMotionHit(struct SBTLUNIT_WORK *wk, s32 motionno, SKLID skillid);
void sbtlCallBtlGetItemtbl(void);
s32 sbtlCallBtlGetNumUnitGroupID(void);
s32 sbtlCallBtlMapInit(void);
s32 sbtlCheckNextAction(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchPacData(s32 major, s32 minor, s32 devilid, SBTL_HIT_PACFILE *file, s32 size);
s32 sbtlCallBtlSearchPacData2(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 DrawProc2(s32 major, s32 minor, s32 devilid, void *data, s32 size);
s32 sbtlCallBtlSearchUnitID32(void);
s32 sbtlCallBtlSearchUnitID33(void);
s32 sbtlCallBtlSearchUnitID7(s32 x, s32 y);
struct SBTLDVLTK_LISTWORK *sbtlCallChipAllocZeroDirect(void);
s32 sbtlCallMemFreeDirect6(struct SBTL_MODELBIND_LIST *lwk, u8 flg);
s32 sbtlCallPadVibeFadeUpdate(struct SBTLEFF_LISTWORK *lwk);
#ifdef VERSION_RAIDOU1
s32 sbtlCallSetOt(void);
#endif
s32 sbtlCallSin2(void);
s32 Proc_sbtlDvltk(struct SBTLDVLTK_LISTWORK *lwk);
s32 sbtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlSetHitStop(s32 sttime, s32 cmpcnt);
#ifdef VERSION_RAIDOU2
void sbtlLeaf110(f32 x, f32 y, f32 z);
void sbtlRefSbtl54(s32 num);
#endif
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
#ifdef VERSION_RAIDOU2
struct SBTLUNIT_WORK *sbtlGetNakama(s8 idx);
s32 sbtlGetGun_Num(void);
void sbtlRefSbtl66(s32 num);
s32 sbtlRefSbtl71(s32 num);
E_CAMMOD_CUT sbtlGetCutFlg(void);
s32 sbtlCheckSummonUnit(void);
s32 sbtlCheckSummonUnit_Dead_or_ALive(s8 idx);
#endif
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
void FakeRndHit(struct SBTLUNIT_WORK *wk, s32 pow);
s32 sbtlCallBtlSearchUnitID12(void);
void sbtlBeginSetsumaSub(struct SBTLUNIT_WORK *wk);
s32 sbtlCallHirukoYukaGoIdx2B(u32 color);
void *sbtlCallGetProcessData43(void);
void sbtlSummonMenuFuncOpen00Sub(u32 no);
#ifdef VERSION_RAIDOU1
s32 sbtlRefSbtl27(void);
s32 sbtlRefSbtl29(void);
#endif
void sbtlRefSbtl4(void);
void sbtlRefSbtl45(void);
void sbtlInitTutorial(void);
s32 sbtlRefSbtl47(void);
s32 sbtlRefSbtl62(struct SBTLUNIT_WORK *wk);
void CalcAisyo_sbtlCalc(struct SBTLUNIT_WORK *wk);
void sbtlGetPlayerTec_ParaUp(sdatUnitWork_t *unit, sdatParaWork_t *para);
s32 sbtlReleaseDelList(struct SBTLEFF_DELLISTWORK *lwk, u8 flg);
s32 sbtlReleaseDvltk_List(struct SBTLDVLTK_LISTWORK *lwk, u8 flg);
s32 sbtlReleaseSTP_List(SBTL_STP *stp, u8 flg);
s32 sbtlCreateSTP_Unit(SBTL_STP *stp, struct SBTLUNIT_WORK *wk, s16 id, s32 time);
#ifdef VERSION_RAIDOU1
s32 sbtlSbattle2(s32 flg);
#endif
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

#ifdef VERSION_RAIDOU2
typedef struct RED_GUAGE {
    s32 bgn;
    s32 len;
    s32 cnt;
} RED_GUAGE;
void sbtlInitRedGg(RED_GUAGE *pRedGg, s32 hp);
void sbtlAnalyzeLoyalty(s32 x, s32 y, RED_GUAGE *pRedGg, s32 hp, s32 maxhp);
extern s32 slQueryYesNoDraw_slMainData;
extern s32 slQueryYesNoDraw_slMainData2;
extern s32 slQueryYesNoDraw_slMainData3;
extern s32 slQueryYesNoDraw_slMainData4;
extern s32 slData10;
PLAYER_LUCK sbtlGetPlayerLuckStep(void);
s32 sbtlBtlCheckSummonSub(sdatUnitWork_t *unit);
s16 cmpProcSequenceSub(s32 stage);
void pbCallEffectDelete(void);
void slCallEffectLoadEPL(void);
s32 sbtlCallBtlGetCutFlg(void);
s32 sbtlCallGetActiveScene(void);
s32 sbtlMainDraw(void);
s32 EventProc(void);
s32 sbtlListProc(struct SBTLEFF_LISTWORK *lwk);
s32 sbtlAnimListProc_sbtlDMG(void);
s32 sbtlDrawAnimList(void);
s32 Draw_sbtlSM(struct SBTLSM_LISTWORK *lwk);
void sbtlInitEffect(void);
void DrawTime(void);
/* Battle texture files sbtlBattleTmx loads: k_01..04, h_01..04 and b_roul.spr, each a handle and its data. */
extern void *sbtlBattleTmxData, *sbtlBattleTmxData2, *sbtlBattleTmxData3, *sbtlBattleTmxData4;
extern void *sbtlBattleTmxData5, *sbtlBattleTmxData6, *sbtlBattleTmxData7, *sbtlBattleTmxData8, *sbtlData150;
extern u32 sbtlData142, sbtlData143, sbtlData144, sbtlData145;
extern u32 sbtlData146, sbtlData147, sbtlData148, sbtlData149, sbtlBattleTmxData9;
#endif

#endif
