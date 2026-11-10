#include "shared/interface/itfMesManager/decls.h"

void itfMesMngMarkBitStat(s32 id, u32 flag)
{
    instanceMes_tag *rec = pinst[id].mes;

    rec->stat |= flag & 0xFFFF0000;
}
