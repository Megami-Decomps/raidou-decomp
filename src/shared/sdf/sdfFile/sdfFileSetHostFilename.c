#include "shared/sdf/sdfFile/decls.h"

char *sdfFileSetHostFilename(char *aDst, char *aSrc) {
    if (*aSrc == 0x2F) {
        strcpy(aDst, sdfFileHostRootPath);
        return strcat(aDst, aSrc);
    }
    *(sdfHostPrefix_t *)aDst = sdfFileHostPrefix[0];
    return strcat(aDst, aSrc);
}
