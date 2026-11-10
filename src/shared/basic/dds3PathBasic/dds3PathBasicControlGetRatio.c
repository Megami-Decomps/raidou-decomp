#include "shared/basic/dds3PathBasic/decls.h"

f32 dds3PathBasicControlGetRatio(dds3PathBasicControl_t *pControl)
{
    return pControl->frame / pControl->frameMax;
}
