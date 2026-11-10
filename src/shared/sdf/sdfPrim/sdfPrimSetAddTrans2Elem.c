#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x48;
    aElem->alpha.data = 0x48;
    aElem->alpha.reg = 0x43;
}
