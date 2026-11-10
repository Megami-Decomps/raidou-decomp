#include "shared/effect/effPCPMisc/decls.h"

effPCPRadiateThunder_tag *effPCPRadiateThunderCopy(effPCPRadiateThunder_tag *work) {
    effPCPRadiateThunder_tag *groupSet = effPCPRadiateThunderCreate(&work->param, 0);
    u32 groupIndex;
    u32 groupByteOffset;
    u32 entryCount;
    u32 allocationBytes;
    u32 groupByteWidth;
    u8 *activeGroupFlags;
    u32 entryIndex;

    if (work->phAttach != 0) {
        entryCount = work->param.number;
        allocationBytes = entryCount * 16;
        groupByteWidth = entryCount * 4;
        groupIndex = 0;
        activeGroupFlags = work->param.activeGroups;
        groupByteOffset = 0;
        groupSet->hAttachMem = sdfMemAllocDirect(allocationBytes);
        groupSet->phAttach = EE_ADDR_AS(effPartHandle_t **, sdfMemLock(groupSet->hAttachMem));
        memset(groupSet->phAttach, 0, allocationBytes);
        for (; groupIndex < 4; groupIndex++) {
            effPartHandle_t **destinationHandles = (effPartHandle_t **)((u8 *)groupSet->phAttach + groupByteOffset);

            if (*activeGroupFlags != 0) {
                effPartHandle_t *sourceHandle = *(effPartHandle_t **)(groupByteOffset + EE_ADDR(work->phAttach));
                for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
                    *destinationHandles++ = dds3EffectPartCopy(sourceHandle);
                }
            }
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    return groupSet;
}
