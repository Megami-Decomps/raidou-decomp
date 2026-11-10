#include "shared/basic/dds3EffectObjectBasic/decls.h"

dds3Basic_t *dds3AddP2AEffectObjectBasic(const char *path, void *firstVector, saddr_t secondVectorAddress) {
    void *bill;

    bill = dds3BillboardFileLoad(1, path);
    return AddP2AEffect(bill, firstVector, secondVectorAddress);
}
