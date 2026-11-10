#include "shared/basic/dds3ObjectBase/decls.h"

dds3PathBasicControl_t *dds3ObjectBaseGetPathControl(dds3Basic_t *pObjectBasic)
{
    return dds3MoverBasicGetPathControl(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}
