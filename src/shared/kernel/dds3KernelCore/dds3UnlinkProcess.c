#include "shared/kernel/dds3KernelCore/decls.h"

void dds3UnlinkProcess(dds3ProcessID_t *target)
{
    dds3ProcessID_t *parent = target->parent;
    dds3ProcessID_t *p;

    if (parent == NULL) {
        return;
    }
    if (parent->child == target) {
        parent->child = target->sibling;
    } else {
        p = parent->child;
        while (p->sibling != target) {
            p = p->sibling;
        }
        p->sibling = target->sibling;
    }
    target->parent = NULL;
    target->sibling = NULL;
}
