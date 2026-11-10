#include "shared/file/fileManager/decls.h"

void fileExecCallback(void) {
    fileMngCb_tag *work = &fileQueueHead;
    fileCb_t *node;

    WaitSema(work->semaId);
    while ((node = work->endCallbackLink) != NULL) {
        work->endCallbackLink = node->donenext;
        SignalSema(work->semaId);
        node->callback(node, EE_ADDR(node->arg));
        WaitSema(work->semaId);
    }
    SignalSema(work->semaId);
}
