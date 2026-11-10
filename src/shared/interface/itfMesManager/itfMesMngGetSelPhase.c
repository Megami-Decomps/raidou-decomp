#include "shared/interface/itfMesManager/decls.h"

/* Return the option-block signed status word, not the separate entry status byte. */
s16 itfMesMngGetSelPhase(s32 msgid) {
    return pinst[msgid].mes->select.phase;
}
