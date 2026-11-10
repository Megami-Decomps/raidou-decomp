#include "shared/basic/dds3ObjectBase/decls.h"

dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *pObjectBasic, s32 itemType)
{
    return dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType];
}
