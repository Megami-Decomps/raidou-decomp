#include "shared/model/mdlManager/decls.h"

void mdlRegist(s32 aMajor, s32 aMinor, s32 aFlag,
               sdfModelTexList_t *aTexList, sdfModelDef_t *aData,
               sdfMemHandle_t *aHandle)
{
    dds3ModelResrc_t *groupNode;
    dds3ModelResrc_t *previousHead;
    s32 slotIndex;

    mdlUnRegist(aMajor, aMinor);
    groupNode = sdfChipAllocDirect(sizeof(dds3ModelResrc_t));
    previousHead = mdlResrcMajorList[aMajor];
    if (previousHead != NULL)
    {
        previousHead->prev = groupNode;
    }
    mdlResrcMajorList[aMajor] = groupNode;
    groupNode->next = previousHead;
    groupNode->major = aMajor;
    groupNode->minor = aMinor;
    groupNode->texList = aTexList;
    groupNode->modelDefPtr = aData;
    groupNode->modelDefHandle = aHandle;
    groupNode->prev = NULL;
    groupNode->models = NULL;
    for (slotIndex = 0; slotIndex != MDL_SLOT_COUNT; slotIndex++)
    {
        groupNode->playbooks[slotIndex].flags = 0;
        groupNode->playbooks[slotIndex].ptr = NULL;
        groupNode->playbooks[slotIndex].handle = NULL;
    }
    groupNode->autoFreeFlag = aFlag & 1;
    groupNode->effectMemHandle = NULL;
    groupNode->effectData = NULL;
    groupNode->effectResrcList = NULL;
    groupNode->scale = 1.0f;
    groupNode->radius = 100.0f;
}
