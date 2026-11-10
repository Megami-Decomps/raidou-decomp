#include "shared/sdf/sdfFile/decls.h"

void sdfFileSetHddFilename(char *aDst, char *aSrc) {
    memcpy(aDst, sdfFileHddPrefix, 6);
    strcat(aDst, aSrc);
}
