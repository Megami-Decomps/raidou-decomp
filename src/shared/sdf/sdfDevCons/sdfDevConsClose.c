#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsClose(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsUnlink(aHndl);
    sdfMemFreeDirect(aHndl->bufferHandle);
    sdfChipFreeDirect(aHndl);
}
