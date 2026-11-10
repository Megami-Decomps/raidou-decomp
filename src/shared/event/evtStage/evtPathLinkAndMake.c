#include "shared/event/evtStage/decls.h"

/* Attach object to the node referenced by owner's owned handle. */
s32 evtPathLinkAndMake(dds3Basic_t *path, dds3Basic_t *obj) {
    dds3ObjectBaseControlWorkData_t *ref;
    dds3Basic_t *slotNode;

    if (path == NULL) {
        return 0;
    }
    ref = dds3GetBaseControlWorkData(obj);
    if (ref == NULL) {
        return 0;
    }
    slotNode = ref->pBasic[1];
    if (slotNode == NULL) {
        return 0;
    }
    dds3MoverBasicSetPath(slotNode, path);
    dds3MoverBasicMakePath(slotNode);
    return 1;
}
