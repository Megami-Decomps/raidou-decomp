#include "shared/kernel/dds3KernelCore/decls.h"

/* The name compare runs back to front, from the terminator down to byte 0. */
dds3ProcessID_t *dds3SearchProcessName(char *namestr)
{
    s32 len;
    s32 hash;
    s32 list;
    s32 i;
    dds3ProcessID_t *p;

    p = NULL;
    hash = 0;
    len = 0;
    while (namestr[len] != '\0') {
        hash += namestr[len];
        len++;
    }

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
            if (p->namehash == hash) {
                for (i = len;; i--) {
                    if (namestr[i] != p->name[i]) {
                        break;
                    }
                    if (i == 0) {
                        return p;
                    }
                }
            }
            p = p->next;
        }
    }
    return NULL;
}
