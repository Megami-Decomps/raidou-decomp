#include "shared/sdf/sdfMotion/decls.h"

sdfMotionNode_t *sdfMotGenMorph(sdfMotionCtrlr_t *aCtrlr,
                                                           s32 aTypeId, s32 aNodeId) {
    sdfMotNodeMorph_t *entry = sdfChipAllocDirect(sizeof(sdfMotNodeMorph_t));

    sdfMotGenMorphInit(entry, aCtrlr, &sdfMotMorphTable, aNodeId);
    return &entry->nodeBase;
}
