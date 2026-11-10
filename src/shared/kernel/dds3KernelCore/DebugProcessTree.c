#include "shared/kernel/dds3KernelCore/decls.h"

void DebugProcessTree(dds3ProcessID_t *ps, s32 level)
{
    printf(dds3DebugTreeLineFmt, dds3ProcessTreeDepth, ps);
    if (ps->child != NULL) {
        if (ps->parent == NULL) {
            dds3ProcessTreeDepth[0] = ' ';
        } else {
            if (ps->sibling == NULL) {
                dds3ProcessTreeDepth[level * 4] = ' ';
            } else {
                dds3ProcessTreeDepth[level * 4] = '|';
            }
        }
        dds3ProcessTreeDepth[level * 4 + 4] = 0;
        DebugProcessTree(ps->child, level + 1);
        dds3ProcessTreeDepth[level * 4] = 0;
    }
    if (ps->sibling != NULL) {
        DebugProcessTree(ps->sibling, level);
    }
}
