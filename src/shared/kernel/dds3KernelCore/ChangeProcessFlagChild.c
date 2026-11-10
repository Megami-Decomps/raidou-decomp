#include "shared/kernel/dds3KernelCore/decls.h"

void ChangeProcessFlagChild(s32 change, dds3ProcessID_t *parent, u32 flag)
{
    dds3ProcessID_t *p;

    if (change != 0) {
        parent->flag |= flag & 0x0FFFFFF0;
    } else {
        parent->flag &= ~(flag & 0x0FFFFFF0);
    }
    p = parent->child;
    while (p != NULL) {
        ChangeProcessFlagChild(change, p, flag);
        p = p->sibling;
    }
}
