#include "shared/file/fileManager/decls.h"

s32 fileSaveCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->state = 3;
        fileWrite(data);
        break;
    case 6:
        WaitSema(work->semaId);
        data->ptr += length;
        data->rest -= length;
        if (data->rest == 0) {
            data->state = 5;
            if (work->queue == (fileRequest_t *)data) {
                work->queue = (fileRequest_t *)data->next;
                if (data->next == NULL) {
                    work->queueTail = NULL;
                }
            }
        } else {
            data->state = 3;
        }
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
            saved = fileCallbackStatus;
            fileCallbackStatus = 3;
            fileExecQueue();
            fileCallbackStatus = saved;
        } else {
            SignalSema(work->semaId);
            fileWrite(data);
        }
        break;
    case 7:
        sdfFileCmdFree(fileHandle);
        data->dev = NULL;
        data->state = 6;
        WaitSema(work->semaId);
        fileUnlinkCloseWait(work, (fileRequest_t *)data);
        if (data->callback != NULL) {
            fileCb_t **link = &work->endCallbackLink;
            fileCb_t *next;

            while ((next = *link) != NULL) {
                link = &next->donenext;
            }
            *link = data;
        }
        SignalSema(work->semaId);
        break;
    }
    return 0;
}
