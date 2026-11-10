#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3AddP2AEffectObjectBasic2(u32 effdata, void *pos, s32 rot) {
    struct billHandle_t *bill;

    bill = dds3BillboardCreate(1, effdata);
    AddP2AEffect(bill, pos, rot);
}
