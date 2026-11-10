#include "common.h"

/* Declarations from smt3-decomp src/smt3/newbattle/nbKoukaProcess.c */
#include "common.h"

#include "newbattle.h"

#include "kernel.h"

#include "nb.h"

#include "event.h"

#include "ps2asm.h"

#include "fpu.h"

#include "sdfFrame.h"

/* nbKoukaPacket_t.type, as the kouka dispatchers switch on it. Names and
 * values from the IDB; the code corroborates them, with 39 and 40 calling
 * effPCPBossBUNSIN* and 41 and 42 calling effPCPBossONGY*. */
typedef enum NB_KOUKA_TYPE {
    SKILLEFF = 0,
    TARMOVE = 1,
    HOMEMOVE = 2,
    ROT = 3,
    HOMEROT = 4,
    UNITCOL = 5,
    UNITHOMECOL = 6,
    UNITA = 7,
    UNITHOMEA = 8,
    DMGMOT = 9,
    BADDMGMOT = 10,
    NB_KOUKA_TYPE_RETURN = 11,
    NB_KOUKA_TYPE_SUMMON = 12,
    SAKE = 13,
    SKILLCOLOFF = 14,
    HITEFF = 15,
    NUMEFF = 16,
    HPMP = 17,
    PRESS = 18,
    BADSTAT = 19,
    BADMOTKAIFUKU = 20,
    SKILLEFF2 = 21,
    MOJIEFF = 22,
    BADEFF = 23,
    BADEFFKAIFUKU = 24,
    SE = 25,
    SKILLEFF1 = 26,
    MOTION = 27,
    SE2 = 28,
    SELOCK = 29,
    HITSTOP = 30,
    NEWPRESS = 31,
    MOTIONSTOP = 32,
    MOTIONSTOPCLS = 33,
    ANALYZE = 34,
    JYOKYOHELP = 35,
    DEADPCM = 36,
    BADMOT = 37,
    FORMONOFF = 38,
    BOSSONGBUN = 39,
    BOSSONGBUN2 = 40,
    BOSSONGOUT = 41,
    BOSSONGOUT2 = 42,
    SKILLEFF3 = 43,
    FORMFLAGONOFF = 44,
    NB_KOUKA_TYPE_FLY = 45,
    DEADEVENT = 46,
    FADEOUT = 47,
    FADEIN = 48,
    SKILLNAME = 49,
    HOJO = 50,
    FLARECHARGE = 51,
    PADSHOCK = 52,
    EASYDMGMOT = 53,
    SKILLCOLON = 54,
    SKILLMOTION = 55,
    STKPNL_FOUT = 56,
    STKPNL_END = 57,
    HATUDOEFF_END = 58
} NB_KOUKA_TYPE;

typedef union
{
    s8 c[4];
    u8 uc[4];
    s16 s[2];
    u16 us[2];
    s32 i;
    f32 f;
    void *p;
} nbKoukaData_u;

typedef struct nbKoukaPacket_t
{
    s16 startframe;
    s16 framelength;
    s8 type;
    u8 flag;
    u16 time;
    s32 uniqueid;
    struct nbKoukaPacket_t *next;
    struct nbKoukaPacket_t *prev;
    nbFormation_t *form;
    datUnitWork_t *unit;
    struct nbParty_t *party;
    s32 partyno;
    struct effBTLCutHandle_t *cuteff;
    nbKoukaData_u data1;
    nbKoukaData_u data2;
    sdfVector4_t v;
} nbKoukaPacket_t;

extern dds3ProcessID_t *dds3SearchProcessName(char *namestr);

extern void *dds3GetProcessData(dds3ProcessID_t *id);

extern void *sdfChipAllocDirect(u32 aSize);

extern void sdfChipFreeDirect(void *aPtr);

extern void nbPanelStockFadeOUT(void);

extern void nbPanelStockEnd(void);

extern void dds3PadVibrationSet(s32 port, s32 power, s32 frame);

extern void dds3SlowOn(u32 aSlowTime, u32 slowcnt);

extern void dds3Stop(u32 frame);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 evtGetAgeOfMoon16(void);

extern void evtSetAgeOfMoon(s32 age);

extern void effBTLSkillEnd(void);

extern void effBTLSkillStart(s32 id, s32 stat);

extern void effBTLSystemDelete(struct effBTLSysHandle_t *hEffBTLSys);

extern void effBTLSystemDraw(struct effBTLSysHandle_t *hEffBTLSys);

extern struct effBTLSysHandle_t *effBTLSystemCreate(nbFormation_t *pFormation, s32 id);

extern void nbAddHpMp(s32 formindex, s32 type, s32 n);

extern void nbDispJyokyoHelp(s32 msgid, s32 no, s32 var_id, s32 var_ind);

extern void nbDispSkillName(s32 id);

extern void nbFormDispOff(s32 formindex);

extern void nbFormDispOn(s32 formindex);

extern nbMainProcessData_t *nbGetMainProcessData(void);

