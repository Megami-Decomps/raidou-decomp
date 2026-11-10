#include "shared/basic/dds3MoverBasic/decls.h"

void dds3MoverBasicSetCallBackPrimaryControl(dds3Basic_t *mover, dds3BasicControlProc_t func)
{
    dds3MoverBasicWorkData_t *work = mover->workdata;

    work->pFunc = func;
}
