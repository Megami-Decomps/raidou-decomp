#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = SDF_GS_ALPHA_TEST;
    aElem->test.reg = SDF_GS_TEST_2;
    aElem->alpha.data = SDF_GS_DEFAULT_ALPHA;
    aElem->alpha.reg = SDF_GS_ALPHA_2;
}
