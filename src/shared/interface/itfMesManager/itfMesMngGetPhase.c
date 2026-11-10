#include "shared/interface/itfMesManager/decls.h"

s8 itfMesMngGetPhase(s32 msgid)
{
    return pinst[msgid].mes->message.phase;
}
