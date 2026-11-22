#ifndef SBTLDMG_H
#define SBTLDMG_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct plistHandle_t;
struct SBTLDMG_ANIMWORK;
struct SBTLDMG_HITLISTWORK;
struct SBTLDMG_HITWORK;
struct SBTLEFF_WORK;
struct SBTLSKILLHIT_POSWORK;
struct SBTLSKILLHIT_TBL;
struct SBTLSKILLP_TBL;
struct SBTLSKILL_TBL;
struct SBTLUNIT_LISTWORK;
struct SBTLUNIT_WORK;
struct SBTLEXP_LISTWORK;
struct mdlObj_t;
struct sdfMemHandle_t;
struct sdatUnitWork_t;

/* Hit position rule (HD names). */
typedef struct SBTLSKILLHIT_POSWORK {
    u8 iti;
    u8 pad01[3];
    s32 iti_no; /* SBTLUNIT_MARK */
    s8 takasa;
    s8 muki;
    u8 pad0A[2];
} SBTLSKILLHIT_POSWORK;

typedef struct SBTLSKILLIDOU_WORK {
    f32 per;
    f32 w;
    f32 h;
    u8 pad0C[4];
} SBTLSKILLIDOU_WORK;

typedef struct SBTLDMG_CYLINDERWORK {
    f32 scale;
    f32 height;
    f32 area_r;
    f32 xrad;
} SBTLDMG_CYLINDERWORK;

/* One attack hit record as stored in a hit file (HD names). */
typedef struct SBTLSKILLHIT_TBL {
    u32 flg; /* SBTLDMG_HITFLG; 0x1000: BpData follows in the file, 0x2000: EpData and time follow */
    s8 type;
    u8 pad05[3];
    s32 starttime;
    s32 lifetime;
    f32 x;
    f32 y;
    s8 targetflg;
    u8 pad19;
    u16 area_type;
    f32 area_r2;
    f32 area_r;
    f32 rad;
    f32 rad_area;
    f32 homingpara;
    f32 z;
    f32 dir;
    f32 sokudo;
    f32 kasokudo;
    SBTLSKILLHIT_POSWORK start;
    SBTLSKILLHIT_POSWORK end;
    SBTLSKILLIDOU_WORK apos1;
    SBTLSKILLIDOU_WORK apos2;
    s8 muki;
    u8 pad79[3];
    f32 shatei;
    s32 motionno;
    SBTLDMG_CYLINDERWORK cy;
} SBTLSKILLHIT_TBL;

typedef struct SBTL_HIT_PACFILE {
    s32 num;
    SBTLSKILLHIT_TBL entry[1];
} SBTL_HIT_PACFILE;

/* Effect timing copied after a hit record (HD SBTLEFF_EFFTIME). */
typedef struct SBTLEFF_EFFTIME {
    f32 counter;
    f32 st_time;
    f32 life;
    f32 fin_time;
    f32 fout_time;
} SBTLEFF_EFFTIME;

/* 200-byte allocation holding one hit record and the data blocks that follow it in the file. */
typedef struct SBTL_HIT_PACDATA {
    void *BpData;
    struct sdfMemHandle_t *handle;
    struct sdfMemHandle_t *handle08; /* BpData */
    u8 pad0C[4];
    struct sdfMemHandle_t *handle10; /* EpData */
    struct plistHandle_t *hPartsList;
    s32 BpSize;
    void *EpData;
    SBTLSKILLHIT_TBL entry;
    SBTLEFF_EFFTIME time;
} SBTL_HIT_PACDATA;

#define SBTL_HIT_PACDATA_MAX 16

/* Attack hit records of one model; also held per unit in SBTLUNIT_WORK.hitwk. */
typedef struct SBTL_PACDATA_HITLIST {
    u8 pad00[4];
    struct fileRequest *hFile;
    u8 pad08[4];
    s32 num;
    s16 gstat;
    u16 status;
    SBTL_HIT_PACDATA *pwk[SBTL_HIT_PACDATA_MAX];
} SBTL_PACDATA_HITLIST;

