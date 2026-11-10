#include "shared/interface/itfMesManager/decls.h"

/* Read the manager's 16-bit flags, distinct from itfMessageFlags. */
u16 itfMesMngGetStatManager(void) {
    return itfMesMngManager.flags;
}
