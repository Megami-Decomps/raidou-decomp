#include "shared/basic/dds3EffectObjectBasic/decls.h"

void *dds3AddMG12EffectObjectBasic(s32 type, void *file) {
    u32 resolvedId;
    void *resourceHandle;
    void *created;

    resolvedId = 0;
    resourceHandle = sdfDevLoadFileEx(file, &resolvedId, 0);
    created = AddMG12Effect(type, resolvedId);
    sdfMemFreeDirect(resourceHandle);
    return created;
}
