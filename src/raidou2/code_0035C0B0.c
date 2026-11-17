#include "common.h"
#include "dds3KernelCore.h"
#include "TitleMain.h"
#include "sdfMemory.h"
#include "sdfTex.h"

s32 rstCallGetProcessData31(dds3ProcessID_t *PID)
{
    titleData_t *data = dds3GetProcessData(PID);

    data->Timer = 0;
    return 1;
}

s32 rstLeaf11(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData);

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

s32 rstLeaf12(void)
{
    return 1;
}

s32 TitleEndWaitCalcProcess(void)
{
    return 1;
}

INCLUDE_ASM(const s32, "code_0035C0B0", TitleNewGameStart);

INCLUDE_ASM(const s32, "code_0035C0B0", TitleEndWaitCalcProcess_TitleMain);

s32 TitleEndWaitDrawProcess(dds3ProcessID_t *PID) {
    titleData_t *data = dds3GetProcessData(PID);
    s32 ret;

    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}

INCLUDE_ASM(const s32, "code_0035C0B0", TitleDLCInit);

INCLUDE_ASM(const s32, "code_0035C0B0", TitleConfigDestroy);

INCLUDE_ASM(const s32, "code_0035C0B0", TitleConfigCalcProcess);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData33);

INCLUDE_ASM(const s32, "code_0035C0B0", TitleAdvertiseInit);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData34);

INCLUDE_ASM(const s32, "code_0035C0B0", TitleAdvertiseCalcProcess);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData35);

sdfTexHandle_t *rstCallDevLoadFileEx(const char *filename)
{
    u32 resource = 0;
    sdfMemHandle_t *fileAllocation = sdfDevLoadFileEx(filename, &resource, 0);
    sdfTexHandle_t *texture = sdfTexSet(EE_ADDR_AS(TMX_HEADER_t *, resource));

    sdfMemFreeDelayedDirect(fileAllocation);
    return texture;
}

INCLUDE_ASM(const s32, "code_0035C0B0", TitleFirstCalcProcess);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallTexFreeDelayed);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData2);

INCLUDE_ASM(const s32, "code_0035C0B0", rstCallGetProcessData3);

INCLUDE_ASM(const s32, "code_0035C0B0", fldCallGetProcessData);
