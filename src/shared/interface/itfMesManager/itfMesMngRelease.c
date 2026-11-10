#include "shared/interface/itfMesManager/decls.h"

/* Destroy an installed nonnegative window slot; no upper-bound validation. */
void itfMesMngRelease(s32 id) {
    if (id >= 0 && pinst[id].mes != NULL) {
        DestructFlow(id);
    }
}
