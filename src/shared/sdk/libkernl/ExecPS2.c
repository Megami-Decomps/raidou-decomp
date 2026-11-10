#include "shared/sdk/libkernl/decls.h"

s32 ExecPS2(void *entry, void *gp, s32 count, const char *const *arguments)
{
    SetArg(execDefaultPath, count, arguments);
    TerminateLibrary();
    return _ExecPS2(entry, gp, count,
                    &((execArgumentBlock_t *)execArgumentBlock)->arguments[1]);
}
