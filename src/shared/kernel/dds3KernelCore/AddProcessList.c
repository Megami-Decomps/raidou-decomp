#include "shared/kernel/dds3KernelCore/decls.h"

void AddProcessList(dds3ProcessID_t *setp)
{
    dds3ProcessID_t *p;

    switch (setp->flag & 0xF) {
    case 1:
        p = startinglistf;
        break;
    case 2:
        p = runninglistf;
        break;
    case 3:
        p = endinglistf;
        break;
    case 0:
    default:
        return;
    }

    if (p == NULL) {
        switch (setp->flag & 0xF) {
        case 1:
            startinglistf = setp;
            startinglistl = setp;
            break;
        case 2:
            runninglistf = setp;
            runninglistl = setp;
            break;
        case 3:
            endinglistf = setp;
            endinglistl = setp;
            break;
        }
        setp->prev = NULL;
        setp->next = NULL;
    } else {
        while (p != NULL) {
            if (setp->priority < p->priority) {
                if (p->prev != NULL) {
                    p->prev->next = setp;
                    setp->prev = p->prev;
                    setp->next = p;
                    p->prev = setp;
                } else {
                    switch (setp->flag & 0xF) {
                    case 1:
                        startinglistf = setp;
                        break;
                    case 2:
                        runninglistf = setp;
                        break;
                    case 3:
                        endinglistf = setp;
                        break;
                    }
                    setp->prev = NULL;
                    setp->next = p;
                    p->prev = setp;
                }
                break;
            }
            p = p->next;
        }
        if (p == NULL) {
            switch (setp->flag & 0xF) {
            case 1:
                startinglistl->next = setp;
                setp->prev = startinglistl;
                startinglistl = setp;
                break;
            case 2:
                runninglistl->next = setp;
                setp->prev = runninglistl;
                runninglistl = setp;
                break;
            case 3:
                endinglistl->next = setp;
                setp->prev = endinglistl;
                endinglistl = setp;
                break;
            }
            setp->next = NULL;
        }
    }

    switch (setp->flag & 0xF) {
    case 1:
        startinglistcnt++;
        break;
    case 2:
        runninglistcnt++;
        break;
    case 3:
        endinglistcnt++;
        break;
    }
}
