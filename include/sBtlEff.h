#ifndef SBTLEFF_H
#define SBTLEFF_H

#include "common.h"
#include "enums.h"
#include "sdf_types.h"

struct SBTLEFF_DELLISTWORK;
struct SBTLEFF_LISTWORK;
struct SBTLEFF_WORK;
struct SBTLUNIT_WORK;
struct SBTLUNIT_LISTWORK;

struct plistHandle_t;
struct sdfMemHandle_t;
struct fileRequest;
struct SBTL_HIT_PACDATA;

/* Effect placement data of one pack entry (52 bytes in R1). Partial layout. */
typedef struct EFFPDATA_WORK {
    u8 type; /* SBTLEFF_BEDTYPE; retail reads it with lbu */
    u8 pad01[0x4 - 0x1];
    s32 eplno;
    f32 starttime;
    f32 life;
    f32 fadeout_time;
    u8 iti;
    u8 pad15[0x3];
    s32 iti_no;
    s8 takasa;
    s8 muki;
    u8 pad1E[0x28 - 0x1E];
    f32 scale;
    s32 flg; /* SBTLEFF_FLG */
    u8 pad30[0x34 - 0x30];
} EFFPDATA_WORK;

/* One effect of a loaded pack (0xA0 bytes). */
typedef struct SBTLEFF_PACDATAWORK {
    struct sdfMemHandle_t *handle; /* pData */
    u8 pad04[4];
    struct sdfMemHandle_t *selfHandle;
    void *pData;
    struct plistHandle_t *hPartsList;
    s32 flg;
    EFFPDATA_WORK wk;
    u8 pad4C[0x58 - 0x4C];
    s32 hitnum;
    u8 pad5C[0x60 - 0x5C];
    struct SBTL_HIT_PACDATA *hit[16];
} SBTLEFF_PACDATAWORK;

#define SBTLEFF_PACDATA_MAX 16
#ifdef VERSION_RAIDOU2
#define SBTLEFF_PACLIST_MAX 150
#else
#define SBTLEFF_PACLIST_MAX 48
#endif

/* Effect pack list entry (0x5C bytes, 0x60 in Raidou 2), SBTL_WORK2.effpac. */
typedef struct SBTLEFF_PACLIST {
    struct sdfMemHandle_t *handle;
    struct fileRequest *file;
#ifdef VERSION_RAIDOU2
    struct fileRequest *pacHandle; /* pacLoad */
#endif
    void *pData;
    s32 num;
    s16 gstat;
    u16 status; /* EFFPAC_ST */
    s16 skillID;
    u8 pad16[0x2];
    u32 SMGID;
    SBTLEFF_PACDATAWORK *pwk[SBTLEFF_PACDATA_MAX];
} SBTLEFF_PACLIST;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(SBTLEFF_PACLISTSizeCheck, SBTLEFF_PACLIST, 0x60);
#else
SIZE_CHECK(SBTLEFF_PACLISTSizeCheck, SBTLEFF_PACLIST, 0x5C);
#endif

typedef struct SBTLEFF_LISTWORK {
    s32 serial;
    s32 serialID2;
    s32 num;
    struct SBTLEFF_WORK *wk;
} SBTLEFF_LISTWORK;

typedef struct SBTLEFF_DELWORK {
    struct sdfMemHandle_t *handle;
    SBTLEFF_DELST gstat;
    SKLID sklid;
    s32 GroupID;
    SBTLUNIT_SIDE side;
    struct SBTLEFF_DELWORK *prev;
    struct SBTLEFF_DELWORK *next;
} SBTLEFF_DELWORK;

typedef struct SBTLEFF_DELLISTWORK {
    s32 serial;
    s32 num;
    SBTLEFF_DELWORK *dwk;
} SBTLEFF_DELLISTWORK;

/* Effect data handles, inline in SBTLEFF_WORK. */
typedef struct SBTLEFF_DATAWORK {
    void *pData;
    struct sdfMemHandle_t *hData;
    void *hFile;
    struct plistHandle_t *hPartsList;
} SBTLEFF_DATAWORK;

