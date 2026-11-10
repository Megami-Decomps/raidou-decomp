#include "shared/file/fileManager/decls.h"

void fileSyncAll(void) {
    fileMngCb_tag *work = &fileQueueHead;
    while (work->queue != 0 || work->closeLink != 0) {
        fileManager();
    }
}
