#include "shared/field/fldProcess/decls.h"

s32 fldEncountTestFlag(u32 flag)
{
    return (fldEncountFlags & flag) != 0;
}
