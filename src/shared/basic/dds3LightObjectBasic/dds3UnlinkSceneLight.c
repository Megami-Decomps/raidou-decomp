#include "shared/basic/dds3LightObjectBasic/decls.h"

void dds3UnlinkSceneLight(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    work->Flag &= ~3;
}
