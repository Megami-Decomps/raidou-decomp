#include "shared/kernel/dds3KernelCore/decls.h"

void dds3EndingProcess(void)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *endp;

    p = endinglistf;
    while (p != NULL) {
        if (p->shutdowntimer > 0) {
            p->shutdowntimer--;
        }
        endp = p;
        p = p->next;
        if (endp->shutdowntimer == 0) {
            KillProcess(endp);
        }
    }
}
