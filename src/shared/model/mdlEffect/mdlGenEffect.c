#include "shared/model/mdlEffect/decls.h"

/* Find the requested record list and associate each entry with this motion slot. */
void mdlGenEffect(mdlObj_t *aHandle, s32 aGroup, s32 aPlayGroup) {
    pibHeaderChunk_t *recordList = mdlPibGetChunk(aHandle, aGroup);
    if (recordList != NULL) {
        pibHeaderChunk_t *recordCursor = mdlPibGetFirstChunkItem(recordList);
        while (recordCursor != NULL) {
            mdlCreateEffect(aHandle, recordCursor, aPlayGroup);
            recordCursor = mdlPibGetNextChunkItem(recordCursor);
        }
    }
}
