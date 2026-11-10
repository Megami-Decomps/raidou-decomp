#include "shared/model/mdlManager/decls.h"

s32 mdlFileLoad(s32 major, s32 minor, s32 regFlags, const char *filename, void (*callback)(u32),
                u32 parm)
{
    mdlManager_mdlFileLoadCb_t *job;
    filePacRequest_t *request;

    job = sdfChipAllocZeroDirect(sizeof(mdlManager_mdlFileLoadCb_t));
    job->group = major;
    job->id = minor;
    job->arg = regFlags;
    job->doneArg = parm;
    job->done = callback;
    request = pacLoadEx(filename, 0, 0, mdlFileLoadCallback, job);
    job->owner = request;
    if (callback == NULL)
    {
        pacSync(request);
        mdlFileCbFree(job);
    }
    return 0;
}
