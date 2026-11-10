#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseOffFlag(dds3Basic_t *pObjectBasic, s32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    control->flag &= ~flag;
}
