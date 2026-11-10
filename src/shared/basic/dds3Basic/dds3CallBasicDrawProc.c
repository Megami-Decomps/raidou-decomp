#include "shared/basic/dds3Basic/decls.h"

s32 dds3CallBasicDrawProc(dds3Basic_t *basic)
{
    dds3BasicFunction_t *function;
    s32 result;

    result = 1;
    if (basic != NULL) {
        function = basic->basicfunction;
        if (function != NULL && function->drawprocuder != NULL) {
            result = ((s32 (*)(dds3Basic_t *))function->drawprocuder)(basic);
        }
    }
    return result;
}
