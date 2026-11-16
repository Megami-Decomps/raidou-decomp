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

typedef struct SBTLUNIT_MODEL {
    u8 pad000[0x20];
    sdfVector4_t vec20;
    sdfVector4_t vec30;
    u8 pad40[0x90 - 0x40];
    sdfVector4_t scrPos3;
    u8 padA0[0xF4 - 0xA0];
    u16 mode;
    u8 pad0F6[0x2];
    u32 flags;
    s32 field0FC;
    s32 field100;
    s32 field104;
    u8 pad108[0x11A - 0x108];
    u16 field11A;
    f32 field11C;
    f32 field120;
    f32 field124;
    f32 field128;
    f32 field12C;
    f32 field130;
    u8 pad134[0x224 - 0x134];
    s32 field224;
} SBTLUNIT_MODEL;

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
s32 CreateSTP(struct SBTLUNIT_WORK *wk, s32 id, s32 bframe, s32 aframe, s8 flg);
s32 Draw_sbtlUnit(void);
s32 Exit_sbtlUnit(void);
s32 Init_sbtlUnit(s8 flg);
s32 ResetAction(struct SBTLUNIT_WORK *wk, s8 flg);
s32 sbtlCallFalseHuuma(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetNumOfEffectSAB(struct SBTLUNIT_WORK *wk, SKLID skillid, s32 after_frame);
s32 GuardProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode);
s32 CheckStop_BadStatus(struct SBTLUNIT_WORK *wk);
s32 CheckAction_Nakama(struct SBTLUNIT_WORK *wk);
s32 BeginSetsuma(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlGetPlayerUnit(s32 id);
s32 sbtlCallBtlGetBadStatus5(struct SBTLUNIT_WORK *wk);
s32 sbtlRefSbtl16(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlCreateUnitList2(struct SBTLUNIT_WORK *wk);
s32 sbtlCallMemset11(struct SBTLUNIT_WORK *wk, const sdfVector4_t *rot, s8 type, f32 range, f32 width, f32 width2);
s32 sbtlCheckArea(struct SBTLUNIT_WORK *wk, f32 r, const sdfVector4_t *pos);
void UnitMV(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos);
void sbtlCallBtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist2(struct SBTLUNIT_WORK *wk);
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
s32 sbtlCheckMapHit_Cross(const sdfVector4_t *pos0, const sdfVector4_t *ppos, f32 dis);
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
f32 sbtlGetMotionMove(struct SBTLUNIT_WORK *wk, f32 frame);
void sbtlCallBtlGetMotionVisualData3(struct SBTLUNIT_WORK *wk);
void sbtlCallPreCalcStrWidth(struct SBTLUNIT_WORK *wk);
void sbtlBtlSetMotionSBtlUnitSub(struct SBTLUNIT_WORK *wk);
s32 sbtlSetLight_Map(struct SBTLUNIT_WORK *wk, s32 frame, s32 flg);
void sbtlGetRotate(struct SBTLUNIT_WORK *wk, const sdfVector4_t *pos);
struct SBTLUNIT_WORK *sbtlSetSummon(s32 major, s32 minor, s32 index);
s32 sbtlCallBtlGetSkilltbl10(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetSklVisualTbl(struct SBTLUNIT_WORK *wk, SKLID skillid, u16 type, s32 a3, s8 a4);
s32 sbtlCallBtlGetDevilDataSBtlSub4(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlLoadAttackHitData(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetMotionVisualData2(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlCheckTateSkillSelectTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList(u8 side, s32 flg);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList_sbtlUnit(u8 side, s32 flg);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_MASK(u8 side, u32 mask, u32 mask_v);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_AI(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_Group(SBTLUNIT_SIDE side, s32 GroupID);
struct SBTLUNIT_LISTWORK *sbtlCreateTargetList_PForward(SBTLUNIT_SIDE side);
struct SBTLUNIT_LISTWORK *sbtlCreateUnitList(s32 number);
s32 sbtlDeleteUnit(struct SBTLUNIT_WORK *wk);
s32 sbtlEntryUnitList(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlGetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlGetNumUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
s32 sbtlGetNumUnit_GroupID2(struct SBTLUNIT_LISTWORK *lwk, s32 serialID, s32 flg);
s32 sbtlRefSbtl33(struct SBTLUNIT_WORK *wk);
s32 sbtlReleaseUnitList_ID(struct SBTLUNIT_LISTWORK *lwk, s32 id);
s32 sbtlResetSummon_E(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlSearchUnit_GroupID(struct SBTLUNIT_LISTWORK *lwk, s32 serialID);
struct SBTLUNIT_WORK *sbtlSearchUnit_ID(struct SBTLUNIT_LISTWORK *lwk, s32 ID);
s32 sbtlSearchUnit_Nakama(struct SBTLUNIT_WORK *wk);
s32 sbtlSetAIMode(struct SBTLUNIT_WORK *wk);
void sbtlSetEndBadStatus(void);
s32 sbtlSetEndData(void);
s32 sbtlSetFade(struct SBTLUNIT_WORK *wk, SBTLUNIT_VST flg, s32 start, s32 count, s32 para);
s32 sbtlSetLight_SetPos(struct SBTLUNIT_WORK *wk, s8 lightno);
s32 sbtlSetUnitLight(struct SBTLUNIT_WORK *wk, s32, u32 diffuse, u32 ambient, s32 flg, s32 flg2);
s32 sbtlSetUnitLight_Dir(struct SBTLUNIT_WORK *wk, const sdfVector4_t *dir, s32 flg);

/* sbtlSetLight_Map's copy of the map lights. */
extern mapLightParam_t sbtlSetLight_MapData;
/* Battle model file number per devil, indexed by minor. */
extern s32 sbtlCheckModelDataLoadEnd_sbtlUnitData[];

/* Motion number per SBTLUNIT_MOTION, one row per motion set. */
extern s32 sbtlBtlCheckMotionNoSubData[][56];

/* Partial layout: only the fields read so far are named, the rest is padding. */
struct SBTLUNIT_WORK {
    f32 visual[4];
    f32 field10;
    u8 pad014[0x4];
    f32 field18;
    u8 pad01C[0xC];
    f32 visualA;
    u8 pad02C[0x4];
    sdfVector4_t field30;
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t rotBase;
    sdfVector4_t nextpos;
    sdfVector4_t rotbk;
    u8 pad090[0x10];
    sdfVector4_t posSaved;
    u8 pad0B0[0x10];
    mapLightParam_t light;
    SBTLSOUND_DEVILWORK se;
    struct sdfMemHandle_t *handle;
    struct fileRequest *pacHandle;
    s32 GroupID;
    s32 field1A0;
    u16 mode;
    u8 pad1A6[0x2];
    sdatUnitWork_t unit;
    s32 haveskill[2];
    s32 param[9];
    u8 pad2E4[0x8];
    struct SBTL_AISYO_TBL aisyoBase;
    u8 pad32C[0x370 - 0x32C];
    s16 field370;
    u8 pad372[0x3A0 - 0x372];
    s32 major;
    s32 minor;
    s32 motionno;
    s32 motionID;
    f32 m_rate;
    u8 visualKind;
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
    u8 pad3D4[0x8];
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
    u8 pad406[0x2];
    struct dds3Basic_t *pBasic_d;
    struct evtUnit_t *hEveUnit_d;
    struct mdlObj_t *hModel_d;
    u8 field414;
    u8 field415;
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
    u32 status;
    u8 pad43C[0x14];
    SKLID skillid;
    u32 next_st;
    u8 pad458[0x460 - 0x458];
    s32 stockID; /* stock slot of a summoned demon, -1 otherwise */
    u8 pad464[0x474 - 0x464];
    s32 field474;
    u8 pad478[0xC];
    struct SBTLUNIT_MODEL *model;
    struct SBTLAI_WORK *aiwk;
    s16 actCounter;
    s16 actMax;
    u8 pad490[0x2];
    s16 field492; /* compared with a hit's actID when flg & 0x800 */
    u8 pad494[0x4];
    s16 field498;
    u8 pad49A[0x6];
    s16 field4A0;
    s16 field4A2;
    s32 damage_serialID;
    s32 damage_workID;
    u8 pad4AC[0x14];
    s32 bst_effserialID2;
    u8 pad4C4[0xC];
    struct sdfMemHandle_t *msgHandle;
    struct fileRequest *msgFile;
    void *msgData;
    s32 msgPos;
    s32 msgSize;
    s32 msgOffset;
    s16 msgState;
    u8 pad4EA[0x2];
    s32 field4EC;
    u8 pad4F0[0x3];
    u8 field4F3;
    SBTL_PACDATA_HITLIST hitwk;
};

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

#endif
