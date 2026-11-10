#include "shared/sdf/sdfMemory/decls.h"

void sdfMemGetInfo(sdfMemInfo_t *aInfo)
{
    sdfMemHandle_t *handle = sdfMemCB.headGuard.next;
    s32 total = 0;
    s32 freeSize = 0;
    s32 maxFreeSize = 0;
    s32 minFreeSize = 0;
    s32 handleCount = 0;
    s32 freeHandleCount = 0;
    saddr_t size;
    u16 stat;

    for (; handle->stat != SDF_MEM_STAT_END; handle = handle->next) {
        size = EE_ADDR_S(handle->next->ptr) - EE_ADDR_S(handle->ptr);
        stat = handle->stat;
        handleCount++;
        total += size;
        if (stat == SDF_MEM_STAT_FREE) {
            if (minFreeSize == 0 || size < minFreeSize) {
                minFreeSize = size;
            }
            if (maxFreeSize < size) {
                maxFreeSize = size;
            }
            freeSize += size;
            freeHandleCount++;
        }
    }
    aInfo->total = total;
    aInfo->freeSize = freeSize;
    aInfo->minFreeSize = minFreeSize;
    aInfo->maxFreeSize = maxFreeSize;
    aInfo->handleCount = handleCount;
    aInfo->freeHandleCount = freeHandleCount;
}
