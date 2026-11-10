#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseMakePath(dds3Basic_t *pObjectBasic, dds3Basic_t *pPathBasic)
{
    dds3MoverBasicSetPath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1), pPathBasic);
    dds3MoverBasicMakePath(pObjectBasic);
}
