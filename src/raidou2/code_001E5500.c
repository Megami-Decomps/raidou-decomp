#include "common.h"
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

INCLUDE_ASM(const s32, "code_001E5500", evtRequest);

INCLUDE_ASM(const s32, "code_001E5500", evtGetTex);

INCLUDE_ASM(const s32, "code_001E5500", evtGetMaskTex);

INCLUDE_ASM(const s32, "code_001E5500", evtGetMaskOfs);

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

INCLUDE_ASM(const s32, "code_001E5500", evtTexFree);
