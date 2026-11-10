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

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5560);

evtFrameInfo_t *AddFrameInfo2(evtViewerObject_s *group, s32 frame, evtViewerInfo_t *viewer)
{
    return evtAddFrameInfo(group, frame);
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5C48);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelFrameInfo);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5D08);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5DF0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5ED0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5F10);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C5FB0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6000);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6078);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C60B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C60E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6168);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6218);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6238);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6258);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C62E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6368);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C63D8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6460);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C64C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6528);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6668);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C68C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6900);

INCLUDE_ASM(const s32, "event/evtEventViewer", DelBED);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6A18);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6AA8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6B20);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6CA0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6E90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C6F98);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C70C8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7130);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7318);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7A68);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7B40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7C58);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7DF8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C7F40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C8060);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C8248);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C8E90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9080);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9420);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C98C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9940);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9A58);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9D70);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9E20);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001C9F20);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA0A8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA180);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA1B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA1E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA330);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA340);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA478);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA6A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA6D0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA778);

void func_001CA970(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA978);

void func_001CA9E0(void) {
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CA9E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAAB0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAD68);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CADB0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAE30);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAEB0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAF18);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAF48);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CAFC8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB000);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB1C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5240);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5250);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5260);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5270);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB1C8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB3B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB810);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54A0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B54F0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5500);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5510);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5520);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5530);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5540);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5550);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5560);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5570);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5580);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5590);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55A0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55B0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55C0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55D0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55E0);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B55F0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CB840);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CC8C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CC9F0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCC90);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCCF8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCDA0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCDA8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCE00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCEF8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CCF98);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD000);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD020);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD0F8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD150);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD220);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD500);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5C70);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD550);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD830);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD858);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CD990);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CDAB8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CDB58);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CDE50);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CDF40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CDF80);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE010);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE0A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE0A8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE260);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE280);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE2A0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE2B8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_001CE358);

INCLUDE_RODATA(const s32, "event/evtEventViewer", D_004B5DD8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF910);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF912);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF914);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF916);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF91C);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF920);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF924);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF928);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF92C);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF930);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF938);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF940);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF948);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF950);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF958);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF960);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF968);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF970);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF978);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF980);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF988);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF990);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF998);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9A0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9A8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9B0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9B8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9C0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9C8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9D0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9D8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9E0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9E8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9F0);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DF9F8);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA00);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA08);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA10);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA18);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA20);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA28);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA30);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA38);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA40);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA48);

INCLUDE_SDATA(const s32, "event/evtEventViewer", D_004DFA50);

