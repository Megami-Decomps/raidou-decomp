#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "jmsMisc.h"
#include "TitleMain.h"
#include "newdata.h"
#include "sdk_libc.h"
#include "cfg.h"
#include "dat.h"
#include "fldProcess.h"
#include "dds3AdminiProcess.h"
#include "sdfSound.h"
#include "dds3PadManager.h"
#include "code_r1_001CC1D0.h"

extern jmsSeqUnit_t rstData2;
extern jmsSeqUnit_t rstCallGetProcessDataData;
extern jmsSeqUnit_t rstCallGetProcessDataData2;

s32 rstCallGetProcessData31(dds3ProcessID_t *PID)
{
    titleData_t *data = dds3GetProcessData(PID);

    data->Timer = 0;
    return 1;
}

/* body: matched in raidou @ 6575c15: src/raidou1/code_002F5BD0.c:413, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstLeaf11(dds3ProcessID_t *PID) {
    return 1;
}

s32 rstCallGetProcessData(dds3ProcessID_t *PID) {
    titleData_t *data;
    s32 ret;

    data = dds3GetProcessData(PID);
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    evtCallPlay(25, 5400, 30, 1, data);
    if (TitleCalcCursor(&data->Cursor)) {
        data->Timer = 0;
    }
    if (dds3PadButtonState[0x21] < 0) {
        data->Timer = 1200;
    }
    if (data->Timer < 1200) {
        if (dds3PadButtonState[0x23] < 0 || dds3PadButtonState[0x2C] < 0) {
            switch (data->Cursor.Index) {
            case 0:
            case 1:
                jmsSeqJumpAndFree(&data->seqUnit, &rstCallGetProcessDataData);
                break;
            case 2:
                jmsSeqJumpAndFree(&data->seqUnit, &rstCallGetProcessDataData2);
                p2sdPlaySE(0, 127, 63);
                break;
            }
        }
    } else {
        jmsSeqJumpAndFree(&data->seqUnit, &rstData2);
        p2sdPlaySE(1, 127, 63);
    }
    data->Timer++;
    return 0;
}

s32 rstCallGetProcessData32(dds3ProcessID_t *PID) {
    titleData_t *data;
    s32 ret;

    data = dds3GetProcessData(PID);
    rstFunc(data);
    evtFunc11(&data->Cursor, data->sprite);
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

/* body: matched in raidou @ 6575c15: src/raidou1/code_002F5BD0.c:413, found, same function 1.00 (words 2/2, calls 1.00, shape 1.00) */
s32 rstLeaf12(dds3ProcessID_t *PID) {
    return 1;
}

INCLUDE_ASM(const s32, "code_002F6C00", TitleEndWaitCalcProcess);

void rstCallMemset(void) {
    sdfMemHandle_t *handle;
    s32 field;

    handle = DDS3_GBWK->memHandle;
    memset(DDS3_GBWK, 0, sizeof(dds3GlobalWork_t));
    DDS3_GBWK->memHandle = handle;
    cfgCopyFlagBakToGbl();
    dds3GlobalWorkUserInit2(0);
    cfgChkFlagChange();
    field = 690;
    dds3AdminiCallSequence(9, &field, sizeof(field), 0);
}

s32 rstCallGetProcessData2(dds3ProcessID_t *PID) {
    titleData_t *data;
    s32 ret;

    data = dds3GetProcessData(PID);
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 0, PID);
    if (ret != 0) {
        return ret;
    }
    ret = evtCallStop(data);
    if (ret == 0) {
        return ret;
    }
    switch (data->Cursor.Index) {
    case 0:
        dds3AdminiCallSequence(3, NULL, 0, 0);
        p2sdPlaySE(0x500002, 127, 63);
        break;
    case 1:
        evtCallBgmStop(data);
        rstCallMemset();
        p2sdPlaySE(0x500002, 127, 63);
        break;
    }
    data->Cursor.Index = -1;
    return 0;
}

s32 TitleEndWaitDrawProcess(dds3ProcessID_t *PID)
{
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

INCLUDE_ASM(const s32, "code_002F6C00", TitleDLCInit);

INCLUDE_ASM(const s32, "code_002F6C00", TitleConfigDestroy);

INCLUDE_ASM(const s32, "code_002F6C00", TitleConfigCalcProcess);

dds3ProcessFunc_t rstCallGetProcessData33(dds3ProcessID_t *PID) {
    titleData_t *data;
    dds3ProcessFunc_t ret;

    data = dds3GetProcessData(PID);
    rstFunc(data);
    evtFunc11(&data->Cursor, data->sprite);
    ret = menuRunPanel(&data->panel, &data->seqUnit, 1, PID);
    menuRunPanel(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

s32 TitleAdvertiseInit(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);

    data->SkipTimer = 0;
    data->AdvSkipFlag = 0;
    data->Timer = 0;
    return 1;
}

INCLUDE_ASM(const s32, "code_002F6C00", rstCallGetProcessData34);

INCLUDE_ASM(const s32, "code_002F6C00", TitleAdvertiseCalcProcess);

INCLUDE_ASM(const s32, "code_002F6C00", rstCallGetProcessData35);
