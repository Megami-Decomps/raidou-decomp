#include "shared/file/fileManager/decls.h"

void fileLinkQueue(fileRequest_t *req, s32 kind, const char *name, void *callback, void *arg) {
    fileMngCb_tag *q = &fileQueueHead;
    char *dup;
    s32 first;

    req->kind = kind;
    dup = sdfStrdupChip(name);
    req->callback = (void (*)(fileRequest_t *, u32))callback;
    req->name = dup;
    req->arg = arg;

    WaitSema(q->semaId);
    if (q->queueTail == NULL) {
        q->queue = req;
        first = 1;
    } else {
        q->queueTail->next = req;
        first = 0;
    }
    q->queueTail = req;
    SignalSema(q->semaId);

    if (first != 0) {
        fileExecQueue();
    }
}
