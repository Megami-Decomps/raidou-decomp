#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldProcess.c */
#include "common.h"

#include "basic.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "newdata.h"

#include "field.h"

#include "effect.h"

#include "sdfFrame.h"

extern u32 dds3GetKernelCount(void);

extern s32 fldEncountCount;

extern u32 fldEncountTimer[2];

extern char dds3DebugLabelB[], dds3DebugLabelG[], dds3DebugLabelR[], dds3DebugFmtInt3[], dds3DebugFmtFloat3[];

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3SetActiveScene(dds3Basic_t *scene);

extern s32 evtBitCheck(s32 no);

extern void evtBitOff(s32 no);

extern void evtBitOn(s32 no);

extern s32 ProcRandomEncountCheck(void);

extern void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);

extern char *strcpy(char *destination, const char *source);

extern void *mhfldWaveFile;

extern void *mhfldPlayerFile;

extern void *mhfldAnaFile;

extern s32 gFldRndEncBtl;

extern u8 fldLoadNextGetStatus2(void);

extern u32 fldLoadNextGetStatus(void);

extern void dds3SetActiveScene(dds3Basic_t *scene);

extern s32 evtBitCheck(s32 no);

extern void evtBitOff(s32 no);

extern void evtBitOn(s32 no);

extern s32 ProcRandomEncountCheck(void);

extern void fldLoadNextFile_SetId_BtlRet(u32 field, u32 area);

extern char *strcpy(char *destination, const char *source);

extern s32 fldLoadNextTask(void);

extern void fldFreeNextFile(void);

extern s32 fldBgmIsLoading(void);

extern void fldWaitBgmLoad(void);

extern s32 sdfDelayedResrcCheck(void);

extern s32 fldNext_fld;

extern s32 fldNextArea;

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern s16 fldRun_MAP_OX;

extern s16 fldRun_MAP_OZ;

extern s16 fldRun_MAP_BX;

extern s16 fldRun_MAP_BZ;

extern void fldDoor_EveBtlExit(s32 field, s32 eveno);

typedef struct {
    u32 flags;
    s32 stage;
} fldSequenceController_t;

extern fldDanteGunState_t fldRunDanteGunFlag;

extern u8 fldDoorExitFlag;

extern s8 fldDoorExitMode[];

extern char pointResName[];

extern const char fldProcDrawName[];

extern char fldProcSequenceName[];

extern s32 fldProcShutDownFlg;

extern s32 ProcSequence(dds3ProcessID_t *id);

extern s32 ProcDraw(dds3ProcessID_t *id);

extern void fldFreeSnowSeed(void);

extern void fldAutoMapSeqSetFlag(u32 flag);

extern void fldFreeInputPanel(void);

extern void evtDisableMoonDraw(void);

extern void fldFreeAddressProcess(void);

extern void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *hDistortionBlur);

extern void campKillProcesses(void);

extern void scrAllReleaseScript(void);

extern dds3Basic_t *fldAnaBasic;

extern effBlurScatterWork_t *hDistortionBlur;

extern const char fldProcDrawName[];

extern s32 gfldPlayerSize;

extern void *gfldAnaPtr;

extern s32 gfldAnaSize;

extern void *gfldWavePtr;

extern s32 gfldWaveSize;

extern void *gfldPlayerPtr;

extern s32 gfldPlayerSizeBk;

extern void dds3AdminiCallSequence(s32 sequence, void *data, s32 datasize, s32 restore);

extern void fldSetNextField(s32 field, s32 parameter);

extern fldSceneParam_t fldSceneParam;

extern s32 gFldNowDZone;

extern s32 fldProcCampWait;

extern s32 campCheckExitState(void);

extern void fldEncountClearFlag(u32 flag);

extern dds3Basic_t *fldRunTama1Basic;

extern dds3Basic_t *fldRunTama2Basic;

extern dds3Basic_t *fldRunTama3Basic;

extern s32 fldRunTamaNext;

extern void evtEnableMoonDraw(void);

extern void fldSkyRefreshTbl(void);

extern char fldCameraUnitName[];

extern void fldStatusSet(void);

extern dds3Basic_t *dds3GetActiveScene(void);

extern void dds3VisibleSceneBasic(dds3Basic_t *basic, s32 visible);

extern u8 cmpGetLoadState(void);

extern void dds3FadeOut2(s32 frame);

extern void evtKillFieldScript(void);

extern void p2sdStopAllSe(void);

extern void dds3FadeIn2(s32 frame);

extern s32 fldPlayerChkAna(void);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern void fldPlayerPosRot(void);

extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);

extern void fldPlayerMotReset(void);

extern void cmpProcessStart(void);

extern saddr_t dds3SceneBasicStartScript(dds3Basic_t *scene, const char *procname);

