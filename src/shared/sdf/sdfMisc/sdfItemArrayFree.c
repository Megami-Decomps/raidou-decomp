#include "shared/sdf/sdfMisc/decls.h"

void sdfItemArrayFree(sdfItemArray_t *aArray)
{
    sdfMemFreeDirect(aArray->memHandle);
    sdfChipFreeDirect(aArray);
}
