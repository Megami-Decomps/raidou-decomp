#ifndef SBTLUNIT_H
#define SBTLUNIT_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"
#include "sdatUnit.h"
#include "mapLight.h"
#include "kernel.h"
#include "sbtlDMG.h"

struct SBTLUNIT_LISTWORK;
struct mdlObj_t;
struct dds3Basic_t;
struct evtUnit_t;
struct SBTLAI_WORK;
struct fileRequest;
struct sdfMemHandle_t;
struct effRefObj_t;
struct SBTL_STP;

#ifdef VERSION_RAIDOU2
/* Partial raidou2 layout: only the fields read so far are named, the rest is padding. */
typedef struct SBTLUNIT_MODEL {
    sdfVector4_t vec00;
    u8 pad010[0x10];
    sdfVector4_t vec20; /* HD searchpos */
    u8 pad030[0x10];
    sdfVector4_t hitbk_dir;
    u8 pad050[0x40];
    sdfVector4_t scrPos3;
    u8 pad0A0[0x68];
    struct sdfMemHandle_t *handle;
    u16 mode;
    u8 pad10E[0x2];
    u32 flags;
    s32 field0FC;
    s32 field100;
    s32 field104;
    u8 pad120[0x8];
    s32 frame;
    s32 cmpframe;
    s32 target_frame;
    s32 mahaskl_waitframe;
    u8 pad138[0xC];
    u8 field118;
    u8 pad145;
    u16 stat;
    f32 field11C;
    f32 field120;
    f32 field124;
    f32 field128;
    f32 field12C;
    f32 field130;
    u8 pad160[0x170 - 0x160];
    s32 kaihiID; /* unit the N_Kaihi AI dodges (HD kaihiID) */
    f32 hitbk_cnt;
    f32 hitbk_cmpcnt;
    f32 hitbk_rate;
    s8 nonempflg;
    u8 pad181;
    s16 h_serialID;
    s8 dataflg;
    u8 pad185[0x3];
    s32 datamode;
    s32 data_p[2];
    s32 ai_target[3]; /* unit IDs found by sbtlCallBtlCreateSideList2 (HD ai_target) */
    u8 pad1A0[0x1A4 - 0x1A0];
    u32 useskl[41]; /* skill-used bits, one per SKLID */
    u32 chkskl[41]; /* 0x248: skill-checked bits (HD chkskl); length not read yet */
    u8 pad2EC[0x2F4 - 0x2EC];
    s32 scrMotion; /* HD name */
} SBTLUNIT_MODEL;
#else
typedef struct SBTLUNIT_MODEL {
    sdfVector4_t vec00;
    u8 pad010[0x10];
    sdfVector4_t vec20;
    sdfVector4_t vec30;
    sdfVector4_t hitbk_dir;
    sdfVector4_t scrPos;
    sdfVector4_t scrRot;
    sdfVector4_t scrPos2;
    sdfVector4_t scrRot2;
    sdfVector4_t scrPos3;
    u8 padA0[0xF0 - 0xA0];
    struct sdfMemHandle_t *handle;
    u16 mode;
    u8 pad0F6[0x2];
    u32 flags;
    s32 field0FC;
    s32 field100;
    s32 field104;
    s32 field108;
    s32 field10C;
    s32 frame;
    s32 cmpframe;
    u8 field118;
    u8 pad119;
    u16 stat;
    f32 field11C;
    f32 field120;
    f32 field124;
    f32 field128;
    f32 field12C;
    f32 field130;
    u8 pad134[0x140 - 0x134];
    s16 field140;
    u8 pad142[0x144 - 0x142];
    s32 kaihiID; /* unit the N_Kaihi AI dodges (HD kaihiID) */
    s16 path_mode;
    u8 pad14A[0x2];
    s32 path_cnt;
    s16 path_datap;
    u8 pad152[0x2];
    sdfVector4Unaligned_t path_ofspos;
    f32 hitbk_cnt;
    f32 hitbk_cmpcnt;
    f32 hitbk_rate;
    s8 nonempflg;
    u8 pad171;
    s16 h_serialID;
    s8 dataflg;
    u8 pad175[0x3];
    s32 datamode;
    s32 data_p[2];
    s32 ai_target[3]; /* unit IDs found by sbtlCallBtlCreateSideList2 (HD ai_target) */
    u8 pad190[0x194 - 0x190];
    u32 useskl[17]; /* skill-used bits, one per SKLID */
    u32 chkskl[19]; /* 0x1D8: skill-checked bits (HD chkskl), up to field224 */
    s32 field224;
} SBTLUNIT_MODEL;
#endif

