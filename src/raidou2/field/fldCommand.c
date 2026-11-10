#include "common.h"

/* Declarations from smt3-decomp src/smt3/field/fldCommand.c */
#include "common.h"

#include "basic.h"

#include "model.h"

#include "newdata.h"

#include "scr.h"

#include "field.h"

#include "ps2asm.h"

#include "sdfFrame.h"

typedef struct
{
    void *resourceState;
    u32 roomNumber;
} fldMatterWork_t;

typedef struct
{
    s32 unk00;
    s32 value;
} fldDanteParam_t;

typedef struct fldUnit_t fldUnit_t;

typedef struct fldInputPanel_t fldInputPanel_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

extern s32 gFldSe3DNum;

extern u32 fldSeLastBgm;

extern s32 fldSeBgmId;

extern s32 fldBgmReloadStep;

extern s32 MidiNo;

extern s32 fldBgmLoadStep;

extern s32 fldBgmSmgAddr;

extern sdfMemHandle_t *fldAutoMapBuf;

extern fldAmAreaData_t *fldAutoMapHeader;

extern s32 fldAutoMapData;

extern u32 AutoMapSeq;

extern s32 fldAutoMapField;

extern s32 fldAutoMapBBox;

extern s32 fldTakaraCount;

extern sdfTexHandle_t *fldUiTexture;

extern s32 fldNpcModelTable;

extern sdfTexHandle_t *fldTitleMiniTex;

extern sdfTexHandle_t *fldTitleMiniWork;

extern s32 fldSnowActive;

extern s32 slProcShutDownFlg;

extern s32 gSlWindowCnt;

extern s32 gslInfoCode;

extern s32 slChooseSaveIndex;

extern s32 slExitWaitCnt;

extern s32 gslMode;

extern u32 gslErrorCode;

extern u32 slQueryYesNoMsg;

extern u32 gslDrawFlagFile;

extern s32 gslCheckCardWait;

extern u32 gSlNowCardCheck;

extern u32 gslDrawFlagBg;

extern addr_t baseIconFileHandle;

extern u32 gslErrorKeyFlag;

extern s32 slFileSelError;

extern u32 gSlEndingMode;

extern s32 gslAnimCnt;

extern u32 gslExitType;

extern s32 slQueryYesNoCursor;

extern s32 slQueryYesNoKeyFlag;

extern u32 gSlExitMode;

extern s32 scrGetIntPara(s32);

fldInputPanel_t *fldInputPanelSetMessage(s32 no);

fldInputPanel_t *fldInputPanelSetDraw(s32 flag);

extern void fldReleaseUnitPad(void);

dds3Basic_t *dds3GetActiveScene(void);

dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *pSceneBasic);

extern void fldPlayerMotReset(void);

extern s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, void *func);

extern void fldRegistUnitPad(void);

dds3Basic_t *fldGetUnitFollowCamera(void);

dds3Basic_t *dds3SetActiveCameraSceneBasic(dds3Basic_t *pSceneBasic, dds3Basic_t *pCameraBasic);

extern void dds3CameraObjectBasicGetTgtCam(dds3Basic_t *pBasic, f32 *tgtpos,
                                                       f32 *campos);

extern void fldCamGetPhokanParam(void);

extern f32 fldCamPhokanSelX[];

extern f32 fldCamPhokanRot[];

extern s32 fldCamPhokanState;

extern s32 fldCamPHokanMode;

dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *pBasic, u32 serialID, s32 type);

char *scrGetStrPara(s32 n);

dds3Basic_t *dds3GetControlScene(void);

dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const u8 *name);

#include "sdk_libc.h"

extern char fldHojiNameKey[];

extern char *strstr(const char *, const char *);

extern fldDanteGunState_t fldRunDanteGunFlag;

extern void fldSkySetFilter(s32 idx, s32 mode, s32 hokan);

extern void fldSetFilterAnime(s32);

extern void fldSetModelFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

dds3BasicArrayHandle_t *dds3SceneBasicGetList(dds3Basic_t *pBasic, s32 type);

u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *hndl);

u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *hndl);

u32 dds3GetBasicArray(const dds3BasicArrayHandle_t *hndl);

void dds3MatterObjectBasicSetAlpMode(dds3Basic_t *aPtr, u32 alpmode);

u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *hndl);

void dds3FreeBasicArrayHandle(dds3BasicArrayHandle_t *hndl);

extern void fldSetAnimeFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

void dds3ObjectBaseMatterAnimeSpeed(dds3Basic_t *pObjectBasic, u32 mode);