extern datUnitWork_t *nbGetUnitWorkFromFormindex(s32 formindex);

extern void nbPanelAnalyzeDraw(datUnitWork_t *pUnitWork);

extern void nbPanelDraw(s32 flag);

extern void nbReturnUnit(s32 partyno, s32 lost, s32 dead);

extern void nbRunKoukaType1(s32 partyindex, sdfVector4_t *pos, f32 time);

extern void nbRunKoukaType2(s32 partyindex, f32 time);

extern void nbSetDamage(nbFormation_t *form, f32 range, s32 week);

extern void nbSetDamage2(nbFormation_t *form, f32 range);

extern void nbSetBadstatMotion(nbFormation_t *form, s32 type, f32 range);

extern void nbSetBadstatMotion2(nbFormation_t *form);

extern void nbSetUnitMotion(nbFormation_t *form, s32 motion2, s32 mode, f32 ritu);

extern void nbSetWaitMotion(nbFormation_t *form, s32 motion, f32 ritu);

extern void nbSetBadStat(s32 formindex, s32 sw, s32 badstat);

extern s32 nbSetHojoKouka(s32 formindex, s32 hojotype, s32 hojopoint, s32 nvirtual);

extern void nbSetSakeKoukaParam(nbFormation_t *form, f32 range, f32 power);

