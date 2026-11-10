#include "shared/interface/itfMesManager/decls.h"

/* Store the signed byte entry status; index and payload pointer are trusted. */
void itfMesMngSetPhase(s32 msgid, s8 phase) {
    pinst[msgid].mes->message.phase = phase;
}
