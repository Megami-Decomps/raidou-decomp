#include "common.h"
#include "dds3KernelCore.h"
#include "sdfMemory.h"
#include "sdfTex.h"
#include "evtTmxPreLoad.h"

void Destroy_evtTmxPreLoad(dds3ProcessID_t *PID)
{
    evtTmxPreLoad_t *pData = dds3GetProcessData(PID);
    s32 i;

    for (i = 0; i < 2; i++) {
        if (pData->tex[i] != NULL) {
            sdfTexReleaseReference(pData->tex[i]);
        }
        if (pData->maskTex[i] != NULL) {
            sdfTexReleaseReference(pData->maskTex[i]);
        }
    }
    sdfMemFreeDirect(pData->memHandle);
    TmxPreLoadPID = NULL;
}

INCLUDE_ASM(const s32, "evtTmxPreLoad", GetIndexFromUID_evtTmxPreLoad);

INCLUDE_ASM(const s32, "evtTmxPreLoad", evtLeaf28);

INCLUDE_ASM(const s32, "evtTmxPreLoad", EndCallback_evtTmxPreLoad);
