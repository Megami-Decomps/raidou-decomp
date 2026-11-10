#include "shared/model/mdlManager/decls.h"

void TbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.texList = sdfTexSetTB(fileGetLoadPtr(fileHandle));
    sdfMemFreeDirect(fileGetMemHandle(fileHandle));
    fileHandleFree(fileHandle);
}