extern void fldSetNpcFlag(s32 world, s32 stage, s32 room, s32 value);

extern void fldGbSetGimic(s32 fld, s32 area, s32 hojiID, s32 toplast);

extern void fldSetAtariFlag(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern s32 fldSetModelFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern s32 fldSetAnimeFlag2(s32 area, s32 slotIndex, s32 bit, s32 enabled);

extern void fldSetAnimeId2(s32 area, s32 slotIndex, s32 valueIndex, s32 value);

extern void fldAutoMapOpenBlock2(s32 fld, s32 area, s32 SBlock);

extern void fldAutoMapCloseBlock2(s32 fld, s32 area, s32 SBlock);

u32 fldGetRestoreMode(void);

scrProcessWork_s *scrGetScriptHandle(void);

extern s32 fldFindEveRngIndex(addr_t);

extern s32 fldDoor_GetHasigoTbl(s32);

extern s32 fldDoor_GetElvTbl(s32);

extern s32 fldElvCheckEnd(void);

extern s32 fldWapGetDoor(s32 getmode);

extern void scrSetIntReturnValue(s32 i);

extern s32 fldDoor_ChkWarp(void);

extern s32 fldInfNext(s32 aline, s32 asel);

extern s32 fldInfGet(s32 aline, s32 acol);

extern s32 fldInfChkFastPanel(s32 aline);

extern void fldInfToWAP(void);

void fldBgmLoad(void);

extern void fldSePlay(s32 seid);

extern void fldSeStop(s32 seid);

extern void fldMotionSePlay(s32 seid, s32 subid);

extern const char *fldEveHitResName_Find(addr_t pid);

extern void fldDoor_ExitCamera(const char *evename);

extern s32 fldDoor_GetWarpEff(void);

extern fldUnit_t *evtGetUnitHandle(s32 serial);

extern void fldNpcLightSet(fldUnit_t *hEveUnit);

void fldAnahoriSelectInit(void);

extern u32 fldAnahoriSelectGet(void);

void fldAnahoriSelectNext(void);

void fldAnahoriSelectDel(void);

extern fldDanteParam_t *fldDanteUnit;

extern void fldRunDanteStanceSet(s32);

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern s32 fldInputPanelCheckKey(void);

extern void fldEncountSetFlag(s32 flags);

extern void fldEncountClearFlag(s32 flags);

extern char *strcpy(char *dst, const char *src);

extern void fldSkySetTbl(s32 idx, s32 hokan);

extern s32 fldGetTakaraOpen(s32 room);

extern void fldSetTakaraOpen(s32 room);

extern void fldDoor_ElvExit(s32 select);

extern void fldElvSetFloor(s32 first, s32 second);

extern void fldElvReset(void);

extern s32 fldEveHitResID_Find(addr_t pid);

extern void fldDoorRun_FindStart(s32 resid);

extern void fldBgmContinue(void);

extern void fldMotionSeStop(s32 seid, s32 subid);

extern void fldTitleStart(s32 fld, s32 waittime);

extern f32 fldAutoMapBgScroll;

extern void evtBitOn(s32 no);

extern void evtBitOff(s32 no);

typedef struct
{
    f32 x;
    f32 y;
    f32 z;
    s16 flg;
    u8 spdx;
    u8 spdy;
    s16 life;
    s16 alp;
} fldSeedWork_t;

extern fldSeedWork_t *gFldSeedBuff;

extern sdfMemHandle_t *fldSnowSeedBuf;

extern s32 gFldKyouseiTalk;

extern void fldFreeNextFile(void);

extern void fldDoor_WarpEx(s32 ex_no);

extern s32 fldNpcModelFreeIdx;

extern dds3Basic_t *fldNpcFreeCount;

extern s32 fldNpcCamTarget;

extern s32 slDDS3To31Flg;

extern s32 version_convert_ver;

extern u32 slIconSysSaveBuff;

extern u32 slIconSysSaveSize;

extern void fldDoor_Warp(const char *evename);

extern sdfTexHandle_t *fldEATexture;

extern sdfTexHandle_t *fldVLTexture;

extern sdfTexHandle_t *fldVLBlurHandle;

extern sdfTexHandle_t *fldSnowBlurHandle;

extern void *fldDmgBlurEffect;

extern void *fldTDmgEffect;

extern sdfTexHandle_t *fldTitleFldTex;

extern sdfTexHandle_t *fldTitleDun1Tex;

extern sdfTexHandle_t *fldTitleDun2Tex;

extern sdfTexHandle_t *fldTitleDun3Tex;

extern s32 gFldBgmBit;

extern void *fldDmgBlurHandle;

extern void *fldNpcBlurHandle;

extern void fldBgmPlay(void);

extern struct effNode_t *fldTakaraSlotCount;

extern struct effNode_t *fldNpcLoadIdx;

extern f32 slNextPageDraw_fc;

extern sdfTexHandle_t *slProcPtr;

extern s32 evtGetAgeOfMoon(void);

extern s32 dds3GetRandIntA(s32 aLimit, s32 name);

extern s32 fldItemBoxRndTable[];

extern char fldFixCameraMissingMsg[];

extern void resetCamTgtPlayer(void);

extern s32 gSlSelCnt;

extern s32 gFldSnowLife;

extern s32 gFldSnowMode;

extern f32 gFldSnowX;

extern f32 gFldSnowY;

extern f32 gFldSnowZ;

extern f32 gFldSnowYD;

extern f32 gFldSnowSpd;

extern f32 gFldSnowW;

extern s32 fldSeSmgId;

extern s32 fldEveHit_ChkDir(dds3Basic_t *unit, dds3Basic_t *put);

extern s32 fldEveHit_ChkDir2(dds3Basic_t *unit, dds3Basic_t *put);

extern sdfMemHandle_t *fldNpcModelCount;

extern sdfMemHandle_t *fldTakara1Mem;

extern sdfMemHandle_t *fldTakara3Mem;

extern sdfMemHandle_t *fldTakara2Mem;

extern sdfTexHandle_t *hEffPart;

extern void *fldItemBoxCount;

extern void fldLoadNextFile(s32 field, s32 area);

extern void fldDoor_WarpCash(const char *evename);

void fldBgmFadeOut(void);

void fldBgmStop(void);

void fldNpcHojiRefresh(void);

void fldPauseBgm(void);

s32 fldWaitBgmReload(void);

s32 fldCommand_ACT_WIN_DSP(void);

s32 fldCommand_ACT_WIN_DSP2(void);

s32 fldCommand_ACT_WIN_DSP3(void);

s32 fldCommand_PAD_DISABLE(void);

s32 fldCommand_PAD_ENABLE(void);

u32 fldCommand_ENCOUNT_ON(void);

u32 fldCommand_ENCOUNT_OFF(void);

s32 fldCommand_SET_FIX_CAMERA(void);

extern f32 fldCamPitch;

extern f32 fldCamEye[];

extern f32 fldCamTarget[];

extern f32 sdfSin(f32 angle);

extern f32 sdfCos(f32 angle);

extern s32 fldPlayerFastSet(void);

extern void fldCamSetChase(void);

extern void resetCamNormal(void);

s32 fldCommand_SET_CHASE_CAMERA(void);

s32 func_00158548(void);

s32 fldCommand_CAM_DIR(void);

s32 fldCommand_CAM_SEL_PHOKAN(void);

s32 fldCommand_SET_NPC_CAMERA(void);

extern void evtDisableMoonDraw(void);

extern void evtEnableMoonDraw(void);

extern void evtDisableMoonDraw2(void);

extern void evtEnableMoonDraw2(void);

s32 fldCommand_SET_ALLPANEL(void);

void fldPanelSet(s32 mode);

s32 fldCommand_SET_CASH_F(void);

s32 fldCommand_FREE_CASH_F(void);

s32 fldCommand_SEARCH_ID(const char *pName);

s32 fldCommand_GetHojiID(char *pName);

s32 fldCommand_SET_CAMTBL(void);

s32 fldCommand_FLD_KANKYO(void);

s32 fldCommand_FLD_FILTER(void);

s32 fldCommand_FLD_MODEL_ON(void);

s32 fldCommand_FLD_MODEL_OFF(void);

s32 fldCommand_FLD_ANIME_ON(void);

s32 fldCommand_FLD_ANIME_OFF(void);

s32 fldCommand_FLD_ATARI_ON(void);

s32 fldCommand_FLD_ATARI_OFF(void);

s32 fldCommand_FLD_MODEL_ON2(void);

s32 fldCommand_FLD_MODEL_OFF2(void);

s32 fldCommand_FLD_ANIME_ON2(void);

s32 fldCommand_FLD_ANIME_OFF2(void);

s32 fldCommand_FLD_ANIME_ID2(void);

s32 fldCommand_FLD_MAP_ON(void);

s32 fldCommand_FLD_MAP_OFF(void);

s32 fldCommand_FLD_NPC_ON(void);

s32 fldCommand_FLD_NPC_OFF(void);

s32 fldCommand_FLD_GIMIC_HOJI(void);

s32 fldCommand_FLD_CHK_BTL(void);

s32 fldCommand_FLD_CHK_ANA(void);

s32 fldCommand_FLD_CHK_SISETU(void);

s32 fldCommand_FLD_CHK_EVE(void);

s32 fldCommand_FLD_GET_TAKARA_TBL(void);

u32 fldCommand_FLD_SET_TAKARA_OPEN(void);

u32 fldCommand_FLD_CHK_TAKARA_OPEN(void);

s32 fldCommand_FLD_WARP_CALL_FIELD(void);

s32 fldCommand_FLD_CALL_WAPEX(void);

s32 fldCommand_FLD_WARP_CASH_F(void);

u32 fldCommand_FLD_WARP_HASIGO_GETTBL(void);

u32 fldCommand_FLD_WARP_ELV_GETTBL(void);

u32 fldCommand_FLD_WARP_ELV_EXIT(void);

u32 fldCommand_FLD_ELV_FLOORSTART(void);

u32 fldCommand_FLD_ELV_FLOOREND(void);

u32 fldCommand_FLD_ELV_ENDCHK(void);

s32 fldCommand_FLD_WARP_GET_DOOR(void);

u32 fldCommand_FLD_WARP_RUN_START(void);

s32 fldCommand_FLD_INF_NEXT(void);

s32 fldCommand_FLD_INF_GET(void);

void fldCommand_FLD_INF_GETPANEL(void);

s32 fldCommand_FLD_INF_GOWAP(void);

u32 fldCommand_FLD_BGM_RELOAD(void);

s32 fldCommand_FLD_BGM_RELOAD_WAIT(void);

s32 fldCommand_FLD_BGM_PLAY(void);

s32 fldCommand_FLD_BGM_CONTINUE(void);

s32 fldCommand_FLD_BGM_PAUSE(void);

s32 fldCommand_FLD_BGM_STOP(void);

s32 fldCommand_FLD_BGM_FADE(void);

s32 fldCommand_FLD_SE_PLAY(void);

s32 fldCommand_FLD_SE_STOP(void);

s32 fldCommand_FLD_MSE_PLAY(void);

s32 fldCommand_FLD_MSE_PLAY2(void);

u32 fldCommand_FLD_MSE_STOP2(void);

u32 fldCommand_FLD_TITLE_START(void);

u32 fldCommand_FLD_TITLE_START2(void);

s32 fldCommand_FLD_SET_ENCOUNT(void);

s32 fldCommand_FLD_EXIT_CAMERA(void);

s32 fldCommand_FLD_WARP_GET_WARPEFF(void);

s32 fldCommand_FLD_MODEL_LIGHT(void);

typedef struct {
    u16 item;
    u16 par;
} datTakaraSagasiItem_t;

typedef struct {
    u32 flag;
    datTakaraSagasiItem_t itemtbl[8];
} datTakaraSagasiTable_t;

typedef struct {
    u32 flag;
    datTakaraSagasiTable_t tbl[4];
} datTakaraSagasi_t;

extern datTakaraSagasi_t datSagasiSkillTable;

extern u32 fldEncountSagasiFlag;

s32 fldCommand_FLD_SAGASI_AKUMA(void);

u32 fldCommand_FLD_ANAHORI_INIT(void);

u32 fldCommand_FLD_ANAHORI_GET(void);

u32 fldCommand_FLD_ANAHORI_NEXT(void);

u32 fldCommand_FLD_ANAHORI_DEL(void);

s32 fldCommand_FLD_NPC_SINEN(void);

s32 fldCommand_FLD_GET_KIRETA(void);

s32 fldCommand_FLD_GET_ITEMBOX_RND(void);

s32 fldCommand_FLD_MODEL_DANTE(void);

u32 fldCommand_FLD_DANTE_AI_STOP(void);

u32 fldCommand_FLD_DANTE_AI_START(void);

s32 fldCommand_FLD_DANTE_STANCE(void);
/* End of declarations from src/smt3/field/fldCommand.c */

s32 fldCommand_GetHojiID(char *pName) {
    s32 index;
    if (strstr(pName, fldHojiNameKey) == 0) return 0;
    for (index = 0; index < 32; index++) {
        if (pName[index] == '\0') {
            if (index < 4) return 0;
            return (pName[index - 2] - '0') * 10 + (pName[index - 1] - '0');
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "field/fldCommand", D_004DF1F0);

