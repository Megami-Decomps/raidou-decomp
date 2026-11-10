#include "shared/sdf/sdfSystem/decls.h"

s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority) {
    sdfKernelThreadParam_t param;

    memset(&param, 0, sizeof(param));
    param.entry = entryAddress;
    param.stack = workspace;
    param.stackSize = stackBytes;
    param.priority = priority;
    param.gp = &_gp;
    return CreateThread(&param);
}
