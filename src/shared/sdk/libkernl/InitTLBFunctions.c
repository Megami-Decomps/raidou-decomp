#include "shared/sdk/libkernl/decls.h"

void InitTLBFunctions(void)
{
    u32 index;
    u32 i;
    void *entry;

    setup_0(tlbEntryPairs[0].index, tlbEntryPairs[0].entry);
    Copy_2((void *)0x80075000, TLBFunctionsImage, 0x330);
    FlushCache(0);
    FlushCache(2);
    setup_0(tlbEntryPairs[1].index, tlbEntryPairs[1].entry);
    setup_0(tlbEntryPairs[2].index, tlbEntryPairs[2].entry);
    for (i = 3; i < 8; i++) {
        index = tlbEntryPairs[i].index;
        entry = GetEntryAddress_1(index);
        setup_0(tlbEntryPairs[i].index, entry);
    }
    execArgumentBlock = GetEntryAddress_1(3);
}
