#ifndef SBTLPLAYER_H
#define SBTLPLAYER_H

#include "common.h"
#include "enums.h"
#include "basic.h"
#include "sdf_types.h"

struct SBTLUNIT_WORK;
struct sBtlMenu_t;

typedef struct SBTLPLAYER_VIBE {
    s32 in;
    s32 life;
    s32 out;
    s32 power;
} SBTLPLAYER_VIBE;

/* One of the eleven sbtlData3..sbtlData13 entries that sbtlRefSbtl11 copies into SBTL_WORK 0x74C. */
typedef struct SBTL_REF11_DATA {
    u16 field00;
    u16 field02;
    u16 field04;
    u16 field06;
    u32 field08; /* low 16 bits: frames after the window opens; 0x10000: wait for them */
    u32 key[4]; /* GetAnlogKeyToVecAndDir input masks */
    s32 field1C;
} SBTL_REF11_DATA;

extern SBTL_REF11_DATA sbtlData3;
extern SBTL_REF11_DATA sbtlData4;
extern SBTL_REF11_DATA sbtlData5;
extern SBTL_REF11_DATA sbtlData6;
extern SBTL_REF11_DATA sbtlData7;
extern SBTL_REF11_DATA sbtlData8;
extern SBTL_REF11_DATA sbtlData9;
extern SBTL_REF11_DATA sbtlData10;
extern SBTL_REF11_DATA sbtlData11;
extern SBTL_REF11_DATA sbtlData12;
extern SBTL_REF11_DATA sbtlData13;
extern s32 *sbtlCallDeleteAllData[];

s32 DeleteAll(struct SBTLUNIT_WORK *wk);
s32 sbtlCallDeleteAll(struct SBTLUNIT_WORK *wk);
s32 GetAnlogKeyToVecAndDir(struct SBTLUNIT_WORK *wk, u32 key, s32 type);
s32 FalseHuuma(struct SBTLUNIT_WORK *wk);
s32 nakamamodeCallGetActiveScene(dds3PoseSet_t *pose, dds3Basic_t *basic);
s32 nakamamode_UnitPad(dds3PoseSet_t *pose, dds3Basic_t *basic);
s32 sbtlLeaf63(struct SBTLUNIT_WORK *wk);
void sbtlCallBtlResetMenuPara(void);
s32 sbtlCallBtlGetBadStatus2(struct SBTLUNIT_WORK *wk);
s32 ChkHuuma_PlayerInputOk(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetBadStatus3(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetBadStatus4(struct SBTLUNIT_WORK *wk);
#ifdef VERSION_RAIDOU2
s32 sbtlCallBtlGetNakama3(void);
void Check_Stls(struct SBTLUNIT_WORK *wk);
void sbtlRefSbtl11(struct SBTLUNIT_WORK *wk);
#else
s32 sbtlRefSbtl11(struct SBTLUNIT_WORK *wk, s32 type);
#endif
void sbtlRefSbtl18(struct SBTLUNIT_WORK *wk, s32 mode);
f32 GetAnlogKey(struct SBTLUNIT_WORK *wk, sdfVector4_t *dir);
s32 sbtlCallBtlCheckDamageTarget4(struct SBTLUNIT_WORK *wk);
s32 CheckHuuma_Moon(struct SBTLUNIT_WORK *wk);
s32 CheckHuuma2(struct SBTLUNIT_WORK *wk);
s32 sbtlCallGetAnlogKey(struct SBTLUNIT_WORK *wk, s32 input);
s8 sbtlNormalProc_Huuma(struct SBTLUNIT_WORK *wk);
void sbtlRefSbtl14(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchUnitID11(struct SBTLUNIT_WORK *wk, s32 flg);
s32 NormalProc(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlDrawProcessSub(struct sBtlMenu_t *menu);
s32 sbtlCheckBit(u32 *bits, s32 bit);
s32 sbtlCheckPad(void);
s32 sbtlClearEffHitData(struct SBTLUNIT_WORK *wk);
s32 sbtlGetRandomTarget(struct SBTLUNIT_WORK *wk, s32 *skill0, s32 *skill1);
s32 sbtlNormalProcNakamaSub(struct SBTLUNIT_WORK *wk);
void sbtlResetColorFlag(void);
void sbtlResetCharge(struct SBTLUNIT_WORK *wk);
struct SBTLUNIT_WORK *sbtlGetPlayer_HitTarget(void);
#ifdef VERSION_RAIDOU2
struct SBTLUNIT_WORK *sbtlRefSbtl10(s32 ID);
#else
s32 sbtlRefSbtl10(struct SBTLUNIT_WORK *wk);
#endif
void sbtlSetNSTOP(void);
extern const SBTLPLAYER_VIBE sbtlSetVibeTable[11];
void sbtlSetVibe(SBTLPLAYER_VTYPE mode);
void sbtlSetDevilID_EnemyRot(s32 _DevilID, f32 _Dir);
s32 sbtlCallBtlCheckDamageTarget3(struct SBTLUNIT_WORK *wk);
#ifdef VERSION_RAIDOU1
f32 sbtlCallBtlSearchUnitID6(f32 rate);
#endif
#ifndef VERSION_RAIDOU2
struct SBTLUNIT_WORK *sbtlCallMemset6(s32 index, s32 flg);
#endif
s32 sbtlCallCheckActionNakama(struct SBTLUNIT_WORK *wk);
s32 sbtlRefSbtl12(void);
s32 sbtlCallBtlSetReturn(struct SBTLUNIT_WORK *wk);
s32 sbtlSetReturn(struct SBTLUNIT_WORK *wk);
s32 sbtlActiveAction(void);
s32 sbtlCheckUnit_Exec_R(void);
s32 sbtlCallLightSet(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchUnitID8(struct SBTLUNIT_WORK *wk);
s32 sbtlCallMemset10(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetSkilltbl2(struct SBTLUNIT_WORK *wk, s16 *mode);
s32 sbtlPlayerRKaitenInit(struct SBTLUNIT_WORK *wk, s16 *mode);
s32 NormalProc_Nakama(struct SBTLUNIT_WORK *wk);
s32 sbtlNormalProcNakamaSub2(s32 no);
s32 AtkProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode);
s32 KaihiProc_Nakama(struct SBTLUNIT_WORK *wk, s16 *mode);
#ifndef VERSION_RAIDOU2
s32 sbtlLeaf3(struct SBTLUNIT_WORK *wk, s16 *mode);
#endif

#endif
