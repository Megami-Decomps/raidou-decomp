#include "shared/event/evtStage/decls.h"

void evtPathDir(dds3Basic_t *basic, u32 dir) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicControlAlterMode(slotData, dir);
    }
}
