#include "shared/kernel/dds3AdminiProcess/decls.h"

s8 dds3AdminiGetActiveSequence(void)
{
    return GetAdminiProcessData()->NowSequence;
}
