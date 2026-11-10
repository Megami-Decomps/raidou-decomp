#include "shared/sdf/sdfFileIO/decls.h"

/* Unlink a cached request under interrupt protection, then release its storage. */
void sdfFileCmdUnlink(void *aHandle)
{
    sdfFileCmdHandle_t *state = aHandle;
    s32 interrupts;
    sdfFileCmdHandle_t *previous;
    sdfFileCmdHandle_t *next;

    interrupts = DIntr();
    previous = state->prev;
    next = state->next;
    if (previous == NULL) {
        sdfFileCmdCacheHead = next;
    } else {
        previous->next = next;
    }
    if (next == NULL) {
        sdfFileCmdCacheTail = previous;
    } else {
        next->prev = previous;
    }
    sdfFileCmdCount -= 1;
    if (interrupts != 0) {
        EIntr();
    }
    sdfChipFreeDirect(state->filename);
    sdfChipFreeDirect(state);
}