/* Attacker data copied into a hit (HD SBTLCALC_DMGWORK_A). */
typedef struct SBTLCALC_DMGWORK_A {
    u8 aside; /* SBTLUNIT_SIDE */
    u8 pad01;
    s16 adevilid;
    s16 aListID;
    s16 alevel;
    s32 aatk;
    s32 atec;
    f32 ahp;
    f32 amaxhp;
    s32 gun_power;
    s32 sword_power;
} SBTLCALC_DMGWORK_A;

/* Path of a moving hit (HD sbtldmg_idouwork_tag). */
typedef struct SBTLDMG_IDOUWORK {
    sdfVector4_t apos0;
    sdfVector4_t apos1;
    sdfVector4_t apos;
    f32 apos0_per;
    f32 apos1_per;
    f32 apos0_w;
    f32 apos0_h;
    f32 apos1_w;
    f32 apos1_h;
    f32 time;
    f32 addtime;
} SBTLDMG_IDOUWORK;

/* One live hit volume, 0x23C bytes (HD names). */
typedef struct SBTLDMG_HITWORK {
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t oldpos;
    sdfVector4_t endpos;
    SBTLDMG_IDOUWORK iwk;
    u8 pad090[0x100 - 0x90];
    struct sdfMemHandle_t *handle;
    u32 flg; /* SBTLDMG_HITFLG */
    s32 serialID;
    s32 serialID2;
    s16 actioncnt;
    u8 pad112[2];
    s32 actionf;
    s32 acterID;
    s32 actID;
    s32 eListID;
    SKLID skillid;
    s32 workID;
    s32 targetID;
    u16 acterside; /* SBTLUNIT_SIDE */
    u16 side;      /* SBTLUNIT_SIDE */
    s8 tchangeflg;
    u8 pad135[3];
    s32 st;
    s32 counter;
    s32 starttime;
    s32 life;
    f32 area_r;
    f32 area_r2;
    f32 area_rt;
    SBTLDMG_CYLINDERWORK cy;
    f32 spd;
    f32 sokudo;
    f32 kasokudo;
    f32 shatei;
    f32 homingpara;
    f32 dir;
    SBTLSKILLHIT_POSWORK start;
    SBTLSKILLHIT_POSWORK end;
    SBTLSKILLHIT_TBL *hittbl;
    f32 x;
    f32 y;
    f32 z;
    u8 BEDtype;
    s8 hittype;
    s8 targetflg;
    u8 pad1A7;
    u32 skillflg;
    u16 type;
    u16 area;
    u16 atype;
    u8 pad1B2[2];
    f32 rad;
    f32 rad_area;
    u32 attr_flg;
    f32 hitbk_cnt;
    f32 hitbk_rate;
    struct plistHandle_t *hPartsList;
    f32 EPL_counter;
    f32 EPL_st_time;
    f32 EPL_fin_time;
    f32 EPL_life;
    f32 EPL_fout_time;
    f32 EPL_alpha;
    f32 EPL_in_alpha;
    f32 EPL_out_alpha;
    u32 abgr; /* HD SBTLDMG_HITWORK.abgr */
    struct mdlObj_t *hModel;
    struct dds3Basic_t *pBasic;
    struct evtUnit_t *hEveUnit;
    s8 pathflg;
    s8 fKaraburi;
    s8 f1St;
    u8 pad1FF;
    SBTLCALC_DMGWORK_A awk;
    f32 acterpos[3];
    struct SBTLUNIT_LISTWORK *lwk;
    struct SBTLUNIT_LISTWORK *hitlwk;
    struct SBTLDMG_HITWORK *prev;
    struct SBTLDMG_HITWORK *next;
} SBTLDMG_HITWORK;

/* Skill hit parameters (HD sBtlDMG SBTLSKILL_TBL). */
typedef struct SBTLSKILL_TBL {
    u16 type;
    u16 area;
    s32 mp;
    s32 mag;
    s32 starttime;
    s32 lifetime;
    s32 loopcnt;
    s32 looptime;
    s8 targetflg;
    u16 area_type;
    u32 flg;
    f32 dis;
    f32 area_r;
    f32 rad;
    f32 rad_area;
    u32 attr_flg;
    f32 hitbk_cnt;
    f32 hitbk_rate;
} SBTLSKILL_TBL;
SIZE_CHECK(sbtlSkillTblSizeCheck, SBTLSKILL_TBL, 0x40);

