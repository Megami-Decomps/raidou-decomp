/* Declarations of smt3-decomp src/smt3/event/evtEventViewer.c, for its bodies in this directory. */
#ifndef SHARED_EVENT_EVTEVENTVIEWER_H
#define SHARED_EVENT_EVTEVENTVIEWER_H

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

extern char evtViewerPathLinkMsg[];

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

SYMBOL_AT(sdfGrOt_t, sdfDebugGrOt, sdfGrOtTable, 0xA60);

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

s32 EnterMain(void);

s32 EnterSelect(s32 unused, s32 menuMode);

s32 EnterObjList(void);

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

#endif
