#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseFreePath(dds3Basic_t *pObjectBasic)
{
    dds3MoverBasicFreePath(dds3ObjectBaseGetControlBasic(pObjectBasic, 1));
}
