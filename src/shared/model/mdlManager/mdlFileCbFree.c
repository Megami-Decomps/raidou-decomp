#include "shared/model/mdlManager/decls.h"

void mdlFileCbFree(mdlManager_mdlFileLoadCb_t *cb)
{
    pacFileHandleFree_(cb->owner);
    sdfChipFreeDirect(cb);
}
