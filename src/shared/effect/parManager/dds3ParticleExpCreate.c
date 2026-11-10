#include "shared/effect/parManager/decls.h"

dds3Particle_Basic_t *dds3ParticleExpCreate(s32 kind, parKindResource_t *resource) {
    dds3Particle_Basic_t *object = (dds3Particle_Basic_t *)((u8 *)resource + resource->offset + 0x10);
    billHandle_t *billboard;

    if (resource->type != 3 || resource->offset != 0) {
        object->numberO = PAR_BILLBOARD_CLONE_FROM_RESOURCE;
        object = dds3ParticleFuncTable[kind].func(object);
        billboard = dds3BillboardCreate(resource->type, EE_ADDR((resource + 1)));
        dds3BillboardSetScale(billboard, object->billboardScale, object->billboardScaleY);
        dds3BillboardSetMode(billboard, object->billboardMode);
        dds3BillboardSetAnimColorAdvMode(billboard);
        object->hBillboard = billboard;
    } else {
        object = dds3ParticleFuncTable[kind].func(object);
    }
    object->type = kind;
    return object;
}
