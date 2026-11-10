#include "shared/interface/itfMesManager/decls.h"

/* Set manager flags; the stored result is narrowed to its existing u16 field. */
void itfMesMngSetBitStatManager(u32 flags) {
    itfMesMngManager.flags |= flags;
}
