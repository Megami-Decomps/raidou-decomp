#include "shared/interface/itfMesManager/decls.h"

/* Install/relocate a message resource and cache MSG1's trailing-entry word as
 * the temporary font entry. Return the previous resource without releasing it. */
itfMesResource_t *itfMesMngSetResourcePtr(s32 id, itfMesResource_t *presource) {
    instanceMes_tag *mes = pinst[id].mes;
    itfMesResource_t *previous = mes->pdata;
    itfMesBinTypeHeader_t *entry;
    u8 *temporaryFontEntry;

    mes->pdata = presource;
    ResolveResourceAddress((itfMesResource_t *)presource);
    entry = GetTypeHeader2(presource);
    temporaryFontEntry = 0;
    if (presource->magic == ITF_MES_MAGIC_MSG1) {
        /* itfMesBinTypeHeader_t is an invented type and the IDB has no counterpart,
         * so +8 past its two known words has no name to take. */
        temporaryFontEntry = *(u8 **)((u8 *)entry + 8);
    }
    mes->pext = temporaryFontEntry;
    return previous;
}
