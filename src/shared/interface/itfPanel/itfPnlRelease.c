#include "shared/interface/itfPanel/decls.h"

/* A sprite with a retained payload owns a secondary allocation. Release that
 * before its primary allocation; NULL is a no-op. */
void itfPnlRelease(itfpanel_t *handle) {
    if (handle != NULL) {
        if (handle->pwork != NULL) {
            sdfMemFreeDirect(handle->payloadAllocation);
        }
        sdfMemFreeDirect(handle->allocation);
    }
}