/* One live effect (0x12C bytes, 0x134 in R2). Partial layout, HD names. */
typedef struct SBTLEFF_WORK {
    sdfVector4_t pos;
    sdfVector4_t ppos;
    sdfVector4_t rot;
    u8 pad030[0xA0 - 0x30];
    struct sdfMemHandle_t *handle;
    s32 acterID;
    s32 targetID;
    s32 effectid; /* EFFID */
    s32 eListID;
    s32 hitserialID;
    s32 workID;
    s32 serialID2;
    s32 alpha;
    s32 alpha_cnt;
    u32 rgba;
    u8 pad0CC[0xD0 - 0xCC];
    s32 flg; /* SBTLEFF_FLG */
    u8 pad0D4[0xD8 - 0xD4];
    f32 scale;
    u8 pad0DC[0xE4 - 0xDC];
    s32 st; /* SBTLEFF_ST */
    s32 counter;
    s32 cmpcounter;
    s32 starttime;
    s32 life;
    s32 fin_time;
    s32 fout_time;
    s32 helperno; /* SBTLUNIT_MARK */
    SKLID BEDsklid;
    u8 BEDtype; /* SBTLEFF_BEDTYPE */
    u8 pad109[0x10C - 0x109];
    s32 BEDno;
    u8 BEDacterside; /* SBTLUNIT_SIDE bits */
    u8 pad111[0x114 - 0x111];
    SBTLEFF_DATAWORK dwk;
#ifdef VERSION_RAIDOU2
    u8 pad124[0x128 - 0x124]; /* one of HD's pHitEff and sklid */
#endif
    struct SBTLEFF_WORK *prev;
    struct SBTLEFF_WORK *next;
#ifdef VERSION_RAIDOU2
    u8 unk130;
    u8 pad131[0x134 - 0x131];
#endif
} SBTLEFF_WORK;
#ifdef VERSION_RAIDOU2
SIZE_CHECK(SBTLEFF_WORKSizeCheck, SBTLEFF_WORK, 0x134);
#else
SIZE_CHECK(SBTLEFF_WORKSizeCheck, SBTLEFF_WORK, 0x12C);
#endif

typedef struct sbtlFileEntry_t {
    char *path;
    struct {
        void **dst;
        s32 arg;
    } slot[18];
} sbtlFileEntry_t;

extern sbtlFileEntry_t sbtlFunc2Data[10];

void sbtlCallFileOpen(sbtlFileEntry_t *entry);
s32 sbtlCallFreeAuto(void **dst, s32 size, s32 *src);

