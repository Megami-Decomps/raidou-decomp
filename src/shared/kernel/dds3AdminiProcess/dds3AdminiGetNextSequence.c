#include "shared/kernel/dds3AdminiProcess/decls.h"

s8 dds3AdminiGetNextSequence(void)
{
    return GetAdminiProcessData()->NextSequence;
}
