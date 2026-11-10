#include "shared/kernel/dds3KernelCore/decls.h"

void RunningProcess(void)
{
    dds3ProcessID_t *active = runninglistf;
    dds3ProcessID_t *destroyPending = endinglistf;
    dds3ProcessID_t *current;
    /* Each queue cursor uses a saved predecessor if RunProcess unlinks its node. */
    dds3ProcessID_t *drainPrevious;

    for (;;) {
        if (active != 0) {
            if (destroyPending != 0) {
                dds3ProcessID_t *previous;

                if (active->priority <= destroyPending->priority) {
                    current = active;
                    previous = current->prev;
                    if (RunProcess(current) != 0) {
                        active = current->next;
                    } else {
                        active = previous != 0 ? previous->next : runninglistf;
                    }
                } else {
                    current = destroyPending;
                    previous = current->prev;
                    if (RunProcess(current) != 0) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = previous != 0 ? previous->next : endinglistf;
                    }
                }
            } else {

                do {
                    current = active;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != 0) {
                        active = current->next;
                    } else {
                        active = drainPrevious != 0 ? drainPrevious->next : runninglistf;
                    }
                } while (active != 0);
                return;
            }
        } else {
            if (destroyPending != 0) {

                do {
                    current = destroyPending;
                    drainPrevious = current->prev;
                    if (RunProcess(current) != 0) {
                        destroyPending = current->next;
                    } else {
                        destroyPending = drainPrevious != 0 ? drainPrevious->next : endinglistf;
                    }
                } while (destroyPending != 0);
            }
            return;
        }
    }
}
