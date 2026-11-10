#include "shared/model/mdlManager/decls.h"

void mdlMakeHddFileListDummy(void)
{
    char *name;
    char *src;
    s32 i;

    mdlReleaseHddFileList();
    mdlHddFileIndexHandle = sdfMemAllocDirect(sizeof(dds3ModelFileIndex_t));
    mdlHddFileIndex = sdfMemLock(mdlHddFileIndexHandle);
    mdlHddFileIndex->major = 5;
    mdlHddFileIndex->minor = 0;
    mdlHddFileIndex->scale = 0x1000;
    mdlHddFileIndex->radius = 1000;
    mdlHddFileListHandle = sdfMemAllocDirect(sizeof(dds3ModelFileTable_t));
    mdlHddFileList = sdfMemLock(mdlHddFileListHandle);
    for (i = 0; i != 3; i++)
    {
        switch (i)
        {
        case 0:
            src = modelFileTable[0].entries->modelFile;
            break;
        case 1:
            src = modelFileTable[0].entries->texFile;
            break;
        default:
            src = modelFileTable[0].entries->motionFile;
            break;
        }
        if (src == NULL)
        {
            continue;
        }
        name = sdfChipAllocDirect(strlen(src) + 1);
        strcpy(name, src);
        switch (i)
        {
        case 0:
            mdlHddFileList->modelFile = name;
            break;
        case 1:
            mdlHddFileList->texFile = name;
            break;
        case 2:
            mdlHddFileList->motionFile = name;
            break;
        }
    }
    modelFileKindTable[5].entries = mdlHddFileIndex;
    modelFileKindTable[5].count = 1;
    modelFileTable[5].entries = mdlHddFileList;
    modelFileTable[5].count = 1;
}
