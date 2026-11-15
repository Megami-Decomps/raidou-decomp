#ifndef EVTTMXPRELOAD_H
#define EVTTMXPRELOAD_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"

struct evtTmxPreLoadQue_t;
struct evtTmxPreLoad_t;

s32 GetTblIndex(s32 EventNo);
s32 SelectFreeTex(struct evtTmxPreLoad_t *pData);
void Start_evtTmxPreLoad(struct evtTmxPreLoad_t *pData);
void UpdateQue(struct evtTmxPreLoadQue_t *pQue);
s32 evtChkLoad(s32 Mode);
s32 evtGetFileNums(s32 EventNo);
s32 evtGetMaskOfs(u32 UID, sdfIVector2_t *pOfs);
void *evtGetMaskTex(u32 UID);
void *evtGetTex(u32 UID);
void evtProcessEnd(void);
s32 evtProcessStart(dds3ProcessID_t *ParentPID, DDS3_PRIORITY Priority);
s32 evtRequest(u32 UID, s32 ReqFrame);
void evtTexFree(void);

#endif
