#include "shared/sdf/sdfSystem/decls.h"

s32 sdfCreateThreadWithStack(void (*entryAddress)(), s32 stackBytes, s32 priority) {
    void *workspace;

    workspace = sdfAllocAuto(stackBytes);
    return CreateThreadSimple((void *)entryAddress, workspace, stackBytes, priority);
}
