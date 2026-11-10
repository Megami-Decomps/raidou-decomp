#include "shared/basic/dds3LightObjectBasic/decls.h"

s32 dds3AllocEventBasicWorkData(dds3Basic_t *basic)
{
    void *work;

    work = sdfChipAllocDirect(28);
    basic->workdata = work;
    memset(work, 0, 28);
    return 1;
}
