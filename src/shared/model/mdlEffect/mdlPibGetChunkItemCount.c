#include "shared/model/mdlEffect/decls.h"

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlPibGetChunkItemCount(pibHeaderChunk_t *data) {
    s32 recordCount;
    pibHeaderChunk_t *recordCursor;

    if (data == NULL) {
        return 0;
    }
    recordCursor = mdlPibGetFirstChunkItem(data);
    recordCount = 0;
    while (recordCursor != NULL) {
        recordCount++;
        recordCursor = mdlPibGetNextChunkItem(recordCursor);
    }
    return recordCount;
}
