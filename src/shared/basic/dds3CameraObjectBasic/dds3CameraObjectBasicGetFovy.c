#include "shared/basic/dds3CameraObjectBasic/decls.h"

f32 dds3CameraObjectBasicGetFovy(dds3Basic_t *pBasic)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;

    return work->fovy;
}
