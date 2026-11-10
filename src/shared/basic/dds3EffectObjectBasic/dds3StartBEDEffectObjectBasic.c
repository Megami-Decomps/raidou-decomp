#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3StartBEDEffectObjectBasic(dds3Basic_t *basic, u32 cutno) {
    dds3EffectObjectBasicWorkData_t *data;

    data = basic->workdata;
    if (data->type != EFF_OBJ_STATE_EVENT_NODE) {
        return;
    }
    if (data->para == NULL) {
        return;
    }
    if (data->handle == NULL) {
        return;
    }
    if (data->cuth != NULL) {
        effEveBEDCutDelete(data->cuth);
        data->cuth = NULL;
    }
    data->cuth = effEveBEDCutCreate(data->handle, (u16)(cutno & EFF_OBJ_ENTRY_ID_MASK), data->para);
}
