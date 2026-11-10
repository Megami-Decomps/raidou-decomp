#include "shared/basic/dds3ObjectBase/decls.h"

void *dds3ObjectBaseGetModelHandle(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pModel;
}
