#include "shared/sdf/sdfChip/decls.h"

/* Promote the next page on the partial list to current; an empty list clears it. */
void sdfChipAllocNextPage(sdfChipCB_tag *aCB)
{
    sdfChipPageInfo_tag *nextPage;

    nextPage = aCB->freeLink;
    if (nextPage != NULL) {
        aCB->freeLink = nextPage->next;
    }
    aCB->currentPage = nextPage;
}
