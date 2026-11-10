#include "shared/model/mdlEffect/decls.h"

/* Release all items for this slot without losing the link across consecutive matches. */
void mdlStopEffect(mdlObj_t *aHandle, s32 aPlayGroup) {
    dds3ModelEffectCtrlr_t **itemLink = &aHandle->resourceItems;
    dds3ModelEffectCtrlr_t *item = *itemLink;
    while (item != 0) {
        if (item->playGroup == aPlayGroup) {
            dds3ModelEffectCtrlr_t *nextItem = item->next;
            mdlFreeEffect(item);
            *itemLink = nextItem;
            item = nextItem;
        } else {
            itemLink = &item->next;
            item = item->next;
        }
    }
}
