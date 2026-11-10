#include "shared/sdf/sdfMemory/decls.h"

/* sdfMemAllocDirect without the full-heap callback: returns NULL instead. */
sdfMemHandle_t *sdfMemAlloc2Direct(s32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    s32 want = (aSize + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    s32 intrEnabled;
    sdfMemHandle_t *handle;

    intrEnabled = DIntr();
    for (handle = cb->headGuard.next;; handle = handle->next) {
        if (handle->stat != SDF_MEM_STAT_FREE) {
            if (handle->stat == SDF_MEM_STAT_END) {
                if (intrEnabled != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            saddr_t have = EE_ADDR_S(handle->next->ptr) - EE_ADDR_S(handle->ptr);

            if (have >= want) {
                if (want < have) {
                    sdfMemHandle_t *split = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    split->prev = handle;
                    split->ptr = EE_ADDR_AS(void *, (EE_ADDR_S(handle->ptr) + want));
                    split->stat = SDF_MEM_STAT_FREE;
                    split->next = handle->next;
                    split->mlock = 0;
                    handle->next->prev = split;
                    handle->next = split;
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
