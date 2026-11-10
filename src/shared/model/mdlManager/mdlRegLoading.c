#include "shared/model/mdlManager/decls.h"

void mdlRegLoading(s32 aMajor, s32 aMinor)
{
    mdlLoadingTag_t *entry;
    mdlLoadingTag_t **head;

    entry = sdfChipAllocDirect(sizeof(mdlLoadingTag_t));
    head = &mdlLoadingMajorList[aMajor];
    entry->minor = aMinor;
    entry->next = *head;
    *head = entry;
}
