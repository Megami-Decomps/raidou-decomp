#include "shared/model/mdlManager/decls.h"

void mdlUnRegistAll(void)
{
    u32 groupIndex;
    dds3ModelResrc_t **groupHead;
    dds3ModelResrc_t *groupNode;
    dds3ModelResrc_t *nextNode;

    groupIndex = 0;
    groupHead = mdlResrcMajorList;
    do
    {
        groupNode = *groupHead;
        while (groupNode != NULL)
        {
            nextNode = groupNode->next;
            mdlUnRegistResrc(groupNode);
            groupNode = nextNode;
        }
        groupIndex++;
        groupHead++;
    } while (groupIndex < MDL_GROUP_COUNT);
}
