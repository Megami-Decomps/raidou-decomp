#include "shared/kernel/dds3KernelProcess/decls.h"

s32 dds3BasicDrawProcess(dds3ProcessID_t *id) {
    dds3CallBasicDrawProc(dds3TheWorld);
    return 0;
}
