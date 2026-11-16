#include "common.h"
#include "sdfChip.h"
#include "sdk_libc.h"
#include "nbDebugProcess.h"

effList_t *InitDebugProcessData(s32 mode)
{
    effList_t *list;

    list = sdfChipAllocDirect(sizeof(effList_t));
    memset(list, 0, sizeof(effList_t));
    list->mode = mode;
    list->count = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}
