#include "shared/basic/dds3MoverBasic/decls.h"

dds3Basic_t *dds3AddMoverBasic(dds3Basic_t *pObjectBasic)
{
    dds3Basic_t *basic;
    dds3MoverBasicWorkData_t *work;

    basic = dds3AddSceneBasicData(BASIC_TYPE_MOVER);
    work = basic->workdata;
    basic->serialID = dds3AllocObjectId();
    work->pObjectBasic = pObjectBasic;
    basic->name = dds3MoverNameTable[dds3MoverCount];
    dds3MoverCount++;
    dds3MoverCount %= 10;
    return basic;
}
