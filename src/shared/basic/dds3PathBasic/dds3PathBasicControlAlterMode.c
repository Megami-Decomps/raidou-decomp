#include "shared/basic/dds3PathBasic/decls.h"

void dds3PathBasicControlAlterMode(dds3PathBasicControl_t *pControl, s32 mode)
{
    pControl->direction = mode;
}