extern void effBTLCutDelete(struct effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLDelete(void *hBTLEffect);

extern void effBTLSystemCut1Delete(struct effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLSystemDelete2(struct effBTLSysHandle_t *hEffBTLSys);

extern void effPCPBossBUNSINDelete(void *pPCPBossBUNSIN);

extern void effPCPBossBUNSIN2Delete(void *pPCPBossBUNSIN2);

extern void effPCPBossONGYDelete(void *pPCPBossONGY);

extern void effPCPBossONGY2Delete(void *pPCPBossONGY2);

extern void nbDelKoukaEntry(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void nbReturnKoukaUnit(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void effBTLCutDraw(struct effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLSystemCut1Draw(struct effBTLCutHandle_t *hBTLCutEffect);

extern struct effBTLCutHandle_t *effBTLCutCreate(void *hBTLEffect, u16 CutNum, nbFormation_t *pFormation,
                                                 s32 stat);

extern s32 evtSetUnitColorA(evtUnit_t *h, s32 frame, u32 a);

extern s32 evtSetUnitColorRGB(evtUnit_t *h, s32 frame, u32 rgb);

extern void evtSetWaitHokanFrame(evtUnit_t *h, s32 before, s32 smooth);

extern s32 p2sdLoadCheckMidi(s32 MidiNo);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern void p2sdStopSe(u32 sound);

extern void datDebugPrintf(const char *fmt, ...);

extern s32 p2sdGetPcmState(void);

extern s32 p2sdLoadPcmFile(char *name, s32 channel);

#include "sdk_libc.h"

extern char nbPcmLoadNameFormat[];

extern void dds3FadeIn2(s32 frame);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern u32 effBTLCutGetFrame(struct effBTLCutHandle_t *hBTLCutEffect);

extern void effBTLCutSetCutEndFrame(struct effBTLCutHandle_t *hBTLCutEffect, s32 frame);

extern void effPCPBossBUNSINDraw(void *pPCPBossBUNSIN);

extern void effPCPBossBUNSIN2Draw(void *pPCPBossBUNSIN2);

extern void effPCPBossONGYDraw(void *pPCPBossONGY);

extern void effPCPBossONGY2Draw(void *pPCPBossONGY2);

extern void nbClipKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void nbDrawKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void RunFlyKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void RunNumEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void RunStockPanelEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void RunStockPanelFadeOutKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void RunUnitColKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void nbAddOrder(s32 findex);

extern void nbSetEnemyFormation(nbMainProcessData_t *data, s32 group);

extern void nbSetPlayerFormation(nbMainProcessData_t *data, s32 empty);

extern void nbSummonUnit(s32 side, s32 stockno, s32 formindex);

extern void nbUpdateOrder(void);

extern nbKoukaProcessData_t *InitKoukaProcessData(nbMainProcessData_t *data);

extern void nbLinkPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void nbDelKoukaPacket(nbKoukaPacket_t *p);

extern s32 nbKoukaProcess1(dds3ProcessID_t *id);

extern s32 nbKoukaShutdown(dds3ProcessID_t *id);

nbKoukaProcessData_t *nbGetKoukaProcessData(void);

s32 nbGetKoukaPacketCnt(void);

s32 nbGetUniqueIdKoukaPacketCnt(saddr_t uniqueid);

s32 nbCountKoukaByType(s32 type);

void nbLinkPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbDelKoukaEntry(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

nbKoukaPacket_t *nbAddKoukaPacket(nbKoukaProcessData_t *k, s32 startframe, s32 framelength,
                                  s32 uniqueid);

void DelKoukaPacket(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbDelKoukaPacket(nbKoukaPacket_t *p);

void nbDelAllUniqueIDKoukaPacket(s32 uniqueid);

void nbDelAllKoukaPacketType(s32 type);

/* The action-ready caller supplies only form; native never consumes $a1. */
void nbDelAllMojiNumEffectKoukaPacket(nbFormation_t *form);

void nbDelUnEventKoukaPacket(void);

void nbOnOffKoukaPacketFlag(s32 type, u32 flg, s32 sw);

s32 nbCheckSePacket(s32 start, s32 se, s32 frame);

void RunBossEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunHatudoEffectEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSkillEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbRunKoukaCase1(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void ToMatrix(void);

extern void dds3RotationArc(void);

extern void evtSetUnitRot1(evtUnit_t *unit, s128 *rotation, s32 parameter);

extern void *memset(void *dst, s32 value, size_t size);

void RunNumEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunUnitColKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunMotionStopKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunMotionKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunDamageMotionKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbReturnKoukaUnit(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSummonKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSakeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunBadEffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunBadEffKaifukuKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunHitEffectKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void effBTLModelGetSphereCenter(nbFormation_t *form);

extern s32 effBTLSkillCheckRun(void);

extern s32 effBTLSkillGetID(void);

extern nbFormation_t *effBTLSkillGetInvokeFormation(void);

extern s32 effBTLModelGetID(nbFormation_t *form);

extern u32 effBTLSkillGetTargetNumber(void);

extern nbFormation_t *effBTLSkillGetTargetFormation(s32 number);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);

extern s32 nbClipEffectNum(s32 screen[4]);

/* The position is a hidden input/output in vf10, not packet->v. Both number
 * draw paths supply it before calling; all exits leave the selected world
 * position in vf10 for the caller's position-relative offset calculation. */
void NumEffectClip(nbKoukaPacket_t *p);

extern void nbGetKoukaMarkPos(nbFormation_t *form);

extern void NumEffectClip(nbKoukaPacket_t *packet);

extern s32 nbClipEffectNum(s32 screen[4]);

extern void nbDrawKoukaSprite(sdfTexHandle_t *th, u32 value, s32 x, s32 y, u32 col, s32 align);

void nbClipKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern void nbGetKoukaMarkPos(nbFormation_t *form);

extern void nbLoadKoukaPos(nbFormation_t *form);

extern void NumEffectClip(nbKoukaPacket_t *packet);

extern s32 nbClipEffectNum(s32 screen[4]);

extern void nbPnl_rsetTest(s32 ate, s32 atst, s32 aref, s32 afail, s32 date, s32 datm, s32 zte, s32 ztst);

extern void nbPnl_rsetAlpha(s32 mode);

extern void nbPnlSprite(s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 uw, s32 vh,
                        s32 color, sdfTexHandle_t *texture);

void nbDrawKoukaNum(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbApplyHpMp(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbRunKoukaCase12(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void nbApplyBadStat(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunBadMotKaifukuKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunPcmKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunHitStopKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunAnalyzeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunHelpKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunFormOnOffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunFormFlagOnOffKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

extern s32 nbCheckModelLoad(nbMainProcessData_t *data, s32 major, s32 minor);

extern s32 nbLoadMidiFile(s32 no);

extern void nbAddFormEntry(void *buffer, s32 group, s32 id);

extern void *sdfMemLock(sdfMemHandle_t *handle);

extern void datSetBadStat(datUnitWork_t *work, u32 badstat);

extern u32 datGetMaxHp(datUnitWork_t *work);

extern u32 datGetMaxMp(datUnitWork_t *work);

extern void dds3DeleteSceneBasicData(dds3Basic_t *basic);

extern s32 evtSetRegistUnit(s32 major, s32 minor);

extern dds3Basic_t *dds3GetActiveScene(void);

extern dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *basic, u32 serial, s32 type);

extern void dds3LinkLightObjectToUnitObject(dds3Basic_t *light, dds3Basic_t *unit, s32 frame);

#include "basic.h"

#include "dds3obj.h"

extern void mdlSetPosition(mdlObj_t *model);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask);

extern sdfVector4_t nbFlyKoukaRotation;

void RunFlyKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunDeadEventKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunFadeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSkillNameKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunHojoKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunFlareChargeKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunPadShockKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunSkillColorKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunStockPanelFadeOutKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

void RunStockPanelEndKouka(nbKoukaProcessData_t *k, nbKoukaPacket_t *p);

s32 nbKoukaProcess1(dds3ProcessID_t *id);

s32 nbKoukaShutdown(dds3ProcessID_t *id);

nbKoukaProcessData_t *InitKoukaProcessData(nbMainProcessData_t *data);

void nbInitKoukaProcess(nbMainProcessData_t *data);
/* End of declarations from src/smt3/newbattle/nbKoukaProcess.c */

void nbDelKoukaPacket(nbKoukaPacket_t *p)
{
    DelKoukaPacket(nbGetKoukaProcessData(), p);
}
