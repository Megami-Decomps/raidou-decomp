#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicSetAnmeID(dds3Basic_t *basic, u32 animeID)
{
    dds3BillboardSetAnimNumber(((dds3EffectObjectBasicWorkData_t *)basic->workdata)->handle, animeID);
}
