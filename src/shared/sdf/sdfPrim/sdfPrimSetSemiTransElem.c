#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x717FB;
    aElem->test.reg = 0x47;
    aElem->alpha.data = 0x44;
    aElem->alpha.reg = 0x42;
}
