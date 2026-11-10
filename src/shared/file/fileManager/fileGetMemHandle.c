#include "shared/file/fileManager/decls.h"

sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle) {
    return fileHandle->handle;
}
