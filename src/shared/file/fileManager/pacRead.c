#include "shared/file/fileManager/decls.h"

void pacRead(fileRequest_t *pacHandle) {
    fileMngCb_tag *work = &fileQueueHead;
    u8 slotIndex;
    s32 chunkBytes;

    WaitSema(work->semaId);
    if (work->freeCount == 0) {
        SignalSema(work->semaId);
        return;
    }
    if (pacHandle->state != 3) {
        SignalSema(work->semaId);
        return;
    }
    pacHandle->state = 4;
    slotIndex = work->readIndex;
    if (slotIndex == 3) {
        work->readIndex = 0;
    } else {
        work->readIndex = slotIndex + 1;
    }
    pacHandle->slot = slotIndex;
    work->freeCount--;
    work->data[slotIndex].req = pacHandle;
    chunkBytes = pacHandle->rest;
    if (chunkBytes > 0x10000) {
        chunkBytes = 0x10000;
    }
    SignalSema(work->semaId);
    sdfFileCmdRead((sdfFileCmd_t *)pacHandle->dev,
                    work->buffer + (slotIndex << 16), chunkBytes);
}
