#include "shared/model/mdlManager/decls.h"

void TbMbAbLoadEnd(mdlManager_mdlTbMbAbLoadCb_t *tmaCb)
{
    mdlRegistModelSet(tmaCb->group, tmaCb->id, tmaCb->options, &tmaCb->payload);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(tmaCb->group, tmaCb->id);
    SignalSema(mdlManagerWork);
    sdfChipFreeDirect(tmaCb);
}
