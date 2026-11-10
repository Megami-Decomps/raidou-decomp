#include "shared/kernel/dds3KernelCore/decls.h"

void StartingProcess(void)
{
    dds3ProcessID_t *p;
    dds3ProcessID_t *startp;

    p = startinglistf;
    while (p != NULL) {
        if (p->starttimer > 0) {
            p->starttimer--;
        }
        startp = p;
        p = p->next;
        if (startp->starttimer == 0) {
            StartProcess(startp);
        }
    }
}
