#include "shared/file/fileManager/decls.h"

u32 fileGetLoadRemain(fileRequest_t *fileHandle) {
    return fileHandle->rest;
}
