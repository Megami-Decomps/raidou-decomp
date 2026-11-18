#include "common.h"
#include "evtCutIn.h"
#include "evtPicture.h"
#include "evt.h"
#include "dds3KernelCore.h"
#include "evtTmxPreLoad.h"

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGrAllocPacketBuff2);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallMemAllocDirect3);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData23);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData24);

INCLUDE_ASM(const s32, "code_001E37C0", CrossFadeShutdown);

INCLUDE_ASM(const s32, "code_001E37C0", evtStartCrossFade);

INCLUDE_ASM(const s32, "code_001E37C0", evtCrossFadeGetStat);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetMemHandle2);

INCLUDE_ASM(const s32, "code_001E37C0", SetJyocyuData);

INCLUDE_ASM(const s32, "code_001E37C0", evtLeaf24);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallDrawPanelSprite);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData25);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData26);

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData27);

INCLUDE_ASM(const s32, "code_001E37C0", evtStartRain);

void evtCallGetProcessData28(void) {
    evtTmxPreLoad_t *pData;

    if (evtStartRainData2 != NULL) {
        pData = dds3GetProcessData(evtStartRainData2);
        pData->flags |= 2;
    }
}

INCLUDE_ASM(const s32, "code_001E37C0", evtCallGetProcessData29);

INCLUDE_ASM(const s32, "code_001E37C0", scrCallGetProcessData);

s32 GetIndexFromUID(evtCutInData_t *pData, u32 UID) {
    s32 index;
    s32 i;

    index = -1;
    if (UID == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        if (pData->cutin_uid[i] == UID) {
            index = i;
            break;
        }
    }
    return index;
}

void AddUID(s32 _restart, dds3ProcessID_t *pid) {
    s32 index;
    s32 i;

    index = -1;
    if (_restart == 0) {
        return;
    }
    for (i = 0; i < 16; i++) {
        if (Data_evtCutIn.cutin_uid[i] == 0) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        Data_evtCutIn.cutin_uid[index] = _restart;
        Data_evtCutIn.cutin_pid[index] = pid;
    }
}

void DelUID(u32 UID) {
    s32 i;

    i = GetIndexFromUID(&Data_evtCutIn, UID);
    if (i >= 0) {
        Data_evtCutIn.cutin_uid[i] = 0;
        Data_evtCutIn.cutin_pid[i] = NULL;
    }
}

s32 GetRegistNums(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 16; i++) {
        if (Data_evtCutIn.cutin_uid[i] != 0) {
            count++;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "code_001E37C0", evtChkUID);

dds3ProcessID_t *evtGetPIDfromUID(u32 UID) {
    s32 i;

    i = GetIndexFromUID(&Data_evtCutIn, UID);
    if (i >= 0) {
        return Data_evtCutIn.cutin_pid[i];
    }
    return NULL;
}

INCLUDE_ASM(const s32, "code_001E37C0", scrCallPictureSetEndProcess);

void DelProcessAll(void) {
    s32 i;
    u32 uid;

    for (i = 0; i < 16; i++) {
        uid = Data_evtCutIn.cutin_uid[i];
        if (uid != 0) {
            evtPictureSetEndProcess(Data_evtCutIn.cutin_pid[i]);
            DelUID(uid);
        }
    }
}

void Callback_evtCutIn(u32 UID) {
    DelUID(UID);
}

s32 DestructProcess_evtCutIn(dds3ProcessID_t *PID) {
    return 0;
}

void DestructDestroy_evtCutIn(dds3ProcessID_t *PID) {
    DelProcessAll();
}

INCLUDE_ASM(const s32, "code_001E37C0", evtStart);

dds3ProcessID_t *evtStartCommand(dds3ProcessID_t *ParentPID, s32 X, s32 Y, s32 Type, u32 UID) {
    u32 type;
    dds3ProcessID_t *pid;
    evtPictureProcessData_t *data;

    switch (Type) {
    case 0:
        type = 0x10001;
        break;
    case 1:
        type = 0x10002;
        break;
    case 2:
        type = 0x20001;
        break;
    case 3:
        type = 0x20002;
        break;
    default:
        type = 0x30002;
        break;
    }
    pid = evtStart(NULL, ParentPID, X * 16, Y * 8, 11004, 0, 0, type, UID);
    data = dds3GetProcessData(pid);
    data->flag |= 2;
    return pid;
}

void evtEndCommand(dds3ProcessID_t *PID) {
    evtPictureProcessData_t *data;

    if (PID != NULL) {
        data = dds3GetProcessData(PID);
        data->flag |= 4;
    }
}
