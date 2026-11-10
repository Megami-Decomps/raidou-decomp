#include "shared/basic/dds3EffectObjectBasic/decls.h"

void *dds3AddBEDEffectObjectBasic(void *file, void *pos, void *rot) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddBEDEffect(EE_ADDR_AS(effBTL_t *, resolvedId), pos, rot);
    sdfMemFreeDirect(resourceHandle);
    return created;
}
