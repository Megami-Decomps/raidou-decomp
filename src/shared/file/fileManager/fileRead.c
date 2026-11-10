#include "shared/file/fileManager/decls.h"

void fileRead(fileCb_t *fileHandle) {
    fileMngCb_tag *q = &fileQueueHead;

    WaitSema(q->semaId);
    if (fileHandle->state != 3) {
        SignalSema(q->semaId);
        return;
    }
    fileHandle->state = 4;
    SignalSema(q->semaId);
    sdfFileCmdRead(fileHandle->dev, EE_ADDR_AS(void *, fileHandle->ptr), fileHandle->rest <= 0x8000 ? fileHandle->rest : 0x8000);
}
