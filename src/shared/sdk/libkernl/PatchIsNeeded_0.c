#include "shared/sdk/libkernl/decls.h"

s32 PatchIsNeeded_0(void)
{
    u32 original;
    u32 changed;

    GetOsdConfigParam(&original);
    changed = (original & 0xFFFF1FFF) | 0x2000;
    SetOsdConfigParam(&changed);
    GetOsdConfigParam(&changed);
    SetOsdConfigParam(&original);
    return ((changed >> 13) & 7) == 0;
}
