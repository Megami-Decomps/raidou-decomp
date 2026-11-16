#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "evtTmxPreLoad.h"

s32 evtChkLoad(s32 Mode) {
    evtTmxPreLoad_t *pData;

    if (TmxPreLoadPID == NULL) {
        return 0;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    if (Mode != 0 && pData->que.count > 0) {
        return 1;
    }
    if (pData->flags & 1) {
        return 1;
    }
    return 0;
}

s32 evtRequest(u32 UID, s32 ReqFrame) {
    evtTmxPreLoad_t *pData;
    s32 i;

    if (TmxPreLoadPID == NULL) {
        return 0;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    if ((u16)UID == 0) {
        return 0;
    }
    for (i = 0; i < pData->que.count; i++) {
        if (pData->que.id[i] == UID) {
            return 0;
        }
    }
    if (pData->que.count < 2) {
        pData->que.count++;
    }
    pData->que.id[pData->que.count - 1] = UID;
    pData->que.arg[pData->que.count - 1] = ReqFrame;
    return 1;
}

void *evtGetTex(u32 UID) {
    evtTmxPreLoad_t *pData;
    s32 i;

    if (TmxPreLoadPID == NULL) {
        return NULL;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    i = GetIndexFromUID_evtTmxPreLoad(pData, UID);
    if (i < 0) {
        return NULL;
    }
    return pData->tex[i];
}

void *evtGetMaskTex(u32 UID) {
    evtTmxPreLoad_t *pData;
    s32 i;

    if (TmxPreLoadPID == NULL) {
        return NULL;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    i = GetIndexFromUID_evtTmxPreLoad(pData, UID);
    if (i < 0) {
        return NULL;
    }
    return pData->maskTex[i];
}

s32 evtGetMaskOfs(u32 UID, sdfIVector2_t *pOfs) {
    evtTmxPreLoad_t *pData;
    s32 i;

    if (TmxPreLoadPID == NULL) {
        return 0;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    i = GetIndexFromUID_evtTmxPreLoad(pData, UID);
    if (i < 0) {
        return 0;
    }
    pOfs->x = pData->maskOfs[i][0];
    pOfs->y = pData->maskOfs[i][1];
    return 1;
}

s32 Process_evtTmxPreLoad(dds3ProcessID_t *PID) {
    evtTmxPreLoad_t *pData;

    if (TmxPreLoadPID == NULL) {
        return 0;
    }
    pData = dds3GetProcessData(TmxPreLoadPID);
    if (evtChkLoad(0) != 0) {
        return 0;
    }
    if (pData->unk8 & 1) {
        return -1;
    }
    Start_evtTmxPreLoad(pData);
    return 0;
}

s32 evtProcessStart(dds3ProcessID_t *ParentPID, DDS3_PRIORITY Priority) {
    if (TmxPreLoadPID != NULL) {
        return 1;
    }
    TmxPreLoadPID = dds3StartProcess("TmxPreLoad", Priority, 0, 0, Process_evtTmxPreLoad,
        (dds3ProcessFunc_t)Destroy_evtTmxPreLoad, Create_evtTmxPreLoad());
    if (ParentPID != NULL) {
        dds3LinkProcess(ParentPID, TmxPreLoadPID);
    }
    return 0;
}

void evtProcessEnd(void) {
    evtTmxPreLoad_t *pData;

    if (TmxPreLoadPID != NULL) {
        pData = dds3GetProcessData(TmxPreLoadPID);
        pData->unk8 |= 1;
    }
}

INCLUDE_ASM(const s32, "code_001CF388", evtTexFree);
