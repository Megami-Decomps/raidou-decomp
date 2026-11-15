#ifndef EVTTMXPRELOAD_H
#define EVTTMXPRELOAD_H

#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "fileManager.h"

typedef struct evtTmxPreLoadQue_t {
    u32 id[2];
    u32 arg[2];
    u32 state[2];
    s32 count;
} evtTmxPreLoadQue_t;

typedef struct evtTmxPreLoad_t {
    u32 unk0;
    u32 flags;
    u32 unk8;
    s32 count;
    sdfTexHandle_t *tex[2];
    sdfTexHandle_t *maskTex[2];
    s32 size[2][2];
    u32 ids[2];
    u32 order[2];
    u32 arg[2];
    evtTmxPreLoadQue_t que;
} evtTmxPreLoad_t;

/* One 12-byte row per event; retail labels +4 and +8 of the table as separate symbols. */
typedef struct evtTmxPreLoadEntry_t {
    s32 eventNo;
    s32 fileNums;
    char **names;
} evtTmxPreLoadEntry_t;

extern evtTmxPreLoadEntry_t GetTblIndexData[27];
extern dds3ProcessID_t *TmxPreLoadPID;

s32 CheckActionProcessEnd(evtTmxPreLoad_t *pData, u32 id);
void evtCallGetLoadSize(fileRequest_t *req, evtTmxPreLoad_t *pData);

s32 GetTblIndex(s32 EventNo);
s32 SelectFreeTex(evtTmxPreLoad_t *pData);
void Start_evtTmxPreLoad(evtTmxPreLoad_t *pData);
void UpdateQue(evtTmxPreLoadQue_t *pQue);
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
