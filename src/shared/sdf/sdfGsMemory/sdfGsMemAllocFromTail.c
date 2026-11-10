#include "shared/sdf/sdfGsMemory/decls.h"

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 aSize, s32 aType)
{
    sdfGsMemSlice_t *slice;
    sdfGsMemSlice_t *alloc;
    s32 reenable;

    reenable = DIntr();
    for (slice = gsMemHandleLinkTail;; slice = slice->prev) {
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
        alloc->next = slice->next;
        if (alloc->next == NULL) {
            gsMemHandleLinkTail = alloc;
        } else {
            slice->next->prev = alloc;
        }
        alloc->type = aType;
        slice->next = alloc;
        alloc->addr = slice->addr + (slice->size - aSize);
        alloc->size = aSize;
        alloc->prev = slice;
        slice->size -= aSize;
        if (reenable != 0) {
            EIntr();
        }
        return alloc;
    }
}
