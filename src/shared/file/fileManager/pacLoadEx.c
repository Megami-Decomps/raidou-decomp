#include "shared/file/fileManager/decls.h"

fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(sizeof(fileRequest_t));

    sdfPacInit(&req->pac, EE_ADDR_AS(void *, callback));
    if (headOrTail != 0) {
        sdfPacSetFromTail(&req->pac);
    }
    fileLinkQueue(req, 1, filename, EE_ADDR_AS(void *, pendCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}
