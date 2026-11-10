#include "shared/effect/billManager/decls.h"

billHandle_t *dds3BillboardCopy(const billHandle_t *hBillboard) {
    billHandle_t *copy;
    dds3BillResource_tag *data;
    u16 sourceKind;
    void (*sourceCallback)(struct billHandle_t *);

    if (hBillboard->type == 1) {
        copy = dds3Billboard_P2ACopy(hBillboard);
        dds3Billboard_BasicInit(copy);
        copy->type = hBillboard->type;
        copy->callback = hBillboard->callback;
    } else {
        copy = dds3Billboard_NormalCreate(NULL);
        dds3Billboard_BasicInit(copy);
        sourceKind = hBillboard->type;
        data = hBillboard->u.child;
        sourceCallback = hBillboard->callback;
        copy->type = sourceKind;
        copy->callback = sourceCallback;
        data->count = data->count + 1;
        copy->u.child = data;
    }
    return copy;
}
