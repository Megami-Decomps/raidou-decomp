#include "shared/basic/dds3ObjectBase/decls.h"

dds3BasicArrayHandle_t *dds3ObjectBaseGetEventArray(dds3Basic_t *pObjectBasic)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->hEventBasicArray;
}
