#include "shared/field/fldProcess/decls.h"

s32 fldAutoMapSeqTestFlag(u32 flag)
{
    return (fldAutoMapSeqFlags.flags & flag) != 0;
}
