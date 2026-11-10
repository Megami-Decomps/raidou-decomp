#include "shared/sdf/sdfFile/decls.h"

/* Set the current device directory, ending in a slash; path is assumed nonempty. */
void sdfFileSetHostRoot(char *aRootPath) {
    s32 length = strlen(aRootPath);
    ((sdfDevicePrefix_t *)sdfFileHostRootPath)->head = ((sdfDevicePrefix_t *)sdfFileHostPrefix)->head;
    ((sdfDevicePrefix_t *)sdfFileHostRootPath)->tail = ((sdfDevicePrefix_t *)sdfFileHostPrefix)->tail;
    memcpy(sdfFileHostRootPath + 6, aRootPath, length);
    if (aRootPath[length - 1] != '/') {
        sdfFileHostRootPath[length + 6] = '/';
        sdfFileHostRootPath[length + 7] = 0;
    } else {
        sdfFileHostRootPath[length + 6] = 0;
    }
}
