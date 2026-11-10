#include "shared/model/mdlEffect/decls.h"

void mdlCreateEffect_BLUR(mdlObj_t *owner, pibHeaderChunk_t *record) {
    mdlSetBlur(owner, record->payload.stream.selectorA,
                  record->payload.stream.selectorB,
                  record->parameter.word, record->tail.word10);
}
