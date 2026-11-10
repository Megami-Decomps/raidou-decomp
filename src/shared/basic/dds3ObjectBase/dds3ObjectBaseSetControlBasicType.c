#include "shared/basic/dds3ObjectBase/decls.h"

dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic, s32 itemType)
{
    dds3Basic_t *previous;

    previous = dds3ObjectBaseGetControlBasic(pObjectBasic, itemType);
    dds3GetBaseControlWorkData(pObjectBasic)->pBasic[itemType] = pItemBasic;
    return previous;
}
