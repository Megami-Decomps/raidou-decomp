#include "shared/model/mdlManager/decls.h"

void mdlUnRegistResrcMotion(dds3ModelResrc_t *aResrc, s32 aGroup)
{
    mdlObj_t *ctx;

    if (aResrc != NULL)
    {
        if (aResrc->playbooks[aGroup].ptr == NULL)
        {
            return;
        }
        for (ctx = aResrc->models; ctx != NULL; ctx = ctx->next)
        {
            mdlFreeMotion(ctx, aGroup);
        }
        if (aResrc->autoFreeFlag != 0)
        {
            if (aResrc->playbooks[aGroup].handle != NULL)
            {
                sdfMemFreeDirect(aResrc->playbooks[aGroup].handle);
            }
        }
        aResrc->playbooks[aGroup].ptr = NULL;
        aResrc->playbooks[aGroup].handle = NULL;
    }
}
