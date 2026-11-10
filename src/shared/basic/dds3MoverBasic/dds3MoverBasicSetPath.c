#include "shared/basic/dds3MoverBasic/decls.h"

void dds3MoverBasicSetPath(dds3Basic_t *pMoverBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    work->pPathBasic = pPathBasic;
}
