#include "shared/kernel/dds3KernelCore/decls.h"

dds3ProcessID_t *dds3SearchProcessPriority(u32 priority)
{
    s32 list;
    dds3ProcessID_t *p;

    p = NULL;
    for (list = 0; list < 3; list++) {
        switch (list) {
        case 0:
            p = startinglistf;
            break;
        case 1:
            p = runninglistf;
            break;
        case 2:
            p = endinglistf;
            break;
        }
        while (p != NULL) {
            if (p->priority == priority) {
                return p;
            }
            p = p->next;
        }
    }
    return NULL;
}
