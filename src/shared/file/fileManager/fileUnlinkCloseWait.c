#include "shared/file/fileManager/decls.h"

void fileUnlinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle) {
    fileRequest_t **link = &cb->closeLink;
    fileRequest_t *p;

    while ((p = *link) != fileHandle) {
        link = &p->next;
    }
    *link = fileHandle->next;
}
