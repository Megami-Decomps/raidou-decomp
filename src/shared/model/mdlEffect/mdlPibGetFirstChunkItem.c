#include "shared/model/mdlEffect/decls.h"

/* The first eight bytes belong to the enclosing list, not its first record. */
pibHeaderChunk_t *mdlPibGetFirstChunkItem(pibHeaderChunk_t *data) {
    pibHeaderChunk_t *firstRecord = (pibHeaderChunk_t *)((u8 *)data + MDL_RECORD_LIST_HEADER_BYTES);

    if (firstRecord->group == MDL_RECORD_END_KIND) {
        firstRecord = NULL;
    }
    return firstRecord;
}
