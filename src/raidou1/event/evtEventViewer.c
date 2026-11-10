#include "common.h"

/* Declarations from smt3-decomp src/smt3/event/evtEventViewer.c */
#include "common.h"

#include "basic.h"

#include "event.h"

#include "evtPolygonMovie.h"

#include "effect.h"

#include "kernel_draw.h"

#include "ps2asm.h"

#include "kernel.h"

#include "model.h"

#include "motion.h"

/* The viewer's current menu, which EnterMain switches on to decide what Enter does
 * from each one. Names from the IDB. */
typedef enum EVT_VIEWER_MENU {
    EVT_VIEWER_MENU_NON = 0,
    EVT_VIEWER_MENU_MAIN = 1,
    EVT_VIEWER_MENU_SELECT = 2,
    EVT_VIEWER_MENU_FILENAME = 3,
    EVT_VIEWER_MENU_OBJLIST = 4,
    EVT_VIEWER_MENU_FRAMELIST = 5,
    EVT_VIEWER_MENU_FRAMECHANGE = 6,
    EVT_VIEWER_MENU_INTCHANGE = 7,
    EVT_VIEWER_MENU_FLOATCHANGE = 8,
    EVT_VIEWER_MENU_FILENAME2 = 9,
    EVT_VIEWER_MENU_BASICLIST = 10,
    EVT_VIEWER_MENU_DATA = 11,
    EVT_VIEWER_MENU_EVENTPOS = 12,
    EVT_VIEWER_MENU_MAX = 13
} EVT_VIEWER_MENU;

extern evtFrameInfo_t *evtAddFrameInfo(evtViewerObject_s *track, s32 frame);

extern void sdfChipFreeDirect(void *aPtr);

extern void dds3DeleteSceneBasicData(void *basic);

extern void evtInitViewerInfo(void);

extern s32 dds3KillProcessName(const char *name, s32 shutdowntimer);

extern evtViewerInfo_t *viewerinfo;

extern void *memset(void *a, s32 b, size_t c);

extern void *sdfChipAllocDirect(s32 aSize);

extern void evtFreePolygonMovieWorkData(void *wdata);

extern dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *scene, s32 type, const char *name);

extern void *dds3AddBEDEffectObjectBasic2(void *data, f32 *pos, f32 *rot);

extern void dds3EffectObjectBasicStatOn(void *basic, s32 s);

extern void dds3StartBEDEffectObjectBasic(void *basic, s32 cutno);

extern void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe);

extern dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *scene, const char *name);

extern void dds3EffectObjectBasicLinkObject(dds3Basic_t *basic, dds3Basic_t *target);

extern s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index);

extern s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj);

extern dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *pObjectBasic);

typedef struct polyMovieObject_t polyMovieObject_t;

extern s32 evtStopMover(polyMovieObject_t *mover, s32 sw, s32 baseframe, s32 nowframe);

extern const char evtViewerPathLinkMsg[];

extern s8 evtFrameListFieldTable[];

extern s32 PopMenu(void);

extern s32 strcmp(const char *a, const char *b);

extern char *strcpy(char *dst, const char *src);

extern void evtRunFrame(s32 nowframe);

extern dds3ProcessID_t *evtInitFrameVar(void);

extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);

extern void evtFreeStage(void);

extern s32 sdfDelayedResrcCheck(void);

extern void evtFreeViewerInfo(void);

extern void **dds3GetProcessData(void *id);

extern void sdfMemFreeDirect(void *aHandle);

extern s32 SearchNameTable(const char *name);

extern void evtStopFrame(s32 sw, u32 nowframe);

extern void *dds3SetActiveCameraSceneBasic(void *pSceneBasic, void *pCameraBasic);

extern f32 dds3CameraObjectBasicGetFovy(void *pBasic);

extern void dds3SetScreenFovy(f32 fovy);

extern void evtRunUnitMode(dds3Basic_t *unit, s32 mode);

extern void *sdfMemAllocDirect(s32 aSize);

extern void *sdfMemLock(void *aHandle);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer, dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);

