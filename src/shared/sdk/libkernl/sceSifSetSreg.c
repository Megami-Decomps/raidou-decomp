#include "shared/sdk/libkernl/decls.h"

u32 sceSifSetSreg(s32 index, u32 value)
{
    return sifSregs[index] = value;
}
