#include "shared/file/fileManager/decls.h"

fileRequest_t *fileSaveEx(const char *filename, const void *buff, s32 size, s32 endCallback, s32 callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(0x30);

    req->top = EE_ADDR(buff);
    req->ptr = EE_ADDR(buff);
    req->size = size;
    req->rest = size;
    fileLinkQueue(req, 2, filename, EE_ADDR_AS(void *, endCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}
