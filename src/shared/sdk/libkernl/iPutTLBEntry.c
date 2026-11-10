#include "shared/sdk/libkernl/decls.h"

/* Existing retail symbol is retained; this is the checked indexed setter,
 * not the separate interrupt-context PutTLBEntry syscall stub. */
s32 iPutTLBEntry(u32 index, u32 pageMask, u32 entryHi, u32 entryLo0, u32 entryLo1)
{
    if (index - 13U >= 35U) {
        return -1;
    }
    return _SetTLBEntry(index, pageMask, entryHi, entryLo0, entryLo1);
}
