#include "shared/model/mdlManager/decls.h"

s32 mdlCheckLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t *cursor;

    cursor = mdlLoadingMajorList[aMajor];
    while (cursor != NULL)
    {
        if (cursor->minor == aMinor)
        {
            return 1;
        }
        cursor = cursor->next;
    }
    return 0;
}
