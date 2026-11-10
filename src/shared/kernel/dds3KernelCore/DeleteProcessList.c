#include "shared/kernel/dds3KernelCore/decls.h"

void DeleteProcessList(dds3ProcessID_t *setp)
{
    if ((setp->flag & 0xF) == 0) {
        return;
    }
    if ((setp->flag & 0xF) >= 4) {
        return;
    }

    if (setp->prev != NULL) {
        setp->prev->next = setp->next;
    } else {
        switch (setp->flag & 0xF) {
        case 1:
            startinglistf = setp->next;
            break;
        case 2:
            runninglistf = setp->next;
            break;
        case 3:
            endinglistf = setp->next;
            break;
        }
    }

    if (setp->next != NULL) {
        setp->next->prev = setp->prev;
    } else {
        switch (setp->flag & 0xF) {
        case 1:
            startinglistl = setp->prev;
            break;
        case 2:
            runninglistl = setp->prev;
            break;
        case 3:
            endinglistl = setp->prev;
            break;
        }
    }

    setp->next = NULL;
    setp->prev = NULL;

    switch (setp->flag & 0xF) {
    case 1:
        startinglistcnt--;
        break;
    case 2:
        runninglistcnt--;
        break;
    case 3:
        endinglistcnt--;
        break;
    }
}
