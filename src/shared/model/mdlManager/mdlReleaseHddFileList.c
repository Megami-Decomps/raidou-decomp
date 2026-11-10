#include "shared/model/mdlManager/decls.h"

void mdlReleaseHddFileList(void)
{
    dds3ModelFileTable_t *entries;
    s32 count;
    s32 i;

    count = modelFileTable[5].count;
    if (count > 0)
    {
        entries = modelFileTable[5].entries;
        i = 0;
        do
        {
            sdfChipFreeDirect(entries[i].modelFile);
            sdfChipFreeDirect(entries[i].texFile);
            sdfChipFreeDirect(entries[i].motionFile);
            i++;
        } while (i < count);
    }
    modelFileKindTable[5].entries = NULL;
    modelFileKindTable[5].count = 0;
    modelFileTable[5].entries = NULL;
    modelFileTable[5].count = 0;
    sdfMemFreeHandleDirect(&mdlHddFileIndexHandle);
    sdfMemFreeHandleDirect(&mdlHddFileListHandle);
}
