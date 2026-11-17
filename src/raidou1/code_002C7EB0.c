#include "common.h"
#include "sdfChip.h"
#include "code_r1_002C7ED0.h"

void effDestroyEffectList(effList_t *list)
{
    sdfChipFreeDirect(list);
}

u32 mnuGetValueRecordOwner(const effList_t *list)
{
    return list->mode;
}
