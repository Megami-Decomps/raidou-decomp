#include "shared/sdf/sdfSortOt/decls.h"

void sdfModelSetOtCallbacks(sdfGrOt_t *aOt) {
    aOt->regFunc = sdfGrSortOtReg;
    aOt->callbackFunc = sdfGrSortOtCallback;
}
