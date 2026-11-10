#include "shared/sdf/sdfDraw/decls.h"

void sdfPreDrawChainInit(sdfPreDrawChain_t *aChain) {
    aChain->tagHead = NULL;
    aChain->tail = NULL;
}
