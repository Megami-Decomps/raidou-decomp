#include "shared/file/fileManager/decls.h"

s32 fileLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->dev = fileHandle;
        data->state = cause;
        sdfFileCmdFileSize(fileHandle);
        break;
    case 4: {
        sdfMemHandle_t *handle;

        data->rest = length;
        data->size = length;
        if (data->mode == 0) {
            handle = sdfMemAlloc2Direct(length);
            data->handle = handle;
            if (handle == NULL) {
                data->retry = 10;
                data->state = 7;
                break;
            }
        } else {
            handle = sdfMemAllocFromTailDirect(length);
            data->handle = handle;
        }
        {
            u32 address = sdfMemLock(handle);

            data->ptr = address;
            data->top = address;
            data->state = 3;
        }
        fileRead(data);
        break;
    }
    case 5:
        WaitSema(work->semaId);
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
            data->ptr += length;
        }
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
            saved = fileCallbackStatus;
            fileCallbackStatus = 2;
            fileExecQueue();
            fileCallbackStatus = saved;
        } else {
            SignalSema(work->semaId);
            fileRead(data);
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
