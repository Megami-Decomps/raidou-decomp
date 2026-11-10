#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3Basic_t *dds3AddTMXEffectObjectBasic3(dds3Basic_t *ebasic, void *pos, saddr_t rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCopy((struct billHandle_t *)((dds3EffectObjectBasicWorkData_t *)ebasic->workdata)->handle);
    return AddTMXEffect(bill, pos, rot);
}
