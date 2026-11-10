#include "shared/model/mdlManager/decls.h"

void mdlInitManager(void)
{
    s32 groupIndex;

    mdlManagerWork = sdfCreateSema(1, 0x7F, 0);
    for (groupIndex = 0; groupIndex != MDL_GROUP_COUNT; groupIndex++)
    {
        mdlResrcMajorList[groupIndex] = NULL;
        mdlLoadingMajorList[groupIndex] = NULL;
    }
}
