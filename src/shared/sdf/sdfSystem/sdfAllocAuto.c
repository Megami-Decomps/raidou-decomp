#include "shared/sdf/sdfSystem/decls.h"

void *sdfAllocAuto(s32 size)
{
    void *p;

    if (size > 0x400) {
        p = sdfMemLock(sdfMemAllocDirect(size));
        size = 0;
    } else {
        p = sdfChipAllocDirect(size);
        size = 0;
    }
    return p;
}
