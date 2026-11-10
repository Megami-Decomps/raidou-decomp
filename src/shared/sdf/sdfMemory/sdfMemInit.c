#include "shared/sdf/sdfMemory/decls.h"

/* Align the usable span to 128 bytes and link one free handle between the guards. */
void sdfMemInit(u32 aSize)
{
    sdfMemHandleCB_tag *cb = &sdfMemCB;
    sdfMemHandle_t *freeHandle;
    addr_t alignedTop;
    addr_t alignedEnd;

    cb->allocPtr = malloc(aSize);
    cb->allocSize = aSize;
    freeHandle = sdfChipAllocDirect(sizeof(sdfMemHandle_t));
    alignedTop = (EE_ADDR(cb->allocPtr) + SDF_MEM_ALIGN_MASK) & ~SDF_MEM_ALIGN_MASK;
    alignedEnd = (EE_ADDR(cb->allocPtr) + aSize) & ~SDF_MEM_ALIGN_MASK;
    cb->headGuard.prev = NULL;
    cb->headGuard.next = freeHandle;
    cb->headGuard.stat = SDF_MEM_STAT_END;
    cb->headGuard.mlock = -1;
    cb->tailGuard.stat = SDF_MEM_STAT_END;
    cb->tailGuard.mlock = -1;
    cb->tailGuard.ptr = EE_ADDR_AS(void *, alignedEnd);
    cb->tailGuard.prev = freeHandle;
    cb->tailGuard.next = NULL;
    cb->headGuard.ptr = EE_ADDR_AS(void *, alignedTop);
    freeHandle->prev = &cb->headGuard;
    freeHandle->stat = SDF_MEM_STAT_FREE;
    freeHandle->next = &cb->tailGuard;
    freeHandle->ptr = EE_ADDR_AS(void *, alignedTop);
    freeHandle->mlock = 0;
    sdfMemFullCallback = NULL;
    sdfDelayedResrcInitCB(&sdfMemDelayedResrcCB, (sdfDelayedResrcFreeFunc)sdfMemFreeDirect);
}
