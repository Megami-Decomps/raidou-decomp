#include "shared/sdf/sdfGsMemory/decls.h"

s32 sdfGsMemCheckHandle(sdfGsMemSlice_t *aHandle) {
    sdfGsMemSlice_t *node = gsMemHandleLinkHead;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == (void *)aHandle) {
            return 1;
        }
        node = node->next;
    } while (node != NULL);
    return 0;
}
