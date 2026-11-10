#include "shared/sdf/sdfTex/decls.h"

sdfTexHandle_t *sdfTexSearch(u32 id)
{
    sdfTexHandle_t *handle;

    for (handle = sdfTexHandleLink; handle != NULL; handle = handle->next) {
        if (handle->texId == id) {
            return handle;
        }
    }
    return NULL;
}
