#include "shared/effect/effPCPMisc/decls.h"

void effPCPAgiraoFireDelete(effPCPAgiraoFire_tag *hData) {
    struct effHandle_t **handleCursor;
    u32 slotIndex;

    handleCursor = hData->handles;
    for (slotIndex = 0; slotIndex < 7; slotIndex++) {
        dds3EffectDelete(handleCursor[14]);
        dds3EffectDelete(handleCursor[7]);
        dds3EffectDelete(handleCursor[0]);
        handleCursor++;
    }
    sdfChipFreeDirect(hData);
}
