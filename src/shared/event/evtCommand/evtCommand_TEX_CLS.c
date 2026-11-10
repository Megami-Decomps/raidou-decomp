#include "shared/event/evtCommand/decls.h"

u32 evtCommand_TEX_CLS(void)
{
    void *id;

    id = EE_ADDR_AS(void *, scrGetIntPara(0));
    if (dds3ExistProcess(id) != 0) {
        evtKillPictureProcess(id);
    }
    return 1;
}
