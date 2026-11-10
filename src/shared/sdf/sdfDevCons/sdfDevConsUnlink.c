#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsUnlink(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsCB_tag *prev;
    sdfDevConsCB_tag *next;

    prev = aHndl->prev;
    next = aHndl->next;
    if (prev) {
        prev->next = next;
    } else {
        devConsTop = next;
    }
    if (next) {
        next->prev = prev;
    }
}
