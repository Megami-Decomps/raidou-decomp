#include "shared/effect/effPCPMisc/decls.h"

effPCPRadiateThunder_tag *effPCPRadiateThunderCreate(const effPCPRadiateThunderParam_t *pParam, void *pData) {
    effPCPRadiateThunderParam_t *header = (effPCPRadiateThunderParam_t *)pParam;
    void **sourceResources = pData;
    u32 entryCount = header->number;
    sdfMemHandle_t *workResource = sdfMemAllocDirect(entryCount * 0x18 + 0x180);
    effPCPRadiateThunder_tag *groupSet = EE_ADDR_AS(void *, sdfMemLock(workResource));
    effPCPRadiateThunderPart_tag *groupEntryCursor;
    u32 index;
    u32 duplicateIndex;

    groupSet->param = *header;
    groupSet->scale = 1.0f;
    groupSet->abgr = 0x80808080;
    groupSet->hMem = workResource;
    groupSet->height = header->ThunderParam.start[1];
    groupEntryCursor = (effPCPRadiateThunderPart_tag *)(groupSet + 1);
    groupSet->pPart = groupEntryCursor;
    groupSet->phAttach = 0;
    if (sourceResources != NULL) {
        u32 allocationBytes = entryCount * 16;
        void **sourceResourceCursor = sourceResources;
        u32 groupByteOffset;
        u32 groupByteWidth;
        u8 *activeGroupFlags;

        index = 0;
        groupSet->hAttachMem = sdfMemAllocDirect(allocationBytes);
        activeGroupFlags = header->activeGroups;
        groupByteOffset = 0;
        groupByteWidth = entryCount * 4;
        groupSet->phAttach = EE_ADDR_AS(effPartHandle_t **, sdfMemLock(groupSet->hAttachMem));
        memset(groupSet->phAttach, 0, allocationBytes);
        for (; index < 4; index++) {
            effPartHandle_t **destinationHandles = (effPartHandle_t **)((u8 *)groupSet->phAttach + groupByteOffset);

            if (*activeGroupFlags != 0) {
                effPartHandle_t *firstCreatedHandle;

                if (index < 2) {
                    *destinationHandles = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EFFECT_NODE, *sourceResourceCursor);
                } else {
                    *destinationHandles = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, *sourceResourceCursor);
                }
                firstCreatedHandle = *destinationHandles;
                for (duplicateIndex = 1; duplicateIndex < entryCount; duplicateIndex++) {
                    destinationHandles++;
                    *destinationHandles = dds3EffectPartCopy(firstCreatedHandle);
                }
            }
            sourceResourceCursor++;
            activeGroupFlags++;
            groupByteOffset += groupByteWidth;
        }
    }
    for (index = 0; index < entryCount; index++) {
        groupEntryCursor->pThunderPoly = effPCP_ThunderPolyCreate(&header->ThunderParam);
        groupEntryCursor->frame = 0;
        groupEntryCursor++;
    }
    return groupSet;
}
