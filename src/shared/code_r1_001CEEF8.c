#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "evtTmxPreLoad.h"
#include "sdfTex.h"
#include "fileManager.h"
#include "sdk_libc.h"

s32 GetTblIndex(s32 EventNo) {
    u32 i;

    if (TmxPreLoadPID == NULL) {
        return -1;
    }
    dds3GetProcessData(TmxPreLoadPID);
    for (i = 0; i < 27; i++) {
        if (EventNo == GetTblIndexData[i].eventNo) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "code_r1_001CEEF8", evtGetFileNums);

INCLUDE_ASM(const s32, "code_r1_001CEEF8", UpdateQue);

INCLUDE_ASM(const s32, "code_r1_001CEEF8", SelectFreeTex);

#ifdef NON_MATCHING
/* Draft: not yet compared with retail (the type of evtCallGetLoadSize and the word cleared after ids[count] are unconfirmed). */
void Start_evtTmxPreLoad(evtTmxPreLoad_t *pData) {
    char path[64];
    s32 i;

    if (pData->que.count == 0) {
        return;
    }
    for (i = 0; i < pData->que.count; i++) {
        u32 id = pData->que.id[i];
        u32 arg = pData->que.arg[i];
        u32 eventNo = id >> 16;
        s32 index = GetTblIndex(eventNo);
        s32 slot;

        if (index < 0) {
            continue;
        }
        slot = CheckActionProcessEnd(pData, id);
        if (slot >= 0) {
            pData->arg[slot] = arg;
            pData->que.state[i] = 1;
            continue;
        }
        if (pData->flags & 1) {
            continue;
        }
        sprintf(path, "%se%d/%s", "/e_haikei/", eventNo, GetTblIndexData[index].names[id & 0xFFFF]);
        if (pData->count == 2) {
            s32 j;

            slot = SelectFreeTex(pData);
            if (pData->tex[slot] != NULL) {
                sdfTexReleaseReference(pData->tex[slot]);
            }
            if (pData->maskTex[slot] != NULL) {
                sdfTexReleaseReference(pData->maskTex[slot]);
            }
            for (j = slot; j < pData->count - 1; j++) {
                pData->tex[j] = pData->tex[j + 1];
                pData->maskTex[j] = pData->maskTex[j + 1];
                pData->ids[j] = pData->ids[j + 1];
                pData->order[j] = pData->order[j + 1];
                pData->arg[j] = pData->arg[j + 1];
            }
            pData->count--;
            pData->size[pData->count][0] = 0;
            pData->size[pData->count][1] = 0;
            pData->tex[pData->count] = NULL;
            pData->maskTex[pData->count] = NULL;
        }
        pData->ids[pData->count] = id;
        pData->ids[pData->count + 1] = 0;
        pData->arg[pData->count] = arg;
        pData->count++;
        fileLoadEx(path, 0, EE_ADDR(evtCallGetLoadSize), EE_ADDR(pData));
        pData->flags |= 1;
        pData->que.state[i] = 1;
    }
    UpdateQue(&pData->que);
}
#else
INCLUDE_ASM(const s32, "code_r1_001CEEF8", Start_evtTmxPreLoad);
#endif
