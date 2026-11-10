#include "shared/model/mdlManager/decls.h"

dds3ModelResrc_t *mdlSearchResrc(s32 aMajor, s32 aMinor)
{
    dds3ModelResrc_t *entry;

    entry = mdlResrcMajorList[aMajor];
    while (entry != NULL)
    {
        if (entry->minor == aMinor)
        {
            break;
        }
        entry = entry->next;
    }
    return entry;
}
