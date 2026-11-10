#include "shared/sdf/sdfMemory/decls.h"

/* Resize the span a handle owns, keeping the handle itself. Growing absorbs or
 * trims the free neighbour when it can, and otherwise moves the data and swaps
 * the two handles' contents so the caller's handle survives. */
void sdfMemReallocRecord(sdfMemHandle_t *aHandle, s32 aSize)
{
    sdfMemHandle_t *next;
    sdfMemHandle_t *replacement;
    saddr_t ptr;
    s32 intrEnabled;
    saddr_t currentSize;
    saddr_t available;

    intrEnabled = DIntr();
    next = aHandle->next;
    ptr = EE_ADDR_S(aHandle->ptr);
    currentSize = EE_ADDR_S(next->ptr) - ptr;
    aSize = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    if (aSize == currentSize) {
        if (intrEnabled != 0) {
            EIntr();
        }
        return;
    }
    if (currentSize < aSize) {
        available = EE_ADDR_S(next->next->ptr) - EE_ADDR_S(next->ptr) + currentSize;
        if (next->stat == SDF_MEM_STAT_FREE && available >= aSize) {
            if (available == aSize) {
                next->next->prev = aHandle;
                aHandle->next = next->next;
                sdfChipFreeDirect(next);
            } else {
                next->ptr = EE_ADDR_AS(void *, (ptr + aSize));
            }
        } else {
            saddr_t oldPtr;

            replacement = sdfMemAllocDirect(aSize);
            memcpy(replacement->ptr, aHandle->ptr, currentSize);
            ptr = EE_ADDR_S(aHandle->prev);
            oldPtr = EE_ADDR_S(aHandle->ptr);
            aHandle->prev = replacement->prev;
            aHandle->ptr = replacement->ptr;
            aHandle->next = replacement->next;
            replacement->prev = EE_ADDR_AS(sdfMemHandle_t *, ptr);
            replacement->next = next;
            replacement->ptr = EE_ADDR_AS(void *, oldPtr);
            aHandle->prev->next = aHandle;
            aHandle->next->prev = aHandle;
            replacement->prev->next = replacement;
            replacement->next->prev = replacement;
            sdfMemFreeDirect(replacement);
        }
    } else if (next->stat == SDF_MEM_STAT_FREE) {
        next->ptr = EE_ADDR_AS(void *, (ptr + aSize));
    } else {
        ptr = EE_ADDR_S(sdfChipAllocDirect(sizeof(sdfMemHandle_t)));
        (EE_ADDR_AS(sdfMemHandle_t *, ptr))->prev = aHandle;
        (EE_ADDR_AS(sdfMemHandle_t *, ptr))->next = next;
        next->prev = EE_ADDR_AS(sdfMemHandle_t *, ptr);
        (EE_ADDR_AS(sdfMemHandle_t *, ptr))->stat = SDF_MEM_STAT_FREE;
        aHandle->next = EE_ADDR_AS(sdfMemHandle_t *, ptr);
        (EE_ADDR_AS(sdfMemHandle_t *, ptr))->mlock = 0;
        (EE_ADDR_AS(sdfMemHandle_t *, ptr))->ptr = EE_ADDR_AS(void *, (EE_ADDR_S(aHandle->ptr) + aSize));
    }
    if (intrEnabled != 0) {
        EIntr();
    }
}
