#include "shared/event/evtPicture/decls.h"

void evtKillPictureProcess(dds3ProcessID_t *id)
{
    dds3KillProcess(id, 1);
}
