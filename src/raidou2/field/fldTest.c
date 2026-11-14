#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldTest.c */
#include "common.h"

#include "sdk_libgcc.h"

#include "basic.h"

#include "dds3obj.h"

#include "kernel.h"

#include "libsdf.h"

#include "ps2asm.h"

#include "newdata.h"

#include "field.h"

#include "event.h"

#include "model.h"

#include "motion.h"

#include "sdfFrame.h"

extern s32 fldDoor_ElvTakasaToFloor(s32 takasa);

extern dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

extern s32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

extern dds3Basic_t *dds3GetBasicArray(dds3BasicArrayHandle_t *hndl);

extern s32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

extern void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void rsetOtOfs(u32 otofs);

extern void fldDrwZSave(void);

extern void fldDrwZLoad(void);

extern f32 R360_LR(f32 a, f32 b);

extern sdfVector4_t fldCamTarget;

extern f32 sdfAtan2(f32 aY, f32 aX);

extern dds3Basic_t *dds3GetControlScene(void);

extern void dds3GetPosPoseSet(dds3Basic_t *pBasic);

extern void fldRunDanteShutdown(void);

extern void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 flag);

extern s16 fldRun_MAP_OX;

extern s16 fldRun_MAP_OZ;

extern s16 fldRun_MAP_BX;

extern s16 fldRun_MAP_BZ;

extern u32 dds3GetKernelCount(void);

extern s32 evtBitCheck(s32 no);

extern void fldSetElvCameraScene(s32 dir);

typedef struct {
    u16 sizeX;
    u16 sizeZ;
    u32 cellsOffset;
} fldRunMapGrid_t;

extern void fldMakeFilePath(char *pathstr, s32 field, s32 area);

#include "sdk_libc.h"

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic);

extern void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, u32 flag);

extern void mdlSingleMotion(void *aHandle, s32 aGroup, s32 aNumber);

extern s32 FroFtoI(f32 fx, f32 fz, u16 *rx, u16 *rz);

extern u16 fldRunDanteStartX[2];

extern void fldRunDanteCollision(void);

extern void fldRunDanteAi(void);

extern void fldRunDanteSetModel(void);

extern void fldRunDanteSe(void);

extern void fldRunEffTask(void);

extern void fldRunTamaSe(void);

extern void fldRunDanteStanceSet(s32 stance);

extern s32 p2sdLoadCheckMidi(u32 MidiNo);

extern s32 p2sdLoadSMGFile(u32 MidiNo);

extern void sdfDrawSync(void);

extern s32 nbGetEncountOn(void);

extern void rsetTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void rsetAlpha(s32 mode);

extern void fldSkyDrawFilter(void);

extern void fldDrawFrameSprite(void);

extern u32 dds3AllocObjectId(void);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 serialID, void *aPos, void *aRot);

extern void dds3SetUnitObjectId(dds3Basic_t *basic, const char *name);

extern void dds3ObjectBaseMakeUnit(dds3Basic_t *pObjectBasic, s32 major, s32 minor);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void fldFindDoorEvent(void *event);

extern void evtBitOn(s32 no);

extern void fldRunTamaFree(void);

extern void p2sdFadeOutBgm(u32 arg);

extern void p2sdStopBGM(u32 arg);

/* One loadable field effect: the resource handle trio the loader fills, then
 * the live effect or part handle and the placement the task applies. */
typedef struct {
    sdfMemHandle_t *mhFile;
    void *ptr;
    s32 size;
    void *pcf;
    void *d3p;
    s32 cnt;
    f32 x;
    f32 y;
    f32 z;
    f32 r;
} fldTest_FLDRUN_EFF_t;

typedef struct {
    fldTest_FLDRUN_EFF_t *pEff;
    s32 time;
    s32 effmode;
    s32 rotmode;
    f32 off_cam_range_min;
    f32 off_cam_range_max;
} fldTest_FLDRUN_EFFTBL_t;

extern fldTest_FLDRUN_EFFTBL_t *fldRunEffTables[];

extern char fldElvFloorLabels[];

extern void FromRotation(f32 aAngle);

extern dds3Basic_t *fldRunTama1Basic;

extern dds3Basic_t *fldRunTama2Basic;

extern dds3Basic_t *fldRunTama3Basic;

extern s32 fldRunTamaNext;

extern const sdfVector4Unaligned_t fldRunTamaSetAxis;

extern const sdfVector4_t fldRunTamaAxis;

extern const char fldTama1UnitName[], fldTama2UnitName[], fldTama3UnitName[];

extern const char fldDsparkPcfPath[], fldDsmoke2Path[], fldDspark02Path[], fldGun02aPath[];

extern const char fldGun01a0Path[], fldMuzzleB010Path[], fldDsmoke1Path[], fldDflash1Path[];

extern fldTest_FLDRUN_EFF_t fldDspark01File;

extern fldTest_FLDRUN_EFF_t fldDsmoke2File;

extern fldTest_FLDRUN_EFF_t fldDspark02File;

extern fldTest_FLDRUN_EFF_t fldGun02aFile;

extern fldTest_FLDRUN_EFF_t fldGun01a0File;