/* Damage number animation. */
#ifdef VERSION_RAIDOU2
typedef struct SBTLDMG_ANIMWORK {
    sdfVector4_t pos;
    struct sdfMemHandle_t *handle;
    s16 ID;
    s16 st;
    u32 rgba;
    u32 rgba2;
    s32 counter;
    s32 counter_pos;
    s32 dmg;
    s32 dmg_anm;
    s32 dmg_anm_add;
    s32 field34;
    s16 time;
    s16 cmptime;
    s32 Atk_ListID;
    s32 Dmg_ListID;
    s32 side; /* 0, 1 or 2: sbtlDamagePanelR, N or E */
    s32 minor;
    s32 rgbano;
} SBTLDMG_ANIMWORK;

SIZE_CHECK(sbtlDmgAnimWorkSizeCheck, SBTLDMG_ANIMWORK, 0x50);
#else
typedef struct SBTLDMG_ANIMWORK {
    sdfVector4_t pos;
    struct sdfMemHandle_t *handle;
    s16 ID;
    s16 st;
    u32 rgba;
    s32 counter;
    s32 dmg;
    s32 dmg_anm;
    s16 time;
    s16 cmptime;
} SBTLDMG_ANIMWORK;
#endif

typedef struct SBTLDMG_LISTWORK {
    s32 num;
    struct sdfMemHandle_t *handle;
    SBTLDMG_ANIMWORK **unit; /* SBTLDMG_ANIM_MAX entries, locked from handle */
} SBTLDMG_LISTWORK;

#define SBTLDMG_ANIM_MAX 40

typedef struct SBTLDMG_HITLISTWORK {
    s32 num;
    s32 serialID;
    s32 serialID2;
    struct SBTLDMG_HITWORK *wk;
} SBTLDMG_HITLISTWORK;

