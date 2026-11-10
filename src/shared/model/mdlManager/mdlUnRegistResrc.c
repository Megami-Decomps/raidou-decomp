#include "shared/model/mdlManager/decls.h"

void mdlUnRegistResrc(dds3ModelResrc_t *resrc_p)
{
    dds3ModelResrc_t *previousNode;
    dds3ModelResrc_t *nextNode;
    u8 ownsResources;
    s32 slotIndex;

    if (resrc_p == NULL)
    {
        return;
    }
    previousNode = resrc_p->prev;
    nextNode = resrc_p->next;
    if (previousNode == NULL)
    {
        mdlResrcMajorList[resrc_p->major] = nextNode;
    }
    else
    {
        previousNode->next = nextNode;
    }
    if (nextNode != NULL)
    {
        nextNode->prev = previousNode;
    }
    ownsResources = resrc_p->autoFreeFlag;
    resrc_p->autoFreeFlag = 0;
    if (resrc_p->models != NULL)
    {
        do
        {
            mdlFree(resrc_p->models);
        } while (resrc_p->models != NULL);
    }
    if (ownsResources != 0)
    {
        sdfTexListFree(resrc_p->texList, 1);
        sdfMemFreeDelayedDirect(resrc_p->modelDefHandle);
        for (slotIndex = 0; slotIndex != MDL_SLOT_COUNT; slotIndex++)
        {
            if (resrc_p->playbooks[slotIndex].handle != NULL)
            {
                sdfMemFreeDirect(resrc_p->playbooks[slotIndex].handle);
            }
        }
    }
    mdlEffectResrcListFree(resrc_p->effectResrcList);
    sdfMemFreeDirect(resrc_p->effectMemHandle);
    sdfChipFreeDirect(resrc_p);
}
