#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicGetObjectIndex(dds3Basic_t *basic) {
    dds3EffectObjectBasicWorkData_t *data;
    dds3Basic_t *owner;
    effOwnerBillPayload_t *bill;

    data = basic->workdata;
    if (data->stat & EFF_OBJ_FLAG_OWNER_BILL_ENTRY) {
        owner = data->linkbasic;
        if (((u8 *)&owner->type)[3] == EFF_OBJ_OWNER_BILL_KIND) {
            bill = (effOwnerBillPayload_t *)((dds3EffectObjectBasicWorkData_t *)owner->workdata)->handle;
            if (bill->state != 0) {
                return;
            }
            sdfModelGetMarkPosMatrix(((effOwnerBillParam_t *)bill->params)->model, data->linkindex);
        }
    }
}
