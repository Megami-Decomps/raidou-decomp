#include "shared/sdk/libkernl/decls.h"

u32 GetSystemCallEntry(s32 index)
{
    u32 entry;

    Copy(&entry, &systemCallTable[index], 4);
    return entry;
}
