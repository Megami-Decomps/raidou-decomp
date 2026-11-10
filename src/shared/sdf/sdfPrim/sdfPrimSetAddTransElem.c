#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x47;
    aElem->alpha.data = 0x48;
    aElem->alpha.reg = 0x42;
}
