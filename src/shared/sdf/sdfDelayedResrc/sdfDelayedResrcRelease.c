#include "shared/sdf/sdfDelayedResrc/decls.h"

void sdfDelayedResrcRelease(sdfDelayedResrcChunk_t *aChunk) {
    sdfDelayedResrcChunk_t *nextPendingNode;
    sdfDelayedResrcItems_t *entryBuffer;
    sdfDelayedResrcItems_t *nextEntryBuffer;
    SdfPendingCallback entryHandler;
    s32 entryCount;
    s32 entryIndex;

    if (aChunk != NULL) {
        do {
            entryCount = SDF_PENDING_BUFFER_CAPACITY - aChunk->itemsFreeCount;
            entryHandler = aChunk->owner->freeFuncPtr;
            entryBuffer = aChunk->items;
            if (entryBuffer != NULL) {
                do {
                    entryIndex = 0;
                    do {
                        entryHandler(entryBuffer->items[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < entryCount);
                    nextEntryBuffer = entryBuffer->next;
                    sdfChipFreeDirect(entryBuffer);
                    entryBuffer = nextEntryBuffer;
                    entryCount = SDF_PENDING_BUFFER_CAPACITY;
                } while (entryBuffer != NULL);
            }
            nextPendingNode = aChunk->next;
            sdfChipFreeDirect(aChunk);
            aChunk = nextPendingNode;
        } while (aChunk != NULL);
    }
}
