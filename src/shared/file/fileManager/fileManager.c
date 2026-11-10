#include "shared/file/fileManager/decls.h"

s32 fileManager(void) {
    fileMngCb_tag *work = &fileQueueHead;
    u32 fullcnt;
    pacFileRequest_t **link;
    pacFileRequest_t *req;

    fileExecCallback();
    fullcnt = work->activeCount;
    while (fullcnt != 0) {
        u32 slot = work->index;
        const u8 *input = work->buffer + (slot << 16);
        fileCb_t *job = (fileCb_t *)work->data[slot].req;
        u8 kind = job->kind;

        if (kind == 1) {
            pacFileRequest_t *req = (pacFileRequest_t *)job;
            if (req != NULL &&
                sdfPacDecode(&req->pac, input, work->data[slot].size) == -1) {
                fileUnlinkQueue((fileRequest_t *)req);
                req->pacrest = kind;
                if (req->callback != NULL) {
                    pacFileRequest_t **append = &work->pacEndCallbackLink;
                    pacFileRequest_t *next;

                    while ((next = *append) != NULL) {
                        append = &next->cbnext;
                    }
                    *append = req;
                }
            }
            WaitSema(work->semaId);
            slot++;
            if (slot == 4) {
                slot = 0;
            }
            work->index = slot;
            work->freeCount++;
            work->activeCount--;
            SignalSema(work->semaId);
            fullcnt = work->activeCount;
        }
    }
    fileExecQueue();
    link = &work->pacEndCallbackLink;
    while ((req = *link) != NULL) {
        if (req->state == 6) {
            *link = req->cbnext;
            ((void (*)(pacFileRequest_t *, void *))req->callback)(req, req->arg);
        } else {
            link = &req->cbnext;
        }
    }
    return work->queue != NULL;
}
