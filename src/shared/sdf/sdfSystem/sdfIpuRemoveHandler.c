#include "shared/sdf/sdfSystem/decls.h"

void sdfIpuRemoveHandler(sdfHandlerNode_t *target) {
    sdfHandlerNode_t **link = &sdfDmacHandlerTable;
    sdfHandlerNode_t *node = *link;

    while (node != NULL) {
        if (node == target) {
            *link = node->next;
            switch (node->handlerKind) {
            case 0:
                DisableIntc(node->channel);
                RemoveIntcHandler(node->channel, node->handlerId);
                break;
            case 1:
                RemoveDmacHandler(node->channel, node->handlerId);
                break;
            case 2:
                RemoveSbusIntcHandler(node->channel);
                break;
            }
            sdfFreeAuto(target);
            return;
        }
        /* Non-head handles are followed through their own link. */
        link = &target->next;
        node = *link;
    }
}
