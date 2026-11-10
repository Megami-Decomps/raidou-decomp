#include "shared/file/fileManager/decls.h"

s32 pacLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->dev = fileHandle;
        data->state = 2;
        sdfFileCmdFileSize(fileHandle);
        break;
    case 4:
        data->rest = length;
        data->state = 3;
        data->size = length;
        pacRead((fileRequest_t *)data);
        break;
    case 5:
        WaitSema(work->semaId);
        if (data->pacrest != 0) {
            data->state = 5;
        } else {
            work->data[data->slot].size = length;
            data->rest -= length;
            if (data->rest == 0) {
                data->state = 5;
                work->queue = (fileRequest_t *)data->next;
                if (data->next == NULL) {
                    work->queueTail = NULL;
                }
            } else {
                data->state = 3;
            }
        }
        work->activeCount++;
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
        } else {
            SignalSema(work->semaId);
        }
        saved = fileCallbackStatus;
        fileCallbackStatus = 1;
        fileExecQueue();
        fileCallbackStatus = saved;
        break;
    case 7:
        WaitSema(work->semaId);
        fileUnlinkCloseWait(work, (fileRequest_t *)data);
        SignalSema(work->semaId);
        sdfFileCmdFree(fileHandle);
        data->dev = NULL;
        data->state = 6;
        break;
    }
    return 0;
}
