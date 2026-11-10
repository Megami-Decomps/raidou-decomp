#include "shared/sdk/libkernl/decls.h"

u32 sceSifGetSreg(s32 index)
{
    return sifSregs[index];
}
