#include "shared/interface/frFont/decls.h"

/* Retain the chain if any fade value changed; otherwise release it and return NULL. */
frFontHandle_t *frDestroyFontRequest(frFontHandle_t *pFRQ) {
    if (frEraseEffect(pFRQ) != 0) {
        return pFRQ;
    }
    return frDestroyFontRequestNow(pFRQ);
}
