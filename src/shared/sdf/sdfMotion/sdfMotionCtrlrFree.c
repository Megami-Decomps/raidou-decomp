#include "shared/sdf/sdfMotion/decls.h"

void sdfMotionCtrlrFree(sdfMotionCtrlr_t *aCtrlr)
{
    sdfMotionCtrlr_t **link;
    sdfMotionCtrlr_t *cur;
    sdfMotionNode_t **items;
    s32 count;
    s32 i;

    if (aCtrlr == NULL) {
        return;
    }
    if (aCtrlr->rootNode != NULL) {
        link = &aCtrlr->rootNode->motion;
        for (;;) {
            cur = *link;
            if (cur == NULL) {
                break;
            }
            if (cur == aCtrlr) {
                *link = aCtrlr->next;
                break;
            }
            link = &cur->next;
        }
    }
    count = aCtrlr->nodeList->count;
    items = aCtrlr->nodeList->items;
    for (i = 0; i < count; i++) {
        sdfMotionNodeFree(items[i]);
    }
    sdfItemArrayFree(aCtrlr->nodeList);
    sdfChipFreeDirect(aCtrlr);
}
