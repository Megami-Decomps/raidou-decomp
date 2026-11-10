#include "shared/kernel/dds3KernelCore/decls.h"

/* The 0x0FFFFFF0 mask keeps callers out of the state nibble and the top four
 * bits of flag. */
void ChangeProcessFlag(s32 change, dds3ProcessID_t *target, u32 flag, s32 type)
{
    dds3ProcessID_t *p = NULL;
    s32 list;

    switch (type) {
    case 0:
        if (change != 0) {
            target->flag |= flag & 0x0FFFFFF0;
        } else {
            target->flag &= ~(flag & 0x0FFFFFF0);
        }
        return;
    case 1:
    case 3:
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
            if (p != NULL) {
                u32 mask = flag & 0x0FFFFFF0;

                while (p != NULL) {
                    if ((p != target && type == 1) || type == 3) {
                        if (change != 0) {
                            p->flag |= mask;
                        } else {
                            p->flag &= ~mask;
                        }
                    }
                    p = p->next;
                }
            }
        }
        return;
    case 2:
        ChangeProcessFlagChild(change, target, flag);
        break;
    }
}
