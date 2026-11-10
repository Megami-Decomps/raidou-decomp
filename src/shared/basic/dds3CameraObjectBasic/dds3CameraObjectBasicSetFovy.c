#include "shared/basic/dds3CameraObjectBasic/decls.h"

void dds3CameraObjectBasicSetFovy(dds3Basic_t *pBasic, f32 fovy)
{
    dds3CameraObjectBasicWorkData_t *work = pBasic->workdata;

    work->fovy = fovy;
    work->flag |= 1;
}
