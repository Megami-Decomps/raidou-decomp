#include "shared/event/evtEventPack/decls.h"

/* Retain a ready pack and select its first entry-point record.
 * Clearing the request handle and marking completion occur on separate calls;
 * no entry-point record leaves the existing entryPoint value untouched. */
void evtCheckEventScriptLoaded(evtPackProcessData_t *state) {
    evtPackHeader_t *header;
    s32 entryPointIndex;

    if (state->pendingRequest != NULL) {
        if (pacFileCheckEnd(state->pendingRequest) != 0) {
            state->resourceAllocation = fileGetMemHandle((struct FileRequest *)state->pendingRequest);
            fileHandleFree(state->pendingRequest);
            state->pendingRequest = NULL;
            header = EE_ADDR_AS(evtPackHeader_t *, sdfMemLock(state->resourceAllocation));
            state->payload = (u8 *)header;
            state->header = header;
            state->entries = header->entries;
            for (entryPointIndex = 0; entryPointIndex < state->header->entryCount; entryPointIndex++) {
                if (state->entries[entryPointIndex].kind == EVT_PACK_ENTRY_POINT_KIND) {
                    state->entryPoint = state->payload + state->entries[entryPointIndex].dataOffset;
                    break;
                }
            }
        }
    } else {
        printf(evtLoadDoneMessage);
        state->stat = EVT_PACK_LOAD_COMPLETE;
    }
}
