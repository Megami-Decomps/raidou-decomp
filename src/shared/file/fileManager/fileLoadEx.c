#include "shared/file/fileManager/decls.h"

fileRequest_t *fileLoadEx(const char *filename, u32 headOrTail, addr_t endCallback, addr_t callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(0x30);

    req->mode = headOrTail;
    fileLinkQueue(req, 0, filename, EE_ADDR_AS(void *, endCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}
