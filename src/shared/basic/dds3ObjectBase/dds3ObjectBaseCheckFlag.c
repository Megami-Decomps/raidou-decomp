#include "shared/basic/dds3ObjectBase/decls.h"

u8 dds3ObjectBaseCheckFlag(dds3Basic_t *pObjectBasic, u32 flag)
{
    dds3ObjectBaseControlWorkData_t *control;

    control = dds3GetBaseControlWorkData(pObjectBasic);
    return (control->flag & flag) != 0;
}
