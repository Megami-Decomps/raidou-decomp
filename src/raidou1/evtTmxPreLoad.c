#include "common.h"
#include "kernel.h"
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

s32 GetIndexFromUID_evtTmxPreLoad(evtTmxPreLoad_t *pData, u32 id)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        if (pData->ids[i] == id) {
            return i;
        }
    }
    return -1;
}

/* NON_MATCHING: 2 of 19 words match.  */
#ifdef NON_MATCHING
s32 evtLeaf28(evtTmxPreLoad_t *pData)
{
    s32 used[2];
    s32 i;

    used[0] = 0;
    used[1] = 0;

    for (i = 0; i < 2; i++) {
        if (pData->order[i] != 0) {
            used[pData->order[i] - 1] = 1;
        }
    }
    return used[0] != 0;
}
#else
INCLUDE_ASM(const s32, "evtTmxPreLoad", evtLeaf28);
#endif

/* NON_MATCHING: 43 of 96 words match.  */
#ifdef NON_MATCHING
void EndCallback_evtTmxPreLoad(fileRequest_t *req, evtTmxPreLoad_t *pData)
{
    s32 fromHead;
    u32 loadSize;
    sdfMemHandle_t *handle;
    TMX_HEADER_t *tmx;
    u32 fileSize;

    fromHead = evtLeaf28(pData);
    pData->order[pData->count - 1] = fromHead + 1;
    loadSize = fileGetLoadSize(req);
    handle = fileGetMemHandle(req);
    tmx = sdfMemLock(handle);
    if (fromHead == 0) {
        pData->tex[pData->count - 1] = sdfTexSet(tmx);
    } else {
        pData->tex[pData->count - 1] = sdfTexSetFromHead(tmx);
    }
    fileSize = tmx->fileSize;
    tmx = (TMX_HEADER_t *)((u8 *)tmx + fileSize);
    if (fileSize < loadSize) {
        if (fromHead == 0) {
            pData->maskTex[pData->count - 1] = sdfTexSet(tmx);
        } else {
            pData->maskTex[pData->count - 1] = sdfTexSetFromHead(tmx);
        }
        pData->maskOfs[pData->count - 1][0] = (s16)((tmx->maskOfs[0][0] << 8) | tmx->maskOfs[0][1]);
        pData->maskOfs[pData->count - 1][1] = (s16)((tmx->maskOfs[1][0] << 8) | tmx->maskOfs[1][1]);
    }
    sdfMemFreeDirect(handle);
    fileHandleFree(req);
    pData->flags = (pData->flags & ~1) | 2;
}
#else
INCLUDE_ASM(const s32, "evtTmxPreLoad", EndCallback_evtTmxPreLoad);
#endif
