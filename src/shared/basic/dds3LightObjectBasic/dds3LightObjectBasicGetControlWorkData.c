#include "shared/basic/dds3LightObjectBasic/decls.h"

dds3ObjectBaseControlWorkData_t *dds3LightObjectBasicGetControlWorkData(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    return work->ObjectBaseControlWorkData;
}
