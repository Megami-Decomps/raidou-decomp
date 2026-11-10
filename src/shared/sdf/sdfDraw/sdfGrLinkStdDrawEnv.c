#include "shared/sdf/sdfDraw/decls.h"

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv) {
    sdfPreDrawTagLink(aPreDrawTag, aEnv);
    sdfDrawTagLink(aTag, EE_ADDR_S(aEnv) + SDF_QWORD_BYTES);
}
