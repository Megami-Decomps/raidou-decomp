#include "shared/interface/itfMesManager/decls.h"

void CommitVarTable(s32 *pvar)
{
    s32 i;

    for (i = 0; i < 32; i++) {
        frSetVariableRequest(i, pvar[i]);
    }
}
