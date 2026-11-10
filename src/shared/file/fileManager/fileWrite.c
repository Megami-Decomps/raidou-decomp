#include "shared/file/fileManager/decls.h"

void fileWrite(fileCb_t *fileHandle) {
    fileMngCb_tag *q = &fileQueueHead;

    WaitSema(q->semaId);
    if (fileHandle->state != 3) {
        SignalSema(q->semaId);
        return;
    }
    fileHandle->state = 4;
    SignalSema(q->semaId);
    sdfFileCmdWrite(fileHandle->dev, EE_ADDR_AS(void *, fileHandle->ptr), fileHandle->rest <= 0x8000 ? fileHandle->rest : 0x8000);
}
