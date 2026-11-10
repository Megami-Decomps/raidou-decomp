#include "shared/sdf/sdfDevCons/decls.h"

void sdfDevConsInit(void)
{
    if (!alreadyInitDevCons) {
        alreadyInitDevCons = 1;
        sdfDebugFontHandle = sdfTexSet(sdfDebugFontTex);
    }
}
