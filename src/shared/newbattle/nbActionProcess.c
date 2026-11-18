#include "common.h"
#include "sBtlItem.h"
#include "sdfMemory.h"
#include "sdfChip.h"

s32 sbtlDeleteItemList(SBTLITEM_LISTWORK *lwk) {
    if (lwk->handle != NULL) {
        sdfMemFreeDirect(lwk->handle);
        lwk->handle = NULL;
    }
    sdfChipFreeDirect(lwk);
    return 1;
}
