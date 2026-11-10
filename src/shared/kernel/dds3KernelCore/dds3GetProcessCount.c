#include "shared/kernel/dds3KernelCore/decls.h"

u32 dds3GetProcessCount(u32 type)
{
    switch (type & 0xF) {
    case 1:
        return startinglistcnt;
    case 2:
        return runninglistcnt;
    case 3:
        return endinglistcnt;
    }
    return 0;
}
