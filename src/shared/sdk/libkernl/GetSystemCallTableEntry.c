#include "shared/sdk/libkernl/decls.h"

void GetSystemCallTableEntry(void)
{
    u32 *searchEntry;
    u32 *copyEntry;
    u32 *searchBase;
    u32 *copyBase;

    setup(systemCallEntryPairs[0].index, systemCallEntryPairs[0].entry);
    setup(systemCallEntryPairs[1].index, systemCallEntryPairs[1].entry);
    searchEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                              EE_ADDR(kFindAddress));
    copyEntry = FindAddress((u32 *)0x80000000, (u32 *)0x80080000,
                            EE_ADDR(kCopy));
    searchBase = searchEntry - 0x83;
    copyBase = copyEntry - 0x5A;
    while (searchBase != copyBase) {
        if (searchBase < copyBase) {
            searchEntry = FindAddress(searchEntry + 1, (u32 *)0x80080000,
                                      EE_ADDR(kFindAddress));
            searchBase = searchEntry - 0x83;
        } else {
            copyEntry = FindAddress(copyEntry + 1, (u32 *)0x80080000,
                                    EE_ADDR(kCopy));
            copyBase = copyEntry - 0x5A;
        }
    }
    systemCallTable = searchBase;
}