struct SBTLUNIT_WORK;

/* Footstep sound state of a devil unit. */
typedef struct SBTLSOUND_FOOTWORK {
    f32 prevFrame;
    s32 prevSMGID;
    s32 played;
    s32 pad0C;
} SBTLSOUND_FOOTWORK;

/* Sound state of a unit, embedded in SBTLUNIT_WORK at 0x130. */
typedef struct SBTLSOUND_DEVILWORK {
    struct sdfMemHandle_t *at3Handle;
    s32 flag;
    struct sdfMemHandle_t *loadHandle;
    struct fileRequest *loadFile;
    s32 field10;
    void *pBossAt3Data;
    s32 bossAt3gstat;
    s32 bossAt3Size;
    void *loadData;
    s32 loadgstat;
    s32 loadSize;
    s32 mot_SMGID;
    s32 mot_SMGID2;
    s32 mot_SMGID3;
    s32 mot_status;
    s32 mot_nowcnt;
    s32 mot_startcnt;
    s32 mot_seid;
    s32 mot_vol;
    s32 mot_step;
    s32 mot_motionID;
    SBTLSOUND_FOOTWORK footwk;
} SBTLSOUND_DEVILWORK;

s32 CheckLiveTarget(struct SBTLUNIT_WORK *wk);
s32 CreateSTP(struct SBTL_STP *lwk, s32 ListID, const sdfVector4_t *pos, s16 id, s32 time);
s32 Draw_sbtlUnit(void);
s32 Exit_sbtlUnit(void);
s32 Init_sbtlUnit(s8 flg);
s32 ResetAction(struct SBTLUNIT_WORK *wk, s8 flg);
s32 sbtlResetAction(struct SBTLUNIT_WORK *wk);
s32 sbtlSetUseSkill_sbtlActAI(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 after_frame);
s32 GuardProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode);
s32 CheckStop_BadStatus(struct SBTLUNIT_WORK *wk);
s32 CheckAction_Nakama(struct SBTLUNIT_WORK *wk);
s32 BeginSetsuma(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlGetPlayerUnit(s32 id);
s32 sbtlGetPlayerID(void);
s32 sbtlCallBtlGetBadStatus5(struct SBTLUNIT_WORK *wk);
s32 sbtlRefSbtl16(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCallBtlCreateUnitList(u8 side);
void sbtlCallBtlCreateUnitList2(struct SBTLUNIT_WORK *wk);
s32 sbtlGetNearTarget_Random(struct SBTLUNIT_WORK *wk, const sdfVector4_t *rot, f32 area_r, f32 rad0, f32 rad1, s8 flg);
s32 sbtlCheckArea(struct SBTLUNIT_WORK *wk, f32 r, const sdfVector4_t *pos);
void UnitMV(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos);
s32 sbtlCallBtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk);
void sbtlLeaf59(struct SBTLUNIT_WORK *wk, u32 color);
s32 sbtlCheckExist(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckHITCheck_Char(struct SBTLUNIT_WORK *wk);
void sbtlCheckGuardMotion(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckModelDataLoadEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist2_sbtlUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlSetMotion(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlDeleteUnitList(struct SBTLUNIT_LISTWORK *lwk);
s32 sbtlCheckMapHit_Data(f32 posx, f32 posz, f32 *px, f32 *pz);
s32 sbtlCallBtlCheckMapHitData(f32 posx, f32 posz);
s32 sbtlCallBtlCreateEnemyList(dds3ProcessID_t *id);
s32 sbtlModelDataLoad(dds3ProcessID_t *id);
s32 sbtlCallBtlCreateUnit(s8 flg);
#ifdef VERSION_RAIDOU2
extern const char Init_sbtlUnitStrSbtlunitProc[];
s32 Proc_sbtlUnit(dds3ProcessID_t *id);
s32 sbtlSetPlayerData(s8 flg);
#endif
s32 EventBattleStopChack(void);
s32 sbtlRefSbtl8(struct SBTLUNIT_WORK *wk);
s32 sbtlFadeProc(struct SBTLUNIT_WORK *wk);
s32 sbtlCallLoad(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlModelDataLoadSBtlUnitSub(struct SBTLUNIT_WORK *wk);
s32 AllDeleteEnemy(struct SBTLUNIT_WORK *wk);
s32 sbtlModelDataLoad_sbtlUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckModelDataLoadEnd_sbtlUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlCheckEnshutuSummonUnit2(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetDevilDataSBtlSub3(struct SBTLUNIT_WORK *wk);
s8 sbtlCheckMapHit_Cross(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis);
u32 UnitRegist(MODELTYPE major, s32 minor);
s32 sbtlCallPlayMotionSENow(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlCheckMotionNoSub(struct SBTLUNIT_WORK *wk, u8 flg, s32 no);
s32 sbtlCreateEnemyList(void);
s32 sbtlCheckReturn_Unit(struct SBTLUNIT_WORK *wk);
s32 sbtlSearchReturn_Unit(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckLiveTarget_NotHitDmg(struct SBTLUNIT_WORK *wk);
s32 CheckMapHit(struct SBTLUNIT_WORK *wk, const sdfVector4_t *oldpos);
s32 sbtlRestartMotion(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlCreateUnit(s32 major, s32 minor, s16 mode);
SBTLUNIT_MODEL *sbtlCreate(void);
s32 sbtlDelete(SBTLUNIT_MODEL *model);
f32 sbtlGetMotionMove(struct SBTLUNIT_WORK *wk, f32 frame);
s32 sbtlCallBtlGetMotionVisualData3(struct SBTLUNIT_WORK *wk);
void sbtlCallPreCalcStrWidth(struct SBTLUNIT_WORK *wk);
void sbtlBtlSetMotionSBtlUnitSub(struct SBTLUNIT_WORK *wk);
s32 sbtlSetLight_Map(struct SBTLUNIT_WORK *wk, s32 frame, s32 flg);
void sbtlGetRotate(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index, s32 flg);
s32 sbtlCallBtlGetSkilltbl10(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckYukaDamageEffect(struct SBTLUNIT_WORK *wk, SKLID skillid, u16 type, s32 a3, s8 a4);
s32 sbtlCallBtlGetDevilDataSBtlSub4(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlLoadAttackHitData(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetMotionVisualData2(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
#ifdef VERSION_RAIDOU2
s32 sbtlCheckLoopMotion(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlSetMotion_ID(struct SBTLUNIT_WORK *wk, s32 id, s32 bframe, s32 aframe, s8 flg);
void sbtlCallMemFreeDirect(MODELTYPE major, s32 minor, s32 devilid);
void cmpCallMemFreeDirect(struct SBTLUNIT_WORK *wk);
void cmpLeaf3(struct SBTLUNIT_WORK *wk);
s32 sbtlDrawShadow(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlGetRaceName(struct SBTLUNIT_WORK *wk);
s32 sbtlGetMotionNo(struct SBTLUNIT_WORK *wk, u8 side, s32 no);
#endif
s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlCheckTateSkillSelectTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList(u8 side, s32 flg);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList_sbtlUnit(u8 side, s32 flg);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_MASK(u8 side, u32 mask, u32 mask_v);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList(u8 side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_AI(u8 side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Group(u8 side, s32 GroupID);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_PForward(u8 side);
struct SBTLUNIT_LISTWORK *sbtlCreateUnitList(s32 number);
s32 sbtlDeleteUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlEntryUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlGetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlGetNumUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
s32 sbtlGetNumUnit_GroupID2(struct SBTLUNIT_LISTWORK *lwk, s32 serialID, s32 flg);
s32 sbtlRefSbtl33(struct SBTLUNIT_WORK *wk);
s32 sbtlChangeModelData(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlSetEndDataSub(struct SBTLUNIT_WORK *wk, s32 stockID);
s32 sbtlDeleteAttackHitData(struct SBTLUNIT_WORK *wk);
void sbtlCallMemFreeDirect2(struct SBTLUNIT_WORK *wk);
s32 sbtlRefSbtl19(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlCreateDelWork(struct SBTLUNIT_WORK *wk);
void sbtlPacData_FreeIndex(MODELTYPE major, s32 minor, s32 devilID);
s32 sbtlCallDeleteSceneBasicData(struct SBTLUNIT_WORK *wk);
s32 sbtlReleaseUnitList_ID(struct SBTLUNIT_LISTWORK *lwk, s32 id);
s32 sbtlResetSummon_E(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlSearchUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
struct SBTLUNIT_WORK *sbtlSearchUnit_ID(struct SBTLUNIT_LISTWORK *lwk, s32 ID);
s32 ChangeTarget_BSHelper(void);
s32 sbtlSearchUnit_Nakama(struct SBTLUNIT_WORK *wk);
s32 sbtlSetAIMode(struct SBTLUNIT_WORK *wk);
s32 sbtlEntryStockData(s32 devilid);
s32 sbtlEntryStockData_Huuma(void);
void sbtlSetEndBadStatus(void);
s32 sbtlSetEndData(void);
s32 sbtlSetFade(struct SBTLUNIT_WORK *wk, SBTLUNIT_VST flg, s32 start, s32 count, s32 para);
s32 sbtlSetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlSetUnitLight(struct SBTLUNIT_WORK *wk, s32, u32 diffuse, u32 ambient, s32 flg, s32 flg2);
s32 sbtlSetUnitLight_Dir(struct SBTLUNIT_WORK *wk, const sdfVector4_t *dir, s32 flg);

/* sbtlSetLight_Map's copy of the map lights. */
extern mapLightParam_t sbtlCallGetLightParamData;
extern mapLightParam_t LightSetData;
extern mapLightParam_t sbtlSetLight_MapData;
/* Battle model file number per devil, indexed by minor. */
extern s32 sbtlCheckModelDataLoadEnd_sbtlUnitData[];

/* Motion number per SBTLUNIT_MOTION, one row per motion set. */
#ifdef VERSION_RAIDOU2
extern s32 sbtlGetMotionNoData[][56];
extern s32 sbtlGetMotionNoData2[56]; /* side bit 0x80 */
extern s32 sbtlGetMotionNoData3[56]; /* minor > 0 */
#else
extern s32 sbtlBtlCheckMotionNoSubData[][56];
#endif

#ifdef VERSION_RAIDOU2
/* A kaja (stat change) slot of a unit; only the count is read so far. */
typedef struct SBTLSKL_KAJA_WORK {
    s32 m_cnt;
    s32 field4;
} SBTLSKL_KAJA_WORK;

typedef struct SBTLSKL_BARRIER_WORK {
    s32 field0;
    s32 field4;
    s32 size;
    s32 fieldC;
} SBTLSKL_BARRIER_WORK;

/* Partial raidou2 layout: only the fields read so far are named, the rest is padding. */
struct SBTLUNIT_WORK {
    u8 pad000[0x20];
    sdfVector4_t vPosBfrCmb; /* pos saved by sbtlSetCmbBfrPos */
    f32 visual[4];
    sdfVector4_t mv_spd;
    sdfVector4_t max_mvspd;
    sdfVector4_t mv_spdbk;
    sdfVector4_t pos;
    sdfVector4_t rot;
    u8 pad090[0x20];
    sdfVector4_t field0B0;
    u8 pad0C0[0x10];
    sdfVector4_t posSaved;
    u8 pad0E0[0xF0 - 0xE0];
    s32 fieldF0;
    s32 fieldF4;
    s32 fieldF8;
    f32 fieldFC; /* 1.0 from sbtlCreateUnit */
    mapLightParam_t light;
    SBTLSOUND_DEVILWORK se;
    u8 pad170[0x1D4 - 0x170 - sizeof(SBTLSOUND_DEVILWORK)];
    struct sdfMemHandle_t *handle;
    struct fileRequest *pacHandle;
    s32 GroupID;
    s32 field1A0;
    u16 mode;
    u8 pad1E6[0x2];
    sdatUnitWork_t unit;
    s32 haveskill[19]; /* [0..15] copied to unit.haveskill by sbtlCheckAutoSkill_Enemy; sbtlCallBtlSearchDelWorkSide reads [2..17] */
    u8 pad3AC[0x3B0 - 0x3AC];
    SBTL_AISYO_TBL aisyo_bkup; /* unit.aisyo as loaded, before battle changes */
    u8 pad3F0[0x3F4 - 0x3F0];
    sdatParaWork_t kajyapara; /* stat changes added to unit.now by sbtlCheckBattlePara */
    u8 pad40C[0x4];
    SBTLSKL_KAJA_WORK kaja[3][2];
    u8 pad440[0x4];
    s32 karnCnt;
    u8 pad448[0x44C - 0x448];
    s32 field44C;
    u8 pad450[0x474 - 0x450];
    SBTLSKL_BARRIER_WORK barrier;
    u8 pad484[0x4A4 - 0x484];
    s32 field4A4;
    u8 pad4A8[0x4AC - 0x4A8];
    s32 field4AC;
    u8 pad4B0[0x4BC - 0x4B0];
    s32 major;
    s32 minor;
    s32 motionno;
    s32 motionID;
    f32 m_rate;
    s8 visualKind;
    u8 pad4D1[0x1];
    s16 motion_setno;
    s16 fade_sttime;
    s16 fadecnt;
    s32 fadepara;
    s32 cmpfade;
    u32 color;
    u32 chgcolor;
    u32 dstcolor;
    s32 fadergb;
    u32 field3D4;
    u8 pad4F4[0x8];
    s32 next_motionno;
    s32 next_motionID;
    u8 pad504[0x4];
    struct dds3Basic_t *pBasic;
    struct evtUnit_t *hEveUnit;
    struct mdlObj_t *hModel;
    s8 gstat;
    s8 changeflg;
    u8 pad516[0x6];
    s32 field400;
    s16 field404;
    s16 field406;
    struct dds3Basic_t *pBasic_d;
    struct evtUnit_t *hEveUnit_d;
    struct mdlObj_t *hModel_d;
    u8 field414;
    s8 field415; /* set when sbtlCheckMotionMove moved the unit */
    u8 pad532[0x2];
    u32 flg;
    u32 vstatus;
    u32 flg2;
    f32 field424;
    f32 field428;
    f32 angleY;
    f32 deg;
    s16 degcnt;
    u8 pad552[0x2];
    u32 status;
    s32 framecnt;
    u8 pad55C[0x4];
    s32 field560;
    s32 field564;
    u8 pad568[0x4];
    SKLID skillid;
    u8 pad570[0x10];
    SKLID order_sklidbk;
    u8 pad584[0x1];
    s8 action_flg;
    u8 pad586[0xA];
    s32 field474;
    u8 pad594[0x8];
    s32 scrStep; /* HD name */
    u8 pad5A0[0x4];
    s32 field5A4;
    u8 pad5A8[0x8];
    struct SBTLUNIT_MODEL *model;
    struct SBTLAI_WORK *aiwk;
    s16 actCounter;
    s16 actMax;
    u8 pad5BC[0x2];
    s16 field492; /* compared with a hit's actID when flg & 0x800 */
    s16 field494;
    u8 pad5C2[0x2];
    s16 field498;
    u8 pad5C6[0x6];
    s16 field4A0;
    s16 field4A2;
    s32 damage_serialID;
    s32 damage_workID;
    u8 pad5D8[0x5DE - 0x5D8];
    s16 field5DE;
    s32 field5E0;
    s16 field5E4;
    u8 pad5E6[0x5F0 - 0x5E6];
    s32 bst_effserialID2;
    u8 pad5F4[0x600 - 0x5F4];
    struct sdfMemHandle_t *msgHandle;
    struct fileRequest *msgFile;
    void *msgData;
    u8 pad60C[0x4];
    struct sdfMemHandle_t *cacheHandle;
    s32 field4E0;
    struct effRefObj_t *cacheRef;
    struct sdfMemHandle_t *field61C; /* a second cache, freed like cacheHandle */
    s32 field620;
    struct effRefObj_t *field624; /* icon drawn by sbtlCallSetFontDrawOT */
    u8 pad628[0x630 - 0x628];
    s8 light_buff[3];
    s8 field4F3;
    SBTL_PACDATA_HITLIST hitwk;
    s32 field688;
    u8 pad68C[0x6A0 - 0x68C];
    s32 field6A0; /* nonzero: motion column sbtlGetMotionNo uses for motions 0..2 */
    s8 bOldDispOff; /* vstatus bit 0x1000 when sbtlSetCmbBfrPos ran */
    u8 pad6A5[0x6A8 - 0x6A5];
    SKLID field6A8; /* skill name sbtlCallSetFontDrawOT draws */
    u8 pad6AC[0x6C0 - 0x6AC];
    s32 field6C0; /* 1 or 2 when sbtlCheck finds type 4 or 6 at death */
    u32 field6C4; /* bit 0x40 blocks hitback */
    s32 byctCnt;
    u8 pad6CC[0x6E9 - 0x6CC];
    s8 field6E9; /* set by trmSetProcessTerm; HD talkBreakDownFlg, blocks talk */
    u8 pad6EA[0x6F4 - 0x6EA];
    s8 field6F4; /* sbtlForceDelete_SkillFlag_Acter returns 0 when set */
    u8 pad6F5[0x704 - 0x6F5];
    s32 field704;
    s32 field708; /* frames left of the HP bar sbtlCallSetFontDrawOT draws */
    f32 nakama_asimoto_anim;
    s32 field710;
    s32 field714;
    s32 field718;
    s32 field71C;
    u8 pad720[0x72C - 0x720];
    s32 field72C;
    s32 mag_on1; /* HD names; -1 takes the boss event table's value */
    s32 mag_on2;
    s32 mag_on3;
    s32 dead_on;
    s32 no_dead_se; /* HD name; skips the dead voice */
    s32 asimoto_mode; /* HD name */
    f32 asimoto_pos_x;
    f32 asimoto_pos_z;
    f32 asimoto_pos_x2;
    f32 asimoto_pos_z2;
    f32 asimoto_pos_x3;
    f32 asimoto_pos_z3;
    f32 asimoto_pos_x4;
    f32 asimoto_pos_z4;
    s32 field768;
    s32 field76C;
    s32 miusinai;
    s32 kuramasi;
    s32 field778;
    u8 pad77C[0x4];
    s32 field780;
    u8 pad784[0x78C - 0x784];
    s32 field78C;
    f32 field790; /* -4000.0 from sbtlCreateUnit */
    s32 field794;
    s32 L2_muteki; /* HD name */
    s32 field79C; /* set when trmCallBtlCheckReturnUnit cannot start the wait motion */
    s32 field7A0;
    s32 field7A4;
    s32 field7A8;
    s32 field7AC;
    s32 field7B0;
    s32 eff_wait_kaisin;
    u8 pad7B8[0x7EC - 0x7B8];
    s32 cap_skillname_x; /* HD name; screen position of the skill name */
    s32 cap_skillname_y;
    /* sbtlInitLoadAttackHitData_pData clears 0x7F4 bytes */
};
#else
/* Partial layout: only the fields read so far are named, the rest is padding. */
struct SBTLUNIT_WORK {
    f32 visual[4];
    sdfVector4_t mv_spd;
    sdfVector4_t max_mvspd;
    sdfVector4_t mv_spdbk;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t rotBase;
    sdfVector4_t nextpos;
    sdfVector4_t rotbk;
    u8 pad090[0x10];
    sdfVector4_t posSaved;
    sdfVector4_t effpos;
    mapLightParam_t light;
    SBTLSOUND_DEVILWORK se;
    struct sdfMemHandle_t *handle;
    struct fileRequest *pacHandle;
    s32 GroupID;
    s32 field1A0;
    u16 mode;
    u8 pad1A6[0x2];
    sdatUnitWork_t unit;
    s32 haveskill[11]; /* [2..10] come from the devil table's param (HD haveskill) */
    u8 pad2E4[0x8];
    struct SBTL_AISYO_TBL aisyoBase;
    u8 pad32C[0x330 - 0x32C];
    sdatParaWork_t kajyapara; /* stat changes added to unit.now by sbtlCheckBattlePara */
    s8 field348;
    u8 pad349[0x34C - 0x349];
    u32 field34C; /* buff bits: 0x1000000, 0x2000000 raise damage dealt, 0x4000000 lowers damage taken */
    f32 field350; /* percent for field34C */
    u8 pad354[0x358 - 0x354];
    u32 field358;
    f32 field35C; /* percent for field358 */
    s16 field360;
    s16 field362;
    s16 rejyenecounter;
    s16 rejyenecmpcnt;
    s16 rejyenehelper; /* unit ID the regen effect plays on when flg & 0x1000 */
    u8 pad36A[0x2];
    s32 rejyenepara;
    s16 field370;
    s16 slipcounter;
    s16 slipcmpcnt;
    s16 slip_sklid;
    s32 slippara;
    u32 field37C; /* attribute bits; sbtlCheckAisyo_Old takes the lowest */
    u32 field380; /* attribute bits ORed into the affinities field37C selects */
    s16 field384;
    u8 pad386[0x2];
    SKLID barriersklid;
    s16 tdancnt;
    s16 tdancounter;
    s16 tdancmpcnt;
    s16 tdanhitcnt;
    s16 tdanhelper;
    u8 pad396[0x2];
    s32 tdanpara;
    s16 autoreccnt;
    u8 pad39E[0x2];
    s32 major;
    s32 minor;
    s32 motionno;
    s32 motionID;
    f32 m_rate;
    s8 visualKind;
    u8 pad3B5[0x1];
    s16 motion_setno;
    s16 fade_sttime;
    s16 fadecnt;
    s32 fadepara;
    s32 cmpfade;
    u32 color;
    u32 chgcolor;
    u32 dstcolor;
    s32 fadergb;
    u32 field3D4;
    u32 field3D8;
    s16 field3DC;
    s16 field3DE;
    s32 next_motionno;
    s32 next_motionID;
    s16 beforeframe;
    s16 afterframe;
    struct dds3Basic_t *pBasic;
    struct evtUnit_t *hEveUnit;
    struct mdlObj_t *hModel;
    s8 gstat;
    s8 changeflg;
    u8 pad3FA[0x2];
    s32 minor_d;
    s32 field400;
    s16 field404;
    s16 field406;
    struct dds3Basic_t *pBasic_d;
    struct evtUnit_t *hEveUnit_d;
    struct mdlObj_t *hModel_d;
    u8 field414;
    s8 field415;
    u8 pad416[0x2];
    u32 flg;
    u32 vstatus;
    u32 flg2;
    f32 field424;
    f32 field428;
    f32 angleY;
    f32 deg;
    s16 degcnt;
    u8 pad436[0x2];
    /* 0x438 to 0x484 is HD's SBTLACTION_WORK without cmpframecnt3 and effectTime, held inline. */
    u32 status;
    s32 framecnt;
    s32 framecntB;
    s32 framecntC;
    s32 cmpframecnt;
    s32 cmpframecnt2;
    SKLID skillid;
    u32 next_st;
    SKLID order_skillid;
    s32 order_targetID;
    s32 stockID; /* stock slot of a summoned demon, -1 otherwise */
    SKLID order_sklidbk;
    s8 order_setflg;
    s8 action_flg;
    u8 pad46A[2];
    s32 action_count;
    s32 counter;
    s32 field474;
    s32 tok_st;
    s32 tok_datap;
    u32 effect_flg; /* SBTLACTION_EFFECT */
    struct SBTLUNIT_MODEL *model;
    struct SBTLAI_WORK *aiwk;
    s16 actCounter;
    s16 actMax;
    s16 field490;
    s16 field492; /* compared with a hit's actID when flg & 0x800 */
    s16 field494;
    u8 pad496[0x2];
    s16 field498;
    s16 hitsliphelper;
    s32 huttobiID;
    s16 field4A0;
    s16 field4A2;
    s32 damage_serialID;
    s32 damage_workID;
    s16 damage_actionf;
    s16 damage_actionf_cnt;
    s16 damage_hitcnt;
    s16 nguard_dmgcnt;
    s32 field4B4;
    s32 field4B8;
    u8 pad4BC[0x4];
    s32 bst_effserialID2;
    u8 pad4C4[0x8];
    s32 field4CC;
    struct sdfMemHandle_t *msgHandle;
    struct fileRequest *msgFile;
    void *msgData;
    struct sdfMemHandle_t *cacheHandle;
    s32 field4E0;
    struct effRefObj_t *cacheRef;
    s16 msgState;
    u8 pad4EA[0x2];
    s32 field4EC;
    s8 light_buff[3];
    s8 field4F3;
    SBTL_PACDATA_HITLIST hitwk;
};
#endif

/* One slot of a unit list. */
typedef struct SBTLUNIT_LISTWK {
    s32 ID;
    struct SBTLUNIT_WORK *pUnit;
} SBTLUNIT_LISTWK;

/* Unit list made by sbtlCreateUnitList. */
typedef struct SBTLUNIT_LISTWORK {
    s32 number;
    s32 count;
    struct sdfMemHandle_t *handle;
    SBTLUNIT_LISTWK *pUnitL;
} SBTLUNIT_LISTWORK;

void sbtlDeath(struct SBTLUNIT_WORK *wk);
#ifndef VERSION_RAIDOU2
void sbtlDamage(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 damage);
#endif

#ifdef VERSION_RAIDOU2
void sbtlCallSync(void);
void KaihiInit_Sub(struct SBTLUNIT_WORK *wk, s32 mode);
void sbtlKaihiInit(struct SBTLUNIT_WORK *wk);
u32 sbtlCheckKajaAnyEx(struct SBTLUNIT_WORK *wk, s32 *count);
s32 sbtlCheckEnableUseKaja(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlCheckDamageTargetTemp(struct SBTLUNIT_WORK *wk);
void sbtlSetRunAway(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Analyze(u8 side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Talk(u8 side);
struct SBTLUNIT_WORK *sbtlSetSummon_sbtlUnit(s32 a, s32 motionId, s32 stock, s8 flg);
#endif

#endif
