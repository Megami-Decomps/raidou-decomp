#include "shared/sdf/sdfMemory/decls.h"

/* First-fit from the high end: the handle keeps its tail and the remainder
 * becomes a new free handle in front of it. */
sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    s32 want = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    s32 intrEnabled;
    sdfMemHandle_t *handle;

    intrEnabled = DIntr();
    for (handle = cb->tailGuard.prev;; handle = handle->prev) {
        if (handle->stat != SDF_MEM_STAT_FREE) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
        } else {
            saddr_t have = EE_ADDR_S(handle->next->ptr) - EE_ADDR_S(handle->ptr);

            if (have >= want) {
                if (want < have) {
                    sdfMemHandle_t *split = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    split->prev = handle;
                    split->next = handle->next;
                    split->ptr = EE_ADDR_AS(void *, (EE_ADDR_S(handle->ptr) + (have - want)));
                    split->stat = SDF_MEM_STAT_FREE;
                    split->mlock = 0;
                    handle->next->prev = split;
                    handle->next = split;
                    handle = split;
                }
                handle->stat = SDF_MEM_STAT_USED;
                if (intrEnabled != 0) {
                    EIntr();
                }
                return handle;
            }
        }
    }
}
