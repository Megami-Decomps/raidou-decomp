#include "shared/interface/itfMesManager/decls.h"

u32 itfMesMngGetStat(s32 id)
{
    return pinst[id].mes->stat;
}
