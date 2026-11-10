#include "shared/basic/dds3ObjectBase/decls.h"

void dds3ObjectBaseOnFlag(dds3Basic_t *basic, s32 mask)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(basic);
    control->flag |= mask;
}
