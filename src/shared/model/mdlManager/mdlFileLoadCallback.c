#include "shared/model/mdlManager/decls.h"

void mdlFileLoadCallback(filePacRequest_t *pacHandle, mdlManager_mdlFileLoadCb_t *data)
{
    data->owner = pacHandle;
    mdlPackSet(pacHandle->queueHead, data->group, data->id, data->arg);
    WaitSema(mdlManagerWork);
    mdlUnRegLoading(data->group, data->id);
    SignalSema(mdlManagerWork);
    if (data->done != NULL)
    {
        data->done(data->doneArg);
        mdlFileCbFree(data);
    }
}
