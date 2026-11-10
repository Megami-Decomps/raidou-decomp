#include "shared/basic/dds3LightObjectBasic/decls.h"

void dds3LinkLightObjectToUnitObject(dds3Basic_t *lightbasic, dds3Basic_t *unitbasic, s32 frame)
{
    dds3UnitObjectBasicWorkData_t *work = unitbasic->workdata;

    evtLinkLightObjectBasic(work->hUnit, lightbasic, frame);
}
