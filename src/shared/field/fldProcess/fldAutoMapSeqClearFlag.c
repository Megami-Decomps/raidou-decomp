#include "shared/field/fldProcess/decls.h"

void fldAutoMapSeqClearFlag(u32 flag)
{
    fldAutoMapSeqFlags.flags &= ~flag;
}
