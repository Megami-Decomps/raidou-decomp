#ifndef SBTLUNIT_H
#define SBTLUNIT_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"
#include "sdatUnit.h"

struct SBTLUNIT_LISTWORK;

typedef struct SBTLUNIT_MODEL {
    u8 pad000[0x20];
    sdfVector4_t vec20;
    sdfVector4_t vec30;
    u8 pad40[0xF8 - 0x40];
    u32 flags;
    s32 field0FC;
    s32 field100;
    u8 pad104[0x11C - 0x104];
    f32 field11C;
    f32 field120;
    f32 field124;
    f32 field128;
    f32 field12C;
    f32 field130;
} SBTLUNIT_MODEL;

struct SBTLUNIT_WORK;

s32 CheckLiveTarget(struct SBTLUNIT_WORK *wk);
s32 CreateSTP(struct SBTLUNIT_WORK *wk, s32 id, s32 bframe, s32 aframe, s8 flg);
s32 Draw_sbtlUnit(void);
s32 Exit_sbtlUnit(void);
s32 Init_sbtlUnit(s8 flg);
s32 ResetAction(struct SBTLUNIT_WORK *wk, s8 flg);
s32 UnitMV(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlCheckDamageTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlChangeModelColor(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckExist2(struct SBTLUNIT_WORK *wk);
void sbtlCheckGuardMotion(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckModelDataLoadEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionEnd(struct SBTLUNIT_WORK *wk);
s32 sbtlCheckMotionNo(struct SBTLUNIT_WORK *wk, SBTLUNIT_MOTION no);
s32 sbtlCheckTateSkillSelectTarget(struct SBTLUNIT_WORK *wk);
s32 sbtlClearUnitList(struct SBTLUNIT_LISTWORK *wk);
struct SBTLUNIT_LISTWORK *sbtlCreateSideList(SBTLUNIT_SIDE side, s32 flg);
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

/* Partial layout: only the fields read so far are named, the rest is padding. */
struct SBTLUNIT_WORK {
    f32 visual[4];
    u8 pad010[0x8];
    s32 zero18;
    u8 pad01C[0xC];
    f32 visualA;
    u8 pad02C[0x14];
    sdfVector4_t pos;
    sdfVector4_t rot;
    sdfVector4_t rotBase;
    u8 pad070[0x30];
    sdfVector4_t posSaved;
    u8 pad0B0[0xF0];
    s32 field1A0;
    u16 mode;
    u8 pad1A6[0x2];
    sdatUnitWork_t unit;
    u8 pad2B8[0x8];
    s32 param[9];
    u8 pad2E4[0x8];
    struct SBTL_AISYO_TBL aisyoBase;
    u8 pad32C[0x74];
    s32 field3A0;
    u8 pad3A4[0x10];
    u8 visualKind;
    u8 pad3B5[0x1];
    s16 field3B6;
    u8 pad3B8[0xC];
    s32 field3C4;
    u8 pad3C8[0x16];
    s16 field3DE;
    u8 pad3E0[0x20];
    s32 field400;
    s16 field404;
    u8 pad406[0xE];
    u8 field414;
    u8 pad415[0x3];
    SBTLUNIT_FLG flg;
    u32 field41C;
    u32 flg2;
    s32 field424;
    f32 field428;
    f32 angleY;
    u8 pad430[0x8];
    s32 field438;
    u8 pad43C[0x14];
    SKLID skillid;
    u8 pad454[0x30];
    struct SBTLUNIT_MODEL *model;
    u8 pad488[0x4];
    s16 actCounter;
    s16 actMax;
    u8 pad490[0x40];
    struct sdfMemHandle_t *msgHandle;
    struct fileRequest *msgFile;
    void *msgData;
    s32 msgPos;
    s32 msgSize;
    s32 msgOffset;
    s16 msgState;
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
