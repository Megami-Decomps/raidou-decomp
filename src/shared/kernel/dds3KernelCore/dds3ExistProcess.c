#include "shared/kernel/dds3KernelCore/decls.h"

s32 dds3ExistProcess(dds3ProcessID_t *id)
{
    dds3ProcessID_t *p = NULL;
    s32 list;

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
            if (p == id) {
                return 1;
            }
            p = p->next;
        }
    }
    return 0;
}
