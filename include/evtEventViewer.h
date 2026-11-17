#ifndef EVT_EVENT_VIEWER_H
#define EVT_EVENT_VIEWER_H

#include "common.h"

/* File formats used to split event-viewer tracks during serialization. */
typedef s32 EvtViewerTrackFileFormat;

enum {
    EVT_VIEWER_TRACK_FILE_FORMAT_PM2 = 2,
    EVT_VIEWER_TRACK_FILE_FORMAT_PM3 = 3
};

/* Sign-extended 32-bit view of the signed timeline-key parameter halfword:
 * bits 12-15 select a script integer, and bits 0-11 hold its required value.
 * Selector zero disables the condition. */
typedef s32 EvtViewerIndexedCondition;

/* The predicate returns only these two outcomes. */
typedef enum EvtViewerIndexedConditionResult {
    EVT_VIEWER_INDEXED_CONDITION_NOT_MATCHED = 0,
    EVT_VIEWER_INDEXED_CONDITION_MATCHED = 1
} EvtViewerIndexedConditionResult;

/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;

#include "basic.h"
#include "event.h"
#include "kernel.h"
#include "motion.h"

struct dds3ModelHandle_t;
struct evtViewerObject_t;

void LinkViewerObject(struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo);
void UnlinkViewerObject(struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo);
struct evtViewerObject_t *evtAddViewerObject(s32 type, evtViewerInfo_t *pInfo);
s32 AddEffectTable2(s32 nowframe, struct evtViewerObject_t *obj, evtFrameInfo_t *, evtViewerInfo_t *pInfo);
sdfMotionCtrlr_t *CreateMotion(mdlObj_t *aHandle, s32 aGroup);
void DelBED(evtFrameInfo_t *frame);
void DelEffectTable_evtEventViewer(s32 index, evtViewerInfo_t *pInfo);
void scrCallPictureSetEndProcess(s32 no);
void evtCallDelFrameInfo(struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo);
void DelFrameInfo(struct evtViewerObject_t *parent, evtFrameInfo_t *);
void DrawEffect(evtViewerInfo_t *pInfo);
s32 DrawMessage(evtViewerInfo_t *pInfo);
void DrawTmxTexture(evtViewerInfo_t *pInfo);
void DrawTmxTextureFrom2Frame(evtViewerInfo_t *pInfo, struct evtViewerObject_t *obj, evtFrameInfo_t *nearframe, evtFrameInfo_t *frame);
s32 End_evtViewer(evtViewerInfo_t *pInfo);
void EventSkipStart(evtViewerInfo_t *pInfo);
void FreeInfo(evtViewerInfo_t *pInfo);
void evtCallMemset7(evtViewerInfo_t *pInfo);
void evtEventViewerApplyEffectChannels(s32 nowframe, evtViewerInfo_t *pInfo);
s32 evtCallGetModelHandleFromUID(evtViewerInfo_t *pInfo, sdfTexHandle_t *pTex);
mdlObj_t *GetModelHandleFromUID(s32 UID, dds3Basic_t **unit);
void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe, evtViewerInfo_t *pInfo);
void LinkFrameInfo(struct evtViewerObject_t *parent, evtFrameInfo_t *);
void MessageManager(evtViewerInfo_t *pInfo);
void OutFrame1(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
dds3ProcessFunc_t Process2_evtViewer(dds3ProcessID_t *id);
dds3ProcessFunc_t Process3(dds3ProcessID_t *id);
dds3ProcessID_t *evtInitFrameVar(void);
void *AddBED(void *data, s32 cutno, s32 nameindex, s32 posindex, s32 baseframe, evtViewerInfo_t *pInfo);
void CalcLightCycle(pmdSceneLightTable_t *sl, evtFrameInfo_t *frame, s32 nowframe, evtViewerInfo_t *pInfo);
void ReqMessage(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
void RunFrame1(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
void RunFrame2(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, evtFrameInfo_t *nearframe, s32 nowframe, s32 *l, evtViewerInfo_t *pInfo, s32 offset);
void RunFrame3(s32 nowframe, evtViewerInfo_t *pInfo);
void RunFrameUnit(struct evtViewerObject_t *obj, s32 nowframe);
void evtRunFrameSub(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
void CalcNextMesSet(evtViewerInfo_t *pInfo);
dds3Basic_t *SearchBasicFromNameIndex(s32 nameindex, evtViewerInfo_t *pInfo);
s32 SearchNameTable(char *name, evtViewerInfo_t *pInfo);
void SetActiveCamera(evtViewerInfo_t *pInfo);
void SortFrameInfo(struct evtViewerObject_t *parent);
void UnlinkFrameInfo(struct evtViewerObject_t *parent, evtFrameInfo_t *);
s32 Validate1stAt3Bgm(evtViewerInfo_t *pInfo);
s32 ValidateAt3Bgm(evtViewerInfo_t *pInfo);
void ValidateMovie(evtViewerInfo_t *pInfo);
void ValidatePLTMX(evtViewerInfo_t *pInfo);
void ValidateRenderTex(evtViewerInfo_t *pInfo);
void ValidateUnitMotionFrame(s32 nowframe, evtViewerInfo_t *pInfo, s32 mode);
evtFrameInfo_t *evtAddFrameInfo_evtViewer(struct evtViewerObject_t *parent, s32 frame, evtViewerInfo_t *pInfo);
s32 evtAddNameTable(char *name, evtViewerInfo_t *pInfo);
struct evtViewerObject_t *evtAddViewerObject(s32 type, evtViewerInfo_t *pInfo);
void evtViewerDispatchFlagMode(evtViewerInfo_t *viewer);
s32 evtCheckProcess(void);
s32 evtCheckLastSkipStat(void);
void evtDelProcess(u32 UID);
void evtEnd(dds3ProcessID_t *id);
struct evtPolygonMovieWork_t;
s32 evtEndEvtViewerSub(struct evtPolygonMovieWork_t *wdata);
sdfTexHandle_t *evtCallMemAllocDirect(void);
void evtCallChipFreeDirect(void);
evtFrameInfo_t *evtGetSelectFrameInfo(evtViewerInfo_t *pInfo);
void evtIgnoreAtracPlay(dds3ProcessID_t *id);
void evtChgSeq(dds3ProcessID_t *id, s32 seqno);
s32 evtSetFrame(dds3ProcessID_t *id, s32 frame);
void oldEventSceneUnload(struct evtPolygonMovieWork_t *wdata);
sdfTexHandle_t *evtCallMemAllocDirect(void);
void evtIgnoreFade(dds3ProcessID_t *id);
void evtMessageDelete(evtViewerInfo_t *pInfo, s32 Mode);
s32 rcvIsHealPhase(struct evtPolygonMovieWork_t *wdata);
void evtNoSkipMes(dds3ProcessID_t *id);
dds3ProcessFunc_t evtProcess2_RealPlay(dds3ProcessID_t *id);
void evtRunFrame(s32 nowframe, evtViewerInfo_t *pInfo);
void PushMenu(s32 a0, s32 a1, s32 a2, evtViewerInfo_t *viewer);
void evtUnRegistUnit(evtViewerInfo_t *pInfo);
void evtUnitMotionChange(s32 nowframe, dds3Basic_t *b, struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo, s32 mode);
void UnitMotionChangeForWait(s32 nowframe, dds3Basic_t *b, struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo);
void evtValidateUnitMotionFrameForWait(s32 nowframe, evtViewerInfo_t *pInfo);
void evtViewerStopFrame(s32 sw, u32 nowframe, evtViewerInfo_t *pInfo);
/* A zero selector matches unconditionally; otherwise the integer at
 * script.ints[199 + selector] must equal the encoded 12-bit value. */
EvtViewerIndexedConditionResult sndVoiceStatus(EvtViewerIndexedCondition condition);

/* Chip list at evtWork2: evtCallChipAllocDirect shifts the
 * ten 0x44-byte entries down by one when count reaches ten. */
typedef struct evtWork2Entry_t {
    u8 unk0[0x30];
    void *chip;
    u8 unk34[0x10];
} evtWork2Entry_t;

typedef struct evtWork2_t {
    u32 unk0;
    evtWork2Entry_t info[10];
    s32 count;
    s32 unk2B0;
} evtWork2_t;

extern evtWork2_t evtWork2;
extern dds3ProcessID_t *pmv_pid;
extern s32 last_skip_stat;
extern struct evtViewerObject_t *evt_camera;

dds3TaskUpdateResult_t evtCallGetProcessData11(dds3ProcessID_t *task);
void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task);
void evtEventViewerInitCampFontResource(evtViewerInfo_t *viewer);
void DelProcessAll(void);

#endif
