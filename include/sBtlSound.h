#ifndef SBTLSOUND_H
#define SBTLSOUND_H

#include "common.h"
#include "sdf_types.h"

/* Footstep frames of one devil's walk motion. */
typedef struct DVLFOOTSE_WORK {
    u8 frameNum;
    u8 sedataNum;
    u16 flg;
    f32 frame[4];
} DVLFOOTSE_WORK;

extern DVLFOOTSE_WORK *sbtlDevilFootSEProcSubData;

/* Per-SE entry; count goes up once per battle sound frame. */
typedef struct SBTLSOUND_SEENTRY {
    s32 count;
    s32 field04;
    s32 field08;
} SBTLSOUND_SEENTRY;

extern SBTLSOUND_SEENTRY sbtlWork[256];

#ifdef VERSION_RAIDOU2
/* Queued SE request, played later by the battle sound process. */
typedef struct SBTLSOUND_SEQUEUE {
    s32 active;
    s32 type;
    u32 seid;
    s32 vol;
} SBTLSOUND_SEQUEUE;

extern SBTLSOUND_SEENTRY sbtlWork10[256];
extern SBTLSOUND_SEQUEUE sbtlWork11[32];
extern s32 sbtlCallBtlSEVolAngleCalcData;
extern DVLFOOTSE_WORK sbtlDevilFootSEProcSubData2;
extern DVLFOOTSE_WORK *sbtlDevilFootSEProcSubData3;

void sbtlLeaf2(void);
void sbtlLeaf3(u32 seid, s32 type, s32 vol);
#endif

struct SBTLEFF_PACLIST;
struct SBTLUNIT_WORK;
struct SBTLSOUND_FOOTWORK;

void sbtlRefSbtl5(struct SBTLUNIT_WORK *wk);
void sbtlSummonDvTaskSub(struct SBTLSOUND_FOOTWORK *fwk);
#ifdef VERSION_RAIDOU2
void cmpLeaf3(struct SBTLUNIT_WORK *wk);
#endif
s32 NakamaMotion3DSE(struct SBTLSOUND_FOOTWORK *fwk, s32 devilID, f32 frame);
DVLFOOTSE_WORK *sbtlDevilFootSEProcSub(s32 devilID);
void sbtlBtlLoadSkillSESub(s32 arg);
s32 sbtlBtlPlay3DSEBaseVolSub(const sdfVector4_t *pos, f32 maxlen, f32 minlen, s32 basevol, s32 minvol);
void sbtlSEVolAngleCalc(u32 no, s32 vol);
u32 sbtlPlayMotionSENowSub2(s32 devilID, s32 motionID);
f32 sbtlPlayMotionSENowSub(struct SBTLUNIT_WORK *wk, s32 motionID);
f32 sbtlGetSETime(struct SBTLUNIT_WORK *wk, s32 motionID);
u32 sbtlPlayMotionSENowSub3(s32 devilID, s32 motionID);
void PlayMotionSENow(struct SBTLUNIT_WORK *wk, s32 motionID, s32 startcnt);
void sbtlCallEffectShutdown(struct SBTLUNIT_WORK *wk);
void sbtlCallEffectShutdown2(struct SBTLUNIT_WORK *wk);
s32 sbtlBtlForceDeleteEffectWorkIDSub(s32 arg);
void sbtlSoundatBossAt(s32 id, char *name);
void sbtlStopSkillSE(SKLID skillid, s32 type);
void sbtlPlaySkillSE(const sdfVector4_t *pos, SKLID skillid, s32 type);
void sbtlStopSE(u32 seid);
#ifdef VERSION_RAIDOU2
void sbtlPlayDeadVoice(struct SBTLUNIT_WORK *wk);
s32 sbtlSEVolAngleCalc_sbtlSound(const sdfVector4_t *pos, f32 maxlen, f32 minlen, s32 basevol, s32 minvol);
#endif
void sbtlRefSbtl6(struct SBTLUNIT_WORK *wk, u32 seid, s32 vol, s32 motionID);
u32 sbtlCallGetRandIntA4(struct SBTLUNIT_WORK *wk);
#ifdef VERSION_RAIDOU2
u32 sbtlGetDevilSESub(s32 devilID, s32 motionID);
u32 GetDevilSE(s32 devilID, s32 motionID);
#else
u32 sbtlGetDevilSESub(s32 devilID);
#endif
s32 sbtlCallBtlSearchPacData3(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetDevilSESub(struct SBTLUNIT_WORK *wk);
void DevilFootSEProc(struct SBTLUNIT_WORK *wk);
u32 sbtlCallGetRandIntA5(struct SBTLSOUND_FOOTWORK *fwk, s32 devilID);
u32 sbtlGetBossSE(struct SBTLUNIT_WORK *wk, s32 motionID);
s32 sbtlLoadSkillSE(struct SBTLEFF_PACLIST *ewk);
void sbtlRefSbtl7(void);
u32 sbtlCallBtlGetSkilltbl(s32 skillID, s32 type);
#ifdef VERSION_RAIDOU2
/* raidou1 names this function sbtlCallBtlGetSkilltbl. */
u32 GetSkillSE(s32 skillID, s32 type);
void sbtlStopSkillSE(SKLID skillid, s32 type);
#endif
void sbtlCheckBossSE(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlGetBadStatus(struct SBTLUNIT_WORK *wk);
void sbtlPlay3DSE_BaseVol(u32 no, const sdfVector4_t *pos, s32 flg);
void sbtlPlayDamageSE(const sdfVector4_t *pos, s32 damageType);
void sbtlPlaySpecialSE(const sdfVector4_t *pos, s32 specialType);

#endif
