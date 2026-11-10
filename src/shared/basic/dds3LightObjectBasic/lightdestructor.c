#include "shared/basic/dds3LightObjectBasic/decls.h"

void lightdestructor(dds3Basic_t *basic)
{
    dds3LightObjectBasicWorkData_t *work = basic->workdata;

    sdfChipFreeDirect(work->LightPacket);
    dds3ObjectBaseFreeControlWorkData(work->ObjectBaseControlWorkData);
    sdfChipFreeDirect(basic->workdata);
    basic->workdata = NULL;
    dds3FreePoseSet(basic);
}
