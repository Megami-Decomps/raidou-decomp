#ifndef DATCALC_H
#define DATCALC_H

#include "newdata.h"
#include "result2.h"

typedef struct {
    u8 head[6];
    fclSkillParam_t entry[24];
} datLevelUpRecord_t;

extern datLevelUpRecord_t datLevelUpTable[];

/* paratype indexes datUnitWork_t::param[6]; the retail code uses 0, 2, 3 and 5
 * and no source names them, so they stay plain integers. */
s32 datGetDefPow(datUnitWork_t *work);
s32 datAddHp(datUnitWork_t *work, s32 hp);
s32 datGetBaseParam(datUnitWork_t *work, s32 paratype);
s32 datGetParam(datUnitWork_t *work, s32 paratype);
s32 datBossRashChk(s32 eid);

extern datBossPress_t datBossPress[40];

struct sdatUnitWork_t;

s32 datAddMag(s32 id);
void datAddMaka(s32 add);
void datAddItem(s32 itemId, s32 count);
s32 datCheckMag(s32 mag);
s32 datCheckItem(s32 itemId, s32 count);
s32 datCheckMoney(s32 money);
s32 datCheckDying(struct sdatUnitWork_t *pUnit);
void datResetBadStat(struct sdatUnitWork_t *pWork, u32 badstat);
#ifdef VERSION_RAIDOU1
void datCallCalcGetBadStatus(s32 type);
s32 datCallCalcGetBadStatus2(s32 bst);
#endif

#endif /* DATCALC_H */
