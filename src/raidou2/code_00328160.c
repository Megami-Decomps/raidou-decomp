#include "common.h"
#include "sdfChip.h"
#include "code_r1_002C7ED0.h"

void effDestroyEffectList(effList_t *list)
{
    sdfChipFreeDirect(list);
}

INCLUDE_ASM(const s32, "code_00328160", mnuGetValueRecordOwner);
