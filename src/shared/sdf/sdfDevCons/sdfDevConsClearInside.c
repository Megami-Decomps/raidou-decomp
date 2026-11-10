#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsClearInside(sdfDevConsCB_tag *aHndl)
{
    aHndl->locX = 0;
    aHndl->locY = 0;
    memset(aHndl->bufferPtr, 0, aHndl->w * aHndl->h * 2);
}
