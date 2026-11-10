#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsPut(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr)
{
    sdfDevConsPutInside(aHndl, aChar, aAttr);
}
