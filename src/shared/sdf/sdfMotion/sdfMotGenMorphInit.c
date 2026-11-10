#include "shared/sdf/sdfMotion/decls.h"

void sdfMotGenMorphInit(sdfMotNodeMorph_t *aBase,
                                 sdfMotionCtrlr_t *aCtrlr, sdfMotionNodeVTBL_t *aVTBL, s32 aNodeId) {
    sdfMotionNodeBaseInit((sdfMotionNode_t *)&aBase->nodeBase,
                            aCtrlr, aVTBL);
    aBase->morphNode = &((sdfModelMorph_t *)aCtrlr->rootNode->morphList->items)[aNodeId];
}
