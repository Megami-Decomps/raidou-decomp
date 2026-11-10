#include "shared/effect/effPCPScatter/decls.h"

dds3ScatterPolyList_tag *dds3ScatterPolyListCreate(s32 face) {
    dds3ScatterPolyList_tag *pool;
    sdfMemHandle_t *allocation;
    u32 *recordBlock;
    s32 slotCount;
    s32 recordWords;
    s32 auxWords;
    u32 allocationBytes;

    slotCount = face * EFF_SCATTER_POOL_SLOTS_PER_GROUP;
    recordWords = slotCount * EFF_SCATTER_POOL_RECORD_WORDS_PER_SLOT;
    auxWords = slotCount * EFF_SCATTER_POOL_AUX_WORDS_PER_SLOT;
    allocationBytes = (recordWords + auxWords) * EFF_SCATTER_WORD_BYTES + EFF_SCATTER_POOL_CONTROL_BYTES;
    allocation = sdfMemAllocDirect(allocationBytes);
    recordBlock = EE_ADDR_AS(u32 *, sdfMemLock(allocation));
    memset(recordBlock, 0, allocationBytes);
    pool = (dds3ScatterPolyList_tag *)(recordBlock + (recordWords + auxWords));
    pool->recordBase = (u8 *)recordBlock;
    pool->poolMode = 1;
    pool->auxRecordBase = (u8 *)(recordBlock + recordWords);
    pool->secondWordCount = auxWords;
    pool->allocation = allocation;
    pool->scale = 1.0f;
    pool->color = EFF_SCATTER_NEUTRAL_COLOR;
    pool->sharedResource = 0;
    pool->drawAsset = sdfMatNew();
    sdfMatSetConstant(pool->drawAsset, 1.0f);
    memset(effPCPScatterDrawParams, 0, EFF_SCATTER_DRAW_TEMPLATE_BYTES);
    effPCPScatterDrawParams->primitive = 0x4000;
    return pool;
}
