#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3ObjectBaseControlWorkData_t *dds3EffectObjectBasicGetControlWorkData(dds3Basic_t *pBasic)
{
    return ((dds3EffectObjectBasicWorkData_t *)pBasic->workdata)->pObjectBaseControlWorkData;
}
