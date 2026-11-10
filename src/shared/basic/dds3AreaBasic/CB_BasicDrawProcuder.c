#include "shared/basic/dds3AreaBasic/decls.h"

s32 CB_BasicDrawProcuder(dds3Basic_t *pBasic)
{
    if (pBasic->basicfunction->drawprocuder != NULL) {
        return ((dds3BasicProc_t)pBasic->basicfunction->drawprocuder)(pBasic);
    }
    return 1;
}
