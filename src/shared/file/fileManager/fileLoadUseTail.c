#include "shared/file/fileManager/decls.h"

fileRequest_t *fileLoadUseTail(const char *filename) {
    return fileLoadEx(filename, 1, 0, 0);
}