extern fldTest_FLDRUN_EFF_t fldMuzzleB010File;

extern fldTest_FLDRUN_EFF_t fldDsmoke1File;

extern fldTest_FLDRUN_EFF_t fldDflash1File;

extern void Mult(void);

typedef struct {
    f32 f[10];
} fldTamaSeFrame_t;

typedef struct {
    s16 v[10];
} fldTamaSeVol_t;

extern const fldTamaSeFrame_t fldRunTamaSeFrames1;

extern const fldTamaSeFrame_t fldRunTamaSeFrames2;

extern const fldTamaSeFrame_t fldRunTamaSeFrames3;

extern const fldTamaSeVol_t fldTamaSeVol;

extern s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z);

extern void frSetFontDrawOT(s32 OtType);

extern void fldKanjiDraw(s32 x, s32 y, u32 rgba, const char *moji);

extern void p2sdPlaySE(s32 id, s32 vol, s32 pan);

extern s32 fldProcCampChk(void);

extern s32 fldEncountTestFlag(u32 flag);

typedef struct effNode_t effNode_t;

extern void dds3EffectDelete(effNode_t *pData);

extern void dds3EffectDraw(effNode_t *pData);

extern s32 dds3EffectGetState(effNode_t *hEffect);

typedef struct effResourceHeader_t effResourceHeader_t;

extern effNode_t *dds3EffectLoad(effResourceHeader_t *pD3P);

extern void dds3EffectSetPosition(effNode_t *pData, sdfVector4_t *pPos);

extern void dds3EffectSetRotateMatrix(effNode_t *pData, sdfMatrix4_t *pMatrix);

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

extern effPartHandle_t *dds3EffectPartCreate(u16 type, void *pData);

extern void dds3EffectPartDelete(effPartHandle_t *hEffPart);

extern void dds3EffectPartDraw(effPartHandle_t *hEffPart);

extern void dds3EffectPartSetPosition(effPartHandle_t *hEffPart, sdfVector4_t *pPos);

extern void dds3EffectPartSetRotateMatrix(effPartHandle_t *hEffPart, void *pMatrix);

extern void SetRotateMatrixY(f32 aAngle);

typedef struct {
    char name[8];
    s16 ix;
    s16 iz;
    f32 angle;
} fldDanteDoor_t;

/* Dante's chase work block. Only the fields the chase code reaches are known. */
typedef struct
{
    s32 status;
    f32 x;
    f32 y;
    f32 z;
    f32 turn;
    f32 oldframe;
    f32 nowframe;
    s16 ix;
    s16 iz;
    u16 targetIx;
    u16 targetIz;
    u16 tikamiti;
    s16 wkmode;
    fldMapCell_t *routeCell;
    void *RouteDataTop;
    s32 now;
    s32 goal;
    s32 nextframe;
    f32 angleToPlayer;
    f32 speed;
    s32 cnt_ex;
    s32 gunlight_anime;
    u32 flag;
    s32 update_flag;
    s32 bgm_mode;
    f32 d_p_len;
    f32 d_p_dir;
    f32 d_c_dir;
    s32 stance;
    fldDanteDoor_t *pDoor;
    u16 mapIxOld;
    u16 mapIzOld;
    s32 old_status;
} fldTest_FLDRUN_DANTE_t;

extern const char fldTestDrawName[];

extern fldTest_FLDRUN_DANTE_t fldRun_Dante;

extern s32 fldRunDanteSe_asise;

extern s32 fldElvState;

extern s32 fldElvFloorTarget;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern sdfMemHandle_t *fldRunMapData;

extern fldRunMapHeader_t *fldRunMapHeader;

extern fldMapCell_t *fldRunMapCellData;

extern void *pFroMapHead;

extern void *fldRunMapScale;

extern dds3ProcessID_t *dds3StartProcess(char *v1, u32 dDS3_PRIORITY_DRAW_PB, s32 v2,
                                         s32 v3, dds3ProcessFunc_t drawExit,
                                         dds3ProcessFunc_t deleteExit, void *p);

extern s32 dds3KillProcessName(char *name, s32 shutdowntimer);

extern void sdfMemFreeDelayedDirect(sdfMemHandle_t *aHandle);

extern s32 fldAutoMapChkMode(void);

extern void fldClearElvCameraScene(void);

extern void fldRunDanteGetDistAngle(f32 *dist, f32 *angle);

extern s32 ooyTestDrawProc(dds3ProcessID_t *aProcId);

s32 ooyTestDrawProc(dds3ProcessID_t *aProcId);

void InitOoyTest(void);

void KillOoyTest(void);

void fldElvGetFloorDigits(s32 x, s32 y, s32 floor);

void fldElvSetFloor(s32 start, s32 end);

void fldElvReset(void);

s32 fldElvCheckEnd(void);

/* NON_MATCHING. Behaviour checked against retail: 6 PCSX2 call vectors give the same return and memory,
 * field underground facility, field unknown realm savestates, covering only the paths those calls took.
 * (sdfport bc29478, tools/funcvec, tests/funcvec.sh). */
extern const char fldTestDrawName[16];

void drawSceneBasicList(void);

