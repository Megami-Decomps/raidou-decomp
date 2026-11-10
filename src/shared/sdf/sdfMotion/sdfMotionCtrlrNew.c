#include "shared/sdf/sdfMotion/decls.h"

sdfMotionCtrlr_t *sdfMotionCtrlrNew(sdfModelRootNode_t *aModel, sdfMotionPlayBook_t *aBook)
{
    sdfMotionCtrlr_t *ctrlr;
    sdfMotionNodeDef_t *def;
    s32 numOfNode;
    s32 i;

    ctrlr = sdfChipAllocZeroDirect(sizeof(sdfMotionCtrlr_t));
    numOfNode = aBook->numOfNode;
    ctrlr->next = aModel->motion;
    aModel->motion = ctrlr;
    ctrlr->rootNode = aModel;
    ctrlr->playbook = aBook;
    ctrlr->nodeList = sdfItemArrayNew(numOfNode, 4, 8);
    ctrlr->nodeList->count = numOfNode;
    def = ctrlr->playbook->nodeDefList;
    for (i = 0; i < numOfNode; i++) {
        ctrlr->nodeList->items[i] = sdfMotionNodeNew(ctrlr, def->typeId, def->nodeId);
        def++;
    }
    ctrlr->stat = 0;
    ctrlr->speed = 1.0f;
    return ctrlr;
}
