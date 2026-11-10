#include "shared/model/mdlManager/decls.h"

void mdlFreeBlur(mdlObj_t *aHandle)
{
    dds3ModelBlurCb_t *list;
    dds3ModelBlurInstance_t *slot;
    dds3ModelBlurInstance_t *next;

    list = aHandle->devList;
    if (list == NULL)
    {
        return;
    }
    next = list->instants;
    while (next != NULL)
    {
        slot = next;
        next = next->next;
        sdfModelRootFree(slot->modelobj, 1, 1);
        sdfChipFreeDirect(slot);
    }
    sdfChipFreeDirect(list);
    aHandle->devList = NULL;
}
