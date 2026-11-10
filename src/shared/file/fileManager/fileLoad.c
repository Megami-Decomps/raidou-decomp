#include "shared/file/fileManager/decls.h"

fileRequest_t *fileLoad(const char *filename) {
    return fileLoadEx(filename, 0, 0, 0);
}
