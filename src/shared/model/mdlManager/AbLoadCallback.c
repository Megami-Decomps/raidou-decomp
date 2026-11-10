#include "shared/model/mdlManager/decls.h"

void AbLoadCallback(fileRequest_t *fileHandle, mdlManager_mdlTbMbAbLoadCb_t *parm)
{
    parm->payload.playbookHandle = fileGetMemHandle(fileHandle);
    parm->payload.playbookPtr = sdfFixupMotionBinary(fileGetLoadPtr(fileHandle));
    fileHandleFree(fileHandle);
    TbMbAbLoadEnd(parm);
}
