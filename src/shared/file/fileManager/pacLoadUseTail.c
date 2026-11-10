#include "shared/file/fileManager/decls.h"

fileRequest_t *pacLoadUseTail(const char *filename) {
    return pacLoadEx(filename, 1, 0, 0, 0);
}
