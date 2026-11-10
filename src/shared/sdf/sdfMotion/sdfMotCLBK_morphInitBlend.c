#include "shared/sdf/sdfMotion/decls.h"

void sdfMotCLBK_morphInitBlend(sdfMotNodeMorph_t *aThis) {
    aThis->blendSrc = *aThis->morphNode;
}
