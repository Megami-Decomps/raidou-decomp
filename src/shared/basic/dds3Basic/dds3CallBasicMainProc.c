#include "shared/basic/dds3Basic/decls.h"

s32 dds3CallBasicMainProc(dds3Basic_t *pBasic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (pBasic != NULL) {
        function = pBasic->basicfunction;
        if (function != NULL && function->mainprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->mainprocuder)(pBasic);
        }
    }
    return result;
}
