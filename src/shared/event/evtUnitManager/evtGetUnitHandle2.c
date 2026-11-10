#include "shared/event/evtUnitManager/decls.h"

evtUnit_t *evtGetUnitHandle2(dds3Basic_t *unit) {
    if (unit == NULL) {
        return NULL;
    }
    return ((dds3UnitObjectBasicWorkData_t *)unit->workdata)->hUnit;
}
