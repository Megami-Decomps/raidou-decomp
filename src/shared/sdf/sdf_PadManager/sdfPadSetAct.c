#include "shared/sdf/sdf_PadManager/decls.h"

void sdfPadSetAct(s32 pad, s32 value0, s32 value1) {
    sdfPadCb_t *entry = &sdfPadCb[pad];
    entry->actData = value0 & 0xFF;
    sdfPadCb[pad].actDataBack = value1 & 0xFF;
}
