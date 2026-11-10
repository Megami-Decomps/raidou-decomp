#include "shared/kernel/dds3AdminiProcess/decls.h"

dds3AdminiData_t *GetAdminiProcessData(void)
{
    return dds3GetProcessData(dds3SearchProcessName(dds3AdminiProcessName));
}
