#include "shared/sdf/sdfDraw/decls.h"

void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag) {
    if (aChain->tail == NULL) {
        aChain->tagHead = aTag;
    }
    else {
        aChain->tail->tail->next = aTag->tail;
    }
    aChain->tail = aTag;
}
