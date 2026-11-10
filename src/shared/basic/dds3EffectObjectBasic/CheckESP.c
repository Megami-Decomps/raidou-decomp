#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3EffectObjectBasicWorkData_t *CheckESP(dds3Basic_t *b) {
    dds3EffectObjectBasicWorkData_t *data;

    data = NULL;
    if (b == NULL) {
        return data;
    }
    if (((u8 *)&b->type)[3] != EFF_OBJ_KIND) {
        return data;
    }
    data = b->workdata;
    if (data->type != EFF_OBJ_STATE_PARAMETERS_READY) {
        data = NULL;
    }
    return data;
}
