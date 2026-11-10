#include "shared/interface/itfMesManager/decls.h"

/* Replace a text slot with a copied NUL-terminated encoded string. */
void itfMesMngSetVarImmediate(s32 id, u32 index, const char *pstr) {
    SetVar(pinst[id].mes, index, pstr, 0);
}
