#include "shared/sdf/sdfDelayedResrc/decls.h"

void sdfDelayedResrcEntry(sdfDelayedResrcCB_t *aCB, void *aItem) {
    sdfDelayedResrcChunk_t *pendingNode;
    sdfDelayedResrcItems_t *entryBuffer;
    s32 freeEntries;

    if (aItem != NULL) {
        WaitSema(sdfDelayedResrcSemaId);
        pendingNode = aCB->chunk;
        if (pendingNode == NULL) {
            pendingNode = (sdfDelayedResrcChunk_t *)sdfChipAllocDirect(sizeof(sdfDelayedResrcChunk_t));
            pendingNode->itemsFreeCount = 0;
            pendingNode->next = sdfDelayedResrcLink;
            pendingNode->items = NULL;
            pendingNode->owner = aCB;
            aCB->chunk = pendingNode;
            sdfDelayedResrcLink = pendingNode;
        }
        freeEntries = pendingNode->itemsFreeCount;
        entryBuffer = pendingNode->items;
        if (freeEntries == 0) {
            sdfDelayedResrcItems_t *newBuffer = (sdfDelayedResrcItems_t *)sdfChipAllocDirect(sizeof(sdfDelayedResrcItems_t));
            newBuffer->next = entryBuffer;
            pendingNode->items = newBuffer;
            entryBuffer = newBuffer;
            freeEntries = SDF_PENDING_BUFFER_CAPACITY;
        }
        entryBuffer->items[SDF_PENDING_BUFFER_CAPACITY - freeEntries] = aItem;
        pendingNode->itemsFreeCount = freeEntries - 1;
        SignalSema(sdfDelayedResrcSemaId);
    }
}
