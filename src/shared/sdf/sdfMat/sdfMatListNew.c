#include "shared/sdf/sdfMat/decls.h"

sdfItemArray_t *sdfMatListNew(s32 aInitCapa)
{
    return sdfItemArrayNew(aInitCapa, 4, 8);
}
