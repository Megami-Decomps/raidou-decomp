#include "shared/sdf/sdfGsMemory/decls.h"

sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 aSize, s32 aType)
{
    sdfGsMemSlice_t *slice;
    sdfGsMemSlice_t *alloc;
    s32 reenable;

    reenable = DIntr();
    for (slice = gsMemHandleLinkHead;; slice = slice->next) {
        if (slice == NULL) {
            if (reenable != 0) {
                EIntr();
            }
            if (sdfGsErrorHandler != NULL) {
                sdfGsErrorHandler(aSize, aType);
            }
        }
        if (slice->type != 0 || slice->size < aSize) {
            continue;
        }
        if (slice->size == aSize) {
            slice->type = aType;
            if (reenable != 0) {
                EIntr();
            }
            return slice;
        }
        alloc = sdfChipAllocDirect(sizeof(*alloc));
        alloc->prev = slice->prev;
        if (alloc->prev == NULL) {
            gsMemHandleLinkHead = alloc;
        } else {
            slice->prev->next = alloc;
        }
        alloc->type = aType;
        slice->prev = alloc;
        alloc->addr = slice->addr;
        alloc->size = aSize;
        alloc->next = slice;
        slice->addr += aSize;
        slice->size -= aSize;
        if (reenable != 0) {
            EIntr();
        }
        return alloc;
    }
}
