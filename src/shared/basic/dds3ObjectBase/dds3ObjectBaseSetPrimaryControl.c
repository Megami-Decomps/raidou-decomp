#include "shared/basic/dds3ObjectBase/decls.h"

s32 dds3ObjectBaseSetPrimaryControl(dds3Basic_t *basic, dds3BasicControlProc_t func)
{
    dds3Basic_t *mover;

    mover = dds3ObjectBaseGetControlBasic(basic, 1);
    if (mover == NULL) {
        return 0;
    }
    dds3MoverBasicSetCallBackPrimaryControl(mover, func);
    return 1;
}
