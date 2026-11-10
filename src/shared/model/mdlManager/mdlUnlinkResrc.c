#include "shared/model/mdlManager/decls.h"

void mdlUnlinkResrc(mdlObj_t *aHandle)
{
    mdlObj_t *nextContext;
    mdlObj_t *previousContext;
    dds3ModelResrc_t *group;

    nextContext = aHandle->next;
    previousContext = aHandle->prev;
    if (nextContext != NULL)
    {
        nextContext->prev = previousContext;
    }
    if (previousContext != NULL)
    {
        previousContext->next = nextContext;
    }
    else
    {
        group = aHandle->sub;
        if (nextContext != NULL)
        {
            group->models = nextContext;
        }
        else
        {
            group->models = NULL;
            if (group->autoFreeFlag != 0)
            {
                mdlUnRegistResrc(group);
            }
        }
    }
}