extern dds3ProcessID_t *evtInitSky(void);

extern u32 dds3BackColor;

extern void PushMenu(s32 no, s32 x, s32 y);

extern void DelEffectTable_evtEventViewer(s32 index);

extern void DelBED(evtFrameInfo_t *frame);

extern s8 dds3PadButtonState[];

extern effScreenDrawParams_t *evtGetUnityCalcArea(void);

extern void dds3EffectBlur(void *area);

extern void Eve_PartialMultiBlurON(void);

extern void Eve_PartialMultiBlurOFF(void);

extern void Eve_PartialMultiFilterON(void);

extern void Eve_PartialMultiFilterOFF(void);

extern void Eve_DistortionBlurON(void);

extern void Eve_DistortionBlurOFF(void);

extern void Eve_RippleBlurON(void);

extern void Eve_RippleBlurOFF(void);

extern void Eve_FilterON(void);

extern void Eve_FilterOFF(void);

extern void evtMakeViewerPath(s32 kind);

extern void evtViewerShutdown(s32 kind);

extern u16 evt_file_minor;

extern u16 evtFileMajor;

extern void evtViewerStopFrame(void);

#include "sdk_libc.h"

extern void mdlUnRegist(s32 aMajor, s32 aMinor);

extern s32 evtViewerFrameSetMenuTable[];

extern s32 capwait;

extern void DrawEffect(void);

extern void FrameSelect(void);

extern dds3Basic_t *dds3GetActiveScene(void);

extern char *evtViewerFreeCameraName;

extern char *evtViewerGameCameraName;

extern char evtViewerCameraNameFormat[];

extern sdfGrOt_t sdfGrOtTable[];

extern sdfGrOt_t sdfDebugGrOt;

extern void *dds3FontReqAllocDrawTag(void);

extern s32 sdfDevPrintf(s32 aX, s32 aY, u32 aZ, s32 aAttr, const char *aStr, ...);

extern void sdfDrawTagLink(void *aTag, addr_t aPacket);

extern void SetActiveCamera(void);

extern saddr_t evtEventViewerProcess3(void);

extern saddr_t evtEventViewerProcess4(void);

void LinkViewerObject(evtViewerObject_s *obj);

void UnlinkViewerObject(evtViewerObject_s *obj);

void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

void DelViewerObject(evtViewerObject_s *obj);

/* Insert after existing equal keys, keeping ascending order and incrementing count. */
void LinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

/* Unlink an attached node, clear its links, and decrement the owning list's count. */
void UnlinkFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

/* Walk the linked list and reinsert the first out-of-order successor. */
void SortFrameInfo(evtViewerObject_s *parent);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer);

evtFrameInfo_t *evtGetSelectFrameInfo(void);

/* Release a queued node from its entry, freeing its buffer unless the entry id is 0x12. */
void DelFrameInfo(evtViewerObject_s *parent, evtFrameInfo_t *f);

void DelFrameInfo2(void);

/* Insert the track in ascending type order and bump the viewer's track count. */
void LinkViewerObject(evtViewerObject_s *obj);

void UnlinkViewerObject(evtViewerObject_s *obj);

evtViewerObject_s *evtAddViewerObject(s32 type);

s32 evtGetViewerObjectTypeCount(s32 type);

s32 evtGetAllFrameInfoCount(void);

void DelViewerObject(evtViewerObject_s *obj);

void evtSetFirstFrame(void);

void evtSetLastFrame(void);

void evtInitViewerInfo(void);

void evtFreeViewerInfo(void);

void evtUnRegistUnit(void);

s32 SearchNameTable(const char *name);

s32 evtAddNameTable(const char *name);

void *SearchBasicFromNameIndex_evtEventViewer(s32 index);

extern const char evtViewerPathLinkMsg[40];

void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe);

extern dds3Basic_t *dds3AddD3PEffectObjectBasic2(void *effdata, void *pos, void *rot);

extern dds3Basic_t *dds3AddMG12EffectObjectBasic2(s32 type, void *effdata);

