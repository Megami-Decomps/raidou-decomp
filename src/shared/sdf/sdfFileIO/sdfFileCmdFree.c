#include "shared/sdf/sdfFileIO/decls.h"

s32 sdfFileCmdFree(sdfFileCmd_t *aHandle)
{
    if (aHandle->stat < SDF_FILE_CMD_STAT_ERROR) {
        if (aHandle->stat >= SDF_FILE_CMD_STAT_SUSPEND) {
            sdfFileCmdUnlink(aHandle);
            return 0;
        }
    }
    return -1;
}
