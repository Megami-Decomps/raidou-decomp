#include "shared/event/evtStage/decls.h"

void evtPathStart(dds3Basic_t *basic)
{
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        dds3PathBasicPauseOff(slotData);
    }
}