void fldRunDanteClear(void);

void fldRunTamaInit(void);

void fldRunEffSet(s32 id, f32 x, f32 y, f32 z, f32 r);

void fldRunTamaSet(f32 x, f32 y, f32 z, f32 r);

extern const sdfVector4_t fldRunTamaAxis;

extern const char fldTama1UnitName[16];

extern const char fldTama2UnitName[16];

extern const char fldTama3UnitName[16];

extern const char fldDsparkPcfPath[24];

extern const char fldDsmoke2Path[24];

extern const char fldDspark02Path[24];

extern const char fldGun02aPath[24];

extern const char fldGun01a0Path[24];

extern const char fldMuzzleB010Path[32];

extern const char fldDsmoke1Path[24];

extern const char fldDflash1Path[24];

extern const sdfVector4Unaligned_t fldRunTamaSetAxis;

extern const fldTamaSeFrame_t fldRunTamaSeFrames1;

extern const fldTamaSeFrame_t fldRunTamaSeFrames2;

extern const fldTamaSeFrame_t fldRunTamaSeFrames3;

extern const fldTamaSeVol_t fldTamaSeVol;

void fldMakeFroFileName(void);

void fldFreeRunMapData(void);

s32 FroFtoI(f32 fx, f32 fz, u16 *rx, u16 *rz);

void fldMapIndexToPos(s16 ix, s16 iz, f32 *px, f32 *pz);

fldMapCell_t *fldFindMapCell(s16 x, s16 y, s16 z, s16 w);

void fldRunDanteGetDistAngle(f32 *pDist, f32 *pAngle);

void fldGetDistAngleTo(f32 *pDist, f32 *pAngle, f32 x, f32 z);

f32 fldTurnToward(f32 cur, f32 target, f32 speed, f32 minStep);

void func_0011C790(void);

void func_0011C798(void);

void func_0011C7B8(void);

void fldRunDanteDoin(void);

extern s32 fldHitChkEnemy(const f32 *position, const f32 *target, f32 *hitPosition);

extern void fldRunAiCng(s32 mode);

void fldRunDanteMove(void);

void fldRunDantePwak(void);

void fldRunDanteSoba(void);

/* Native "Re1KO\n" string: seven bytes including NUL; keep its .sdata slot. */
extern const char fldMsgRe1KO[];

void fldRunDantePatr(void);

extern dds3Basic_t *fldDanteUnit;

extern s32 fldPlayerStepDist;

extern u32 dds3GetRandA(void *name);

extern void datPartyDamageFld(s32 mode);

extern void fldRunEffSet(s32 id, f32 x, f32 y, f32 z, f32 r);

extern evtUnit_t *dds3GetUnitHandleObjectBasic(dds3Basic_t *basic);

extern s32 sdfModelGetMarkPos(sdfModelRootNode_t *root, s32 mark);

extern const f32 fldRunDanteGun1Frames[4];

extern const s32 fldRunDanteGun1Marks[4];

extern const f32 fldRunDanteGun2Frames[6];

extern const s32 fldRunDanteGun2Marks[8];

void fldRunDanteAi(void);

void fldRunDanteLoadBgm(void);

void fldRunDanteSwapBgm(s32 toDante);

void fldRunDanteInit(void);

void fldRunDanteShutdown(void);

void fldRunDanteFacePlayer(void);

s32 fldRunDanteDistToTarget(f32 x, f32 y, f32 z);

void fldRunDanteSe(void);

void fldRunView(void);

s32 fldRunEncountChk(void);

extern const sdfVector4_t fldRunDanteAxis;

extern s32 gFldElvFloorNow;

extern s32 fldElvFloorFrom;

extern s32 fldElvFloorNum;

extern s32 fldElvFloorTo;

extern s32 fldElvFloorDisp;

extern s32 fldElvFloorTarget;

extern s32 fldElvState;

extern s32 fldShadowWork;

extern s32 fldSdwBoxAlpha;

extern s32 fldSdwLightTracking;

extern s16 fldRun_MAP_OX;

extern s16 fldRun_MAP_OZ;

extern s16 fldRun_MAP_BX;

extern s16 fldRun_MAP_BZ;

extern sdfMemHandle_t *fldRunMapData;

extern fldRunMapHeader_t *fldRunMapHeader;

extern fldMapCell_t *fldRunMapCellData;

extern void *pFroMapHead;

extern void *fldRunMapScale;

extern dds3Basic_t *fldRunTama1Basic;

extern dds3Basic_t *fldRunTama2Basic;

extern dds3Basic_t *fldRunTama3Basic;

extern s32 fldRunTamaNext;

extern s32 fldRunDanteSe_asise;

extern dds3Basic_t *fldUnitControlHandle;

extern u32 fldPlayerUnit;

extern dds3Basic_t *fldDanteUnit;
/* End of declarations from src/smt3/field/fldTest.c */

void InitOoyTest(void)
{
    dds3StartProcess(fldTestDrawName, DDS3_USERDRAW_PRIORITY, 0, 0, ooyTestDrawProc, NULL, NULL);
}

INCLUDE_RODATA(const s32, "field/fldTest", D_r1_00447278);

