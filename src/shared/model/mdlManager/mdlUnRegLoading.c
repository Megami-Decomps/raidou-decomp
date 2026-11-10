#include "shared/model/mdlManager/decls.h"

void mdlUnRegLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t **link;
    mdlLoadingTag_t *cursor;

    link = &mdlLoadingMajorList[aMajor];
    cursor = *link;
    if (cursor == NULL)
    {
        return;
    }
    do
    {
        if (cursor->minor == aMinor)
        {
            *link = cursor->next;
            sdfChipFreeDirect(cursor);
            break;
        }
        else
        {
            link = &cursor->next;
            cursor = cursor->next;
        }
    } while (cursor != NULL);
}
