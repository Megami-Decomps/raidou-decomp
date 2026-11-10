#include "shared/basic/dds3LightObjectBasic/decls.h"

void dds3UnlinkLightObjectFromUnitObject(dds3Basic_t *unitbasic, s32 frame)
{
    dds3UnitObjectBasicWorkData_t *work = unitbasic->workdata;

    evtUnlinkLightObjectBasic(work->hUnit, frame);
}
