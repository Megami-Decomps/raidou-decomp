#include "shared/model/mdlEffect/decls.h"

/* Follow the relative links in a resource's record table to find an ID. */
pibHeaderChunk_t *mdlPibGetChunk(mdlObj_t *handle, s32 group) {
    pibHeaderChunk_t *recordTable = handle->sub->effectData;
    pibHeaderChunk_t *recordCursor;
    s32 remainingRecords;

    if (recordTable == NULL) {
        return NULL;
    }
    recordCursor = recordTable;
    remainingRecords = mdlPibGetChunkCount(recordCursor);
    do {
        remainingRecords--;
        recordCursor = EE_ADDR_AS(pibHeaderChunk_t *, (EE_ADDR_S(recordCursor) + recordCursor->nextOffset));
        if (remainingRecords == -1) {
            return NULL;
        }
    } while (recordCursor->group != group);
    return recordCursor;
}
