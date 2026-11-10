#include "shared/file/fileManager/decls.h"

fileRequest_t *fileSave(const char *filename, const void *buff, s32 size) {
    return fileSaveEx(filename, buff, size, 0, 0);
}
