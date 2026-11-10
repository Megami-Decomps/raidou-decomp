#include "shared/sdk/libkernl/decls.h"

void InitExecPS2(void)
{
    u32 index;
    u32 i;
    void *entry;

    if (PatchIsNeeded_0() == 0) {
        return;
    }
    gcc2_compiled__15(execEntryPairs[0].index, execEntryPairs[0].entry);
    Copy_0((void *)0x80074000, execPs2Payload, 0x7A8);
    FlushCache(0);
    FlushCache(2);
    gcc2_compiled__15(execEntryPairs[1].index, execEntryPairs[1].entry);
    for (i = 2; i < 3; i++) {
        index = execEntryPairs[i].index;
        entry = GetEntryAddress(index);
        gcc2_compiled__15(execEntryPairs[i].index, entry);
    }
}
