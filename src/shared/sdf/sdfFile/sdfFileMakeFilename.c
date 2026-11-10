#include "shared/sdf/sdfFile/decls.h"

/* Return an allocated device path, normalizing disc-path case and separators. */
char *sdfFileMakeFilename(char *aFilename) {
    s32 length = strlen(aFilename);
    char *result = sdfChipAllocDirect(0x80);
    char *cursor;
    s32 character;
    if (strchr(aFilename, ':') != NULL) {
        memcpy(result, aFilename, length);
        result[length] = 0;
        if (strncmp(result, sdfFileCdromName, 5) == 0) {
            cursor = result + 5;
            character = *cursor;
            while (character != 0) {
                if (character == '/') *cursor = '\\';
                else if (character >= 'a' && character <= 'z') *cursor = character - 0x20;
                cursor++;
                character = *cursor;
            }
            *cursor++ = ';';
            *cursor++ = '1';
            *cursor = 0;
        }
    } else if (sdfFileChkCdFile(aFilename)) {
        sdfFileSetCdFilename(result, aFilename);
    } else if (sdfFileChkHostFile(aFilename)) {
        sdfFileSetHostFilename(result, aFilename);
    } else if (sdfFileChkHddFile(aFilename)) {
        sdfFileSetHddFilename(result, aFilename);
    } else {
        switch (sdfFileDefaultSrcType) {
        case 0: sdfFileSetCdFilename(result, aFilename); break;
        case 2: sdfFileSetHddFilename(result, aFilename); break;
        case 1: sdfFileSetHostFilename(result, aFilename); break;
        default: strcpy(result, aFilename); break;
        }
    }
    return result;
}
