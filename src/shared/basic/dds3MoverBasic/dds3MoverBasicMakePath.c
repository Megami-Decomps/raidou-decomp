#include "shared/basic/dds3MoverBasic/decls.h"

void dds3MoverBasicMakePath(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    if (work->pPathControl != NULL) {
        dds3DeletePathControl(work->pPathControl);
    }
    work->pPathControl = dds3AllocPathBasicWorkData(work->pPathBasic);
}
