#include "common.h"
#include "sdk_libc.h"
#include "fileManager.h"
#include "mapLbWork.h"

extern void datDebugPrintf(const char *fmt, ...);
extern void fldCallMemFreeDelayedDirect4(void);
extern void mapCallMemFreeDirect(void);

/* NON_MATCHING: 39 of 203 words match. Register allocation is shifted from the first instruction, control flow is the same. */
#ifdef NON_MATCHING
void fldJmsQuestMapLoad_fld(s32 area, s32 sub) {
    char path[32];
    fileRequest_t *req;
    sdfPacDecodeResrc_t *res;
    sdfMemHandle_t *handle;
    u8 *data;
    u8 *list;
    mapLbHead_t *head;
    s32 idx;
    s32 i;
    s32 x;
    s32 y;
    s32 n;

    idx = 0;
    head = NULL;
    fldCallMemFreeDelayedDirect4();
    if (fldJmsQuestMapLoad_fldData != NULL) {
        mapCallMemFreeDirect();
        fldJmsQuestMapLoad_fldData = NULL;
        fldJmsQuestMapLoad_fldData2 = NULL;
    }
    datDebugPrintf("LOAD PAC%02d\n", sub);
    fileSyncAll();
    if (area == 9 && sub >= 11) {
        sub = 10;
    }
    sprintf(path, "/map/map%02d/PAC%02d.LB", area, sub);
    req = pacLoad(path);
    pacSync(req);
    res = req->pac.resrcHead;
    if (res != NULL) {
        handle = fldJmsQuestMapLoad_fldData;
        data = fldJmsQuestMapLoad_fldData2;
        while (res != NULL) {
            if (idx == 0) {
                head = (mapLbHead_t *)res->data;
                evtStageTestState2.handle = res->resrc;
            } else if (idx == 1) {
                handle = res->resrc;
                data = res->data;
            }
            res = res->next;
            idx++;
        }
        fldJmsQuestMapLoad_fldData2 = data;
        fldJmsQuestMapLoad_fldData = handle;
    }
    pacFileHandleFree_(req);
    evtStageTestState2.head = head;
    list = (u8 *)head + 16;
    n = head->w * head->h;
    for (i = 0; i < n; i++) {
        evtStageTestState2.entry[i] = list;
        list += head->entrySize & 0x7FFFFFFF;
    }
    fldData23 = 0;
    if (evtStageTestState2.head->entrySize >= 0) {
        evtStageTestState2.sub = NULL;
        for (i = 80; i >= 0; i--) {
            evtStageTestState2.cell[i] = NULL;
        }
    } else {
        evtStageTestState2.sub = (mapLbHead_t *)list;
        list += 0x30;
        fldData23 = 1;
        n = 0;
        for (y = 0; y < evtStageTestState2.sub->h; y++) {
            for (x = 0; x < evtStageTestState2.sub->w; x++) {
                if ((evtStageTestState2.sub->mask[y] >> x) & 1) {
                    evtStageTestState2.cell[n] = list;
                    list += evtStageTestState2.sub->entrySize & 0x7FFFFFFF;
                } else {
                    evtStageTestState2.cell[n] = NULL;
                }
                n++;
            }
        }
    }
    if (fldData[(area << 6) + sub] != 0) {
        data = fldJmsQuestMapLoad_fldData2;
        fldCallSetOt3Data = NULL;
        fldData21 = (mapLbIndex_t *)data;
        mapSKYBGEDITData4 = (mapLbSprite_t *)(data + 16);
        for (i = 99; i >= 0; i--) {
            fldData5[i] = NULL;
        }
        if (fldData21->sectionOfs != 0) {
            fldCallSetOt3Data = (mapLbSection_t *)(data + fldData21->sectionOfs);
            for (i = 0; i < 100; i++) {
                if (fldCallSetOt3Data->count[i] != 0) {
                    fldData5[i] = (mapLbSprite_t *)(data + fldCallSetOt3Data->ofs[i]);
                }
            }
        }
    }
}
#else
INCLUDE_ASM(const s32, "code_r1_00156B68", fldJmsQuestMapLoad_fld);
#endif
