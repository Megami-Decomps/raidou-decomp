#include "shared/effect/effPart/decls.h"

void *dds3EffectPCPDataGetData(dds3EffectPCPData_t *hPCPData, u32 number)
{
    dds3EffectPCPData_Part_t *part = &hPCPData->pcpData[number];

    return (u8 *)hPCPData + part->offset;
}
