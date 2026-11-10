#include "shared/model/mdlEffect/decls.h"

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
pibHeaderChunk_t *mdlPibGetNextChunkItem(pibHeaderChunk_t *data) {
    pibHeaderChunk_t *nextRecord = EE_ADDR_AS(pibHeaderChunk_t *, (EE_ADDR_S(data) + data->nextOffset));

    if (nextRecord->group == MDL_RECORD_END_KIND) {
        nextRecord = NULL;
    }
    return nextRecord;
}
