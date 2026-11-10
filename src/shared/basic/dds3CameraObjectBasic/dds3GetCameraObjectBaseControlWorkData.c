#include "shared/basic/dds3CameraObjectBasic/decls.h"

dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *basic)
{
    dds3CameraObjectBasicWorkData_t *work = basic->workdata;

    return work->pObjectBaseControlWorkData;
}
