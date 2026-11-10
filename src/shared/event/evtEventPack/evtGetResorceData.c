#include "shared/event/evtEventPack/decls.h"

void *evtGetResorceData(u32 eventno, s32 rid) {
    s32 i;
    evtPackProcessData_t *data;
    dds3ProcessID_t *task;

    task = evtSearchEventPack(eventno);
    if (task != 0) {
        data = (evtPackProcessData_t *)dds3GetProcessData(task);
        if (data->stat != 2) {
            return 0;
        }
        for (i = 0; i < data->header->entryCount; i++) {
            if (data->entries[i].secondaryResourceId == rid) {
                return data->payload + data->entries[i].dataOffset;
            }
        }
        return 0;
    }
    return NULL;
}
