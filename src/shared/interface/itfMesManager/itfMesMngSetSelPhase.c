#include "shared/interface/itfMesManager/decls.h"

void itfMesMngSetSelPhase(s32 msgid, s16 phase)
{
    pinst[msgid].mes->select.phase = phase;
}
