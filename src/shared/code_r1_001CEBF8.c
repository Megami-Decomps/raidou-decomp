#include "common.h"
#include "sdfMemory.h"
#include "sdk_libc.h"
#include "evtTmxPreLoad.h"

evtTmxPreLoad_t *Create_evtTmxPreLoad(void) {
    sdfMemHandle_t *handle;
    evtTmxPreLoad_t *pData;

    handle = sdfMemAllocDirect(sizeof(evtTmxPreLoad_t));
    pData = sdfMemLock(handle);
    memset(pData, 0, sizeof(evtTmxPreLoad_t));
    pData->memHandle = handle;
    return pData;
}
