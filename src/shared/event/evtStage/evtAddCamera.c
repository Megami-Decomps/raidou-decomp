#include "shared/event/evtStage/decls.h"

dds3Basic_t *evtAddCamera(f32 *eye, f32 *rot) {
    dds3Basic_t *obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), eye, rot);

    if (obj == 0) {
        return obj;
    }
    dds3GetRadiusPoseSet(obj, 1.0f);
    return obj;
}
