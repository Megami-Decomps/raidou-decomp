#include "shared/event/evtPicture/decls.h"

void evtPictureDrawOn(dds3ProcessID_t *id)
{
    evtPictureProcessData_t *work = dds3GetProcessData(id);
    work->flag |= 1;
}
