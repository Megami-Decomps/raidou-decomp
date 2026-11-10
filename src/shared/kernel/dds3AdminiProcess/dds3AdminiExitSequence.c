#include "shared/kernel/dds3AdminiProcess/decls.h"

void dds3AdminiExitSequence(void)
{
    dds3AdminiData_t *work;

    work = GetAdminiProcessData();
    work->Stat |= 2;
}