void UnsetKaraburiSub(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);
void sbtlAddSerialID2(void);
/* flg is unused; retail callers pass 0. */
s32 sbtlCallBtlCheckALLSkill(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, s32 flg);
void sbtlAnalyzeAisyo(struct SBTLUNIT_WORK *wk, s32 mode, sdfVector4_t *pos);
SBTLDMG_LISTWORK *sbtlAnimListProc(void);
s32 sbtlCHitFunc_ATTACK(struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, struct SBTLDMG_HITWORK *twk, struct SBTLSKILL_TBL *tbl);
s32 sbtlCalcVibe(void);
#ifdef VERSION_RAIDOU2
/* the aura value when wk has an aura whose kind is in kindFlg, else -1 */
s32 sbtlIsAura(struct SBTLUNIT_WORK *wk, s32 kindFlg);
s32 sbtlGetCorrectionParamLuck(void);
#endif
struct SBTLUNIT_WORK *sbtlCheckALLSkill(struct SBTLUNIT_WORK *wk, s32 mode);
s32 sbtlCallColorSet3(void);
s32 sbtlCheckDamageTarget_Skilltbl(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *wk, struct SBTLSKILLP_TBL *ptbl, SKLID skillid, s32 flg);
s32 sbtlCheckHitArea(struct SBTLDMG_HITWORK *wk, SKLID skillid);
s32 sbtlCheckPlayerDead(void);
s32 sbtlCheckActDamage(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock);
void sbtlSetMukouDamage(struct SBTLUNIT_WORK *wk, s32 flg);
u16 sbtlSetDamage(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 damage);
s32 sbtlGetStartTime_Damage(void);
s32 sbtlSetHitback(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, u32 attr);
void sbtlCallBtlSetMotionSBtlUnit4(struct SBTLUNIT_WORK *wk, s32 damage);
#ifdef VERSION_RAIDOU1
s32 sbtlCreateAnim_Unit(struct SBTLUNIT_WORK *wk, s32 damage, s16 time, s32 mode);
#else
s32 sbtlCreateAnim_Unit(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *atk_wk, s32 damage, s16 time, s32 mode);
#endif
s32 GetDamage(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 acterID, struct SBTLDMG_HITWORK *hwk, s32 attr);
s32 sbtlCallBtlGetPlayerData4(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, SKLID skillid, s32 attr);
#ifdef VERSION_RAIDOU1 /* Raidou 2's sbtlRefSbtl68 is other code: a distance without the camera depth */
s32 sbtlCheckSpecialDamage_Stop(struct SBTLUNIT_WORK *wk);
#else
f32 sbtlRefSbtl68(sdfVector4_t *pos0, sdfVector4_t *pos1);
#endif
s32 sbtlCallCheckBadStatus(struct SBTLUNIT_WORK *acter, struct SBTLUNIT_WORK *wk, SKLID skillid, s32 flg);
s32 CheckAutoSkill(struct SBTLUNIT_WORK *wk);
#ifdef VERSION_RAIDOU2
void datSetKillDevil(struct SBTLUNIT_WORK *wk);
void sbtlSetAddMag_Setsuma(struct SBTLUNIT_WORK *wk, s32 mag);
u16 sbtlGetDevilHuumaItem(s32 devilId);
PLAYER_LUCK sbtlGetPlayerLuckStep(void);
void sbtlRefDDS3GBWK6(s32 devilId, s32 num);
struct SBTLUNIT_WORK *sbtlGetPChara(void);
s32 sbtlIsDeadBgn(struct SBTLUNIT_WORK *wk);
s32 sbtlIsEnemyLastOne(struct SBTLUNIT_WORK *wk);
s32 sbtlCheck(struct SBTLUNIT_WORK *wk, s16 type, s16 skillid);
s32 sbtlCheckDeadEventMag(struct SBTLUNIT_WORK *wk);
SKLID sbtlChkHaveMemSklParty(SKLID id);
void gohFunc6(s32 devilId);
void gohKarteRegistDevilBtl(struct sdatUnitWork_t *unit, u32 *karte, s32 a2, s32 a3);
void sbtlSetDamage_L_EnemyAll(void);
void sbtlCallBtlCreateSideListSBtlUnit4(void);
#endif
s32 sbtlGetDamageSub2(struct SBTLUNIT_WORK *wk, s32 skillid);
s32 sbtlSetRirituNakamaHD(struct SBTLEXP_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 id, s32 time, u32 bstf);
s32 sbtlSetDeadDamage(struct SBTLUNIT_WORK *wk, s32 flg);
void sbtlSetSpecialDamage_DamageOnly(struct SBTLUNIT_WORK *wk);
s32 sbtlCallMemset15(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk);
s32 sbtlCallMemAllocDirect(s32 dmg, const sdfVector4_t *pos, s32 type, s16 cmptime);
void sbtlSetStoneDeadDamage(struct SBTLUNIT_WORK *wk, s32 damage);
#ifdef VERSION_RAIDOU2
s32 sbtlBtlSetDamageProcSBtlDMGSub(SKLID skillid);
s32 sbtlCallBtlGetStartTimeDamage(struct SBTLUNIT_WORK *wk, s32 attr);
void sbtlCallBtlCreateAnimUnit2(struct SBTLUNIT_WORK *wk, s32 damage);
/* flg is unused; retail callers pass 0. */
s32 sbtlSEISHINCNTR(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, s32 flg);
void sbtlDltHit_by_UnitSkl(struct SBTLDMG_HITLISTWORK *lwk, s32 unitID, SKLID sklID);
#endif
s32 CheckDamageTarget(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *hwk, SKLID skillid);
s32 sbtlCheckGround_sbtlDMG(struct SBTLDMG_HITWORK *hwk, s32 *hitflg);
s32 sbtlCallBtlForceDeleteSerialID(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk, s32 *hitflg);
struct SBTLUNIT_WORK *sbtlCheckGround(struct SBTLDMG_HITWORK *hwk);
s32 sbtlCheckHitArea_sbtlDMG(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk);
s32 sbtlCheckTargetList_csr(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlClearHitC(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlCreateHitC_None(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *acter, struct SBTLDMG_HITWORK *twk, SKLID skillid, struct SBTLSKILLP_TBL *tbl, const sdfVector4_t *pos, s32 serialID2);
s32 sbtlCreateHitC_New(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, s32 acterID, SKLID skillid, struct SBTLSKILLP_TBL *tbl, struct SBTLSKILLHIT_TBL *hittbl, struct SBTL_HIT_PACDATA *pwk, u16 acterside, s32 serialID2);
#ifdef VERSION_RAIDOU2
/* raidou2 symbol name of sbtlCreateHitC_New. */
s32 sbtlCallMemset6(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLUNIT_WORK *wk, struct SBTLEFF_WORK *ewk, s32 acterID, SKLID skillid, struct SBTLSKILLP_TBL *tbl, struct SBTLSKILLHIT_TBL *hittbl, struct SBTL_HIT_PACDATA *pwk, u16 acterside, s32 serialID2);
#endif
SBTLDMG_HITLISTWORK *sbtlCreateHitList(void);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Normal(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlCreateTargetList_csr(struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlDeleteAnimList(SBTLDMG_LISTWORK *lwk);
s32 sbtlDeleteHitList(struct SBTLDMG_HITLISTWORK *wk);
s32 sbtlDrawAttachEPL(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlGetHit_OnlyEndPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_TBL *hittbl, struct SBTLSKILLHIT_POSWORK *pwk_st, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, u16 tbltype);
s32 sbtlGetKaraburiNum(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);
SBTLUNIT_SIDE sbtlGetSide(struct SBTLUNIT_WORK *wk, u16 area, u16 acterside, s8 *flg);
s32 sbtlHitCProc(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlHitCProc_Idou(struct SBTLDMG_HITLISTWORK *lwk);
s32 sbtlInitLoadAttackHitData_pData(void);
s32 sbtlForceDelete_AttackHitData(void);
s32 sbtlLoadAttackHitData(struct SBTLUNIT_WORK *wk, s32 id, void *ppData);
s32 sbtlSearchAttackHitData_Copy(struct SBTLUNIT_WORK *wk);
s32 sbtlLoadAttackHitData_Decode(struct SBTLUNIT_WORK *wk);
s32 sbtlLoadAttackHitData_pData(s32 index, void *pData);
s32 sbtlBtlLoadAttackHitDataDecodeSub(struct SBTLUNIT_WORK *wk, s32 id);
s32 sbtlRegistAnimList(SBTLDMG_LISTWORK *lwk, s32 id);
s32 sbtlReleaseAnim_ID(SBTLDMG_LISTWORK *lwk, SBTLDMG_ANIMWORK *wk);
s32 sbtlSetDamageProc(struct SBTLDMG_HITWORK *hwk, u32 attr);
void sbtlCallSprintf8(const sdfVector4_t *pos, s32 dmg, u32 rgba);
s32 sbtlReleaseHitC_List(struct SBTLDMG_HITLISTWORK *lwk, u8 flg);
s32 sbtlResetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype);
void sbtlResetIdouPara(struct SBTLDMG_HITWORK *hwk);
s32 sbtlSearchDMG_Unit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wk);
struct SBTLDMG_HITWORK *sbtlSearchHit_SerialID(struct SBTLDMG_HITLISTWORK *lwk, s32 serialID);
s32 sbtlSetApos12(const sdfVector4_t *startpos, const sdfVector4_t *endpos, const sdfVector4_t *rot, struct SBTLSKILLHIT_TBL *hittbl, sdfVector4_t *apos1, sdfVector4_t *apos2);
s32 sbtlSetEntryUnitList_Hit(struct SBTLDMG_HITWORK *hwk, struct SBTLUNIT_WORK *wklock);
s32 sbtlSetHitEndPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_TBL *hittbl, struct SBTLSKILLHIT_POSWORK *pwk_st, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, u16 tbltype);
s32 sbtlSetHitPos(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, sdfVector4_t *rott, u16 tbltype);
s32 sbtlSetHitPos_XYZ(struct SBTLUNIT_WORK *wk, struct SBTLDMG_HITWORK *twk, struct SBTLEFF_WORK *ewk, struct SBTLSKILLHIT_POSWORK *pwk, sdfVector4_t *pos, sdfVector4_t *rot, u16 tbltype);
void sbtlUnsetKaraburi(struct SBTLDMG_HITLISTWORK *lwk, struct SBTLDMG_HITWORK *hwk);
s32 sbtlSetDeadDamageProc(struct SBTLUNIT_WORK *wk, struct SBTLUNIT_WORK *wklock, struct SBTLDMG_HITWORK *hwk, s32 damage, u32 attr);


/* Unit that holds the hit data loaded once from the player's data (HD playerhit). */
extern struct SBTLUNIT_WORK sbtlWork2;
extern f32 sbtlHitCProc_IdouWork;

#endif