extern s32 fldChkVortex(void);

extern s32 fldProcCampChk(void);

extern s32 fldProcAmChk(void);

extern s32 fldEncountTestFlag(u32 flag);

extern s32 fldAutoMapSeqTestFlag(u32 flag);

extern void fldAutoMapSeqClearFlag(u32 flag);

extern void fldEncountSetFlag(u32 flag);

extern void fldReleaseUnitPad(void);

extern void fldAutoMapSeqStart(void);

typedef struct scrProcessWork_s scrProcessWork_s;

typedef struct dds3FontReq_t dds3FontReq_t;

/* destroy runs when the request leaves the list, draw once per frame while it
 * is still on it. */
typedef struct
{
    void (*destroy)(dds3FontReq_t *req);
    void (*draw)(dds3FontReq_t *req);
} dds3FontReqFunc_t;

struct dds3FontReq_t
{
    dds3FontReq_t *prev;
    dds3FontReq_t *next;
    s32 group;
    dds3FontReqFunc_t *func;
};

extern dds3FontReqFunc_t dds3FontReqTable;

extern dds3FontReqFunc_t dds3FontReqBoxTable;

extern u8 dds3PadButtonState[];

extern const char fldKiretaLabel[];

extern fldSequenceController_t fldAutoMapSeqFlags;

/* The retail load is its own symbol, not a field of the controller. */
extern s32 fldSequenceState;

extern u32 fldEncountFlags;

extern s32 fldRunDanteSe_asise;

extern s8 slNewGameBit2208, slNewGameBit2209, slNewGameBit2210, slNewGameBit2211;

extern s8 slNewGameBit2212, slNewGameBit2213, slNewGameBit2323, slNewGameBit162;

extern u16 dds3NewGameSavedHalf0C;

extern s32 D_0054EA18;

extern sdfMemHandle_t *D_0054EA1C;

extern s32 gFldAnahoriAkumaIdx;

extern s32 gFldAnahoriAkumaIdxMax;

extern s32 fldProcAmWait;

extern s32 fldNext_fld;

extern s32 fldNextArea;

extern s32 fldNextFieldId;

extern s32 fldElvState;

extern s32 fldElvFloorTarget;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern sdfMemHandle_t *fldPlayerAnimData;

extern sdfMemHandle_t *fldRunMapData;

extern fldRunMapHeader_t *fldRunMapHeader;

extern fldMapCell_t *fldRunMapCellData;

extern void *pFroMapHead;

extern void *fldRunMapScale;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern scrProcessWork_s *scrSearchScriptName(char *scriptname);

extern u32 fldAutoMapChkMode(void);

extern void fldSetTerminalState(s32 type, s32 idx);

extern void fldPauseBgm(void);

extern void fldProcNextJump(void);

extern s32 fldDoor_SiseExit(void);

extern void fldEveHit_End(void);

extern void fldProcShutDown(void);

void InitOoyTest(void);

void fldRegistUnitPad(void);

/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
extern const char fldKiretaLabel[16];

s32 fldProcKiretaChk(void);

s32 fldProcHirakenaiChk(void);

extern fldUnitControl_t *fldGetUnitControl(void);

extern effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *param);

extern dds3ProcessID_t *dds3SearchProcessPriority(s32 priority);

extern dds3Basic_t *fldUnitControlHandle;

extern u32 dds3BackColor;

extern s32 gFldStart, fldSkyEnable, gfldPlayerHasiCnt;

extern s32 fldWmWarpMode;

extern s32 fmProcessChk(void), fldGetRestoreMode(void), fldWaitBgmReload(void);

extern s32 dds3FadeCheck(void), fldEveHit_ChkEveHitAfter(void), fldPlayerCalc(void);

extern void fldEveHit_ChkEveHit(dds3Basic_t *basic);

extern void fldSkySetTbl(s32 index, s32 blend), fldSkySetTableAnime(s32 type);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *basic, s32 flag);

extern void fldPanelSet(s32 mode);

extern void *fldCamMain(void);

extern void fldResetSequenceParams(void);

extern void evtEnableMoonCalc(void);

extern void fldReadyScene(void);

extern void fldInitInputPanel(void);

extern void fldInitAddressProcess(void);

extern void fldCamResetNpc(void);

extern void fldSnowSeedInit(void);

extern void fldSnowSeedReset(void);

extern void fldBgmLoad(void);

extern void fldBgmPlay(void);

extern void fldPlayFieldBgm(void);

extern void fldBgmContinue(void);

extern void fldSnowInit(void);

extern void fldRegistUnitPad(void);

extern void fldSetSnowActive(void);

extern void fldRunView(void);

extern void encStart(void);

extern void fldLightMaTask(void);

