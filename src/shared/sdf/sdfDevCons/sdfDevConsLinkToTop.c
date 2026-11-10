#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsLinkToTop(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsCB_tag *top;

    top = devConsTop;
    aHndl->prev = NULL;
    aHndl->next = top;
    if (top) {
        top->prev = aHndl;
    }
    devConsTop = aHndl;
}
