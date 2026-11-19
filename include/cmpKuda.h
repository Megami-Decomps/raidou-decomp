#ifndef CMPKUDA_H
#define CMPKUDA_H

#include "common.h"
#include "enums.h"
#include "sdatUnit.h"

s32 cmpUpdateSkillHD(void);
void cmpUpdateUseItemEnebleDecide(void);
#ifdef VERSION_RAIDOU2
void cmpSetFilterEx(sdatUnitWork_t *psUse, ecmpKudaSelectMode eSelMode, ecmpKudaCMU iSelectAttrId,
                    ecmpKudaCMU iDecideAttrId);
void cmpSetFilter(sdatUnitWork_t *psUse, ecmpKudaSelectMode eSelMode, ecmpKudaCMU iAttrId);
void cmpSetFilterBadStatus(BSTF iBadStatus);
void cmpClearFilter(void);
#endif

#endif
