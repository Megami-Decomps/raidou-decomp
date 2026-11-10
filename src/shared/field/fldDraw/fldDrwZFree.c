#include "shared/field/fldDraw/decls.h"

void fldDrwZFree(void)
{
    if (fldDrwZBuf != NULL) {
        sdfFreeAutoDelayed(fldDrwZBuf);
        fldDrwZBuf = NULL;
    }
}
