#include "shared/file/fileManager/decls.h"

fileRequest_t *pacLoad(const char *filename) {
    return pacLoadEx(filename, 0, 0, 0, 0);
}
