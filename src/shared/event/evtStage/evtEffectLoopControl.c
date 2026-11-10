#include "shared/event/evtStage/decls.h"

void evtEffectLoopControl(dds3Basic_t *eff, s32 sw) {
    evtNodeInner_t *inner = eff->workdata;
    void *node;

    switch (inner->kind) {
    case 1:
    case 7:
    case 8:
        break;
    default:
        return;
    }
    node = inner->node;
    if (sw == 0) {
        dds3EffectSetLoop(node, sw);
    }
}
