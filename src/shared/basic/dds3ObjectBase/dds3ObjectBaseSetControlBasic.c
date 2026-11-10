#include "shared/basic/dds3ObjectBase/decls.h"

dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *pObjectBasic, dds3Basic_t *pItemBasic)
{
    if (pItemBasic == NULL) {
        return NULL;
    }
    return dds3ObjectBaseSetControlBasicType(pObjectBasic, pItemBasic,
                                             BasicTypeToItemType(pItemBasic->type >> 24));
}
