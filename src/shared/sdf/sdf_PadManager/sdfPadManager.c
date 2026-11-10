#include "shared/sdf/sdf_PadManager/decls.h"

void sdfPadManager(void) {
    s32 padIndex;
    for (padIndex = 0; padIndex != SDF_PAD_ENTRY_COUNT; padIndex++) {
        sdfPadControl(&sdfPadCb[padIndex]);
    }
}
