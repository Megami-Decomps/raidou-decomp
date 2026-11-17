#include "common.h"
#include "basic.h"
#include "dds3MoverBasic.h"

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3MoverBasic.c:175, rework, same function 1.00 (words 3/3, calls 1.00, shape 1.00) */
dds3PathBasicControl_t *dds3MoverBasicGetPathControl(dds3Basic_t *pMoverBasic)
{
    dds3MoverBasicWorkData_t *work = pMoverBasic->workdata;

    return work->pPathControl;
}

/* body: matched in smt3 @ 9401f79e: src/smt3/basic/dds3MoverBasic.c:182, match, same function 1.00 (words 5/5, calls 1.00, shape 1.00) */
s32 dds3MoverBasicUpdate(dds3Basic_t *pBasic)
{
    return MoverBasicMainprocuder(pBasic);
}

u32 dds3ObjectBaseSetControlBasicSub(u32 type)
{
    u32 itemType = 0;

    switch (type) {
    case BASIC_TYPE_MOVER:
        itemType = 1;
        break;
    case BASIC_TYPE_STATICMODEL:
    case BASIC_TYPE_EFFECT:
    case BASIC_TYPE_EVENT:
        itemType = 3;
        break;
    case BASIC_TYPE_AREA:
        itemType = 4;
        break;
    }
    return itemType;
}