s32 sbtlLoadEffectData_Nakama(struct SBTLUNIT_WORK *wk);
s32 EffFunc_DEADDAMAGE(struct SBTLEFF_WORK *wk);
s32 EffFunc_Hanyou(struct SBTLEFF_WORK *wk);
s32 EffFunc_Hanyou_BED(struct SBTLEFF_WORK *wk);
s32 EffFunc_NMLDAMAGE(struct SBTLEFF_WORK *wk);
s32 EffFunc_Slip_G(struct SBTLEFF_WORK *wk);
f32 GetDataScale(struct SBTLUNIT_WORK *wk, SKLID sklid);
void LoadOnMemoryData_BED(struct SBTLEFF_WORK *wk, struct SBTLUNIT_WORK *wklock, u16 no);
s32 SetEffectPos(struct SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk);
struct SBTLEFF_LISTWORK *sbtlCallColorSet5(void);
s32 sbtlCheckDelWork_Request(struct SBTLEFF_DELLISTWORK *lwk, s32 GroupID);
s32 sbtlCheckDeleteEffect(struct SBTLEFF_WORK *wk);
struct SBTLEFF_DELLISTWORK *sbtlCreateDelList(void);
s32 sbtlCreateDelWork(struct SBTLEFF_DELLISTWORK *lwk, SKLID skillid, SBTLUNIT_SIDE side, s32 GroupID);
s32 sbtlDelProc(struct SBTLEFF_DELLISTWORK *lwk);
s32 sbtlDeleteDelList(struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteEffList(struct SBTLEFF_LISTWORK *lwk);
s32 sbtlDeleteEffectData_SerialID2(struct SBTLEFF_LISTWORK *lwk, s32 serialID);
s32 sbtlForceReset(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlDeleteEffectData_SkillID(struct SBTLEFF_LISTWORK *lwk, SKLID skillid);
s32 sbtlDeleteOnData(s32 flg);
s32 sbtlFadeOut_BadStatusEffect(struct SBTLUNIT_WORK *wk, s32 alpha);
s32 sbtlForceDelete_EffectWork(s8 flg);
s32 sbtlForceDelete_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
s32 sbtlForceDelete_ID_Acter(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, EFFID effectid);
s32 sbtlForceDelete_ID_SummonFR(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk);
s32 sbtlForceDelete_SPID(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID skillid);
s32 sbtlForceDelete_SerialID(struct SBTLEFF_LISTWORK *lwk, s32 serialID);
s32 sbtlForceDelete_SkillFlag(u32 flg);
s32 sbtlForceResetDecAlpha(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 alpha);
s32 sbtlGetEffectSerialID2_Now(void);
s32 sbtlGetEffectStat_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
struct SBTLEFF_WORK *sbtlGetEffectWork_ID(struct SBTLEFF_LISTWORK *lwk, s32 eListID);
s32 sbtlInitEffectWork(void);
s32 sbtlLoadEffectData_ID(SKLID id);
s32 sbtlLoadEffectData_IDMask(SKLID id, EFFPAC_ST flg);
s32 sbtlLoadEndCheck_PlayerOnMemData(void);
s32 sbtlCallBtlLoadEffectDataIDMask(void);
#ifdef VERSION_RAIDOU2
/* R1 sbtlCallBtlLoadEffectDataIDMask, sbtlCallBtlGetSkilltbl10 and sbtlLoadEffectData_Nakama at the same call sites. */
s32 evtCommand_PMV_LOAD_END_WAIT(void);
s32 sbtlLoadEffectData(struct SBTLUNIT_WORK *wk);
s32 sbtlCallBtlSearchDelWorkSide(struct SBTLUNIT_WORK *wk);
#endif
s32 sbtlFunc(s32 id, void *pData);
s32 sbtlLoadPlayerOnMemData(void);
s32 sbtlReleaseEff(struct SBTLEFF_LISTWORK *lwk, u8 flg);
s32 sbtlResetAIPattern(struct SBTLUNIT_WORK *wk, u32 flg);
s32 sbtlSearchDelWork_Side(struct SBTLEFF_DELLISTWORK *lwk, SBTLUNIT_SIDE side);
s32 sbtlSearchEffectData(SKLID skillid);
s32 sbtlCheckEffectData(struct SBTLUNIT_WORK *wk, SKLID skillid, u8 flg, s8 type, u16 side, s32 id, s32 param);
void sbtlSetEffectData(struct SBTLEFF_WORK *ewk, struct SBTLUNIT_WORK *wk, SKLID sklid, s32 workID, u8 type, s32 no, u16 acterside,
                       s32 serialID2, s32 sttime);
s32 sbtlCallBtlCreateUnitList3(struct SBTLUNIT_WORK *wk, SKLID sklid, u8 type, s32 index, s32 id, u16 acterside, s32 serialID2,
                                s16 tflg, s32 sttime, s16 count);
s32 sbtlSearchEffect_ID(struct SBTLEFF_LISTWORK *lwk, EFFID effectid);
s32 sbtlSearchEffect_SAB_Acter(struct SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID BEDsklid, s32 BEDtype);
s32 sbtlSearchEffect_WorkID(struct SBTLEFF_LISTWORK *lwk, s32 workid);
s32 sbtlSetDecAlpha_SerialID(struct SBTLEFF_LISTWORK *lwk, s32 serialID, s32 alpha);
s32 sbtlSetDecAlpha_SerialID2(struct SBTLEFF_LISTWORK *lwk, s32 serialID2, s32 alpha);
s32 CheckEffectDataSingle(s32 type, s32 cmp);
s32 sbtlLeaf7(s32 id);
#ifdef VERSION_RAIDOU2
/* raidou2 returns the pack effect's wk. */
EFFPDATA_WORK *sbtlBtlSetEffectDataSub(s32 id, s32 no);
#else
SBTLEFF_PACDATAWORK *sbtlBtlSetEffectDataSub(s32 id, s32 no);
#endif
EFFPDATA_WORK *sbtlGetDmgEffScale(s32 id, s32 no);
s32 sbtlBtlCheckDeleteEffectSub(struct SBTLEFF_WORK *wk);
s32 sbtlBtlLoadEffectDataIDMaskSub(SKLID skillid);
void sbtlGetEffectData_Num(void);
s32 sbtlSearchEffect_SAB(struct SBTLEFF_LISTWORK *lwk, SKLID BEDsklid, s32 BEDtype);
s32 sbtlForceDelete_EffectWork_ID(SKLID id, s8 flg);
s32 CheckEffectData_Core(struct SBTLUNIT_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, SKLID sklid);
void Draw_sbtlEff(EFFID id, s32 *starttime, s32 *life);
s32 sbtlCallMemset18(SBTLEFF_PACLIST *pac, SKLID skillid, u16 flg, void *pData);
s32 sbtlGetNumOfEffect_SAB(struct SBTLEFF_LISTWORK *lwk, s32 BEDtype, u8 side);
SBTLEFF_WORK *sbtlCallMemAllocDirect3(SBTLEFF_LISTWORK *lwk, struct SBTLUNIT_WORK *wk, s32 id, s32 no);
s32 sbtlRefSbtl26(struct SBTLEFF_WORK *wk);

/* Per-effect process functions, indexed by SBTLEFF_WORK.effectid. */
extern s32 (*sbtlCallPadVibeFadeUpdateData[])(struct SBTLEFF_WORK *wk);
s32 sbtlEffSetActer(s32 id);
s32 sbtlEffSetTarget(s32 id);
extern struct SBTLUNIT_WORK *sbtleffbo_acter; /* acter set by sbtlEffSetActer */
extern struct SBTLUNIT_WORK *sbtleffbo_target; /* target set by sbtlEffSetTarget */

/* epl_otchkflg: set while drawing effects that take the OT check. */
extern u16 sbtlCallMemset17Data[]; /* SBTLEFF_FLG bits per EFFID */
extern s32 sbtlCallSetOtData;

#endif
