#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseCreateMover(dds3Basic_t *pObjectBasic)
{
    if (dds3ObjectBaseGetControlBasic(pObjectBasic, 1) == NULL) {
        dds3ObjectBaseSetControlBasic(pObjectBasic, dds3AddMoverBasic(pObjectBasic));
    }
}
