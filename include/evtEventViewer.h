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

/* A zero selector matches unconditionally; otherwise the integer at
 * script.ints[199 + selector] must equal the encoded 12-bit value. */
EvtViewerIndexedConditionResult evtViewerTestIndexedCondition(
    EvtViewerIndexedCondition condition);

/* Callback result: 0 retains the callback, -1 finishes the task, and
 * other words select a replacement callback. */
typedef s32 dds3TaskUpdateResult_t;

#include "basic.h"
#include "event.h"
#include "kernel.h"
#include "motion.h"

struct dds3ModelHandle_t;
struct evtViewerObject_t;

s32 AddEffectTable2(s32 nowframe, struct evtViewerObject_t *obj, evtFrameInfo_t *, evtViewerInfo_t *pInfo);
sdfMotionCtrlr_t *CreateMotion(struct dds3ModelHandle_t *aHandle, s32 aGroup);
s32 DelBED(u32 UID);
void DelFrameInfo(struct evtViewerObject_t *parent, evtFrameInfo_t *);
void DrawEffect(evtViewerInfo_t *pInfo);
s32 DrawMessage(evtViewerInfo_t *pInfo);
void DrawTmxTexture(evtViewerInfo_t *pInfo);
s32 DrawTmxTextureFrom2Frame(evtViewerInfo_t *pInfo, struct evtViewerObject_t *obj, evtFrameInfo_t *nearframe, evtFrameInfo_t *frame);
void End_evtViewer(evtViewerInfo_t *pInfo);
void EventSkipStart(evtViewerInfo_t *pInfo);
void FreeInfo(evtViewerInfo_t *pInfo);
struct dds3ModelHandle_t *GetModelHandleFromUID(s32 UID, dds3Basic_t *unit);
void LinkEffectBasic(dds3Basic_t *eff, s32 nameindex, s32 posindex, s32 baseframe, evtViewerInfo_t *pInfo);
void LinkFrameInfo(struct evtViewerObject_t *parent, evtFrameInfo_t *);
void MessageManager(evtViewerInfo_t *pInfo);
void OutFrame1(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
dds3ProcessFunc_t Process2_evtViewer(dds3ProcessID_t *id);
dds3ProcessFunc_t Process3(dds3ProcessID_t *id);
void ReqMessage(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
void RunFrame1(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, s32 nowframe, s32 length, evtViewerInfo_t *pInfo);
void RunFrame2(struct evtViewerObject_t *obj, evtFrameInfo_t *frame, evtFrameInfo_t *nearframe, s32 nowframe, s32 *l, evtViewerInfo_t *pInfo, s32 offset);
void RunFrame3(s32 nowframe, evtViewerInfo_t *pInfo);
void RunFrameUnit(struct evtViewerObject_t *obj, s32 nowframe);
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
s32 evtCheckProcess(void);
void evtDelProcess(u32 UID);
void evtEnd(dds3ProcessID_t *id);
dds3ProcessID_t *evtGetPIDfromUID(u32 UID);
void evtIgnoreAtracPlay(dds3ProcessID_t *id);
void evtIgnoreFade(dds3ProcessID_t *id);
void evtMessageDelete(evtViewerInfo_t *pInfo, s32 Mode);
void evtNoSkipMes(dds3ProcessID_t *id);
dds3ProcessFunc_t evtProcess2_RealPlay(dds3ProcessID_t *id);
void evtRunFrame(s32 nowframe, evtViewerInfo_t *pInfo);
void evtUnRegistUnit(evtViewerInfo_t *pInfo);
void evtUnitMotionChange(s32 nowframe, dds3Basic_t *b, struct evtViewerObject_t *obj, evtViewerInfo_t *pInfo, s32 mode);
void evtValidateUnitMotionFrameForWait(s32 nowframe, evtViewerInfo_t *pInfo);
void evtViewerStopFrame(s32 sw, u32 nowframe, evtViewerInfo_t *pInfo);
char *sndVoiceStatus(char *lbl);

dds3TaskUpdateResult_t evtCallGetProcessData11(dds3ProcessID_t *task);
void evtEventViewerDestroyActiveAlternate(dds3ProcessID_t *task);
void evtEventViewerInitCampFontResource(evtViewerInfo_t *viewer);

#endif
