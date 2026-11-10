#include "shared/event/evtEventPack/decls.h"

/* Free the three-word parameter block owned by the motion-SE task. */
void evtFreeScriptProcessData(dds3ProcessID_t *task)
{
    sdfChipFreeDirect(EE_ADDR_PTR(dds3GetProcessData(task)));
}
