#include "shared/sdf/sdfGsMemory/decls.h"

void sdfGsMemInit(void)
{
    sdfGsMemSlice_t *slice;

    slice = sdfChipAllocDirect(sizeof(sdfGsMemSlice_t));
    slice->size = 0x100000;
    slice->prev = NULL;
    slice->next = NULL;
    slice->type = 0;
    slice->addr = 0;
    gsMemHandleLinkHead = slice;
    gsMemHandleLinkTail = slice;
    sdfDelayedResrcInitCB(&gsMemDelayedResrcCB, sdfGsMemFree);
}
