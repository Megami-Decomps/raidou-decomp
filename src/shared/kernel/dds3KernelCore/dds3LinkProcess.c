#include "shared/kernel/dds3KernelCore/decls.h"

void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target)
{
    dds3ProcessID_t *p;

    if (target->parent != NULL) {
        dds3UnlinkProcess(target);
    }
    if (parent->child != NULL) {
        p = parent->child;
        while (p->sibling != NULL) {
            p = p->sibling;
        }
        p->sibling = target;
    } else {
        parent->child = target;
    }
    target->parent = parent;
}
