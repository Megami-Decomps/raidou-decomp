#include "shared/sdf/sdfMat/decls.h"

sdfItemArray_t *sdfTexListNew(s32 aInitCapa)
{
    return sdfItemArrayNew(aInitCapa, 4, 4);
}
