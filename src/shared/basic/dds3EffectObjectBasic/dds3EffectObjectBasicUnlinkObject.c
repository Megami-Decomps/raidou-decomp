#include "shared/basic/dds3EffectObjectBasic/decls.h"

void dds3EffectObjectBasicUnlinkObject(dds3Basic_t *basic)
{
    dds3EffectObjectBasicWorkData_t *work = basic->workdata;

    work->linkindex = 0;
    work->linkbasic = NULL;
    work->linkbasictype = 0;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_LINK;
    work->stat &= ~EFF_OBJ_FLAG_OWNER_BILL_ENTRY;
}
