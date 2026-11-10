#include "shared/kernel/dds3KernelCore/decls.h"

dds3ProcessID_t *dds3SearchProcessNumber(u32 type, s32 n)
{
    dds3ProcessID_t *p = startinglistf;
    s32 i;

    switch (type & 0xF) {
    case 1:
        break;
    case 2:
        p = runninglistf;
        break;
    case 3:
        p = endinglistf;
        break;
    case 0:
    default:
        return NULL;
    }
    for (i = 0; i < n && p != NULL; i++) {
        p = p->next;
    }
    return p;
}
