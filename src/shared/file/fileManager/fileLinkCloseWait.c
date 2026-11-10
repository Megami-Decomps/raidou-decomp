#include "shared/file/fileManager/decls.h"

void fileLinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle) {
    fileHandle->next = cb->closeLink;
    cb->closeLink = fileHandle;
}