extern s32 dds3EffectObjectPresetMG12(dds3Basic_t *eff);

extern s32 dds3EffectObjectSetMG12Pos(dds3Basic_t *eff, dds3Basic_t *p1,
                                    dds3Basic_t *p2, dds3Basic_t *p3, dds3Basic_t *p4);

s32 AddEffectTable2(s32 nowframe, evtViewerObject_s *object, evtFrameInfo_t *frame);

void DelEffectTable_evtEventViewer(s32 index);

void *AddBED(void *data, s32 cutno, s32 nameindex, s32 posindex, s32 baseframe);

void DelBED(evtFrameInfo_t *frame);

void DrawEffect(void);

void SetActiveCamera(void);

extern void evtEffectLoopControl(dds3Basic_t *effect, s32 sw);

extern s32 evtPlaySe(s32 eventNo, s32 soundNo);

extern void dds3SetQuake(s32 frame, s32 range);

extern void dds3StopQuake(void);

void RunFrame1(evtViewerObject_s *track, evtFrameInfo_t *frameInfo, s32 nowFrame, s32 length);

void OutFrame1(evtViewerObject_s *track, evtFrameInfo_t *node, s32 nowFrame, s32 length);

extern evtUnit_t *evtGetUnitHandle2(dds3Basic_t *basic);

extern void evtLinkLightObjectBasic(evtUnit_t *unit, dds3Basic_t *light,
                                    s32 frame);

extern void evtUnlinkLightObjectBasic(evtUnit_t *unit, s32 frame);

extern dds3Basic_t *dds3GetActiveScene(void);

void RunFrame3(s32 nowframe);

void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame);

extern void RunFrameUnit(evtViewerObject_s *track, s32 nowFrame);

extern void RunFrame2(evtViewerObject_s *track, evtFrameInfo_t *next,
                      evtFrameInfo_t *previous, s32 nowFrame, s32 *nearestFrame);

extern evtViewerObject_s *evt_camera;

extern s8 dds3ScreenBlurFlag;

void evtRunFrame(s32 nowFrame);

void DrawActiveCameraName(void);

void SelectActiveCamera(void);

extern void sdfMotionSeek(sdfMotionCtrlr_t *motion, f32 frame);

extern void sdfMotionPause(sdfMotionCtrlr_t *motion);

extern void sdfMotionPauseOff(sdfMotionCtrlr_t *motion);

void evtStopFrame(s32 sw, u32 nowFrame);

void evtViewerStopFrame(void);

void FrameSelect(void);

void PushMenu(s32 no, s32 x, s32 y);

s32 PopMenu(void);

u32 evtEventViewerProcess5_0(void);

extern const char D_0053B620[32];

s32 EnterMain(void);

s32 EnterSelect(s32 unused, s32 menuMode);

s32 EnterObjList(void);

extern const char evtFrameFadeBlackOut[16];

extern const char evtFrameFadeBlackIn[16];

extern const char evtFrameFadeWhiteOut[16];

extern const char evtFrameFadeWhiteIn[16];

extern const char evtViewerUnitVshadowOn[16];

extern const char evtViewerUnitMshadowOn[16];

extern const char evtViewerUnitShadowOff[16];

extern const char evtViewerUnitDispOff[16];

s32 EnterFrameChange(void);

s32 EnterIntFloatChange(void);

s32 EnterFilename2(void);

s32 EnterBasicList(s32 action, s32 previousMenu);

s32 EnterEventPos(void);

extern s16 winoff_x;

extern s32 evtViewerDispatch(s32 actionIndex, s32 x, s32 y);

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdowntimer);

extern s32 (*evtViewerMenuHandlers[])(s32 action, s32 previousMenu);

saddr_t evtEventViewerProcess4(void);

saddr_t evtEventViewerProcess3(void);

saddr_t evtEventViewerProcess2(void *id);

saddr_t evtEventViewerProcess1(void);

void evtEventViewerShutdown(void *id);

void evtInitEventViewer(void);

s32 evtKillEventViewer(void);

extern evtViewerInfo_t *viewerinfo;

