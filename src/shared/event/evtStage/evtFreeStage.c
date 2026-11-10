#include "shared/event/evtStage/decls.h"

void evtFreeStage(void)
{
    dds3Basic_t *node;

    node = (dds3Basic_t *)dds3GetControlScene();
    if (node != 0) {
        dds3DeleteScene(node);
    }
}
