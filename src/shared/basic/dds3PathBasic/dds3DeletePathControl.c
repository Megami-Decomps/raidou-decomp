#include "shared/basic/dds3PathBasic/decls.h"

void dds3DeletePathControl(dds3PathBasicControl_t *work)
{
    dds3FreeSplineHandle(work->pSplineHandle);
    sdfChipFreeDirect(work);
}