extern void fldSkyCalcTable(void);

extern void fldSkyCalcFilter(void);

extern void fldHitWM_Mask(void);

extern void fldPlaySe3D(void);

extern void fldBgmBitChk(void);

extern void fldAutoMapProc(void);

extern void fldNpcCalc(void);

extern void fldItemBoxCalcTask(void);

s32 fldProcCampChk(void);

s32 ProcCampCheck(void);

s32 fldProcAmChk(void);

s32 ProcAmCheck(void);

s32 RetryMapLoad(void);

s32 ProcSequence(dds3ProcessID_t *id);

extern s32 nbGetEncountOn(void);

extern void fldDbgPrint2D(s32 x, s32 y, const char *text);

extern void fldNpcDraw(void);

extern void fldWaveTask(void);

extern void fldAutoMapDraw(void);

extern void fldSnowTask(void);

extern void fldElvDrawFloorNum(void);

extern void dds3EffectBlur(const dds3Effect_Blur_t *source);

extern void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);

#define DRAW_FIELD_BLUR(color, mode, angle, zoom) { \
    blur.abgr = (color); \
    blur.blend = (mode); \
    blur.rotate = (angle); \
    blur.scale = (zoom); \
    blur.center[0] = 0; \
    blur.center[1] = 0; \
    blur.xy[0][0] = 0; \
    blur.xy[0][1] = 0; \
    blur.xy[1][0] = 0x200; \
    blur.xy[1][1] = 0x1C0; \
    dds3EffectBlur(&blur); \
}

s32 ProcDraw(dds3ProcessID_t *id);

void fldAutoMapSeqSetFlag(u32 flag);

void fldAutoMapSeqClearFlag(u32 flag);

s32 fldAutoMapSeqTestFlag(u32 flag);

void fldEncountSetFlag(u32 flag);

void fldEncountClearFlag(u32 flag);

s32 fldEncountTestFlag(u32 flag);

void fldProcTrigSequence(fldSceneParam_t *srcParam, u32 mode);

void fldProcShutDown(void);

s32 fldProcShutDown2(void);

void fldProcSetTerminal(s32 type, s32 idx);

void fldProc_SisetuReturnPoint(s32 fld, s32 area, void *pName);

void fldProc_CallSisetuReturn(void);

void fldSetNextField(s32 fld, s32 area);

void fldProcNextJump(void);

extern s32 fldSequenceState;

extern u32 fldEncountBtlId;

extern const char fldProcDrawName[16];

extern s32 fldNextFieldId;

extern s32 fldEncountCount;

extern s32 fldEncountSagasiFlag;

extern effBlurScatterWork_t *hDistortionBlur;

extern s32 gFldRndEncBtl;

extern s32 fldEncountCheckWork;

extern s32 fldProcCampWait;

extern s32 fldProcAmWait;

extern s32 gFldStart;

extern s32 fldProcShutDownFlg;

extern s32 fldNext_fld;

extern s32 fldNextArea;

extern fldSequenceController_t fldAutoMapSeqFlags;

extern u32 fldEncountFlags;

extern u32 fldEncountTimer[2];
/* End of declarations from src/smt3/field/fldProcess.c */

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447450);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012CA98);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012F7A0);

void fldAutoMapSeqSetFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags |= flag;
}

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
}

void fldEncountSetFlag(u32 flag)
{
    fldEncountFlags |= flag;
}

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}

INCLUDE_ASM(const s32, "field/fldProcess", func_0012F950);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FA80);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FBF8);

INCLUDE_RODATA(const s32, "field/fldProcess", D_004477F0);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447808);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447820);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447938);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447948);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FC18);

void func_0012FCF0(void) {
}

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FCF8);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FE08);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FE50);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FEA8);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FEC0);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FEF8);

INCLUDE_ASM(const s32, "field/fldProcess", func_0012FF48);

INCLUDE_ASM(const s32, "field/fldProcess", func_001301C0);

INCLUDE_ASM(const s32, "field/fldProcess", func_001301D0);

INCLUDE_RODATA(const s32, "field/fldProcess", D_004479B8);

INCLUDE_ASM(const s32, "field/fldProcess", func_00130240);

INCLUDE_ASM(const s32, "field/fldProcess", func_00131718);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447AB0);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447AC8);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447AE0);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447AF0);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447B00);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447B10);

INCLUDE_RODATA(const s32, "field/fldProcess", D_00447B20);

INCLUDE_RODATA(const s32, "field/fldProcess", jtbl_00447B30);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6BC);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6C0);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6C4);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6C8);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6CC);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6D0);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6D4);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6D8);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6DC);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6E0);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6E4);

INCLUDE_SDATA(const s32, "field/fldProcess", D_0046B6E8);

