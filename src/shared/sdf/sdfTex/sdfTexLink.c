#include "shared/sdf/sdfTex/decls.h"

void sdfTexLink(sdfTexHandle_t *handle)
{
    handle->prev = NULL;
    if (sdfTexHandleLink != NULL) {
        handle->next = sdfTexHandleLink;
        sdfTexHandleLink->prev = handle;
    } else {
        handle->next = NULL;
    }
    sdfTexHandleLink = handle;
}
