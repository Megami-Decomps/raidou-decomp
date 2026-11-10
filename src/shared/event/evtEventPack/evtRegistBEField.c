#include "shared/event/evtEventPack/decls.h"

/* Start a field BE from the task's resource table when all four payloads exist. */
s32 evtRegistBEField(s32 eventno, s32 rid) {
    evtPackProcessData_t *packState;
    evtPackEntry_t *entry;
    s32 entryCount;
    s32 remainingEntryCount;
    s32 *areaRoomIds;
    s32 *resource0Data;
    s32 *resource1Data;
    const sdfTextureOffsetListHeader_t *textureOffsets;

    packState = evtGetPackProcessData(eventno);
    areaRoomIds = NULL;
    resource1Data = NULL;
    resource0Data = NULL;
    textureOffsets = NULL;
    entryCount = packState->header->entryCount;
    if (entryCount > 0) {
        entry = packState->entries;
        remainingEntryCount = entryCount;
        do {
            if (entry->resourceId == rid) {
                switch (entry->kind) {
                case 2:
                    resource0Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 3:
                    resource1Data = (s32 *)(packState->payload + entry->dataOffset);
                    break;
                case 4:
                    textureOffsets = (const sdfTextureOffsetListHeader_t *)(packState->payload + entry->dataOffset);
                    break;
                }
            }
            if (entry->secondaryResourceId == rid && entry->kind == 1) {
                areaRoomIds = (s32 *)(packState->payload + entry->dataOffset);
            }
            entry++;
        } while (--remainingEntryCount != 0);
    }

    if (areaRoomIds != NULL && resource0Data != NULL && resource1Data != NULL && textureOffsets != NULL) {
        evtLoadArea_FromMemory(areaRoomIds[0], areaRoomIds[1], EE_ADDR_S(resource0Data), EE_ADDR_S(resource1Data), textureOffsets, 0);
        fldFileAddrResetMode(1);
        dds3DrawFlag |= 0x02000000;
        return 1;
    }
    return 0;
}
