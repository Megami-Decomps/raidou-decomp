#include "common.h"
#include "dds3EventBasic.h"
#include "dds3SceneBasic.h"

dds3Basic_t *dds3AddEventBasic(u32 serialID, void *pData, const char *pName)
{
    dds3Basic_t *basic = dds3AddSceneBasicData(BASIC_TYPE_EVENT);

    basic->workdata = pData;
    basic->serialID = serialID;
    basic->name = pName;
    return basic;
}
