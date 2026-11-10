#include "shared/sdf/sdfFileIO/decls.h"

s32 sdfFileCmdClearError(sdfFileCmd_t *aHandle)
{
    if (aHandle->stat != 9) {
        return -1;
    }
    aHandle->error = 0;
    aHandle->stat = 7;
    return 0;
}
