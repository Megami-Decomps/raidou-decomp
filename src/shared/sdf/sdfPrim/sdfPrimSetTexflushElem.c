#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *aElem) {
    aElem->data = 0;
    aElem->reg = SDF_GS_TEXFLUSH;
}
