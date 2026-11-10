#include "shared/sdf/sdfFile/decls.h"

/* Convert a relative disc path to the drive's uppercase backslash form with ;1 suffix. */
void sdfFileSetCdFilename(char *aDst, char *aSrc) {
    s32 c;
    memcpy(aDst, sdfFileCdromPrefix, 8);
    aDst += 7;
    c = *aSrc++;
    while (c != 0) {
        if (c == '/') {
            c = '\\';
        }
        if (c >= 'a' && c <= 'z') {
            c -= 0x20;
        }
        *aDst++ = c;
        c = *aSrc++;
    }
    aDst[0] = ';';
    aDst[1] = '1';
    aDst[2] = 0;
}
