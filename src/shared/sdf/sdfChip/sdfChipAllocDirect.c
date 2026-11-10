#include "shared/sdf/sdfChip/decls.h"

/* Allocate from the size class that covers the request; classes are powers of
 * two from 16 bytes up, and each page hands out from its own chip list. */
void *sdfChipAllocDirect(s32 aSize)
{
    s32 index = 0;
    sdfChipCB_tag *cb;
    sdfChip_t *chip;
    s32 intrEnabled;

    if (aSize > 16) {
        MMI_PLZCW(index, aSize - 1);
        index = 27 - (index & 0xFF);
    }
    cb = &sdfChipCB[index];
    intrEnabled = DIntr();
    if (cb->currentPage == NULL) {
        chip = sdfChipAllocNewPage(cb);
    } else {
        chip = ((sdfChip_t *(*)(sdfChipCB_tag *, sdfChipPageInfo_tag *))cb->currentPage->alloc)(
            cb, cb->currentPage);
    }
    if (intrEnabled != 0) {
        EIntr();
    }
    return chip;
}
