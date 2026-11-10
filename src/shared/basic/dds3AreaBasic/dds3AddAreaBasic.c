#include "shared/basic/dds3AreaBasic/decls.h"

dds3Basic_t *dds3AddAreaBasic(u32 serialID, char *pName)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_AREA);

    basic->serialID = serialID;
    basic->name = pName;
    return basic;
}
