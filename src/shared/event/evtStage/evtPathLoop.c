#include "shared/event/evtStage/decls.h"

void evtPathLoop(dds3Basic_t *basic, s32 sw) {
    dds3PathBasicControl_t *data = dds3ObjectBaseGetPathControl(basic);

    if (data == NULL) {
        return;
    }
    if (sw != 0) {
        dds3PathBasicLoopOn(data);
    } else {
        dds3PathBasicLoopOff(data);
    }
}
