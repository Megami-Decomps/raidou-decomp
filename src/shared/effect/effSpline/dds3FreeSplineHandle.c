#include "shared/effect/effSpline/decls.h"

void dds3FreeSplineHandle(dds3Spline_t *pSpline)
{
    if (pSpline == NULL) {
        return;
    }
    if (pSpline->coefs != NULL) {
        sdfMemFreeDirect(pSpline->coefHandle);
    }
    sdfMemFreeDirect(pSpline->handle);
}
