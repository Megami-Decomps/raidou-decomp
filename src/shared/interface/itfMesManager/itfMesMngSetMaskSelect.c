#include "shared/interface/itfMesManager/decls.h"

void itfMesMngSetMaskSelect(s32 id, u32 maskpattern)
{
    pinst[id].mes->select.mask = maskpattern;
}
