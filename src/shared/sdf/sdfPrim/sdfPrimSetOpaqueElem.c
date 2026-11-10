#include "shared/sdf/sdfPrim/decls.h"

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = SDF_GS_ALPHA_TEST;
    aElem->test.reg = SDF_GS_TEST_1;
    aElem->alpha.data = SDF_GS_DEFAULT_ALPHA;
    aElem->alpha.reg = SDF_GS_ALPHA_1;
}
