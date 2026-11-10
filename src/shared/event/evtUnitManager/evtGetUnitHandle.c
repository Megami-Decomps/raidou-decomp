#include "shared/event/evtUnitManager/decls.h"

evtUnit_t *evtGetUnitHandle(s32 serial) {
    dds3Basic_t *obj = dds3SearchSceneBasic(dds3GetActiveScene(), serial, EFF_WORLD_KIND_FOLLOW_MODEL);

    if (obj != NULL) {
        return ((dds3UnitObjectBasicWorkData_t *)obj->workdata)->hUnit;
    }
    return NULL;
}
