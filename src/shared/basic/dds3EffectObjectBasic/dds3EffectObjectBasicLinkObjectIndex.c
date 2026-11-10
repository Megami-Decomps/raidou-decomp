#include "shared/basic/dds3EffectObjectBasic/decls.h"

s32 dds3EffectObjectBasicLinkObjectIndex(dds3Basic_t *basic, dds3Basic_t *target, s32 index) {
    dds3EffectObjectBasicWorkData_t *data;

    if (dds3EffectObjectBasicLinkObject(basic, target) == 0) {
        return 1;
    }
    data = basic->workdata;
    data->linkindex = index;
    data->stat |= EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
    return 1;
}
