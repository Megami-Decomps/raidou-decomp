#include "shared/file/fileManager/decls.h"

void fileUnlinkQueue(fileRequest_t *fileHandle) {
    fileMngCb_tag *work = &fileQueueHead;
    fileRequest_t *previousNode;
    fileRequest_t *currentNode;
    s32 slotIndex;

    WaitSema(work->semaId);
    for (slotIndex = 0; slotIndex != 4; slotIndex++) {
        if (work->data[slotIndex].req == fileHandle) {
            work->data[slotIndex].req = NULL;
        }
    }
    previousNode = NULL;
    currentNode = work->queue;
    while (currentNode != NULL) {
        if (currentNode == fileHandle) {
            if (currentNode->next == NULL) {
                work->queueTail = previousNode;
            }
            if (previousNode == NULL) {
                work->queue = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    SignalSema(work->semaId);
}
