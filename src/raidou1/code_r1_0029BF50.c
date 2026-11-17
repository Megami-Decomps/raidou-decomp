#include "common.h"
#include "sBtlUnit.h"
#include "sBtlSkill.h"

s32 sdfRemoveTaskItem(struct SBTLUNIT_WORK *wk, s32 arg)
{
    struct SBTLSKILLP_TBL *tbl;

    if ((wk->flg & SBTLUNIT_FLG_SETSUMA_B_MUTEKI) != 0) {
        return 1;
    }
    tbl = sbtlGetSkilltbl(wk->skillid);
    if ((tbl->type & 0x1000) != 0) {
        return 1;
    }
    return sbtlCheckCost_SkillID(wk, wk->skillid, arg);
}
