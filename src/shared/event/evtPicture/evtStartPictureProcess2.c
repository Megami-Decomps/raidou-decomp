#include "shared/event/evtPicture/decls.h"

dds3ProcessID_t *evtStartPictureProcess2(u32 priority, const char *texdata)
{
    evtPictureProcessData_t *work = InitPictureProcessData();

    LoadTexture(work, texdata);
    return dds3StartProcess(evtPictureProcName, priority, 1, 1, evtPictureProcess1, evtPictureShutdown, work);
}
