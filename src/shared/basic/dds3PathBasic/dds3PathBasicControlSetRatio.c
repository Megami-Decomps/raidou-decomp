#include "shared/basic/dds3PathBasic/decls.h"

void dds3PathBasicControlSetRatio(dds3PathBasicControl_t *pControl, f32 ratio)
{
    pControl->frame = ratio * pControl->frameMax;
}
