#include "shared/model/mdlManager/decls.h"

void mdlGetFilebase(s32 aMajor, s32 aMinor, char *aName, s32 aNameLeng) {
    dds3ModelFileIndex_t *selection;
    dds3ModelFileTable_t *pathEntry;
    const char *path;
    const char *basename;
    s32 pathLength;
    s32 startIndex;
    s32 endIndex;
    s32 copyLength;

    selection = (dds3ModelFileIndex_t *)modelFileKindTable[aMajor].entries;
    selection += aMinor;
    pathEntry = (dds3ModelFileTable_t *)modelFileTable[selection->major].entries;
    pathEntry += selection->minor;
    path = pathEntry->modelFile;
    startIndex = strlen(path);
    pathLength = startIndex;
    startIndex--;

    while (1) {
        basename = &path[startIndex];
        if (*basename == '/') {
            startIndex++;
            basename = &path[startIndex];
            break;
        }
        if (startIndex == 0) {
            break;
        }
        startIndex--;
    }

    endIndex = startIndex;
    do {
        endIndex++;
        if (endIndex >= pathLength) {
            break;
        }
    } while (path[endIndex] != '.');

    copyLength = endIndex - startIndex;
    aNameLeng--;
    if (aNameLeng < copyLength) {
        copyLength = aNameLeng;
    }
    memcpy(aName, basename, copyLength);
    aName[copyLength] = '\0';
}
