#include "shared/model/mdlManager/decls.h"

void MbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.modelDefHandle = fileGetMemHandle(fileHandle);
    parm->payload.modelDefPtr = sdfFixupModelBinary(fileGetLoadPtr(fileHandle));
    fileHandleFree(fileHandle);
    if (parm->deferred == 0)
    {
        TbMbAbLoadEnd(parm);
    }
}
