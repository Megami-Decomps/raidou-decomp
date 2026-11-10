#include "shared/event/evtStage/decls.h"

/* Scale the object's retained path time within the clamped multiplier range. */
void evtPathRate(dds3Basic_t *basic, f32 rate) {
    dds3PathBasicControl_t *slotData;

    slotData = dds3ObjectBaseGetPathControl(basic);
    if (slotData != 0) {
        if (rate < 0.0f) {
            rate = 0.0f;
        }
        if (rate > 1.0f) {
            rate = 1.0f;
        }
        dds3PathBasicControlSetRatio(slotData, rate);
    }
}
