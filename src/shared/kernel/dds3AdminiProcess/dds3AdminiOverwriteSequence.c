#include "shared/kernel/dds3AdminiProcess/decls.h"

void dds3AdminiOverwriteSequence(s32 sequence, void *data, u32 datasize)
{
    dds3AdminiData_t *work;

    dds3AdminiCallSequence(sequence, data, datasize, 0);
    work = GetAdminiProcessData();
    work->Stat |= 8;
}
