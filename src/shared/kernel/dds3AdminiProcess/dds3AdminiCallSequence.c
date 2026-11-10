#include "shared/kernel/dds3AdminiProcess/decls.h"

/* Requests retain byte-sized sequence/size storage: 256 bytes records zero.
 * Release the old payload before copying the new one. */
void dds3AdminiCallSequence(s32 sequence, void *data, u32 datasize, s32 restore)
{
    dds3AdminiData_t *work;
    void *previousData;
    u32 flag;

    if (data == NULL || datasize <= 0x100) {
        work = GetAdminiProcessData();
        previousData = work->CallData;
        work->NextSequence = sequence;
        flag = work->Stat;
        flag |= 1;
        flag &= ~8;
        flag &= ~0x10000;
        work->Stat = flag;
        work->CallCount = 2;
        if (previousData != NULL) {
            sdfChipFreeDirect(previousData);
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (data != NULL) {
            work->CallData = sdfChipAllocDirect(datasize);
            memcpy(work->CallData, data, datasize);
            work->CallDataSize = datasize;
        } else {
            work->CallData = NULL;
            work->CallDataSize = 0;
        }
        if (restore != 0) {
            work->Stat |= 4;
        } else {
            work->Stat &= ~4;
        }
    }
}
