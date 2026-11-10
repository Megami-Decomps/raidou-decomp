#include "shared/field/fldProcess/decls.h"

void fldEncountClearFlag(u32 flag)
{
    fldEncountFlags &= ~flag;
}
