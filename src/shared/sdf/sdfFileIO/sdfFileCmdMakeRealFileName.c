#include "shared/sdf/sdfFileIO/decls.h"

/* Allocate a worker-specific device path; an unsupported worker returns NULL. */
char *sdfFileCmdMakeRealFileName(const char *aSrcName, s32 aDeviceType) {
    s32 length = strlen(aSrcName);
    char *result;
    s32 index;
    s32 character;
    char *base;
    s32 baseLength;
    switch (aDeviceType) {
    case 0:
        result = sdfChipAllocDirect(length + 10);
        memcpy(result, sdfFileCmdCdromPrefix, 7);
        index = 0;
        character = aSrcName[index];
        while (character != 0) {
            if (character == '/') character = '\\';
            else if (character >= 'a' && character <= 'z') character -= 0x20;
            result[index + 7] = character;
            index++;
            character = aSrcName[index];
        }
        result[index + 7] = ';';
        result[index + 8] = '1';
        result[index + 9] = 0;
        break;
    case 1:
        if (aSrcName[0] != '/') {
            result = sdfChipAllocDirect(length + 7);
            memcpy(result, sdfFileCmdHostPrefix, 6);
            memcpy(result + 6, aSrcName, length);
            result[length + 6] = 0;
        } else {
            base = sdfFileGetHostRoot();
            baseLength = strlen(base);
            result = sdfChipAllocDirect(length + baseLength);
            memcpy(result, base, baseLength);
            memcpy(result + baseLength, aSrcName + 1, length);
        }
        break;
    case 2:
        result = sdfChipAllocDirect(length + 6);
        memcpy(result, sdfFileCmdPfsPrefix, 5);
        memcpy(result + 5, aSrcName, length);
        result[length + 5] = 0;
        break;
    case 3:
        result = sdfChipAllocDirect(length + 1);
        strcpy(result, aSrcName);
        break;
    default:
        return NULL;
    }
    return result;
}
