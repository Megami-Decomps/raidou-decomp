#include "shared/file/fileManager/decls.h"

u32 fileGetLoadSize(const fileRequest_t *fileHandle) {
    return fileHandle->size;
}