extern u16 evtFileMajor;

extern u16 evt_file_minor;

extern s16 winoff_x;

extern evtViewerObject_s *evt_camera;

extern char *evtViewerFreeCameraName;

extern char *evtViewerGameCameraName;

extern char evtViewerCameraNameFormat[3];

extern s32 capwait;

extern char evtViewerStrNo[3];

extern char evtViewerStrYes[4];

extern char evtViewerStrOn[3];

extern char evtViewerStrOff[4];

extern char evtViewerStrSub[4];

extern char evtViewerStrAdd[4];

extern char evtViewerStrNormal[7];

extern char evtFrameFadeNameAllStop[8];

extern char evtFrameFadeNameVolUp[6];

extern char evtFrameFadeNameVolDown[8];

extern char evtFrameFadeNameTrans[6];

extern char evtFrameFadeNameFadeOut[8];

extern char evtFrameFadeNameFadeIn[7];

extern char evtFrameFadeNamePlay[5];

extern char evtViewerUnitControlNameDispOn[8];

extern char evtViewerProcNameSlight[7];

extern char evtViewerProcNameSfog[5];

extern char evtViewerProcNameFilter[7];

extern char evtViewerProcNameMfilter[8];
/* End of declarations from src/smt3/event/evtEventViewer.c */

INCLUDE_ASM(const s32, "event/evtEventViewer", LinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", UnlinkFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", SortFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AF498);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFB80);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFC40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFD28);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerProcessPending);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFE48);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFEE8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFF38);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001AFFB0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerCountEntries);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerSumNodeCounts);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B00A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0150);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0170);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0190);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0220);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B02A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0310);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0398);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B03F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0460);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B05A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B07F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0838);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelBED);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0950);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B09E0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0A58);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0BD8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0DC8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B0ED0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1000);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1068);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1250);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B19A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1A78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1B90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1D30);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1E78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B1F98);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B2180);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B2DC8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B2FB8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B3358);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewFindGlyphAtOrBefore);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerClampMovieTimes);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B3990);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B3CA8);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerApplyGlyphLodChannel);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerAttachIndexedObject);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B3FE0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B40B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B40E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4120);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerHasUpdateFlag);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4278);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B43B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B45D8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4608);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B46B0);

void func_001B48A8(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B48B0);

void func_001B4918(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerFindLatestMatchingMotionKey);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B49E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerDispatchFlagMode);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4CE8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4D68);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4DE8);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerPopHistory);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4E80);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4F00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B4F38);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerReturnZero);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F070);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F080);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F090);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F0A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B5100);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B52E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerPushMode5CommandHistory);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F2D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F2E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F2F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F300);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F310);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F320);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F330);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F340);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F350);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F360);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F370);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F380);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F390);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3A0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F3F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F400);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F410);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044F420);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B5778);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B67F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B6928);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdSetValue);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerStoreCommandInSelectedField);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdNoOp);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B6CE0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerCommitGroupSelection);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B6E30);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B6ED0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdCancelSelection);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewCmdResolveSlot);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7030);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7088);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7158);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerClearPendingNodeAndPushHistory);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044FAA0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7488);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtViewerPickNextHandler);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7790);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B78C8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B79F0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7A90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7D88);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7E78);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7EB8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7F48);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7FD8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B7FE0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerDestroyActive);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerDestroyActiveAlternate);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerInitCampFontResource);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B81F0);

INCLUDE_ASM(const s32, "event/evtEventViewer", evtEventViewerDestroyTask);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B82B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B82E0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8340);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8370);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B83A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8438);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8448);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8450);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8470);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B84B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B84E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001B8598);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_0044FC08);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6AA);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6AC);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6AE);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6B4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6BC);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6C4);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6D0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6D8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6E0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6E8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6F0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C6F8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C700);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C708);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C710);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C718);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C720);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C728);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C730);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C738);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C740);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C748);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C750);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C758);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C760);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C768);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C770);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C778);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C780);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C788);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C790);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C798);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7A0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7B0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7D0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7D8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7E0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_0046C7E8);

