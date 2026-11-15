#include "common.h"
#include "kernel.h"
#include "dds3KernelCore.h"
#include "include_asm.h"
#include "jmsMisc.h"
#include "TitleMain.h"

extern const titleKeyTable_t rstCallGetProcessData30Table;

s32 rstCallGetProcessData30(dds3ProcessID_t *PID)
{
    titleData_t *data;
    titleKeyTable_t keys;
    s32 ret;

    data = dds3GetProcessData(PID);
    keys = rstCallGetProcessData30Table;
    rstFunc(data);
    evtFunc10(0, 256, data->sprite);
    evtFunc10(1, evtFunc9(data->introFrame, keys.key, 3), data->sprite);
    data->introFrame++;
    if (data->introFrame > keys.key[2].frame) {
        data->introFrame = 0;
    }
    ret = jmsSeqChange(&data->panel, &data->seqUnit, 1, PID);
    jmsSeqChange(&data->panel, &data->seqUnit, 2, PID);
    return ret;
}
